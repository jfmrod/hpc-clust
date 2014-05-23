#ifndef EVARCLASSCONVERTER_H
#define EVARCLASSCONVERTER_H

#include "eutils.h"

class evar;
class evarBase;

class eclassConverterBase
{
 public:
  virtual evar operator()(evarBase* from)=0;
};

#include <typeinfo>
using namespace std;

class estr;

template <class T,size_t (*)(const estr&)>
class estrhashof;

void addConverter(const type_info& to,const type_info& from,eclassConverterBase* converter);
eclassConverterBase* getConverter(const type_info& to,const type_info& from);


///////////////////////////////////////////////////////////////////////////////
//
//  Class Converters
//
template <class T1,class T2>
class eclassConverterCast : public eclassConverterBase
{
 public:
  evar operator()(evarBase* from);
};

template <class T1,class T2>
class eclassConverterDynamicCast : public eclassConverterBase
{
 public:
  evar operator()(evarBase* from);
};


template <class T1,class T2>
class eclassConverterPointer : public eclassConverterBase
{
 public:
  evar operator()(evarBase* from);
};
template <class T1,class T2>
class eclassConverterObject : public eclassConverterBase
{
 public:
  evar operator()(evarBase* from);
};





template <class T1,class T2>
class eclassConverterUserFunc : public eclassConverterBase
{
 public:
  T1 (*userFunc)(T2);
  eclassConverterUserFunc(T1 (*userFunc)(T2));
  evar operator()(evarBase* from);
};


template <class T1,class T2>
class eclassConverterMethod : public eclassConverterBase
{
 public:
  T1 (T2::*method)() const;
  eclassConverterMethod(T1 (T2::*method)() const);
  evar operator()(evarBase* from);
};


#define epregisterClassConverterCast(t1,t2)  addConverter(typeid(t1),typeid(t2),new eclassConverterCast<t1,t2>)
#define epregisterClassConverterCast2(t1,t2) { epregisterClassConverterCast(t1,t2); epregisterClassConverterCast(t2,t1); }

#define epregisterClassConverterDynamicCast(t1,t2)  addConverter(typeid(t1),typeid(t2),new eclassConverterDynamicCast<t1,t2>)

#define epregisterClassConverterPointer(t1,t2) addConverter(typeid(t1),typeid(t2),new eclassConverterPointer<t1,t2>)
#define epregisterClassConverterObject(t1,t2) addConverter(typeid(t1),typeid(t2),new eclassConverterObject<t1,t2>)

#define epregisterClassConverterMethod(t1,t2,m) addConverter(typeid(t1),typeid(t2),new eclassConverterMethod<t1,t2>(&t2::m))


#include "evar.h"


template <class T1,class T2>
evar eclassConverterCast<T1,T2>::operator()(evarBase* from)
{
  return(new T1( (T1)*dynamic_cast<evarType<T2>*>(from)->object ));
}

template <class T1,class T2>
evar eclassConverterDynamicCast<T1,T2>::operator()(evarBase* from)
{
  return( *dynamic_cast<T1*>(dynamic_cast<evarType<T2>*>(from)->object) );
}

template <class T1,class T2>
evar eclassConverterPointer<T1,T2>::operator()(evarBase* from)
{
  T1 tmp;
  tmp = ((T1)dynamic_cast<evarType<T2>*>(from)->object);
  return(&tmp);
}

template <class T1,class T2>
evar eclassConverterObject<T1,T2>::operator()(evarBase* from)
{
  return( *dynamic_cast<evarType<T2>*>(from)->object );
}








template <class T1,class T2>
eclassConverterUserFunc<T1,T2>::eclassConverterUserFunc(T1 (*_userFunc)(T2)): userFunc(_userFunc) {}

template <class T1,class T2>
evar eclassConverterUserFunc<T1,T2>::operator()(evarBase* from)
{
  T1* resobj;
  resobj=new T1( userFunc( *dynamic_cast<evarType<T2>*>(from)->object ) );
  return(resobj);
}


template <class T1,class T2>
eclassConverterMethod<T1,T2>::eclassConverterMethod(T1 (T2::*_method)() const): method(_method) {}

template <class T1,class T2>
evar eclassConverterMethod<T1,T2>::operator()(evarBase* from)
{
  return( (dynamic_cast<evarType<T2>*>(from)->object->*method)() );
}

#endif

