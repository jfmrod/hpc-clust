#include <eutils/emain.h>
#include <eutils/efile.h>
#include <eutils/eheap.h>
#include <eutils/esystem.h>

#include "cluster-common.h"


estrarray arr;
earray<eseqdist> mindists;

eseqcluster cluster;
const int tbucket=10000;


void doClustering()
{
  cout << "# starting clustering " << endl;
  int i;

  cout << "# reading sorted distances to file \"sorted-dists.dat\"" << endl;
  estr datastr;
  mindists.serial(datastr);
  efile f("sorted-dists.bacteria.dat");
  f.read(datastr);
  f.close();
  ldieif(mindists.unserial(datastr,0)==-1,"error reading sorted-dists.bacteria.dat");
  cout << "# done" << endl;

  cout << "# min: " << mindists[0].dist << " max: "<<mindists[mindists.size()-1].dist << endl;
  for (i=mindists.size()-1; i>=0; --i){
      if (i%tbucket==0) { cout << i/tbucket << " "<< mindists[i].dist << " " << arr.size()-cluster.mergecount<<" " << cluster.smatrix.size() << endl; }
//    if (mindists.size()%tbucket==0) { cout << mindists.size()/tbucket << "."; flush(cout); }
//    if (mindists.size()/tbucket==55) exit(0);
    cluster.add(mindists[i]);
//    mindists.erase(i);
  }
  cout << endl;

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

  epregisterClass(earray<eseqdist>);
  epregisterClassInheritance(earray<eseqdist>,ebasearray);
  epregisterClassMethod(earray<eseqdist>,subset);
  epregisterClassSerializeMethod(earray<eseqdist>);

  ldieif(argvc<2,"syntax: "+efile(argv[0]).basename()+" <file>");

  load_accs(argv[1],arr);
  cluster.init(arr.size());

  doClustering();

  return(0);
}

