#ifndef __ECORR__
#define __ECORR__

class evector;


double ecorr(const evector& x,const evector& y);
double eautocorr(const evector& x,int lag);


#endif

