#include <eutils/emain.h>
#include <eutils/efile.h>
#include <eutils/eheap.h>

#include "cluster-common.h"


int seqcount=0;
earray<eintarray> profile;

void initProfile(int seqlen)
{
  int i,j;
  profile.reserve(seqlen);
  for (i=0; i<seqlen; ++i){
    profile.add(eintarray());
    profile[i].init(7);
    for (j=0; j<profile[i].size(); ++j)
      profile[i][j]=0;
  }
}

void load_seqs_profile(const estr& filename)
{
  int seqlen=-1;
  estr line;
  efile f(filename);

  int i,j;

  while (f.readln(line)){
    if (line.len()==0 || line[0]=='#') continue;
    
    i=line.find(" ");
    if (i==-1) continue;
    line.del(0,i);
    line.trim();
    if (seqlen==-1) { seqlen=line.size(); initProfile(seqlen); }
    else if (seqlen!=line.size()) ldie("some sequences have different size");
    line.uppercase();

    bool gapopen=false;
    unsigned char n;
    for (j=0; j<line.size(); ++j){
      n=nuc_compress(line[j]);
      if (n<=0x03u){
        ++profile[j][n];
        if (gapopen){
          gapopen=false;
          ++profile[j][0x06u];
        }
      }else if (n==0x0F){
        ++profile[j][0x04u]; 
        if (!gapopen){
          gapopen=true;
          ++profile[j][0x05u];
        }
      }
    }
    ++seqcount;
  }
  cout << "# seqs: " << seqcount << endl;
}

float getProfileConsensusGap(int i)
{
  int j;
  float cons=(float)profile[i][0];
  for (j=1; j<=4; ++j)
    if (cons<(float)profile[i][j]) cons=(float)profile[i][j];
  cons=(cons)/(profile[i][0]+profile[i][1]+profile[i][2]+profile[i][3]+profile[i][4]);
  cons=(cons-0.20)/0.80;
  return(cons);
}

float getProfileConsensus(int i)
{
  int j;
  float cons=(float)profile[i][0];
  for (j=1; j<4; ++j)
    if (cons<(float)profile[i][j]) cons=(float)profile[i][j];
  cons=(cons)/(profile[i][0]+profile[i][1]+profile[i][2]+profile[i][3]);
  cons=(cons-0.25)/0.75;
  return(cons);
}

float getProfileAverageConsensus(int i)
{
  int j;
  float avgcons=0.0;
  int count=0;
  for (j=max(i-25,0); j<MIN(i+25,profile.size()); ++j){
    avgcons+=getProfileConsensus(j);
    ++count;
  }
  return(avgcons/count);
}

int emain()
{
  eparseArgs(argvc,argv);
  ldieif(argvc<2,"syntax: "+efile(argv[0]).basename()+" <file>");

  load_seqs_profile(argv[1]);

  int i,j;
  float cons;
  float gapcons;
  float avgcons;
  for (i=0; i<profile.size(); ++i){
    cons=getProfileConsensus(i);
    gapcons=getProfileConsensusGap(i);
    avgcons=getProfileAverageConsensus(i);
    printf("%4.1i %6.3f %6.3f %6.3f %6.3f %6.3f %6.3f %6.3f %6.3f %6.3f %6.3f\n",i,cons,avgcons,gapcons,(float)profile[i][0]/seqcount,(float)profile[i][1]/seqcount,(float)profile[i][2]/seqcount,(float)profile[i][3]/seqcount,(float)profile[i][4]/seqcount,(float)profile[i][5]/seqcount,(float)profile[i][6]/seqcount);
  }

  return(0);
}

