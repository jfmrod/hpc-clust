
#include "ekstract.h"
#include "eregexp.h"
#include "eiostream.h"

void ekstract::create_template(const estr &text,const estr &var)
{
  eregexp re;

  if (var.size())
    re.compile(var);
  else
    re.compile("\\{[_[:alnum:]]+\\}");

  eregexp re2;
  int preve;
  estr before,after;

  preve=-1;
  int i,j;
//  for (i=0; i<text.len() && i!=-1; i=re.e){
    re.match(text,i);
    if (re.b!=-1){
      cout << "Found var:" << text.substr(re.b,re.e-re.b)<<endl;

	    before = text.substr(re.b-5,5);
	    after = text.substr(re.e,5);
	
	    cout << "looking for: \"" << before << ".*"<<after<<"\""<<endl;
	    re2.compile(before+".*"+after);
	    
	    for (j=0; j<text.len() && j!=-1; j=re2.e){
        re2.match(text,re.e);
        if (re2.b!=-1)
          cout << text.substr(re.b,re.e-re.b)<<endl;
      }
	
//	    if (preve!=-1) cout << "in between: "<<text.substr(preve,re.b-preve) << endl;
      preve=re.e;
    }
//  }
//  if (preve!=-1) cout << "at end: "<<text.substr(preve) << endl;
}


