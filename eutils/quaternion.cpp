#include "quaternion.h"

#include "auxmath.h"

const equaternion qX(qCartesian(evector3(1.0,0.0,0.0)));
const equaternion qY(qCartesian(evector3(0.0,1.0,0.0)));
const equaternion qZ(qCartesian(evector3(0.0,0.0,1.0)));


ostream& operator<<(ostream& stream,const equaternion& q)
{
  stream << "[equaternion "<<q.w<<","<<q.x<<","<<q.y<<","<<q.z<<"]";
  return(stream);
}


equaternion::equaternion(const equaternion& q): x(q.x),y(q.y),z(q.z),w(q.w) {}

equaternion::equaternion(): w(1.0),x(0.0),y(0.0),z(0.0)
{
}

//const float DEGTORAD = MPI/180.0;
//const float RADTODEG = 180.0/MPI;

equaternion::equaternion(float angle,const evector3& vector)
{
  angle = angle/2.0;
  float s = sin(angle);
  w = cos(angle);
  x = s*vector.x;
  y = s*vector.y;
  z = s*vector.z;
//  normalize();
}

equaternion::equaternion(float fw, float fx, float fy, float fz): x(fx),y(fy),z(fz),w(fw)
{
}

float equaternion::len() const
{
  return(sqrt(x*x+y*y+z*z+w*w));
}

void equaternion::normalize()
{
  float l = 1.0/len();
  w = w*l;
  x = x*l;
  y = y*l;
  z = z*l;
}  

equaternion equaternion::unit() const
{
  float l = 1.0/len();
  return(equaternion(w*l,x*l,y*l,z*l));
}

equaternion equaternion::conj() const
{
  return(equaternion(w,-x,-y,-z));
}

equaternion& equaternion::rotate(float angle,const evector3& vector)
{
  *this = (*this) * equaternion(angle,vector);
//  normalize();
  return(*this);
}  

equaternion& equaternion::rotatef(float angle,float x, float y, float z)
{
  *this = (*this) * equaternion(angle,evector3(x,y,z));
//  normalize();
  return(*this);
}  


equaternion equaternion::operator *(const equaternion& a) const
{
  return(equaternion( w*a.w - x*a.x - y*a.y - z*a.z ,
                      w*a.x + x*a.w + y*a.z - z*a.y ,
                      w*a.y + y*a.w + z*a.x - x*a.z ,
                      w*a.z + z*a.w + x*a.y - y*a.x));
}

equaternion equaternion::operator +(const equaternion& a) const
{
  return(equaternion(w+a.w,x+a.x,y+a.y,z+a.z));
}

equaternion equaternion::operator -(const equaternion& a) const
{
  return(equaternion(w-a.w,x-a.x,y-a.y,z-a.z));
}

equaternion equaternion::operator *(float a) const
{
  return(equaternion(w*a,x*a,y*a,z*a));
}

equaternion equaternion::operator /(float a) const
{
  float t = 1.0/a;
  return(equaternion(w*t,x*t,y*t,z*t));
}

equaternion& equaternion::operator +=(const equaternion& a)
{
  x+=a.x;
  y+=a.y;
  z+=a.z;
  w+=a.w;
  return(*this);
}

equaternion& equaternion::operator -=(const equaternion& a)
{
  x-=a.x;
  y-=a.y;
  z-=a.z;
  w-=a.w;
  return(*this);
}



equaternion equaternion::operator *(const evector3& a) const
{
  return((*this)*equaternion(0.0,a.x,a.y,a.z));
}


evector3 equaternion::vec() const
{
  return(evector3(x,y,z));
}



evector3 equaternion::ux() const
{
  return(evector3(1.0 - 2.0*y*y - 2.0*z*z ,   2.0*x*y - 2.0*w*z   ,   2.0*x*z + 2.0*w*y ));
}  

evector3 equaternion::uy() const
{
  return(evector3( 2.0*x*y + 2.0*w*z , 1.0 - 2.0*x*x - 2.0*z*z ,    2.0*y*z - 2.0*w*x ));  
}  

evector3 equaternion::uz() const
{
  return(evector3( 2.0*x*z - 2.0*w*y , 2.0*y*z + 2.0*w*x , 1.0 - 2.0*x*x - 2.0*y*y));
}  

ematrix3 equaternion::mRotation() const
{
/*
  float xy,wz;
  float xz,wy;
  float yzwz;
  float x2,y2,z2;
*/  
/*
  xy = x*y; wz = w*z;
  
  xz = x*z; wy = w*y;
  
  yzwz = 2.0*(y*z - w*z);
  
  x2=x*x; y2=y*y; z2=z*z;
  
  
  return(ematrix3( 1.0 - 2.0*(y2 + z2) ,  2.0*( xy - wz )  ,  2.0*( xz + wy )  , 
                    2.0*( xy + wz )    , 1.0 - 2.0*(x2 + z2 ),   yzwz     ,
                    2.0*( xz - wy )    ,     yzwz      , 1.0 - 2.0 *(x2 + y2) ));
*/

//  return(ematrix3( x*x + w*w - y*y - z*z , 2.0*x*y - 2.0*w*z , 2.0*x*z + 2.0*y*w,
//                   2.0*x*y + 2.0*w*z , w*w + y*y - x*x - z*z , 2.0*y*z - 2.0*w*x,
//                   2.0*x*z - 2.0*y*w , 2.0*y*z + 2.0*w*x     , w*w + z*z - x*x - y*y ));

  return(ematrix3( 1.0 - 2.0*y*y - 2.0*z*z , 2.0*x*y - 2.0*w*z       , 2.0*x*z + 2.0*y*w,
                   2.0*x*y + 2.0*w*z       , 1.0 - 2.0*x*x - 2.0*z*z , 2.0*y*z - 2.0*w*x,
                   2.0*x*z - 2.0*y*w       , 2.0*y*z + 2.0*w*x       , 1.0 - 2.0*x*x - 2.0*y*y ));

}

evector3 equaternion::spherical() const
{
  float latitude, longitude;
  float cos_angle = w;
  float sin_angle = sqrt( 1.0 - cos_angle * cos_angle );
  float angle     = acos( cos_angle ) * 2.0;

  if ( fabs( sin_angle ) < 0.0005 )
      sin_angle = 1.0;
      
  sin_angle = 1.0/sin_angle;

  float tx,ty,tz; 
  tx = x * sin_angle;
  ty = y * sin_angle;
  tz = z * sin_angle;

  latitude = -asin( ty );

  if ( tx * tx + tz * tz < 0.0005 )
    longitude   = 0;
  else
    longitude  = atan2( tx, tz );

  if ( longitude < 0 )
    longitude += 360.0;
  return(evector3(longitude,latitude,angle));
}  

equaternion qSpherical(float lon, float lat, float angle)
{
  float sin_a = sin( angle / 2.0 );
  float cos_a = cos( angle / 2.0 );

  float sin_lat = sin( lat );
  float cos_lat = cos( lat );

  float sin_long = sin( lon );
  float cos_long = cos( lon );

  return(equaternion(cos_a,  sin_a * cos_lat * sin_long , sin_a * sin_lat, sin_a * sin_lat * cos_long ));
}

equaternion qCartesian(const evector3& vec)
{
  float rxz;
  float phi,teta;
  
  rxz = sqrt(vec.x*vec.x+vec.z*vec.z);
  
  phi = atan(vec.y/rxz);
  teta = atan2(vec.x,vec.z);
  
  return(equaternion() * equaternion(phi,ux3) * equaternion(-teta,uy3));
}  


