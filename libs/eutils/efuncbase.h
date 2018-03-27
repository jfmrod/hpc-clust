#ifndef EFUNCBASE_H
#define EFUNCBASE_H

#include "eutils.h"

template <class T>
class earray;
class efunc;
class evar;
typedef earray<evar> evararray;

#include <typeinfo>
using namespace std;

class efuncBase
{
 public:
  efuncBase();
  virtual ~efuncBase();

  int pcount;

  virtual const type_info& getTypeid()=0; 
  virtual evar call(const evararray& args)=0;
  virtual void updateInfo(efunc *f)=0;
};

#endif

