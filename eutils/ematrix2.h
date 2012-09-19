#ifndef EMATRIX2_H
#define EMATRIX2_H

#include "eutils.h"

#include "vector2.h"

class ematrix2
{
 public:
  float x[4];
  ematrix2();
  ematrix2(float x0,float x1,float x2,float x3);

  ~ematrix2();
  
  float det();
  
  ematrix2 unit();
  
  float& operator()(int i,int j);
  ematrix2 operator*(float f);
  ematrix2 operator/(float f);

  evector2 operator*(evector2 v);
  
  ematrix2 operator*(ematrix2 m);
  ematrix2 operator+(ematrix2 m);
  ematrix2 operator-(ematrix2 m);
  
  ematrix2 operator=(ematrix2 m);  
};

ematrix2 m2Rot(float ang);

#endif

