#include <eutils/emain.h>
#include <eutils/efile.h>
#include <eutils/eheap.h>
#include <eutils/ethread.h>
#include <math.h>

#include "cluster-common.h"

estrarray arr;
ebasicarray<float> mindists;
eintarray mindists_ref;
eintarray mindists_calc;
eintarray done;
eintarray seqs;
eintarray seqs_count;
ebasicarray<float> seqs_maxdist;
ebasicarray<float> seqs_mindist;
ebasicarray<float> seqs_mindist2;
earray<eintarray> seqs_seqs;

earray<ebasicarray<float> > seqs_dists;


void print_seqs()
{
  int i,j;

  for (i=0; i<seqs_maxdist.size(); ++i){
    seqs_maxdist[i]=0.0;
    seqs_mindist2[i]=1.0;
  }


  float tmpdist;

  for (i=0; i<arr.size(); ++i){
    if (done[i]) continue;
    if (seqs_maxdist[mindists_ref[i]]<mindists[i])
      seqs_maxdist[mindists_ref[i]]=mindists[i];
    for (j=0; j<arr.size(); ++j){
      if (i==j || done[j] || mindists_ref[i]==mindists_ref[j]) continue;
      tmpdist=MAX(seqs_dists[mindists_ref[i]][j]-seqs_dists[mindists_ref[i]][i],seqs_dists[mindists_ref[j]][i]-seqs_dists[mindists_ref[j]][j]);
      if (tmpdist<seqs_mindist2[mindists_ref[i]]) seqs_mindist2[mindists_ref[i]]=tmpdist;
      if (tmpdist<seqs_mindist2[mindists_ref[j]]) seqs_mindist2[mindists_ref[j]]=tmpdist;
    }
  }
//  cout << "--- global mindist between any two sequences belonging to different clusters: " << mindist << endl;

  for (i=0; i<seqs.size(); ++i){
    cout << i << " " << seqs_count[i] << " " << seqs_maxdist[i] << " " << seqs_mindist2[i] << " " << seqs_mindist[i] << " {";
    for (j=0; j<seqs_seqs[i].size(); ++j)
      cout << seqs_seqs[i][j] << ",";
    cout << "}" << endl;
  }
}

void partition(float thres)
{
  int i,j;
  int imax;
  int ref;
  int distcount;
  int totaldistcount=0;
  float tmpdist,maxdist;

  done.reserve(arr.size());
  mindists.reserve(arr.size());
  mindists_ref.reserve(arr.size());
  mindists_calc.reserve(arr.size());

  for (i=0; i<arr.size(); ++i)
    done.add(0);

  for (i=0; i<arr.size(); ++i){
    mindists.add(1.0);
    mindists_ref.add(0);
    mindists_calc.add(0);
  }

  ref=(int)(rnd.uniform()*arr.size());

  // find the maximally distant sequence from a random sequence
  maxdist=0.0;
  for (i=0; i<arr.size(); ++i){
    if (i==ref) continue;
    tmpdist=1.0-dist(arr[ref],arr[i]); 
    if (maxdist<tmpdist) { tmpdist=maxdist; imax=i; }
  }
  ref=imax;
  

  maxdist=1.0;
  done[ref]=1;
  mindists[ref]=0.0;
  seqs.add(ref);
  seqs_count.add(arr.size());
  seqs_maxdist.add(0.0);
  seqs_mindist.add(1.0);
  seqs_mindist2.add(1.0);
  seqs_seqs.add(eintarray());
  seqs_dists.add(ebasicarray<float>());
  seqs_dists[seqs_dists.size()-1].init(arr.size());
  while (maxdist>thres){
    maxdist=thres;
    distcount=0;
    for (i=0; i<arr.size(); ++i){
//      if (done[i] || mindists[i]<thres) continue;
      if (done[i]) { seqs_dists[seqs_dists.size()-1][i]=1.0-dist(arr[ref],arr[i]); continue; }

/*
      if (done[i] || mindists[i]<maxdist) continue;
      for (j=mindists_calc[i]; j<seqs.size(); ++j){
        ++distcount;
        tmpdist=1.0-dist(arr[seqs[j]],arr[i]);
        if (tmpdist<mindists[i]) { mindists[i]=tmpdist; mindists_ref[i]=ref; }
        if (mindists[i]<maxdist) break;
      }
      if (mindists[i]>maxdist) {
        maxdist=mindists[i];
        imax=i;
      }
      mindists_calc[i]=j;
*/
      ++distcount;
      tmpdist=1.0-dist(arr[ref],arr[i]);
      seqs_dists[seqs_dists.size()-1][i]=tmpdist;
      if (tmpdist<mindists[i]) {
        mindists[i]=tmpdist;
        --seqs_count[mindists_ref[i]];
        if (seqs_seqs[seqs_seqs.size()-1].find(mindists_ref[i])==-1)
          seqs_seqs[seqs_seqs.size()-1].add(mindists_ref[i]);
        mindists_ref[i]=seqs.size()-1;
        ++seqs_count[mindists_ref[i]];
//        if (seqs_maxdist[seqs.size()-1]<tmpdist)
//          seqs_maxdist[seqs.size()-1]=tmpdist;
      }
      if (mindists[i]>maxdist) {
        maxdist=mindists[i];
        imax=i;
      }
    }
    totaldistcount+=distcount;
    if (distcount==0) break;
    print_seqs();
    cout << "---------"<<endl;

    ref=imax;
    done[ref]=1;
    --seqs_count[mindists_ref[ref]];
    seqs.add(ref);
    seqs_count.add(0);
    seqs_maxdist.add(0.0);
    seqs_mindist.add(maxdist);
    seqs_mindist2.add(1.0);
    seqs_seqs.add(eintarray());
    seqs_dists.add(ebasicarray<float>());
    seqs_dists[seqs_dists.size()-1].init(arr.size());
    cout << arr.size() << " " << seqs.size() << " imax: " << imax << " parent: " << mindists_ref[ref] << " maxdist: " << maxdist << " distcount: " << distcount << endl;
  }
  cout << "Total distances calculated: " << totaldistcount << endl;
  cout << "Total distances full calculation: " << ((long int)arr.size()*(long int)arr.size()/2l-(long int)arr.size()) << endl;
  cout << "Fraction calculated: " << (float)totaldistcount/((long int)arr.size()*(long int)arr.size()/2l-(long int)arr.size()) << endl;

/*
  efile f("hist.dat");
  for (i=0; i<seqs.size(); ++i){
    f.write(estr(i)+" "+seqs_count[i]+" "+seqs_maxdist[i]+" "+seqs_mindist[i]+" {");
    for (j=0; j<seqs_seqs[i].size(); ++j)
      f.write(estr(seqs_seqs[i][j])+",");
    f.write("}\n");
  }
  f.close();
*/
}


int emain()
{
  float t=0.05;
  epregister(t);

  eparseArgs(argvc,argv);

  ldieif(argvc<2,"syntax: "+efile(argv[0]).basename()+" <seqali>");

  load_seqs(argv[1],arr);

  partition(t);

  return(0);
}

