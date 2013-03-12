#include "eseq.h"

#include <eutils/eregexp.h>
#include <eutils/earrayof.h>
#include <eutils/ehashmap.h>


void loadSeqs(estrhashof<eseq>& seqs,const efile& f)
{
  estr line;
  estrarray parts;
//  f.open(argv[2],"r"); // seqfile
  while (f.readln(line)){
    if (line.len()==0 || line[0]=='#') continue;
    parts=re_explode(line,"[\t ]+");
    seqs.add(parts[0],eseq(parts[0]));
//    seqsind.add(parts[0]);
  }
  cout << "# loaded sequences: " << f.name << " (" << seqs.size() << ")" << endl;
}

void loadSeqsInfo(estrhashof<eseq>& seqs,const efile& f)
{
  eregexp re_strain("ATCC|IFO|NBRC|DSM|SIMO|CCBAU|GMBAE|CCUG|LMG|RMA|HTCC|KCTC|PCC|KMM|BCC|CIP|LPPA|NRRL|KACC|NRCM|YIM|BGSC|CCTCC|VKM|IMSNU|CSF|STM|ORS|CICC|NIBH|JCM|NCIMB|IAM|IFO|NRIC|MBIC|KOPRI|HBUM|RITF|NCTC|KLDS|IHB|BIHB");
  
  estr line;
  estrarray taxparts;
  estr acc,tax,isosrc,strain,organism;
//  f.open(argv[5],"r"); // infofile
  while (f.readln(line)){
    if (line.len()==0 || line[0]=='#') continue;

    if (line[0]=='>'){
      if (acc.len()>0 && seqs.exists(acc)){
        if (tax[tax.len()-1]=='.') tax.del(-1);
        taxparts=tax.explode(";");
        
        seqs[acc].organism=organism;
        seqs[acc].isosrc=isosrc;
        if (taxparts.size()>3 && tax.ifind("environmental")==-1 && re_strain.match(strain)>0 || acc.find("_")!=-1){
          seqs[acc].tax=tax;
          seqs[acc].strain=strain;
        }
      }
      acc=line.substr(1);
      strain.clear();
      tax.clear();
      isosrc.clear();
      organism.clear();
    }else if (line.substr(0,9)=="taxonomy=")
      tax=line.substr(line.find("=")+1);
    else if (line.substr(0,9)=="organism=")
      organism=line.substr(line.find("=")+1);
    else if (line.substr(0,17)=="isolation_source=")
      isosrc=line.substr(line.find("=")+1);
    else if (line.substr(0,7)=="strain=")
      strain=line.substr(line.find("=")+1);
  }
  cout << "# loaded info: " << f.name << endl;
}


