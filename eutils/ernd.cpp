#include "ernd.h"

#include <sys/types.h>
#include <time.h>
#include <fcntl.h>

#include "logger.h"

#ifndef _MSC_VER
#include <sys/time.h>
#endif
#ifdef _WIN32
#include <windows.h>
#endif


ernd rnd;

#ifdef EUTILS_HAVE_LIBGSL

double ernd::uniform()
{
  return((double)gsl_rng_uniform(grng));
}

double ernd::gamma(double k,double t)
{
  return((double)gsl_ran_gamma(grng,k,t));
}

double ernd::gaussian(double sigma)
{
  return((double)gsl_ran_gaussian(grng,sigma));
}

double ernd::exponential(double rate)
{
  return((double)gsl_ran_exponential(grng,rate));
}

unsigned int ernd::geometric(double mean)
{
  return(gsl_ran_geometric(grng,mean));
}

ernd::ernd()
{
  grng = gsl_rng_alloc(gsl_rng_ranlxd2);
#ifdef _WIN32
  _LARGE_INTEGER tptime;
  QueryPerformanceCounter(&tptime);
  seed=tptime.LowPart;
  srand(seed);
  gsl_rng_set(grng,seed);
#else
//  timeval tmptv;
//  gettimeofday(&tmptv,0x00);
//  gsl_rng_set(grng,tmptv.tv_usec);

  int fd=open("/dev/random",O_RDONLY);
  ldieif(fd==-1,"Error opening /dev/random");
//  unsigned long seed;
  ldieif(read(fd,&seed,sizeof(unsigned long))==-1,"Error reading from /dev/random");
  close(fd);
  gsl_rng_set(grng,seed);
#endif
}

ernd::~ernd()
{
  gsl_rng_free(grng);
}

#else

  #ifdef WIN32
    #include <windows.h>
  #endif
  #include <stdlib.h>

  
double ernd::uniform()
{
  return((double)rand()/((double)RAND_MAX+1.0));
}

/*
double ernd::gaussian(double sigma)
{
  lerror("eutils compiled without libgsl support, so gaussian is not implemented");
//  return((double)gsl_ran_gaussian(grng,sigma));
  return(1.0);
}
*/
ernd::ernd()
{
#ifdef _WIN32
  _LARGE_INTEGER tptime;
  QueryPerformanceCounter(&tptime);
  seed=tptime.LowPart;
  srand(seed);
#else
//  timeval tmptv;
//  gettimeofday(&tmptv,0);
//  iseed=tmptv.tv_usec;
//  srand(iseed);

  int fd=open("/dev/random",O_RDONLY);
  ldieif(fd==-1,"Error opening /dev/random");
//  unsigned long seed;
  ldieif(read(fd,&seed,sizeof(unsigned long))==-1,"Error reading from /dev/random");
  close(fd);
  srand(seed);
#endif
}

ernd::~ernd()
{
}

#endif


