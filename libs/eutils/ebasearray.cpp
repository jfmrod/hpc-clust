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
  int i=0;
  unserial(s,i);
}

void ebasearray::serial(estr& data) const
{
  serialint(size(),data);
  int i;
  for (i=0; i<size(); ++i)
    getvar(i).serial(data);
}

int ebasearray::unserial(const estr& data,int i)
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
  int i;
  stream << "[";
  for (i=0; i<(int)arr.size()-1; ++i)
    stream << arr.getvar(i) << ",";
  if (i<arr.size())
    stream << arr.getvar(i);
  stream << "]";
  return(stream);
}

