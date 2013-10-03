#ifndef ESTATS_H
#define ESTATS_H

#include "../ebasicarray_dec.h"

class estats
{
 public:
  ebasicarray<double> values;

  double mean;
  double stddev;
  double median;
  double p99;
  double p95;
  double p75;
  double p45;
  double p5;
  double p1;

  void reset();
  void add(double value);
  void calc();
};

class ebin
{
 public:
  edoublearray xbin;
  eintarray    ybin;
  int maxcount;
  
  eintarray iv;
  edoublearray values;
  void calc(int nbins=0);
};


class ebin2
{
 public:
  edoublearray xbin;
  edoublearray ybin;
  eintarray    ycount;
  int maxcount;

  double xmin;
  double xmax;
  
  eintarray iv;
  edoublearray xvalues;
  edoublearray yvalues;
  void calc(int nbins=0);
};



#include <iostream>

using namespace std;

ostream& operator<<(ostream& stream, const estats& stats);

#endif

