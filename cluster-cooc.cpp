#include <eutils/emain.h>
#include <eutils/efile.h>
#include <eutils/eheap.h>
#include <eutils/etimer.h>

#include "cluster-common.h"

eseqcluster cluster;

int partsFinished=0;
int partsTotal=100;
estrarray arr;
estrhashof<int> arrind;
unsigned totaldists;

ebasicarray<float> cooc_dist_mat;

int seqlen=0;

int emain()
{ 
  estr ofile="cluster.dat";
  estr dfile;
  float t=0.90;
  int ncpus=1;
  epregister(t);
  epregister(ncpus);
  epregister(ofile);
  epregister(dfile);
  estr outfile="cooc_distances.dat";
  epregister(outfile);
  eparseArgs(argvc,argv);

  epregisterClass(eseqdist);
  epregisterClassSerializeMethod(eseqdist);
  epregisterClassProperty(eseqdist,dist);
  epregisterClassProperty(eseqdist,x);
  epregisterClassProperty(eseqdist,y);

  epregisterClass(ebasicarray<eseqdist>);
  epregisterClassInheritance(ebasicarray<eseqdist>,ebasearray);
  epregisterClassMethod(ebasicarray<eseqdist>,subset);
  epregisterClassSerializeMethod(ebasicarray<eseqdist>);


  ldieif(argvc<4,"syntax: "+efile(argv[0]).basename()+" <seqs> <clusters> <samples>");

  int i;
  load_seqs_compressed(argv[1],arr,arrind,seqlen);

/*  
  float dtime,stime;
  etimer t1;
  t1.reset();

  etaskman taskman;

  efile df(dfile);
  if (dfile.len()==0 || !df.exists()){
    cout << "# computing distances" << endl;
    for (i=0; i<partsTotal; ++i)
      taskman.addTask(efunc(cluster,&eseqcluster::calc),evararray(arr,(const int&)seqlen,(const int&)i,(const int&)partsTotal,(const float&)t));

    taskman.createThread(ncpus);
    cout << "# finished creating threads: "<<ncpus << endl;
    taskman.wait();
    cout << "# all tasks finished" << endl;

    dtime=t1.lap()*0.001;
    cout << "# time calculating distances: " << dtime << endl;
    cout << "# distances within threshold: " << cluster.dists.size() << endl;

    cluster.dists.sort();
    stime=t1.lap()*0.001;

/*
    if (dfile.len()){
      cout << "# saving distances to file: "<<dfile << endl;
      estr str;
      mindists.serial(str);
      df.write(str);
      df.close();
    }
*//*
  }else{


    cout << "# loading distances from file: "<<dfile << endl;

/*
    estr str;
    df.read(str);
    ldieif(mindists.unserial(str,0)==-1,"problem loading distance file: "+dfile);
    df.close();
*//*
  } 

  totaldists=cluster.dists.size();
  cout << "# time sorting distances: " << stime << endl;

  cout << "# initializing cluster"<<endl;
  cl5uster.init(arr.size());

  cout << "# starting clustering"<<endl;
  t1.reset();
  int tmp;
  for (i=cluster.dists.size()-1; i>=0; --i){
    cluster.add(i);
//    cluster.update(i-1);
//    if ((arr.size()-cluster.mergecount)%1000==0) cluster.update(i-1);
    if (i%10000==0) { cout << i/10000 << " "<< cluster.dists[i].dist << " " << arr.size()-cluster.mergecount << " " << cluster.smatrix.size() << endl; }
//    if (i%10000==0) { tmp=cluster.update(mindists,i-1); cout << i/10000 << " "<< mindists[i].dist << " " << arr.size()-cluster.mergecount << " " << cluster.smatrix.size() << " " << tmp << endl; }
  }
  float clustime=t1.lap()*0.001;
  cout << "# time calculating distances: " << dtime << endl;
  cout << "# time sorting distances: " << stime << endl;
  cout << "# time clustering: " << clustime << endl;
  cout << "# total time: " << dtime+clustime+stime << endl;
  cout << "# distances within threshold: " << totaldists << endl;

  cluster.save(ofile,arr);
  cout << "# done writing "<<ofile << endl;
*/

  eintarray otuarr;
  int otucount;
  load_clusters(argv[2],arrind,otuarr,otucount);

  estrhashof<eintarray> samples;
  estrhash accsample;
  load_samples(argv[3],arrind,samples,accsample);


  earray<eintarray> neigharr;
  cooc_init(neigharr,arr,otuarr,samples,accsample,arrind,otucount);

/*
  cooc_dist_mat.reserve(arr.size()*(arr.size()+1)/2-arr.size());
  for (i=0; i<arr.size()*(arr.size()+1)/2-arr.size(); ++i)
    cooc_dist_mat.add(0.0);
*/

//  cooc_calc(0,arr.size()/2,cooc_dist_mat,neigharr);
  cout << "# Calculating cooc distances" << endl;
  cooc_calc2(0,10,cooc_dist_mat,neigharr);

  cout << "# Writing cooc distances to file" << endl;
  efile of(outfile);
  for (i=0; i<cooc_dist_mat.size(); ++i)
    of.write(estr(cooc_dist_mat[i])+"\n");
  of.close();


/*
  efile of(outfile);
  int otucount=0;
  for (i=0; i<otus.size(); ++i){
    if (otus[i].size()==0) continue;
    of.write(">OTU"+estr(otucount)+" "+estr(otus[i].size())+"\n");
    for (j=0; j<otus[i].size(); ++j)
      of.write(arr.keys(otus[i][j])+"\n");
    ++otucount;
  }
*/

  return(0);
}

