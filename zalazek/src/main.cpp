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

using namespace std;

int main(int argc, char **argv)
{
  // sprawdzenie czy liczba parametrów jest poprawna
  // if (argc != 3)
  // {
  //   cerr << "Usage: " << argv[0] << " <config_file.xml> <instructions_file.xml>" << endl;
  //   return 1;
  // }

  //   const char *configFileName = argv[1];
  //   const char *instructionFile = argv[2];
  /* *************************************** */
  // AbstractScene * scene = new Scene;
  // AbstractComChannel * com_channel = new ComChannel;

  void *pLibHnd_Move = dlopen("libInterp4Move.so", RTLD_LAZY);
  AbstractInterp4Command *(*pCreateCmd_Move)(void);
  void *pFun;

  if (!pLibHnd_Move)
  {
    cerr << "!!! Brak biblioteki: Interp4Move.so" << endl;
    return 1;
  }

  pFun = dlsym(pLibHnd_Move, "CreateCmd");
  if (!pFun)
  {
    cerr << "!!! Nie znaleziono funkcji CreateCmd" << endl;
    return 1;
  }
  pCreateCmd_Move = reinterpret_cast<AbstractInterp4Command *(*)()>(pFun);

  AbstractInterp4Command *pCmd = pCreateCmd_Move();

  std::fstream commandFile("commands.cmd");

  if (!commandFile.is_open()){
    cerr << "file opening error" << endl;
    return 1;
  }

  string line;

  while (getline(commandFile, line)){
    istringstream iss(line);
    pCmd->ReadParams(iss);
    cout << "przeczytalem: " << line << endl;
  }


  cout << endl;
  cout << pCmd->GetCmdName() << endl;
  cout << endl;
  pCmd->PrintSyntax();
  cout << endl;
  pCmd->PrintCmd();
  cout << endl;
  pCmd->PrintParams();

  delete pCmd;

  commandFile.close();

  dlclose(pLibHnd_Move);
}
