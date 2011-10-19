#include <eutils/emain.h>
#include <eutils/efile.h>
#include <eutils/eheap.h>
#include <eutils/ernd.h>
#include <eutils/eregexp.h>

earray<eintarray> profile;

eregexp re_wspace("[ \t]+");

void loadProfile(const estr& filename)
{
  efile f(filename);

  ldieif(!f.exists(),"file not found: "+filename);

  int i;
  estr line;
  while (f.readln(line)){
    if (line.len()==0 || line[0]=='#') continue;
    estrarray arr(re_explode(line,re_wspace));
    ldieif(arr.size()<9,"not enough elements in line: "+line);
    profile.add(eintarray());
    ldieif(arr[0].i()!=(int)(profile.size()-1),"wrong id on line: "+line);
    profile[profile.size()-1].init(7);
    for (i=0; i<7; ++i)
      profile[profile.size()-1][i]=arr[i+2].f();
  }
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
  seqs[g][i]=0;
  for (i=1; i<seqs[g].size(); ++i)
    seqs[g][i]=-1;
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
  int id;
  int pid;
  int dtime;
  int btime;
  int ntime;
  int xtime;
  eintarray branches;
  bool extant;
  multiset<ebranch*,cmp_btime>::iterator bit;
  multiset<ebranch*,cmp_dtime>::iterator dit;
  ebranch(int _id,int _pid,int _ntime,int _btime,int _dtime): id(_id),pid(_pid),ntime(_ntime),btime(_btime),dtime(_dtime),extant(true),xtime(-1) {}
  void add(multiset<ebranch*,cmp_btime>& bset,multiset<ebranch*,cmp_dtime>& dset) { bit=bset.insert(this); dit=dset.insert(this); }
  void remove(multiset<ebranch*,cmp_btime>& bset,multiset<ebranch*,cmp_dtime>& dset) { bset.erase(bit); dset.erase(dit); }
};

bool cmp_dtime::operator()(const ebranch* left,const ebranch* right){ return(left->dtime < right->dtime); }
bool cmp_btime::operator()(const ebranch* left,const ebranch* right){ return(left->btime < right->btime); }

void makeRandomPhylogeneticTree()
{
  const double brate=0.001;
  const double drate=0.00001;

  multiset<ebranch*,cmp_btime> birth;
  multiset<ebranch*,cmp_dtime> death;
  earray<ebranch> branches;

  int sidcount=0;
  int btime=0;
  int dtime=0;
  int cspecies=1;
  ebranch *rbranch = new ebranch(sidcount++,-1,btime,btime+rnd.exponential(1.0/brate),dtime+rnd.exponential(1.0/drate));
  rbranch->add(birth,death);
  branches.addref(rbranch);
  int j;

  
  while (btime<100000 && cspecies>0){
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
      cout << btime << " birth: " << b->id << " " <<nb1->id << " " << nb2->id << " cspecies: " << cspecies << endl;
    }else{
      btime += (d->dtime-dtime)/cspecies;
      dtime = d->dtime;
      --cspecies;
      d->remove(birth,death);
      d->extant=false;
      d->xtime=btime;
      cout << btime << " death: " << d->id << " pid: " << d->pid << " cspecies: " << cspecies << endl;
      while (d->pid!=-1 && d->branches.size()==0 && !d->extant) {
        j=branches[d->pid].branches.find(d->id);
        lassert(j==-1);
        branches[ d->pid ].branches.erase( j );
        d=&branches[ d->pid ];
      }
//      delete d;
    }
    // else just remove this species from list
  }

  int i,tmpid,tmpid2;
  for (i=1; i<branches.size(); ++i){
    if (branches[i].branches.size()==1){
      tmpid=branches[i].branches[0];
      tmpid2=branches[i].pid;
      branches[i].branches.clear();
      branches[tmpid].ntime=branches[i].ntime;
      branches[tmpid].pid=branches[i].pid;

      // update child id and xtime for parent
      if (tmpid2>=0){
        j=branches[tmpid2].branches.find(i);
        branches[tmpid2].branches[j]=tmpid;
        branches[tmpid2].xtime=branches[i].xtime;
      }
    }
  }

  eintarray stack;
  eintarray stacki;
  earray<estr> stackstr;
  stack.add(0);
  stacki.add(0);
  stackstr.add(estr());
  while (stack.size()){
    i=stack.size()-1;
//    cout << stack[i]<<":" <<stacki[i] << " " << branches[stack[i]].branches.size() << endl;
    if (stacki[i]<branches[stack[i]].branches.size()){
      tmpid=branches[stack[i]].branches[stacki[i]];
      if (branches[tmpid].branches.size()) {
        stack.add(tmpid);
        stacki.add(0);
        stackstr.add(estr());
      } else {
        lassert(!branches[tmpid].extant);
        if (stackstr[i].size())
          stackstr[i]+=",";
        stackstr[i]+=estr(tmpid)+": "+(btime-branches[tmpid].ntime);
      }
      ++stacki[i];
    }else{
      // pop stack
      if (stack.size()==1) break;

      cout << branches[stack[i]].pid << " <- " << stack[i] << ": " << (branches[stack[i]].xtime-branches[stack[i]].ntime) << endl;
      if (stackstr[i-1].len())
        stackstr[i-1]+=",";
      stackstr[i-1]+="("+stackstr[i]+")";

      stack.erase(i);
      stacki.erase(i);
      stackstr.erase(i);
    }
  }

  cout << stackstr[0] << endl;
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
  eparseArgs(argvc,argv);
  ldieif(argvc<2,"syntax: "+efile(argv[0]).basename()+" <file>");

  loadProfile(argv[1]);

  earray<eintarray> seqstree,seqstree2;
  generateCoalescenceTree(seqstree,10);
  generateCoalescenceTree(seqstree2,10);

  int g,i;
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

  estr p1,p2;
  p1="AAAAAAAAAAAAAAAAAAA";
  p2=p1;

  float mutrate=0.05; // per base per generation
  int divergence=10;
  int nextmut;

  nextmut=rnd.exponential(1.0/mutrate);
  for (i=0; i<divergence; ++i){
    while (nextmut<p1.len()) {
      mutateseq(p1,nextmut);
      nextmut+=rnd.exponential(1.0/mutrate);
    }
  }
  nextmut=rnd.exponential(1.0/mutrate);
  for (i=0; i<divergence; ++i){
    while (nextmut<p2.len()) {
      mutateseq(p2,nextmut);
      nextmut+=rnd.exponential(1.0/mutrate);
    }
  }
  
  earray<estr> seqstr,tmpseqstr;
  seqstr.init(10);

  seqstr[0]=p1;
  nextmut=rnd.exponential(1.0/mutrate);
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

  cout << "A1: "<<p1<<endl;
  for (i=0; i<seqstr.size(); ++i)
    cout << "    " << seqstr[i] << endl;

  cout << endl;

  seqstr[0]=p2;
  nextmut=rnd.exponential(1.0/mutrate);
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

  cout << "A2: "<<p2<<endl;
  for (i=0; i<seqstr.size(); ++i)
    cout << "    " << seqstr[i] << endl;


  makeRandomPhylogeneticTree();

  return(0);
}

