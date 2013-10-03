#ifndef EFUNC_H
#define EFUNC_H

#include "efunc_dec.h"
#include "efunctype.h"
#include "evarclassmethod.h"

template <class T,class M>
efunc::efunc(T& obj,M pmethod,const evararray& args): fReturn(0x00),defargs(args)
{
  func = new efuncObjMethod(obj,new eclassMethod<T,M>(pmethod,"",args));
  ++func->pcount;

  updateInfo();
}

template <class F>
efunc::efunc(F _func,const evararray& args): defargs(args),fReturn(0x00)
{
  func = new efuncType<F>(_func);
  ++func->pcount; 

  updateInfo();
}

template <class F>
efunc& efunc::operator=(F _func)
{
  clear();
  func=new efuncType<F>(_func);
  ++func->pcount;

  updateInfo();
  return(*this);
}

template <class R,class A1>
R efunc::call(A1 a1)
{
  if (func==0x00) return(R());

  if (typeid(R(*)(A1))==func->getTypeid())
    return((*static_cast< efuncType<R(*)(A1)>* >(func)->func)(a1));

  evararray arr;
  arr.add(a1);
  evar res(call(arr));
  if (res.getTypeid() == typeid(R))
    return(res.getarg( (void(*)(R))0x00 )); 

  res=res.convert(typeid(R)).var;
  if (res.isNull())
    return(R());
  return(res.getarg( (void(*)(R))0x00 )); 
}

template <class A1>
evar efunc::operator()(A1 a1)
{
  if (func==0x00) return(evar());

  if (typeid(void(*)(A1))==func->getTypeid()){
    (*static_cast< efuncType<void(*)(A1)>* >(func)->func)(a1);
    return(evar());
  }

  evararray arr;
  arr.add(a1);
  return(call(arr));
}

template <class A1,class A2>
evar efunc::operator()(A1 a1,A2 a2)
{
  if (func==0x00) return(evar());

  if (typeid(void(*)(A1,A2))==func->getTypeid()){
    (*static_cast< efuncType<void(*)(A1,A2)>* >(func)->func)(a1,a2);
  }

  evararray arr;
  arr.add(a1);
  arr.add(a2);
  return(call(arr));
}

template <class A1,class A2,class A3>
evar efunc::operator()(A1 a1,A2 a2,A3 a3)
{
  if (func==0x00) return(evar());

  evararray arr;
  arr.add(a1);
  arr.add(a2);
  arr.add(a3);
  return(call(arr));
}

template <class A1,class A2,class A3,class A4>
evar efunc::operator()(A1 a1,A2 a2,A3 a3,A4 a4)
{
  if (func==0x00) return(evar());

  evararray arr;
  arr.add(a1);
  arr.add(a2);
  arr.add(a3);
  arr.add(a4);
  return(call(arr));
}

template <class A1,class A2,class A3,class A4,class A5>
evar efunc::operator()(A1 a1,A2 a2,A3 a3,A4 a4,A5 a5)
{
  if (func==0x00) return(evar());

  evararray arr;
  arr.add(a1);
  arr.add(a2);
  arr.add(a3);
  arr.add(a4);
  arr.add(a5);
  return(call(arr));
}



#endif

