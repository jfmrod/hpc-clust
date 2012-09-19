#include "ematrix.h"

#include <math.h>

#include "logger.h"

#ifdef EUTILS_HAVE_LIBGSL
  #include <gsl/gsl_blas.h>
  #include <gsl/gsl_eigen.h>
#endif

evector::evector(): w(0),vector(0x00) {}

evector::evector(const evector& v): w(0),vector(0x00)
{
  operator=(v);
}
evector::evector(int w,double *values): w(0),vector(0x00)
{
  create(w);
  int i;
  if (values){
    for (i=0; i<w; ++i)
      vector[i]=values[i];
  }
}

evector::~evector()
{
  clear();
}


void evector::clear()
{
  if (vector){
    delete[] vector;
    vector=0x00;
    w=0;
  }
}

void evector::create(int _w)
{
  clear();
  w=_w;
  vector=new double[w];
#ifdef EUTILS_HAVE_LIBGSL
  gsl_vecview = gsl_vector_view_array(vector,w);
#endif
}
 

evector& evector::operator=(const evector& v)
{
  if (!v.w){ clear(); return(*this); }
  if (w!=v.w)
    create(v.w);
#ifdef EUTILS_HAVE_LIBGSL
  gsl_vector_memcpy(&gsl_vecview.vector,&v.gsl_vecview.vector);
#else
  int i;
  for (i=0; i<v.w; ++i)
    vector[i]=v.vector[i];
#endif
  return(*this);
}

evector& evector::operator+=(const evector& v)
{
  if (!v.w){ clear(); return(*this); }
  if(w!=v.w){ lerror("vectors are not the same size"); return(*this); }

  int i;
  for (i=0; i<v.w; ++i)
    operator()(i)+=v(i);
  return(*this);
}

double& evector::operator()(int x)
{
  return(vector[x]);
}

const double& evector::operator()(int x) const
{
  return(vector[x]);
}

evector evector::operator*(const ematrix& m) const
{
  evector res(m.w);
#ifdef EUTILS_HAVE_LIBGSL
  gsl_blas_dgemv (CblasTrans,1.0, &m.gsl_matview.matrix, &gsl_vecview.vector, 0.0, &res.gsl_vecview.vector);
#else
  lerror("only available with libgsl support");
#endif
  return(res);
}

evector evector::operator*(double f) const
{
  int i;
  evector res(*this);
  for (i=0; i<res.w; ++i)
    res(i)=res(i)*f;
  return(res);
}

double evector::operator*(const evector& v) const
{
  double res;
  res=0.0;
#ifdef EUTILS_HAVE_LIBGSL
  gsl_blas_ddot(&gsl_vecview.vector,&v.gsl_vecview.vector,&res);
#else
  lerror("only available with libgsl support");
#endif
  return(res);
}


double evector::length() const
{
  return(sqrt(operator*(*this)));
}


ostream& operator<<(ostream& stream,const evector& v)
{
  stream << "evector {";
  int i;
  for (i=0; i<v.w; ++i)
    stream << " " <<v(i);
  stream <<" }";
  return(stream);
}




ematrix::ematrix(): w(0),h(0),matrix(0x00)
{}

ematrix::ematrix(const ematrix& m): w(0),h(0),matrix(0x00) {
  operator=(m);
}

ematrix::ematrix(int w,int h,double *values): matrix(0x00)
{
  create(w,h);

  int i;
  if (values){
    for (i=0; i<w*h; ++i)
      matrix[i]=values[i];
  }
}

ematrix::~ematrix()
{
  clear();
}

#include "eregexp.h"
#include "logger.h"

void ematrix::save(efile file)
{
//  if (!file.open("w")) { lwarn("unable to open file: \""+file.filename+"\" for writing"); return; }

  file.write(estr(h)+" "+estr(w)+"\n");
  int i,j;

  for (i=0; i<h; ++i){
    for (j=0; j<w; ++j){
      file.write(estr(matrix[i*w+j])+" ");
    }
    file.write("\n");
  }
  file.close();
}

void ematrix::load(efile file)
{
  clear();

//  if (!file.open("r")) { lwarn("unable to load matrix from file: \""+file.filename+"\""); return; }

  estr s;
  file.readln(s);

  cout << s << endl;

  cout << reint.match(s,0) << endl;

  if (!grepint(s,h)) { lwarn("unable to get height for matrix"); return; }
  if (!grepint(s,w)) { lwarn("unable to get width for matrix"); return; }

  linfo("reading file");
  file.read(s);
  linfo("creating matrix of size: "+estr(h)+"x"+estr(w));

  create(w,h);

  estr space;
  if (s.find(" ")!=-1) space=" ";
  else if (s.find("\t")!=-1) space="\t";
  else { lerror("field separator is neither space or tab"); return; }


  estr tmp;
  linfo("reading values from file into matrix");
  int i,j,t;
  for (i=0; i<h && s.len(); ++i){ //ldinfo("element: "+estr(i));
    tmp=s.getline();
    t=0;
    for (j=0;t!=-1 && j<w; ++j){
//      cout << "j: "<<j<<"  t: "<<t<<endl;
      sscanf(&tmp[t],"%lg",&matrix[i*w+j]);
      t=tmp.find(space,t+1);
    }
    lwarnif(j!=w,"no. of columns defined bigger than it really is: "+estr(j)+" != "+estr(w));
  }
  lwarnif(i!=h,"no. of rows defined bigger than it really is: "+estr(i)+" != "+estr(h));

//  if (i!=w*h) lwarn("size of matrix is defined bigger than it really is: "+estr(i)+" != "+estr(w*h));
  linfo("done");
  file.close();
}

#define SWAP(a,b) { t=a; a=b; b=t; }

void ematrix::copytranspose(const ematrix& mat)
{
  clear();
  w=mat.h;
  h=mat.w;
  create(w,h);

  int i,j;
  for (i=0; i<mat.h; ++i){
    for (j=0; j<mat.w; ++j)
      matrix[j*w+i]=mat.matrix[i*mat.w+j];
  }
}


void ematrix::clear()
{
  if (matrix){
    delete[] matrix;
    matrix=0x00;
    h=0;
    w=0;
  }
}

void ematrix::create(int _w,int _h)
{
  clear();
  if (!_w || !_h) return;
  w=_w;
  h=_h;
  matrix=new double[w*h];
#ifdef EUTILS_HAVE_LIBGSL
  gsl_matview = gsl_matrix_view_array(matrix,h,w);
#endif
}
 

ematrix& ematrix::operator=(const ematrix& m)
{
  if (m.matrix==0x00){ clear(); return(*this); }
  if (w!=m.w || h!=m.h)
    create(m.w,m.h);
#ifdef EUTILS_HAVE_LIBGSL
  gsl_matrix_memcpy(&gsl_matview.matrix,&m.gsl_matview.matrix);
#else
  int i;
  for (i=0; i<m.w*m.h; ++i)
    matrix[i] = m.matrix[i];
#endif
  return(*this);
}

double& ematrix::operator()(int i,int j)
{
  return(matrix[i*w+j]);
}

const double& ematrix::operator()(int i,int j) const
{
  return(matrix[i*w+j]);
}

evector ematrix::row(int i) const
{
  evector res(w);
  int a;
  for (a=0; a<w; ++a)
   res[a]=operator()(i,a);
  return(res);
}

evector ematrix::col(int j) const
{
  evector res(h);
  int a;
  for (a=0; a<h; ++a)
   res[a]=operator()(a,j);
  return(res);
}

evector ematrix::operator*(const evector& v) const
{
  evector res(h);
#ifdef EUTILS_HAVE_LIBGSL
  gsl_blas_dgemv (CblasNoTrans,1.0, &gsl_matview.matrix, &v.gsl_vecview.vector, 0.0, &res.gsl_vecview.vector);
#else
  lerror("only available with libgsl support");
#endif
  return(res);
}

void ematrix::mulrow(int i,double f)
{
  double *p=&matrix[i*w];
  for (i=0; i<w; ++i,++p)
    *p = *p*f;
}

void ematrix::addmulrow(int i,double f,int i2)
{
  double *p=&matrix[i*w];
  double *p2=&matrix[i2*w];
  for (i=0; i<w; ++i,++p,++p2)
    *p += *p2*f;
}

void ematrix::swap(int i,int i2)
{
//  cout << "swapping rows: "<<i<<","<<i2<<endl;
#ifdef EUTILS_HAVE_LIBGSL
  gsl_matrix_swap_rows(&gsl_matview.matrix,i,i2);
#else
  lerror("only available with libgsl support");
#endif
}

void ematrix::swapcols(int j,int j2)
{
//  cout << "swapping rows: "<<i<<","<<i2<<endl;
#ifdef EUTILS_HAVE_LIBGSL
  gsl_matrix_swap_columns(&gsl_matview.matrix,j,j2);
#else
  lerror("only available with libgsl support");
#endif
}

#include <math.h>

#define MIN(a,b) ( ( (a) < (b) ) ? (a) : (b) )


bool isLeftMost(const ematrix& A,int i,int j)
{
  for (--j; j>=0; --j){
    if (A(i,j)!=0.0) return(false);
  }
  return(true);
}

int rref(ematrix& A)
{
  double fmax;
  int imax;
  int m,i,i2,j;
  m=MIN(A.w,A.h);
  int rank;
  rank=0;
  for (j=0; j<m && j<A.w; ++j){
    // find the pivot which is the largest absolute value
    imax=-1; fmax=0.0;
    for (i=rank; i<A.h; ++i){
      if (fabs(A(i,j))>fmax) { fmax=fabs(A(i,j)); imax=i; }
    }
    if (imax==-1 || fabs(fmax)<=1.0e-5) { ++m; continue; }

//    cout <<"pivot("<<imax<<","<<j<<"): "<<A(imax,j)<<endl;

    A.mulrow(imax,1.0/A(imax,j));
    for (i2=0; i2<A.h; ++i2){
      if (A(i2,j)!=0.0 && i2!=imax){
//        cout << "eliminating row: "<<i2<<" coef:"<<A(i2,j)<<endl;
        A.addmulrow(i2,-A(i2,j),imax);
      }
    }
    if (imax!=rank) A.swap(imax,rank);
    ++rank;
  }
  return(rank);
//  cout << "rank is: "<<rank<<endl;
}


void nullspacer(ematrix& A,ematrix& n)
{
  double fmax;
  int imax;
  int i,i2,j;
  int rank;

  ematrix m;

  m.create(A.w,A.w);
  for (i=0; i<A.h && i<m.h; ++i){
    for (j=0; j<A.w; ++j)
      m(i,j)=A(i,j);
  }
  
  for (; i<m.h; ++i){
    for (j=0; j<m.w; ++j)
      m(i,j)=0.0;
  }

  rank=0;
  for (j=0; j<m.w; ++j){
    // find the pivot which is the largest absolute value
    imax=-1; fmax=0.0;
    for (i=0; i<m.h; ++i){
      if ((i>j || m(i,i)!=1.0) && fabs(m(i,j))>fmax) { fmax=fabs(m(i,j)); imax=i; }
    }
    if (imax==-1 || fabs(fmax)<=1.0e-5) { continue; }

//    cout <<"pivot("<<imax<<","<<j<<"): "<<A(imax,j)<<endl;

    m.mulrow(imax,1.0/m(imax,j));
    for (i2=0; i2<m.h; ++i2){
      if (m(i2,j)!=0.0 && i2!=imax){
//        cout << "eliminating row: "<<i2<<" coef:"<<A(i2,j)<<endl;
        m.addmulrow(i2,-m(i2,j),imax);
      }
    }
    if (imax!=j) m.swap(imax,j);
//    cout << "swapped imax: "<<imax<<" j: "<<j<<endl;
//    if (rank!=j) A.swapcols(j,rank);
    ++rank;
  }

//  A=m;

  n.create(m.w,m.w-rank);
  i2=0;
  int k;
  for (j=0; j<m.w; ++j){
    if (m(j,j)!=1.0){
      for (k=0; k<j; ++k)
        n(i2,k) = -m(k,j);
      if (k<m.w)
        { n(i2,k)=1.0; ++k; }
      for (; k<n.w; ++k)
        n(i2,k) = 0.0;
      ++i2;
    }
  }
}

void nullspace(ematrix& A,ematrix& n)
{
  ematrix v;
  evector s;

  svd(A,v,s);

  int rank;
  for (rank=0; rank<s.w; ++rank)
    if (s(rank) < 1.0e-10) break;

  n.create(v.h,v.w-rank);
  int i,j;
  for (i=rank; i<v.w; ++i){
    for (j=0; j<v.h; ++j){
      if (fabs(v(j,i))<1.0e-10)
        n(i-rank,j)=0.0;
      else
        n(i-rank,j) = v(j,i);
    }
  }
}


void svd(ematrix& A,ematrix& V,evector& S)
{
#ifdef EUTILS_HAVE_LIBGSL
  gsl_vector *v_work = gsl_vector_alloc(A.w);

  if (A.w > A.h){
    ematrix T(A.w,A.w);
    int i,j;
    for (i=0; i<A.h; ++i){
      for (j=0; j<A.w; ++j)
        T(i,j)=A(i,j);
    }
    for (; i<T.h; ++i){
      for (j=0; j<T.w; ++j)
        T(i,j)=0.0;
    }
    if (V.w != T.w || V.w != V.h)
      V.create(T.w,T.w);
    if (S.w != T.w)
      S.create(T.w);
    gsl_linalg_SV_decomp(&T.gsl_matview.matrix,&V.gsl_matview.matrix,&S.gsl_vecview.vector,v_work);
  }
  else{
    if (V.w != A.w || V.w != V.h)
      V.create(A.w,A.w);
    if (S.w != A.w)
      S.create(A.w);
    gsl_linalg_SV_decomp(&A.gsl_matview.matrix,&V.gsl_matview.matrix,&S.gsl_vecview.vector,v_work);
  }

  gsl_vector_free(v_work);
#else
  lerror("only available with libgsl support");
#endif
}

evector eigenvalues(ematrix& A)
{
/*
  if (A.w!=A.h) { lerror("eigen values can only be computed for square matrices"); return(evector()); }

  gsl_eigen_nonsymm_workspace *gsl_esn_ws=gsl_eigen_nosysmm_alloc(A.w);

  evector tmpv;
  tmpv.create(A.w);

  gsl_eigen_nonsymm(&A.gsl_matview.matrix,&tmpv.gsl_vecview.vector,gsl_esn_ws);

  gsl_eigen_nosymm_free(gsl_esn_ws);
  return(tmpv);
*/
}


ostream& operator<<(ostream& stream,const ematrix& m)
{
  stream << "ematrix {"<<endl;
  int i,j;
  for (i=0; i<m.h; ++i){
    for (j=0; j<m.w; ++j)
      stream << " " <<m(i,j);
    stream << endl;
  }
  stream <<"}";
  return(stream);
}

