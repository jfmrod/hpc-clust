#include "estats.h"
#include "../ebasicarray.h"
#include "../eintarray.h"
#include "../eheap.h"
#include "../evar.h"
#include <math.h>

ostream& operator<<(ostream& stream, const estats& stats)
{
  stream << "( mean: " << stats.mean << " stddev: " << stats.stddev << " median: " << stats.median << " p99: " << stats.p99 <<","<< stats.p1 << " p95: " << stats.p95 <<","<< stats.p5 << " p45: " << stats.p45 <<","<< stats.p75 << " )";
  return(stream);
}

void estats::reset()
{
  values.clear();
  mean=0.0; stddev=0.0; median=0.0;
  p1=0.0; p5=0.0; p45=0.0; p75=0.0; p95=0.0; p99=0.0;
}

void estats::add(double value)
{
  values.add(value);
}

double getInterpPercentileValue(ebasicarray<double>& values,eintarray& sind,double perc)
{
  int i1=(int)(perc*values.size());

  if (i1==values.size()-1) return(values[sind[i1]]);

  int f=(perc*values.size())-i1;
  double v1=values[sind[i1]];
  double dv=values[sind[i1+1]]-v1;
  return(v1 + dv*f);
}

double getPercentileValue(ebasicarray<double>& values,eintarray& sind,double perc)
{
  return(values[sind[(int)(perc*values.size()+0.5)]]);
}

void estats::calc()
{
  eintarray sind(iheapsort(values));

  mean=0.0; stddev=0.0;
  int i;
  for (i=0; i<sind.size(); ++i){
    mean+=values[sind[i]];
    stddev+=values[sind[i]]*values[sind[i]];
  }
 
  p1=getPercentileValue(values,sind,0.01); 
  p5=getPercentileValue(values,sind,0.05); 
  p45=getPercentileValue(values,sind,0.45);
  p75=getPercentileValue(values,sind,0.75);
  p95=getPercentileValue(values,sind,0.95);
  p99=getPercentileValue(values,sind,0.99);

  mean=mean/values.size();
  stddev=sqrt(stddev/values.size()-mean*mean);

  median=values[sind[values.size()/2]];
  if (values.size()%2==0)
    median=(median+values[sind[values.size()/2+1]])/2.0;
}


void ebin::calc(int nbins)
{
  xbin.clear(); ybin.clear(); maxcount=0; iv.clear();
  if (values.size()==0) return;
  if (nbins==0) nbins=sqrt(values.size());

  iv=iheapsort(values);
  xbin.init(nbins);
  double xmin,xmax,xlen;
  xmin=values[iv[0]];
  xmax=values[iv[iv.size()-1]];
  xlen=(xmax-xmin);
  int i;
  for (i=0; i<nbins; ++i)
    xbin[i]=xmin+xlen*i/nbins+xlen*0.5/nbins;
  ybin.init(nbins,0);
  int j;
  for (i=0; i<values.size(); ++i){
    j=(values[i]-xmin)*nbins/xlen;
    if (j==nbins) --j;
    ++ybin[j];
  }
  for (i=0; i<ybin.size(); ++i)
    if (ybin[i]>maxcount) maxcount=ybin[i];
}

ostream& operator<<(ostream& stream, const ebin& bin)
{
  stream << "[ebin nbins: " << bin.xbin.size() << " values: " << bin.values.size() << "]";
  return(stream);
}

void ebin2::calc(int nbins)
{
  xbin.clear(); ybin.clear(); ycount.clear(); maxcount=0; iv.clear();
  if (xvalues.size()==0 || xvalues.size()!=yvalues.size()) return;
  if (nbins==0) nbins=sqrt(xvalues.size());

  iv=iheapsort(xvalues);
  xbin.init(nbins);
  double xmin,xmax,xlen;
  xmin=xvalues[iv[0]];
  xmax=xvalues[iv[iv.size()-1]];
  xlen=(xmax-xmin);
  int i;
  for (i=0; i<nbins; ++i)
    xbin[i]=xmin+xlen*i/nbins+xlen*0.5/nbins;
  ycount.init(nbins,0);
  ybin.init(nbins,0.0);

  int j;
  for (i=0; i<xvalues.size(); ++i){
    j=(xvalues[i]-xmin)*nbins/xlen;
    if (j==nbins) --j;
    ++ycount[j];
    ybin[j]+=yvalues[i];
  }
  for (i=0; i<ycount.size(); ++i){
    if (ycount[i]>0)
      ybin[i]=ybin[i]/ycount[i];
    if (ycount[i]>maxcount) maxcount=ycount[i];
  }
}

ostream& operator<<(ostream& stream, const ebin2& bin)
{
  stream << "[ebin2 nbins: " << bin.xbin.size() << " values: " << bin.xvalues.size() << "]";
  return(stream);
}


