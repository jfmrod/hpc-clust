#include "eseqclusterdata.h"

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
    ldieif(parts.size()!=3,"fields in line different than 3: "+line);
    mergearr.add(eseqdist(parts[1].i(),parts[2].i(),parts[0].f()));
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


