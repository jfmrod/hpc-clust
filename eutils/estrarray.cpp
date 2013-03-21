
#include "evar.h"
#include "estrarray.h"
#include "efile.h"


const estr emptystr;

estrarray estrarray::subset(long i,long l) const
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
    size_t i;
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
  size_t i;

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

estrarray::estrarray(char **str,size_t n): earrayof<estr,estr>()
{
  size_t i;
  for (i=0; i<n; ++i)
    add(str[i]);
}

estr estrarray::implode(const estr &seperator,size_t i,size_t l) const
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


estrarray estrarray::ffind(const estrarray &needle,size_t i) const
{
  estrarray res;
  size_t j;

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

estrarray estrarray::operator[](const eintarray& iarr) const
{
  estrarray tmparr;
  size_t i;
  for (i=0; i<iarr.size(); ++i)
    tmparr.add(keys(iarr[i]),values(iarr[i]));
  return(tmparr);
}

const estr& estrarray::operator[](const estr &key) const
{
  size_t i;
  for (i=0; i<size(); ++i){
    if (_keys[i] && *_keys[i] == key)
      return(*_values[i]);
  }
  // do not add a new entry on the list if this one does not exist 
  return(emptystr);
}

estr &estrarray::operator[](const estr &key)
{
  size_t i;
  for (i=0; i<size(); ++i){
    if (_keys[i] && *_keys[i] == key)
      return(*_values[i]);
  }
  // add a new entry on the list if this one does not exist 
  return(add(key,estr()));
}


const estr& estrarray::operator[](size_t i) const
{
  return(*_values[i]);
}

estr &estrarray::operator[](size_t i)
{
  return(*_values[i]);
}

size_t estrarray::ifind(const estr& value,size_t i)
{
  for (; i<size(); ++i)
    if (values(i).icmp(value)) return(i);
  return(-1);
}

size_t estrarray::ifindkey(const estr& key,size_t i)
{
  for (; i<size(); ++i)
    if (keys(i).icmp(key)) return(i);
  return(-1);
}
  
size_t estrarray::refind(const eregexp& re,size_t i)
{
  for (; i<size(); ++i)
    if (re.match(values(i))!=-1) return(i);
  return(-1);
}

size_t estrarray::refindkey(const eregexp& re,size_t i)
{
  for (; i<size(); ++i)
    if (re.match(keys(i))!=-1) return(i);
  return(-1);
}


eintarray estrarray::ifindall(const estr& value)
{
  eintarray tmparr;
  size_t i;
  for (i=ifind(value,0); i!=-1; i=ifind(value,i+1))
    tmparr.add(i);
  return(tmparr);
}

eintarray estrarray::ifindallkey(const estr& key)
{
  eintarray tmparr;
  size_t i;
  for (i=ifindkey(key,0); i!=-1; i=ifindkey(key,i+1))
    tmparr.add(i);
  return(tmparr);
}

eintarray estrarray::refindall(const estr& value)
{
  eintarray tmparr;
  size_t i;
  for (i=refind(value,0); i!=-1; i=refind(value,i+1))
    tmparr.add(i);
  return(tmparr);
}

eintarray estrarray::refindallkey(const estr& key)
{
  eintarray tmparr;
  size_t i;
  for (i=refindkey(key,0); i!=-1; i=refindkey(key,i+1))
    tmparr.add(i);
  return(tmparr);
}






