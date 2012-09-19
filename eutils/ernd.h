#ifndef ERND_H
#define ERND_H

#include "eutils.h"

#ifdef EUTILS_HAVE_LIBGSL
 #include <gsl/gsl_rng.h>
 #include <gsl/gsl_randist.h>
#endif

class ernd
{
 public:
  ernd();
  ~ernd();

  unsigned long seed;

//  void setSeed(int seed);
//  int getSeed();

  double uniform();
#ifdef EUTILS_HAVE_LIBGSL
  gsl_rng *grng;
  double gamma(double k,double t);
  double gaussian(double sigma);
  double exponential(double rate);
  unsigned int geometric(double mean);
#endif
};

extern ernd rnd;

#endif

