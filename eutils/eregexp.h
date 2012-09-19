#ifndef EREGEXP_H
#define EREGEXP_H

#include "eutils.h"

#include <sys/types.h>

#include <regex.h>

#include <stdio.h>

#include "estr.h"
#include "estrarray.h"

class eregexp
{
 private:
  regex_t reg;
  size_t nmatch;
  regmatch_t *pmatch;
  bool compiled;
 public:
  int b;
  int e;

  eregexp();
  eregexp(const char *str);
  eregexp(const estr& str);
  eregexp(const estr& str,int nmatch);
  ~eregexp();
  
  void compile(const estr &matchstr,int cflags=0x00);
  int match(const estr& text,int pos=0,int eflags=0x00);
  int match(const estr& text,int pos,int& b,int& e,int eflags=0x00) const;
};

int re_strpos(const estr& haystack,const eregexp& needle,int pos=0);

estr re_match(const estr& str,const eregexp& re,int pos=0);
void re_match(const estr& str,const eregexp& re,estr& res,int pos=0);


estr re_replace(const estr& str,const eregexp& re,const estr& replacestr);

estrarray re_explode(const estr& str,const eregexp& re);
estrarray& re_explode(const estr& str,const eregexp& re,estrarray &list);

inline bool grep(estr& str,const eregexp& re,estr& ret,int pos=0)
{
  int b,e;
  re.match(str,pos,b,e);
  if (b==-1) return(false);
  str.del(b,e-b);
  return(true); 
}

inline estr grep(estr& str,const eregexp& re,int pos=0)
{
  int b,e;
  re.match(str,pos,b,e);
  if (b==-1) return("");
  estr ret(str.substr(b,e-b));
  str.del(b,e-b);
  return(ret); 
}

extern eregexp refloat;
extern eregexp reint;

inline bool grepdouble(estr& str,double& ret,int pos=0)
{
  refloat.match(str);
  if (refloat.b==-1) return(false);
  
  sscanf(&str[refloat.b],"%lg",&ret);
  return(true);
}

inline bool grepfloat(estr &str,float& ret,int pos=0)
{
  estr sret;
  if (grep(str,refloat,sret,pos)){
    ret=sret.f(); return(true);
  }
  return(false);
}

inline bool grepint(estr &str,int& ret,int pos=0)
{
  estr sret;
  if (grep(str,reint,sret,pos)){
    ret=sret.i(); return(true);
  }
  return(false);
}

#endif

