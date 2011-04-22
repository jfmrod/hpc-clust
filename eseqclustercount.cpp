#include "eseqcluster.h"
#include "cluster-common.h"

#include <eutils/efile.h>

eseqdistCount::eseqdistCount(): count(1) {}
eseqdistCount::eseqdistCount(int _x,int _y,float _dist): x(_x),y(_y),dist(_dist),count(1) {}


void eseqdistCount::serial(estr& data) const
{
//  ::serial(x,data); ::serial(y,data); ::serial(dist,data);
  ::serial(x,data); ::serial(y,data); ::serial(dist,data); ::serial(count,data);
}

int eseqdistCount::unserial(const estr& data,int i)
{
  i=::unserial(x,data,i);
  if (i==-1) return(i);
  i=::unserial(y,data,i);
  if (i==-1) return(i);
  i=::unserial(dist,data,i);
  if (i==-1) return(i);
  i=::unserial(count,data,i);
  return(i);
}


eseqclusterCount::eseqclusterCount(){}


void eseqclusterCount::calc(estrarray& arr,int seqlen,int node,int tnodes,float thres)
{
  eblockarray<eseqdistCount> tmpdist;

  calc_dists_nogap_compressed(arr,tmpdist,seqlen,node,tnodes,thres);

  mutexDists.lock();
  dists.merge(tmpdist);
  mutexDists.unlock();
}



void eseqclusterCount::check(ebasicarray<eseqdistCount>& dists)
{
  int i;
  estr xystr;
  bool duplicate=false;
  for (i=0; i<dists.size(); ++i){
    if (i%(dists.size()/10)==0) { cout << i*10/dists.size(); flush(cout); }
    xy2estr(dists[i].x,dists[i].y,xystr);

    ebasichashmap<estr,int>::iter it;

//    cout << dists[i].dist << " " << dists[i].x << " " << dists[i].y;
    it=smatrix.get(xystr);
    if (it != smatrix.end()) {
      cout << "duplicate found: "+estr(dists[i].x)+","+dists[i].y << endl;
//      cout << " *";
      duplicate=true;
    }else
      smatrix.add(xystr,1);
//    cout << endl;
  }
  smatrix.clear();

  ldieif(duplicate,"duplicates found");
  cout << "# no duplicates found!" << endl;
}

void eseqclusterCount::init(int count) {
  int i;
  scount.reserve(count);
  scluster.reserve(count);
  smerge.reserve(count);
  inter.reserve(count);
  incluster.reserve(count);
  for (i=0; i<count; ++i){
    scount.add(1);
    scluster.add(i);
    smerge.add(-1);
    incluster.add(list<int>());
    incluster[i].push_back(i);
    inter.add(list<int>());
  }
  cout << "# initializing cluster with: "<< count<< " seqs" << endl; 
  cout << "# initializing smatrix with: " << (long int)(count)*(long int)(count)/(long int)(20000)<< " elements" << endl; 
  smatrix.reserve((long int)(count)*(long int)(count)/(long int)(20000));
//  cout << "# smatrix._hashitems = " << smatrix._hashitems << endl;
  mergecount=0;
}

int eseqclusterCount::update(int s)
{
//  int count=0;
  int i,j;

  int updcount=0;
  int updind[smerge.size()];
  for (i=0; i<smerge.size(); ++i)
    updind[i]=-1;

  for (i=0; i<smerge.size(); ++i){
    if (smerge[i]>=0){
      if (updind[smerge[i]]==-1) {
        updind[smerge[i]]=updcount;
        ++updcount;
      }
      updind[i]=updind[smerge[i]];
    }
  }

  if (updcount==0) return(0);

  cout << "# updating: " << updcount << " merges smerge.size: "<<smerge.size()<<endl;

  int *uarr=new int[updcount*smerge.size()];
  ldieif (uarr==0x00,"not enough memory");
  for (j=0; j<updcount*smerge.size(); ++j)
    uarr[j]=-1;

  for (i=s; i>=0; --i){
    if (dists[i].count==0) continue;

    if (smerge[dists[i].x]>=0){
      j=uarr[updind[smerge[dists[i].x]]*smerge.size()+scluster[dists[i].y]];
      if (j>=0){
        dists[i].count+=dists[j].count;
        dists[j].count=0;
      }
      uarr[updind[smerge[dists[i].x]]*smerge.size()+scluster[dists[i].y]]=i;
    }else if (smerge[dists[i].y]>=0){
      j=uarr[updind[smerge[dists[i].y]]*smerge.size()+scluster[dists[i].x]];
      if (j>=0){
        dists[i].count+=dists[j].count;
        dists[j].count=0;
      }
      uarr[updind[smerge[dists[i].y]]*smerge.size()+scluster[dists[i].x]]=i;
    }
  }

  for (i=0; i<smerge.size(); ++i)
    smerge[i]=-1;

  delete uarr;
  return(0);
//  return(count);
}

int eseqclusterCount::update(int s,int x,int y)
{
//  int count=0;
  int i,j;

  eintarray uarr;
  uarr.reserve(smerge.size());
  for (i=0; i<smerge.size(); ++i)
    uarr.add(-1);

  for (i=s; i>=0; --i){
    if (dists[i].count==0) continue;

    if (scluster[dists[i].x]==x || scluster[dists[i].x]==y){
      j=uarr[scluster[dists[i].y]];
      if (j>=0){
        dists[i].count+=dists[j].count;
        dists[j].count=0;
//        ++count;
      }
      uarr[scluster[dists[i].y]]=i;
    }else if (scluster[dists[i].y]==x || scluster[dists[i].y]==y){
      j=uarr[scluster[dists[i].x]];
      if (j>=0){
        dists[i].count+=dists[j].count;
        dists[j].count=0;
//        ++count;
      }
      uarr[scluster[dists[i].x]]=i;
    }
  }
  return(0);
//  return(count);
}

int eseqclusterCount::update(eblockarray<eseqdistCount>& dists,int s)
{
  int count=0;
  int i,j;
  ebasicstrhashof<int> cmatrix;
  ebasichashmap<estr,int>::iter it;
  estr tmps; 

  for (i=s; i>=0; --i){
    if (dists[i].count==0) continue;

    if (smerge[dists[i].x]>=0 || smerge[dists[i].y]>=0){
      xy2estr(scluster[dists[i].x],scluster[dists[i].y],tmps);
      it=cmatrix.get(tmps);
      if (it==cmatrix.end())
        cmatrix.add(tmps,i);
      else{
        j=*it;
        dists[i].count+=dists[j].count;
        dists[j].count=0;
        *it=i;
        ++count;
      }
    }
  }

  for (i=0; i<smerge.size(); ++i)
    smerge[i]=-1;

  return(count);
}

void eseqclusterCount::merge(int x,int y)
{
  ldieif(x==y,"should not happen!");
  ldieif(scount[x]==0 || scount[y]==0,"also should not happen");

  smerge[x]=x;
  smerge[y]=x;

  scount[x]+=scount[y];
  scount[y]=0;

  list<int>::iterator it;
  for (it=incluster[y].begin(); it!=incluster[y].end(); ++it){
    scluster[*it]=x;
    incluster[x].push_back(*it);
  }

  estr tmpstr,tmpstr2;

  int i,j;
  for (it=inter[y].begin(); it!=inter[y].end(); ++it){
    j=scluster[*it];
    if (x==j || y==j) continue;
    xy2estr(x,j,tmpstr);
    xy2estr(y,j,tmpstr2);

    ebasichashmap<estr,int>::iter tmpit2=smatrix.get(tmpstr2);
    if(tmpit2==smatrix.end()) continue;

    ebasichashmap<estr,int>::iter tmpit=smatrix.get(tmpstr);
    if (tmpit!=smatrix.end())
      *tmpit+=*tmpit2;
    else{
      smatrix.add(tmpstr,*tmpit2);
      inter[x].push_back(j);
    }
    smatrix.erase(tmpit2);
  }
  ++mergecount;
}

void eseqclusterCount::add(int ind){
  if (dists[ind].count==0) return;
  ldieif(dists[ind].x<0 || dists[ind].y<0 || dists[ind].x>=scluster.size() || dists[ind].y>=scluster.size(),"out of bounds: sdist.x: "+estr(dists[ind].x)+" sdist.y: "+estr(dists[ind].y)+" scluster.size(): "+estr(scluster.size()));

  int x=scluster[dists[ind].x];
  int y=scluster[dists[ind].y];

  ldieif(x<0 || y<0 || x>=scluster.size() || y>=scluster.size(),"out of bounds: sdist.x: "+estr(x)+" sdist.y: "+estr(y)+" scluster.size(): "+estr(scluster.size()));
  int tmp;
  if (x>y) { tmp=x; x=y; y=tmp; tmp=dists[ind].x; dists[ind].x=dists[ind].y; dists[ind].y=tmp; }

  int links;
  int i;
  estr xystr;

//  cout << x << " " << y << " " << sdist.dist << endl;
  ldieif(x==y,"should not happen: "+estr(x)+","+estr(y)+" --- "+estr(dists[ind].x)+","+estr(dists[ind].y));

  xy2estr(x,y,xystr);

  ebasichashmap<estr,int>::iter it;

  it=smatrix.get(xystr);
  if (it==smatrix.end()){
    if (scount[x]*scount[y]==dists[ind].count){
//    if (scount[x]*scount[y]==1){
      merge(x,y);
//      update(ind-1,x,y);
      cout << scluster.size()-mergecount << " " << dists[ind].dist << " " << x << " " << y << endl;
//      sleep(1);
    }else{
      smatrix.add(xystr,dists[ind].count);
//      smatrix.add(xystr,1);
      inter[x].push_back(y); inter[y].push_back(x);
    }
    return;
  }

  (*it)+=dists[ind].count;
//  ++(*it);

  // complete linkage
  if ((*it)==scount[x]*scount[y]){
    merge(x,y);
//    update(ind-1,x,y);
    cout << scluster.size()-mergecount << " " << dists[ind].dist << " " << x << " " << y << endl;
//    sleep(1);
//    cout << sdist.dist << " " << x << " " << y << endl;
    smatrix.erase(it);
  }
}

void eseqclusterCount::save(const estr& filename,const estrarray& arr)
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

