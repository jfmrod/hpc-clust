#ifndef EVAR_H
#define EVAR_H

#include "eutils.h"
#include <typeinfo>

#include "evar_dec.h"
#include "ebasicarray_dec.h"
#include "earray_dec.h"
#include "earrayof_dec.h"
#include "ehashmap_dec.h"
#include "efunc_dec.h"
#include "evarclass_dec.h"

#include "ebasicarray.h"
#include "earray.h"
#include "earrayof.h"
#include "ehashmap.h"
#include "efunc.h"
#include "evarclass.h"

#include "evarbase.h"
#include "evartype.h"

#include "logger.h"

#include "estr.h"

//#include "estrarray.h"

template <class T>
evar::evar(T* value)
{
  var=new evarType<T>(value);
  ++var->pcount;
}
template <class T>
evar::evar(const T* value)
{
  var=new evarType<T>((T*)value);
  ++var->pcount;
}
template <class T>
evar::evar(const T& value)
{
  var=new evarType<T>((T&)value);
//  if (getClasses().exists(typeid(T).name()))
//    var=getClasses()[typeid(T).name()].fcopy(var);
  ++var->pcount;
}
template <class T>
evar::evar(T& value)
{
  var=new evarType<T>((T&)value);
//  if (getClasses().exists(typeid(T).name()))
//    var=getClasses()[typeid(T).name()].fcopy(var);
  ++var->pcount;
}




template <class T>
T& evar::get() const
{
  if (var==0x00) { lerror("empty var"); throw("trying to get from empty envar"); }

  if (var->getTypeid() == typeid(T))
    return(*dynamic_cast<evarType<T>* >(var)->object);

  lwarn("unable to get variable");
  throw("unable to get variable");
}

template <class T>
T& evar::getarg(void (*pfunc)(T)) const
{
  if (var==0x00) { lwarn("empty var"); throw("trying to get from empty envar"); }
  if (typeid(T)==var->getTypeid())
    return(*dynamic_cast< evarType<T>* >(var)->object);
  lwarn("unable to get variable");
  throw("unable to get variable");
}
template <class T>
T& evar::getarg(void (*pfunc)(const T&)) const
{
  if (var==0x00) { lwarn("empty var"); throw("trying to get from empty envar"); }
  if (typeid(T)==var->getTypeid())
    return(*dynamic_cast< evarType<T>* >(var)->object);
  lwarn("unable to get variable");
  throw("unable to get variable");
}
template <class T>
T& evar::getarg(void (*pfunc)(T&)) const
{
  if (var==0x00) { lwarn("empty var"); throw("trying to get from empty envar"); }
  if (typeid(T)==var->getTypeid())
    return(*dynamic_cast< evarType<T>* >(var)->object);
  lwarn("unable to get variable");
  throw("unable to get variable");
}



#include "evarclassconverter.h"
#include "evarclass.h"

template <class T>
void serial(const T& value,estr& data)
{
  evar var(value);
//  var.var->cleanup=false;
  if (!getClasses().exists(var.getClass())) { lerror(estr("class not registered: ")+var.getClass()); return; }

  getClasses().values(var.getClass()).fserial(var.var,data);
}

template <class T>
int unserial(T& value,const estr& data,int i)
{
  evar var(value);
//  var.var->cleanup=false;
  if (!getClasses().exists(var.getClass())) { lerror(estr("class not registered: ")+var.getClass()); return(i); }
  return(getClasses().values(var.getClass()).funserial(var.var,data,i));
}




/*
template <class T>
bool evar::isType() const
{
  if (var==0x00) { lwarn("empty var"); throw("trying to get from empty envar"); }
  if (typeid(T)==var->getTypeid())
    return(true);
  return(false);
}
*/


/*
inline void evar::set(const evar& value) { if (!isNull()) var->set(value); }
inline evar evar::call(const estr& method,evararray& args)
    { if (!isNull()) return(var->call(method,args)); else lerror("calling method in unitialized evar"); return(evar()); }
inline evar evar::get(const estr& property) const
    { if (!isNull()) return(var->get(property)); else lerror("calling get in unitialized evar"); return(evar()); }
inline void evar::set(const estr& property,const evar& value)
    { if (!isNull()) var->set(property,value); else lerror("calling set in unitialized evar"); }


template <class T>
void evar::setref(T& value)
{
  clear();
  var=new evarType<T>(value);
  if (var)
    ++var->pcount;
}
*/
/*
template <class T>
evar& evar::operator=(T* value)
{
  var = new evarType<T>(*value);
  ++var->pcount;
  return(*this);
}

template <class T>
evar& evar::operator=(const T& value)
{
  if (var && var->getClassTypeid() == typeid(T)){
    ((evarType<T>*)var)->get() = value;
    return(*this);
  }
  clearVar();
  var = new evarType<T>(*new T(value));
  ++var->pcount;
  return(*this);
}
*/

#endif

