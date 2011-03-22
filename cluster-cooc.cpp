#include <eutils/emain.h>
#include <eutils/efile.h>
#include <eutils/eheap.h>
//#include <eutils/ethread.h>

inline float dist(const estr& s1,const estr& s2)
{
  int i;
  int count=0;
  for (i=0; i<s1.len(); ++i){
    if (s1[i]==s2[i]) ++count;
  }
  return((float)count/(float)s1.len());
}

inline void getL(int& l,int w,int x,int y)
{
  --w;
  if (x>w/2) { x=w-x; y-=1; l=x*w+y; return; }

  y-=x+1;
  l=x*w+y;
  return;
}

inline void getXY(int l,int w,int& x,int& y)
{
  --w;
  x=(l/w);
  y=(l%w);
  if (y<w-x){ y+=x+1; return; }
  y+=1;
  x=w-x;
}

estrhash arr;
estrhashof<int> arrind;
estrhashof<earray<estr> > samples;
estrhash accsample;
ebasicarray<float> dist_mat;
ebasicarray<float> dist_mat_cooc;

inline float get_dist(int i,int j){ int l; if (i>j){ l=i; i=j; j=l; } getL(l,arr.size(),i,j); return(dist_mat[l]); }

void cluster_seqs(eintarray& seqs,earray<eintarray>& otus)
{
  ebasicarray<float> dists;
  int i,i2,j;
  for (i=0; i<seqs.size()/2; ++i){
    for (j=i+1; j<seqs.size(); ++j){
      dists.add(get_dist(seqs[i],seqs[j]));
    }
    i2=seqs.size()-i-1;
    for (j=i2+1; j<seqs.size(); ++j)
      dists.add(get_dist(seqs[i2],seqs[j]));
  }
  if (seqs.size()%2==1){
    i=seqs.size()/2;
    for (j=i+1; j<seqs.size(); ++j)
      dists.add(get_dist(seqs[i],seqs[j]));
  }
  
  int x,y;
//  cout << "# seqs.size(): " << seqs.size() << endl;
//  cout << "# dist.size(): " << dists.size() << endl;
  eintarray sorti(sort(dists));
/*
  for (i=dists.size()-1; i>0; --i){
    if (dists[sorti[i]]<0.90) break;
    getXY(sorti[i],seqs.size(),x,y);
    cout << dists[sorti[i]] << " " << x << " " << y << endl;
  }
*/

  eintarray otuarr;
  for (i=0; i<seqs.size(); ++i)
    otuarr.add(-1);

  otus.clear();

//  earray<eintarray> otus;

  int arr1,arr2;
  for (i=dists.size()-1; i>0; --i){
    getXY(sorti[i],seqs.size(),x,y);
    if (dists[sorti[i]]<0.90) break;
    ldieif(x>=seqs.size() || x<0 || y<0 || y>=seqs.size(),"something wrong in x,y: "+estr(x)+","+y+"    "+estr(seqs.size()));
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
      arr1=otuarr[y]; arr2=otuarr[x];
      if (otuarr[x]<otuarr[y]) { arr1=otuarr[x]; arr2=otuarr[y]; }
      otus[arr1]+=otus[arr2];
      otus[arr2].clear();
    }
//    cout << dsort[i] << " " << x << " " << seq1[dsort[i]] << " " << y << " " << seq2[dsort[i]] << " " << dist_mat[dsort[i]] << endl;
  }

//  cout << "# tmp total otus: " << otus.size() << endl;

  for (i=0; i<otus.size(); ++i)
    if (otus[i].size()==0)
      { otus.erase(i); --i; }

  for (i=0; i<otuarr.size(); ++i){
    if (otuarr[i]==-1){
      otus.add(eintarray());
      otus[otus.size()-1].add(i);
    }
  }

//  cout << "# total otus: " << otus.size() << endl;
}

inline float dist_cooc(int a,int b)
{
  int i,j;
  eintarray seqs;
  estr sample1,sample2;
  sample1=accsample[arr.keys(a)];
  sample2=accsample[arr.keys(b)];
  for (i=0; i<samples[sample1].size(); ++i){
    if (arrind.exists(samples[sample1][i]))
      seqs.add(arrind[samples[sample1][i]]);
  }
  for (i=0; i<samples[sample2].size(); ++i){
    if (arrind.exists(samples[sample2][i]))
      seqs.add(arrind[samples[sample2][i]]);
  }

  earray<eintarray> clusters;
  cluster_seqs(seqs,clusters);

  // count how many clusters have seqs in both neighborhoods
  int sharedc=0;
  for (i=0; i<clusters.size(); ++i){
    int occurs=0x00;
    for (j=0; j<clusters[i].size(); ++j){
      if (clusters[i][j]<samples[sample1].size())
        occurs=occurs|0x01;
      else
        occurs=occurs|0x02;
    }
    if (occurs==0x03) ++sharedc;
  }

  if (sharedc>0)
    cout << "# " << a << " " << b << " clusters: " << clusters.size() << " shared: " << sharedc << " (" << (float)sharedc/clusters.size() << ")" << endl;

  return((float)sharedc/clusters.size());
}


void calc_cooc()
{
  int c=0;
  int i,i2,j;
  for (i=0; i<arr.size()/2; ++i){
    for (j=i+1; j<arr.size(); ++j)
      dist_mat_cooc[c++]=dist_cooc(i,j);
    i2=arr.size()-i-1;
    for (j=i2+1; j<arr.size(); ++j)
      dist_mat_cooc[c++]=dist_cooc(i2,j);
  }
  if (arr.size()%2==1){
    i=arr.size()/2;
    for (j=i+1; j<arr.size(); ++j)
      dist_mat_cooc[c++]=dist_cooc(i,j);
  }
//  cout << "# c: " << c << endl;
}

void calc2()
{
  int c=0;
  int i,i2,j;
  for (i=0; i<arr.size()/2; ++i){
    for (j=i+1; j<arr.size(); ++j)
      dist_mat[c++]=dist(arr.values(i),arr.values(j));
    i2=arr.size()-i-1;
    for (j=i2+1; j<arr.size(); ++j)
      dist_mat[c++]=dist(arr.values(i2),arr.values(j));
  }
  if (arr.size()%2==1){
    i=arr.size()/2;
    for (j=i+1; j<arr.size(); ++j)
      dist_mat[c++]=dist(arr.values(i),arr.values(j));
  }
//  cout << "# c: " << c << endl;
}

void calc(int start,int end)
{
  int c=0;
  int i,i2,j;
  for (i=start; i<end; ++i){
    getL(c,arr.size(),i,i+1);
    for (j=i+1; j<arr.size(); ++j)
      dist_mat[c++]=dist(arr.values(i),arr.values(j));
    i2=arr.size()-i-1;
    for (j=i2+1; j<arr.size(); ++j)
      dist_mat[c++]=dist(arr.values(i2),arr.values(j));
  }
  if (end==arr.size()/2 && arr.size()%2==1){
    getL(c,arr.size(),i,i+1);
    i=arr.size()/2;
    for (j=i+1; j<arr.size(); ++j)
      dist_mat[c++]=dist(arr.values(i),arr.values(j));
  }
}

int emain()
{
  eparseArgs(argvc,argv);

  ldieif(argvc<2,"syntax: "+efile(argv[0]).basename()+" <seqali> <samples>");


  estr line;
  estr name;
  efile f(argv[1]);

  int i,j;

  while (f.readln(line)){
    if (line.len()==0 || line[0]=='#') continue;
    
    i=line.find(" ");
    if (i==-1) continue;
    name=line.substr(0,i);
    line.del(0,i);
    line.trim();

    i=name.find("|");
    name.del(i);

    if (!arr.exists(name)){
      arrind.add(name,arr.size());
      arr.add(name,line);
    }else
      arr[name]+=line;
  }
  cout << "# seqs: " << arr.size() << endl;

  efile f2(argv[2]);

  f2.readln(line);
  i=line.find("--");
  while (i!=-1){
    name=line.substr(i+3);
    j=samples.size();
    samples.add(name,earray<estr>());
    do {
      if (!f2.readln(line)) { i=-1; break; }
      i=line.find("--");
      if (i==-1){ samples[j].add(line); accsample.add(line,name); }
    } while(i==-1);
  }

  cout << "# sample count: " << samples.size() << endl;
  cout << "# sample seqs: " << accsample.size() << endl;


  int i2;

//  for (i=0; i<arr.size()-1; ++i)
//    for (j=i+1; j<arr.size(); ++j)
//      dist_mat.add(dist(arr.values(i),arr.values(j)));
  for (i=0; i<arr.size()*(arr.size()+1)/2-arr.size(); ++i){
    dist_mat.add(0.0);
    dist_mat_cooc.add(0.0);
  }

/*
  int c=0;
  int l;
  for (i=0; i<arr.size()/2; ++i){
    getL(c,arr.size(),i,j);
    for (j=i+1; j<arr.size(); ++j)
      dist_mat[c++]=dist(arr.values(i),arr.values(j));
    i2=arr.size()-i-1;
    for (j=i2+1; j<arr.size(); ++j)
      dist_mat[c++]=dist(arr.values(i2),arr.values(j));
  }
  if (arr.size()%2==1){
    i=arr.size()/2;
    for (j=i+1; j<arr.size(); ++j)
      dist_mat[c++]=dist(arr.values(i),arr.values(j));
  }
*/

/*
  ethread thread;
  thread.wait();
  evararray vararr;

  vararr.add(new int(0));
  vararr.add(new int(arr.size()/4));
  vararr.add(dist_mat);
  vararr.add(arr);
  thread.run(efunc(calc,vararr));
*/

//  calc(0,arr.size()/4,dist_mat,arr);
//  calc(arr.size()/4,arr.size()/2,dist_mat,arr);

  calc2();
  cout << "# distances: " << dist_mat.size() << endl;

  calc_cooc();


//      dist_mat[i*arr.size()-2*i+j]+=dist(arr.values(i),arr.values(j));

  eintarray dsort(sort(dist_mat));

  eintarray otuarr;
  for (i=0; i<arr.size(); ++i)
    otuarr.add(-1);

  earray<eintarray> otus;

  int x,y;
  int arr1,arr2;
  for (i=dist_mat.size()-1; i>0; --i){
    getXY(dsort[i],arr.size(),x,y);
    if (dist_mat[dsort[i]]<0.97) break;
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
      arr1=otuarr[y]; arr2=otuarr[x];
      if (otuarr[x]<otuarr[y]) { arr1=otuarr[x]; arr2=otuarr[y]; }
      otus[arr1]+=otus[arr2];
      otus[arr2].clear();
    }
//    cout << dsort[i] << " " << x << " " << seq1[dsort[i]] << " " << y << " " << seq2[dsort[i]] << " " << dist_mat[dsort[i]] << endl;
  }

  cout << "# tmp total otus: " << otus.size() << endl;

  for (i=0; i<otus.size(); ++i)
    if (otus[i].size()==0)
      { otus.erase(i); --i; }

  cout << "# total otus: " << otus.size() << endl;

  estr outfile="otu.dat";

  efile of(outfile);
  int otucount=0;
  for (i=0; i<otus.size(); ++i){
    if (otus[i].size()==0) continue;
    of.write(">OTU"+estr(otucount)+" "+estr(otus[i].size())+"\n");
    for (j=0; j<otus[i].size(); ++j)
      of.write(arr.keys(otus[i][j])+"\n");
    ++otucount;
  }

  return(0);
}

