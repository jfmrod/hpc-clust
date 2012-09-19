#include "eseqclusterstep.h"
#include "cluster-common.h"

#include <eutils/efile.h>


void eseqclusterstep::init(int count,const estr& ofilename,const estr& seqsfile,const earray<eintarray>& dupslist) {
  ofile.open(ofilename,"w");
  ofile.write("# seqsfile: "+seqsfile+"\n");
  int i,j;
  scount.init(count,1);
  smerge.init(count,-1);
  otu.init(count,-1);
  incluster.init(count);
  mergecount=0;

  scluster.reserve(count);
  for (i=0; i<count; ++i){
    scluster.add(i);
    incluster[i].push_back(i);
  }
  for (i=0; i<dupslist.size(); ++i){
    for (j=1; j<dupslist[i].size(); ++j){
      ++mergecount;
      ofile.write(estr(scluster.size()-mergecount)+" 1.0 "+dupslist[i][0]+" "+dupslist[i][j]+"\n");
//      clusterData.mergearr.add(eseqdist(dupslist[i][0],dupslist[i][j],1.0));
    }
  }

  step=50;
  otucount=0;
  otumembers.clear();
  otudist.clear();
  cout << "# initializing cluster with: "<< count<< " seqs" << endl; 
}

void eseqclusterstep::merge(int x,int y,int dx,int dy,float dist)
{
  if (x==y) return;
  ldieif(scount[x]==0 || scount[y]==0,"also should not happen");

  smerge[x]=x;
  smerge[y]=x;

  list<int>::iterator it;

  if (scount[x]>=step && scount[y]>=step){
    if (otu[x]==-1){
      otumembers.add(eintarray());
      otudist.add(dist);
      for (it=incluster[x].begin(); it!=incluster[x].end(); ++it){
        otu[*it]=otucount;
        otumembers[otucount].add(*it);
      }
      ++otucount;
    }
    if (otu[y]==-1){
      otumembers.add(eintarray());
      otudist.add(dist);
      for (it=incluster[y].begin(); it!=incluster[y].end(); ++it){
        otu[*it]=otucount;
        otumembers[otucount].add(*it);
      }
      ++otucount;
    }
  }else{
    // here we add clusters of few sequences that should have been added to an already formed otu
    if (scount[y]<step){
      if (otu[dx]!=-1 && (otudist[otu[dx]]-dist)<0.05){
        for (it=incluster[y].begin(); it!=incluster[y].end(); ++it){
          otu[*it]=otu[dx];
          otumembers[otu[dx]].add(*it);
        }
      }else if (otu[x]!=-1){
        for (it=incluster[y].begin(); it!=incluster[y].end(); ++it){
          otu[*it]=otu[x];
          otumembers[otu[x]].add(*it);
        }
      }
    }
    if (scount[x]<step){
      if (otu[dy]!=-1 && (otudist[otu[dy]]-dist)<0.05){
        for (it=incluster[x].begin(); it!=incluster[x].end(); ++it){
          otu[*it]=otu[dy];
          otumembers[otu[dy]].add(*it);
        }
      }else if (otu[y]!=-1){
        for (it=incluster[y].begin(); it!=incluster[y].end(); ++it){
          otu[*it]=otu[y];
          otumembers[otu[y]].add(*it);
        }
      }
    }
/*
    // here we add clusters of few sequences that should have been added to an already formed otu
    if (scount[y]<step && otu[x]!=-1){
      for (it=incluster[y].begin(); it!=incluster[y].end(); ++it){
        otu[*it]=otu[x];
        otumembers[otu[x]].add(*it);
      }
    }
    if (scount[x]<step && otu[y]!=-1){
      for (it=incluster[x].begin(); it!=incluster[x].end(); ++it){
        otu[*it]=otu[y];
        otumembers[otu[y]].add(*it);
      }
    }
*/
  }

  scount[x]+=scount[y];
  scount[y]=0;

  for (it=incluster[y].begin(); it!=incluster[y].end(); ++it){
    scluster[*it]=x;
    incluster[x].push_back(*it);
  }
  ++mergecount;
  
//  cout << scluster.size()-mergecount << " " << dist << " " << x << " " << y << endl;
//  ofile.write(estr(scluster.size()-mergecount)+" "+dist+" "+x+" "+y+"\n");
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
  int i,j;
  estr otustr;
  estrhashof<eintarray> otus;

  efile f(filename);
  for (i=0; i<otumembers.size(); ++i){
    f.write(">OTU"+estr(i)+" "+otudist[i]+" "+otumembers[i].size()+"\n");
    for (j=0; j<otumembers[i].size(); ++j){
      f.write(arr.keys(otumembers[i][j])+"\n");
    }
  }
  f.close();
}

