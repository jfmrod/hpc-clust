#ifndef ETEST_H
#define ETEST_H

#include "estr.h"
#include "efunc.h"

class etestItem
{
 public:
  efunc func;
  estr name;
  estr file;
  etestItem(const efunc& func,const estr& name,const estr& file);
};

class etest
{
 public:
  earray<etestItem> tests;
  void run();
};

etest& getTest();

#define etestAdd(func) etestItem testItem ## func(func,#func,__FILE__)

#endif

