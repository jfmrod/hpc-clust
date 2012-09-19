#ifndef EUTILS_H
#define EUTILS_H


#ifndef _WIN32
 #include "eutils-config.h"
#else
 #ifndef WINVER                // Allow use of features specific to Windows 95 and Windows NT 4 or later.
  #define WINVER 0x0501        // Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
 #endif

 #ifndef _WIN32_WINNT        // Allow use of features specific to Windows NT 4 or later.
  #define _WIN32_WINNT 0x0501        // Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
 #endif                       

 #ifndef _WIN32_WINDOWS        // Allow use of features specific to Windows 98 or later.
  #define _WIN32_WINDOWS 0x0501 // Change this to the appropriate value to target Windows Me or later.
 #endif

 #include <windows.h>

 #include "eutils-win32-config.h"
#endif

#endif

