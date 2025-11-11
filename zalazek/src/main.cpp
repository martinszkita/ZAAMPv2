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

using namespace std;

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
    cerr << "file opening error" << endl;
    return 1;
  }

  string line;
  vector<void *> openLibs;
  vector<std::unique_ptr<AbstractInterp4Command>> commands;
  map<string, shared_ptr<AbstractMobileObj>> mapMobileObjects;

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
      std::cout << "Sukces: udalo sie wysłać na serwer: "<< oss.str() << std::endl;
    }

    mapMobileObjects[cube.name] = make_shared<MobileObj>(cube.name);
  }

  // Dodawanie obiektów do sceny
  scene.SetObjects(mapMobileObjects);

  // wczytywanie poleceń z pliku do vectora
  while (getline(commandFile, line))
  {
    istringstream iss(line);
    string commandName;

    iss >> commandName; // pierwsze slowo w linii to nazwa komendy

    bool commandNameOk = false;

    for (const auto &elem : commandNames)
    {
      if (commandName == elem)
      {
        commandNameOk = true;
        break;
      }
    }

    if (!commandNameOk)
    {
      cerr << "bledna nazwa komendy: "<< commandName<< " w pliku: " << line << " " << commandFileName << endl;
      return 1;
    }

    string libName = "libInterp4" + commandName + ".so"; // przygotowana nazwa biblioteki dynamicznej

    void *pLibHandle = dlopen(libName.c_str(), RTLD_LAZY); // uchwyt do biblioteki

    if (!pLibHandle)
    {
      cerr << "!!! Brak biblioteki: " << libName << endl;
      cerr << dlerror() << endl;
      return 1;
    }

    cout << "Zaladowalem biblioteke: " << libName << endl;

    openLibs.push_back(pLibHandle);
    AbstractInterp4Command *(*pCreateCmd)(void); // wskaznik na funkcje ktora zwraca AbstractInterp4Command * i nie przyjmuje argumentow
    void *pFun = dlsym(pLibHandle, "CreateCmd");

    if (!pFun)
    {
      cerr << "!!! Nie znaleziono funkcji CreateCmd dla polecenia" << commandName << endl;
      return 1;
    }

    pCreateCmd = reinterpret_cast<AbstractInterp4Command *(*)()>(pFun);
    std::unique_ptr<AbstractInterp4Command> pCmd(pCreateCmd());
    pCmd->ReadParams(iss);
    cout << endl;

    cout << "dodalem komende: " << pCmd->GetCmdName() << " do vectora polecen! \n";
    // pCmd->PrintParams();
    cout << endl;

    commands.push_back(std::move(pCmd));
  }

  commandFile.close();


  cout << "poczatek wysylania komend do serwera! \n";
  // wysylanie polecenia do serwera
  for (const auto &cmd : commands)
  {
    if (std::strcmp(cmd->GetCmdName(), "Move") == 0)
    {
      cout << "wykryłem komende move! \n";
      if (auto *move = dynamic_cast<Interp4Move *>(cmd.get()))
      {
        comChannel.SendMoveCommand(
            move->getRobotName(),
            move->getSpeed(),
            move->getDistance());
      }
      else{
        cout << "inna komenda niz move, skipuje.";
        continue;
      }
    }
    else{
      cerr << "error w wysyłaniu polecen do serwera! \n";
      return 1;
    }
  }

  for (const auto &lib : openLibs)
  {
    dlclose(lib);
  }

  ClientSender.CancelCountinueLooping();
  Thread4Sending.join();
  close(Socket4Sending);

  return 0;
}
