#include "estrhash.h"
#include "ehashmap.h"

#include "efile.h"

estr estrhash::join(const estr& fseparator,const estr& vseparator)
{
  estr str;

  int i;
  for (i=0; i<size()-1; ++i){
    if (keys(i).len()){
      str += keys(i);
      str += vseparator;
    }
    str += values(i);
    str += fseparator;
  }
  if (size()){  
    if (keys(i).len()){
      str += keys(i);
      str += vseparator;
    }
    str += values(i);
  }
  return(str);
}

void estrhash::split(const estr& str,const estr& fseparator,const estr& vseparator)
{
  estrarray items,pairs;
  int i;

  items = str.explode(fseparator);
  for (i=0; i<items.size(); ++i){
    pairs = items.values(i).explode(vseparator);
    if (pairs.size()>1)
      add(pairs.values(0),pairs.implode(vseparator,1));
    else if (pairs.size()==1)
      add(pairs.values(0),"");
  }
}

void estrhash::load(const estr& filename)
{
  estr text;
  efile(filename,"r").read(text);
  split(text,"\n","=");
}

void estrhash::save(const estr& filename)
{
  efile(filename,"w").write(join("\n","="));
}



