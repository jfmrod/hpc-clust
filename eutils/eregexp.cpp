#include "eregexp.h"

#include "logger.h"
#include "evar.h"
#include "estr.h"
#include "estrarray.h"


eregexp refloat("^-?[0-9]*\\.?[0-9]+([eE]-?[0-9]+)?([ 	]+|$)");
eregexp reint("^-?[0-9]+([ 	]+|$)");

estr grep(estr& str,const eregexp& re,int pos)
{
  int b,e;
  re.match(str,pos,b,e);
  if (b==-1) return("");
  estr ret(str.substr(b,e-b));
  str.del(b,e-b);
  return(ret); 
}

bool grepdouble(estr& str,double& ret,int pos)
{
  int b;
  b=refloat.match(str);
  if (b==-1) return(false);
  
  sscanf(&str[b],"%lg",&ret);
  return(true);
}

bool grepfloat(estr &str,float& ret,int pos)
{
  estr sret;
  if (grep(str,refloat,sret,pos)){
    ret=sret.f(); return(true);
  }
  return(false);
}

bool grepint(estr &str,int& ret,int pos)
{
  estr sret;
  if (grep(str,reint,sret,pos)){
    ret=sret.i(); return(true);
  }
  return(false);
}

bool grep(estr& str,const eregexp& re,estr& ret,int pos)
{
  int b,e;
  re.match(str,pos,b,e);
  if (b==-1) return(false);
  str.del(b,e-b);
  return(true); 
}





eregexp::eregexp(): nmatch(1),pmatch(0x00),compiled(false)
{
 pmatch = new regmatch_t[nmatch];
}

eregexp::eregexp(const char *str): nmatch(1),pmatch(0x00),compiled(false)
{
 pmatch = new regmatch_t[nmatch];
 if (strlen(str))
   compile(str);
}

eregexp::eregexp(const estr& str): nmatch(1),pmatch(0x00),compiled(false)
{
 pmatch = new regmatch_t[nmatch];
 if (str.len())
   compile(str);
}

eregexp::eregexp(const estr& str,int _nmatch): nmatch(_nmatch),pmatch(0x00),compiled(false)
{
 pmatch = new regmatch_t[nmatch];
 if (str.len())
   compile(str);
}

eregexp::~eregexp()
{
  if (compiled){
    regfree(&reg);
    compiled=false;
  }
  if (pmatch){
    delete[] pmatch;
    pmatch=0x00;
  }
}

void eregexp::compile(const estr &matchstr, int cflags)
{
  if (compiled){
    regfree(&reg);
    compiled=false;
  }
  cflags = cflags | REG_EXTENDED;
  compiled=true;
  ldieif(regcomp(&reg,matchstr._str,cflags),"error compiling regex: "+matchstr);
}

#define dcheck(a,b) dieif(a,b)

int eregexp::match(const estr &text, int pos, int eflags) const
{
  if (pos==text.len()) { /* b=-1; e=-1; */ return(-1); }

  lderrorif(pos>=text.len(),"eregexp: pos is bigger than string size");

  int res;
  res = regexec(&reg,&text._str[pos],nmatch,pmatch,eflags);
  if (res==REG_NOMATCH){ /* b=-1; e=-1; */ return(-1); }
  
  if (res==0){
//    b=pos+pmatch[0].rm_so;
//    e=pos+pmatch[0].rm_eo;
    return(pos+pmatch[0].rm_so);
  } else{
//    b=-1; e=-1;
    lerror("eregexp: error matching pattern");
    return(-1);
  }
}

int eregexp::match(const estr& text,int pos,int& b,int& e,int eflags) const
{
  if (pos==text.len()) { b=-1; e=-1; return(-1); }

  lderrorif(pos>=text.len(),"eregexp: pos is bigger than string size");

  int res;
  res = regexec(&reg,&text._str[pos],nmatch,pmatch,eflags);
  if (res==REG_NOMATCH){ b=-1; e=-1; return(-1); }
  
  if (res==0){
    b=pos+pmatch[0].rm_so;
    e=pos+pmatch[0].rm_eo;
    return(pos+pmatch[0].rm_so);
  } else{
    b=-1; e=-1;
    lerror("eregexp: error matching pattern");
    return(-1);
  }
}

int re_strpos(const estr& haystack,const eregexp& needle,int pos)
{
  int b,e;
  return(needle.match(haystack,pos,b,e));
}

void re_match(const estr& str,const eregexp& match,estr& res,int pos)
{
  int b,e;
  match.match(str,pos,b,e);
  if (b==-1) return;
  res=str.substr(b,e-b);
}


estr re_match(const estr& str,const eregexp& match,int pos)
{
  int b,e;
  match.match(str,pos,b,e);
  if (b==-1) return("");
  return(str.substr(b,e-b));
}

estrarray re_explode(const estr& str,const eregexp& re)
{
  estrarray res;
  int i,b,e,k;
  if (str.len()==0) return(res);

  k=0;
  i=re.match(str,0,b,e);
  while (i!=-1 && i<str.len()){
    res.add(str.substr(k,i-k));
//  cout << str.substr(k,i-k) << endl;
//  cout << "re.b: "<< re.b <<" ,re.e: "<<re.e<<endl;
    k=e;
    i=re.match(str,e,b,e);
  }
  
  res.add(str.substr(k));
  return(res);
}

estrarray& re_explode(const estr& str,const eregexp& re,estrarray& list)
{
  int i,b,e,k;
  list.clear();
  if (str.len()==0) return(list);

  k=0;
  i=re.match(str,0,b,e);
  while (i!=-1 && i<str.len()){
    list.add(str.substr(k,i-k));
//  cout << "re.b: "<< re.b <<" ,re.e: "<<re.e<<endl;
    k=e;
    i=re.match(str,e,b,e);
  }
  
  list.add(str.substr(k));

  return(list);
}

estr re_replace(const estr& str,const eregexp& re,const estr& replace)
{
  estr res;
  int i,b,e,k;

  k=0;
  i=re.match(str,0,b,e);
  while (i!=-1 && i<str.len()){
    res += str.substr(k,i-k);
    res += replace;
//    cout << str.substr(k,i-k) << endl;
//    cout << "re.b: "<< re.b <<" ,re.e: "<<re.e<<endl;
    k=e;
    i=re.match(str,e,b,e);
  }
  res += str.substr(k);
  return(res);
}

