#ifndef EMATRIX4_H
#define EMATRIX4_H

#include "eutils.h"

//#include "vector3.h"

class ematrix4
{
 public:
  float x[16];
  ematrix4();
  ematrix4(float x0,float x1,float x2,float x3,float x4,float x5,float x6,float x7,float x8,float x9,float x10,float x11,float x12,float x13,float x14,float x15);
  ematrix4(float *x);
  ematrix4(double *x);
  ematrix4(const ematrix4& matrix);

  ~ematrix4();
  
//  float det() const;
//  ematrix4 unit() const;
  
  ematrix4 operator*(float f) const;
  ematrix4 operator/(float f) const;

//  evector4 operator*(const evector4& v) const;
  
  ematrix4 operator*(const ematrix4& m) const;
  ematrix4 operator+(const ematrix4& m) const;
  ematrix4 operator-(const ematrix4& m) const;
  
  ematrix4& operator=(const ematrix4& m);  
};

#include <iostream>

using namespace std;

ostream& operator<<(ostream& stream,const ematrix4& m);

#endif

