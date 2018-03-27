#ifndef EMATRIX3_H
#define EMATRIX3_H

#include "eutils.h"

#include "vector3.h"

class ematrix3
{
 public:
  float x[9];
  ematrix3();
  ematrix3(float x0,float x1,float x2,float x3,float x4,float x5,float x6,float x7,float x8);

  ~ematrix3();
  
  float det() const;
  
  ematrix3 unit() const;
  
  ematrix3 operator*(float f) const;
  ematrix3 operator/(float f) const;

  evector3 operator*(const evector3& v) const;
  
  ematrix3 operator*(const ematrix3& m) const;
  ematrix3 operator+(const ematrix3& m) const;
  ematrix3 operator-(const ematrix3& m) const;
  
  ematrix3& operator=(const ematrix3& m);  
};

ematrix3 mEuler(float phi, float psi, float teta);

ematrix3 mRotX(float ang);
ematrix3 mRotY(float ang);
ematrix3 mRotZ(float ang);
ematrix3 mRot(float ang, evector3 vec);
  

#endif

