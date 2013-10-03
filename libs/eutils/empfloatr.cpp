#include "empfloatr.h"

empfloatr::empfloatr()
{
  mpfr_init(num);
}
empfloatr::empfloatr(const empfloatr& mpfloat)
{
  mpfr_init_set(num,mpfloat.num,MPFR_RNDZ);
}
empfloatr::empfloatr(const empfloat& mpfloat)
{
  mpfr_init_set_f(num,mpfloat.num,MPFR_RNDZ);
}
empfloatr::empfloatr(const empint& mpint)
{
  mpfr_init(num);
  mpfr_set_z(num,mpint.num,MPFR_RNDZ);
}
empfloatr::empfloatr(unsigned int ui)
{
  mpfr_init_set_ui(num,ui,MPFR_RNDZ);
}
empfloatr::empfloatr(int i)
{
  mpfr_init_set_si(num,i,MPFR_RNDZ);
}
empfloatr::empfloatr(double v)
{
  mpfr_init_set_d(num,v,MPFR_RNDZ);
}
empfloatr::~empfloatr()
{
  mpfr_clear(num);
}

empfloatr& empfloatr::operator=(double v)
{
  mpfr_set_d(num,v,MPFR_RNDZ);
  return(*this);
}
empfloatr& empfloatr::operator=(const empint& vmp)
{
  mpfr_set_z(num,vmp.num,MPFR_RNDZ);
  return(*this);
}
empfloatr& empfloatr::operator=(const empfloat& vmp)
{
  mpfr_set_f(num,vmp.num,MPFR_RNDZ);
  return(*this);
}
empfloatr& empfloatr::operator=(const empfloatr& vmp)
{
  mpfr_set(num,vmp.num,MPFR_RNDZ);
  return(*this);
}

empfloatr& empfloatr::operator-=(const empfloatr& vmp)
{
  mpfr_sub(num,num,vmp.num,MPFR_RNDZ);
  return(*this);
}

empfloatr& empfloatr::operator+=(const empfloatr& vmp)
{
  mpfr_add(num,num,vmp.num,MPFR_RNDZ);
  return(*this);
}

empfloatr& empfloatr::log()
{
  mpfr_log(num,num,MPFR_RNDZ);
  return(*this);
}
empfloatr& empfloatr::sqrt()
{
  mpfr_sqrt(num,num,MPFR_RNDZ);
  return(*this);
}
empfloatr empfloatr::abs() const
{
  empfloatr rval;
  mpfr_abs(rval.num,num,MPFR_RNDZ);
  return(rval);
}

empfloatr& empfloatr::multiply(const empfloatr& vmp)
{
  mpfr_mul(num,num,vmp.num,MPFR_RNDZ);
  return(*this);
}

empfloatr empfloatr::operator*(const empfloatr& vmp) const
{
  empfloatr rval;
  mpfr_mul(rval.num,num,vmp.num,MPFR_RNDZ);
  return(rval);
}
empfloatr empfloatr::operator+(const empfloatr& vmp) const
{
  empfloatr rval;
  mpfr_add(rval.num,num,vmp.num,MPFR_RNDZ);
  return(rval);
}
empfloatr empfloatr::operator-(const empfloatr& vmp) const
{
  empfloatr rval;
  mpfr_sub(rval.num,num,vmp.num,MPFR_RNDZ);
  return(rval);
}
empfloatr empfloatr::operator/(const empfloatr& vmp) const
{
  empfloatr rval;
  mpfr_div(rval.num,num,vmp.num,MPFR_RNDZ);
  return(rval);
}

bool empfloatr::operator==(double v)
{
  return(mpfr_cmp_d(num,v)==0);
}
bool empfloatr::operator==(const empfloatr& vmp)
{
  return(mpfr_cmp(num,vmp.num)==0);
}
bool empfloatr::operator!=(double v)
{
  return(mpfr_cmp_d(num,v)!=0);
}
bool empfloatr::operator!=(const empfloatr& vmp)
{
  return(mpfr_cmp(num,vmp.num)!=0);
}
bool empfloatr::operator<(double v)
{
  return(mpfr_cmp_d(num,v)<0);
}
bool empfloatr::operator<(const empfloatr& vmp)
{
  return(mpfr_cmp(num,vmp.num)<0);
}
bool empfloatr::operator>(double vui)
{
  return(mpfr_cmp_d(num,vui)>0);
}
bool empfloatr::operator>(const empfloatr& vmp)
{
  return(mpfr_cmp(num,vmp.num)>0);
}
bool empfloatr::operator<=(double vui)
{
  return(mpfr_cmp_d(num,vui)<=0);
}
bool empfloatr::operator<=(const empfloatr& vmp)
{
  return(mpfr_cmp(num,vmp.num)<=0);
}
bool empfloatr::operator>=(double vui)
{
  return(mpfr_cmp_d(num,vui)>=0);
}
bool empfloatr::operator>=(const empfloatr& vmp)
{
  return(mpfr_cmp(num,vmp.num)>=0);
}





ostream& operator<<(ostream &stream,const empfloatr& mpfloat)
{
  stream << mpfloat.num;

  return(stream);
}



