#include <eutils/emain.h>
#include <eutils/efile.h>
#include <eutils/eheap.h>
#include <eutils/ernd.h>
#include <eutils/eregexp.h>

#include "cluster-common.h"
#include "seq-profile.h"

#include "../sali/etree.h"

#include <stack>

earray<ebasicarray<float> > profile;

estr ofile;


void generateChimeras(estrarray& chimeras,estrarray& seqs,float percentage)
{
  int i,j,l,cutpos;

  estr chimseq;
  for (i=0; i<seqs.size()*percentage; ++i){
    chimseq.clear();
    j=rnd.uniform()*seqs.size();
    do l=rnd.uniform()*seqs.size(); while (l==j);
    cutpos=rnd.uniform()*seqs[j].len();
    chimseq=seqs[j].substr(0,cutpos);
    chimseq+=seqs[l].substr(cutpos);
    chimeras.add(estr("CHIMERA_")+i+"|cutpos="+cutpos+seqs.keys(j)+"|"+seqs.keys(l),chimseq);
  }
}



void makeSequencesWithTree(const estr& initseq,etree& tree,double mutrate,estrarray& seqs)
{
  double mutscale=10.0/mutrate/2.0;
  etreenode* tmpnode;
  stack<etreenode*> tnstack;
  stack<int>        tnstackChild;
  stack<estr>       tnstackSeq;

  estr tmpseq;

  tnstack.push(&tree.root);
  tnstackChild.push(0);
  tnstackSeq.push(initseq);
  
  while (tnstack.size()>0){
    if (tnstackChild.top()<tnstack.top()->childs.size()){
      tmpseq=tnstackSeq.top();
      tmpnode=tnstack.top()->childs[tnstackChild.top()];
      ++tnstackChild.top();
      if (tmpnode->childs.size()>0){
        tnstack.push(tmpnode);
        tnstackChild.push(0);
        profileMutateTimes(tmpseq,profile,mutrate,tmpnode->blength*mutscale);
        tnstackSeq.push(tmpseq);
      }else{
        profileMutateTimes(tmpseq,profile,mutrate,tmpnode->blength*mutscale);
        seqs.add(tmpnode->id,tmpseq);
      }
    }else{
      tnstack.pop();
      tnstackChild.pop();
      tnstackSeq.pop();
    }
  }
}


/*
  cout << "# number of taxonomic nodes: " << branches.size() << endl;
  list<ebranch*>::iterator it;
  int count=0;
  for (it=branches.begin(); it!=branches.end(); ++it){
    (*it)->id=count++;
  }

  ebasicarray<ebranch*> stack;
  ebranch *b2;
  eintarray stacki;
  earray<estr> stacknewick;
  earray<estr> stacktax;
  earray<estr> stackseq;
  stack.add(rbranch);
  stacki.add(0);
  stacknewick.add(estr());
  stackseq.add(initseq);
  stacktax.add("0");
  estr tmpseq;

  long int nextmut=rnd.uniform()*rnd.exponential(1.0/mutrate);
  while (stack.size()){
    i=stack.size()-1;
    b=stack[i];
//    cout << b->id <<":" <<stacki[i] << " " << b->extant << " " <<  b->branches.size() << endl;
    if (stacki[i]<b->branches.size()){
      b2=b->branches[stacki[i]];
      tmpseq=stackseq[i];
      if (b2->branches.size()) {
//        mutateSeqLength(tmpseq,mutrate,b2->xtime-b2->ntime);
        profileMutateTimes(tmpseq,profile,mutrate,nextmut,b2->xtime-b2->ntime);
        stackseq.add(tmpseq);
        stack.add(b2);
        stacki.add(0);
        stacknewick.add(estr());
        stacktax.add(stacktax[i]+";"+b2->id);
        cout << "# " << stacktax[i]+";"+b2->id << " " << tmpseq << endl;
      } else {
        ldieif(!b2->extant,"branch: "+estr(b2->id)+" is not extant: "+b2->branches.size());
        if (stacknewick[i].size())
          stacknewick[i]+=",";
        stacknewick[i]+=estr(b2->id)+": "+(btime-b2->ntime);
//        mutateSeqLength(tmpseq,mutrate,btime-b2->ntime);
        profileMutateTimes(tmpseq,profile,mutrate,nextmut,btime-b2->ntime);
        seqs.add(estr(b2->id)+"|"+stacktax[i]+";"+b2->id,tmpseq);
      }
      ++stacki[i];
    }else{
      // pop stack
      if (stack.size()==1) break;

//      cout << (b->parent!=0x00?b->parent->id:0) << " <- " << b->id << ": " << (b->xtime-b->ntime) << " " << b->ntime << " " << b->xtime << " " << b->branches.size() << endl;
      if (stacknewick[i-1].len())
        stacknewick[i-1]+=",";
      stacknewick[i-1]+="("+stacknewick[i]+")"+estr(b->id)+": "+(b->xtime-b->ntime);

      stackseq.erase(i);
      stack.erase(i);
      stacki.erase(i);
      stacknewick.erase(i);
      stacktax.erase(i);
    }
  }

  cout << "# (" << stacknewick[0] << ")" << endl;
}
*/

int emain()
{
  double mutrate=1.0e-9;

  epregister(ofile);
  epregister(mutrate);
  eparseArgs(argvc,argv);
  ldieif(argvc<3,"syntax: "+efile(argv[0]).basename()+" <file.profile> <file.tree>");


  estr initseq;

  profileLoad(profile,argv[1]);
  profileInitSeq(initseq,profile);

  etree tree;
  tree.load(argv[2]);

  estrarray mcaseqs;
  makeSequencesWithTree(initseq,tree,mutrate,mcaseqs);
  
  int i;
  
  for (i=0; i<mcaseqs.size(); ++i)
    printf("%-30s %s\n",mcaseqs.keys(i)._str,mcaseqs[i]._str);


/*
  estrarray chimeras;
  generateChimeras(chimeras,seqs,0.05);

  for (i=0; i<chimeras.size(); ++i)
      printf("%-30s %s\n",chimeras.keys(i)._str,chimeras.values(i)._str);
*/

  return(0);
}

