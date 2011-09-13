#include <eutils/emain.h>
#include <eutils/efile.h>
#include <eutils/eheap.h>
#include <eutils/esystem.h>

#include "cluster-common.h"

estrarray arr;
eseqcluster cl;


void readDistance(efile& f,eseqdist& sdist)
{
  estr tmpstr;
  long int i;
  sdist.serial(tmpstr);
  f.write(tmpstr);
}

void doClustering(const estr& seqsfile,const estr& inputfile)
{
  load_accs(seqsfile,arr);
  cl.init(arr.size(),"output.txt");

  cout << "# starting clustering " << endl;
  int i,lasti;

  efile f(inputfile);

  estr data;
  eseqdist sdist;
  while (f.read(data,1024)){
    lasti=0;
    for (i=sdist.unserial(data,0); i!=-1; i=sdist.unserial(data,i)){
//      cout << i << " "<< data.len() << " " <<  sdist.dist << endl;
      cl.add(sdist);
      lasti=i;
    }
//    cout << i << " " << lasti<<" "<< data.len() << endl;
    data=data.substr(lasti);
  }
}


int emain()
{
  estr inputfile;
  epregister(inputfile);
  eparseArgs(argvc,argv);

  epregisterClass(eseqdist);
  epregisterClassSerializeMethod(eseqdist);
  epregisterClassProperty(eseqdist,dist);
  epregisterClassProperty(eseqdist,x);
  epregisterClassProperty(eseqdist,y);

  ldieif(argvc<3,"syntax: "+efile(argv[0]).basename()+" <seqs> <dists>");

  doClustering(argv[1],argv[2]);

  return(0);
}

