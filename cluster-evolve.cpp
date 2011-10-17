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

  return(0);
}

