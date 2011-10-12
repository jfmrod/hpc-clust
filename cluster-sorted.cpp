#include <eutils/emain.h>
#include <eutils/efile.h>
#include <eutils/eheap.h>
#include <eutils/esystem.h>

#include "cluster-common.h"
#include "eseqclusteravg.h"

estrarray arr;
eseqcluster cl;
eseqclustersingle sl;
eseqclusteravg al;


/*
void writeDistance(efile& f,eseqdist& sdist)
{
  estr tmpstr;
  long int i;
  sdist.serial(tmpstr);
  f.write(tmpstr);
}
*/

inline int readDistance(eseqdist& sdist,const estr& data,int i)
{
  uint32_t *pstr=(uint32_t*)&data._str[i];
  int j;
  if (data.len()-i<2*sizeof(uint32_t)+sizeof(float)) return(-1);
  sdist.x=*pstr; ++pstr;
  sdist.y=*pstr; ++pstr;
  sdist.dist=*(float*)pstr; ++pstr;
  return(i+2*sizeof(uint32_t)+sizeof(float));
}

void doClustering(const estr& seqsfile,const estr& inputfile,const estr& output)
{
  load_accs(seqsfile,arr);
  al.init(arr.size(),output+".al.dat",seqsfile);
  cl.init(arr.size(),output+".cl.dat",seqsfile);
  sl.init(arr.size(),output+".sl.dat",seqsfile);

  cout << "# starting clustering " << endl;
  int i,lasti;

  efile f(inputfile);

  estr data;
  eseqdist sdist;
  while (f.read(data,1024*4)){
    lasti=0;
    for (i=readDistance(sdist,data,0); i!=-1; i=readDistance(sdist,data,i)){
//      cout << i << " "<< data.len() << " " <<  sdist.dist << endl;
      al.add(sdist);
      cl.add(sdist);
      sl.add(sdist);
      lasti=i;
    }
//    cout << i << " " << lasti<<" "<< data.len() << endl;
    data=data.substr(lasti);
  }
}


int emain()
{
  estr inputfile;
  estr output="default";
  epregister(inputfile);
  epregister(output);
  eparseArgs(argvc,argv);

  epregisterClass(eseqdist);
  epregisterClassSerializeMethod(eseqdist);
  epregisterClassProperty(eseqdist,dist);
  epregisterClassProperty(eseqdist,x);
  epregisterClassProperty(eseqdist,y);

  ldieif(argvc<3,"syntax: "+efile(argv[0]).basename()+" <seqs> <dists>");

  doClustering(argv[1],argv[2],output);

  return(0);
}

