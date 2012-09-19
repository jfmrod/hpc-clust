#include "emodule.h"
#include "logger.h"
#include "eparser.h"

#include <dlfcn.h>

void emodule_load(const estr& module)
{
  void *hModule=dlopen(module._str,RTLD_NOW | RTLD_GLOBAL);
  if (hModule==0x00){ lwarn("module loading error: "+module); return; }
  void (*module_register)(eparser*)=(void (*)(eparser*))dlsym(hModule,"eutils_register");
  if (module_register==0x00) { lwarn("module register function not found: "+module); return; }
  (*module_register)(getParser());
  cout << "successfully registered module: "+module << endl;
}
