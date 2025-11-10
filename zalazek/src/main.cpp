#include <iostream>
#include <dlfcn.h>
#include <cassert>
#include "AbstractInterp4Command.hh"
#include "AbstractComChannel.hh"
#include "Scene.hh"
#include "ComChannel.hh"
#include <fstream>
#include <istream>
#include <sstream>
#include "CommandNames.hh"
#include "xmlinterp.hh"

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
      }
    }

    if (!commandNameOk)
    {
      cerr << "bledna nazwa komendy w pliku: "<< line <<" " << commandFileName << endl;
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

    cout << "Zaladowalem biblioteke: " << libName;

    openLibs.push_back(pLibHandle);
    AbstractInterp4Command *(*pCreateCmd)(void); // wskaznik na funkcje ktora zwraca AbstractInterp4Command * i nie przyjmuje argumentow
    void *pFun = dlsym(pLibHandle, "CreateCmd");

    if (!pFun)
    {
      cerr << "!!! Nie znaleziono funkcji CreateCmd dla polecenia" << commandName << endl;
      return 1;
    }

    pCreateCmd = reinterpret_cast<AbstractInterp4Command *(*)()>(pFun);
    AbstractInterp4Command *pCmd = pCreateCmd();

    pCmd->ReadParams(iss);
    cout << endl;
    pCmd->PrintParams();

    cout << endl;

    delete pCmd;
  }

  commandFile.close();

  for (const auto &lib : openLibs)
  {
    dlclose(lib);
  }
}
