#include "earray.h"

template <>
void earray<evar>::serial(estr& data) const
{
  serialint(size(),data);
  if (size()==0) return;

  size_t ilen=data.len();

  if (data.len()-ilen>0){ data.reserve(data.len()*size()); }

  size_t i;
  for (i=0; i<size(); ++i)
    at(i).serial(data);
//    ::serial(at(i),data);
}


//  evar var(value);
//  var.var->cleanup=false;
//  if (!getClasses().exists(var.getClass())) { lerror(estr("class not registered: ")+var.getClass()); return(i); }
//  return(getClasses().values(var.getClass()).funserial(var.var,data,i));


template <>
size_t earray<evar>::unserial(const estr& data,size_t i)
{
  clear();
  unsigned int count;
  evar var;
//  T var;
  i=unserialint(count,data,i);
  if (i==-1) return(-1);

  while(count && i<data.len()){
    i=var.unserial(data,i);
    if (i==-1) return(-1);
    add(var);
    var.clear();
    --count;
  }

  if (count) return(-1);

  return(i);
}

template <>
void earray<evar>::addvar(evar& var)
{
  add(var);
}

template <>
evar earray<evar>::getvar(size_t i) const
{
  return(at(i));
}
