#include "eutils.h"
#include "evar.h"
#include "efunc.h"
#include "efunctype.h"
#include <typeinfo>

efuncObjMethod::efuncObjMethod(const evar& _object,eclassMethodBase *_method): object(_object),method(_method) {}

const type_info& efuncObjMethod::getTypeid()
{
  return(method->getTypeid());
}
/*
template <class T,class M>
evar efuncObjMethod<T,M>::operator()(evarBase* obj,evararray &args)
{
  return(eclassMethodCall(dynamic_cast<evarType<T>*>(obj)->object,pmethod,args));
}
*/

evar efuncObjMethod::call(const evararray &args)
{
  return(method->operator()(object.var,args));
}

void efuncObjMethod::updateInfo(efunc *f)
{
  f->fReturn=method->fReturn;
  f->fArgs  =method->fArgs;
}
