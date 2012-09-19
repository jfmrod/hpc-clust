
#include "estrarray.h"
#include "etable.h"
#include "efile.h"


const estr emptystr;

estrarray estrarray::subset(int i,int l) const
{
  estrarray tmpa;
 
  if (i<0) i+=size();
  if (l<0) l+=size()-i+1;

  if (i<0 || l<0 || !size()) return(tmpa);

  l+=i;
  for(;i<l;++i){
    if (_keys[i])
      tmpa.add(*_keys[i],*_values[i]);
    else
      tmpa.add(*_values[i]);
  }
  return(tmpa);
}

estr estrarray::join(const estr& fseparator,const estr& vseparator) const
{
  estr str;

  if (size()){  
    int i;
    for (i=0; i<size()-1; ++i){
      if (keys(i).len()){
         str += keys(i);
         str += vseparator;
      }
      str += values(i);
      str += fseparator;
    }
    if (keys(i).len()){
      str += keys(i);
      str += vseparator;
    }
    str += values(i);
  }
  return(str);
}


void estrarray::split(const estr& str,const estr& fseparator,const estr& vseparator)
{
  estrarray items,pairs;
  int i;

  items = str.explode(fseparator);
  for (i=0; i<items.size(); ++i){
    pairs = items.values(i).explode(vseparator);
    if (pairs.size()>1)
      add(pairs.values(0),pairs.implode(vseparator,1));
    else if (pairs.size()==1)
      add(pairs.values(0));
  }
}

void estrarray::load(const estr& filename)
{
  estr text;
  efile(filename,"r").read(text);
  split(text,"\n","=");
}

void estrarray::save(const estr& filename)
{
  efile(filename,"w").write(join("\n","="));
}


estrarray::estrarray(): earrayof<estr,estr>()
{
}

estrarray::estrarray(const estrarray& strarr): earrayof<estr,estr>(strarr)
{
}

/*
estrarray::estrarray(const ebasicarray<estr>& strarr): earrayof<estr,estr>(strarr)
{
}
*/
	
estrarray::estrarray(const char *cstr): earrayof<estr,estr>()
{
  split(cstr,",","=");
}


estrarray::estrarray(const estr& str): earrayof<estr,estr>()
{
  split(str,",","=");
}

estrarray::estrarray(char **str,int n): earrayof<estr,estr>()
{
  int i;
  for (i=0; i<n; ++i)
    add(str[i]);
}

estr estrarray::implode(const estr &seperator,int i,int l) const
{
  estr res;

  if (i<0) i+=size();
  if (l==-1) l+=size()-i+1;

  if (i<0 || !l || !size()) return(res);

  l+=i;
  for (;i<l-1;++i){
    res += values(i);
    res += seperator;
  }
  res += values(i);

  return(res);
}


estrarray estrarray::ffind(const estrarray &needle,int i) const
{
  estrarray res;
  int j;

  if (i<0) i+=size();

  if (i<0) return(res);

  for (; i<size(); ++i){
    for (j=0; j<needle.size(); ++j){
      if (values(i).find(needle.values(j))!=-1)
        res += values(i);
    }
  }
  return(res);
}

const estr& estrarray::operator[](const estr &key) const
{
  int i;
  for (i=0; i<size(); ++i){
    if (_keys[i] && *_keys[i] == key)
      return(*_values[i]);
  }
  // do not add a new entry on the list if this one does not exist 
  return(emptystr);
}

estr &estrarray::operator[](const estr &key)
{
  int i;
  for (i=0; i<size(); ++i){
    if (_keys[i] && *_keys[i] == key)
      return(*_values[i]);
  }
  // add a new entry on the list if this one does not exist 
  return(add(key,estr()));
}


const estr& estrarray::operator[](int i) const
{
  return(*_values[i]);
}

estr &estrarray::operator[](int i)
{
  return(*_values[i]);
}

