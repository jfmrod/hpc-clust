#ifndef ETIMER_H
#define ETIMER_H

#include "eutils.h"

#ifndef _MSC_VER
 #include <sys/time.h>
#endif

#ifdef _WIN32
 #include <windows.h>
#endif

#undef check

class etimer
{
 private:
#ifdef _WIN32
  double pifrq;
  _LARGE_INTEGER counter;
#else
  timeval counter;
#endif
 public:
  etimer();

  void reset();
  double check();
  double lap();
};


long egettimestamp();

#endif

