#include "vector2.h"
#include "auxmath.h"

const evector2 u02(0.0,0.0);
const evector2 ux2(1.0,0.0);
const evector2 uy2(0.0,1.0);

ostream& operator<<(ostream& stream,const evector2& vec)
{
  stream << "("<<vec.x<<","<<vec.y<<")";
  return(stream);
}

evector2::evector2()
{
}

evector2::evector2(float fx, float fy): x(fx),y(fy)
{
}

evector2::evector2(const evector2 &vec): x(vec.x),y(vec.y)
{
}


float evector2::len() const
{
  return(sqrt(x*x+y*y));
}

evector2 evector2::unit() const
{
  float l = len();
  return(evector2(x/l,y/l));
}

evector2 evector2::perp() const
{
  return(this->rot(MPI2));
}

evector2 evector2::punit() const
{
  return((this->rot(MPI2)).unit());
}

evector2 evector2::rot(float angle) const
{
  float c,s;
  c = cos(angle); s = sin(angle);
  return(evector2( x*c+y*s, -x*s+y*c));
}

evector2 evector2::proj(const evector2 &a) const
{
  return((*this)*((*this)*a));
}

float evector2::operator *(const evector2 &a) const
{
  return(a.x*x + a.y*y);
}

float evector2::operator ^(const evector2 &a) const
{
  return(a.x*y - a.y*x);
}

evector2 evector2::operator +(const evector2 &a) const
{
  return(evector2(x+a.x,y+a.y));
}

evector2 evector2::operator -(const evector2 &a) const
{
  return(evector2(x-a.x,y-a.y));
}

evector2 evector2::operator -() const
{
  return(evector2(-x,-y));
}

evector2 evector2::operator *(float a) const
{
  return(evector2(x*a,y*a));
}

evector2 evector2::operator /(float a) const
{
  float t = 1.0/a;
  return(evector2(x*t,y*t));
}

evector2 &evector2::operator =(const evector2 &a)
{
  x=a.x;
  y=a.y;
  return(*this);
}

evector2 &evector2::operator +=(const evector2 &a)
{
  x+=a.x;
  y+=a.y;
  return(*this);
}

evector2 &evector2::operator -=(const evector2 &a)
{
  x-=a.x;
  y-=a.y;
  return(*this);
}

bool evector2::operator ==(const evector2 &a) const
{
  return( x == a.x && y == a.y );
}

bool evector2::operator <(const evector2 &a) const
{
  return( fabs(x) < fabs(a.x) && fabs(y) < fabs(a.y) );
}

bool evector2::operator >(const evector2 &a) const
{
  return( fabs(x) > fabs(a.x) || fabs(y) > fabs(a.y) );
}



float evector2::angle(const evector2 &a) const
{
  return(atan2f(perp()*a,(*this)*a ));
}


evector2 polar(float angle)
{
  return(evector2(1.0,0.0).rot(angle));
}

evector2 polar(float angle, float length)
{
  return(evector2(length,0.0).rot(angle));
}


evector2 intersect(evector2 line, evector2 pos, evector2 line2)
{
  float cx,cy;
  float tx,ty;
  float rx;

  cx = line.unit()*line2;
  cy = line.punit()*line2;
  tx = line.unit()*pos;
  ty = line.punit()*pos;

  rx = -cx*ty/cy+tx;

  return(line.unit()*rx);
}

evector2 operator*(float f,const evector2 &v)
{
  return(evector2(v.x*f,v.y*f));
}

evector2 operator/(float f,const evector2 &v)
{
  return(evector2(v.x*f,v.y*f));
}


