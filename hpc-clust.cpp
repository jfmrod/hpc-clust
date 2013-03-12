#include <eutils/emain.h>
#include <eutils/estrarray.h>
#include <eutils/efile.h>
#include <eutils/eheap.h>
#include <eutils/etime.h>
#include <eutils/etimer.h>
#include <eutils/eoption.h>
#include <eutils/eparalgor.h>
#include <eutils/esystem.h>

#include "hpc-clust-config.h"
#include "cluster-common.h"
#include "eseqclusteravg.h"

eblockarray<eseqdist> dists;

eseqcluster clcluster; // complete linkage
eseqclustersingle slcluster; // single linkage
eseqclusteravg alcluster; // avg linkage

int partsFinished=0;
int partsTotal=100;

estrarray arr;
unsigned totaldists;

int seqlen=0;

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


void help()
{
  printf("HPC-CLUST v%s\n",HPC_CLUST_PACKAGE_VERSION);
  printf("by Joao F. Matias Rodrigues and Christian von Mering\n");
  printf("Institute of Molecular Life Sciences, University of Zurich, Switzerland\n");
  printf("\n");
  printf("Usage:\n");
  printf("    %s [...] <-sl true|-cl true|-al true> aligned_seqs\n",efile(argv[0]).basename()._str);
  printf("\n");
  printf("Cluster a set of multiple aligned sequences until a given threshold.\n");
  printf("Example: hpc-clust -ncpus 4 -t 0.8 -dfunc gap -sl true myalignedseqs.sto\n"); 
  printf("\n");
  printf("Optional arguments:\n");
  printf("%10s    %s.\n","-t","distance threshold until which to do the clustering [default: 0.9]");
  printf("%10s    %s\n","-dfunc","distance function to use: nogap, gap, tamura [default: nogap]");
  printf("%10s    %s\n","-ncpus","number of threads to use [default: 1]");
  printf("%10s    %s\n","-ofile","output filename [defaults to input filename]. \".sl\",\".cl\", or \".al\" extensions will be appended");
  printf("\n");
  printf("At least one is required:\n");
  printf("%10s    %s\n","-sl true","perform single-linkage clustering");
  printf("%10s    %s\n","-cl true","perform complete-linkage clustering");
  printf("%10s    %s\n","-al true","perform average-linkage clustering");
  printf("\n");
  printf("Report bugs to: jfmrod@konceptfx.com\n");
  printf("http://www.konceptfx.com/hpc-clust/\n");

  exit(0);
}

int emain()
{ 
  bool cl=false;
  bool sl=false;
  bool al=false;
  bool cdist=false;
  epregister(cl);
  epregister(sl);
  epregister(al);
  epregister(cdist);
  epregisterFunc(help);

  eoption<efunc> dfunc;

  dfunc.choice=0;
  dfunc.add("gap",t_calc_dists_u<estrarray,eseqdist,eblockarray<eseqdist>,dist_compressed>);
  dfunc.add("nogap",t_calc_dists_u<estrarray,eseqdist,eblockarray<eseqdist>,dist_nogap_compressed>);
  dfunc.add("nogapsingle",t_calc_dists_u<estrarray,eseqdist,eblockarray<eseqdist>,dist_nogapsingle_compressed>);
  dfunc.add("tamura",t_calc_dists_u<estrarray,eseqdist,eblockarray<eseqdist>,dist_tamura_compressed>);

  epregisterClass(eoption<efunc>);
  epregisterClassMethod2(eoption<efunc>,operator=,int,(const estr& val));

  epregister(dfunc);

  int winlen=70;
  epregister(winlen);

  estr ofile;
  estr dfile;
  float t=0.90;
  int ncpus=1;
  bool ignoreUnique=false;
  epregister(ignoreUnique);
  epregister(t);
  epregister(ncpus);
  epregister(ofile);
  epregister(dfile);
  epregister(ignoreMemThres);
  eparseArgs(argvc,argv);

//  cout << "# initializing identity lookup table" << endl;
//  initLookupTable();

  if(argvc<2) {
    cout << "syntax: "+efile(argv[0]).basename()+" <-sl true|-cl true|-al true> <seqali>" << endl;
    cout << "\""+efile(argv[0]).basename()+ " --help\" for more help" << endl;
    exit(-1);
  }
  if(!cl && !sl && !al) {
    cout << "syntax: "+efile(argv[0]).basename()+" <-sl true|-cl true|-al true> <seqali>" << endl;
    cout << "please choose at least one clustering method <-sl true|-cl true|-al true>" << endl;
    cout << "\""+efile(argv[0]).basename()+ " --help\" for more help" << endl;
    exit(-1);
  }

  cout << "# " << date() << endl;
  cout << "# " << args2str(argvc,argv) << endl;
  cout << "# system RAM: " << getSystem()->getTotalRam()/1024 << "Mb" << endl;
  cout << "# free system RAM: " << (getSystem()->getFreeRam()+getSystem()->getBufferRam())/1024 << "Mb" << endl;
  cout << "# process memory limit: " << ((getSystem()->getMemLimit()&0x3fffffffffffff)==0x3fffffffffffff?estr("unlimited"):estr(getSystem()->getMemLimit()/1024/1024)+"Mb") << endl;

  warnMemThres=MIN(MIN(getSystem()->getTotalRam(),getSystem()->getMemLimit()/1024),getSystem()->getFreeRam()+getSystem()->getBufferRam())*0.6/1024;
  exitMemThres=MIN(MIN(getSystem()->getTotalRam(),getSystem()->getMemLimit()/1024),getSystem()->getFreeRam()+getSystem()->getBufferRam())*0.65/1024;

  cout << "# warning memory threshold: " << warnMemThres << "Mb" << endl;
  cout << "# exit memory threshold: " << exitMemThres << "Mb" << endl;


 
  if (ofile.len()==0)
    ofile=argv[1];


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

  long int i,j;
    load_seqs_compressed(argv[1],arr,seqlen);

  ebasicstrhashof<int> duphash;
  ebasicstrhashof<int>::iter it;
  eintarray uniqind;
  earray<eintarray> dupslist;
  if (!ignoreUnique){
    duphash.reserve(arr.size());
    for (i=0; i<arr.size(); ++i){
      if (i%(arr.size()/10)==0)
        fprintf(stderr,"\r%li/%i",i,arr.size());
      it=duphash.get(arr.values(i));
      if (it==duphash.end())
        { uniqind.add(i); duphash.add(arr.values(i),uniqind.size()-1); dupslist.add(eintarray(i)); }
      else 
        dupslist[it.value()].add(i);
    }
    fprintf(stderr,"\n");
  }else{
    uniqind.init(arr.size());
    for (i=0; i<uniqind.size(); ++i)
      uniqind[i]=i;
  }
  cout << endl;
  cout << "# unique seqs: " << uniqind.size() << endl;

  long int maxdists=uniqind.size()*(uniqind.size()-1)/2;
  long int maxmem=maxdists*sizeof(eseqdist)/1024/1024;
  cout << "# maximum number of distance pairs: " << maxdists << " (" << maxmem << "Mb)" << endl;

  if (maxmem > warnMemThres){
    cout << "# WARNING: Number of sequences provided may require more memory than is currently available on this system." << endl;
    cout << "#           Please monitor the memory usage of this program and check the log at the end. This program will" << endl;
    cout << "#           automatically exit if it reaches the exitMemThres value shown above. You can force the program" << endl;
    cout << "#           to ignore this threshold using the argument: -ignoreMemThres true" << endl;
    cout << "#           Memory requirements can be reduced by increasing the clustering threshold, or reducing the number" << endl;
    cout << "#           of sequences to be clustered. For more information and tips on optimizing hpc-clust memory" << endl;
    cout << "#           usage please refer to the documentation." << endl;
  }

  
  float dtime,stime;
  etimer t1;
  t1.reset();

  etaskman taskman;

  efile df(dfile);
    cout << "# computing distances" << endl;
    if (partsTotal>(arr.size()-1)*arr.size()/20) partsTotal=(arr.size()-1)*arr.size()/20;
    for (i=0; i<partsTotal; ++i)
      taskman.addTask(dfunc.value(),evararray(mutex,uniqind,arr,dists,(const int&)seqlen,(const int&)i,(const int&)partsTotal,(const float&)t,(const int&)winlen));

    taskman.createThread(ncpus);
    taskman.wait();

    dtime=t1.lap()*0.001;
    cout << "# time calculating distances: " << dtime << endl;
    cout << "# distances within threshold: " << dists.size() << endl;

    cout << "# number of tasks: " << taskman.tasks.size() << endl;
    radix256sort<eblockarray<eseqdist>,radixKey>(dists);
    cout << "# number of tasks: " << taskman.tasks.size() << endl;
    stime=t1.lap()*0.001;

    if (dfile.len()){
      cout << "# saving distances to file: "<<dfile << endl;
      for (i=0; i<dists.size(); ++i)
        df.write(estr(dists[i].x)+" "+dists[i].y+" "+dists[i].dist+"\n");
//      estr str;
//      mindists.serial(str);
//      df.write(str);
      for (i=0; i<dupslist.size(); ++i){
        for (j=1; j<dupslist[i].size(); ++j)
          df.write(estr(dupslist[i][0])+" "+dupslist[i][j]+" 1.0\n");
      }
      df.close();
    }
//  }else{
//    cout << "# loading distances from file: "<<dfile << endl;
/*
    estr str;
    df.read(str);
    ldieif(mindists.unserial(str,0)==-1,"problem loading distance file: "+dfile);
    df.close();
*/
//  } 

  totaldists=dists.size();
  cout << "# time sorting distances: " << stime << endl;

  cout << "# initializing cluster"<<endl;
  if (cl)
    clcluster.init(arr.size(),ofile+".cl",argv[1],dupslist);
  if (sl)
    slcluster.init(arr.size(),ofile+".sl",argv[1],dupslist);
  if (al)
    alcluster.init(arr.size(),ofile+".al",argv[1],dupslist);

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
  }
  float clustime=t1.lap()*0.001;
  cout << "# time calculating distances: " << dtime << endl;
  cout << "# time sorting distances: " << stime << endl;
  cout << "# time clustering: " << clustime << endl;
  cout << "# total time: " << dtime+clustime+stime << endl;
  cout << "# distances within threshold: " << totaldists << endl;

  if (cdist){
    efile fsl,fcl,fal;
    if (sl) fsl.open(ofile+".sl.dist","w");
    if (cl) fcl.open(ofile+".cl.dist","w");
    if (al) fal.open(ofile+".cl.dist","w");

    for (i=dists.size()-1; i>=0; --i){
      if (sl) fsl.write(estr(dists[i].x)+" "+dists[i].y+" "+dists[i].dist+" "+slcluster.clusterData.getMergeDistance(dists[i].x,dists[i].y)+"\n");
      if (cl) fcl.write(estr(dists[i].x)+" "+dists[i].y+" "+dists[i].dist+" "+clcluster.clusterData.getMergeDistance(dists[i].x,dists[i].y)+"\n");
      if (al) fal.write(estr(dists[i].x)+" "+dists[i].y+" "+dists[i].dist+" "+alcluster.clusterData.getMergeDistance(dists[i].x,dists[i].y)+"\n");
    }
  }
  return(0);
}

