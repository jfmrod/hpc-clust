
#include "matrix3.h"

#include "auxmath.h"


ematrix3::ematrix3()
{
  x[0]=0.0,x[1]=0.0,x[2]=0.0,x[3]=0.0,x[4]=0.0,x[5]=0.0,x[6]=0.0,x[7]=0.0,x[8]=0.0;
}

ematrix3::ematrix3(float x0,float x1,float x2,float x3,float x4,float x5,float x6,float x7,float x8)
{
  x[0]=x0,x[1]=x1,x[2]=x2,x[3]=x3,x[4]=x4,x[5]=x5,x[6]=x6,x[7]=x7,x[8]=x8;
}

ematrix3::~ematrix3()
{
}  
  
float ematrix3::det() const
{
  return( x[0]*x[4]*x[8] + x[1]*x[5]*x[6] + x[3]*x[7]*x[2] - x[2]*x[4]*x[6] - x[1]*x[3]*x[8] - x[5]*x[7]*x[0] );
}  
  
ematrix3 ematrix3::unit() const
{
  return(*this/det());
}  

ematrix3 ematrix3::operator*(float f) const
{
  return(ematrix3(x[0]*f,x[1]*f,x[2]*f,x[3]*f,x[4]*f,x[5]*f,x[6]*f,x[7]*f,x[8]*f)); 
}
  
ematrix3 ematrix3::operator/(float f) const
{
  f=1.0/f;  
  return(ematrix3(x[0]*f,x[1]*f,x[2]*f,x[3]*f,x[4]*f,x[5]*f,x[6]*f,x[7]*f,x[8]*f)); 
}  

evector3 ematrix3::operator*(const evector3& v) const
{
  return(evector3(x[0]*v.x + x[1]*v.y + x[2]*v.z,
                  x[3]*v.x + x[4]*v.y + x[5]*v.z,
                  x[6]*v.x + x[7]*v.y + x[8]*v.z));
}  
  
ematrix3 ematrix3::operator*(const ematrix3& m) const
{
  return(ematrix3(x[0]*m.x[0] + x[1]*m.x[3] + x[2]*m.x[6], x[0]*m.x[1] + x[1]*m.x[4] + x[2]*m.x[7], x[0]*m.x[2] + x[1]*m.x[5] + x[2]*m.x[8],
                  x[3]*m.x[0] + x[4]*m.x[3] + x[5]*m.x[6], x[3]*m.x[1] + x[4]*m.x[4] + x[5]*m.x[7], x[3]*m.x[2] + x[4]*m.x[5] + x[5]*m.x[8],
                  x[6]*m.x[0] + x[7]*m.x[3] + x[8]*m.x[6], x[6]*m.x[1] + x[7]*m.x[4] + x[8]*m.x[7], x[6]*m.x[2] + x[7]*m.x[5] + x[8]*m.x[8]));
                  
}
  
ematrix3 ematrix3::operator+(const ematrix3& m) const
{
  return(ematrix3(x[0]+m.x[0],x[1]+m.x[1],x[2]+m.x[2],x[3]+m.x[3],x[4]+m.x[4],x[5]+m.x[5],x[6]+m.x[6],x[7]+m.x[7],x[8]+m.x[8]));
}
  
ematrix3 ematrix3::operator-(const ematrix3& m) const
{
  return(ematrix3(x[0]-m.x[0],x[1]-m.x[1],x[2]-m.x[2],x[3]-m.x[3],x[4]-m.x[4],x[5]-m.x[5],x[6]-m.x[6],x[7]-m.x[7],x[8]-m.x[8]));
}  
  
ematrix3& ematrix3::operator=(const ematrix3& m)
{
  x[0]=m.x[0],x[1]=m.x[1],x[2]=m.x[2],x[3]=m.x[3],x[4]=m.x[4],x[5]=m.x[5],x[6]=m.x[6],x[7]=m.x[7],x[8]=m.x[8];
  return(*this);
}


ematrix3 mEuler(float phi, float psi, float teta)
{
  float c1,s1,c2,s2,c3,s3;
  c1=cos(phi),s1=sin(phi);
  c2=cos(psi),s2=sin(psi);
  c3=cos(teta),s3=sin(teta);
  
  ematrix3 m1(c3,s3,0.0,-s3,c3,0.0,0.0,0.0,1.0);
  ematrix3 m2(c2,0.0,-s2,0.0,1.0,0.0,s2,0.0,c2);
  ematrix3 m3(c1,s1,0.0,-s1,c1,0.0,0.0,0.0,1.0);
  
  return(m3*m2*m1);
}

ematrix3 mRotX(float ang)
{
  float c,s;
  c=cos(ang),s=sin(ang);
  return(ematrix3(1.0,0.0,0.0,0.0,c,s,0.0,-s,c));  
}  

ematrix3 mRotY(float ang)
{
  float c,s;
  c=cos(ang),s=sin(ang);
  return(ematrix3(c,0.0,-s,0.0,1.0,0.0,s,0.0,c));  
}  

ematrix3 mRotZ(float ang)
{
  float c,s;
  c=cos(ang),s=sin(ang);
  return(ematrix3(c,s,0.0,-s,c,0.0,0.0,0.0,1.0));  
}  

ematrix3 mRot(float ang, evector3 vec)
{
//  return(mRotX(ang)*vec.x+mRotY(ang)*vec.y+mRotZ(ang)*vec.z); 
  return(ematrix3( 1.0 + ( 1.0 - cos(ang))*(vec.x*vec.x-1), -vec.z * sin(ang) + (1.0-cos(ang))*vec.x*vec.y, vec.y*sin(ang) + (1.0-cos(ang))*vec.x*vec.z,
          vec.z*sin(ang) + (1.0-cos(ang))*vec.x*vec.y         , 1.0 + (1.0-cos(ang))*(vec.y*vec.y-1.0)    ,-vec.x*sin(ang) + (1.0-cos(ang))*vec.y*vec.z,
          -vec.y*sin(ang) + (1.0-cos(ang))*vec.x*vec.z       , vec.x*sin(ang) + (1.0-cos(ang))*vec.y*vec.z   , 1.0+(1.0-cos(ang))*(vec.z*vec.z-1)));

}


