#ifndef EFUNCTYPE_H
#define EFUNCTYPE_H

#include "eutils.h"
#include <typeinfo>

class evar;

#include "evararray.h"
#include "efuncbase.h"
#include "evarclass.h"
#include "evarclassmethod.h"

template <class F>
class efuncType : public efuncBase
{
 public:
  F func;
  efuncType(F func);

  const type_info& getTypeid() { return(typeid(F)); }

  evar call(const evararray &args);
  void updateInfo(efunc *f);
};


//template <class T,class M>
class efuncObjMethod : public efuncBase //,public eclassMethodBase
{
 public:
  evar object;
  eclassMethodBase *method;
//  M pmethod;
//  efuncObjMethod(T& obj,M pmethod);
  efuncObjMethod(const evar& object,eclassMethodBase *method);


  const type_info& getTypeid();

//  evar operator()(evarBase* obj,evararray &args);
  evar call(const evararray &args);
  void updateInfo(efunc *f);
};





#include "evar_dec.h"
#include "efuncalls.h"
#include "efuncinfo.h"

template <class F>
efuncType<F>::efuncType(F _func): func(_func) {}

template <class F>
evar efuncType<F>::call(const evararray &args)
{
  return(efuncall(func,args));
}

template <class F>
void efuncType<F>::updateInfo(efunc *f)
{
  efuncinfo(func,*f);
}


/*
template <class T,class M>
efuncObjMethod<T,M>::efuncObjMethod(T& _obj,M _pmethod): pobject(&_obj),pmethod(_pmethod) {}

template <class T,class M>
evar efuncObjMethod<T,M>::operator()(evarBase* obj,evararray &args)
{
  return(eclassMethodCall(dynamic_cast<evarType<T>*>(obj)->object,pmethod,args));
}

template <class T,class M>
evar efuncObjMethod<T,M>::call(evararray &args)
{
  return(eclassMethodCall(pobject,pmethod,args));
}

template <class T,class M>
void efuncObjMethod<T,M>::updateInfo(efunc *f)
{
  eclassMethodInfo(pmethod,*(eclassMethodBase*)this);
  f->fReturn=eclassMethodBase::fReturn;
  f->fArgs  =eclassMethodBase::fArgs;
}
*/

#endif

