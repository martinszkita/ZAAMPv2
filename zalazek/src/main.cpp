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

#define INTERP4(commandName) Interp4#commandName

using namespace std;

// zrobić kolekcję wtyczek (MAP)

int main(int argc, char **argv)
{
  // sprawdzenie czy liczba parametrów jest poprawna
  if (argc != 3)
  {
    cerr << "Usage: " << argv[0] << " <config_file.xml> <instructions_file.xml>" << endl;
    return 1;
  }
  const char *configFileName = argv[1];
  const char *commandFileName = argv[2];

  Configuration config = XMLInterp4Config::redConfigurationFromXML(configFileName);

  ifstream commandFile;
  commandFile.open(commandFileName);

  if (!commandFile.is_open())
  {
    cerr << "command file opening error" << endl;
    return 1;
  }
  map<string, void *> loadedLibraries;
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
    void *pluginHangle = dlopen(plugin.c_str(), RTLD_LAZY);

    if (!pluginHangle)
    {
      cerr << "!!! Brak biblioteki: " << plugin << endl;
      cerr << dlerror() << endl;
      return 1;
    }

    cout << "Zaladowalem biblioteke: " << plugin << endl;

    // ladowanie bibliotek z comfig.xml i dodawanie do mapy
    loadedLibraries.insert({plugin, pluginHangle});

    // tworzenie prototypow interpów
    AbstractInterp4Command *(*pCreateCmd)(void);

    pCreateCmd = (AbstractInterp4Command * (*)()) dlsym(pluginHangle, "createCmd");

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

  // wczytywanie poleceń z pliku do vectora
  string line;

  while (getline(commandFile, line))
  {
    istringstream iss(line);
    string commandName;

    iss >> commandName;

    auto it = mInterps.find(commandName);
    if (it == mInterps.end()){
      cerr << "nie znaleziono interpretera dla polecenia : " << commandName << endl;
      continue;
    }

    AbstractInterp4Command * interp = it->second();



    // // wskaznik na funkcje ktora zwraca AbstractInterp4Command * i nie przyjmuje argumentow
    // pCreateCmd = reinterpret_cast<AbstractInterp4Command *(*)()>(pFun);
    // std::unique_ptr<AbstractInterp4Command> pCmd(pCreateCmd());

    // pCmd->ReadParams(iss);
    // cout << endl;

    // cout << "dostalem komende: " << pCmd->GetCmdName() << "\n";
    // // pCmd->PrintParams();
    // cout << endl;

    // // exec
    // // delete cmd
  }

  commandFile.close();

  std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(3000)));
  cout << "poczatek wysylania komend do serwera! \n";

  // wysylanie polecenia do serwera
  for (const auto &cmd : commands)
  {
    std::string name = cmd->GetCmdName();

    if (std::strcmp(name.c_str(), "Move") == 0)
    {
      Interp4Move *move = dynamic_cast<Interp4Move *>(cmd.get());
      std::string robotName = move->getRobotName();
      move->ExecCmd(scene, robotName.c_str(), comChannel);
      delete move;
    }
    else if (std::strcmp(name.c_str(), "Rotate") == 0)
    {
      Interp4Rotate *rotate = dynamic_cast<Interp4Rotate *>(cmd.get());
      std::string robotName = rotate->GetRobotName();
      rotate->ExecCmd(scene, robotName.c_str(), comChannel);
      delete rotate;
    }
  }

  ClientSender.CancelCountinueLooping();
  Thread4Sending.join();
  close(Socket4Sending);

  return 0;
}
