#include "cluster-common.h"
#include "eseqclusterdata.h"

eseqclusterData::eseqclusterData(): count(0) {}

int eseqclusterData::getOTU(float dist,eintarray& otuarr)
{
  int i,j;
  for (i=0; i<otuarr.size(); ++i)
    otuarr[i]=i;

  // point at the seq we merged with
  for (i=0; i<mergearr.size() && mergearr[i].dist>=dist; ++i)
    otuarr[mergearr[i].y]=mergearr[i].x;

  // translate all the merged seq ids to the last seq id of the otu
  for (i=0; i<otuarr.size(); ++i){
    for (j=i; j!=otuarr[j]; j=otuarr[j]);
    otuarr[i]=j;
  }

  eintarray otuind;
  otuind.init(otuarr.size(),-1);

  // translate otu indexes into an index from 0 to the total number of otus
  int otucount=0;
  for (i=0; i<otuarr.size(); ++i){
    if (otuind[otuarr[i]]==-1)
      otuind[otuarr[i]]=otucount++;
    otuarr[i]=otuind[otuarr[i]];
  }
  return(otucount);
}

int eseqclusterData::getOTU(float dist,earray<eintarray>& otus,int size)
{
  eintarray otuarr;
  int i,j;
  otuarr.init(size);
  for (i=0; i<size; ++i)
    otuarr[i]=i;

  // point at the seq we merged with
  for (i=0; i<mergearr.size() && mergearr[i].dist>=dist; ++i)
    otuarr[mergearr[i].y]=mergearr[i].x;

  eintarray otuind;
  otuind.init(otuarr.size(),-1);

  // translate all the merged seq ids to the last seq id of the otu
  for (i=0; i<otuarr.size(); ++i){
    for (j=otuarr[i]; j!=otuarr[j]; j=otuarr[j]);
    if (otuind[j]==-1){
      otuind[j]=otus.size();
      otus.add(eintarray());
    }
    otuarr[i]=otuind[j];
    otus[otuind[j]].add(i);
  }
  return(otus.size());
}


void eseqclusterData::getCluster(const eintarray& seqs, eintarray& seqcluster,float& cdist)
{
  if (seqs.size()<2) { lwarn("seqs is empty or contains only one sequence: "+estr(seqs.size())); return; }
  if (mergearr.size()==0 || count==0) { lwarn("empty merge array, load the cluster merge file"); return; }

  int i,j,k;

  earray<eintarray> otus;
  otus.init(count);
  eintarray otuarr;
  otuarr.init(count);
  for (i=0; i<otuarr.size(); ++i){
    otuarr[i]=i;
    otus[i].add(i);
  }

//  cout << "# finding cluster for: " << seqs.size() << endl;

  // point at the seq we merged with
  float lastdist=mergearr[0].dist;
  bool allin=false;
  for (i=0; i<mergearr.size(); ++i){
    if (lastdist != mergearr[i].dist){
      allin=true;
      for (k=seqs[0]; k!=otuarr[k]; k=otuarr[k]);
      otuarr[seqs[0]]=k;
      for (j=1; j<seqs.size(); ++j){
        for (k=seqs[j]; k!=otuarr[k]; k=otuarr[k]);
        otuarr[seqs[j]]=k;
        if (otuarr[seqs[0]] != otuarr[seqs[j]]) { allin=false; break; }
      }
      if (allin) break;
      lastdist=mergearr[i].dist;
//      cout << "# lastdist: " << lastdist << endl;
    }

   
    otuarr[mergearr[i].y]=mergearr[i].x;
/*
    otus[mergearr[i].x]+=otus[mergearr[i].y];
    for (j=0; j<otus[mergearr[i].y].size(); ++j)
      otuarr[otus[mergearr[i].y][j]]=mergearr[i].x;
    otus[mergearr[i].y].clear();
*/
  }

//  cout << "# finished" << endl;

  if (allin){
    cdist=lastdist;
    for (k=seqs[0]; k!=otuarr[k]; k=otuarr[k]);
    otuarr[seqs[0]]=k;
    for (i=0; i<otuarr.size(); ++i){
      for (k=i; k!=otuarr[k]; k=otuarr[k]);
      otuarr[i]=k;
      if (otuarr[i]==otuarr[seqs[0]])
        seqcluster.add(i);
    }
    return;
  }
  for (j=0; j<seqs.size(); ++j){
    for (k=seqs[j]; k!=otuarr[k]; k=otuarr[k]);
    otuarr[seqs[j]]=k;
  }
  estr str;
  for (j=0; j<seqs.size(); ++j)
    str+=","+estr(otuarr[seqs[j]]);
  cout << str.substr(1) << endl; 
  cdist=-1.0;
}

void eseqclusterData::save(const efile& f)
{
  int i;
  for (i=0; i<mergearr.size(); ++i)
    f.write(estr(mergearr[i].dist)+" "+mergearr[i].x+" "+mergearr[i].y+"\n");
}

void eseqclusterData::load(const efile& f,int _count)
{
  count=_count;
  mergearr.clear();
  estr line;
  estrarray parts;
  while (f.readln(line)){
    if (line.len()==0 || line[0]=='#') continue;
    parts=line.explode(" ");
    if (parts.size()==3){
      mergearr.add(eseqdist(parts[1].i(),parts[2].i(),parts[0].f()));
    }else if (parts.size()==4){
      mergearr.add(eseqdist(parts[2].i(),parts[3].i(),parts[1].f()));
    }else
      ldie("fields in line different than 3: "+line);
  }
}

float eseqclusterData::getMergeDistance(int x,int y)
{
  int i;
  for (i=0; i<mergearr.size(); ++i){
    if (mergearr[i].y==x) x=mergearr[i].x;
    if (mergearr[i].y==y) y=mergearr[i].x;
    if (x==y) return(mergearr[i].dist);
  }
  ldie("did not find merge distance");
  return(0.0);
}


