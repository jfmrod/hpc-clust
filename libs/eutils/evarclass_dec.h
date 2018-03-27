#ifndef ECLASS_DEC_H
#define ECLASS_DEC_H

#include "eutils.h"

///////////////////////////////////////////////////////////////////////////////
//
//  Classes
//


class estr;
class evar;
template <class T>
class earray;
typedef earray<evar> evararray;



class eclassPropertyBase;
class eclassMethodBase;

#include "efunc_dec.h"
#include "evartype.h"
#include "estrhashof.h"

#include <iostream>
using namespace std;

class eclassBase
{
 public:
  estrhashof<eclassPropertyBase> properties;
  estrhashof<ebasicarray<eclassMethodBase*> > methods;
  estrhashof<eclassBase*> parents;

  earray<efunc> create;

  virtual estr getName()=0;
  virtual const type_info& getTypeid()=0;

  virtual bool hasCout() { return(false); }

  virtual evarBase* fnew() { return(0x00); }
  virtual int funserial(evarBase* var,const estr& data,int i) { return(i); }
  virtual void fserial(evarBase* var,estr& data) {}
  virtual ostream& fcout(ostream& stream,evarBase* var) { stream<<"<"<<getName()<<">"; return(stream); }
  virtual evarBase* fcopy(evarBase* var) { return(0x00); }
  virtual void fassign(evarBase* to,evarBase* from) {}
};

template <class T>
class eclass : public eclassBase
{
 public:
  eclass(): pfcout(0x00),pfcopy(0x00),pfassign(0x00),pfserial(0x00),pfunserial(0x00) {}

  const type_info& getTypeid(){ return(typeid(T)); }
  estr getName(){ return(typeid(T).name()); }

  ostream& (*pfcout)(ostream&,const T&);
  T* (*pfcopy)(const T&);
  void (*pfassign)(T&,const T&);

  void (*pfserial)(const T&,estr&);
  int (*pfunserial)(T&,const estr&,int i);

  virtual bool hasCout() { if (pfcout) return(true); return(false); }

  virtual evarBase* fcopy(evarBase* var){ if (pfcopy) { return(new evarType<T>((*pfcopy)(*dynamic_cast<evarType<T>*>(var)->object))); } return(0x00); }
  virtual void fassign(evarBase* to,evarBase* from){ if (pfassign) (*pfassign)(*dynamic_cast<evarType<T>*>(to)->object,*dynamic_cast<evarType<T>*>(from)->object); }
  virtual ostream& fcout(ostream& stream,evarBase* var){ if (pfcout) { return((*pfcout)(stream,*dynamic_cast<evarType<T>*>(var)->object)); } stream << "<" << getName() << ">"; return(stream); }

  virtual int funserial(evarBase* var,const estr& data,int i) { if (pfunserial) return((*pfunserial)(*dynamic_cast<evarType<T>*>(var)->object,data,i)); return(i); }
  virtual void fserial(evarBase* var,estr& data) { if (pfserial) (*pfserial)(*dynamic_cast<evarType<T>*>(var)->object,data); }
};

estrhashof<eclassBase>& getClasses();
estrhashof<eclassBase*>& getClassNames();



#define GETCLASS(c,T) { if (!getClasses().exists(typeid(T).name())){                           \
                          c = new eclass<T>;                                                        \
                          getClasses().addref(typeid(T).name(),c);                           \
                        }else{                                                                       \
                          c = dynamic_cast< eclass<T>* >( &getClasses().values(typeid(T).name()) ); \
                        } }

#define epregisterFuncMethod(primitive,func,name)  epregisterFuncMethod2<primitive>(func,name)


#define epregisterClassMethod(c,m)               epregisterClassMethod3<c>(&c::m,#m)
#define epregisterClassMethodA(c,m,rm)           epregisterClassMethod3<c>(&c::m,rm)
#define epregisterClassMethodD(c,m,defargs)      epregisterClassMethod3<c>(&c::m,#m,defargs)
#define epregisterClassMethodI(c,m,i)            epregisterClassMethod3<c>(&c::m,#m,evararray(),i)
#define epregisterClassMethodDI(c,m,defargs,i)   epregisterClassMethod3<c>(&c::m,#m,defargs,i)
#define epregisterClassMethod2(c,m,r,a)          epregisterClassMethod3<c>((r(c::*)a)&c::m,#m)
#define epregisterClassMethod2A(c,m,rm,r,a)          epregisterClassMethod3<c>((r(c::*)a)&c::m,rm)
#define epregisterClassMethod2D(c,m,r,a,defargs) epregisterClassMethod3<c>((r(c::*)a)&c::m,#m,defargs)
#define epregisterClassMethod4(c,m,r,a,n)          epregisterClassMethod3<c>((r(c::*)a)&c::m,n)

#define epregisterClassProperty(c,p)       epregisterClassProperty2<c>(&c::p,#p)
#define epregisterClassPropertyI(c,p,i)    epregisterClassProperty2<c>(&c::p,#p,i)

#define epregisterClassInheritance(c,pc) { epregisterClassInheritance2<c,pc>(); epregisterClassConverterDynamicCast(pc,c); }

#define epregisterClassCout(c) epregisterClassCout2<c>(&eclassCout<c>)

#define epregisterClass(c) { epregisterClassConstructor(c,()); epregisterClassAssign(c); epregisterClassCopy(c); }

#define epregisterClassAssign(c) epregisterClassAssign2<c>(&eclassAssign<c>)

#define epregisterClassCopy(c) epregisterClassCopy2<c>(&eclassCopy<c>)
#define epregisterClassSerialMethod(c) epregisterClassSerial2<c>(&eclassSerialMethod<c>)
#define epregisterClassSerialType(c) epregisterClassSerial2<c>(&eclassSerialType<c>)
#define epregisterClassUnserialMethod(c) epregisterClassUnserial2<c>(&eclassUnserialMethod<c>)
#define epregisterClassUnserialType(c) epregisterClassUnserial2<c>(&eclassUnserialType<c>)
#define epregisterClassSerializeType(c) { epregisterClassSerialType(c); epregisterClassUnserialType(c); }
#define epregisterClassSerializeMethod(c) { epregisterClassSerialMethod(c); epregisterClassUnserialMethod(c); }


#endif

