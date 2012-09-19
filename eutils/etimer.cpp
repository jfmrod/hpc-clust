#include "etimer.h"


etimer::etimer()
{
#ifdef _WIN32
  _LARGE_INTEGER fTime;
  QueryPerformanceFrequency(&fTime);
  pifrq = (double)1.0f/(double)fTime.LowPart;
#endif
}

void etimer::reset()
{
#ifdef _WIN32
  QueryPerformanceCounter(&counter);  
#else
  gettimeofday(&counter,0);
#endif
}

double etimer::check()
{
#ifdef _WIN32
  _LARGE_INTEGER tptime;
  QueryPerformanceCounter(&tptime);
  return((tptime.LowPart - counter.LowPart)*pifrq*1.0E3);
#else
  timeval tmptv;
  gettimeofday(&tmptv,0);
  return((tmptv.tv_sec-counter.tv_sec)*1.0E3+(tmptv.tv_usec-counter.tv_usec)*1.0E-3);
#endif
}

double etimer::lap()
{
#ifdef _WIN32
  _LARGE_INTEGER tptime;
  double  tmp;
  QueryPerformanceCounter(&tptime);
  tmp = (tptime.LowPart - counter.LowPart)*pifrq*1.0E3;
  counter = tptime;
  return(tmp);
#else
  timeval tmptv;
  double tmp;
  gettimeofday(&tmptv,0);
  tmp = (tmptv.tv_sec-counter.tv_sec)*1.0E3+(tmptv.tv_usec-counter.tv_usec)*1.0E-3;
  counter = tmptv;
  return(tmp);
#endif
}


