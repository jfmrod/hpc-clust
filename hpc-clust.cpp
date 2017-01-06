#include <eutils/emain.h>
#include <eutils/estrarray.h>
#include <eutils/efile.h>
#include <eutils/eheap.h>
#include <eutils/etime.h>
#include <eutils/etimer.h>
#include <eutils/eoption.h>
//#include <eutils/eparalgor.h>
#include <eutils/esystem.h>

#include "hpc-clust-config.h"
#include "cluster-common.h"
#include "eseqclusteravg.h"

eblockarray<eseqdist> dists;

eseqcluster clcluster; // complete linkage
eseqclustersingle slcluster; // single linkage
eseqclusteravg alcluster; // avg linkage

long partsFinished=0;
long partsTotal=10000;

estrarray arr;
unsigned totaldists;

int seqlen=0;
bool ignoreUnique=false;

float radixKey(eblockarray<eseqdist>& dists,long int i)
{
  return(dists[i].dist);
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
  printf("Matias Rodrigues JF, Mering C von. HPC-CLUST: Distributed hierarchical clustering for very large sets of nucleotide sequences. Bioinformatics. 2013.\n");
  printf("\n");
  printf("Usage:\n");
  printf("    %s [...] <-sl true|-cl true|-al true> alignedseqs.fa\n",efile(argv[0]).basename()._str);
  printf("\n");
  printf("Clusters a set of multiple aligned sequences in fasta or stockholm format to a given threshold.\n");
  printf("Example: hpc-clust -nthreads 4 -t 0.8 -dfunc gap -sl true alignedseqs.fa\n"); 
  printf("\n"); 
  printf("Optional arguments:\n");
  printf("%10s    %s\n","-t","distance threshold until which to do the clustering [default: 0.9]");
  printf("%10s    %s\n","-dfunc","distance function to use: gap, nogap, tamura [default: gap]");
  printf("%10s    %s\n","-nthreads","number of threads to use [default: 4]");
  printf("%10s    %s\n","-ofile","output filename [defaults to input filename] + \".sl\",\".cl\", or \".al\" extension");
  printf("\n");
  printf("One or more is required:\n");
  printf("%10s    %s\n","-sl true","perform single-linkage clustering");
  printf("%10s    %s\n","-cl true","perform complete-linkage clustering");
  printf("%10s    %s\n","-al true","perform average-linkage clustering");
  printf("\n");

  printf("After clustering:\n");
  printf("%10s    %s\n","-makeotus <alignment> <mergelog> <threshold>","generate an OTU file at a given threshold");
  printf("%10s    %s\n","-makeotus_mothur <alignment> <mergelog> <threshold>","generate a MOTHUR compatible OTU file at a given threshold");
  printf("%10s    %s\n","-makereps <alignment> <otu>","generate a fasta file of OTU representatives. Sequences chosen have the minimum average distance to other sequences in the OTU.");
  printf("\n");

  printf("Report bugs to: joao.rodrigues@imls.uzh.ch\n");
  printf("http://meringlab.org/software/hpc-clust/\n");

  exit(0);
}

typedef float (*t_dfunc)(const estr&,const estr&,int);

class edistfunc
{
 public:
  efunc calcfunc;
  t_dfunc calcfunc_single;
  edistfunc(const efunc& _calcfunc,t_dfunc _calcfunc_single): calcfunc(_calcfunc),calcfunc_single(_calcfunc_single) {}
};

eoption<edistfunc> dfuncpart;
eoption<edistfunc> dfunc;
etaskman taskman;

int nthreads=4;
int winlen=70;
float t=0.90;

void finduniq(ebasicarray<INDTYPE>& uniqind,earray<ebasicarray<INDTYPE> >& dupslist)
{
  ebasicstrhashof<long> duphash;
  ebasicstrhashof<long>::iter it;
  if (!ignoreUnique){
    duphash.reserve(arr.size());
    for (long i=0; i<arr.size(); ++i){
      if (i%1000==0)
        fprintf(stderr,"\r%li/%li",i,(long)arr.size());
      it=duphash.get(arr.values(i));
      if (it==duphash.end())
        { uniqind.add(i); duphash.add(arr.values(i),uniqind.size()-1); dupslist.add(ebasicarray<INDTYPE>(i)); }
      else 
        dupslist[it.value()].add(i);
    }
    fprintf(stderr,"\r%li\n",(long)arr.size());
  }else{
    uniqind.init(arr.size());
    for (long i=0; i<uniqind.size(); ++i)
      uniqind[i]=i;
  }
  cout << endl;
}


void actionMakeOtusMothur()
{
  estrarray uarr;
  eseqclusterData cdata;
  ldieif(argvc<4,"syntax: "+efile(argv[0]).basename()+" -makeotus_mothur <alignment> <mergelog> <cutoff>");

  cout << "# loading seqs file: " << argv[1] << endl;
  load_seqs(argv[1],uarr);
  cdata.load(argv[2],uarr.size());

  float t=estr(argv[3]).f();
  earray<eintarray> otuarr;
  cdata.getOTU(t,otuarr,uarr.size());

  cout << "label\tnumOtus";
  for (long i=0; i<otuarr.size(); ++i)
    cout << "\tOTU" << i;
  cout << endl;

  cout << (1.0-t) << "\t" << otuarr.size();
  for (long i=0; i<otuarr.size(); ++i){
//    cout << ">OTU" << i << " otu_size="<< otuarr[i].size() << endl;
    cout << "\t" << uarr.keys(otuarr[i][0]);
    for (long j=1; j<otuarr[i].size(); ++j)
      cout << "," << uarr.keys(otuarr[i][j]);
  }
  cout << endl;

  exit(0);
}

void actionMakeOtus()
{
  estrarray uarr;
  eseqclusterData cdata;
  ldieif(argvc<4,"syntax: "+efile(argv[0]).basename()+" -makeotus <alignment> <mergelog> <cutoff>");

  cout << "# loading seqs file: " << argv[1] << endl;
  load_seqs(argv[1],uarr);
  cdata.load(argv[2],uarr.size());

  float t=estr(argv[3]).f();
  earray<eintarray> otuarr;
  cdata.getOTU(t,otuarr,uarr.size());

  for (long i=0; i<otuarr.size(); ++i){
    cout << ">OTU" << i << " otu_size="<< otuarr[i].size() << endl;
    for (long j=0; j<otuarr[i].size(); ++j)
      cout << uarr.keys(otuarr[i][j]) << endl;
  }

  exit(0);
}


void actionMakeReps()
{
  ldieif(argvc<3,"syntax: "+efile(argv[0]).basename()+" -makereps <alignment> <otu>");
  estrhashof<INDTYPE> seqind;

  estrarray uarr;

  cout << "# loading seqs file: " << argv[1] << endl;
  load_seqs_compressed(argv[1],arr,seqind,seqlen);
  load_seqs(argv[1],uarr);

  estrarrayof<ebasicarray<INDTYPE> > otus;

  efile f;
  estr line;
  estrarray parts;
  f.open(argv[2],"r");
  while (!f.eof()){
    f.readln(line);
    if (line.len()==0 || line[0]=='#') continue;
    if (line[0]=='>'){
      otus.add(line,ebasicarray<INDTYPE>());
      continue;
    }
    ldieif(otus.size()==0,"first entry not start of OTU or missing '>'");
    parts=line.explode("\t");
    ldieif(parts.size()==0,"array empty: "+line);
    ldieif(!seqind.exists(parts[0]),"sequence not found: "+parts[0]);
    otus[otus.size()-1].add(seqind[parts[0]]);
  }

  cerr << endl;

  ebasicarray<INDTYPE> tuniqind;
  earray<ebasicarray<INDTYPE> > dupslist;
  finduniq(tuniqind,dupslist);

  eintarray uniqmask;
  uniqmask.init(arr.size(),0);
  for (long i=0; i<tuniqind.size(); ++i)
    uniqmask[tuniqind[i]]=dupslist[i].size();


//  ebasicarray<INDTYPE> uniqind;
  taskman.createThread(nthreads);

  ebasicarray<INDTYPE> uniqind;
  const float t=0.0;
  efloatarray avgdist;
  for (long j=0; j<otus.size(); ++j){
//    cout << "# computing distances for otu "<< j << " size: " << otus[j].size() <<  endl;
    if (otus[j].size()==1){
      cout << otus.keys(j) << " " << arr.keys(otus[j][0]) << " avg_id=1.0 otu_size=1" << endl;
      cout << uarr.values(otus[j][0]) << endl;
      continue;
    }
    uniqind.clear();
    for (long l=0; l<otus[j].size(); ++l){
      if (uniqmask[otus[j][l]]!=0)
        uniqind.add(otus[j][l]);
    }
//    uniqind=otus[j];
    ldieif(uniqind.size()==0,"empty OTU");

    if (uniqind.size()==1){
      cout << otus.keys(j) << " " << arr.keys(uniqind[0]) << " avg_id=1.0 otu_size=" << otus[j].size() << endl;
      cout << uarr.values(uniqind[0]) << endl;
      continue;
    }
    avgdist.clear();
    avgdist.init(arr.size(),0.0);
    dists.clear();
  
    partsTotal=10000;
    if (partsTotal>(uniqind.size()-1l)*uniqind.size()/20l) partsTotal=(uniqind.size()-1l)*uniqind.size()/20l; // make fewer tasks if to few calculations per task
    if (partsTotal<=0) partsTotal=1;
    
    taskman.clear();
    for (long i=0; i<partsTotal; ++i)
      taskman.addTask(dfunc.value().calcfunc,evararray(mutex,uniqind,arr,dists,(const int&)seqlen,(const long int&)i,(const long int&)partsTotal,(const float&)t,(const int&)winlen));
    taskman.wait();
    for (long i=0; i<dists.size(); ++i){
      eseqdist& d(dists[i]);
      avgdist[d.x]+=d.dist*uniqmask[d.y];
      avgdist[d.y]+=d.dist*uniqmask[d.x];
//      cout << "# "<< arr.keys(d.x) << " " << arr.keys(d.y) << " " << d.dist << " " << uniqmask[d.x] << " " << uniqmask[d.y] << endl;
    }
    long k=uniqind[0];
    for (long i=0; i<uniqind.size(); ++i){
      long ti=uniqind[i];
      avgdist[ti]+=uniqmask[ti]-1;
      if (avgdist[k]<avgdist[ti]) {
//        cout << "# " << arr.keys(ti) << " " << ti << " " << uniqmask[ti] << " " << avgdist[ti] << " " << counts[ti] << endl;
        k=ti;
      }
    }
//    cout << "OTU" << j << " " << otus[j].size() << " " << arr.keys(k) << " " << avgdist[k]/(otus[j].size()-1) << " " << dists.size() << endl;
    cout << otus.keys(j) << " " << arr.keys(k) << " avg_id=" << avgdist[k]/(otus[j].size()-1) << " otu_size=" << otus[j].size() << endl;
    cout << uarr.values(k) << endl;
  }
  cerr << endl;

  exit(0);
}

void actionMakePart()
{
  ldieif(argvc<3,"syntax: "+efile(argv[0]).basename()+" -makepart <alignment> <cutoff>");

  cout << "# loading seqs file: " << argv[1] << endl;
  load_seqs_compressed(argv[1],arr,seqlen);

  t=estr(argv[2]).f();

  ebasicarray<INDTYPE> uniqind;
  earray<ebasicarray<INDTYPE> > dupslist;
  finduniq(uniqind,dupslist);
  cout << "# unique seqs: " << uniqind.size() << endl;

  ebasicarray<INDTYPE> otuid;
  otuid.reserve(uniqind.size());
  for (long i=0l; i<uniqind.size(); ++i)
    otuid.add(i);

  cout << "# computing partitions. threshold: " << t << endl;
  if (partsTotal>(arr.size()-1l)*arr.size()/20l) partsTotal=(arr.size()-1l)*arr.size()/20l; // make fewer tasks if to few calculations per task
//  partsTotal=1;
  for (long i=0; i<partsTotal; ++i)
    taskman.addTask(dfuncpart.value().calcfunc,evararray(mutex,uniqind,arr,otuid,(const int&)seqlen,(const long int&)i,(const long int&)partsTotal,(const float&)t,(const int&)winlen));

  taskman.createThread(nthreads);
  taskman.wait();

  cout << endl;

  ebasicarray<INDTYPE> newotuid;
  earray<ebasicarray<INDTYPE> > otus;
  newotuid.init(otuid.size(),-1l);
  long otucount=0;
  for (long i=0; i<otuid.size(); ++i){
    if (newotuid[otuid[i]]==-1l){
      newotuid[otuid[i]]=otucount;
      otus.add(ebasicarray<INDTYPE>());
      ++otucount;
    }
    otuid[i]=newotuid[otuid[i]];
    otus[otuid[i]].add(i);
  }
  cout << "# partitions: " << otus.size() << endl;

  for (long i=0; i<otus.size(); ++i){
    cout << otus[i].size() << ":";
    for (long j=0; j<otus[i].size(); ++j){
//      cout << " " << uniqind[otus[i][j]];
      for (long k=0; k<dupslist[otus[i][j]].size(); ++k)
        cout << " " << dupslist[otus[i][j]][k];
    }
    cout << endl;
  }

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

  dfuncpart.choice=0;
  dfuncpart.add("gap",edistfunc(part_calc_dists_u<estrarray,eseqdist,dist_compressed2>,dist_compressed2));
  dfuncpart.add("nogap",edistfunc(part_calc_dists_u<estrarray,eseqdist,dist_nogap_compressed2>,dist_nogap_compressed2));
  dfuncpart.add("gap2",edistfunc(part_calc_dists_u<estrarray,eseqdist,dist_compressed>,dist_compressed));
  dfuncpart.add("nogap2",edistfunc(part_calc_dists_u<estrarray,eseqdist,dist_nogap_compressed>,dist_nogap_compressed));
  dfuncpart.add("nogapsingle",edistfunc(part_calc_dists_u<estrarray,eseqdist,dist_nogapsingle_compressed>,dist_nogapsingle_compressed));
  dfuncpart.add("tamura",edistfunc(part_calc_dists_u<estrarray,eseqdist,dist_tamura_compressed>,dist_tamura_compressed));
  epregister(dfuncpart);

  dfunc.choice=0;
  dfunc.add("gap",edistfunc(t_calc_dists_u<estrarray,eseqdist,eblockarray<eseqdist>,dist_compressed2>,dist_compressed2));
  dfunc.add("nogap",edistfunc(t_calc_dists_u<estrarray,eseqdist,eblockarray<eseqdist>,dist_nogap_compressed2>,dist_nogap_compressed2));
  dfunc.add("gap2",edistfunc(t_calc_dists_u<estrarray,eseqdist,eblockarray<eseqdist>,dist_compressed>,dist_compressed));
  dfunc.add("nogap2",edistfunc(t_calc_dists_u<estrarray,eseqdist,eblockarray<eseqdist>,dist_nogap_compressed>,dist_nogap_compressed));
  dfunc.add("nogapsingle",edistfunc(t_calc_dists_u<estrarray,eseqdist,eblockarray<eseqdist>,dist_nogapsingle_compressed>,dist_nogapsingle_compressed));
  dfunc.add("tamura",edistfunc(t_calc_dists_u<estrarray,eseqdist,eblockarray<eseqdist>,dist_tamura_compressed>,dist_tamura_compressed));

  epregisterClass(eoption<edistfunc>);
  epregisterClassMethod4(eoption<edistfunc>,operator=,int,(const estr& val),"=");

  epregister(dfunc);

  epregister(winlen);

  estr ofile;
  estr dfile;
  estr dupfile;

  epregister(dupfile);
  epregister(ignoreUnique);
  epregister(t);
  epregister(nthreads);
  epregister(ofile);
  epregister(dfile);
  epregister(ignoreMemThres);

  getParser()->actions.add("makereps",actionMakeReps);
  getParser()->actions.add("makeotus",actionMakeOtus);
  getParser()->actions.add("makeotus_mothur",actionMakeOtusMothur);
  getParser()->actions.add("makepart",actionMakePart);
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

  cout << "# distance function: " << dfunc.key() << endl;

  if (ofile.len()==0)
    ofile=argv[1];

  epregisterClass(eseqdist);
  epregisterClassSerializeMethod(eseqdist);
  epregisterClassProperty(eseqdist,dist);
  epregisterClassProperty(eseqdist,x);
  epregisterClassProperty(eseqdist,y);

  epregisterClass(ebasicarray<eseqdist>);
  epregisterClassInheritance(ebasicarray<eseqdist>,ebasearray);
  epregisterClassMethod(ebasicarray<eseqdist>,subset);
  epregisterClassSerializeMethod(ebasicarray<eseqdist>);

  long i,j;
  cout << "# loading seqs file: " << argv[1] << endl;
  load_seqs_compressed(argv[1],arr,seqlen);
#ifndef HPC_CLUST_USE_LONGIND
  ldieif(arr.size() > (2l<<31),"To cluster more than 2 million sequences please recompile hpc-clust with the --enable-longind flag.");
#endif

  ebasicarray<INDTYPE> uniqind;
  earray<ebasicarray<INDTYPE> > dupslist;
  finduniq(uniqind,dupslist);
  cout << "# unique seqs: " << uniqind.size() << endl;


  if (dupfile.len()){
    efile dupf(dupfile,"w");
    for (i=0; i<dupslist.size(); ++i){
      dupf.write(estr(dupslist[i][0])+" "+estr(dupslist[i].size()));
      for (j=1; j<dupslist[i].size(); ++j)
        dupf.write(estr(" ")+dupslist[i][j]);
      dupf.write("\n");
    }
    dupf.close();
  }

  long maxdists=uniqind.size()*(uniqind.size()-1)/2;
  long maxmem=maxdists*sizeof(eseqdist)/1024/1024;
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

  efile df(dfile);
  cout << "# computing distances" << endl;
//  if ((arr.size()-1l)*arr.size()/2l/partsTotal > 10000l) partsTotal=(arr.size()-1l)*arr.size()/2l/10000l;  // make more tasks if too many calculations per task
  if (partsTotal>(arr.size()-1l)*arr.size()/20l) partsTotal=(arr.size()-1l)*arr.size()/20l; // make fewer tasks if to few calculations per task

//  cout << "partsTotal: " << partsTotal << endl;
  cerr << endl; // needed for keeping track of the progress

  for (i=0; i<partsTotal; ++i)
    taskman.addTask(dfunc.value().calcfunc,evararray(mutex,uniqind,arr,dists,(const int&)seqlen,(const long int&)i,(const long int&)partsTotal,(const float&)t,(const int&)winlen));

  taskman.createThread(nthreads);
  taskman.wait();
  cerr << endl;

  dtime=t1.lap()*0.001;
  cout << "# time calculating distances: " << dtime << endl;
  cout << "# distances within threshold: " << dists.size() << endl;

  cout << "# number of tasks: " << taskman.tasks.size() << endl;
  fradix256sort<eblockarray<eseqdist>,radixKey>(dists);
  cout << "# number of tasks: " << taskman.tasks.size() << endl;
  stime=t1.lap()*0.001;

  if (dfile.len()){
    cout << "# saving distances to file: "<<dfile << endl;
    for (i=0; i<dists.size(); ++i)
      df.write(estr(arr.keys(dists[i].x))+"\t"+arr.keys(dists[i].y)+"\t"+(1.0-dists[i].dist)+"\n");
/*
    for (i=0; i<dupslist.size(); ++i){
      for (j=1; j<dupslist[i].size(); ++j)
        df.write(estr(dupslist[i][0])+" "+dupslist[i][j]+" 1.0\n");
    }
*/
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
    alcluster.init(arr.size(),ofile+".al",argv[1],dupslist,t,dfunc.value().calcfunc_single,arr,seqlen);

  cout << "# starting clustering"<<endl;
  t1.reset();
  for (i=dists.size()-1; i>=0; --i){
    if (cl)
      clcluster.add(dists[i]);
    if (al)
      alcluster.add(dists[i]);
    if (sl)
      slcluster.add(dists[i]);
  }
  if (al)
    alcluster.finalize();

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

