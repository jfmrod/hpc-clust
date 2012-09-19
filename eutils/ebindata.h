#ifndef __EBINDATA__
#define __EBINDATA__

#include "eutils-config.h"

class ebindata
{
  void addbounds(double x);
  void addadjmin(double x);
  void addadjmax(double x);
  void addadjminmax(double x);
 public:
  eintarray buffer;
  int* bins;
  int  nbins;  

  bool adjmax;
  bool adjmin;

  double max;
  double min;
  double binwidth;
  double ibinwidth;

  int count;
  
  ebindata();
  ~ebindata();

  void init(int nbins);
  void init(double min,double max,int nbins=0);

  void clear();

  void (*add)(double x);
  inline void fast_add(double x){ ++bins[ (x-min)*ibinwidth ]; }
};

ebindata::ebindata():bins(0x00),adjmax(true),adjmin(true) {}
ebindata::~ebindata(){
  if (bins) delete[] bins;
  bins=0x00;
}

void ebindata::init(int _nbins)
{
  nbins=_nbins;
  bins=new int[nbins+1];
}

void ebindata::init(double _min,double _max,int _nbins)
{
  adjmax=false; adjmin=false;
  min=_min; max=_max;
  binwidth=(double)(_max-_min)/(double)_nbins;
  ibinwidth=(double)_nbins/(double)(_max-_min);
  create(_nbins); 
}

void ebindata::addadjmin(double x)
{

}

void ebindata::addadjmax(double x)
{

}

void ebindata::addadjminmax(double x)
{

}

void ebindata::addbounds(double x)
{

}

void ebindata::add(double x)
{
  if (!adjmin && !adjmax){

  }

  if (adjmax){
    if (x > max) max=x;
    if (!adjmin && x > min) { buffer.add(x); return; }
  }
  if (adjmin){
    if (x < min) min=x;
    if (!adjmax && x < max)  { buffer.add(x); return; }
  }

}

#endif /* __EBINDATA__ */


