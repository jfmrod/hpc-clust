#ifndef EVARCLASSMETHOD_H
#define EVARCLASSMETHOD_H

#include "eutils.h"

#include "ebasicarray.h"
#include "evararray.h"
#include "estrarray.h"


class evarBase;
class evar;
template <class T>
class earray;
typedef earray<evar> evararray;

class eclassMethodBase
{
 public:
  const type_info* fReturn;
  ebasicarray<const type_info*> fArgs;
  evararray defargs;

  void setDefaultArgs(evararray& args);

  virtual const type_info& getTypeid()=0;
  virtual evar operator()(evarBase* obj,evararray& args)=0;

  eclassMethodBase();
  eclassMethodBase(const evararray& args);
};

ostream& operator<<(ostream& stream,const eclassMethodBase& method);

template <class T,class F>
class eclassMethod : public eclassMethodBase
{
 public:
  F method;
  eclassMethod(F method,const estr& methodname,const evararray& args,const estr& info="");
  evar operator()(evarBase* obj,evararray& args);

  virtual const type_info& getTypeid() { return(typeid(F)); }

  void updateInfo();
};

#include "efunc.h"

template <class T,class F>
class eclassMethodFunc : public eclassMethodBase
{
 public:
  efunc func;
  eclassMethodFunc(F func,const estr& methodname,const evararray& args,const estr& info="");
  evar operator()(evarBase* obj,evararray& args);

  virtual const type_info& getTypeid() {return(typeid(F));}

  void updateInfo();
};

#include "estrarray.h"
#include "evarmethodcalls.h"
#include "evarmethodinfo.h"

template <class T,class F>
eclassMethod<T,F>::eclassMethod(F _method,const estr& methodname,const evararray& args,const estr& info): eclassMethodBase(args),method(_method)
{
  updateInfo();
}

template <class T,class F>
void eclassMethod<T,F>::updateInfo()
{
  eclassMethodInfo(method,*(eclassMethodBase*)this);
}

template <class T,class F>
evar eclassMethod<T,F>::operator()(evarBase* obj,evararray& args)
{
  eclassMethodBase::setDefaultArgs(args);
  return(eclassMethodCall(dynamic_cast<evarType<T>*>(obj)->object,method,args));
}



template <class T,class F>
eclassMethodFunc<T,F>::eclassMethodFunc(F _func,const estr& methodname,const evararray& args,const estr& info): eclassMethodBase(args),func(_func)
{
  updateInfo();
}

template <class T,class F>
void eclassMethodFunc<T,F>::updateInfo()
{
  int i;
  for (i=1; i<func.fArgs.size(); ++i)
    eclassMethodBase::fArgs.add(func.fArgs[i]);
  eclassMethodBase::fReturn = func.fReturn;
}

template <class T,class F>
evar eclassMethodFunc<T,F>::operator()(evarBase* obj,evararray& args)
{
  args.insert(0,obj);
  return(func.call(args));
}

#endif

