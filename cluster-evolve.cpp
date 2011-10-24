#include <eutils/emain.h>
#include <eutils/efile.h>
#include <eutils/eheap.h>
#include <eutils/ernd.h>
#include <eutils/eregexp.h>

#include "cluster-common.h"

earray<ebasicarray<float> > profile;

eregexp re_wspace("[ \t]+");

void profileLoad(const estr& filename)
{
  efile f(filename);

  ldieif(!f.exists(),"file not found: "+filename);

  int i;
  estr line;
  while (f.readln(line)){
    if (line.len()==0 || line[0]=='#') continue;
    estrarray arr(re_explode(line,re_wspace));
    ldieif(arr.size()<10,"not enough elements in line: "+line);
    profile.add(ebasicarray<float>());
    ldieif(arr[0].i()!=(int)(profile.size()-1),"wrong id on line: "+line);
    profile[profile.size()-1].init(6);
    for (i=0; i<6; ++i)
      profile[profile.size()-1][i]=arr[i+3].f();
  }
}

char getMutation(ebasicarray<float>& bpp)
{
  float r=rnd.uniform()*(bpp[0]+bpp[1]+bpp[2]+bpp[3]);
  r-=bpp[0];
  if (r<0.0) return('A');
  r-=bpp[1];
  if (r<0.0) return('T');
  r-=bpp[2];
  if (r<0.0) return('G');
  return('C');
}

void profileInitSeq(estr& seq,earray<ebasicarray<float> >& profile)
{
  int i;
  seq.clear();
  for (i=0; i<profile.size(); ++i)
    seq+=getMutation(profile[i]);
}

void profileMutate(estr& seq,earray<ebasicarray<float> >& profile,float mutrate,int& mutpos)
{
  while (mutpos<seq.len()){
    seq[mutpos]=getMutation(profile[mutpos]);
    mutpos+=rnd.exponential(1.0/mutrate);
  }
  mutpos-=seq.len();
}

void mutateseq(estr& seqstr,int p)
{
  double f=rnd.uniform();
  if (f<0.25)
    seqstr[p]='A';
  else if (f<0.50)
    seqstr[p]='T';
  else if (f<0.75)
    seqstr[p]='G';
  else
    seqstr[p]='C';
}

void generateCoalescenceTree(earray<eintarray>& seqs,int popsize)
{
  int i,j,g,npopsize;
  g=0;
  npopsize=popsize;
  seqs.add(eintarray());
  seqs[0].init(popsize,1);

  eintarray ind;
  ind.init(popsize,-1);
  int indcount;

  for (g=0; npopsize>1; ++g){
    seqs.add(eintarray());
    seqs[g+1].init(seqs[0].size(),0);
    npopsize=0;
    indcount=0;
    for (i=0; i<ind.size(); ++i)
      ind[i]=-1;
    for (i=0; i<seqs[g].size(); ++i){
      if (seqs[g][i]==0){ seqs[g][i]=-1; continue; }
      j=rnd.uniform()*popsize;
      if (ind[j]==-1) ind[j]=indcount++;
      j=ind[j];

      seqs[g][i]=j;
      if (seqs[g+1][j]==0)
        ++npopsize;
      ++seqs[g+1][j];
    }
  }
  seqs[g][0]=0;
  for (i=1; i<seqs[g].size(); ++i)
    seqs[g][i]=-1;
}

void generateSequencesWithCTree(earray<eintarray>& seqstree,const estr& initseq,double mutrate,int popsize,earray<estr>& seqstr)
{
  int nextmut,g,i;
  earray<estr> tmpseqstr;
  seqstr.clear();
  seqstr.init(popsize);

  seqstr[0]=initseq;
  nextmut=rnd.uniform()*rnd.exponential(1.0/mutrate);
  for (g=seqstree.size()-2; g>=0; --g){
    tmpseqstr=seqstr;
    for (i=0; i<seqstree[g].size(); ++i){
      if (seqstree[g][i]==-1) break;
      seqstr[i]=tmpseqstr[ seqstree[g][i] ];
      while (nextmut<seqstr[i].len()) {
        mutateseq(seqstr[i],nextmut);
        nextmut+=rnd.exponential(1.0/mutrate);
      }
      nextmut-=seqstr[i].len();
    }
  }
}

/*
void generateRandomSequences(const estr& initseq,double mutrate,int popsize,earray<estr>& seqstr)
{
  int nextmut,i;
  estr tmpstr;
  seqstr.clear();

  nextmut=rnd.exponential(1.0/mutrate);
  for (i=0; i<popsize; ++i){
    tmpstr=initseq;
    while (nextmut<tmpstr.len()) {
      mutateseq(tmpstr,nextmut);
      nextmut+=rnd.exponential(1.0/mutrate);
    }
    nextmut-=tmpstr.len();
    seqstr.add(tmpstr);
  }
}
*/

void mutateSeqs(earray<estr>& seqstr,double mutrate)
{
  int nextmut,i;

  nextmut=rnd.uniform()*rnd.exponential(1.0/mutrate);
  for (i=0; i<seqstr.size(); ++i){
    while (nextmut<seqstr[i].len()) {
      mutateseq(seqstr[i],nextmut);
      nextmut+=rnd.exponential(1.0/mutrate);
    }
    nextmut-=seqstr[i].len();
  }
}

void mutateSeqLength(estr& seqstr,double mutrate,int length)
{
  long int nextmut,i;

  nextmut=rnd.uniform()*rnd.exponential(1.0/mutrate); // this ensures that the first mutation is randomly distributed
  for (i=0; i<length; ++i){
    while (nextmut<seqstr.len()) {
      mutateseq(seqstr,nextmut);
      nextmut+=rnd.exponential(1.0/mutrate);
    }
    nextmut-=seqstr.len();
  }
}




#include <set>

class ebranch;

class cmp_dtime
{
 public:
  bool operator()(const ebranch* left,const ebranch* right);
};
class cmp_btime
{
 public:
  bool operator()(const ebranch* left,const ebranch* right);
};

class ebranch
{
 public:
  long int id;
  long int pid;
  long int dtime;
  long int btime;
  long int ntime;
  long int xtime;
  eintarray branches;
  bool extant;
  multiset<ebranch*,cmp_btime>::iterator bit;
  multiset<ebranch*,cmp_dtime>::iterator dit;
  ebranch(long int _id,long int _pid,long int _ntime,long int _btime,long int _dtime): id(_id),pid(_pid),ntime(_ntime),btime(_btime),dtime(_dtime),extant(true),xtime(-1) {}
  void add(multiset<ebranch*,cmp_btime>& bset,multiset<ebranch*,cmp_dtime>& dset) { bit=bset.insert(this); dit=dset.insert(this); }
  void remove(multiset<ebranch*,cmp_btime>& bset,multiset<ebranch*,cmp_dtime>& dset) { bset.erase(bit); dset.erase(dit); }
};

bool cmp_dtime::operator()(const ebranch* left,const ebranch* right){ return(left->dtime < right->dtime); }
bool cmp_btime::operator()(const ebranch* left,const ebranch* right){ return(left->btime < right->btime); }

ostream& operator<<(ostream& stream,const ebranch& b)
{
  stream << "[id: " << b.id << " pid: " << b.pid << " ntime: " << b.ntime << " xtime: "<< b.xtime << " extant: " << b.extant << " branches: " << b.branches <<"]";
  return(stream);
}

void makeRandomPhylogeneticTree(const estr& initseq,earray<estr>& seqs,double mutrate,int popsize)
{
  const double brate=1.0e-3;  // speciation every 1000 generations
  const double drate=brate/popsize;

  multiset<ebranch*,cmp_btime> birth;
  multiset<ebranch*,cmp_dtime> death;
  earray<ebranch> branches;

  long int sidcount=0;
  long int btime=0;
  long int dtime=0;
  long int cspecies=1;
  ebranch *rbranch = new ebranch(sidcount++,-1,btime,btime+rnd.exponential(1.0/brate),dtime+rnd.exponential(1.0/drate));
  rbranch->add(birth,death);
  branches.addref(rbranch);
  int j;

  
  while (btime<10000000 && cspecies>0){
    ebranch *b,*d;
    b=(*birth.begin());
    d=(*death.begin());
    lassert(!b->extant || !d->extant);
    if ( b->btime-btime < (d->dtime-dtime)/cspecies ){
      b->remove(birth,death);

      dtime += (b->btime-btime)*cspecies;
      btime=b->btime;
      b->extant=false;
      b->xtime=btime;
//      b->btime=btime+rnd.exponential(1.0/brate);
//      b->add(birth,death);

      b->branches.add(sidcount);
      ebranch *nb1=new ebranch(sidcount++,b->id,btime,btime+rnd.exponential(1.0/brate),b->dtime);
      nb1->add(birth,death);
      branches.addref(nb1);

      b->branches.add(sidcount);
      ebranch *nb2=new ebranch(sidcount++,b->id,btime,btime+rnd.exponential(1.0/brate),dtime+rnd.exponential(1.0/drate));
      nb2->add(birth,death);
      branches.addref(nb2);

      ++cspecies;
//      cout << btime << " birth: " << b->id << " " <<nb1->id << " " << nb2->id << " cspecies: " << cspecies << endl;
    }else{
      btime += (d->dtime-dtime)/cspecies;
      dtime = d->dtime;
      --cspecies;
      d->remove(birth,death);
      d->extant=false;
      d->xtime=btime;
//      cout << btime << " death: " << d->id << " pid: " << d->pid << " cspecies: " << cspecies << endl;
      while (d->pid!=-1 && d->branches.size()==0) {
        j=branches[d->pid].branches.find(d->id);
        lassert(j==-1);
        branches[ d->pid ].branches.erase( j );
        d=&branches[ d->pid ];
      }
//      delete d;
    }
    // else just remove this species from list
  }

  long int i,pid,pid2;

  for (i=1; i<branches.size(); ++i){
//    cout << branches[i] << endl;
    pid=branches[i].pid;
    if (pid==-1 || pid==0 || branches[i].branches.size()==0 && !branches[i].extant) continue;
    if (branches[pid].branches.size()==1){
      pid2=branches[pid].pid;
      branches[pid].branches.clear();
      branches[i].ntime=branches[pid].ntime;
      branches[i].pid=pid2;

      // update child id and xtime for parent
      if (pid2>=0){
//        cout << "pid2: " << branches[pid2] << endl;
        j=branches[pid2].branches.find(pid);
        ldieif(j<0,"not found: "+estr(pid)+" in parent: "+estr(pid2)+" i: "+i);
        branches[pid2].branches[j]=i;
        lassert(branches[pid2].branches[j]!=i);
      }
    }
  }

  eintarray stack;
  eintarray stacki;
  earray<estr> stackstr;
  earray<estr> stackseq;
  stack.add(0);
  stacki.add(0);
  stackstr.add(estr());
  stackseq.add(initseq);
  long int tmpid;
  estr tmpseq;
  while (stack.size()){
    i=stack.size()-1;
//    cout << stack[i]<<":" <<stacki[i] << " " << branches[stack[i]].branches.size() << endl;
    if (stacki[i]<branches[stack[i]].branches.size()){
      tmpid=branches[stack[i]].branches[stacki[i]];
      tmpseq=stackseq[i];
      if (branches[tmpid].branches.size()) {
        mutateSeqLength(tmpseq,mutrate,branches[tmpid].xtime-branches[tmpid].ntime);
        stackseq.add(tmpseq);
        stack.add(tmpid);
        stacki.add(0);
        stackstr.add(estr());
      } else {
        ldieif(!branches[tmpid].extant,"branch: "+estr(tmpid)+" is not extant: "+branches[tmpid].branches.size());
        if (stackstr[i].size())
          stackstr[i]+=",";
        stackstr[i]+=estr(tmpid)+": "+(btime-branches[tmpid].ntime);
        mutateSeqLength(tmpseq,mutrate,btime-branches[tmpid].ntime);
        seqs.add(tmpseq);
      }
      ++stacki[i];
    }else{
      // pop stack
      if (stack.size()==1) break;

//      cout << branches[stack[i]].pid << " <- " << stack[i] << ": " << (branches[stack[i]].xtime-branches[stack[i]].ntime) << " " << branches[stack[i]].ntime << " " << branches[stack[i]].xtime << " " << branches[stack[i]].branches.size() << endl;
      if (stackstr[i-1].len())
        stackstr[i-1]+=",";
      stackstr[i-1]+="("+stackstr[i]+")"+estr(i)+": "+(branches[stack[i]].xtime-branches[stack[i]].ntime);

      stackseq.erase(i);
      stack.erase(i);
      stacki.erase(i);
      stackstr.erase(i);
    }
  }

  cout << "# (" << stackstr[0] << ")" << endl;
/*
  for (i=branches.size()-1; i>0; --i){
    if (branches[i].extant)
      cout << "(" << i << ":" << (btime-branches[i].ntime) << ": " << branches[i].pid << " ("<<branches[branches[i].pid].branches.size()<<") <- " << branches[i].id<< " ("<<branches[i].branches.size()<<")"<< endl;
    else if (branches[i].branches.size()>0)
      cout << branches[i].ntime << ": " << branches[i].pid << " ("<<branches[branches[i].pid].branches.size()<<") <- " << branches[i].id<< " ("<<branches[i].branches.size()<<")"<< endl;
  }
*/
}



int emain()
{
  double mutrate=1.0e-9;
  int popsize=100;
  epregister(mutrate);
  epregister(popsize);
  eparseArgs(argvc,argv);
  ldieif(argvc<2,"syntax: "+efile(argv[0]).basename()+" <file>");


  int g,i,j;
  estr initseq;
  for (i=0; i<900; ++i)
    initseq+='A';

  earray<estr> seqs,popseqs;
  makeRandomPhylogeneticTree(initseq,seqs,mutrate,popsize);

  estr info;
  earray<eintarray> ctree;
  for (i=0; i<seqs.size(); ++i){
    ctree.clear(); popseqs.clear();
    generateCoalescenceTree(ctree,100);
    generateSequencesWithCTree(ctree,seqs[i],mutrate,100,popseqs);
    for (j=0; j<popseqs.size(); ++j){
      info=estr(i)+":"+estr(j);
      printf("%-20s %s\n",info._str,seqs[i]._str);
    }
  }

  exit(0);

/*
  earray<eintarray> ctree;
  generateCoalescenceTree(ctree,10000);
  generateSequencesWithCTree(ctree,seq1,0.01,10000,seqs);

  for (i=0; i<seqs.size(); ++i)
    cout << i << " " << seqs[i] << endl;
  exit(0);
*/

/*
  seqs.init(10000,seq1);
  for (i=0; i<6; ++i)
    mutateSeqs(seqs,0.01);

  for (i=0; i<seqs.size(); ++i)
    cout << i << " " << seqs[i] << endl;
  exit(0);
*/


/*
  profileLoad(argv[1]);
  profileInitSeq(seq1,profile);
  seq2=seq1;

  int mutpos1,mutpos2;
  mutpos1=rnd.exponential(1.0/mutrate);
  mutpos2=rnd.exponential(1.0/mutrate);
  for (i=0; i<10000; ++i){
    profileMutate(seq1,profile,mutrate,mutpos1);
    profileMutate(seq2,profile,mutrate,mutpos2);
    cout << i << " " << dist(seq1,seq2) << endl;
//    cout << seq1 << endl;
//    cout << seq2 << endl;
  }

  exit(0);
*/    

/*
  earray<eintarray> seqstree,seqstree2;
  generateCoalescenceTree(seqstree,10);
  generateCoalescenceTree(seqstree2,10);

  for (g=0; g<seqstree.size(); ++g){
    for (i=0; i<seqstree[g].size(); ++i){
      if (seqstree[g][i]==-1) { printf("%4.1s ","."); continue; }
      printf("%4.1i ",seqstree[g][i]);
    }
    printf("\n");
  }
  printf("\n");
  for (g=0; g<seqstree2.size(); ++g){
    for (i=0; i<seqstree2[g].size(); ++i){
      if (seqstree2[g][i]==-1) { printf("%4.1s ","."); continue; }
      printf("%4.1i ",seqstree2[g][i]);
    }
    printf("\n");
  }
  exit(0);
*/

/*
  estr p1,p2;
  p1="AAAAAAAAAAAAAAAAAAA";
  p2=p1;

  mutrate=0.05; // per base per generation
  int divergence=10;
  int nextmut;

  nextmut=rnd.uniform()*rnd.exponential(1.0/mutrate);
  for (i=0; i<divergence; ++i){
    while (nextmut<p1.len()) {
      mutateseq(p1,nextmut);
      nextmut+=rnd.exponential(1.0/mutrate);
    }
  }
  nextmut=rnd.uniform()*rnd.exponential(1.0/mutrate);
  for (i=0; i<divergence; ++i){
    while (nextmut<p2.len()) {
      mutateseq(p2,nextmut);
      nextmut+=rnd.exponential(1.0/mutrate);
    }
  }
*/

/*  
  earray<estr> seqstr,tmpseqstr;
  seqstr.init(10);


  seqstr[0]=p1;
  nextmut=rnd.uniform()*rnd.exponential(1.0/mutrate);
  for (g=seqstree.size()-2; g>=0; --g){
    tmpseqstr=seqstr;
    for (i=0; i<seqstree[g].size(); ++i){
      if (seqstree[g][i]==-1) break;
      seqstr[i]=tmpseqstr[ seqstree[g][i] ];
      while (nextmut<seqstr[i].len()) {
        mutateseq(seqstr[i],nextmut);
        nextmut+=rnd.exponential(1.0/mutrate);
      }
      nextmut-=seqstr[i].len();
    }
  }

  earray<estr> seqstr;
  generateSequencesWithCTree(seqstree,p1,mutrate,10,seqstr);
  cout << "A1: "<<p1<<endl;
  for (i=0; i<seqstr.size(); ++i)
    cout << "    " << seqstr[i] << endl;

  cout << endl;
*/


/*
  seqstr[0]=p2;
  nextmut=rnd.uniform()*rnd.exponential(1.0/mutrate);
  for (g=seqstree2.size()-2; g>=0; --g){
    tmpseqstr=seqstr;
    for (i=0; i<seqstree2[g].size(); ++i){
      if (seqstree2[g][i]==-1) break;
      seqstr[i]=tmpseqstr[ seqstree2[g][i] ];
      while (nextmut<seqstr[i].len()) {
        mutateseq(seqstr[i],nextmut);
        nextmut+=rnd.exponential(1.0/mutrate);
      }
      nextmut-=seqstr[i].len();
    }
  }

  generateSequencesWithCTree(seqstree2,p2,mutrate,10,seqstr);
  cout << "A2: "<<p2<<endl;
  for (i=0; i<seqstr.size(); ++i)
    cout << "    " << seqstr[i] << endl;
*/


  return(0);
}

