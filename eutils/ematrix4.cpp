#include "ematrix4.h"

#include "auxmath.h"


ostream& operator<<(ostream& stream,const ematrix4& m)
{
  stream << "[ " << m.x[0] << " " << m.x[1] << " " << m.x[2] << " " << m.x[3] << " ]" << endl;
  stream << "[ " << m.x[4] << " " << m.x[5] << " " << m.x[6] << " " << m.x[7] << " ]" << endl;
  stream << "[ " << m.x[8] << " " << m.x[9] << " " << m.x[10] << " " << m.x[11] << " ]" << endl;
  stream << "[ " << m.x[12] << " " << m.x[13] << " " << m.x[14] << " " << m.x[15] << " ]" << endl;
  return(stream);
}

ematrix4::ematrix4()
{
  x[0]=0.0,x[1]=0.0,x[2]=0.0,x[3]=0.0,x[4]=0.0,x[5]=0.0,x[6]=0.0,x[7]=0.0,x[8]=0.0,x[9]=0.0,x[10]=0.0,x[11]=0.0,x[12]=0.0,x[13]=0.0,x[14]=0.0,x[15]=0.0;
}

ematrix4::ematrix4(const ematrix4& m){
  x[0]=m.x[0],x[1]=m.x[1],x[2]=m.x[2],x[3]=m.x[3],x[4]=m.x[4],x[5]=m.x[5],x[6]=m.x[6],x[7]=m.x[7],x[8]=m.x[8],x[9]=m.x[9],x[10]=m.x[10],x[11]=m.x[11],x[12]=m.x[12],x[13]=m.x[13],x[14]=m.x[14],x[15]=m.x[15];
}

ematrix4::ematrix4(float *_x){
  x[0]=_x[0],x[1]=_x[1],x[2]=_x[2],x[3]=_x[3],x[4]=_x[4],x[5]=_x[5],x[6]=_x[6],x[7]=_x[7],x[8]=_x[8],x[9]=_x[9],x[10]=_x[10],x[11]=_x[11],x[12]=_x[12],x[13]=_x[13],x[14]=_x[14],x[15]=_x[15];
}

ematrix4::ematrix4(double *_x){
  x[0]=_x[0],x[1]=_x[1],x[2]=_x[2],x[3]=_x[3],x[4]=_x[4],x[5]=_x[5],x[6]=_x[6],x[7]=_x[7],x[8]=_x[8],x[9]=_x[9],x[10]=_x[10],x[11]=_x[11],x[12]=_x[12],x[13]=_x[13],x[14]=_x[14],x[15]=_x[15];
}


ematrix4::ematrix4(float x0,float x1,float x2,float x3,float x4,float x5,float x6,float x7,float x8,float x9,float x10,float x11,float x12,float x13,float x14,float x15)
{
  x[0]=x0,x[1]=x1,x[2]=x2,x[3]=x3,x[4]=x4,x[5]=x5,x[6]=x6,x[7]=x7,x[8]=x8,x[9]=x9,x[10]=x10,x[11]=x11,x[12]=x12,x[13]=x13,x[14]=x14,x[15]=x15;
}

ematrix4::~ematrix4()
{
}  

ematrix4 ematrix4::operator*(float f) const
{
  return(ematrix4(x[0]*f,x[1]*f,x[2]*f,x[3]*f,x[4]*f,x[5]*f,x[6]*f,x[7]*f,x[8]*f,x[9]*f,x[10]*f,x[11]*f,x[12]*f,x[13]*f,x[14]*f,x[15]*f)); 
}
  
ematrix4 ematrix4::operator/(float f) const
{
  f=1.0/f;  
  return(ematrix4(x[0]*f,x[1]*f,x[2]*f,x[3]*f,x[4]*f,x[5]*f,x[6]*f,x[7]*f,x[8]*f,x[9]*f,x[10]*f,x[11]*f,x[12]*f,x[13]*f,x[14]*f,x[15]*f)); 
}  

/*
evector3 ematrix4::operator*(const evector3& v) const
{
  return(evector3(x[0]*v.x + x[1]*v.y + x[2]*v.z,
                  x[3]*v.x + x[4]*v.y + x[5]*v.z,
                  x[6]*v.x + x[7]*v.y + x[8]*v.z));
}  
*/

ematrix4 ematrix4::operator*(const ematrix4& m) const
{
  return(ematrix4(x[0]*m.x[0] + x[1]*m.x[4] + x[2]*m.x[8] + x[3]*m.x[12], x[0]*m.x[1] + x[1]*m.x[5] + x[2]*m.x[9] + x[3]*m.x[13], x[0]*m.x[2] + x[1]*m.x[6] + x[2]*m.x[10] + x[3]*m.x[14], x[0]*m.x[3] + x[1]*m.x[7] + x[2]*m.x[11] + x[3]*m.x[15], 
                  x[4]*m.x[0] + x[5]*m.x[4] + x[6]*m.x[8] + x[7]*m.x[12], x[4]*m.x[1] + x[5]*m.x[5] + x[6]*m.x[9] + x[7]*m.x[13], x[4]*m.x[2] + x[5]*m.x[6] + x[6]*m.x[10] + x[7]*m.x[14], x[4]*m.x[3] + x[5]*m.x[7] + x[6]*m.x[11] + x[7]*m.x[15], 
                  x[8]*m.x[0] + x[9]*m.x[4] + x[10]*m.x[8] + x[11]*m.x[12], x[8]*m.x[1] + x[9]*m.x[5] + x[10]*m.x[9] + x[11]*m.x[13], x[8]*m.x[2] + x[9]*m.x[6] + x[10]*m.x[10] + x[11]*m.x[14], x[8]*m.x[3] + x[9]*m.x[7] + x[10]*m.x[11] + x[11]*m.x[15], 
                  x[12]*m.x[0] + x[13]*m.x[4] + x[14]*m.x[8] + x[15]*m.x[12], x[12]*m.x[1] + x[13]*m.x[5] + x[14]*m.x[9] + x[15]*m.x[13], x[12]*m.x[2] + x[13]*m.x[6] + x[14]*m.x[10] + x[15]*m.x[14], x[12]*m.x[3] + x[13]*m.x[7] + x[14]*m.x[11] + x[15]*m.x[15]));
}
  
ematrix4 ematrix4::operator+(const ematrix4& m) const
{
  return(ematrix4(x[0]+m.x[0],x[1]+m.x[1],x[2]+m.x[2],x[3]+m.x[3],x[4]+m.x[4],x[5]+m.x[5],x[6]+m.x[6],x[7]+m.x[7],x[8]+m.x[8],x[9]+m.x[9],x[10]+m.x[10],x[11]+m.x[11],x[12]+m.x[12],x[13]+m.x[13],x[14]+m.x[14],x[15]+m.x[15]));
}
  
ematrix4 ematrix4::operator-(const ematrix4& m) const
{
  return(ematrix4(x[0]-m.x[0],x[1]-m.x[1],x[2]-m.x[2],x[3]-m.x[3],x[4]-m.x[4],x[5]-m.x[5],x[6]-m.x[6],x[7]-m.x[7],x[8]-m.x[8],x[9]-m.x[9],x[10]-m.x[10],x[11]-m.x[11],x[12]-m.x[12],x[13]-m.x[13],x[14]-m.x[14],x[15]-m.x[15]));
}  
  
ematrix4& ematrix4::operator=(const ematrix4& m)
{
  x[0]=m.x[0],x[1]=m.x[1],x[2]=m.x[2],x[3]=m.x[3],x[4]=m.x[4],x[5]=m.x[5],x[6]=m.x[6],x[7]=m.x[7],x[8]=m.x[8],x[9]=m.x[9],x[10]=m.x[10],x[11]=m.x[11],x[12]=m.x[12],x[13]=m.x[13],x[14]=m.x[14],x[15]=m.x[15];
  return(*this);
}


