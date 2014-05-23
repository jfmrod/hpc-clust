#ifndef EVARBASE_H
#define EVARBASE_H

#include "eutils.h"

class estr;
class evar;
template <class T>
class earray;
typedef earray<evar> evararray;


class evarBase
{
 public:
  int pcount;
  bool cleanup;

  evarBase():pcount(0) {}
  virtual ~evarBase() {}

  virtual const type_info& getTypeid()=0; 
  virtual const char* getClass()=0;
};

#endif

