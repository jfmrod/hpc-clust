#ifndef EMPINT_H
#define EMPINT_H

#include "eutils.h"

#include <gmp.h>
#include <ostream>

#ifdef EUTILS_HAVE_MPFRLIB
 class empfloatr;
#endif

using namespace std;

class empfloat;
class empint;

class emprnd
{
 public:
  gmp_randstate_t state;
  emprnd();
  ~emprnd();

  empint uniform(const empint& mpint);
};

class empint
{
 public:
  mpz_t num;

  empint();
  empint(unsigned int vui);
  empint(int vi);
  empint(const empint& mpint);
  empint(const empfloat& mpfloat);
  ~empint();

#ifdef EUTILS_HAVE_MPFRLIB
  empint(const empfloatr& mpfloatr);
  empint& operator=(const empfloatr& mpfloatr);
#endif

  unsigned int& get(unsigned int& a) const;

  empint& sqrt();
  empint& sqrt(empint& rest);
  empint abs() const;

  empint& operator=(unsigned int vui);
  empint& operator=(const empint& vmp);
  empint& operator-=(unsigned int vui);
  empint& operator-=(const empint& vmp);
  empint& operator+=(unsigned int vui);
  empint& operator+=(const empint& vmp);
  empint& multiply(unsigned int vui);
  empint& multiply(const empint& vmp);
  empint operator*(unsigned int vui) const;
  empint operator*(const empint& vmp) const;
  empint operator+(unsigned int vui) const;
  empint operator+(const empint& vmp) const;
  empint operator-(unsigned int vui) const;
  empint operator-(const empint& vmp) const;
  empint operator/(unsigned int vui) const;
  empint operator/(const empint& vmp) const;
  empint operator%(unsigned int vui) const;
  empint operator%(const empint& vmp) const;
  bool operator==(unsigned int vui);
  bool operator==(const empint& vmp);
  bool operator!=(unsigned int vui);
  bool operator!=(const empint& vmp);
  bool operator<(unsigned int vui);
  bool operator<(const empint& vmp);
  bool operator<=(unsigned int vui);
  bool operator<=(const empint& vmp);
  bool operator>(unsigned int vui);
  bool operator>=(const empint& vmp);
  bool operator>=(unsigned int vui);
  bool operator>(const empint& vmp);
};

empint gcd(const empint& a,const empint& b);
empint root(const empint& n,empint& rem,unsigned int root);
empint sqrt(const empint& n,empint& rem);
empint pow(const empint& a,unsigned int n);

ostream& operator<<(ostream &stream,const empint& mpint);

class empfloat
{
 public:
  mpf_t num;

  empfloat();
  empfloat(unsigned int vui);
  empfloat(int vi);
  empfloat(double vui);
  empfloat(const empfloat& mpfloat);
  empfloat(const empint& mpint);
  ~empfloat();

#ifdef EUTILS_HAVE_MPFRLIB
  empfloat(const empfloatr& mpfloatr);
  empfloat& operator=(const empfloatr& mpfloatr);
#endif

  empfloat& sqrt();
  empfloat abs() const;

  empfloat& operator=(double vui);
  empfloat& operator=(const empfloat& vmp);
  empfloat& operator-=(const empfloat& vmp);
  empfloat& operator+=(const empfloat& vmp);
  empfloat& multiply(const empfloat& vmp);

  empfloat operator*(double vui) const;
  empfloat operator*(const empfloat& vmp) const;
  empfloat operator+(const empfloat& vmp) const;
  empfloat operator-(const empfloat& vmp) const;
  empfloat operator/(const empfloat& vmp) const;
  empfloat operator%(unsigned int vui) const;
  empfloat operator%(const empfloat& vmp) const;
  bool operator==(double vui);
  bool operator==(const empfloat& vmp);
  bool operator!=(double vui);
  bool operator!=(const empfloat& vmp);
  bool operator<(double vui);
  bool operator<(const empfloat& vmp);
  bool operator<=(double vui);
  bool operator<=(const empfloat& vmp);
  bool operator>(double vui);
  bool operator>=(const empfloat& vmp);
  bool operator>=(double vui);
  bool operator>(const empfloat& vmp);
};

inline empfloat operator+(double v,const empfloat& vmp){ return(vmp+v); }
inline empfloat operator-(double v,const empfloat& vmp){ return(empfloat(v)-vmp); }
inline empfloat operator*(double v,const empfloat& vmp){ return(vmp*v); }
inline empfloat operator/(double v,const empfloat& vmp){ return(empfloat(v)/vmp); }

inline empfloat operator+(unsigned int v,const empfloat& vmp){ return(vmp+v); }
inline empfloat operator-(unsigned int v,const empfloat& vmp){ return(empfloat(v)-vmp); }
inline empfloat operator*(unsigned int v,const empfloat& vmp){ return(vmp*v); }
inline empfloat operator/(unsigned int v,const empfloat& vmp){ return(empfloat(v)/vmp); }


ostream& operator<<(ostream &stream,const empfloat& mpfloat);

#endif

