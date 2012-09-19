
#include "ecorr.h"
#include "../ematrix.h"
#include "../logger.h"

double ecorr(const evector& x,const evector& y)
{
  ldie("ecorr(): not implemented yet!");
  return(0.0);
}

double eautocorr(const evector& x,int t)
{
  double avg;
  double var;
  double Ch;
  int i;

  avg=0.0;
  for (i=0; i<x.size(); ++i)
    avg+=x[i];
  avg=avg/(double)(x.size());

  Ch=0.0; var=0.0;
  for (i=0; i<x.size()-t; ++i){
    double a = x[i]-avg;
    Ch+=a*(x[i+t]-avg);
    var+=a*a;
  }
  for (;i<x.size(); ++i){
    double a = x[i]-avg;
    var+=a*a;
  }
//  cout << "size: "<<x.size()<<"   Ch: " << Ch << "   var: "<<var<<endl;
  return(Ch/var);
}


