#include "ebasearrayof.h"

#include "evar.h"

void ebasearrayof::serial(estr& data) const
{
  serialint(size(),data);
  size_t i;
  for (i=0; i<size(); ++i){
    getvarkey(i).serial(data);
    getvar(i).serial(data);
  }
}

size_t ebasearrayof::unserial(const estr& data,size_t i)
{
  unsigned int count;
  evar keyvar,var;
  i=unserialint(count,data,i);
  if (i==-1) return(-1);
  while(count && i<data.len()){
    keyvar.clear(); var.clear();
    i=keyvar.unserial(data,i);
    if (i==-1) return(-1);
    i=var.unserial(data,i);
    if (i==-1) return(-1);
    addvar(keyvar,var);
    --count;
  }
  if (count) return(-1);
  return(i);
}

ostream& operator<<(ostream& stream,const ebasearrayof& arr)
{
  size_t i;
  stream << "[";
  if (arr.size()==0){ stream << "]"; return(stream); }

  for (i=0; i<arr.size()-1; ++i)
    stream << arr.getvarkey(i) << "=" << arr.getvar(i) << ",";
  stream << arr.getvarkey(i) << "=" << arr.getvar(i);
  stream << "]";
  return(stream);
}

