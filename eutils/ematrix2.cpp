
#include "ematrix2.h"

#include "auxmath.h"
#include "logger.h"


ematrix2::ematrix2()
{
  x[0]=0.0,x[1]=0.0,x[2]=0.0,x[3]=0.0;
}

ematrix2::ematrix2(float x0,float x1,float x2,float x3)
{
  x[0]=x0,x[1]=x1,x[2]=x2,x[3]=x3;
}

ematrix2::~ematrix2()
{
}  
  
float ematrix2::det()
{
  return( x[0]*x[3] - x[1]*x[2] );
}  
  
ematrix2 ematrix2::unit()
{
  return(*this/det());
}  

ematrix2 ematrix2::operator*(float f)
{
  return(ematrix2(x[0]*f,x[1]*f,x[2]*f,x[3]*f)); 
}
  
ematrix2 ematrix2::operator/(float f)
{
  f=1.0/f;  
  return(ematrix2(x[0]*f,x[1]*f,x[2]*f,x[3]*f)); 
}  

float& ematrix2::operator()(int i,int j)
{
  lddieif(i<0 || i>=2 || j<0 || j>=2,"element out of bounds: "+estr(i)+","+estr(j));
  return(x[i*2+j]);
}

evector2 ematrix2::operator*(evector2 v)
{
  return(evector2(x[0]*v.x + x[1]*v.y , x[2]*v.x + x[3]*v.y));
}  
  
ematrix2 ematrix2::operator*(ematrix2 m)
{
  return(ematrix2(x[0]*m.x[0] + x[1]*m.x[2], x[0]*m.x[1] + x[1]*m.x[3], x[2]*m.x[0] + x[3]*m.x[2], x[2]*m.x[1] + x[3]*m.x[3]));
}
  
ematrix2 ematrix2::operator+(ematrix2 m)
{
  return(ematrix2(x[0]+m.x[0],x[1]+m.x[1],x[2]+m.x[2],x[3]+m.x[3]));
}
  
ematrix2 ematrix2::operator-(ematrix2 m)
{
  return(ematrix2(x[0]-m.x[0],x[1]-m.x[1],x[2]-m.x[2],x[3]-m.x[3]));
}  
  
ematrix2 ematrix2::operator=(ematrix2 m)
{
  x[0]=m.x[0],x[1]=m.x[1],x[2]=m.x[2],x[3]=m.x[3];
  return(*this);
}


ematrix2 m2Rot(float ang)
{
  float c,s;
  c=cos(ang),s=sin(ang);
  return(ematrix2(c,s,-s,c));  
}  

