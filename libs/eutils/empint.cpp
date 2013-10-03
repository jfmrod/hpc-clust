#include "empint.h"




emprnd::emprnd()
{
  gmp_randinit_mt(state);
}

emprnd::~emprnd()
{
  gmp_randclear(state);
}

empint emprnd::uniform(const empint& mpint)
{
  empint res;
  mpz_urandomm(res.num,state,mpint.num);
  return(res);
}

empint::empint()
{
  mpz_init(num);
}
empint::empint(const empint& mpint)
{
  mpz_init_set(num,mpint.num);
}
empint::empint(const empfloat& mpfloat)
{
  mpz_init(num);
  mpz_set_f(num,mpfloat.num);
}
empint::empint(unsigned int ui)
{
  mpz_init_set_ui(num,ui);
}
empint::empint(int i)
{
  mpz_init_set_si(num,i);
}
empint::~empint()
{
  mpz_clear(num);
}

#ifdef EUTILS_HAVE_MPFRLIB
#include "empfloatr.h"

empint::empint(const empfloatr& mpfloatr)
{
  mpz_init(num);
  mpfr_get_z(num,mpfloatr.num,MPFR_RNDZ);
}

empint& empint::operator=(const empfloatr& mpfloatr)
{
  mpfr_get_z(num,mpfloatr.num,MPFR_RNDZ);
  return(*this);
}
#endif


unsigned int& empint::get(unsigned int& a) const
{
  a=mpz_get_ui(num);
  return(a);
}







empint& empint::operator=(unsigned int vui)
{
  mpz_set_ui(num,vui);
  return(*this);
}
empint& empint::operator=(const empint& vmp)
{
  mpz_set(num,vmp.num);
  return(*this);
}

empint& empint::operator-=(unsigned int vui)
{
  mpz_sub_ui(num,num,vui);
  return(*this);
}
empint& empint::operator-=(const empint& vmp)
{
  mpz_sub(num,num,vmp.num);
  return(*this);
}

empint& empint::operator+=(unsigned int vui)
{
  mpz_add_ui(num,num,vui);
  return(*this);
}
empint& empint::operator+=(const empint& vmp)
{
  mpz_add(num,num,vmp.num);
  return(*this);
}

empint& empint::sqrt()
{
  mpz_sqrt(num,num);
  return(*this);
}
empint& empint::sqrt(empint& rest)
{
  mpz_sqrtrem(num,rest.num,num);
  return(*this);
}
empint empint::abs() const
{
  empint rval;
  mpz_abs(rval.num,num);
  return(rval);
}


empint& empint::multiply(unsigned int vui)
{
  mpz_mul_ui(num,num,vui);
  return(*this);
}
empint& empint::multiply(const empint& vmp)
{
  mpz_mul(num,num,vmp.num);
  return(*this);
}

empint empint::operator*(unsigned int vui) const
{
  empint rval;
  mpz_mul_ui(rval.num,num,vui);
  return(rval);
}
empint empint::operator*(const empint& vmp) const
{
  empint rval;
  mpz_mul(rval.num,num,vmp.num);
  return(rval);
}
empint empint::operator+(unsigned int vui) const
{
  empint rval;
  mpz_add_ui(rval.num,num,vui);
  return(rval);
}
empint empint::operator+(const empint& vmp) const
{
  empint rval;
  mpz_add(rval.num,num,vmp.num);
  return(rval);
}
empint empint::operator-(unsigned int vui) const
{
  empint rval;
  mpz_sub_ui(rval.num,num,vui);
  return(rval);
}
empint empint::operator-(const empint& vmp) const
{
  empint rval;
  mpz_sub(rval.num,num,vmp.num);
  return(rval);
}
empint empint::operator/(unsigned int vui) const
{
  empint rval;
  mpz_tdiv_q_ui(rval.num,num,vui);
  return(rval);
}
empint empint::operator/(const empint& vmp) const
{
  empint rval;
  mpz_tdiv_q(rval.num,num,vmp.num);
  return(rval);
}
empint empint::operator%(unsigned int vui) const
{
  empint rval;
  mpz_mod_ui(rval.num,num,vui);
  return(rval);
}
empint empint::operator%(const empint& vmp) const
{
  empint rval;
  mpz_mod(rval.num,num,vmp.num);
  return(rval);
}



bool empint::operator==(unsigned int vui)
{
  return(mpz_cmp_ui(num,vui)==0);
}
bool empint::operator==(const empint& vmp)
{
  return(mpz_cmp(num,vmp.num)==0);
}
bool empint::operator!=(unsigned int vui)
{
  return(mpz_cmp_ui(num,vui)!=0);
}
bool empint::operator!=(const empint& vmp)
{
  return(mpz_cmp(num,vmp.num)!=0);
}
bool empint::operator<(unsigned int vui)
{
  return(mpz_cmp_ui(num,vui)<0);
}
bool empint::operator<(const empint& vmp)
{
  return(mpz_cmp(num,vmp.num)<0);
}
bool empint::operator>(unsigned int vui)
{
  return(mpz_cmp_ui(num,vui)>0);
}
bool empint::operator>(const empint& vmp)
{
  return(mpz_cmp(num,vmp.num)>0);
}
bool empint::operator<=(unsigned int vui)
{
  return(mpz_cmp_ui(num,vui)<=0);
}
bool empint::operator<=(const empint& vmp)
{
  return(mpz_cmp(num,vmp.num)<=0);
}
bool empint::operator>=(unsigned int vui)
{
  return(mpz_cmp_ui(num,vui)>=0);
}
bool empint::operator>=(const empint& vmp)
{
  return(mpz_cmp(num,vmp.num)>=0);
}


empint gcd(const empint& a,const empint& b)
{
  empint res;
  mpz_gcd(res.num,a.num,b.num);
  return(res);
}

empint root(const empint& a,empint& rem,unsigned int n)
{
  empint res;
  mpz_rootrem(res.num,rem.num,a.num,n);
  return(res);
}

empint sqrt(const empint& a,empint& rem)
{
  empint res;
  mpz_sqrtrem(res.num,rem.num,a.num);
  return(res);
}

empint pow(const empint& a,unsigned int n)
{
  empint res;
  mpz_pow_ui(res.num,a.num,n);
  return(res);
}




ostream& operator<<(ostream &stream,const empint& mpint)
{
  stream << mpint.num;

  return(stream);
}








empfloat::empfloat()
{
  mpf_init(num);
}
empfloat::empfloat(const empfloat& mpfloat)
{
  mpf_init_set(num,mpfloat.num);
}
empfloat::empfloat(const empint& mpint)
{
  mpf_init(num);
  mpf_set_z(num,mpint.num);
}
empfloat::empfloat(unsigned int ui)
{
  mpf_init_set_ui(num,ui);
}
empfloat::empfloat(int i)
{
  mpf_init_set_si(num,i);
}
empfloat::empfloat(double v)
{
  mpf_init_set_d(num,v);
}
empfloat::~empfloat()
{
  mpf_clear(num);
}

#ifdef EUTILS_HAVE_MPFRLIB
empfloat::empfloat(const empfloatr& mpfloatr)
{
  mpf_init(num);
  mpfr_get_f(num,mpfloatr.num,MPFR_RNDZ);
}

empfloat& empfloat::operator=(const empfloatr& mpfloatr)
{
  mpfr_get_f(num,mpfloatr.num,MPFR_RNDZ);
  return(*this);
}
#endif



empfloat& empfloat::operator=(double v)
{
  mpf_set_d(num,v);
  return(*this);
}
empfloat& empfloat::operator=(const empfloat& vmp)
{
  mpf_set(num,vmp.num);
  return(*this);
}

empfloat& empfloat::operator-=(const empfloat& vmp)
{
  mpf_sub(num,num,vmp.num);
  return(*this);
}

empfloat& empfloat::operator+=(const empfloat& vmp)
{
  mpf_add(num,num,vmp.num);
  return(*this);
}

empfloat& empfloat::sqrt()
{
  mpf_sqrt(num,num);
  return(*this);
}
empfloat empfloat::abs() const
{
  empfloat rval;
  mpf_abs(rval.num,num);
  return(rval);
}

empfloat& empfloat::multiply(const empfloat& vmp)
{
  mpf_mul(num,num,vmp.num);
  return(*this);
}

empfloat empfloat::operator*(const empfloat& vmp) const
{
  empfloat rval;
  mpf_mul(rval.num,num,vmp.num);
  return(rval);
}
empfloat empfloat::operator+(const empfloat& vmp) const
{
  empfloat rval;
  mpf_add(rval.num,num,vmp.num);
  return(rval);
}
empfloat empfloat::operator-(const empfloat& vmp) const
{
  empfloat rval;
  mpf_sub(rval.num,num,vmp.num);
  return(rval);
}
empfloat empfloat::operator/(const empfloat& vmp) const
{
  empfloat rval;
  mpf_div(rval.num,num,vmp.num);
  return(rval);
}

bool empfloat::operator==(double v)
{
  return(mpf_cmp_d(num,v)==0);
}
bool empfloat::operator==(const empfloat& vmp)
{
  return(mpf_cmp(num,vmp.num)==0);
}
bool empfloat::operator!=(double v)
{
  return(mpf_cmp_d(num,v)!=0);
}
bool empfloat::operator!=(const empfloat& vmp)
{
  return(mpf_cmp(num,vmp.num)!=0);
}
bool empfloat::operator<(double v)
{
  return(mpf_cmp_d(num,v)<0);
}
bool empfloat::operator<(const empfloat& vmp)
{
  return(mpf_cmp(num,vmp.num)<0);
}
bool empfloat::operator>(double vui)
{
  return(mpf_cmp_d(num,vui)>0);
}
bool empfloat::operator>(const empfloat& vmp)
{
  return(mpf_cmp(num,vmp.num)>0);
}
bool empfloat::operator<=(double vui)
{
  return(mpf_cmp_d(num,vui)<=0);
}
bool empfloat::operator<=(const empfloat& vmp)
{
  return(mpf_cmp(num,vmp.num)<=0);
}
bool empfloat::operator>=(double vui)
{
  return(mpf_cmp_d(num,vui)>=0);
}
bool empfloat::operator>=(const empfloat& vmp)
{
  return(mpf_cmp(num,vmp.num)>=0);
}





ostream& operator<<(ostream &stream,const empfloat& mpfloat)
{
  stream << mpfloat.num;

  return(stream);
}


