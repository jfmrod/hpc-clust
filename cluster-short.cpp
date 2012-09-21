#include <eutils/emain.h>
#include <eutils/efile.h>
#include <eutils/eheap.h>
#include <eutils/etime.h>
#include <eutils/etimer.h>
#include <eutils/eoption.h>
#include <eutils/eparalgor.h>

#include "cluster-common.h"
#include "eseqclusteravg.h"
#include "eseqclusterstep.h"

//eseqcluster cluster;

eblockarray<eseqdist> dists;

eseqcluster clcluster; // complete linkage
eseqclustersingle slcluster; // single linkage
eseqclusteravg alcluster; // avg linkage
eseqclusterstep stepcluster; 

int partsFinished=0;
int partsTotal=100;
ebasicarray<eshortseq> arr;
unsigned totaldists;

unsigned int radixKey(eblockarray<eseqdist>& dists,long int i)
{
  return((unsigned int)(dists[i].dist*10000));
}

estr args2str(int argvc,char **argv)
{
  estr tmpstr;
  int i;
  for (i=0; i<argvc; ++i)
    tmpstr+=estr(argv[i])+" ";
  tmpstr.del(-1);
  return(tmpstr);
}

emutex mutex;

inline float dist_nogap_short3_compressed(const eshortseq& s1,const eshortseq& s2)
{
//  if (s1.b>s2.e || s2.b>s1.e) return(0.0);

  int s,e;
  s=MAX(s1.b,s2.b);
  e=MIN(s1.e,s2.e);
  if (s>e) return(0.0);

  int len=e-s;
  if (len<10) return(0.0);

  int count=0;
  long int *ep1=(long int*)(s1.seq._str)+(e/16);
  long int *p1=(long int*)(s1.seq._str)+(s/16);
  long int *p2=(long int*)(s2.seq._str)+(s/16);
  if (p1<ep1){
    switch (s%16){
     case 1:
      dist_nogap_inc(*p1,*p2,b4_m1,count,len);
     case 2:
      dist_nogap_inc(*p1,*p2,b4_m2,count,len);
     case 3:
      dist_nogap_inc(*p1,*p2,b4_m3,count,len);
     case 4:
      dist_nogap_inc(*p1,*p2,b4_m4,count,len);
     case 5:
      dist_nogap_inc(*p1,*p2,b4_m5,count,len);
     case 6:
      dist_nogap_inc(*p1,*p2,b4_m6,count,len);
     case 7:
      dist_nogap_inc(*p1,*p2,b4_m7,count,len);
     case 8:
      dist_nogap_inc(*p1,*p2,b4_m8,count,len);
     case 9:
      dist_nogap_inc(*p1,*p2,b4_m9,count,len);
     case 10:
      dist_nogap_inc(*p1,*p2,b4_m10,count,len);
     case 11:
      dist_nogap_inc(*p1,*p2,b4_m11,count,len);
     case 12:
      dist_nogap_inc(*p1,*p2,b4_m12,count,len);
     case 13:
      dist_nogap_inc(*p1,*p2,b4_m13,count,len);
     case 14:
      dist_nogap_inc(*p1,*p2,b4_m14,count,len);
     case 15:
      dist_nogap_inc(*p1,*p2,b4_m15,count,len);
      ++p1;
      ++p2;
    }
  }

  for (; p1<ep1; ++p1,++p2){
    dist_nogap_inc(*p1,*p2,b4_m0,count,len);
    dist_nogap_inc(*p1,*p2,b4_m1,count,len);
    dist_nogap_inc(*p1,*p2,b4_m2,count,len);
    dist_nogap_inc(*p1,*p2,b4_m3,count,len);
    dist_nogap_inc(*p1,*p2,b4_m4,count,len);
    dist_nogap_inc(*p1,*p2,b4_m5,count,len);
    dist_nogap_inc(*p1,*p2,b4_m6,count,len);
    dist_nogap_inc(*p1,*p2,b4_m7,count,len);
    dist_nogap_inc(*p1,*p2,b4_m8,count,len);
    dist_nogap_inc(*p1,*p2,b4_m9,count,len);
    dist_nogap_inc(*p1,*p2,b4_m10,count,len);
    dist_nogap_inc(*p1,*p2,b4_m11,count,len);
    dist_nogap_inc(*p1,*p2,b4_m12,count,len);
    dist_nogap_inc(*p1,*p2,b4_m13,count,len);
    dist_nogap_inc(*p1,*p2,b4_m14,count,len);
    dist_nogap_inc(*p1,*p2,b4_m15,count,len);
  }

  switch (e%16){
   case 15:
    dist_nogap_inc(*p1,*p2,b4_m14,count,len);
   case 14:
    dist_nogap_inc(*p1,*p2,b4_m13,count,len);
   case 13:
    dist_nogap_inc(*p1,*p2,b4_m12,count,len);
   case 12:
    dist_nogap_inc(*p1,*p2,b4_m11,count,len);
   case 11:
    dist_nogap_inc(*p1,*p2,b4_m10,count,len);
   case 10:
    dist_nogap_inc(*p1,*p2,b4_m9,count,len);
   case 9:
    dist_nogap_inc(*p1,*p2,b4_m8,count,len);
   case 8:
    dist_nogap_inc(*p1,*p2,b4_m7,count,len);
   case 7:
    dist_nogap_inc(*p1,*p2,b4_m6,count,len);
   case 6:
    dist_nogap_inc(*p1,*p2,b4_m5,count,len);
   case 5:
    dist_nogap_inc(*p1,*p2,b4_m4,count,len);
   case 4:
    dist_nogap_inc(*p1,*p2,b4_m3,count,len);
   case 3:
    dist_nogap_inc(*p1,*p2,b4_m2,count,len);
   case 2:
    dist_nogap_inc(*p1,*p2,b4_m1,count,len);
   case 1:
    dist_nogap_inc(*p1,*p2,b4_m0,count,len);
  }
  return((float)count/(float)len);
}

inline float dist_nogap_short2_compressed(const eshortseq& s1,const eshortseq& s2)
{
  if (s1.b>s2.e || s2.b>s1.e) return(0.0);

  int s,e;
  s=MIN(s1.b,s2.b);
  e=MAX(s1.e,s2.e);

  int len=e-s;
  int count=0;
  long int *ep1=(long int*)(s1.seq._str)+(e/16);
  long int *p1=(long int*)(s1.seq._str)+(s/16);
  long int *p2=(long int*)(s2.seq._str)+(s/16);
  if (p1<ep1){
    switch (s%16){
     case 1:
      dist_nogap_inc(*p1,*p2,b4_m1,count,len);
     case 2:
      dist_nogap_inc(*p1,*p2,b4_m2,count,len);
     case 3:
      dist_nogap_inc(*p1,*p2,b4_m3,count,len);
     case 4:
      dist_nogap_inc(*p1,*p2,b4_m4,count,len);
     case 5:
      dist_nogap_inc(*p1,*p2,b4_m5,count,len);
     case 6:
      dist_nogap_inc(*p1,*p2,b4_m6,count,len);
     case 7:
      dist_nogap_inc(*p1,*p2,b4_m7,count,len);
     case 8:
      dist_nogap_inc(*p1,*p2,b4_m8,count,len);
     case 9:
      dist_nogap_inc(*p1,*p2,b4_m9,count,len);
     case 10:
      dist_nogap_inc(*p1,*p2,b4_m10,count,len);
     case 11:
      dist_nogap_inc(*p1,*p2,b4_m11,count,len);
     case 12:
      dist_nogap_inc(*p1,*p2,b4_m12,count,len);
     case 13:
      dist_nogap_inc(*p1,*p2,b4_m13,count,len);
     case 14:
      dist_nogap_inc(*p1,*p2,b4_m14,count,len);
     case 15:
      dist_nogap_inc(*p1,*p2,b4_m15,count,len);
      ++p1;
      ++p2;
    }
  }

  for (; p1<ep1; ++p1,++p2){
    dist_nogap_inc(*p1,*p2,b4_m0,count,len);
    dist_nogap_inc(*p1,*p2,b4_m1,count,len);
    dist_nogap_inc(*p1,*p2,b4_m2,count,len);
    dist_nogap_inc(*p1,*p2,b4_m3,count,len);
    dist_nogap_inc(*p1,*p2,b4_m4,count,len);
    dist_nogap_inc(*p1,*p2,b4_m5,count,len);
    dist_nogap_inc(*p1,*p2,b4_m6,count,len);
    dist_nogap_inc(*p1,*p2,b4_m7,count,len);
    dist_nogap_inc(*p1,*p2,b4_m8,count,len);
    dist_nogap_inc(*p1,*p2,b4_m9,count,len);
    dist_nogap_inc(*p1,*p2,b4_m10,count,len);
    dist_nogap_inc(*p1,*p2,b4_m11,count,len);
    dist_nogap_inc(*p1,*p2,b4_m12,count,len);
    dist_nogap_inc(*p1,*p2,b4_m13,count,len);
    dist_nogap_inc(*p1,*p2,b4_m14,count,len);
    dist_nogap_inc(*p1,*p2,b4_m15,count,len);
  }

  switch (e%16){
   case 15:
    dist_nogap_inc(*p1,*p2,b4_m14,count,len);
   case 14:
    dist_nogap_inc(*p1,*p2,b4_m13,count,len);
   case 13:
    dist_nogap_inc(*p1,*p2,b4_m12,count,len);
   case 12:
    dist_nogap_inc(*p1,*p2,b4_m11,count,len);
   case 11:
    dist_nogap_inc(*p1,*p2,b4_m10,count,len);
   case 10:
    dist_nogap_inc(*p1,*p2,b4_m9,count,len);
   case 9:
    dist_nogap_inc(*p1,*p2,b4_m8,count,len);
   case 8:
    dist_nogap_inc(*p1,*p2,b4_m7,count,len);
   case 7:
    dist_nogap_inc(*p1,*p2,b4_m6,count,len);
   case 6:
    dist_nogap_inc(*p1,*p2,b4_m5,count,len);
   case 5:
    dist_nogap_inc(*p1,*p2,b4_m4,count,len);
   case 4:
    dist_nogap_inc(*p1,*p2,b4_m3,count,len);
   case 3:
    dist_nogap_inc(*p1,*p2,b4_m2,count,len);
   case 2:
    dist_nogap_inc(*p1,*p2,b4_m1,count,len);
   case 1:
    dist_nogap_inc(*p1,*p2,b4_m0,count,len);
  }
  return((float)count/(float)len);
}



int emain()
{ 
  cout << "# " << date() << endl;
  cout << "# " << args2str(argvc,argv) << endl;

  bool cl=false;
  bool sl=false;
  bool al=false;
  bool dist=false;
  bool step=false;
  epregister(cl);
  epregister(sl);
  epregister(al);
  epregister(step);

  eoption<efunc> dfunc;

  dfunc.choice=0;
  dfunc.add("nogap",t_calc_dists<ebasicarray<eshortseq>,eseqdist,eblockarray<eseqdist>,dist_nogap_short2_compressed>);
  dfunc.add("nogap2",t_calc_dists<ebasicarray<eshortseq>,eseqdist,eblockarray<eseqdist>,dist_nogap_short3_compressed>);

  epregisterClass(eoption<efunc>);
  epregisterClassMethod2(eoption<efunc>,operator=,int,(const estr& val));

  epregister(dfunc);

  estr ofile;
  float t=0.90;
  int ncpus=1;
  epregister(t);
  epregister(ncpus);
  epregister(ofile);
  eparseArgs(argvc,argv);

  ldieif(!cl && !sl && !al && !step && !dist,"please choose at least one clustering method <-sl true|-cl true|-al true|-dist true>");

  cout << "# distance function: " << dfunc.key() << endl;

  epregisterClass(eseqdist);
  epregisterClassSerializeMethod(eseqdist);
  epregisterClassProperty(eseqdist,dist);
  epregisterClassProperty(eseqdist,x);
  epregisterClassProperty(eseqdist,y);

  epregisterClass(ebasicarray<eseqdist>);
  epregisterClassInheritance(ebasicarray<eseqdist>,ebasearray);
  epregisterClassMethod(ebasicarray<eseqdist>,subset);
  epregisterClassSerializeMethod(ebasicarray<eseqdist>);

  ldieif(argvc<2,"syntax: "+efile(argv[0]).basename()+" <seqali>");

  if (ofile=="")
    ofile=argv[1];

  int i;
  load_short_compressed(argv[1],arr);
  
  float dtime,stime;
  etimer t1;
  t1.reset();


  earray<eintarray> dupslist;

  etaskman taskman;

  cout << "# computing distances" << endl;
  if (partsTotal>(arr.size()-1)*arr.size()/20) partsTotal=(arr.size()-1)*arr.size()/20;
  for (i=0; i<partsTotal; ++i)
    taskman.addTask(dfunc.value(),evararray(mutex,arr,dists,(const int&)i,(const int&)partsTotal,(const float&)t));

  taskman.createThread(ncpus);
  cout << "# finished creating threads: "<<ncpus << endl;
  taskman.wait();
  cout << "# all tasks finished" << endl;

  dtime=t1.lap()*0.001;
  cout << "# time calculating distances: " << dtime << endl;
  cout << "# distances within threshold: " << dists.size() << endl;

//    heapsort(mindists);
//    dists.sort();
//    parRadixSort<eblockarray<eseqdist>,radixKey>(dists,taskman);
  radix256sort<eblockarray<eseqdist>,radixKey>(dists);

  stime=t1.lap()*0.001;

/*
  if (dist){
    efile df(ofile+".dists");
    cout << "# saving distances to file: "<< ofile+".dists" << endl;
    estr str;
    dists.serial(str);
    df.write(str);
    df.close();
  }
*/

  totaldists=dists.size();
  cout << "# time sorting distances: " << stime << endl;

  cout << "# initializing cluster"<<endl;
  if (cl)
    clcluster.init(arr.size(),ofile+".cl.dat",argv[1],dupslist);
  if (sl)
    slcluster.init(arr.size(),ofile+".sl.dat",argv[1],dupslist);
  if (al)
    alcluster.init(arr.size(),ofile+".al.dat",argv[1],dupslist);
  if (step)
    stepcluster.init(arr.size(),ofile+".step.dat",argv[1],dupslist);

  cout << "# starting clustering"<<endl;
  t1.reset();
  int tmp;
  int lastupdate=0;
  for (i=dists.size()-1; i>=0; --i){
    if (cl)
      clcluster.add(dists[i]);
    if (al)
      alcluster.add(dists[i]);
    if (sl)
      slcluster.add(dists[i]);
    if (step)
      stepcluster.add(dists[i]);
  }
  float clustime=t1.lap()*0.001;
  cout << "# time calculating distances: " << dtime << endl;
  cout << "# time sorting distances: " << stime << endl;
  cout << "# time clustering: " << clustime << endl;
  cout << "# total time: " << dtime+clustime+stime << endl;
  cout << "# distances within threshold: " << totaldists << endl;

/*
  if (step){
    stepcluster.save(ofile+".sotu",arr,argv[1]);
    cout << "# done writing step single linkage clustering to: "<<ofile+".sotu" << endl;
  }
*/
  return(0);
}

