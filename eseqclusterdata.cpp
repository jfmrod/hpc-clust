#include "cluster-common.h"
#include "eseqclusterdata.h"

eseqclusterData::eseqclusterData(): count(0) {}

void eseqclusterData::inferTaxonomy(estrhashof<eseq>& seqs)
{
//  eintarray otuarr;
//  otuarr.init(count);

  earray<estrarray> otutax;
  otutax.init(count);

  earray<eintarray> otumembers;
  otumembers.init(count);

  int i,j;
  for (i=0; i<otumembers.size(); ++i){
//    otuarr[i]=i;
    otumembers[i].add(i);
    if (seqs.values(i).tax.len()){
      otutax[i].add("root");
      otutax[i]+=seqs.values(i).tax.explode(";");
//      seqs.values(i).itax=otutax[i][otutax[i].size()-1];
    }
  }

  int k,l;
  // point at the seq we merged with
  for (i=0; i<mergearr.size(); ++i){
//    otuarr[mergearr[i].y]=mergearr[i].x;
    if (otutax[mergearr[i].y].size()==0 && otutax[mergearr[i].x].size()>0){
      // all members of otu Y should get their first inferred taxonomy
      eintarray& ymembers(otumembers[mergearr[i].y]);
      for (j=0; j<ymembers.size(); ++j)
        seqs[ymembers[j]].itax=estr("(")+mergearr[i].dist+")";
    } else if (otutax[mergearr[i].y].size()>0 && otutax[mergearr[i].x].size()==0){
      eintarray& xmembers(otumembers[mergearr[i].x]);
      for (j=0; j<xmembers.size(); ++j)
        seqs[xmembers[j]].itax=estr("(")+mergearr[i].dist+")";
      otutax[mergearr[i].x]=otutax[mergearr[i].y];
    } else if (otutax[mergearr[i].y].size()>0 && otutax[mergearr[i].x].size()>0){
      for (j=1; j<otutax[mergearr[i].y].size() && j<otutax[mergearr[i].x].size() && otutax[mergearr[i].y][j]==otutax[mergearr[i].x][j]; ++j);

      if (j<otutax[mergearr[i].y].size()){
        estr tax;
        for (l=j; l<otutax[mergearr[i].y].size(); ++l)
          tax+=estr(";(")+mergearr[i].dist+")"+otutax[mergearr[i].y][l];
        eintarray& ymembers(otumembers[mergearr[i].y]);
        for (k=0; k<ymembers.size(); ++k)
          seqs[ymembers[k]].itax=tax+seqs[ymembers[k]].itax;
//        otutax[mergearr[i].y].del(j);
      }
      if (j<otutax[mergearr[i].x].size()){
        estr tax;
        for (l=j; l<otutax[mergearr[i].x].size(); ++l)
          tax+=estr(";(")+mergearr[i].dist+")"+otutax[mergearr[i].x][l];
        eintarray& xmembers(otumembers[mergearr[i].x]);
        for (k=0; k<xmembers.size(); ++k)
          seqs[xmembers[k]].itax=tax+seqs[xmembers[k]].itax;
        otutax[mergearr[i].x].del(j);
      }
    }
    otumembers[mergearr[i].x]+=otumembers[mergearr[i].y];
    otumembers[mergearr[i].y].clear();
  }
  for (j=0; j<otumembers.size(); ++j){
    if (otutax[j].size()>1){
      estr tax;
      for (l=1; l<otutax[j].size(); ++l)
        tax+=estr(";()")+otutax[j][l];
      eintarray& members(otumembers[j]);
      for (k=0; k<members.size(); ++k)
        seqs[members[k]].itax=tax+seqs[members[k]].itax;
    }
  }
  for (i=0; i<seqs.size(); ++i)
    seqs[i].itax.del(0,1);
}


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

int eseqclusterData::getTree(const efloatarray& dists,earray<earray<eintarray> >& otus,earray<eintarray>& otusanc,int size)
{
  if (dists.size()==0) return(0);

  eintarray otuarr;
  int i,j,k;
  otuarr.init(size);
  for (i=0; i<size; ++i)
    otuarr[i]=i;

  otus.init(dists.size());
  otusanc.init(dists.size());
  // point at the seq we merged with
  for (k=0; k<mergearr.size() && mergearr[k].dist>=dists[0]; ++k)
    otuarr[mergearr[k].y]=mergearr[k].x;

  eintarray otuind,otuind2;
  otuind.init(otuarr.size(),-1);

  // translate all the merged seq ids to the last seq id of the otu
  for (i=0; i<otuarr.size(); ++i){
    for (j=otuarr[i]; j!=otuarr[j]; j=otuarr[j]);
    otuarr[i]=j;
    if (otuind[j]==-1){
      otuind[j]=otus[0].size();
      otus[0].add(eintarray());
    }
    otus[0][otuind[j]].add(i);
  }

  otusanc[0].init(otus[0].size());
  for (i=0; i<otusanc[0].size(); ++i)
    otusanc[0][i]=i;
  int l;
  eintarray otuarr2;

  for (l=1; l<dists.size(); ++l){
    otusanc[l].init(otus[0].size());

    otuarr2.init(otus[l-1].size());
    for (i=0; i<otuarr2.size(); ++i)
      otuarr2[i]=i;

    for (; k<mergearr.size() && mergearr[k].dist>=dists[l]; ++k)
      otuarr2[otusanc[l-1][otuind[otuarr[mergearr[k].y]]]]=otusanc[l-1][otuind[otuarr[mergearr[k].x]]];

    otuind2.init(otuarr2.size(),-1);
    for (i=0; i<otuarr2.size(); ++i){
      for (j=otuarr2[i]; j!=otuarr2[j]; j=otuarr2[j]);
      otuarr2[i]=j;
      if (otuind2[j]==-1){
        otuind2[j]=otus[l].size();
        otus[l].add(eintarray());
      }
      otus[l][otuind2[j]].add(i);
    }

    for (i=0; i<otus[0].size(); ++i){
      j=otuarr2[otusanc[l-1][i]];
      otusanc[l][i]=otuind2[j];
    }
  }
  return(otus[0].size());
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
//  estr str;
//  for (j=0; j<seqs.size(); ++j)
//    str+=","+estr(otuarr[seqs[j]]);
//  cout << str.substr(1) << endl; 
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
//  ldie("did not find merge distance");
  return(-1.0);
}


