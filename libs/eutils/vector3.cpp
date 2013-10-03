#include "vector3.h"

#include "auxmath.h"
#include "matrix3.h"

const evector3 ux3(1.0,0.0,0.0);
const evector3 uy3(0.0,1.0,0.0);
const evector3 uz3(0.0,0.0,1.0);
const evector3 u03(0.0,0.0,0.0);

std::ostream &operator<<(std::ostream &stream,const evector3 &vec)
{
  stream << "("<<vec.x<<","<<vec.y<<","<<vec.z<<")";
  return(stream);
}


evector3::evector3(): x(0.0), y(0.0), z(0.0)
{
}

evector3::evector3(const evector3& a): x(a.x),y(a.y),z(a.z)
{
}

evector3::evector3(float fx, float fy, float fz): x(fx),y(fy),z(fz)
{
}

evector3::evector3(double fx, double fy, double fz): x(fx),y(fy),z(fz)
{
}

double evector3::len() const
{
  return(sqrt(x*x+y*y+z*z));
}

void evector3::normalize()
{
  double l = 1.0/len();
  x = x*l;
  y = y*l;
  z = z*l;   
}  

evector3 evector3::unit() const
{
  double l = 1.0/len();
  return(evector3(x*l,y*l,z*l));
}

evector3 evector3::proj(const evector3& a) const
{
  return((*this)*((*this)*a));
}

double evector3::operator *(const evector3& a) const
{
  return(a.x*x + a.y*y + a.z*z);
}

evector3 evector3::operator ^(const evector3& a) const
{
  return(evector3(a.y*z-a.z*y , a.z*x-a.x*z  , a.x*y-a.y*x));
}

evector3 evector3::operator +(const evector3& a) const
{
  return(evector3(x+a.x,y+a.y,z+a.z));
}

evector3 evector3::operator -(const evector3& a) const
{
  return(evector3(x-a.x,y-a.y,z-a.z));
}

evector3 evector3::operator *(double a) const
{
  return(evector3(x*a,y*a,z*a));
}

evector3 evector3::operator /(double a) const
{
  double t = 1.0/a;
  return(evector3(x*t,y*t,z*t));
}

evector3& evector3::operator =(const evector3& a)
{
  x=a.x; y=a.y; z=a.z; return(*this);
}

evector3& evector3::operator +=(const evector3& a)
{
  x+=a.x;
  y+=a.y;
  z+=a.z;
  return(*this);
}

evector3& evector3::operator -=(const evector3& a)
{
  x-=a.x;
  y-=a.y;
  z-=a.z;
  return(*this);
}


evector3 evector3::eulerRot(double psi, double phi, double teta) const
{
  return(mEuler(psi,phi,teta)*(*this));
}

evector3 evector3::rot(double ang,const evector3& vec) const
{
  return(mRot(ang,vec)*(*this));
}

//const float DEGRAD = MPI/180.0;

evector3 evector3::rotx(double ang) const
{
  return(mRotX(ang)*(*this)); 
}  

evector3 evector3::roty(double ang) const
{
  return(mRotY(ang)*(*this)); 
}  

evector3 evector3::rotz(double ang) const
{
  return(mRotZ(ang)*(*this)); 
}  


evector3 polar3(const evector3& rot, double len)
{
  return(mRotY(rot.x)*mRotX(rot.y)*mRotZ(rot.z)*evector3(0.0,0.0,-len));
}  

evector3 polar3(double r, double teta, double phi)
{
  return(mRotZ(phi)*mRotX(teta)*evector3(0.0,0.0,-r)); 
}  

