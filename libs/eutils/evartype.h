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
  evarType(T* val): object(val) {cleanup=true;}
  ~evarType() { if (object && cleanup) delete object; }
  
  inline const type_info& getTypeid() {return(typeid(T));}
  inline estr getClass() {return(typeid(T).name());}
};

#endif

