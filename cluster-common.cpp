#include "cluster-common.h"
#include <eutils/efile.h>
#include <eutils/eheap.h>
#include <math.h>

void cluster_init(earray<eintarray>& cvec,const estrhashof<int>& arrind,const eintarray& otuarr,int otucount)
{
  int i,j;

  for (i=0; i<otucount; ++i){
    cvec.add(eintarray());
    cvec[i].reserve(arrind.size());
    for (j=0; j<arrind.size(); ++j)
      cvec[i].add(0);
  }

  for (i=0; i<arrind.size(); ++i){
    j=otuarr[i]; 
    if (j==-1) continue;
    cvec[j][i]=1;
  }
}
void cooc_init(earray<eintarray>& neigharr,const estrhash& arr,const eintarray& otuarr,estrhashof<earray<estr> >& samples,estrhash& accsample,estrhashof<int>& arrind,int otucount)
{
 
  int i,k,l;
  estr acc;
  estr sample;

  cout << "# otucount: " << otucount << endl;


  for (i=0; i<arr.size(); ++i)
    neigharr.add(eintarray());

  cout << "# neighcount: " << neigharr.size() << endl;
  for (i=0; i<arr.size(); ++i){
    acc=arr.keys(i);
    if (!accsample.exists(acc)) continue;
    sample=accsample[acc];

    for (k=0; k<otucount; ++k)
      neigharr[i].add(0);

    ldieif(!samples.exists(sample),"sample not found: "+sample);
    for (k=0; k<samples[sample].size(); ++k){
      if (acc==samples[sample][k]) continue;
      l=otuarr[arrind[samples[sample][k]]];
      ldieif(l<0 || l>=otucount,"Something wrong");
//      cout << " i: " << i << " l: " << l << endl;
      ++neigharr[i][l];
    }
  }

  cout << "# neigharr.size(): " << neigharr.size() << endl;
}

void cooc_calc(int start,int end,ebasicarray<float>& dist_mat,earray<eintarray>& neigharr)
{
  int c=0;
  int i,i2,j;

  for (i=start; i<end; ++i){
    getL(c,neigharr.size(),i,i+1);
    for (j=i+1; j<neigharr.size(); ++j)
      dist_mat[c++]=cooc_dist(i,j,neigharr);
    i2=neigharr.size()-i-1;
    for (j=i2+1; j<neigharr.size(); ++j)
      dist_mat[c++]=cooc_dist(i2,j,neigharr);
  }
  if (end==neigharr.size()/2 && neigharr.size()%2==1){
    getL(c,neigharr.size(),i,i+1);
    i=neigharr.size()/2;
    for (j=i+1; j<neigharr.size(); ++j)
      dist_mat[c++]=cooc_dist(i,j,neigharr);
  }
}




void load_accs(const estr& filename,estrarray& arr)
{
  estr line;
  estr name;
  efile f(filename);

  int i;

  while (f.readln(line)){
    if (line.len()==0 || line[0]=='#') continue;
    
    i=line.find(" ");
    if (i==-1) continue;
    name=line.substr(0,i);
    line.del(0,i);
    line.trim();
//    if (!arr.exists(name)){
      arr.add(name,estr());
//    }else
//      break;
  }
  cout << "# seqs: " << arr.size() << endl;
}

int countGaps(const estr& seq)
{
  int i;
  int gaps=0;
  for (i=0; i<seq.len(); ++i)
    if (seq._str[i]=='-') ++gaps;
  return(gaps);
}

void load_seqs(const estr& filename,estrarray& arr,eintarray& arrgaps)
{
  estr line;
  estr name;
  efile f(filename);

  int i;

  while (f.readln(line)){
    if (line.len()==0 || line[0]=='#') continue;
    
    i=line.find(" ");
    if (i==-1) continue;
    name=line.substr(0,i);
    line.del(0,i);
    line.trim();
//    if (!arr.exists(name)){
    arr.add(name,line.uppercase());
    arrgaps.add(countGaps(line)); 
//      arr[name].reserve(1500);
//    }else
//      arr[name]+=line;
  }
  cout << "# seqs: " << arr.size() << endl;
}

void load_seqs(const estr& filename,estrarray& arr)
{
  estr line;
  estr name;
  efile f(filename);

  int i;

  while (f.readln(line)){
    if (line.len()==0 || line[0]=='#') continue;
    
    i=line.find(" ");
    if (i==-1) continue;
    name=line.substr(0,i);
    line.del(0,i);
    line.trim();
//    if (!arr.exists(name)){
      arr.add(name,line.uppercase());
//      arr[name].reserve(1500);
//    }else
//      arr[name]+=line;
  }
  cout << "# seqs: " << arr.size() << endl;
}

void load_seqs(const estr& filename,estrhash& arr)
{
  estr line;
  estr name;
  efile f(filename);

  int i;

  while (f.readln(line)){
    if (line.len()==0 || line[0]=='#') continue;
    
    i=line.find(" ");
    if (i==-1) continue;
    name=line.substr(0,i);
    line.del(0,i);
    line.trim();
    if (!arr.exists(name)){
      arr.add(name,line.uppercase());
      arr[name].reserve(1500);
    }else
      arr[name]+=line.uppercase();
  }
  cout << "# seqs: " << arr.size() << endl;
}

void load_seqs(const estr& filename,estrhashof<int>& arrind)
{
  estr line;
  estr name;
  efile f(filename);

  int i;

  while (f.readln(line)){
    if (line.len()==0 || line[0]=='#') continue;
	    
    i=line.find(" ");
    if (i==-1) continue;
    name=line.substr(0,i);
    line.del(0,i);
    line.trim();
    if (!arrind.exists(name))
      arrind.add(name,arrind.size());
  }
  cout << "# seqs: " << arrind.size() << endl;
}

void load_clusters(const estr& filename,estrhashof<int>& arrind,eintarray& otuarr,int& otucount)
{
  estr line;
  estr name;
  efile f(filename);

  int i;

  for (i=0; i<arrind.size(); ++i)
    otuarr.add(-1);

  otucount=-1;
  while (f.readln(line)){
    if (line.len()==0 || line[0]=='#') continue;

    if (line[0]=='>') { ++otucount; continue; }
	    
    ldieif(!arrind.exists(line),"something wrong");

    otuarr[arrind[line]]=otucount;

//    if (!arr.exists(name)){
//      arr.add(name,line);
//      arr[name].reserve(1500);
//    }else
//      arr[name]+=line;
  }
  ++otucount;
  cout << "# otucount: " << otucount << endl;
}

eseqdist::eseqdist() {}
eseqdist::eseqdist(int _x,int _y,float _dist): x(_x),y(_y),dist(_dist) {}
//eseqdist::eseqdist(): count(1) {}
//eseqdist::eseqdist(int _x,int _y,float _dist): x(_x),y(_y),dist(_dist),count(1) {}


void eseqdist::serial(estr& data) const
{
  ::serial(x,data); ::serial(y,data); ::serial(dist,data);
//  ::serial(x,data); ::serial(y,data); ::serial(dist,data); ::serial(count,data);
}

int eseqdist::unserial(const estr& data,int i)
{
  i=::unserial(x,data,i);
  if (i==-1) return(i);
  i=::unserial(y,data,i);
  if (i==-1) return(i);
  i=::unserial(dist,data,i);
//  if (i==-1) return(i);
//  i=::unserial(count,data,i);
  return(i);
}


bool eseqcount::operator==(const eseqcount& scount) const{ return(x==scount.x&&y==scount.y || x==scount.y&&y==scount.x); }

unsigned int seqcount_hash(const eseqcount& scount){ return(scount.x*scount.y); }

//eseqcluster::eseqcluster(): smatrix(seqcount_hash) {}
eseqcluster::eseqcluster(){}

void eseqcluster::check(ebasicarray<eseqdist>& dists)
{
  int i;
  estr xystr;
//  eseqcount xy;
  bool duplicate=false;
  for (i=0; i<dists.size(); ++i){
    xy2estr(dists[i].x,dists[i].y,xystr);
//    xy.x=dists[i].x; xy.y=dists[i].y;
    if (smatrix.exists(xystr)) { cout << "duplicate found: "+estr(dists[i].x)+","+dists[i].y << endl; duplicate=true; }
    smatrix.add(xystr,1);
  }
  smatrix.clear();

  ldieif(duplicate,"duplicates found");
  cout << "# no duplicates found!" << endl;
}

void eseqcluster::init(int count) {
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
//    inter.add(eintarray());
//    inter[i].reserve(3000);
  }
  smatrix.reserve((long int)(count)*(long int)(count)/(long int)(200));
  mergecount=0;
}

/*
int eseqcluster::update(ebasicarray<eseqdist>& dists,int s)
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
*/

void eseqcluster::merge(int x,int y)
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

void eseqcluster::add(eseqdist& sdist){
//  if (sdist.count==0) return;

  int x=scluster[sdist.x];
  int y=scluster[sdist.y];
  int tmp;
  if (x>y) { tmp=x; x=y; y=tmp; tmp=sdist.x; sdist.x=sdist.y; sdist.y=tmp; }

  int links;
  int i;
  estr xystr;

  ldieif(x==y,"should not happen: "+estr(x)+","+estr(y)+" --- "+estr(sdist.x)+","+estr(sdist.y));

  xy2estr(x,y,xystr);

  ebasichashmap<estr,int>::iter it;

  it=smatrix.get(xystr);
  if (it==smatrix.end()){
//    if (scount[x]*scount[y]==sdist.count){
    if (scount[x]*scount[y]==1){
      merge(x,y);
    }else{
//      smatrix.add(xystr,sdist.count);
      smatrix.add(xystr,1);
      inter[x].push_back(y); inter[y].push_back(x);
    }
    return;
  }

//  *it+=sdist.count;
  ++(*it);

  // complete linkage
  if (*it==scount[x]*scount[y]){
    merge(x,y);
    smatrix.erase(it);
  }
}

/*
void eseqcluster::merge(int x,int y)
{
  ldieif(x==y,"should not happen!");
  int i;
  cout << "merging: " << x << " --- " << y << endl;
  flush(cout);
  scount[x]+=scount[y];
  scount[y]=0;

  cout << " updating cluster parents: "<< incluster[y].size() << endl;
  flush(cout);
  for (i=0; i<incluster[y].size(); ++i)
    scluster[incluster[y][i]]=x;
  cout << " adding incluster seqs: "<< incluster[y].size() << endl;
  flush(cout);
  incluster[x]+=incluster[y];
  cout << " clearing array: "<< incluster[y].size() << endl;
  flush(cout);
  incluster[y].clear();

  cout << " summing inter distances: "<< inter[y].size() << endl;
  flush(cout);
  estr tmpstr,tmpstr2;
  int j;
  for (i=0; i<inter[y].size(); ++i){
//    j=inter[y][i];
    j=scluster[inter[y][i]];
    xy2estr(x,j,tmpstr);
    xy2estr(y,j,tmpstr2);
    if (x==j || y==j) continue;
    if(!smatrix.exists(tmpstr2)) continue;
//    ldieif(!smatrix.exists(tmpstr2),"something wrong: "+estr(y)+" -> "+estr(j)+" ("+estr(inter[y][i])+")");

    if (smatrix.exists(tmpstr))
      smatrix[tmpstr]+=smatrix[tmpstr2];
    else{
      smatrix.add(tmpstr,smatrix[tmpstr2]);
      inter[x].add(j);
//      inter[j].add(x);
    }
//    cout << "# removing: " << y << " -> " << j << " (" <<inter[y][i]<<")"<<endl;
    smatrix.erase(tmpstr2);
  }
  inter[y].clear();
  ++mergecount;
  cout << "done merging" << endl;
  flush(cout);
}

void eseqcluster::add(eseqdist& sdist){
  int x=scluster[sdist.x];
  int y=scluster[sdist.y];
  int tmp;
  if (x>y) { tmp=x; x=y; y=tmp; tmp=sdist.x; sdist.x=sdist.y; sdist.y=tmp; }

  int links;
  int i;
  estr xystr;

//  ldieif(x==y,"should not happen: "+estr(x)+","+estr(y)+" --- "+estr(sdist.x)+","+estr(sdist.y));

  xy2estr(x,y,xystr);
  if (!smatrix.exists(xystr)){
    if (scount[x]*scount[y]==1){
      merge(x,y);
    }else{
      smatrix.add(xystr,1);
      inter[x].add(y); inter[y].add(x);
    }
    return;
  }

  links=++smatrix[xystr];

  // complete linkage
  if (links==scount[x]*scount[y]){
//    cout << "Merging: " << x <<" ("<<sdist.x<<") "<< " with " << y << " ("<< sdist.y<<") "<< "dist: " <<sdist.dist<<" " << links << endl;
    merge(x,y);
    smatrix.erase(xystr);
//    i=inter[x].find(sdist.y);
//    if (i==-1)
//      i=inter[x].find(y);
//    ldieif(i==-1,"y not found?: "+estr(y)+" ("+estr(sdist.y)+")");
//    inter[x].erase(i);
  }
}
*/
void eseqcluster::save(const estr& filename,const estrarray& arr)
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

/*
eseqdist* eseqmatrix::operator()(int x,int y){
  if (!cols[x] || !cols[y]) return(0x00);
  int i;
  if (x>y) 
    { i=x; x=y; y=i; }
  for (i=0; i<cols[x]->size(); ++i) 
    if (cols[x]->at(i)->y==y)
      return(cols[x]->at(i));
  return(0x00);
}
*/

//ebasicarray<float> mindist;
//eintarray mindist_id,mindist_id2;

inline void getXY2(int l,int w,int& x,int& y)
{
  x=(l/w);
  y=(l%w);
  if (y<w-x){ y+=x; return; }
  x=w-x;
}

/*
int calc_dists(estrarray& arr,earray<eseqdist>& dists,int node,int nodecount,float thres)
{
  int i,j;
  int yend,xend;

  int sq_side=(int)sqrt(arr.size()*arr.size()/(2*nodecount));
  int sq_count=(int)sqrt(2*nodecount);

  float tmpid;
  int x,y;

  cout << "# sq_side: " << sq_side << " sq_count: "<< sq_count << endl;

//  for (i=0; i<nodecount; ++i){
//    getXY2(i,sq_count,x,y);
//    cout << "i: " << i << " x: "<< x<< " y: " <<y<<endl;
//  }

  if (node<sq_count){
//    x=node*2,y=node*2;
    x=node,y=node;
    cout << "node: " << node << " x: "<< x<< " y: " <<y<<endl;
    // compute two diagonals
    xend=x+1==sq_count?arr.size():(x+1)*sq_side;
    yend=y+1==sq_count?arr.size():(y+1)*sq_side;
    for (j=y*sq_side; j<yend; ++j){
      for (i=j+1; i<xend; ++i){
	tmpid=dist(arr.values(i),arr.values(j));
	if (tmpid>thres) dists.add(eseqdist(i,j,tmpid));

      }
    }
      
    
*   ++x; ++y;
   if (x<sq_count){
      cout << "node: " << node << " x: "<< x<< " y: " <<y<<endl;
      xend=x+1==sq_count?arr.size():(x+1)*sq_side;
      yend=y+1==sq_count?arr.size():(y+1)*sq_side;
      for (j=y*sq_side; j<yend; ++j){
        for (i=j+1; i<xend; ++i){
          tmpid=dist(arr.values(i),arr.values(j));
          if (tmpid>thres) dists.add(eseqdist(i,j,tmpid));
        }
      }
    }
*
  }else{
    node-=sq_count;
    getXY(node,sq_count,x,y);
    // compute one square
    cout << "node: " << node+sq_count << " x: "<< x<< " y: " <<y<<endl;
    xend=x+1==sq_count?arr.size():(x+1)*sq_side;
    yend=y+1==sq_count?arr.size():(y+1)*sq_side;
    for (j=y*sq_side; j<yend; ++j){
      for (i=x*sq_side; i<xend; ++i){
        tmpid=dist(arr.values(i),arr.values(j));
        if (tmpid>thres) dists.add(eseqdist(i,j,tmpid));
      }
    }
  }
  heapsort(dists);
  return(dists.size());
}
*/

int calc_dists(estrarray& arr,ebasicarray<eseqdist>& dists,int node,int tnodes,float thres)
{
  int i,i2,j;
  int start,end;

  start=node*(arr.size()-1)/(2*tnodes);
  end=(node+1)*(arr.size()-1)/(2*tnodes);

  float tmpid;

  for (i=start; i<end; ++i){
    for (j=i+1; j<arr.size(); ++j){
      tmpid=dist(arr.values(i),arr.values(j));
      if (tmpid>thres) dists.add(eseqdist(i,j,tmpid));
    }
    i2=arr.size()-i-2;
    for (j=i2+1; j<arr.size(); ++j){
      tmpid=dist(arr.values(i2),arr.values(j));
      if (tmpid>thres) dists.add(eseqdist(i2,j,tmpid));
    }
  }
  if (node==tnodes-1 && arr.size()%2==0){
    i=arr.size()/2-1;
    for (j=i+1; j<arr.size(); ++j){
      tmpid=dist(arr.values(i),arr.values(j));
      if (tmpid>thres) dists.add(eseqdist(i,j,tmpid));
    }
  }

  heapsort(dists);
  return(dists.size());
}

int calc_dists(estrarray& arr,earray<eseqdist>& dists,int node,int tnodes,float thres)
{
  int i,i2,j;
  int start,end;

  start=node*(arr.size()-1)/(2*tnodes);
  end=(node+1)*(arr.size()-1)/(2*tnodes);

  float tmpid;

  for (i=start; i<end; ++i){
    for (j=i+1; j<arr.size(); ++j){
      tmpid=dist(arr.values(i),arr.values(j));
      if (tmpid>thres) dists.add(eseqdist(i,j,tmpid));
    }
    i2=arr.size()-i-2;
    for (j=i2+1; j<arr.size(); ++j){
      tmpid=dist(arr.values(i2),arr.values(j));
      if (tmpid>thres) dists.add(eseqdist(i2,j,tmpid));
    }
  }
  if (node==tnodes-1 && arr.size()%2==0){
    i=arr.size()/2-1;
    for (j=i+1; j<arr.size(); ++j){
      tmpid=dist(arr.values(i),arr.values(j));
      if (tmpid>thres) dists.add(eseqdist(i,j,tmpid));
    }
  }

//  heapsort(dists);
  return(dists.size());
}

/*
int calc_dists_nogap(estrarray& arr,multimap<float,eseqdist>& dists,int node,int tnodes,float thres)
{
  int i,i2,j;
  int start,end;

  start=node*arr.size()/(2*tnodes);
  end=(node+1)*arr.size()/(2*tnodes);

  float tmpid;
  int count=0;

  for (i=start; i<end-1; ++i){
    for (j=i+1; j<arr.size(); ++j){
      tmpid=dist_nogap(arr.values(i),arr.values(j));
      if (tmpid>thres) dists.insert(pair<float,eseqdist>(tmpid,eseqdist(i,j,tmpid)));
    }
    i2=arr.size()-i-2;
    for (j=i2+1; j<arr.size(); ++j){
      tmpid=dist_nogap(arr.values(i2),arr.values(j));
      if (tmpid>thres) dists.insert(pair<float,eseqdist>(tmpid,eseqdist(i2,j,tmpid)));
    }
  }
  if (end==arr.size()/2 && arr.size()%2==1){
    i=arr.size()/2;
    for (j=i+1; j<arr.size(); ++j){
      tmpid=dist_nogap(arr.values(i),arr.values(j));
      if (tmpid>thres) dists.insert(pair<float,eseqdist>(tmpid,eseqdist(i,j,tmpid)));
    }
  }
//  heapsort(dists);
  return(dists.size());
}
*/

int calc_dists_nogap(estrarray& arr,ebasicarray<eseqdist>& dists,int node,int tnodes,float thres)
{
  ldieif(arr.size()==0,"no distances in array");
  int i,i2,j;
  int start,end;

  start=node*(arr.size()-1)/(2*tnodes);
  end=(node+1)*(arr.size()-1)/(2*tnodes);

  float tmpid;
  int count=0;
  dists.reserve(500000);

  for (i=start; i<end; ++i){
    for (j=i+1; j<arr.size(); ++j){
      tmpid=dist_nogap(arr.values(i),arr.values(j));
      if (tmpid>thres) dists.add(eseqdist(i,j,tmpid));
      ++count;
    }
    i2=arr.size()-i-2;
    for (j=i2+1; j<arr.size(); ++j){
      tmpid=dist_nogap(arr.values(i2),arr.values(j));
      if (tmpid>thres) dists.add(eseqdist(i2,j,tmpid));
      ++count;
    }
//    if (i-start==(end-start)/10)
//      cout << "# predicted: " << dists.size()*10 << endl;
//    if (dists.size()>dists.capacity()*2/3)
//      dists.reserve(2*dists.capacity());
  }
  if (node==tnodes-1 && arr.size()%2==0){
    i=arr.size()/2-1;
    for (j=i+1; j<arr.size(); ++j){
      tmpid=dist_nogap(arr.values(i),arr.values(j));
      if (tmpid>thres) dists.add(eseqdist(i,j,tmpid));
      ++count;
    }
  }
//  cout << "# n: " << node << " tnodes: " << tnodes << " start: " << start << " end: " << end << " distances: " << dists.size() << endl;
//  cout << "# computed distances: " << count << endl;
//  heapsort(dists);
  return(dists.size());
}

int calc_dists_nogap(estrarray& arr,earray<eseqdist>& dists,int node,int tnodes,float thres)
{
  int i,i2,j;
  int start,end;

  start=node*(arr.size()-1)/(2*tnodes);
  end=(node+1)*(arr.size()-1)/(2*tnodes);

  float tmpid;

  for (i=start; i<end; ++i){
    for (j=i+1; j<arr.size(); ++j){
      tmpid=dist_nogap(arr.values(i),arr.values(j));
      if (tmpid>thres) dists.add(eseqdist(i,j,tmpid));
    }
    i2=arr.size()-i-2;
    for (j=i2+1; j<arr.size(); ++j){
      tmpid=dist_nogap(arr.values(i2),arr.values(j));
      if (tmpid>thres) dists.add(eseqdist(i2,j,tmpid));
    }
  }
  if (node==tnodes-1 && arr.size()%2==0){
    i=arr.size()/2-1;
    for (j=i+1; j<arr.size(); ++j){
      tmpid=dist_nogap(arr.values(i),arr.values(j));
      if (tmpid>thres) dists.add(eseqdist(i,j,tmpid));
    }
  }
//  heapsort(dists);
  return(dists.size());
}

int calc_dists_nogap(estrarray& arr,eintarray& arrgaps,earray<eseqdist>& dists,int node,int tnodes,float thres)
{
  int i,i2,j;
  int start,end;

  start=node*(arr.size()-1)/(2*tnodes);
  end=(node+1)*(arr.size()-1)/(2*tnodes);

  float tmpid;

  for (i=start; i<end; ++i){
    for (j=i+1; j<arr.size(); ++j){
      tmpid=dist_nogap(arr.values(i),arr.values(j),arrgaps[i],arrgaps[j]);
      if (tmpid>thres) dists.add(eseqdist(i,j,tmpid));
    }
    i2=arr.size()-i-2;
    for (j=i2+1; j<arr.size(); ++j){
      tmpid=dist_nogap(arr.values(i2),arr.values(j),arrgaps[i2],arrgaps[j]);
      if (tmpid>thres) dists.add(eseqdist(i2,j,tmpid));
    }
  }
  if (node==tnodes-1 && arr.size()%2==0){
    i=arr.size()/2-1;
    for (j=i+1; j<arr.size(); ++j){
      tmpid=dist_nogap(arr.values(i),arr.values(j),arrgaps[i],arrgaps[j]);
      if (tmpid>thres) dists.add(eseqdist(i,j,tmpid));
    }
  }
//  heapsort(dists);
  return(dists.size());
}


/*
void update_dists(eseqmatrix& smat,int x,int y)
{
  int i,j;
  eseqdist *pdist;
  if (!smat.cols[x]) return;
  for (i=0; smat.cols[x]->size(); ++i)
    smat.cols[x]->at(i)->skip=true;

  i=0; j=0;
  while (i<smat.cols[x]->size() && j<smat.cols[y]->size()){
//    if (
  }
  for (i=0; smat.cols.size(); ++i){
    ldieif(smat.cols[x]==0x00 || smat.cols[y]==0x00,"not supposed to happen");
    
    *smat(x,i)=min(*smat(x,i),*smat(y,i));
  }
}

void cluster_complete2(const estrhash& arr,earray<eseqdist>& dists,earray<eintarray>& otus,eintarray& otuarr,float t)
{
  int x,y;
  int i,j,l,k;

  eseqmatrix smat;
  smat.add(dists);

  eintarray dsort;
//  heapify(dists,dsort);

//  otus.clear();
//  for (i=0; i<arr.size(); ++i)
//    otus.add(eintarray());
  otuarr.clear();
  for (i=0; i<arr.size(); ++i)
    otuarr.add(i);

  while (dsort.size()){
    x=dists[dsort[i]].x;
    y=dists[dsort[i]].y;

    otuarr[y]=x;
    update_dists(smat,x,y);

//    cout << dsort[i] << " " << x << " " << seq1[dsort[i]] << " " << y << " " << seq2[dsort[i]] << " " << dist_mat[dsort[i]] << endl;
  }
//  cout << "# i: " << dist_mat.size()-i << " ("<<1.0-(float)i/dist_mat.size()<<")" << endl;

  cout << "# tmp total otus: " << otus.size() << endl;

  for (i=0; i<otus.size(); ++i)
    if (otus[i].size()==0)
      { otus.erase(i); --i; }

  for (i=0; i<otuarr.size(); ++i){
    if (otuarr[i]==-1){
      otus.add(eintarray());
      otus[otus.size()-1].add(i);
      otuarr[i]=otus.size()-1;
    }
  }

  cout << "# total otus: " << otus.size() << endl;
}
*/

void cluster_complete(const estrhash& arr,const ebasicarray<float>& dist_mat,const eintarray& dsort,earray<eintarray>& otus,eintarray& otuarr,float t)
{
  int x,y;
  int arr1,arr2;
  int i,j,l,k;
  bool complete;

  otuarr.clear();
  for (i=0; i<arr.size(); ++i)
    otuarr.add(-1);

  for (i=dist_mat.size()-1; i>0; --i){
    getXY(dsort[i],arr.size(),x,y);
    if (dist_mat[dsort[i]]<t) break;
    if (otuarr[x]==otuarr[y] && otuarr[x]!=-1) continue;

    if (otuarr[x]==-1 && otuarr[y]==-1){
      otuarr[x]=otus.size(); otuarr[y]=otus.size();
      otus.add(eintarray());
      otus[otuarr[x]].add(x);
      otus[otuarr[y]].add(y);
    }else if (otuarr[x]==-1){
      complete=true;
      for (j=0; j<otus[otuarr[y]].size(); ++j){
        getL2(l,arr.size(),x,otus[otuarr[y]][j]);
        if (dist_mat[l]<t){ complete=false; break; }
      }
      if (!complete) continue;
      otuarr[x]=otuarr[y];
      otus[otuarr[x]].add(x);
    }else if (otuarr[y]==-1){
      complete=true;
      for (j=0; j<otus[otuarr[x]].size(); ++j){
        getL2(l,arr.size(),y,otus[otuarr[x]][j]);
        if (dist_mat[l]<t){ complete=false; break; }
      }
      if (!complete) continue;
      otuarr[y]=otuarr[x];
      otus[otuarr[y]].add(y);
    }else{
      complete=true;
      for (j=0; j<otus[otuarr[x]].size(); ++j){
        for (k=0; k<otus[otuarr[y]].size(); ++k){
          getL2(l,arr.size(),otus[otuarr[y]][k],otus[otuarr[x]][j]);
          if (dist_mat[l]<t){ complete=false; break; }
        }
        if (!complete) break;
      }
      if (!complete) continue;
      
      arr1=otuarr[x]; arr2=otuarr[y];
      if (arr1>arr2) { arr1=otuarr[y]; arr2=otuarr[x]; }

      for (j=0; j<otus[arr2].size(); ++j)
        otuarr[otus[arr2][j]]=arr1;
      otus[arr1]+=otus[arr2];
      otus[arr1].clear();
    }
//    cout << dsort[i] << " " << x << " " << seq1[dsort[i]] << " " << y << " " << seq2[dsort[i]] << " " << dist_mat[dsort[i]] << endl;
  }
  cout << "# i: " << dist_mat.size()-i << " ("<<1.0-(float)i/dist_mat.size()<<")" << endl;

  cout << "# tmp total otus: " << otus.size() << endl;

  for (i=0; i<otus.size(); ++i)
    if (otus[i].size()==0)
      { otus.erase(i); --i; }

  for (i=0; i<otuarr.size(); ++i){
    if (otuarr[i]==-1){
      otus.add(eintarray());
      otus[otus.size()-1].add(i);
      otuarr[i]=otus.size()-1;
    }
  }

  cout << "# total otus: " << otus.size() << endl;
}

void cluster_single(const estrhash& arr,const ebasicarray<float>& dist_mat,const eintarray& dsort,earray<eintarray>& otus,float t)
{
  int x,y;
  int arr1,arr2;
  int i,j;

  eintarray otuarr;
  for (i=0; i<arr.size(); ++i)
    otuarr.add(-1);

  for (i=dist_mat.size()-1; i>0; --i){
    getXY(dsort[i],arr.size(),x,y);
    if (dist_mat[dsort[i]]<t) break;
    if (otuarr[x]==otuarr[y] && otuarr[x]!=-1) continue;

    if (otuarr[x]==-1 && otuarr[y]==-1){
      otuarr[x]=otus.size(); otuarr[y]=otus.size();
      otus.add(eintarray());
      otus[otuarr[x]].add(x);
      otus[otuarr[y]].add(y);
    }else if (otuarr[x]==-1){
      otuarr[x]=otuarr[y];
      otus[otuarr[x]].add(x);
    }else if (otuarr[y]==-1){
      otuarr[y]=otuarr[x];
      otus[otuarr[y]].add(y);
    }else{
      arr1=otuarr[x]; arr2=otuarr[y];
      if (arr1>arr2) { arr1=otuarr[y]; arr2=otuarr[x]; }

      for (j=0; j<otus[arr2].size(); ++j)
        otuarr[otus[arr2][j]]=arr1;
      otus[arr1]+=otus[arr2];
      otus[arr1].clear();
    }
//    cout << dsort[i] << " " << x << " " << seq1[dsort[i]] << " " << y << " " << seq2[dsort[i]] << " " << dist_mat[dsort[i]] << endl;
  }
  cout << "# i: " << dist_mat.size()-i << " ("<<1.0-(float)i/dist_mat.size()<<")" << endl;

  cout << "# tmp total otus: " << otus.size() << endl;

  for (i=0; i<otus.size(); ++i)
    if (otus[i].size()==0)
      { otus.erase(i); --i; }

  for (i=0; i<otuarr.size(); ++i){
    if (otuarr[i]==-1){
      otus.add(eintarray());
      otus[otus.size()-1].add(i);
      otuarr[i]=otus.size()-1;
    }
  }

  cout << "# total otus: " << otus.size() << endl;
}


