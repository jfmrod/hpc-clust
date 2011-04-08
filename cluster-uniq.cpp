#include <eutils/emain.h>
#include <eutils/efile.h>
#include <eutils/eheap.h>
#include <eutils/edcserver.h>
#include <eutils/esystem.h>

#include "cluster-common.h"


estrarray arr;


int emain()
{
  estr ofile="";
  estr dupfile="";
  epregister(ofile);
  epregister(dupfile);
  eparseArgs(argvc,argv);

  ldieif(argvc<2,"syntax: "+efile(argv[0]).basename()+" [--dupfile <file>] [--ofile <file>] <input_file>");

  load_seqs(argv[1],arr);

  if (!ofile.len()) ofile=estr(argv[1])+".uniq";
  efile f(ofile);
  int count=0;
  estrhash duphash;
  estrarray dups;
  int i;
  for (i=0; i<arr.size(); ++i){
    if (i%(arr.size()/10)==0)
      cout << i/(arr.size()/10);
    if (!duphash.exists(arr.values(i)))
      { f.write(arr.keys(i)+"   "+arr.values(i)+"\n"); duphash.add(arr.values(i),arr.keys(i)); ++count; }
    else 
      dups.add(arr.keys(i),dups[arr.values(i)]);
//    else
//      { /* cout << "# duplicate found: " << arr.keys(i) << " == " << dups[arr.values(i)] << endl; */ arr.erase(i); --i; }
  }
  cout << endl;
  cout << "# unique seqs: " << count << endl;

  if (!dupfile.len()) dupfile=estr(argv[1])+".dup";
  efile fdups(dupfile);
  for (i=0; i<dups.size(); ++i)
    fdups.write(dups.keys(i)+"="+dups.values(i)+"\n");
  cout << "# duplicates: " << dups.size() << endl;
 

  


  return(0);
}

