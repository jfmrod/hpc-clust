#ifndef EMPFLOATR_H
#define EMPFLOATR_H

#include "eutils.h"
#include "empint.h"

#include <mpfr.h>
#include <gmp.h>
#include <ostream>

using namespace std;

class empfloatr
{
 public:
  mpfr_t num;

  empfloatr();
  empfloatr(unsigned int vui);
  empfloatr(int vi);
  empfloatr(double vui);
  empfloatr(const empfloatr& mpfloat);
  empfloatr(const empfloat& mpfloat);
  empfloatr(const empint& mpint);
  ~empfloatr();

  empfloatr& sqrt();
  empfloatr& log();
  empfloatr abs() const;

  empfloatr& operator=(double vui);
  empfloatr& operator=(const empfloatr& vmp);
  empfloatr& operator=(const empfloat& vmp);
  empfloatr& operator=(const empint& vmp);
  empfloatr& operator-=(const empfloatr& vmp);
  empfloatr& operator+=(const empfloatr& vmp);
  empfloatr& multiply(const empfloatr& vmp);

  empfloatr operator*(const empfloatr& vmp) const;
  empfloatr operator+(const empfloatr& vmp) const;
  empfloatr operator-(const empfloatr& vmp) const;
  empfloatr operator/(const empfloatr& vmp) const;
  empfloatr operator%(unsigned int vui) const;
  empfloatr operator%(const empfloatr& vmp) const;
  bool operator==(double vui);
  bool operator==(const empfloatr& vmp);
  bool operator!=(double vui);
  bool operator!=(const empfloatr& vmp);
  bool operator<(double vui);
  bool operator<(const empfloatr& vmp);
  bool operator<=(double vui);
  bool operator<=(const empfloatr& vmp);
  bool operator>(double vui);
  bool operator>=(const empfloatr& vmp);
  bool operator>=(double vui);
  bool operator>(const empfloatr& vmp);
};

inline empfloatr operator+(double v,const empfloatr& vmp){ return(vmp+v); }
inline empfloatr operator-(double v,const empfloatr& vmp){ return(empfloatr(v)-vmp); }
inline empfloatr operator*(double v,const empfloatr& vmp){ return(vmp*v); }
inline empfloatr operator/(double v,const empfloatr& vmp){ return(empfloatr(v)/vmp); }

inline empfloatr operator+(unsigned int v,const empfloatr& vmp){ return(vmp+v); }
inline empfloatr operator-(unsigned int v,const empfloatr& vmp){ return(empfloatr(v)-vmp); }
inline empfloatr operator*(unsigned int v,const empfloatr& vmp){ return(vmp*v); }
inline empfloatr operator/(unsigned int v,const empfloatr& vmp){ return(empfloatr(v)/vmp); }


ostream& operator<<(ostream &stream,const empfloatr& mpfloat);

#endif

