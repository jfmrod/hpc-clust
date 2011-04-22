#include "cluster-common.h"
#include <eutils/efile.h>
#include <eutils/eheap.h>
#include <math.h>

/*
eblockarray::eblockarray(): blocksize(100000), count(0) { blocks.add(new eseqdist[blocksize]); }

eblockarray::~eblockarray()
{
  long int i;
  for (i=0; i<blocks.size(); ++i)
    if (blocks[i]) delete blocks[i];
}

void eblockarray::add(const eseqdist& sdist)
{
  lastblock()[count%blocksize]=sdist;
  ++count;
  if (count%blocksize==0)
    blocks.add(new eseqdist[blocksize]);
}

const eseqdist& eblockarray::operator[](long int i) const
{
//  ldieif(i/blocksize>=blocks.size(),"out of range: blocks.size: "+estr(blocks.size())+" i/blocksize: "+estr(i/blocksize));
  return(blocks[i/blocksize][i%blocksize]);
}

eseqdist& eblockarray::operator[](long int i)
{
//  ldieif(i/blocksize>=blocks.size(),"out of range: blocks.size: "+estr(blocks.size())+" i/blocksize: "+estr(i/blocksize));
  return(blocks[i/blocksize][i%blocksize]);
}

inline void addarray(eseqdist *arr1,long int& i1,long int n1,eseqdist *arr2,long int& i2,long int n2)
{
  for (; i1<n1 && i2<n2; ++i1,++i2)
    arr1[i1]=arr2[i2];
}

eblockarray& eblockarray::merge(eblockarray& barr)
{
  if (!barr.size()) return(*this);

  long int i,j,k;
  eseqdist *tmparr;
  if (blocksize-(count%blocksize) >= barr.count%blocksize){
    i=count%blocksize; j=0;
    addarray(lastblock(),i,blocksize,barr.lastblock(),j,barr.count%blocksize);    
    count+=barr.count;
    if (count%blocksize==0)
      blocks.add(new eseqdist[blocksize]);

    if (barr.blocks.size()>1){
      tmparr=lastblock();
      blocks[blocks.size()-1]=barr.blocks[0];
      barr.blocks[0]=0x00;
      for (j=1; j<barr.blocks.size()-1; ++j){
        blocks.add(barr.blocks[j]);
        barr.blocks[j]=0x00;
      }
      blocks.add(tmparr);
    }
  }else{
    i=count%blocksize; j=0;
    addarray(lastblock(),i,blocksize,barr.lastblock(),j,barr.count%blocksize);
    for (k=0; k<barr.blocks.size()-1; ++k){
      blocks.add(barr.blocks[k]);
      barr.blocks[k]=0x00;
    }

    blocks.add(new eseqdist[blocksize]);
    i=0;
    addarray(lastblock(),i,blocksize,barr.lastblock(),j,barr.count%blocksize);
    count+=barr.count;
  }

  ldieif (count/blocksize>=blocks.size() || count/blocksize+1<blocks.size(),"error in merge: "+estr(count/blocksize)+" "+estr(blocks.size()));

  barr.clear();
  return(*this);
}

void eblockarray::clear()
{
  long int i;
  for (i=0; i<blocks.size(); ++i)
    if (blocks[i]) delete blocks[i];
  blocks.clear();
  count=0;
  blocks.add(new eseqdist[blocksize]);
}

void eblockarray::swap(long int i,long int j)
{
  eseqdist tmp;
  tmp=operator[](i);
  operator[](i)=operator[](j);
  operator[](j)=tmp;
}

void eblockarray::sort()
{
  long int i;
  long int root,child;
  long int tmpsize;

  // heapify
  tmpsize=size();
  for (i=size()/2; i>=0; --i){
    root=i;
    while (root<<1 < tmpsize){
      child=root<<1;
      if (child+1<tmpsize && operator[](child) < operator[](child+1))
        ++child;
      if (operator[](root) < operator[](child)){
        swap(root,child);
        root=child;
      } else
        break;
    }
  }

  // build sorted array
  for (tmpsize=size()-1; tmpsize>0; --tmpsize){
    swap(0,tmpsize);
    root=0;
    while (root<<1 < tmpsize){
      child=root<<1;
      if (child+1<tmpsize && operator[](child) < operator[](child+1))
        ++child;
      if (operator[](root) < operator[](child)){
        swap(root,child);
        root=child;
      } else
        break;
    }
  }
}
*/

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

inline unsigned char nuc_compress(unsigned char c){
  switch(c){
   case 'A': return(0x00);
   case 'T': return(0x01);
   case 'U': return(0x01);
   case 'G': return(0x02);
   case 'C': return(0x03);
   case 'R': return(0x04);
   case 'Y': return(0x05);
   case 'S': return(0x06);
   case 'W': return(0x07);
   case 'K': return(0x08);
   case 'M': return(0x09);
   case 'B': return(0x0A);
   case 'D': return(0x0B);
   case 'H': return(0x0C);
   case 'V': return(0x0D);
   case 'N': return(0x0E);
   case '-': return(0x0F);
  }
  ldie("unknown nucleotide:"+estr(c));
  return(0x0F);
}

estr seq_compress(const estr& seq)
{
  int i;
  estr res;
  res.reserve((seq.len()+1)/2);
  char tmp;
  for (i=0; i<seq.len()-2; i+=2){
    tmp=nuc_compress(seq[i]);
    tmp=tmp|(nuc_compress(seq[i+1])<<4);
    res._str[i/2]=tmp;
  }
  tmp=nuc_compress(seq[i]);
  if (i+1<seq.len())
    tmp=tmp|(nuc_compress(seq[i+1])<<4);
  res._str[i/2]=tmp;
  res._strlen=(i+1)/2;
  return(res);
}

void load_seqs_compressed(const estr& filename,earray<estr>& arr,int& seqlen)
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
    seqlen=line.len();
    arr.add(seq_compress(line.uppercase()));
  }
  cout << "# seqs: " << arr.size() << " seqlen: "<< seqlen<< endl;
}

void load_seqs_compressed(const estr& filename,estrarray& arr,int& seqlen)
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
    seqlen=line.len();
    arr.add(name,seq_compress(line.uppercase()));
  }
  cout << "# seqs: " << arr.size() << " seqlen: "<< seqlen<< endl;
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

void load_seqs(const estr& filename,estrarray& arr,estrhashof<int>& arrind)
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
    if (!arrind.exists(name)){
      arrind.add(name,arrind.size());
      arr.add(name,line);
    }
  }
  cout << "# seqs: " << arrind.size() << endl;
}

void load_clusters(const estr& filename,estrhashof<int>& arrind,earray<eintarray>& otus)
{
  estr line;
  estr name;
  efile f(filename);

  int i=-1;

  while (f.readln(line)){
    if (line.len()==0 || line[0]=='#') continue;

    if (line[0]=='>') { otus.add(eintarray()); ++i; continue; }
	    
    ldieif(!arrind.exists(line),"something wrong");

    otus[i].add(arrind[line]);
  }
  cout << "# otus: " << otus.size() << endl;
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


inline void getXY2(int l,int w,int& x,int& y)
{
  x=(l/w);
  y=(l%w);
  if (y<w-x){ y+=x; return; }
  x=w-x;
}

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
      if (tmpid>=thres) dists.add(eseqdist(i,j,tmpid));
    }
    i2=arr.size()-i-2;
    for (j=i2+1; j<arr.size(); ++j){
      tmpid=dist(arr.values(i2),arr.values(j));
      if (tmpid>=thres) dists.add(eseqdist(i2,j,tmpid));
    }
  }
  if (node==tnodes-1 && arr.size()%2==0){
    i=arr.size()/2-1;
    for (j=i+1; j<arr.size(); ++j){
      tmpid=dist(arr.values(i),arr.values(j));
      if (tmpid>=thres) dists.add(eseqdist(i,j,tmpid));
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
      if (tmpid>=thres) dists.add(eseqdist(i,j,tmpid));
    }
    i2=arr.size()-i-2;
    for (j=i2+1; j<arr.size(); ++j){
      tmpid=dist(arr.values(i2),arr.values(j));
      if (tmpid>=thres) dists.add(eseqdist(i2,j,tmpid));
    }
  }
  if (node==tnodes-1 && arr.size()%2==0){
    i=arr.size()/2-1;
    for (j=i+1; j<arr.size(); ++j){
      tmpid=dist(arr.values(i),arr.values(j));
      if (tmpid>=thres) dists.add(eseqdist(i,j,tmpid));
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

int calc_dists_nogap_compressed(earray<estr>& arr,eblockarray<eseqdist>& dists,int seqlen,int node,int tnodes,float thres)
{
  ldieif(arr.size()==0,"no distances in array");
  long int i,i2,j;
  long int start,end;

  start=(long int)(node)*(long int)(arr.size()-1)/(long int)(2*tnodes);
  end=(long int)(node+1)*(long int)(arr.size()-1)/(long int)(2*tnodes);
//  cerr << "# start: " << start << " end: " << end << endl;

  float tmpid;

  for (i=start; i<end; ++i){
    for (j=i+1; j<arr.size(); ++j){
      tmpid=dist_nogap_compressed(arr[i],arr[j],seqlen);
      if (tmpid>=thres) dists.add(eseqdist(i,j,tmpid));
    }
    i2=arr.size()-i-2;
    for (j=i2+1; j<arr.size(); ++j){
      tmpid=dist_nogap_compressed(arr[i2],arr[j],seqlen);
      if (tmpid>=thres) dists.add(eseqdist(i2,j,tmpid));
    }
//    if (i-start==(end-start)/10)
//      cout << "# predicted: " << dists.size()*10 << endl;
//    if (dists.size()>dists.capacity()*2/3)
//      dists.reserve(2*dists.capacity());
  }
  if (node==tnodes-1 && arr.size()%2==0){
    i=arr.size()/2-1;
    for (j=i+1; j<arr.size(); ++j){
      tmpid=dist_nogap_compressed(arr[i],arr[j],seqlen);
      if (tmpid>=thres) dists.add(eseqdist(i,j,tmpid));
    }
  }
//  cout << "# n: " << node << " tnodes: " << tnodes << " start: " << start << " end: " << end << " distances: " << dists.size() << endl;
//  cout << "# computed distances: " << count << endl;
//  heapsort(dists);
  return(dists.size());
}

int calc_dists_nogap_compressed(estrarray& arr,eblockarray<eseqdist>& dists,int seqlen,int node,int tnodes,float thres)
{
  ldieif(arr.size()==0,"no distances in array");
  int i,i2,j;
  int start,end;

  start=node*(arr.size()-1)/(2*tnodes);
  end=(node+1)*(arr.size()-1)/(2*tnodes);

  float tmpid;
  int count=0;
//  dists.reserve(500000);

  for (i=start; i<end; ++i){
    for (j=i+1; j<arr.size(); ++j){
      tmpid=dist_nogap_compressed(arr.values(i),arr.values(j),seqlen);
      if (tmpid>=thres) dists.add(eseqdist(i,j,tmpid));
      ++count;
    }
    i2=arr.size()-i-2;
    for (j=i2+1; j<arr.size(); ++j){
      tmpid=dist_nogap_compressed(arr.values(i2),arr.values(j),seqlen);
      if (tmpid>=thres) dists.add(eseqdist(i2,j,tmpid));
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
      tmpid=dist_nogap_compressed(arr.values(i),arr.values(j),seqlen);
      if (tmpid>=thres) dists.add(eseqdist(i,j,tmpid));
      ++count;
    }
  }
//  cout << "# n: " << node << " tnodes: " << tnodes << " start: " << start << " end: " << end << " distances: " << dists.size() << endl;
//  cout << "# computed distances: " << count << endl;
//  heapsort(dists);
  return(dists.size());
}

int calc_dists_nogap_compressed(earray<estr>& arr,ebasicarray<eseqdist>& dists,int seqlen,int node,int tnodes,float thres)
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
      tmpid=dist_nogap_compressed(arr[i],arr[j],seqlen);
      if (tmpid>=thres) dists.add(eseqdist(i,j,tmpid));
      ++count;
    }
    i2=arr.size()-i-2;
    for (j=i2+1; j<arr.size(); ++j){
      tmpid=dist_nogap_compressed(arr[i2],arr[j],seqlen);
      if (tmpid>=thres) dists.add(eseqdist(i2,j,tmpid));
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
      tmpid=dist_nogap_compressed(arr[i],arr[j],seqlen);
      if (tmpid>=thres) dists.add(eseqdist(i,j,tmpid));
      ++count;
    }
  }
//  cout << "# n: " << node << " tnodes: " << tnodes << " start: " << start << " end: " << end << " distances: " << dists.size() << endl;
//  cout << "# computed distances: " << count << endl;
//  heapsort(dists);
  return(dists.size());
}

int calc_dists_nogap_compressed(estrarray& arr,ebasicarray<eseqdist>& dists,int seqlen,int node,int tnodes,float thres)
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
      tmpid=dist_nogap_compressed(arr.values(i),arr.values(j),seqlen);
      if (tmpid>=thres) dists.add(eseqdist(i,j,tmpid));
      ++count;
    }
    i2=arr.size()-i-2;
    for (j=i2+1; j<arr.size(); ++j){
      tmpid=dist_nogap_compressed(arr.values(i2),arr.values(j),seqlen);
      if (tmpid>=thres) dists.add(eseqdist(i2,j,tmpid));
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
      tmpid=dist_nogap_compressed(arr.values(i),arr.values(j),seqlen);
      if (tmpid>=thres) dists.add(eseqdist(i,j,tmpid));
      ++count;
    }
  }
//  cout << "# n: " << node << " tnodes: " << tnodes << " start: " << start << " end: " << end << " distances: " << dists.size() << endl;
//  cout << "# computed distances: " << count << endl;
//  heapsort(dists);
  return(dists.size());
}

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
      if (tmpid>=thres) dists.add(eseqdist(i,j,tmpid));
      ++count;
    }
    i2=arr.size()-i-2;
    for (j=i2+1; j<arr.size(); ++j){
      tmpid=dist_nogap(arr.values(i2),arr.values(j));
      if (tmpid>=thres) dists.add(eseqdist(i2,j,tmpid));
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
      if (tmpid>=thres) dists.add(eseqdist(i,j,tmpid));
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
      if (tmpid>=thres) dists.add(eseqdist(i,j,tmpid));
    }
    i2=arr.size()-i-2;
    for (j=i2+1; j<arr.size(); ++j){
      tmpid=dist_nogap(arr.values(i2),arr.values(j));
      if (tmpid>=thres) dists.add(eseqdist(i2,j,tmpid));
    }
  }
  if (node==tnodes-1 && arr.size()%2==0){
    i=arr.size()/2-1;
    for (j=i+1; j<arr.size(); ++j){
      tmpid=dist_nogap(arr.values(i),arr.values(j));
      if (tmpid>=thres) dists.add(eseqdist(i,j,tmpid));
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
      if (tmpid>=thres) dists.add(eseqdist(i,j,tmpid));
    }
    i2=arr.size()-i-2;
    for (j=i2+1; j<arr.size(); ++j){
      tmpid=dist_nogap(arr.values(i2),arr.values(j),arrgaps[i2],arrgaps[j]);
      if (tmpid>=thres) dists.add(eseqdist(i2,j,tmpid));
    }
  }
  if (node==tnodes-1 && arr.size()%2==0){
    i=arr.size()/2-1;
    for (j=i+1; j<arr.size(); ++j){
      tmpid=dist_nogap(arr.values(i),arr.values(j),arrgaps[i],arrgaps[j]);
      if (tmpid>=thres) dists.add(eseqdist(i,j,tmpid));
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


