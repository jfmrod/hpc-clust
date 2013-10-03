#ifndef EVECTOR3_H
#define EVECTOR3_H

#include "eutils.h"

#include <iostream>

class evector3
{
 public:
  double x,y,z;

  evector3();
  evector3(const evector3& a);
  evector3(float x,float y,float z);
  evector3(double x,double y,double z);

  double len() const;

  double   operator *(const evector3& a) const;
  evector3 operator ^(const evector3& a) const;

  void normalize();
  evector3 unit() const;
//  evector3 rot(float angle);
  evector3 proj(const evector3& a) const;
  evector3 operator +(const evector3& a) const;
  evector3 operator -(const evector3& a) const;
  evector3& operator +=(const evector3& a);
  evector3& operator -=(const evector3& a);
  evector3& operator = (const evector3& a);

  evector3 operator *(double a) const;
  evector3 operator /(double a) const;

  evector3 eulerRot(double psi, double phi, double teta) const;
  evector3 rot(double ang,const evector3& vec) const;
  
  evector3 rotx(double ang) const;
  evector3 roty(double ang) const;
  evector3 rotz(double ang) const;

  friend std::ostream &operator<<(std::ostream &stream,const evector3 &vec);
  friend inline evector3 operator*(double v,const evector3 &vec){ return(vec*v); }
};

evector3 polar3(const evector3& rot, double len);
evector3 polar3(double r, double teta, double phi);

//evector3 polar3(float phi, float psi);
//evector3 polar3(float phi, float psi, float length);

extern const evector3 ux3;
extern const evector3 uy3;
extern const evector3 uz3;
extern const evector3 u03;

#endif

