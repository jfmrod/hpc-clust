#ifndef EQUATERNION_H
#define EQUATERNION_H

#include "eutils.h"

#include "vector3.h"
#include "matrix3.h"
#include <ostream>

using namespace std;

class equaternion
{
 public:
  float x,y,z,w;

  equaternion();
  equaternion(const equaternion& quaternion);
  equaternion(float angle,const evector3& vector);
  equaternion(float x,float y, float z, float w);

  float len() const;
  
  equaternion& rotate(float angle,const evector3& vector);
  equaternion& rotatef(float angle,float x, float y, float z);

  evector3 ux() const;
  evector3 uy() const;
  evector3 uz() const;
  evector3 vec() const;

  equaternion operator *(const equaternion& a) const;

  void normalize();
  equaternion unit() const;
  equaternion conj() const;
  
  equaternion operator +(const equaternion& a) const;
  equaternion operator -(const equaternion& a) const;

  equaternion& operator +=(const equaternion& a);
  equaternion& operator -=(const equaternion& a);

  equaternion operator *(float a) const;
  equaternion operator /(float a) const;


  equaternion operator *(const evector3& a) const;

  ematrix3 mRotation() const;
  
  evector3 spherical() const;

  friend ostream& operator<<(ostream& stream,const equaternion& q);
};

ostream& operator<<(ostream& stream,const equaternion& q);

equaternion qSpherical(float lon, float lat, float angle);
equaternion qCartesian(const evector3& vec);

extern const equaternion qX;
extern const equaternion qY;
extern const equaternion qZ;

#endif

