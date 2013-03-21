#include "etimer.h"
#include "logger.h"

#ifdef __APPLE__
#include <mach/clock.h>
#include <mach/mach.h>

clock_serv_t cclock=0x00;

long egettimestamp()
{
  if (cclock==0x00)
    host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &cclock);
  mach_timespec_t mts;
  clock_get_time(cclock, &mts);
//  mach_port_deallocate(mach_task_self(), cclock);
  return((long)mts.tv_sec*1000l + (long)mts.tv_nsec/1000000l);
}
#else
#include <time.h>

long egettimestamp()
{
  struct timespec tp;
  lerrorif(clock_gettime(CLOCK_MONOTONIC, &tp)!=0,"getting clock time");
  return((long)tp.tv_sec*1000l + (long)tp.tv_nsec/1000000l);
}
#endif


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


