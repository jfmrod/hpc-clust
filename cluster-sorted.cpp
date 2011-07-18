#include <eutils/emain.h>
#include <eutils/efile.h>
#include <eutils/eheap.h>
#include <eutils/esystem.h>

#include "cluster-common.h"


estrarray arr;
ebasicarray<eseqdist> mindists;

eseqcluster cluster;
const int tbucket=10000;


void loadarray(ebasicarray<eseqdist>& dists,const estr& filename)
{
  estr data;
  efile f(filename);

  cout << "f.size: " << f.size() << endl;

  ldieif(!f.read(data),"error reading from file");
  cout << "data.len: " << data.len() << endl;
  long int i=0;
  eseqdist d;
  while (i!=-1 && data.len()){
    i=d.unserial(data,i);
    if (i==-1) break;
    dists.add(d);
    data.del(0,i);
    i=0;
  }
  f.close();

/*
  estr datastr;
  mindists.serial(datastr);
  efile f("distances.dat2");
  f.read(datastr);
  f.close();
  ldieif(mindists.unserial(datastr,0)==-1,"error reading sorted-dists.bacteria.dat");
*/
}


void doClustering()
{
  cout << "# starting clustering " << endl;
  int i;

  cout << "# reading sorted distances to file \"sorted-dists.dat\"" << endl;
  loadarray(mindists,"distances-sorted.dat");

  cout << "# done" << endl;

  cout << "# min: " << mindists[0].dist << " max: "<<mindists[mindists.size()-1].dist << endl;


  cluster.check(mindists);
//  for (i=mindists.size()-1; i>=0; --i){
//    cluster.add(mindists[i]);
//  }
//  cout << endl;

  cout << "# done" << endl;

//  while (1) {
//    datastr.clear();
//    mindists.serial(datastr);
//    mindists.clear();
//    mindists.unserial(datastr,0);
//  }
}


int emain()
{
  estr host;
  epregister(mindists);
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

  ldieif(argvc<2,"syntax: "+efile(argv[0]).basename()+" <file>");

  load_accs(argv[1],arr);
  cluster.init(arr.size(),"output.txt");

  doClustering();

  return(0);
}

