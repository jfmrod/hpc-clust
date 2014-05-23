#include "ebasearray.h"

#include "evar.h"
#include "efile.h"

void ebasearray::save(const estr& file) const
{
  efile f(file,"w");
  estr s;
  serial(s);

  cout << "serialized len: " << s.len() << endl;
  f.write(s);
}

void ebasearray::load(const estr& file)
{
  efile f(file,"r");
  estr s;
  f.read(s);
  size_t i=0;
  unserial(s,i);
}

void ebasearray::serial(estr& data) const
{
  serialint(size(),data);
  size_t i;
  for (i=0; i<size(); ++i)
    getvar(i).serial(data);
}

size_t ebasearray::unserial(const estr& data,size_t i)
{
  unsigned int count;
  evar var;
  i=unserialint(count,data,i);
  if (i==-1) return(-1);
  while(count && i<data.len()){
    var.clear();
    i=var.unserial(data,i);
    if (i==-1) return(-1);
    addvar(var);
    --count;
  }
  if (count>0) return(-1);
  return(i);
}

ostream& operator<<(ostream& stream,const ebasearray& arr)
{
  size_t i;
  stream << "[";
  if (arr.size()==0) { stream << "]"; return(stream); }

  for (i=0; i<arr.size()-1; ++i)
    stream << arr.getvar(i) << ",";
  stream << arr.getvar(i);
  stream << "]";
  return(stream);
}

