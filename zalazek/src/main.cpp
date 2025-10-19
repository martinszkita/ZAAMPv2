#include <iostream>
#include <dlfcn.h>
#include <cassert>
#include <vector>
#include "AbstractInterp4Command.hh"
#include "gtsDebug.hh"

using namespace std;

int main(int argc, char **argv)
{
  const char * lib_move_name = "zalazek/libs/libInterp4Move.so";

  void * handle = dlopen(lib_move_name, RTLD_LAZY);

  void (*pFun)() = (void (*)())dlsym(handle, "PrintSyntax");

  pFun();
}
