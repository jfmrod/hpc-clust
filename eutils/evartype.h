#ifndef EVARTYPE_H
#define EVARTYPE_H

#include "eutils.h"
#include "evarbase.h"

///////////////////////////////////////////////////////////////////////////////
//
//  Objects
//

#include <typeinfo>

#include "estr.h"

template <class T>
class evarType : public evarBase
{
 public:
  T* object;
  evarType(): object(0x00) {cleanup=false;}
  evarType(T& val): object(&val) {cleanup=false;}
  evarType(T* val): object(val) {cleanup=false;}
  virtual ~evarType() {}
  
  inline const type_info& getTypeid() {return(typeid(T));}
  inline const char *getClass() {return(typeid(T).name());}
};

template <class T>
class evarTypeClean : public evarType<T>
{
 public:
  evarTypeClean() {}
  evarTypeClean(T& val): evarType<T>(val) {}
  evarTypeClean(T* val): evarType<T>(val) { this->cleanup=true; }
  virtual ~evarTypeClean() { if (this->object && this->cleanup) delete this->object; }
};

#endif

