#include "estr.h"

#include <stdio.h>
#include <ctype.h>

#include "logger.h"
#include "eregexp.h"

#include <inttypes.h>
#include "evar.h"

void estr::serial(estr& data) const
{
  data.reserve(data.len()+sizeof(uint32_t)+len());
  *((uint32_t*)&data[data._strlen])=len();
  strcpy(&data[data._strlen+sizeof(uint32_t)],_str);
  data._strlen+=len()+sizeof(uint32_t);
}

int estr::unserial(const estr& data,int i)
{
  unsigned long len;
  len=*((uint32_t*)&data._str[i]);
  if (data.len()<i+len){ lerror("truncated serial string?"); return(-1); }

  reserve(len);
  strncpy(_str,&data._str[i+sizeof(uint32_t)],len);
  _str[len]=0x00;
  _strlen=len;
  return(i+len+sizeof(uint32_t));
}

bool estr::is_hex() const
{
  if (len()<3) return(false);


  if (_str[0]!='0' || _str[1]!='x') return(false);

  long int i;
  for (i=2; i<len(); ++i){
    if ((_str[i] < '0' || _str[i] > '9') && (_str[i]<'a' || _str[i]>'f') && (_str[i]<'A' || _str[i]>'F')) return(false);
  }
  return(true);
}

bool estr::is_int() const
{
  if (!len()) return(false);

  long int i;
  i=0;
  if (_str[0]=='-' && i+1 < len())
    i=1;
  for (; i<len(); ++i){
    if (_str[i] < '0' || _str[i] > '9') return(false);
  }
  return(true);
}

bool estr::is_float() const
{
  if (!len()) return(false);

  long int i;
  i=0;
  if (_str[0]=='-' && i+1 < len())
    i=1;
  bool exp;
  bool dot;
  bool num;
  num=false;
  exp=false;
  dot=false;
  for (; i<len(); ++i){
    if (_str[i] < '0' || _str[i] > '9'){
      if (!num || exp)
        return(false);
      if (_str[i]=='e' || _str[i]=='E'){
        exp=true;
        if (i+1<len()){
          if (_str[i+1]=='-')
            ++i;
        }else
          return(false);   // exponent letter appearing without exponent number
      }else if (_str[i]=='.' && !dot){
        dot=true;
      }else{
        return(false);
      }
    }else
      num=true;
  }
  return(true);
}

int estr::h() const
{
  int i;
  if (!sscanf(_str,"%x",&i)) i=0;
  return(i);
}

int estr::i() const
{
  int i;
  if (!sscanf(_str,"%i",&i)) i=0;
  return(i);
}

float estr::f() const
{
  float f;
  if (!sscanf(_str,"%f",&f)) f=0.0;
  return(f);
}

double estr::d() const
{
  double d;
  if (!sscanf(_str,"%lf",&d)) d=0.0;
  return(d);
}

estr& estr::uppercase()
{
  char *p;
  p=_str;
  while(*p!=0x00){
    *p = toupper(*p); ++p;
  }
  return(*this);
}

estr& estr::lowercase()
{
  char *p;
  p=_str;
  while(*p!=0x00){
    *p = tolower(*p); ++p;
  }
  return(*this);
}

//eregexp re_endln("\r?\n");

const char *estr::getline(long int &l,const char *echar)
{
  long int i,i2;
  char *ptmp;
//  estr res;

  ptmp=_str;
//  i=re_strpos(*this,re_endln);
  i=find(echar);

  if (i==-1){
//    res=*this;
    l=len();
    del(0,len());
    return(ptmp);
  }

//  res=substr(0,i);
  l=i;
  if (_str[i]==0x0C){
    _str[i]=0x00;
    del(0,i+2); // if we have \r\n remove both, else
  } else {
    _str[i]=0x00;
    del(0,i+1); // just remove \n
  }
  

  return(ptmp);
}

bool estr::getline(estr& line)
{
  line.clear();
  long int i;

  i=find('\n');
  if (i==-1)
    return(false);

  if (i>0 && _str[i-1]=='\r')
    line=substr(0,i-1);
  else
    line=substr(0,i);
  del(0,i+1);

  return(true);
}

const char *estr::getline()
{
  long int i,i2;
  char *ptmp;

  ptmp=_str;
  i=find('\n');
  if (i==-1)
    return(NULL);

  if (i>0 && _str[i-1]==0x0D){
    _str[i-1]=0x00;
    del(0,i+1); // if we have \r\n remove both, else
  } else {
    _str[i]=0x00;
    del(0,i+1); // just remove \n
  }

  return(ptmp);
}

/*
  estr res;
  int i,k;

  k=0;
  i=re.match(str);
  while (i!=-1 && i<str.len()){
    res += str.substr(k,i-k);
    res += replace;
//    cout << str.substr(k,i-k) << endl;
//    cout << "re.b: "<< re.b <<" ,re.e: "<<re.e<<endl;
    k=re.e;
    i=re.match(str,re.e);
  }
  res += str.substr(k);
  return(res);
*/
estr &estr::replace(const estr &pattern,const estr &replace)
{
  estr res;
  long int i,k;

  k=0;
  i=find(pattern);
  while (i!=-1 && i<len()){
    res += substr(k,i-k);
    res += replace;
//    cout << str.substr(k,i-k) << endl;
//    cout << "re.b: "<< re.b <<" ,re.e: "<<re.e<<endl;
    k=i+pattern.len();
    i=find(pattern,k);
  }
  res += substr(k);
  *this=res;
  return(*this);
}

estr& estr::trim()
{
  char *pd,*ps,*p2;
  bool match;
  bool lastspace;

  lastspace=true;
  pd=_str; ps=_str;
  while(*ps!=0x00){
    match=false;
    if (lastspace){
       if (*ps == ' ') match=true;
       else lastspace=false;
    }
    else if (*ps == ' ') lastspace=true;
    if (!match){
      *pd=*ps;
      ++ps; ++pd;
    }
    else
      ++ps;
  }
  if (lastspace && pd != _str) --pd;
  *pd=*ps;
  _strlen = strlen(_str);
  return(*this);
}

void estr::delstr(const char *str)
{
  char *pd,*ps;
  const char *ps2,*p2;
  long int slen;
  bool match;

  slen=strlen(str);
  pd=_str; ps=_str;
  while(*ps!=0x00){
    match=true;
    ps2=ps;
    for (p2=str,ps2=ps; *p2!=0x00 && *ps2!=0x00; ++p2,++ps2){
      if (*ps2 != *p2)
        { match=false; break; }
    }
    if (!match){
      *pd=*ps;
      ++ps; ++pd;
    }
    else
      ps+=slen;
  }
  *pd=*ps;
  _strlen = strlen(_str);
}


void estr::sprintf(const estr &format,...)
{
  va_list ap;
  char tmpsz[1024];

  va_start(ap,format);

  vsnprintf(tmpsz,1024,format._str,ap);
  *this = tmpsz;

  va_end(ap);
}

void estr::remove_chars(const char *remove_chars)
{
  char *pd,*ps;
  const char *p2;
  bool match;

  pd=_str; ps=_str;
  while(*ps!=0x00){
    match=false;
    for (p2=remove_chars; *p2!=0x00; ++p2){
      if (*ps == *p2)
        { match=true; break; }
    }
    if (!match){
      *pd=*ps;
      ++ps; ++pd;
    }
    else
      ++ps;
  }
  *pd=*ps;
  _strlen = strlen(_str);
}

void estr::clean(const char *ignore_chars)
{
  char *p;
  const char *p2;
  for(p=_str; *p!=0x00; ++p){
    for (p2=ignore_chars; *p2!=0x00; ++p2){
      if (*p == *p2)
        { *p = ' '; break; }
    }
  }
}

estrarray estr::explode(const estr &seperator) const
{
  estrarray res;
  long int i,k;
  if (len()==0) return(res);

  k=0;
  i=find(seperator);
  while (i!=-1 && i<len()){
    res.add(substr(k,i-k));
    k=i+seperator.len(); 
    i=find(seperator,i+1);
  }
  
  if (k!=len())
    res.add(substr(k));

  return(res);
}

void estr::_checkNewSize(long int size)
{
  if (size >= _asize){
    delete[] _astr;
    _asize = (size/255+1)*255;
    _astr = new char[_asize];
    _str = _astr;
  }
}

void estr::_checkSize(long int size)
{
  char *tmpsz;
//  printf(" warning: asize %i  size %i  >=  %i     d: %i\n",_asize,size,_asize - (_str-_astr),(_str-_astr));
  if (size >= _asize){
    _asize = (size/255+1)*255;
    tmpsz = new char[_asize];
    memcpy(tmpsz,_str,_strlen+1);
    delete[] _astr;
    _astr = tmpsz;
    _str = _astr;
  }
  else if (size >= _asize - (_str - _astr)){
    // _str pointer is too much forward in _astr, that we have to copy it back
    memmove(_astr,_str,_strlen+1);
    _str=_astr;
  }

}

estr &estr::operator =(const estr &str)
{
  if (&str==this) return(*this); // avoid copying yourself

  _checkNewSize(str.len());
  _str=_astr;
  memcpy(_str,str._str,str.len()+1);
  _strlen = str.len();
  return(*this);
}

estr &estr::operator+=(const estr &str)
{
  long int slen;
  slen=str.len()+_strlen;
  _checkSize(slen);
  memcpy(&_str[len()],str._str,str.len()+1);
  _strlen = slen;
  return(*this);
}

estr  estr::operator+ (const estr &str) const
{
  estr str2;
  str2=*this;
  str2+=str;
  return(str2);
}

estr &estr::operator+=(const char *str)
{
  long int slen;
  slen = strlen(str)+_strlen;
  _checkSize(slen);
  strcat(_str,str);
  _strlen = slen;
  return(*this);
}

estr &estr::operator=(const char *str)
{
  long int slen;
  if (str==0x00) { clear(); return(*this); }
  slen = strlen(str);
  _checkNewSize(slen);
  _str=_astr;
  strcpy(_str,str);
  _strlen = slen;
  return(*this);
}

#ifdef _WIN32
estr &estr::operator=(const wchar_t *str)
{
  long int slen;
  slen=wcslen(str);
  reserve(slen);
  wcstombs(_str,str,slen);
  _str[slen] = 0x00;
  _strlen=slen;
  return(*this);
}
#endif

estr estr::operator+(const char *str) const
{
  long int slen;
  estr str2;
  
  slen=strlen(str);
  str2 =_str;
  str2+=str;
  return(str2);
}

void estr::insert(long int pos,char c)
{
  ldwarnif(pos>_strlen,"estr::insert, out of bounds");

  _checkSize(_strlen+1);
  
  long int i;
  for (i=_strlen; i>=pos; --i)
    _str[i+1] = _str[i];

  _str[pos] = c;
  ++_strlen;
}

void estr::insert(long int pos,const char *str)
{
  ldwarnif(pos>_strlen,"estr::insert, out of bounds");

  long int slen;

  slen = strlen(str);
  _checkSize(_strlen+slen);
  
  long int i;
  for (i=_strlen; i>=pos; --i)
    _str[i+slen] = _str[i];

  for (i=0; i<slen; ++i)
    _str[pos+i] = str[i];
  _strlen+=slen;
}

long int estr::erase(long int i,long int l)
{
  if (i<0) i+=len();
  if (l<0) l+=len()-i+1;
 
  if (i>=len() || i<0 || l<=0) return(0);

  if (i+l>len()) l=len()-i;
  
  long int d;

  if (i==0){
    _str = &_str[l];
//    printf(" d: %i\n ",_str-_astr);
  }else{
    d=len()-i-l+1;
    for (; d>0; --d,++i)
      _str[i]=_str[i+l];
  }

  _strlen -= l;
  return(l); 
}

estr estr::substr(long int i,long int l) const
{
  estr res;

  if (i<0) i+=len();    // if i is negative then assume its counting from the end
  if (l<0) l+=len()-i+1; // if len = -1, copy till the end of the file

  if (i>=len() || i<0 || l<=0) return(res);

  l+=i;
  if (l>len()) l=len();
  for (;i<l;++i)
    res+=_str[i];

  return(res);
}

long int estr::ifind(const estr &needle,long int i) const
{
  char *r;
  if (needle.len()==0) { lwarn("estr::find: needle string is empty!"); return(-1); }

  if (i<0) i+=len();
  if (i>=len() || i<0) return(-1);

  r=strcasestr(&_str[i],needle._str);

  if (!r) return(-1);

  return(r-_str);
}

long int estr::find(const estr &needle,long int i) const
{
  char *r;
  if (needle.len()==0) { lwarn("estr::find: needle string is empty!"); return(-1); }

  if (i<0) i+=len();
  if (i>=len() || i<0) return(-1);

  r=strstr(&_str[i],needle._str);

  if (!r) return(-1);

  return(r-_str);
}

long int estr::count(const estr &needle,long int i) const
{
  long int count;

  lddieif(i<0 || i >= len(),"estr::count index out of bounds");
  count=-1;
  --i;
  do {
    i=find(needle,i+1);
     ++count; 
  }while(i!=-1 && i<len());
  return(count);
}



void estr::init()
{
  _asize = 255;
  _strlen = 0;
  _astr = new char[_asize];
  _str = _astr;
  _str[0] = 0x00;
}


#ifdef __APPLE__
estr::estr(CFStringRef cfstr)
{
  init();
  *this=cfstr;
}

estr& estr::operator=(CFStringRef cfstr)
{
  const char *str;
  str=CFStringGetCStringPtr(cfstr,kCFStringEncodingMacRoman);
  if (str==NULL){
    reserve(CFStringGetLength(cfstr));
    lerrorif(!CFStringGetCString(cfstr,_str,_asize,kCFStringEncodingMacRoman),"did not reserve enough space for CFString");
  }
  *this=str;
}
#endif


estr::estr():_str(0x00)
{
  init();
}

estr::estr(const unsigned long int i):_str(0x00)
{
  init();
  ::sprintf(_str,"%lu",i);
  _strlen = strlen(_str);
}

estr::estr(const unsigned int i):_str(0x00)
{
  init();
  ::sprintf(_str,"%u",i);
  _strlen = strlen(_str);
}

estr::estr(const long int i):_str(0x00)
{
  init();
  ::sprintf(_str,"%li",i);
  _strlen = strlen(_str);
}

estr::estr(const int i):_str(0x00)
{
  init();
  ::sprintf(_str,"%i",i);
  _strlen = strlen(_str);
}

estr::estr(const float f):_str(0x00)
{
  init();
  ::sprintf(_str,"%.8g",f);
  _strlen = strlen(_str);
}

estr::estr(const double d):_str(0x00)
{
  init();
  ::sprintf(_str,"%.16g",d);
  _strlen = strlen(_str);
}

estr::estr(const char c):_str(0x00)
{
  init();
  _str[0] = c;
  _str[1] = 0x00;
  _strlen = 1;
}



estr::estr(const estr &str):_str(0x00)
{
  init();
  *this = str;  
}

estr::estr(const char *str):_str(0x00)
{
  init();
  *this = str;  
}

#ifdef _WIN32
estr::estr(const wchar_t* str): _str(0x00)
{
  init();
  *this = str;
}
#endif

estr::estr(const estrarray &strarr):_str(0x00)
{
  init();

  if (strarr.size()){  
    int i;
    for (i=0; i<strarr.size()-1; ++i){
      if (strarr.keys(i).len()){
         *this += strarr.keys(i);
         *this += "=";
      }
      *this += strarr.values(i);
      *this += ",";
    }
    if (strarr.size()){
      if (strarr.keys(i).len()){
        *this += strarr.keys(i);
        *this += "=";
      }
      *this += strarr.values(i);
    }
  }
}



estr::~estr()
{
  if (_astr)
    delete[] _astr;
}

