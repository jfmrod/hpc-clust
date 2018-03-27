#ifndef _ETYPES_
#define _ETYPES_

#include "eutils-config.h"

#include "estr.h"

class etype;

#include "estrarray.h"

enum evartype { ET_UNDEFINED, ET_ETYPE, ET_INT, ET_FLOAT, ET_DOUBLE, ET_CHAR, ET_PCHAR, ET_ESTR, ET_EARRAY };

class etype
{
 private:
  evartype type;
  void *value;
 public:
  etype();

  etype(const etype &var);

  etype(int var);
  etype(float var);
  etype(double var);
  etype(char   var);
  etype(char  *var);
  etype(const estr &var);
  etype(const estrarray &var);

  ~etype();

  void free_type();
  void set_type(evartype type);
  void set_value(void *_value);
  bool cmp_value(void *_value) const;



  etype &operator=(const etype &var);

  etype operator+(const etype &var);
  etype operator-(const etype &var);
  etype operator*(const etype &var);
  etype operator/(const etype &var);

  bool operator==(const etype &var) const;

  etype &operator[](const etype &i);

  inline etype &operator+=(const etype &var) { return(*this=*this+var); }
  inline etype &operator-=(const etype &var) { return(*this=*this-var); }

  //////////////////////////
  inline etype &operator[](int i) { return((*this)[etype(i)]); }
  inline etype &operator[](float i) { return((*this)[etype(i)]); }
  inline etype &operator[](double i) { return((*this)[etype(i)]); }
  inline etype &operator[](char i) { return((*this)[etype(i)]); }
  inline etype &operator[](char *i) { return((*this)[etype(i)]); }
  inline etype &operator[](const estr &i) { return((*this)[etype(i)]); }

  inline bool operator==(int    var)        const { return(*this==etype(var)); }
  inline bool operator==(float  var)        const { return(*this==etype(var)); }
  inline bool operator==(const double &var) const { return(*this==etype(var)); }
  inline bool operator==(char var)          const { return(*this==etype(var)); }
  inline bool operator==(char *var)         const { return(*this==etype(var)); }
  inline bool operator==(const estr &var)   const { return(*this==etype(var)); }


  inline etype &operator=(const int    &var) { return(*this=etype(var)); }
  inline etype &operator=(const float  &var) { return(*this=etype(var)); }
  inline etype &operator=(const double &var) { return(*this=etype(var)); }

  inline etype operator-(int    var)  { return(*this-etype(var)); }
  inline etype operator-(float  var)  { return(*this-etype(var)); }
  inline etype operator-(double var)  { return(*this-etype(var)); }

  inline etype operator*(int    var)  { return(*this*etype(var)); }
  inline etype operator*(float  var)  { return(*this*etype(var)); }
  inline etype operator*(double var)  { return(*this*etype(var)); }

  inline etype operator/(int    var)  { return(*this/etype(var)); }
  inline etype operator/(float  var)  { return(*this/etype(var)); }
  inline etype operator/(double var)  { return(*this/etype(var)); }

  inline etype &operator-=(const int    &var) { return(*this=*this+etype(var)); }
  inline etype &operator-=(const float  &var) { return(*this=*this+etype(var)); }
  inline etype &operator-=(const double &var) { return(*this=*this+etype(var)); }

  inline etype operator+(int    var)  { return(*this+etype(var)); }
  inline etype operator+(float  var)  { return(*this+etype(var)); }
  inline etype operator+(double var)  { return(*this+etype(var)); }

  inline etype &operator+=(const int    &var) { return(*this=*this+etype(var)); }
  inline etype &operator+=(const float  &var) { return(*this=*this+etype(var)); }
  inline etype &operator+=(const double &var) { return(*this=*this+etype(var)); }

  inline etype &operator++()           { return(*this=*this+etype(1)); }
  inline etype &operator--()           { return(*this=*this-etype(1)); }
  inline etype &operator++(int)      { return(*this=*this+etype(1)); }
  inline etype &operator--(int)      { return(*this=*this-etype(1)); }

  /////////////////////////
  inline etype &operator=(const estrarray &var)  { return(*this=etype(var)); }
  inline etype  operator+(const estrarray &var)  { return(*this+etype(var)); }
  inline etype &operator+=(const estrarray &var) { return(*this=*this+etype(var)); }

  ////////////////////////
  inline etype &operator=(const char   &var)  { return(*this=etype(var)); }
  inline etype &operator=(char*  var)  { return(*this=etype(var)); }
  inline etype &operator=(const estr   &var)  { return(*this=etype(var)); }

  inline etype operator+(char   var)  { return(*this+etype(var)); }
  inline etype operator+(char  *var)  { return(*this+etype(var)); }
  inline etype operator+(estr   var)  { return(*this+etype(var)); }

  inline etype &operator+=(const char   &var) { return(*this=*this+etype(var)); }
  inline etype &operator+=(char*  var) { return(*this=*this+etype(var)); }
  inline etype &operator+=(const estr   &var) { return(*this=*this+etype(var)); }

  int    i() const;
  float  f() const;
  double d() const;
  char   c() const;
  estr   s() const;
  estrarray a() const;
  inline char  *p() { return(s()._str); }

  friend ostream &operator<<(ostream &stream,const etype &var);
};


//int operator=(int &a,etype &var)
etype operator+(int a,const etype &var);
etype operator-(int a,const etype &var);
etype operator*(int a,const etype &var);
etype operator/(int a,const etype &var);


//float operator=(float &a,etype &var)
etype operator+(float a,const etype &var);
etype operator-(float a,const etype &var);
etype operator*(float a,const etype &var);
etype operator/(float a,const etype &var);

//double operator=(double &a,etype &var)
etype operator+(double a,const etype &var);
etype operator-(double a,const etype &var);
etype operator*(double a,const etype &var);
etype operator/(double a,const etype &var);

//estr operator=(estr &a,etype &var)
etype operator+(const estr &a,const etype &var);

etype operator+(char *a,const etype &var);

etype operator+(const estrarray &a,const etype &var);

ostream &operator<<(ostream &stream,const etype &var);


//etype load(etype filename);

#endif

