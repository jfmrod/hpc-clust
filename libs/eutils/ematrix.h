#ifndef EMATRIX_H
#define EMATRIX_H

#include "eutils.h"

#include <iostream>
#include "efile.h"

#ifdef EUTILS_HAVE_LIBGSL
  #include <gsl/gsl_linalg.h>
#endif

using namespace std;

class ematrix;

class evector
{
 public:
  int w;
  double *vector;
#ifdef EUTILS_HAVE_LIBGSL
  gsl_vector_view gsl_vecview;
#endif

  evector();
  evector(const evector& v);
  evector(int w,double *values=0x00);
  ~evector();

  void create(int w);
  void clear();

  double length() const;

  double& operator()(int x);
  const double& operator()(int x) const;
  evector& operator=(const evector& v);
  evector& operator+=(const evector& v);
  evector& operator-=(const evector& v);

  inline double& operator[](int x) { return(vector[x]); }
  inline double& operator[](int x) const { return(vector[x]); }

  inline int size() const { return(w); }

  evector operator*(const ematrix& m) const;
  double  operator*(const evector& v) const;
  evector operator*(double f) const;
};

ostream& operator<<(ostream& stream,const evector& v);

class ematrix
{

 public:
  int w;
  int h;
  double *matrix;
#ifdef EUTILS_HAVE_LIBGSL
  gsl_matrix_view gsl_matview;
#endif

  ematrix();
  ematrix(const ematrix& m);
  ematrix(int w,int h,double *values=0x00);
  ~ematrix();

  void create(int w,int h);
  void clear();

  void load(efile file);
  void save(efile file);

  double& operator()(int x,int y);
  const double& operator()(int x,int y) const;

  evector row(int i) const;
  evector col(int j) const;

  void swap(int i,int i2);
  void swapcols(int j,int j2);
  void mulrow(int i,double f);
  void addmulrow(int i,double f,int i2);

  ematrix& operator=(const ematrix& m);
  void copytranspose(const ematrix& mat);

  evector operator*(const evector& v) const;
};

void nullspace(ematrix& A,ematrix& n);
void nullspacer(ematrix& A,ematrix& n);
int rref(ematrix& A);
void svd(ematrix& A,ematrix& V,evector& S);

evector eigenvalues(ematrix& A);

ostream& operator<<(ostream& stream,const ematrix& m);

#endif

