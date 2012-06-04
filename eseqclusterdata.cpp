#include "cluster-common.h"
#include "eseqclusterdata.h"

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

void eseqclusterData::save(const efile& f)
{
  int i;
  for (i=0; i<mergearr.size(); ++i)
    f.write(estr(mergearr[i].dist)+" "+mergearr[i].x+" "+mergearr[i].y+"\n");
}

void eseqclusterData::load(const efile& f)
{
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


