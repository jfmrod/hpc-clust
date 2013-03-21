#include "eparser.h"

#include "evarcommon.h"
#include "evarclass.h"

#include "esocket.h"

#include "evarclassconverter.h"
#include "estrarray.h"

#include "estr.h"
#include "efile.h"
#include "edir.h"
#include "etimer.h"

#include "evarclassconstructor.h"
#include "evarclasscout.h"
#include "eparser.h"


#include "eregexp.h"

#include "efunc.h"

#include "ernd.h"
#include "eparserinterpreter.h"

#include "esystem.h"

estrarray varexec(const estr& cmd)
{
  estr line;
  efile f(popen(cmd));
  estrarray arr;
  while (f.readln(line))
    arr.add(line);
  return(arr); 
}


void varprint(const evar& var)
{
  cout << var << endl;
}
void vartype(const evar& var)
{
  cout << var.getClass() << endl;
}
void varaddress(const evar& var)
{
  if (var.var)
    cout << &var << " :: " << var.var << " ("<<var.var->pcount<<")"<<endl;
  else
    cout << &var << " :: " << var.var << endl;
}


bool doneRegister=false;

void epregisterFunctions()
{
  if (doneRegister) return;
  doneRegister=true;
  epregisterClass(eregexp);
  epregisterClassConstructor(eregexp,(const estr&));
  epregisterClassConverterCast(eregexp,estr);
  epregisterClassMethod(eregexp,compile);
  epregisterClassMethod2D(eregexp,match,int,(const estr&,int,int) const,evararray(0x00));
  epregisterClassMethod2D(eregexp,match,int,(const estr&,int,int&,int&,int) const,evararray(0x00));
//  epregisterClassProperty(eregexp,b);
//  epregisterClassProperty(eregexp,e);

  epregisterClass(eudpsocket);
//  epregisterClassMethodD(eudpsocket,listen,"0.0.0.0");
  epregisterClassMethodD(eudpsocket,listen,evararray("0.0.0.0"));
  epregisterClassMethod(eudpsocket,setReuseAddress);
  epregisterClassMethod(eudpsocket,close);
  epregisterClassMethod(eudpsocket,shutdown);
  epregisterClassMethod(eudpsocket,recvfrom);
  epregisterClassMethod(eudpsocket,sendto);
  epregisterClassProperty(eudpsocket,onReceive);

  epregisterClass(esocket);
  epregisterClassMethod(esocket,connect);
  epregisterClassMethod(esocket,close);
  epregisterClassMethod(esocket,shutdown);
  epregisterClassMethod2(esocket,recv,int,(estr&,int));
  epregisterClassMethod2(esocket,send,int,(const estr&));
  epregisterClassProperty(esocket,onReceive);

  epregisterClass(earray<esocket>);
  epregisterClassInheritance(earray<esocket>,ebasearray);



/*
  epregisterClass(eserver<esocket>);
  epregisterClassConstructor(eserver<esocket>,());
  epregisterClassMethodD(eserver<esocket>,listen,"0.0.0.0");
  epregisterClassMethod2(eserver<esocket>,accept,esocket*,());
  epregisterClassProperty(eserver<esocket>,onIncoming);
  epregisterClassProperty(eserver<esocket>,sockets);
*/

  epregisterClassMethod2(ebasearrayof,addvar,void,(evar&,evar&));
  epregisterClassMethod2(ebasearrayof,getvar,evar,(int));
  epregisterClassMethod2(ebasearrayof,getvarkey,evar,(int));
  epregisterClassMethod2A(ebasearrayof,addvar,"add",void,(evar&,evar&));
  epregisterClassMethod2A(ebasearrayof,getvarByKey,"[]",evar,(const evar&));
  epregisterClassMethod2A(ebasearrayof,getvar,"values",evar,(int));
  epregisterClassMethod2A(ebasearrayof,getvarkey,"keys",evar,(int));
  epregisterClassMethod2(ebasearrayof,size,int,());

  epregister2(getClasses(),"classes");
  epregisterClassProperty(eclassBase,properties);
  epregisterClassProperty(eclassBase,methods);
  epregisterClassProperty(eclassBase,parents);
 
  epregisterClassConstructor(estrarray,(const estr&));

  epregisterClassMethod2(estrarray,add,estr&,(const estr&));
  epregisterClassMethod2(estrarray,add,estr&,(const estr&,const estr&));
  epregisterClassMethod(estrarray,load);
  epregisterClassMethod(estrarray,save);
  epregisterClassMethod(estrarray,join);
  epregisterClassMethodD(estrarray,find,evararray(0,estrarray::equal));
  epregisterClassMethodD(estrarray,ifind,evararray(0));
  epregisterClassMethodD(estrarray,refind,evararray(0));
  epregisterClassMethodD(estrarray,findkey,evararray(0));
  epregisterClassMethodD(estrarray,ifindkey,evararray(0));
  epregisterClassMethodD(estrarray,refindkey,evararray(0));
  epregisterClassMethod(estrarray,ifindall);
  epregisterClassMethod(estrarray,ifindallkey);
  epregisterClassMethod(estrarray,refindall);
  epregisterClassMethod(estrarray,refindallkey);


  epregisterClassMethod2(estrarray,keys,const estr&,(size_t) const);
  epregisterClassMethod2(estrarray,values,estr&,(size_t));
  epregisterClassMethod4(estrarray,operator[],estr&,(size_t),"[]");
  epregisterClassMethod4(estrarray,operator[],estr&,(const estr&),"[]");

  epregisterClassMethod4(estrarray,operator[],estrarray,(const eintarray&) const,"[]");

  epregisterClassMethod4(estrarray,operator-=,estrarray&,(const estrarray&),"-=");
  epregisterClassMethod4(estrarray,operator-,estrarray,(const estrarray&),"-");
  epregisterClassMethod3<estrarray>((estr& (estrarray::*)(const estr&)) &estrarray::add,"+=");

  epregisterClassMethod2(ebasearray,addvar,void,(evar&));
  epregisterClassMethod2(ebasearray,getvar,evar,(size_t));
  epregisterClassMethod2(ebasearray,size,size_t,());
  epregisterClassMethod2A(ebasearray,addvar,"add",void,(evar&));
  epregisterClassMethod2A(ebasearray,getvar,"[]",evar,(size_t));

  epregisterClass(etimer);
  epregisterClassMethod(etimer,lap);
  epregisterClassMethod(etimer,check);
  epregisterClassMethod(etimer,reset);

  epregisterClassMethod2(evararray,add,evar&,(const evar&));
  epregisterClassMethod4(evararray,operator[],evar&,(size_t),"[]");

  epregisterClass(efile);
  epregisterClassCout(efile);
  epregisterClassConstructor(efile,(const estr&));
  epregisterClassConstructor(efile,(const estr&,const estr&));
  epregisterClassMethod2(efile,open,bool,() const);
  epregisterClassMethod2(efile,open,bool,(const estr&,const estr&));
  epregisterClassMethod(efile,fileno);
  epregisterClassMethod2D(efile,read,int,(estr&,long int) const,evararray(-1l));
  epregisterClassMethod2(efile,read,evar,() const);
  epregisterClassMethod(efile,readln);
  epregisterClassMethod2(efile,write,int,(const estr&) const);
  epregisterClassMethod2(efile,write,int,(const evar&) const);
  epregisterClassProperty(efile,name);
  epregisterClassMethod(efile,size);
  epregisterClassMethod(efile,exists);
  epregisterClassMethod(efile,basename);
  epregisterClassMethod(efile,close);

  epregisterClass(edir);
  epregisterClassCout(edir);
  epregisterClassConstructor(edir,(const estr&));
  epregisterClassProperty(edir,dirs);
  epregisterClassProperty(edir,files);

  epregisterClass(estrarrayof<efile>);
  epregisterClassInheritance(estrarrayof<efile>,ebasearrayof);
  epregisterClassMethod(estrarrayof<efile>,size);
  epregisterClassMethod4(estrarrayof<efile>,operator[],efile&,(const estr&),"[]");
  epregisterClassMethod4(estrarrayof<efile>,operator[],efile&,(size_t),"[]");

  epregisterClass(estrarrayof<edir>);
  epregisterClassInheritance(estrarrayof<edir>,ebasearrayof);
  epregisterClassMethod(estrarrayof<edir>,size);
  epregisterClassMethod2(estrarrayof<edir>,keys,estr&,(size_t));
  epregisterClassMethod2(estrarrayof<edir>,values,edir&,(size_t));
  epregisterClassMethod4(estrarrayof<edir>,operator[],edir&,(const estr&),"[]");
  epregisterClassMethod4(estrarrayof<edir>,operator[],edir&,(size_t),"[]");


  epregisterFuncD(ls,evararray("."));
  epregisterFuncD(cd,evararray("~"));
  epregisterFunc(pwd);
  epregisterFunc2(env,estrarray,());
  
  epregisterClassMethod(ernd,uniform);
#ifdef EUTILS_HAVE_LIBGSL
  epregisterClassMethod(ernd,gamma);
  epregisterClassMethod(ernd,gaussian);
  epregisterClassMethod(ernd,exponential);
  epregisterClassMethod(ernd,geometric);
#endif

  epregisterFunc2(grep,estr,(estr&,const eregexp&,int));
  epregisterFunc3(getParser(),epinterpret,"eval");

  epregisterFunc3(getParser(),varexec,"exec");
  epregisterFunc3(getParser(),varprint,"print");
  epregisterFunc3(getParser(),vartype,"type");
  epregisterFunc3(getParser(),varaddress,"addr");
  epregisterFunc2(serial,void,(const evar&,estr&));
  epregisterFunc2(unserial,evar,(const estr&));

  epregister(rnd);

  epregister2(getSystem(),"sys");
  epregisterClassMethodI(esystem,getTotalRam,"return total amount of RAM in system");
  epregisterClassMethodI(esystem,getFreeRam,"return free amount RAM in system");
  epregisterClassMethodI(esystem,getBufferRam,"return amount of RAM used by buffers");
  epregisterClassMethodI(esystem,getMemLimit,"return maximum memory limit set for process");
}


