#ifndef EFUNC_DEC_H
#define EFUNC_DEC_H

#include "eutils.h"
#include <typeinfo>

class efuncBase;
class estr;
class evar;

#include "ebasicarray_dec.h"
#include "evararray.h"
#include "estr.h"

class eclassMethodBase;

class efunc
{
  void updateInfo();
  void setDefaultArgs(evararray& args);
 public:
  efuncBase* func;
  evararray  defargs;
  
  ebasicarray<const type_info*> fArgs;
  const type_info *       fReturn;

  efunc();
  template <class F>
  efunc(F pfunc,const evararray& args=evararray());

  efunc(const evar& obj,eclassMethodBase *pmethod,const evararray& args=evararray());
  efunc(evar& obj,eclassMethodBase *pmethod,const evararray& args=evararray());

  template <class T,class M>
  efunc(T& obj,M pmethod,const evararray& args=evararray());



  efunc(const efunc& func);
  
  virtual ~efunc();

  void clear();

  inline bool isSet(){ return(func); }

  template <class F>
  efunc& operator=(F pfunc);
 
  efunc& operator=(const efunc& func);
  efunc& setFunc(efuncBase* func);

  evar call(const estr& args="");
  virtual evar call(evararray& args);
  virtual evar call(const evararray& args);
  template <class R,class A1>
  R call(A1 a1);

  evar operator()();
  
  evar operator()(const evararray& args);
  evar operator()(evararray& args);

  template <class A1>
  evar operator()(A1 a1);
  template <class A1,class A2>
  evar operator()(A1 a1,A2 a2);
  template <class A1,class A2,class A3>
  evar operator()(A1 a1,A2 a2,A3 a3);
  template <class A1,class A2,class A3,class A4>
  evar operator()(A1 a1,A2 a2,A3 a3,A4 a4);
  template <class A1,class A2,class A3,class A4,class A5>
  evar operator()(A1 a1,A2 a2,A3 a3,A4 a4,A5 a5);
};

ostream& operator<<(ostream& stream,const efunc& func);


#endif

