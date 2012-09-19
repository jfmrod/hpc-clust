#ifndef EVECTOR2_H
#define EVECTOR2_H

#include "eutils.h"

#include <math.h>
#include <iostream>

using namespace std;

class evector2
{
 public:
  float x,y;

  evector2();
  evector2(float x,float y);
  evector2(const evector2 &vector);

  float len() const;

  float operator *(const evector2 &a) const;
  float operator ^(const evector2 &a) const;

  evector2 unit() const;
  evector2 perp() const;
  evector2 punit() const;
  evector2 rot(float angle) const; 
  evector2 proj(const evector2 &a) const;
  evector2 operator +(const evector2 &a) const;
  evector2 operator -(const evector2 &a) const;
  evector2 &operator +=(const evector2 &a);
  evector2 &operator -=(const evector2 &a);
  evector2 &operator =(const evector2 &a);

  evector2 operator -() const;

  evector2 operator *(float a) const;
  evector2 operator /(float a) const;
  
  float angle(const evector2 &a) const;
  
  bool operator ==(const evector2 &a) const;
  inline bool operator !=(const evector2 &a) const { return(!(*this==a)); }
  bool operator <(const evector2 &a) const;
  bool operator >(const evector2 &a) const;
  friend ostream& operator<<(ostream& stream,const evector2& vec);
};


evector2 polar(float angle);
evector2 polar(float angle, float length);

evector2 intersect(evector2 line, evector2 pos, evector2 line2);

extern const evector2 u02;
extern const evector2 ux2;
extern const evector2 uy2;

evector2 operator*(float f,const evector2 &v);
evector2 operator/(float f,const evector2 &v);

ostream& operator<<(ostream& stream,const evector2& vec);

#endif

