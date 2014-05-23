#include "cluster-common.h"
#include <eutils/efile.h>
#include <eutils/eheap.h>
#include <eutils/eregexp.h>
#include <math.h>

/*
int gap_matrix[16][16];

void initDistMatrix()
{
  unsigned long int i,j;
  for (i=0u; i<16u; ++i){
    for (j=0u; j<16u; ++j){
      gap_matrix[i][j]=0u;
      if (i == j)
        ++gap_matrix[i][j];
    }
  }
}
*/

//char lt_gap_count[0x1u << 16];

//char lt_nogap_len[0x1u<<16];
//char lt_nogap_count[0x1u<<16];

char lt_tamura_len[0x1u<<16];
char lt_tamura_p[0x1u<<16];
char lt_tamura_q[0x1u<<16];
char lt_tamura_gc[0x1u<<16];

/*
#define lt_tamura_init(i,j,lt_len,lt_p,lt_q,mask)  \
  {                                                \
    if ((i&mask)==mask || (j&mask)==mask)          \
      ++lt_len[i^j];                               \
    else if (((i^j)&mask)==(0x02020202u&mask))     \
      ++lt_p[i^j];                                 \
    else if (((i^j)&mask)==(0x03030303u&mask))     \
      ++lt_q[i^j];                                 \
  }
 
#define lt_tamura_gcinit(i,lt_gc,mask) { if ((i&mask)==(0x02020202u&mask) || (i&mask)==(0x03030303u&mask)) ++lt_gc[i]; }

void gap_initLookupTable()
{
  unsigned int i,j;
  for (i=0u; i<0x1u<<16; ++i){
    for (j=0u; j<=i; ++j){
      lt_gap_count[i^j]=0;
      lt_gap_init(i,j,lt_gap_count,0xfu);
      lt_gap_init(i,j,lt_gap_count,0xf0u);
      lt_gap_init(i,j,lt_gap_count,0xf00u);
      lt_gap_init(i,j,lt_gap_count,0xf000u);
    }
  }
}

void nogap_initLookupTable()
{
  unsigned int i,j;
  for (i=0u; i<0x1u<<16; ++i){
    for (j=0u; j<=i; ++j){
      lt_nogap_len[i&j]=0;
      lt_nogap_count[i^j]=0;
      lt_nogap_init(i,j,lt_nogap_len,lt_nogap_count,0xfu);
      lt_nogap_init(i,j,lt_nogap_len,lt_nogap_count,0xf0u);
      lt_nogap_init(i,j,lt_nogap_len,lt_nogap_count,0xf00u);
      lt_nogap_init(i,j,lt_nogap_len,lt_nogap_count,0xf000u);
    }
  }
}

void initLookupTable()
{
  unsigned int i,j;
  for (i=0u; i<0x1u<<16; ++i){
    for (j=0u; j<=i; ++j){
      lt_gap_count[i^j]=0;
      lt_gap_init(i,j,lt_gap_count,0xfu);
      lt_gap_init(i,j,lt_gap_count,0xf0u);
      lt_gap_init(i,j,lt_gap_count,0xf00u);
      lt_gap_init(i,j,lt_gap_count,0xf000u);
      
      lt_nogap_len[i&j]=0;
      lt_nogap_count[i^j]=0;
      lt_nogap_init(i,j,lt_nogap_len,lt_nogap_count,0xfu);
      lt_nogap_init(i,j,lt_nogap_len,lt_nogap_count,0xf0u);
      lt_nogap_init(i,j,lt_nogap_len,lt_nogap_count,0xf00u);
      lt_nogap_init(i,j,lt_nogap_len,lt_nogap_count,0xf000u);

//      lt_tamura_len[i&j]=0;
//      lt_tamura_p[i^j]=0;
//      lt_tamura_q[i^j]=0;
//      lt_tamura_init(i,j,lt_tamura_len,lt_tamura_p,lt_tamura_q,0xfu);

    }

//    lt_tamura_gc[i]=0;
//    lt_tamura_gcinit(i,lt_tamura_gc,0xfu);
//    lt_tamura_gcinit(i,lt_tamura_gc,0xf0u);
//    lt_tamura_gcinit(i,lt_tamura_gc,0xf00u);
//    lt_tamura_gcinit(i,lt_tamura_gc,0xf000u);

  }
}
*/


float warnMemThres=1.0e40;
bool warnedMemThres=false;
float exitMemThres=1.0e40;
bool ignoreMemThres=false;




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
void cooc_init(earray<eintarray>& neigharr,const estrarray& arr,const eintarray& otuarr,estrhashof<eintarray>& samples,estrhash& accsample,estrhashof<int>& arrind,int otucount)
{
 
  int i,k,l;
  estr acc;
  estr sample;

  cout << "# otucount: " << otucount << endl;


  for (i=0; i<arr.size(); ++i)
    neigharr.add(eintarray());

  estrarray tarr;

  cout << "# neighcount: " << neigharr.size() << endl;
  for (i=0; i<arr.size(); ++i){
    acc=arr.keys(i);
    tarr=acc.explode(":");
    acc=tarr[0];

    if (!accsample.exists(acc)){ lwarn("Accession: "+acc+" not found in samples"); continue; }
    sample=accsample[acc];

    for (k=0; k<otucount; ++k)
      neigharr[i].add(0);

    ldieif(!samples.exists(sample),"sample not found: "+sample);
    for (k=0; k<samples[sample].size(); ++k){
      if (acc==samples[sample][k]) continue;
      l=otuarr[samples[sample][k]];
      ldieif(l<0 || l>=otucount,"Something wrong");
//      cout << " i: " << i << " l: " << l << endl;
      ++neigharr[i][l];
    }
  }

  cout << "# neigharr.size(): " << neigharr.size() << endl;
}

void cooc_calc2(int node,int tnodes,ebasicarray<float>& dist_mat,earray<eintarray>& neigharr)
{
  int c=0;
  int i,i2,j;
  int start,end;

  start=node*(neigharr.size()-1)/(2*tnodes);
  end=(node+1)*(neigharr.size()-1)/(2*tnodes);

  for (i=start; i<end; ++i){
//    getL(c,neigharr.size(),i,i+1);
    for (j=i+1; j<neigharr.size(); ++j)
      dist_mat.add(cooc_dist(i,j,neigharr));
    i2=neigharr.size()-i-2;
    for (j=i2+1; j<neigharr.size(); ++j)
      dist_mat.add(cooc_dist(i2,j,neigharr));
  }
  if (node==tnodes-1 && neigharr.size()%2==0){
    i=neigharr.size()/2-1;
    for (j=i+1; j<neigharr.size(); ++j)
      dist_mat.add(cooc_dist(i,j,neigharr));
  }
}

void cooc_calc(int node,int tnodes,ebasicarray<float>& dist_mat,earray<eintarray>& neigharr)
{
  int c=0;
  int i,i2,j;
  int start,end;

  start=node*(neigharr.size()-1)/(2*tnodes);
  end=(node+1)*(neigharr.size()-1)/(2*tnodes);

  for (i=start; i<end; ++i){
    getL(c,neigharr.size(),i,i+1);
    for (j=i+1; j<neigharr.size(); ++j)
      dist_mat[c++]=cooc_dist(i,j,neigharr);
    i2=neigharr.size()-i-2;
    for (j=i2+1; j<neigharr.size(); ++j)
      dist_mat[c++]=cooc_dist(i2,j,neigharr);
  }
  if (node==tnodes-1 && neigharr.size()%2==0){
    i=neigharr.size()/2-1;
    getL(c,neigharr.size(),i,i+1);
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

estr seq_compress(const estr& seq)
{
  int i;
  int slen=((seq.len()+15)>>4)<<3; // Make string 64bit aligned. This is needed in the dist functions
  estr res;
  res.reserve(slen);
  unsigned char tmp;
  for (i=0; i<seq.len()-2; i+=2){
    tmp=nuc_compress(seq[i]);
//    tmp=tmp|(0xF0&(nuc_compress(seq[i+1])<<4));
    tmp=tmp|(nuc_compress(seq[i+1])<<4u);
    res._str[i/2]=tmp;
  }
  tmp=nuc_compress(seq[i]);
  if (i+1<seq.len())
    tmp=tmp|(nuc_compress(seq[i+1])<<4u);
  res._str[i/2]=tmp;
//  res._strlen=(seq.len()+1)/2;
  res._strlen=slen; // needs to be set to full size including 0x00 padding otherwise the padding does not get copied
  for (i=i/2+1; i<slen; ++i)  // Set remaining bytes to 0x00. This is required for the dist functions
    res._str[i]=0x00;

/*
  cout << seq << endl;

  unsigned long *ep1=(unsigned long*)(res._str)+((seq.len()+15)>>4)-1;
  unsigned long *p1=(unsigned long*)res._str;
  for (; p1!=ep1; ++p1){
    printf("%.16lx",*p1);
  }
  printf("%.16lx\n",*p1);
*/
  return(res);
}

ernd rng;

inline char rndnuc()
{
  float tmp=rng.uniform();
  if (tmp<0.25) return('A');
  if (tmp<0.50) return('T');
  if (tmp<0.75) return('G');
  return('C');
}

/*
estr seq_mutate_compress(estr& seq,int& nextmutation,float imutrate)
{
  while (nextmutation<seq.len()){
    seq[nextmutation]=rndnuc();
    nextmutation+=(int)rng.exponential(imutrate);
  }
  nextmutation-=seq.len();
  return(seq_compress(seq));
}
void load_seqs_mutate_compressed(const estr& filename,estrarray& arr,int& seqlen,float avgmutseq)
{
  estr line;
  estr name;
  efile f(filename);

  int i;

  float imutrate;
  int nextmutation=-1;
  

  while (f.readln(line)){
    if (line.len()==0 || line[0]=='#') continue;
    
    i=line.find(" ");
    if (i==-1) continue;
    name=line.substr(0,i);
    line.del(0,i);
    line.trim();
    seqlen=line.len();
    if (nextmutation==-1){
      imutrate=(float)line.len()/avgmutseq;
      nextmutation=(int)rng.exponential(imutrate);
    }
    arr.add(seq_mutate_compress(line.uppercase(),nextmutation,imutrate));
  }
  cout << "# seqs: " << arr.size() << " seqlen: "<< seqlen<< endl;
}
*/

void load_short_compressed(const estr& filename,estrarray& arr,int& seqlen)
{
  estr line;
  estr name;
  estr start;
  estr end;
  efile f(filename);

  int i;

  while (f.readln(line)){
    if (line.len()==0 || line[0]=='#') continue;
    
    i=line.find(" ");
    if (i==-1) continue;
    name=line.substr(0,i);
    line.del(0,i);
    line.trim();
    i=line.find(" ");
    if (i==-1) continue;
    start=line.substr(0,i);
    line.del(0,i);
    line.trim();
    i=line.find(" ");
    if (i==-1) continue;
    end=line.substr(0,i);
    line.del(0,i);
    line.trim();
    seqlen=line.len();
    arr.add(name,seq_compress(line.uppercase()));
  }
  cout << "# seqs: " << arr.size() << " seqlen: "<< seqlen<< endl;
}

void eshortseq::compress()
{
  int i;
  for (i=0; i<seq.len() && seq[i]=='-'; ++i);
  b=i;
  for (i=seq.len()-1; i>=0 && seq[i]=='-'; --i);
  e=i+1;
  ldieif(b>e,"Empty sequence: "+seq);
  seq=seq_compress(seq);
}

void load_short_compressed(const estr& filename,ebasicarray<eshortseq>& arr)
{
  estr line;
  estr name;
  estr start;
  estr end;
  efile f;

  if (filename=="-")
    f.open(stdin);
  else
    f.open(filename,"r");

  eshortseq seq;
  int i;

  estrarray args;
  while (f.readln(line)){
    if (line.len()==0 || line[0]=='#') continue;

    args=line.explode(" ");
    if (args.size()==2){
      seq.name=args[0];
      seq.seq=args[1].uppercase();
      seq.compress();
    }else if (args.size()==4){
      seq.name=args[0];
      seq.seq=args[3].uppercase();
      seq.compress();
    }else
      ldie("error parsing line: "+line);
    arr.add(seq);
  }
  cout << "# seqs: " << arr.size() << endl;
}




void load_seqs_compressed(const estr& filename,earray<estr>& arr,int& seqlen)
{
  estr line;
  estr name;
  estr seq;
  efile f(filename);

  int i;

  bool fasta=false;
  bool firstline=true;
  seqlen=0;
  int lineno=0;
  while (f.readln(line)){
    ++lineno;
    if (line.len()==0 || line[0]=='#') continue;
    
    if (firstline) { firstline=false; if (line[0]=='>') { fasta=true; cout << "# loading fasta formatted sequences file" << endl; } else { cout << "# loading stockholm formatted sequences file" << endl;} }
    if (fasta){ // fasta format
      if (line[0]=='>'){
        if (name.len()){
          ldieif(seqlen>0 && seqlen!=seq.len(),"sequence length mismatch. previous lengths: "+estr(seqlen)+" new length: "+estr(seq.len())+" on line: "+estr(lineno));
          seqlen=seq.len();
          arr.add(seq_compress(seq.uppercase()));
        }
        i=line.find(" ");
        if (i==-1) name=line.substr(1);
        else name=line.substr(1,i-1);
        seq.clear();
      }else{
        line.trim();
        seq+=line;
      }
    }else{ // stockholm non-interleaved format
      i=line.find(" ");
      ldieif(i==-1,"space separator not found on line: "+estr(lineno));
      name=line.substr(0,i);
      line.del(0,i);
      line.trim();
      ldieif(seqlen>0 && seqlen!=line.len(),"sequence length mismatch. previous lengths: "+estr(seqlen)+" new length: "+estr(line.len())+" on line: "+estr(lineno));
      seqlen=line.len();
      arr.add(seq_compress(line.uppercase()));
    }
  }

  if (fasta){
    if (name.len()){
      ldieif(seqlen>0 && seqlen!=seq.len(),"sequence length mismatch. previous lengths: "+estr(seqlen)+" new length: "+estr(seq.len())+" on line: "+estr(lineno));
      seqlen=seq.len();
      arr.add(seq_compress(seq.uppercase()));
    }
  }

  cout << "# seqs: " << arr.size() << " seqlen: "<< seqlen<< endl;
}

void load_seqs_compressed(const estr& filename,estrarray& arr,int& seqlen)
{
  estr line;
  estr name;
  estr seq;
  efile f(filename);

  int i;
  bool fasta=false;
  bool firstline=true;
  seqlen=0;
  int lineno=0;

  while (f.readln(line)){
    ++lineno;
    if (line.len()==0 || line[0]=='#') continue;
    
    if (firstline) { firstline=false; if (line[0]=='>') { fasta=true; cout << "# loading fasta formatted sequences file" << endl; } else { cout << "# loading stockholm formatted sequences file" << endl;} }
    if (fasta){ // fasta format
      if (line[0]=='>'){
        if (name.len()){
          ldieif(seqlen>0 && seqlen!=seq.len(),"sequence length mismatch. previous lengths: "+estr(seqlen)+" new length: "+estr(seq.len())+" on line: "+estr(lineno));
          seqlen=seq.len();
          arr.add(name,seq_compress(seq.uppercase()));
        }
        i=line.find(" ");
        if (i==-1) name=line.substr(1);
        else name=line.substr(1,i-1);
        seq.clear();
      }else{
        line.trim();
        seq+=line;
      }
    }else{ // stockholm non-interleaved format
      i=line.find(" ");
      ldieif(i==-1,"space separator not found on line: "+estr(lineno));
      name=line.substr(0,i);
      line.del(0,i);
      line.trim();
      ldieif(seqlen>0 && seqlen!=line.len(),"sequence length mismatch. previous lengths: "+estr(seqlen)+" new length: "+estr(line.len())+" on line: "+estr(lineno));
      seqlen=line.len();
      arr.add(name,seq_compress(line.uppercase()));
    }
  }
  if (fasta){
    if (name.len()){
      ldieif(seqlen>0 && seqlen!=seq.len(),"sequence length mismatch. previous lengths: "+estr(seqlen)+" new length: "+estr(seq.len())+" on line: "+estr(lineno));
      seqlen=seq.len();
      arr.add(seq_compress(seq.uppercase()));
    }
  }
  cout << "# seqs: " << arr.size() << " seqlen: "<< seqlen<< endl;
}

void load_seqs_compressed(const estr& filename,estrarray& arr,estrhashof<int>& arrind,int& seqlen)
{
  estr line;
  estr name;
  estrarray tmps;
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
    arrind.add(name,arrind.size());
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
	    
    ldieif(!arrind.exists(line),"something wrong: "+line);

    otus[i].add(arrind[line]);
  }
  cout << "# otus: " << otus.size() << endl;
}

void load_clusters(const estr& filename,estrhashof<int>& arrind,eintarray& otuarr,int& otucount)
{
  estr line;
  estr name;
  estrhashof<int> otus;
  efile f(filename);

  int i;

  for (i=0; i<arrind.size(); ++i)
    otuarr.add(-1);

  eregexp space(" +");

  otucount=0;
  estrarray arr;
  estr otu,acc;
  while (f.readln(line)){
    if (line.len()==0 || line[0]=='#') continue;
    arr=re_explode(line,space);

    ldieif(arr.size()<2,"no \"\\t\" found in line: "+line);
    otu=arr[0];
    acc=arr[1];

    arr=acc.explode(":");
    ldieif(arr.size()<2,"no \":\" found in sequence name: "+acc);
    acc=arr[0];

//    ldieif(!arrind.exists(acc),acc+" was not found in arrind");
    if (!otus.exists(otu)){
      otus.add(otu,otus.size());
      ++otucount;
    }
    if (!arrind.exists(acc)){
      arrind.add(acc,arrind.size());
      otuarr.add(0);
    }

    otuarr[arrind[acc]]=otus[otu];
  }
  cout << "# otucount: " << otucount << endl;
}

void load_samples(const estr& filename,estrhashof<int>& arrind,estrhashof<eintarray>& samples,estrhash& accsample)
{
  efile f2(filename);
  estr line;
  estr name;
  int i;

  f2.readln(line);
  i=line.find("--");
  while (i!=-1){
    name=line.substr(i+3);
    samples.add(name,eintarray());
    do {
      if (!f2.readln(line)) { i=-1; break; }
      i=line.find("--");
      if (i==-1 && arrind.exists(line)){ samples[name].add(arrind[line]); accsample.add(line,name); }
    } while(i==-1);
  }  
  cout << "# samples: " << samples.size() << endl;
}

/*
inline void getXY2(int l,int w,int& x,int& y)
{
  x=(l/w);
  y=(l%w);
  if (y<w-x){ y+=x; return; }
  x=w-x;
}
*/

