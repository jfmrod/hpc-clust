#include "eseqclusterstep.h"
#include "cluster-common.h"

#include <eutils/efile.h>


void eseqclusterstep::init(int count,const estr& ofilename,const estr& seqsfile) {
  ofile.open(ofilename,"w");
  ofile.write("# seqsfile: "+seqsfile);
  int i;
  scount.reserve(count);
  scluster.reserve(count);
  smerge.reserve(count);
  incluster.reserve(count);
  for (i=0; i<count; ++i){
    scount.add(1);
    scluster.add(i);
    smerge.add(-1);
    incluster.add(list<int>());
    incluster[i].push_back(i);
  }
  cout << "# initializing cluster with: "<< count<< " seqs" << endl; 
  mergecount=0;
}

void eseqclusterstep::merge(int x,int y,int dx,int dy,float dist)
{
  if (x==y) return;
  ldieif(scount[x]==0 || scount[y]==0,"also should not happen");

  smerge[x]=x;
  smerge[y]=x;

  scount[x]+=scount[y];

  if (scount[x]>=step && scount[y]>=step){
    

  }else{

    if (scount[y]<step){
      otu[y]=otu[x];
      scount[y]=0;
    }
  }

  list<int>::iterator it;
  for (it=incluster[y].begin(); it!=incluster[y].end(); ++it){
    scluster[*it]=x;
    incluster[x].push_back(*it);
  }
  ++mergecount;
  
//  cout << scluster.size()-mergecount << " " << dist << " " << x << " " << y << endl;
  ofile.write(estr(scluster.size()-mergecount)+" "+dist+" "+x+" "+y+"\n");
}

void eseqclusterstep::add(eseqdist& sdist){
//  if (sdist.count==0) return;
  ldieif(sdist.x<0 || sdist.y<0 || sdist.x>=scluster.size() || sdist.y>=scluster.size(),"out of bounds: sdist.x: "+estr(sdist.x)+" sdist.y: "+estr(sdist.y)+" scluster.size(): "+estr(scluster.size()));

  int x=scluster[sdist.x];
  int y=scluster[sdist.y];

  ldieif(x<0 || y<0 || x>=scluster.size() || y>=scluster.size(),"out of bounds: sdist.x: "+estr(x)+" sdist.y: "+estr(y)+" scluster.size(): "+estr(scluster.size()));
  int tmp;
  if (x>y) { tmp=x; x=y; y=tmp; tmp=sdist.x; sdist.x=sdist.y; sdist.y=tmp; }

  merge(x,y,sdist.x,sdist.y,sdist.dist);
}

void eseqclusterstep::save(const estr& filename,const estrarray& arr)
{
  int i;
  estr otustr;
  estrhashof<eintarray> otus;

  efile f(filename+".clstr");
  for (i=0; i<scluster.size(); ++i)
    f.write(estr(scluster[i])+"     "+arr.keys(i)+"\n");
  f.close();

  list<int>::iterator it;
  int otucount=0;
  efile f2(filename);
  for (i=0; i<incluster.size(); ++i){
    if (scount[i]==0) continue;
    f2.write(">OTU"+estr(otucount)+"\n");
    for (it=incluster[i].begin(); it!=incluster[i].end(); ++it)
      f2.write(arr.keys(*it)+"\n");
    ++otucount;
  }
  f2.close();
}

