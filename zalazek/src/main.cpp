#include <iostream>
#include <dlfcn.h>
#include <cassert>
#include "AbstractInterp4Command.hh"
#include "AbstractComChannel.hh"
#include "AbstractMobileObj.hh"
#include "Scene.hh"
#include "ComChannel.hh"
#include <fstream>
#include <istream>
#include <sstream>
#include "CommandNames.hh"
#include "xmlinterp.hh"
#include "Sender.hh"
#include <thread>
#include <memory>
#include "MobileObj.hh"
#include "Interp4Move.hh"
#include "Interp4Rotate.hh"
#include "Interp4Set.hh"
#include "Interp4Pause.hh"
#include <optional>
#include <vector>
#include <filesystem>
#include <cstdlib>

#define INTERP4(commandName) Interp4#commandName

using namespace std;

namespace {

std::string QuotePath(const std::filesystem::path &path)
{
  std::ostringstream oss;
  oss << '"' << path.string() << '"';
  return oss.str();
}

// tworzy przetworzony *.pp przez preprocesor plik z komendami w katalogu nieprzetworzonego pliku
bool PreprocessCommandsFile(const std::string &sourceFile,
                            std::filesystem::path &outputFile)
{
  outputFile =  std::filesystem::path(sourceFile).parent_path()/
    (std::filesystem::path(sourceFile).filename().string() + ".pp");

  std::ostringstream command;
  command << "cpp -P -nostdinc -undef "
          << QuotePath(sourceFile) << ' '
          << QuotePath(outputFile);

  const int result = std::system(command.str().c_str());

  if (result != 0)
  {
    std::cerr << "Nie udało się przetworzyć pliku poleceń przez preprocesor." << std::endl;
    return false;
  }

  return true;
}

}

int main(int argc, char **argv)
{
  // sprawdzenie czy liczba parametrów jest poprawna
  if (argc < 2 || argc > 3)
  {
    cerr << "Usage: " << argv[0] << " <config_file.xml> [commands_file]" << endl;
    return 1;
  }
  const char *configFileName = argv[1];
  const std::string commandFileName = argc == 3 ? argv[2] : "commands";

  std::filesystem::path processedCommands;

  if (!PreprocessCommandsFile(commandFileName, processedCommands))
  {
    return 1;
  }

  Configuration config = XMLInterp4Config::redConfigurationFromXML(configFileName);

  ifstream commandFile;
  commandFile.open(processedCommands);

  if (!commandFile.is_open())
  {
    cerr << "command file opening error" << endl;
    return 1;
  }


  map<string, void *> mLoadedLibraries;
  map<string, AbstractInterp4Command *(*)()> mInterps;
  map<string, shared_ptr<AbstractMobileObj>> mMobileObjects;

  // Inicjalizacja połączenia z serwerem
  Scene scene;
  int Socket4Sending;

  if (!OpenConnection(Socket4Sending))
    return 1;

  Sender ClientSender(Socket4Sending, &scene);
  std::thread Thread4Sending(Fun_CommunicationThread, &ClientSender);
  ComChannel comChannel;

  if (!comChannel.Init(Socket4Sending))
  {
    cerr << "Błąd przy inicjalizacji comChannel!" << endl;
    return 1;
  }

  // czyscimy scene przed wyslaniem
  if (Send(Socket4Sending, "Clear \n") < 0)
  {
    std::cerr << "Error: Nie udalo sie wyslac na serwer!\n";
  }

  else
  {
    std::cout << "Scena została wyczyszczona! " << std::endl;
  }

  // Wysyłanie poleceń z config.xml do serwera
  for (const auto &cube : config.cubes)
  {
    ostringstream oss;
    oss << "AddObj Name=" << cube.name
        << " RGB=" << stringToVecString(cube.color)
        << " Scale=" << stringToVecString(cube.scale)
        << " Shift=" << stringToVecString(cube.shift)
        << " RotXYZ_deg=" << stringToVecString(cube.rotXYZ)
        << " Trans_m=" << stringToVecString(cube.trans_m) << "\n";

    if (Send(Socket4Sending, oss.str().c_str()) < 0)
    {
      std::cerr << "Error: Nie udalo sie wyslac na serwer!\n";
    }
    else
    {
      std::cout << "Sukces: udalo sie wysłać na serwer: " << oss.str() << std::endl;
    }

    mMobileObjects[cube.name] = make_shared<MobileObj>(cube.name);
  }

  // Dodawanie obiektów do sceny
  scene.SetObjects(mMobileObjects);

  for (const auto &plugin : config.plugins)
  {
    if (mLoadedLibraries.find(plugin) != mLoadedLibraries.end())
    {
      cout << "Biblioteka " << plugin << " została już załadowana, pomijam ponowne ładowanie." << endl;
      continue;
    }

    void *pluginHangle = dlopen(plugin.c_str(), RTLD_LAZY);

    if (!pluginHangle)
    {
      cerr << "!!! Brak biblioteki: " << plugin << endl;
      cerr << dlerror() << endl;
      return 1;
    }

    cout << "Zaladowalem biblioteke: " << plugin << endl;

    // ladowanie bibliotek z config.xml i dodawanie do mapy
    mLoadedLibraries.insert({plugin, pluginHangle});

    // tworzenie prototypow interpów
    AbstractInterp4Command *(*pCreateCmd)(void);

    pCreateCmd = (AbstractInterp4Command * (*)()) dlsym(pluginHangle, "CreateCmd");

    if (!pCreateCmd)
    {
      std::cerr << "Brak funkcji CreateCmd w pluginie!" << std::endl;
      return 1;
    }

    AbstractInterp4Command *cmd = pCreateCmd();

    if (!cmd)
    {
      std::cerr << "CreateCmd zwróciło NULL" << std::endl;
      return 1;
    }

    string cmdName = cmd->GetCmdName();
    mInterps.insert({cmdName, pCreateCmd});
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(3000)));
  cout << "poczatek wysylania komend do serwera! \n";

  // wczytywanie poleceń z pliku i natychmiastowe ich wykonywanie
  string line;

  while (getline(commandFile, line))
  {
    if (line.empty())
    {
      continue;
    }

    istringstream iss(line);
    string commandName;

    iss >> commandName;

    if (commandName.empty())
    {
      continue;
    }

    auto it = mInterps.find(commandName);
    if (it == mInterps.end())
    {
      cerr << "nie znaleziono interpretera dla polecenia : " << commandName << endl;
      continue;
    }

    std::unique_ptr<AbstractInterp4Command> interp(it->second());

    if (!interp)
    {
      cerr << "nie udało się utworzyć interpretera polecenia: " << commandName << endl;
      continue;
    }

    if (!interp->ReadParams(iss))
    {
      cerr << "Błąd wczytywania parametrów dla polecenia: " << commandName << endl;
      continue;
    }

    std::string name = interp->GetCmdName();
    std::string robotName;

    if (auto move = dynamic_cast<Interp4Move *>(interp.get()))
    {
      robotName = move->getRobotName();
    }
    else if (auto rotate = dynamic_cast<Interp4Rotate *>(interp.get()))
    {
      robotName = rotate->GetRobotName();
    }
    else if (auto set = dynamic_cast<Interp4Set *>(interp.get()))
    {
      robotName = set->GetRobotName();
    }
    else if (auto pause = dynamic_cast<Interp4Pause *>(interp.get()))
    {
      robotName = pause->GetRobotName();
    }

    if (!interp->ExecCmd(scene, robotName.c_str(), comChannel))
    {
      cerr << "Wykonanie polecenia " << name << " nie powiodło się" << endl;
    }
  }

  commandFile.close();
  ClientSender.CancelCountinueLooping();
  Thread4Sending.join();
  close(Socket4Sending);

  return 0;
}
