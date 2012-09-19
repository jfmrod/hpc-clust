#include "ebasearrayof.h"

#include "evar.h"

void ebasearrayof::serial(estr& data) const
{
  serialint(size(),data);
  int i;
  for (i=0; i<size(); ++i){
    getvarkey(i).serial(data);
    getvar(i).serial(data);
  }
}

int ebasearrayof::unserial(const estr& data,int i)
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
  int i;
  stream << "[";
  for (i=0; i<(int)arr.size()-1; ++i)
    stream << arr.getvarkey(i) << "=" << arr.getvar(i) << ",";
  if (i<arr.size())
    stream << arr.getvarkey(i) << "=" << arr.getvar(i);
  stream << "]";
  return(stream);
}

