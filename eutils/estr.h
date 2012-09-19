#ifndef ESTR_H
#define ESTR_H

#include "eutils.h"

#include <stdarg.h>
#include <stdio.h>

#include <string.h>
#include <iostream>

#ifdef __APPLE__
 #include <Carbon/Carbon.h> 
#endif


using namespace std;
class estrarray;

class estr
{
 private:
  void init();
 public:
 char *_str;
 char *_astr;
 long int _strlen;
 long int _asize;
 
#ifdef _CPP_STRING
  estr(string str);
#endif
#ifdef __APPLE__
  estr(CFStringRef cfstr);
  estr& operator=(CFStringRef str);
#endif
  estr(const estrarray &strarr);
  estr(const estr &str);
  estr(const unsigned long int i);
  estr(const unsigned int i);
  estr(const long int i);
  estr(const int i);
  estr(const float f);
  estr(const double d);
  estr(const char c);
  estr(const char *str);
#ifdef _WIN32
  estr(const wchar_t* str);
#endif
  estr();
  ~estr();

#ifdef __APPLE__
  inline CFStringRef cfstr() const { return(CFStringCreateWithCString(NULL,_str,kCFStringEncodingMacRoman)); }
#endif

  inline long int len() const { return(_strlen); }
  inline long int length() const { return(_strlen); }
  inline long int size() const { return(_strlen); }

  inline void clear() { _str=_astr; _str[0]=0x00; _strlen=0; }

  inline char &operator[](long int pos) { return( _str[pos] ); }
  inline const char operator[](long int pos) const { return( _str[pos] ); }


  inline bool icmp(const estr& str) const { return(_strlen==str._strlen && !strncasecmp(_str,str._str,_strlen)); }

  inline bool operator>(const estr &str)  const { return(memcmp(_str,str._str,min(_strlen,str._strlen))>0); }
  inline bool operator>=(const estr &str)  const { return(memcmp(_str,str._str,min(_strlen,str._strlen))>=0); }
  inline bool operator<(const estr &str)  const { return(memcmp(_str,str._str,min(_strlen,str._strlen))<0); }
  inline bool operator<=(const estr &str)  const { return(memcmp(_str,str._str,min(_strlen,str._strlen))<=0); }
  inline bool operator==(const char *cstr) const { return(_strlen==strlen(cstr) && !memcmp(_str,cstr,_strlen)); }
  inline bool operator==(const estr &str)  const { return(_strlen==str._strlen && !memcmp(_str,str._str,_strlen)); }
  inline bool operator!=(const char *cstr) const { return(_strlen!=strlen(cstr) || memcmp(_str,cstr,_strlen)); }
  inline bool operator!=(const estr &str)  const { return(_strlen!=str._strlen || memcmp(_str,str._str,_strlen)); }
//  inline bool operator!=(const estr &str)  const { return(strcmp(_str,str._str)); }
 
  estr &operator =(const char *str);
  estr &operator =(const estr &str);
  #ifdef _WIN32
  estr &operator =(const wchar_t* str);
  #endif
 
  inline estr &operator=(const int &val)    { return(*this=estr(val)); }
  inline estr &operator=(const float &val)  { return(*this=estr(val)); }
  inline estr &operator=(const double &val) { return(*this=estr(val)); }
  /////////////////

  estr &operator+=(const char *str);
  estr &operator+=(const estr &str);

  inline estr &operator+=(const char &val)   
    { _checkSize(_strlen+1);
      _str[_strlen] = val;
      _str[++_strlen] = 0x00;
      return(*this); }
  inline estr &operator+=(const long int &val)    { return(*this=*this+estr(val)); }
  inline estr &operator+=(const int &val)    { return(*this=*this+estr(val)); }
  inline estr &operator+=(const unsigned int &val)    { return(*this=*this+estr(val)); }
  inline estr &operator+=(const unsigned long int &val)    { return(*this=*this+estr(val)); }
  inline estr &operator+=(const float &val)  { return(*this=*this+estr(val)); }
  inline estr &operator+=(const double &val) { return(*this=*this+estr(val)); }
  ///////////////// 

  estr operator+ (const char *str) const;
  estr operator+ (const estr &str) const;

  inline estr operator+(const long int &val)    const { return(*this+estr(val)); }
  inline estr operator+(const long unsigned int &val)    const { return(*this+estr(val)); }
  inline estr operator+(const unsigned int &val)    const { return(*this+estr(val)); }
  inline estr operator+(const int &val)    const { return(*this+estr(val)); }
  inline estr operator+(const float &val)  const { return(*this+estr(val)); }
  inline estr operator+(const double &val) const { return(*this+estr(val)); }
  /////////////////

  int    h() const;
  int    i() const;
  float  f() const;
  double d() const;

  bool is_hex() const;
  bool is_int() const;
  bool is_float() const;
  bool is_double() const;
//  bool is_str() const;

  /////////////////

  inline bool reserve(long int size) { _checkSize(size); return(true); }
  void _checkNewSize(long int size);
  void _checkSize(long int size);

  const char *getline(long int &len,const char *eline="\n");
  const char *getline();
  bool getline(estr& line);

  estr& trim();
  estr& uppercase();
  estr& lowercase();

  void remove_chars(const char *remove_chars="(){}[]'");
  void clean(const char *ignore_chars="(){}[]'\"\\/+=-_|:;<>,.?`~!@#$%^&*");
  void delstr(const char *str);

  void sprintf(const estr &format,...);
  
  estr &replace(const estr &pattern, const estr &replace);

  void insert(long int i,char c);
  void insert(long int i,const char *str);

  long int erase(long int i,long int len=-1);  
  inline long int rem(long int i,long int len=-1) { return(erase(i,len)); }
  inline long int remove(long int i,long int len=-1) { return(erase(i,len)); }
  inline long int del(long int i,long int len=-1) { return(erase(i,len)); }

  estr substr(long int i,long int len=-1) const;
  long int ifind(const estr &needle,long int i=0) const;
  long int find(const estr &needle,long int i=0) const;
  long int count(const estr &needle,long int i=0) const;



  estrarray explode(const estr &seperator) const;

  inline int &operator>>(int &val) const
    { return(val=i()); }
  inline float &operator>>(float &val) const 
    { return(val=f()); }
  inline double &operator>>(double &val) const
    { return(val=d()); }
  inline estr &operator>>(estr &val) const
    { return(val=*this); }

  inline estr &operator<<(const int &val)
    { return(*this=val); }
  inline estr &operator<<(const float &val)
    { return(*this=val); }
  inline estr &operator<<(const double &val)
    { return(*this=val); }
  inline estr &operator<<(const estr &val)
    { return(*this=val); }


  void serial(estr &data) const;
  int unserial(const estr &data,int i);

  friend ostream &operator<<(ostream &stream,const estr &str);
};

inline estr hex(unsigned char val)
{
  estr res;
  res.reserve(34);
  int len;
  len=sprintf(res._str,"0x%.2hhx",val);
  res._strlen=len;
  return(res);
}

inline estr hex(char val)
{
  estr res;
  res.reserve(34);
  int len;
  len=sprintf(res._str,"0x%.2hhx",val);
  res._strlen=len;
  return(res);
}

inline estr hex(unsigned short val)
{
  estr res;
  res.reserve(34);
  int len;
  len=sprintf(res._str,"0x%.4hx",val);
  res._strlen=len;
  return(res);
}

inline estr hex(short val)
{
  estr res;
  res.reserve(34);
  int len;
  len=sprintf(res._str,"0x%.4hx",val);
  res._strlen=len;
  return(res);
}

inline estr hex(unsigned int val)
{
  estr res;
  res.reserve(34);
  int len;
  len=sprintf(res._str,"0x%.8x",val);
  res._strlen=len;
  return(res);
}

inline estr hex(int val)
{
  estr res;
  res.reserve(34);
  int len;
  len=sprintf(res._str,"0x%.8x",val);
  res._strlen=len;
  return(res);
}

inline estr hex(unsigned long val)
{
  estr res;
  res.reserve(34);
  int len;
  len=sprintf(res._str,"0x%.16lx",val);
  res._strlen=len;
  return(res);
}

inline estr hex(long val)
{
  estr res;
  res.reserve(34);
  int len;
  len=sprintf(res._str,"%.16lx",val);
  res._strlen=len;
  return(res);
}


inline estr operator+(const char *str1,const estr &str2)
{ return(estr(str1)+str2); }

inline estr operator+(long int val,const estr &str2)
{ return(estr(val)+str2); }

inline estr operator+(unsigned long int val,const estr &str2)
{ return(estr(val)+str2); }

inline estr operator+(unsigned int val,const estr &str2)
{ return(estr(val)+str2); }

inline estr operator+(int val,const estr &str2)
{ return(estr(val)+str2); }

inline estr operator+(float val,const estr &str2)
{ return(estr(val)+str2); }

inline estr operator+(double val,const estr &str2)
{ return(estr(val)+str2); }

inline ostream &operator<<(ostream &stream,const estr &str){
  int i;
  for (i=0; i<str.len(); ++i)
    stream << str[i];
  return(stream);
}

#include "estrarray.h"

#endif

