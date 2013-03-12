#include "eseqclusteravg.h"
#include "cluster-common.h"

#include <eutils/efile.h>


eseqclusteravg::eseqclusteravg(){}


void eseqclusteravg::check(ebasicarray<eseqdistCount>& dists)
{
  int i;
  estr xystr;
  bool duplicate=false;
  eseqdisthash checkmatrix;
  for (i=0; i<dists.size(); ++i){
    if (i%(dists.size()/10)==0) { cout << i*10/dists.size(); flush(cout); }
//    xy2estr(dists[i].x,dists[i].y,xystr);

    eseqdisthash::iter it;

//    cout << dists[i].dist << " " << dists[i].x << " " << dists[i].y;
    it=checkmatrix.get(dists[i]);
    if (it != checkmatrix.end()) {
      cout << "duplicate found: "+estr(dists[i].x)+","+dists[i].y << endl;
//      cout << " *";
      duplicate=true;
    }else
      checkmatrix.add(dists[i],1);
//    cout << endl;
  }
  smatrix.clear();

  ldieif(duplicate,"duplicates found");
  cout << "# no duplicates found!" << endl;
}

void eseqclusteravg::init(int count,const estr& filename,const estr& seqsfile,const earray<eintarray>& dupslist)
{
  ofile.open(filename,"w");
  ofile.write("# seqsfile: "+seqsfile+"\n");
  ofile.write("# OTU_count Merge_distance Merged_OTU_id1 Merged_OTU_id2\n");
  int i,j;
  incmaxdist=0.0;
  lastdist=0.0;
  scount.reserve(count);
  scluster.reserve(count);
  smerge.reserve(count);
  inter.reserve(count);
  incluster.reserve(count);
  mergecount=0;
  for (i=0; i<count; ++i){
    scount.add(1);
    scluster.add(i);
    smerge.add(-1);
    incluster.add(list<int>());
    incluster[i].push_back(i);
    inter.add(list<int>());
  }
  for (i=0; i<dupslist.size(); ++i){
    for (j=1; j<dupslist[i].size(); ++j){
      ++mergecount;
      ofile.write(estr(scluster.size()-mergecount)+" 1.0 "+dupslist[i][0]+" "+dupslist[i][j]+"\n");
      clusterData.mergearr.add(eseqdist(dupslist[i][0],dupslist[i][j],1.0));
    }
  }
  cout << "# initializing cluster with: "<< count<< " seqs" << endl; 
  cout << "# initializing smatrix with: " << (long int)(count)*(long int)(count)/20000l/2l<< " elements" << endl; 
  smatrix.reserve((long int)(count)*(long int)(count)/20000l/2l);
//  cout << "# smatrix._hashitems = " << smatrix._hashitems << endl;
}

long int eseqclusteravg::update(eblockarray<eseqdistCount>& dists,long int s)
{
  int count=0;
  int i;
  int smergepos=0;
  eintarray tmpsmerge;
  int updcount;
  int updind[smerge.size()];
  int updcount2;
  int updind2[smerge.size()];

  for (i=0; i<smerge.size(); ++i)
    tmpsmerge.add(-1);

  for (i=0; i<smerge.size(); ++i)
    updind2[i]=-1;

  updcount2=0;
  for (i=0; i<scluster.size(); ++i){
    if (updind2[scluster[i]]==-1){
      updind2[scluster[i]]=updcount2;
      ++updcount2;
    }
  }

  // make sure to only update 100 entries at a time, this will force more passes but use less memory
  do {
    updcount=0;

    for (i=0; i<smerge.size(); ++i)
      updind[i]=-1;

    for (i=0; i<tmpsmerge.size(); ++i)
      tmpsmerge[i]=-1;

    for (; smergepos<smerge.size(); ++smergepos){
      if (smerge[smergepos]>=0 && scluster[smergepos]!=smergepos){
        if (updind[scluster[smergepos]]==-1) {
          updind[scluster[smergepos]]=updcount;
          ++updcount;
        }
        updind[smergepos]=updind[scluster[smergepos]];
        tmpsmerge[smergepos]=scluster[smergepos];
        tmpsmerge[scluster[smergepos]]=scluster[smergepos];
//        ldieif(scluster[scluster[smergepos]]!=scluster[smergepos],"something wrong??");
        if (updcount==100) { ++smergepos; break; }
      }
/*
      if (smerge[smergepos]>=0){
        if (updind[smerge[smergepos]]==-1) {
          updind[smerge[smergepos]]=updcount;
          ++updcount;
        }
        updind[smergepos]=updind[smerge[smergepos]];
        tmpsmerge[smergepos]=smerge[smergepos];
//        if (updcount==100) break;
      }
*/
    }

    if (updcount==0) return(0);

    cerr << "# updating: " << updcount << " merges smerge.size: "<<tmpsmerge.size()<<endl;

    long int *uarr=new long int[updcount*updcount2];
    ldieif (uarr==0x00,"not enough memory");
    long int li,lj;
    for (i=0; i<updcount*updcount2; ++i)
      uarr[i]=-1l;

    for (li=s; li>=0; --li){
      if (dists[li].count==0) continue;

      if (tmpsmerge[dists[li].x]>=0){
        lj=uarr[updind[tmpsmerge[dists[li].x]]*updcount2+updind2[scluster[dists[li].y]]];
        if (lj>=0){
          dists[li].count+=dists[lj].count;
          dists[lj].count=0;
          ++count;
        }
        uarr[updind[tmpsmerge[dists[li].x]]*updcount2+updind2[scluster[dists[li].y]]]=li;
      }else if (tmpsmerge[dists[li].y]>=0){
        lj=uarr[updind[tmpsmerge[dists[li].y]]*updcount2+updind2[scluster[dists[li].x]]];
        if (lj>=0){
          dists[li].count+=dists[lj].count;
          dists[lj].count=0;
          ++count;
        }
        uarr[updind[tmpsmerge[dists[li].y]]*updcount2+updind2[scluster[dists[li].x]]]=li;
      }
    }

    delete[] uarr;
  }while (updcount==100);

  for (i=0; i<smerge.size(); ++i)
    smerge[i]=-1;

  return(count);
}

void eseqclusteravg::merge(const eseqdistCount& sdist)
{
  ldieif(sdist.x==sdist.y,"should not happen!");
  ldieif(scount[sdist.x]==0 || scount[sdist.y]==0,"also should not happen");

  clusterData.mergearr.add(eseqdist(sdist.x,sdist.y,sdist.dist));

  smerge[sdist.x]=sdist.x;
  smerge[sdist.y]=sdist.x;

  scount[sdist.x]+=scount[sdist.y];
  scount[sdist.y]=0;

  list<int>::iterator it;
  for (it=incluster[sdist.y].begin(); it!=incluster[sdist.y].end(); ++it){
    scluster[*it]=sdist.x;
    incluster[sdist.x].push_back(*it);
  }

  eseqdistCount tmpdist,tmpdist2;
//  estr tmpstr,tmpstr2;
  tmpdist.x=sdist.x;
  tmpdist2.x=sdist.y;

  int i,j;
  for (it=inter[sdist.y].begin(); it!=inter[sdist.y].end(); ++it){
    j=scluster[*it];
    if (sdist.x==j || sdist.y==j) continue;
    tmpdist.y=j;
    tmpdist2.y=j;
//    xy2estr(x,j,tmpstr);
//    xy2estr(y,j,tmpstr2);

    eseqdistavghash::iter tmpit2=smatrix.get(tmpdist2);
    if(tmpit2==smatrix.end()) continue;

    eseqdistavghash::iter tmpit=smatrix.get(tmpdist);
    if (tmpit!=smatrix.end()){
/*
      if (scount[tmpit->x]*scount[tmpit->y]==tmpit->count+tmpit2->count){
        if((tmpit->dist*tmpit->count+tmpit2->dist*tmpit2->count)/(tmpit->count+tmpit2->count)>sdist.dist){
          cout << "sdist.dist: " << sdist.dist << " tmpit: " << *tmpit << " tmpit2: " << *tmpit2 << " sdist: " << sdist << " scount[x]: " << scount[sdist.x] << " scount[y]: " << scount[sdist.y] << " scount[j]: "<< scount[j] << endl;;
          exit(-1);
        }
      }
*/
      tmpit->dist=((double)tmpit->dist*tmpit->count+(double)tmpit2->dist*tmpit2->count)/(double)(tmpit->count+tmpit2->count);
      tmpit->count+=tmpit2->count;
    }else{
      tmpdist.dist=tmpit2->dist;
      tmpdist.count=tmpit2->count;
      smatrix.add(tmpdist,tmpdist);
      inter[sdist.x].push_back(j);
      tmpit=smatrix.get(tmpdist);
    }
    // make sure to add merged neighbors which are complete to the complete list
    lassert(scount[tmpit->x]==0 || scount[tmpit->y]==0);
    if (scount[tmpit->x]*scount[tmpit->y]==tmpit->count){
//      ldieif(tmpit->dist>sdist.dist,"sdist.dist: "+estr(sdist.dist)+" tmpit.dist: "+tmpit->dist+" tmpit.count: "+tmpit->count);
      completemerges.insert(*tmpit);
    }
    smatrix.erase(tmpit2);
  }
  ++mergecount;
}

float eseqclusteravg::getIncompleteMaxDist(float newdist)
{
  float maxdist=0.0;
  float tmpdist;
  eseqdistavghash::iter it,maxit;
  maxit=smatrix.end();
  for (it=smatrix.begin(); it!=smatrix.end(); ++it){
//    lassert(scount[it->x]==0 || scount[it->y]==0);
    if (scount[it->x]==0 || scount[it->y]==0) continue;
    if (scount[it->x]*scount[it->y]!=it->count){
      tmpdist=((double)it->count*it->dist+(double)(scount[it->x]*scount[it->y]-it->count)*newdist)/(double)(scount[it->x]*scount[it->y]);
      if (tmpdist>maxdist) { maxdist=tmpdist; maxit=it; }
    }
  }
  if (maxit!=smatrix.end()){
    cout << "# maxit: " << *maxit << " scount[x]: " << scount[maxit->x] << " scount[y]: "<< scount[maxit->y] << " nextthres: " << (completemerges.size()?estr( ((double)scount[maxit->x]*scount[maxit->y]*completemerges.begin()->dist-(double)maxit->count*maxit->dist)/(double)(scount[maxit->x]*scount[maxit->y]-maxit->count)):estr("n/a")) << endl;
  }
  return(maxdist);
}

void eseqclusteravg::clearComplete()
{
  multiset<eseqdistCount>::iterator it,it_next;
  eseqdistavghash::iter sit;
  for (it=completemerges.begin(),it_next=it; it!=completemerges.end(); it=it_next){
    ++it_next;
    sit=smatrix.get(*it);
    if (sit==smatrix.end() || scount[sit->x]==0 || scount[sit->y]==0 || scount[sit->x]*scount[sit->y]!=sit->count)
      completemerges.erase(it);
  }
}

void eseqclusteravg::mergeComplete(float dist)
{
  multiset<eseqdistCount>::iterator it,it_next;
  eseqdistavghash::iter sit;
  for (it=completemerges.begin(); it!=completemerges.end() && it->dist>=dist; it=completemerges.begin()){
    sit=smatrix.get(*it);
    if (sit==smatrix.end() || sit->dist!=it->dist || scount[sit->x]==0 || scount[sit->y]==0 || scount[sit->x]*scount[sit->y]!=sit->count) { completemerges.erase(it); continue; }
    lassert(scluster[sit->x]!=sit->x || scluster[sit->y]!=sit->y);
    cout << "* " << scluster.size()-mergecount << " " << sit->dist << " ("<<sit->dist<<") " << sit->x << " " << scount[sit->x]<< " " << sit->y << " " << scount[sit->y] << " " << smatrix.size() << " " << completemerges.size() << " " << incmaxdist << " " << (completemerges.size()?estr(completemerges.begin()->dist):estr("n/a")) << endl;
    ofile.write(estr(scluster.size()-mergecount)+" "+sit->dist+" "+sit->x+" "+sit->y+"\n");
    merge(*sit);
    smatrix.erase(sit);
    completemerges.erase(it);
  }
}

void eseqclusteravg::add(const eseqdist& sdist){
  ldieif(sdist.x<0 || sdist.y<0 || sdist.x>=scluster.size() || sdist.y>=scluster.size(),"out of bounds: sdist.x: "+estr(sdist.x)+" sdist.y: "+estr(sdist.y)+" scluster.size(): "+estr(scluster.size()));

  if (lastdist!=sdist.dist){
    incmaxdist=getIncompleteMaxDist(sdist.dist);
    while (completemerges.size() && completemerges.begin()->dist>=incmaxdist){
      mergeComplete(incmaxdist);
      incmaxdist=getIncompleteMaxDist(sdist.dist);
    }
    clearComplete();
    lastdist=sdist.dist;
  }

  eseqdistCount tmpdist;
  tmpdist.x=scluster[sdist.x];
  tmpdist.y=scluster[sdist.y];
  tmpdist.dist=sdist.dist;
  tmpdist.count=1;

//  cout << tmpdist << " scount[x]: "<<scount[tmpdist.x] << " scount[y]: " << scount[tmpdist.y] << endl;

  ldieif(tmpdist.x<0 || tmpdist.y<0 || tmpdist.x>=scluster.size() || tmpdist.y>=scluster.size(),"out of bounds: sdist.x: "+estr(tmpdist.x)+" sdist.y: "+estr(tmpdist.y)+" scluster.size(): "+estr(scluster.size()));
//  int tmp;
//  if (tmpdist.x>tmpdist.y) { tmp=tmpdist.x; tmpdist.x=tmpdist.y; tmpdist.y=tmp; }

  int links;
  int i;

  ldieif(tmpdist.x==tmpdist.y,"should not happen: "+estr(tmpdist.x)+","+estr(tmpdist.y)+" --- "+estr(sdist.x)+","+estr(sdist.y));

  eseqdistavghash::iter it;

  it=smatrix.get(tmpdist);
  if (it==smatrix.end()){
    if (scount[tmpdist.x]*scount[tmpdist.y]==tmpdist.count){
      if (tmpdist.dist>=incmaxdist){
        cout << "1 " << scluster.size()-mergecount << " " << tmpdist.dist << " ("<<tmpdist.dist<<") " << tmpdist.x << " " << scount[tmpdist.x]<< " " << tmpdist.y << " " << scount[tmpdist.y] << " " << smatrix.size() << " " << completemerges.size() << " " << incmaxdist << " " << (completemerges.size()?estr(completemerges.begin()->dist):estr("n/a")) << endl;
        ofile.write(estr(scluster.size()-mergecount)+" "+tmpdist.dist+" "+tmpdist.x+" "+tmpdist.y+"\n");
        merge(tmpdist);
      }else{
        smatrix.add(tmpdist,tmpdist);
        inter[tmpdist.x].push_back(tmpdist.y); inter[tmpdist.y].push_back(tmpdist.x);
        completemerges.insert(tmpdist);
      }
    }else{
      smatrix.add(tmpdist,tmpdist);
      inter[tmpdist.x].push_back(tmpdist.y); inter[tmpdist.y].push_back(tmpdist.x);
    }
    return;
  }

  it->dist=((double)it->dist*it->count+(double)tmpdist.dist*tmpdist.count)/(double)(it->count+tmpdist.count);
  it->count+=tmpdist.count;

  // complete linkage
  if (it->count==scount[tmpdist.x]*scount[tmpdist.y]){
    if (it->dist>=incmaxdist){
      cout << "+ " << scluster.size()-mergecount << " " << it->dist << " ("<<tmpdist.dist<<") " << it->x << " " << scount[it->x]<< " " << it->y << " " << scount[it->y] << " " << smatrix.size() << " " << completemerges.size() << " " << incmaxdist << " " << (completemerges.size()?estr(completemerges.begin()->dist):estr("n/a")) << endl;
      ofile.write(estr(scluster.size()-mergecount)+" "+it->dist+" "+it->x+" "+it->y+"\n");
      merge(*it);
      smatrix.erase(it);
//      incmaxdist=getIncompleteMaxDist(sdist.dist);
//      while (completemerges.size() && completemerges.begin()->dist>=incmaxdist){
//        mergeComplete(incmaxdist);
//        incmaxdist=getIncompleteMaxDist(sdist.dist);
//      }
    }else{
      completemerges.insert(*it);
//      cout << "# " << scluster.size()-mergecount << " " << tmpdist.dist << " " << tmpdist.x << " " << tmpdist.y << " " << smatrix.size() << " " << completemerges.size() << " " << incmaxdist << endl;
    }
  }
}

void eseqclusteravg::add(const eseqdistCount& sdist){
  if (sdist.count==0) return;
  ldieif(sdist.x<0 || sdist.y<0 || sdist.x>=scluster.size() || sdist.y>=scluster.size(),"out of bounds: sdist.x: "+estr(sdist.x)+" sdist.y: "+estr(sdist.y)+" scluster.size(): "+estr(scluster.size()));

  eseqdistCount tmpdist;
  tmpdist.x=scluster[sdist.x];
  tmpdist.y=scluster[sdist.y];
  tmpdist.dist=sdist.dist;
  tmpdist.count=sdist.count;

  ldieif(tmpdist.x<0 || tmpdist.y<0 || tmpdist.x>=scluster.size() || tmpdist.y>=scluster.size(),"out of bounds: sdist.x: "+estr(tmpdist.x)+" sdist.y: "+estr(tmpdist.y)+" scluster.size(): "+estr(scluster.size()));
  int tmp;
  if (tmpdist.x>tmpdist.y) { tmp=tmpdist.x; tmpdist.x=tmpdist.y; tmpdist.y=tmp; }

  int links;
  int i;
//  estr xystr;

//  cout << x << " " << y << " " << sdist.dist << endl;
  ldieif(tmpdist.x==tmpdist.y,"should not happen: "+estr(tmpdist.x)+","+estr(tmpdist.y)+" --- "+estr(sdist.x)+","+estr(sdist.y));

//  xy2estr(x,y,xystr);

  eseqdistavghash::iter it;

  it=smatrix.get(tmpdist);
  if (it==smatrix.end()){
    if (scount[tmpdist.x]*scount[tmpdist.y]==sdist.count){
      merge(tmpdist);
      ofile.write(estr(scluster.size()-mergecount)+" "+tmpdist.dist+" "+tmpdist.x+" "+tmpdist.y+"\n");
//      cout << scluster.size()-mergecount << " " << sdist.dist << " " << sdist.x << " " << sdist.y << endl;
    }else{
      smatrix.add(tmpdist,tmpdist);
      inter[tmpdist.x].push_back(tmpdist.y); inter[tmpdist.y].push_back(tmpdist.x);
    }
    return;
  }

  it->dist=(it->dist*it->count+tmpdist.dist*tmpdist.count)/(it->count+tmpdist.count);
  it->count+=tmpdist.count;
//  ++(*it);

  // complete linkage
  if (it->count==scount[tmpdist.x]*scount[tmpdist.y]){
    merge(tmpdist);
//    update(ind-1,x,y);
    ofile.write(estr(scluster.size()-mergecount)+" "+tmpdist.dist+" "+tmpdist.x+" "+tmpdist.y+"\n");
//    cout << scluster.size()-mergecount << " " << tmpdist.dist << " " << tmpdist.x << " " << tmpdist.y << endl;
//    sleep(1);
//    cout << sdist.dist << " " << x << " " << y << endl;
    smatrix.erase(it);
  }
}

/*
void eseqclusteravg::add(int ind){
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
*/

void eseqclusteravg::save(const estr& filename,const estrarray& arr)
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

