#ifndef EVARCLASSPROPERTY_H
#define EVARCLASSPROPERTY_H

#include "eutils.h"

class evar;
class evarBase;

class eclassPropertyBase
{
 public:
  virtual evar operator()(evarBase* var)=0;
};

template <class T,class P>
class eclassProperty : public eclassPropertyBase
{
 public:
  P property;
  eclassProperty(P property,const estr& propertyname,const estr& info);
  virtual evar operator()(evarBase* var);
};

#include "evar_dec.h"

template <class T,class P>
eclassProperty<T,P>::eclassProperty(P _property,const estr&
propertyname,const estr& info): property(_property) {}

template <class T,class P>
evar eclassProperty<T,P>::operator()(evarBase* var)
{
  return(((dynamic_cast<evarType<T>*>(var)->object)->*property));
}

#endif

