#include "eseqclustersingle.h"
#include "cluster-common.h"

#include <eutils/efile.h>


eseqclustersingle::eseqclustersingle(){}


void eseqclustersingle::init(int count,const estr& ofilename,const estr& seqsfile,const earray<eintarray>& dupslist) {
  ofile.open(ofilename,"w");
  ofile.write("# seqsfile: "+seqsfile+"\n");
  ofile.write("# OTU_count Merge_distance Merged_OTU_id1 Merged_OTU_id2\n");
  int i,j;
  mergecount=0;
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
  for (i=0; i<dupslist.size(); ++i){
    for (j=1; j<dupslist[i].size(); ++j){
      ++mergecount;
      ofile.write(estr(scluster.size()-mergecount)+" 1.0 "+dupslist[i][0]+" "+dupslist[i][j]+"\n");
      clusterData.mergearr.add(eseqdist(dupslist[i][0],dupslist[i][j],1.0));
    }
  }
  cout << "# initializing cluster with: "<< count<< " seqs" << endl; 
}

void eseqclustersingle::merge(int x,int y,float dist)
{
  if (x==y) return;
  ldieif(scount[x]==0 || scount[y]==0,"also should not happen");

  clusterData.mergearr.add(eseqdist(x,y,dist));

  smerge[x]=x;
  smerge[y]=x;

  scount[x]+=scount[y];
  scount[y]=0;

  list<int>::iterator it;
  for (it=incluster[y].begin(); it!=incluster[y].end(); ++it){
    scluster[*it]=x;
    incluster[x].push_back(*it);
  }
  ++mergecount;
  
//  cout << scluster.size()-mergecount << " " << dist << " " << x << " " << y << endl;
  ofile.write(estr(scluster.size()-mergecount)+" "+dist+" "+x+" "+y+"\n");
}

void eseqclustersingle::add(const eseqdist& sdist){
//  if (sdist.count==0) return;
  ldieif(sdist.x<0 || sdist.y<0 || sdist.x>=scluster.size() || sdist.y>=scluster.size(),"out of bounds: sdist.x: "+estr(sdist.x)+" sdist.y: "+estr(sdist.y)+" scluster.size(): "+estr(scluster.size()));

  int x=scluster[sdist.x];
  int y=scluster[sdist.y];

  ldieif(x<0 || y<0 || x>=scluster.size() || y>=scluster.size(),"out of bounds: sdist.x: "+estr(x)+" sdist.y: "+estr(y)+" scluster.size(): "+estr(scluster.size()));
  int tmp;
  if (x>y) { tmp=x; x=y; y=tmp; }

  merge(x,y,sdist.dist);
}

/*
void eseqclustersingle::add(int ind){
//  if (dists[ind].count==0) return;
  ldieif(dists[ind].x<0 || dists[ind].y<0 || dists[ind].x>=scluster.size() || dists[ind].y>=scluster.size(),"out of bounds: dists[ind].x: "+estr(dists[ind].x)+" dists[ind].y: "+estr(dists[ind].y)+" scluster.size(): "+estr(scluster.size()));

  int x=scluster[dists[ind].x];
  int y=scluster[dists[ind].y];

  ldieif(x<0 || y<0 || x>=scluster.size() || y>=scluster.size(),"out of bounds: dists[ind].x: "+estr(x)+" dists[ind].y: "+estr(y)+" scluster.size(): "+estr(scluster.size()));
  int tmp;
  if (x>y) { tmp=x; x=y; y=tmp; tmp=dists[ind].x; dists[ind].x=dists[ind].y; dists[ind].y=tmp; }

  merge(x,y,dists[ind].dist);
}
*/

void eseqclustersingle::save(const estr& filename,const estrarray& arr)
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

