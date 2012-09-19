#include "etest.h"

#ifdef EUTILS_HAVE_APOLIB
#include <apop.h>

double etest_fisher(double a,double b,double c,double d)
{
  double vals[4];
  vals[0]=a; vals[1]=b;
  vals[2]=c; vals[3]=d;
  apop_data *apdata=apop_line_to_data(vals,0,2,2);
  apop_data *resdata=apop_test_fisher_exact(apdata);
  double res=apop_data_get(resdata,1,0);
  apop_data_free(apdata);
  apop_data_free(resdata);
  return(res);
}
#endif

