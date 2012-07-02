#include <eutils/emain.h>
#include <eutils/efile.h>
#include <eutils/eheap.h>
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
  ebasicstrhashof<estr> duphash;
  ebasicstrhashof<estr>::iter it;
  duphash.reserve(arr.size());
  estrarray dups;
  int i;
  for (i=0; i<arr.size(); ++i){
    estrarray id(arr.keys(i).explode("|"));
    if (i%(arr.size()/10)==0)
      cout << i/(arr.size()/10);
    it=duphash.get(arr.values(i));
    if (it==duphash.end()) //!duphash.exists(arr.values(i)))
      { f.write(id[0]+" "+arr.values(i)+"\n"); duphash.add(arr.values(i),id[0]); ++count; }
    else 
      dups.add(id[0],it.value());
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
 
  exit(0); // it is faster to call exit if we allocated a lot of data
  


  return(0);
}

