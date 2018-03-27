#ifndef EREGEXP_H
#define EREGEXP_H

#include "eutils.h"

#include <sys/types.h>
#include <regex.h>
#include <stdio.h>

class estr;
class estrarray;

class eregexp
{
 private:
  regex_t reg;
  size_t nmatch;
  regmatch_t *pmatch;
  bool compiled;
 public:
//  int b;
//  int e;

  eregexp();
  eregexp(const char *str);
  eregexp(const estr& str);
  eregexp(const estr& str,int nmatch);
  ~eregexp();
  
  void compile(const estr &matchstr,int cflags=0x00);
//  int match(const estr& text,int pos=0,int eflags=0x00);
  int match(const estr& text,int pos=0,int eflags=0x00) const;
  int match(const estr& text,int pos,int& b,int& e,int eflags=0x00) const;
};

int re_strpos(const estr& haystack,const eregexp& needle,int pos=0);

estr re_match(const estr& str,const eregexp& re,int pos=0);
void re_match(const estr& str,const eregexp& re,estr& res,int pos=0);


estr re_replace(const estr& str,const eregexp& re,const estr& replacestr);

estrarray re_explode(const estr& str,const eregexp& re);
estrarray& re_explode(const estr& str,const eregexp& re,estrarray &list);

extern eregexp refloat;
extern eregexp reint;

bool grep(estr& str,const eregexp& re,estr& ret,int pos=0);
estr grep(estr& str,const eregexp& re,int pos=0);
bool grepdouble(estr& str,double& ret,int pos=0);
bool grepfloat(estr &str,float& ret,int pos=0);
bool grepint(estr &str,int& ret,int pos=0);

#endif

