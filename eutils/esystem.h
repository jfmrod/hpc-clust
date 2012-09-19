#ifndef ESYSTEM_H
#define ESYSTEM_H

#include "eutils.h"

#ifdef _WIN32
 #include "esystem_win32.h"
#elif defined(__APPLE__)
 #include "esystem_osx.h"
#else
 #include "esystem_linux.h"
#endif

#endif

