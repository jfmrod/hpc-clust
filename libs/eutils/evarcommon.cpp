#include "eparser.h"

#include "evarcommon.h"
#include "evarclass.h"

#include "esocket.h"

#include "evarclassconverter.h"
#include "estrarray.h"

#include "estr.h"
#include "efile.h"
#include "edir.h"
#include "etable.h"
#include "ecsv.h"
#include "emysql.h"
#include "etimer.h"

#include "evarclassconstructor.h"
#include "evarclasscout.h"
#include "eparser.h"
#include "eudl.h"
//#include "efilesys.h"

#include "ematrix.h"
#include "vector2.h"
#include "vector3.h"
//#include "ethread.h"


#include "eregexp.h"

#include "efunc.h"

#include "ernd.h"
#include "eparserinterpreter.h"
#include "emodule.h"

typedef unsigned int uint;

uint& uintssum(uint& a,uint b){ a+=b; return(a); }
uint& uintssub(uint& a,uint b){ a-=b; return(a); }
uint uintdec(uint& a) { return(a--); }
uint& uintdecpre(uint& a) { return(--a); }
uint uintinc(uint& a) { return(a++); }
uint& uintincpre(uint& a) { return(++a); }
uint uintsum(uint a,uint b){ return(a+b); }
uint uintsub(uint a,uint b){ return(a-b); }
uint uintmul(uint a,uint b){ return(a*b); }
uint uintdiv(uint a,uint b){ return(b!=0?a/b:0); }
uint uintmod(uint a,uint b){ return(a%b); }

bool uintlt(uint a,uint b){ return(a<b); }
bool uintgt(uint a,uint b){ return(a>b); }
bool uintlteq(uint a,uint b){ return(a<=b); }
bool uintgteq(uint a,uint b){ return(a>=b); }
bool uinteq(uint a,uint b)  { return(a==b); }
bool uintneq(uint a,uint b) { return(a!=b); }

int& intssum(int& a,int b){ a+=b; return(a); }
int& intssub(int& a,int b){ a-=b; return(a); }
int intdec(int& a) { return(a--); }
int& intdecpre(int& a) { return(--a); }
int intinc(int& a) { return(a++); }
int& intincpre(int& a) { return(++a); }
int intsum(int a,int b){ return(a+b); }
int intsub(int a,int b){ return(a-b); }
int intmul(int a,int b){ return(a*b); }
int intdiv(int a,int b){ return(b!=0?a/b:0); }
int intmod(int a,int b){ return(a%b); }

bool intlt(int a,int b){ return(a<b); }
bool intgt(int a,int b){ return(a>b); }
bool intlteq(int a,int b){ return(a<=b); }
bool intgteq(int a,int b){ return(a>=b); }
bool inteq(int a,int b)  { return(a==b); }
bool intneq(int a,int b) { return(a!=b); }

long int& longintssum(long int& a,long int b){ a+=b; return(a); }
long int& longintssub(long int& a,long int b){ a-=b; return(a); }
long int longintdec(long int& a) { return(a--); }
long int& longintdecpre(long int& a) { return(--a); }
long int longintinc(long int& a) { return(a++); }
long int& longintincpre(long int& a) { return(++a); }
long int longintsum(long int a,long int b){ return(a+b); }
long int longintsub(long int a,long int b){ return(a-b); }
long int longintmul(long int a,long int b){ return(a*b); }
long int longintdiv(long int a,long int b){ return(b!=0?a/b:0); }
long int longintmod(long int a,long int b){ return(a%b); }

bool longintlt(long int a,long int b){ return(a<b); }
bool longintgt(long int a,long int b){ return(a>b); }
bool longintlteq(long int a,long int b){ return(a<=b); }
bool longintgteq(long int a,long int b){ return(a>=b); }
bool longinteq(long int a,long int b)  { return(a==b); }
bool longintneq(long int a,long int b) { return(a!=b); }

estr uintstrsum(uint a,const estr& b){ return(a+b); }
estr intstrsum(int a,const estr& b){ return(a+b); }
estr longintstrsum(long int a,const estr& b){ return(a+b); }
estr floatstrsum(float a,const estr& b){ return(a+b); }
estr doublestrsum(double a,const estr& b){ return(a+b); }

float& floatssum(float& a,float b){ a+=b; return(a); }
float& floatssub(float& a,float b){ a-=b; return(a); }
float floatdec(float& a) { return(a--); }
float& floatdecpre(float& a) { return(--a); }
float floatinc(float& a) { return(a++); }
float& floatincpre(float& a) { return(++a); }
float floatsum(float a,float b){ return(a+b); }
float floatsub(float a,float b){ return(a-b); }
float floatmul(float a,float b){ return(a*b); }
float floatdiv(float a,float b){ return(a/b); }

bool floatlt(float a,float b){ return(a<b); }
bool floatgt(float a,float b){ return(a>b); }
bool floatlteq(float a,float b){ return(a<=b); }
bool floatgteq(float a,float b){ return(a>=b); }
bool floateq(float a,float b)  { return(a==b); }
bool floatneq(float a,float b) { return(a!=b); }

double& doublessum(double& a,double b){ a+=b; return(a); }
double& doublessub(double& a,double b){ a-=b; return(a); }
double doubledec(double& a) { return(a--); }
double& doubledecpre(double& a) { return(--a); }
double doubleinc(double& a) { return(a++); }
double& doubleincpre(double& a) { return(++a); }
double doublesum(double a,double b){ return(a+b); }
double doublesub(double a,double b){ return(a-b); }
double doublemul(double a,double b){ return(a*b); }
double doublediv(double a,double b){ return(a/b); }

bool doublelt(double a,double b){ return(a<b); }
bool doublegt(double a,double b){ return(a>b); }
bool doublelteq(double a,double b){ return(a<=b); }
bool doublegteq(double a,double b){ return(a>=b); }
bool doubleeq(double a,double b)  { return(a==b); }
bool doubleneq(double a,double b) { return(a!=b); }

bool booln(bool a){ return(!a); }
bool booland(bool a,bool b){ return(a&&b); }
bool boolor(bool a,bool b) { return(a||b); }
bool boolxor(bool a,bool b){ return(a^b);  }



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



void epregisterCommon()
{
#ifdef EUTILS_REGISTER_COMMON
  epregisterClassConverterCast2(bool,char);
  epregisterClassConverterCast2(bool,int);
  epregisterClassConverterCast2(bool,long int);
  epregisterClassConverterCast2(char,int);
  epregisterClassConverterCast2(char,long int);
  epregisterClassConverterCast2(int,uint);
  epregisterClassConverterCast2(int,long int);
  epregisterClassConverterCast2(int,float);
  epregisterClassConverterCast2(long int,float);
  epregisterClassConverterCast2(int,double);
  epregisterClassConverterCast2(long int,double);
  epregisterClassConverterCast2(float,double);

  epregisterClassConverterCast(estr,char);
  epregisterClassConverterCast(estr,int);
  epregisterClassConverterCast(estr,uint);
  epregisterClassConverterCast(estr,float);
  epregisterClassConverterCast(estr,double);
  
  epregisterClassConverterCast(efile,estr);

  epregisterClassConverterMethod(int,estr,i);
  epregisterClassConverterMethod(float,estr,f);
  epregisterClassConverterMethod(double,estr,d);


  epregisterClass(uint);
  epregisterClassCout(uint);
  epregisterClassSerializeType(uint);
  epregisterFuncMethod(uint,uintinc,"operator++");
  epregisterFuncMethod(uint,uintdec,"operator--");
  epregisterFuncMethod(uint,uintincpre,"operator++prefix");
  epregisterFuncMethod(uint,uintdecpre,"operator--prefix");
  epregisterFuncMethod(uint,uintssum,"operator+=");
  epregisterFuncMethod(uint,uintssub,"operator-=");
  epregisterFuncMethod(uint,uintsum,"operator+");  // the order between uintsum and uintstrsum is important, when a sum is occurring between int + float, if uintstrsum is selected first, then the float will be converted to a string and then concatenated, which is an undesired case. having uintsum first, makes the float be converted to an int and then summed
  epregisterFuncMethod(uint,uintstrsum,"operator+"); 
  epregisterFuncMethod(uint,uintsub,"operator-");
  epregisterFuncMethod(uint,uintmul,"operator*");
  epregisterFuncMethod(uint,uintdiv,"operator/");
  epregisterFuncMethod(uint,uintmod,"operator%");
  epregisterFuncMethod(uint,uintgt,"operator>");
  epregisterFuncMethod(uint,uintlt,"operator<");
  epregisterFuncMethod(uint,uintgteq,"operator<=");
  epregisterFuncMethod(uint,uintlteq,"operator>=");
  epregisterFuncMethod(uint,uinteq,"operator==");
  epregisterFuncMethod(uint,uintneq,"operator!=");

  epregisterClass(int);
  epregisterClassCout(int);
  epregisterClassSerializeType(int);
  epregisterFuncMethod(int,intinc,"operator++");
  epregisterFuncMethod(int,intdec,"operator--");
  epregisterFuncMethod(int,intincpre,"operator++prefix");
  epregisterFuncMethod(int,intdecpre,"operator--prefix");
  epregisterFuncMethod(int,intssum,"operator+=");
  epregisterFuncMethod(int,intssub,"operator-=");
  epregisterFuncMethod(int,intsum,"operator+");
  epregisterFuncMethod(int,intstrsum,"operator+");
  epregisterFuncMethod(int,intsub,"operator-");
  epregisterFuncMethod(int,intmul,"operator*");
  epregisterFuncMethod(int,intdiv,"operator/");
  epregisterFuncMethod(int,intmod,"operator%");
  epregisterFuncMethod(int,intgt,"operator>");
  epregisterFuncMethod(int,intlt,"operator<");
  epregisterFuncMethod(int,intgteq,"operator<=");
  epregisterFuncMethod(int,intlteq,"operator>=");
  epregisterFuncMethod(int,inteq,"operator==");
  epregisterFuncMethod(int,intneq,"operator!=");

  epregisterClass(long int);
  epregisterClassCout(long int);
  epregisterClassSerializeType(long int);
  epregisterFuncMethod(long int,longintinc,"operator++");
  epregisterFuncMethod(long int,longintdec,"operator--");
  epregisterFuncMethod(long int,longintincpre,"operator++prefix");
  epregisterFuncMethod(long int,longintdecpre,"operator--prefix");
  epregisterFuncMethod(long int,longintssum,"operator+=");
  epregisterFuncMethod(long int,longintssub,"operator-=");
  epregisterFuncMethod(long int,longintsum,"operator+");
  epregisterFuncMethod(long int,longintstrsum,"operator+");
  epregisterFuncMethod(long int,longintsub,"operator-");
  epregisterFuncMethod(long int,longintmul,"operator*");
  epregisterFuncMethod(long int,longintdiv,"operator/");
  epregisterFuncMethod(long int,longintmod,"operator%");
  epregisterFuncMethod(long int,longintgt,"operator>");
  epregisterFuncMethod(long int,longintlt,"operator<");
  epregisterFuncMethod(long int,longintgteq,"operator<=");
  epregisterFuncMethod(long int,longintlteq,"operator>=");
  epregisterFuncMethod(long int,longinteq,"operator==");
  epregisterFuncMethod(long int,longintneq,"operator!=");


  epregisterClass(bool);
  epregisterClassCout(bool);
  epregisterClassSerializeType(bool);
  epregisterClassConstructor(bool,(int));
  epregisterFuncMethod(bool,booln,"operator!");
  epregisterFuncMethod(bool,booland,"operator&&");
  epregisterFuncMethod(bool,boolor,"operator||");
  epregisterFuncMethod(bool,boolxor,"operator^");

  epregisterClass(char);
  epregisterClassCout(char);
  epregisterClassSerializeType(char);

  epregisterClass(float);
  epregisterClassCout(float);
  epregisterClassSerializeType(float);
  epregisterFuncMethod(float,floatinc,"operator++");
  epregisterFuncMethod(float,floatdec,"operator--");
  epregisterFuncMethod(float,floatincpre,"operator++prefix");
  epregisterFuncMethod(float,floatdecpre,"operator--prefix");
  epregisterFuncMethod(float,floatssum,"operator+=");
  epregisterFuncMethod(float,floatssub,"operator-=");
  epregisterFuncMethod(float,floatsum,"operator+");
  epregisterFuncMethod(float,floatstrsum,"operator+");
  epregisterFuncMethod(float,floatsub,"operator-");
  epregisterFuncMethod(float,floatmul,"operator*");
  epregisterFuncMethod(float,floatdiv,"operator/");
  epregisterFuncMethod(float,floatgt,"operator>");
  epregisterFuncMethod(float,floatlt,"operator<");
  epregisterFuncMethod(float,floatgteq,"operator<=");
  epregisterFuncMethod(float,floatlteq,"operator>=");
  epregisterFuncMethod(float,floateq,"operator==");
  epregisterFuncMethod(float,floatneq,"operator!=");

  epregisterClass(double);
  epregisterClassCout(double);
  epregisterClassSerializeType(double);
  epregisterFuncMethod(double,doubleinc,"operator++");
  epregisterFuncMethod(double,doubledec,"operator--");
  epregisterFuncMethod(double,doubleincpre,"operator++prefix");
  epregisterFuncMethod(double,doubledecpre,"operator--prefix");
  epregisterFuncMethod(double,doublessum,"operator+=");
  epregisterFuncMethod(double,doublessub,"operator-=");
  epregisterFuncMethod(double,doublesum,"operator+");
  epregisterFuncMethod(double,doublestrsum,"operator+");
  epregisterFuncMethod(double,doublesub,"operator-");
  epregisterFuncMethod(double,doublemul,"operator*");
  epregisterFuncMethod(double,doublediv,"operator/");
  epregisterFuncMethod(double,doublegt,"operator>");
  epregisterFuncMethod(double,doublelt,"operator<");
  epregisterFuncMethod(double,doublegteq,"operator<=");
  epregisterFuncMethod(double,doublelteq,"operator>=");
  epregisterFuncMethod(double,doubleeq,"operator==");
  epregisterFuncMethod(double,doubleneq,"operator!=");

  epregisterClass(efunc);
  epregisterClassMethod2(efunc,operator=,efunc&,(const efunc&));
  epregisterClassMethod2(efunc,operator(),evar,());
  epregisterClassMethod2(efunc,operator(),evar,(const evararray& args));
  epregisterClassMethod2(efunc,call,evar,(const estr&));

//  epregisterClass(ethread);
//  epregisterClassConstructor(ethread,());
//  epregisterClassMethod(ethread,run);
//  epregisterClassMethod(ethread,isBusy);
//  epregisterClassMethod(ethread,wait);

  epregisterClass(eregexp);
  epregisterClassConstructor(eregexp,(const estr&));
  epregisterClassMethod(eregexp,compile);
  epregisterClassMethod2(eregexp,match,int,(const estr&,int,int));
  epregisterClassProperty(eregexp,b);
  epregisterClassProperty(eregexp,e);

  epregisterClass(eudpsocket);
//  epregisterClassMethodD(eudpsocket,listen,"0.0.0.0");
  epregisterClassMethod(eudpsocket,listen);
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

/*
  epregisterClass(eserver<esocket>);
  epregisterClassConstructor(eserver<esocket>,());
  epregisterClassMethodD(eserver<esocket>,listen,"0.0.0.0");
  epregisterClassMethod2(eserver<esocket>,accept,esocket*,());
  epregisterClassProperty(eserver<esocket>,onIncoming);
  epregisterClassProperty(eserver<esocket>,sockets);
*/

  epregisterClass(estr);
  epregisterClassCout(estr);
  epregisterClassSerializeMethod(estr);
  epregisterClassConstructor(estr,(int));
  epregisterClassConstructor(estr,(float));
  epregisterClassConstructor(estr,(double));
  epregisterClassMethod(estr,substr);
  epregisterClassMethodD(estr,find,evararray(0));
  epregisterClassMethod(estr,explode);
  epregisterClassMethod(estr,erase);
  epregisterClassMethod(estr,len);
  epregisterClassMethod2(estr,operator<,bool,(const estr&) const);
  epregisterClassMethod2(estr,operator<=,bool,(const estr&) const);
  epregisterClassMethod2(estr,operator>,bool,(const estr&) const);
  epregisterClassMethod2(estr,operator>=,bool,(const estr&) const);
  epregisterClassMethod2(estr,operator==,bool,(const estr&) const);
  epregisterClassMethod2(estr,operator!=,bool,(const estr&) const);
  epregisterClassMethod2(estr,operator+,estr,(const estr&) const);
  epregisterClassMethod2(estr,operator+=,estr&,(const estr&));
  epregisterClassMethod2(estr,operator[],char&,(long int));


  epregisterClassConverterCast(estrarray,estr);

  epregisterClassCout(ebasearrayof);
  epregisterClassSerializeMethod(ebasearrayof);
  epregisterClassMethod2(ebasearrayof,addvar,void,(evar&,evar&));
  epregisterClassMethod2(ebasearrayof,getvar,evar,(int));
  epregisterClassMethod2(ebasearrayof,getvarkey,evar,(int));
  epregisterClassMethod2A(ebasearrayof,addvar,"add",void,(evar&,evar&));
  epregisterClassMethod2A(ebasearrayof,getvarByKey,"operator[]",evar,(const evar&));
  epregisterClassMethod2A(ebasearrayof,getvar,"values",evar,(int));
  epregisterClassMethod2A(ebasearrayof,getvarkey,"keys",evar,(int));
  epregisterClassMethod2(ebasearrayof,size,int,());

  epregisterClass(estrhashof<evar>);
  epregisterClassCout(estrhashof<evar>);
  epregisterClassInheritance(estrhashof<evar>,ebasearrayof);
  epregisterClassSerializeMethod(estrhashof<evar>);

//  epregisterClassCout(estrhashof<eclassBase>);
  epregisterClassInheritance(estrhashof<eclassBase>,ebasearrayof);
     
  epregisterClassInheritance(estrhashof<eclassPropertyBase>,ebasearrayof);
//  epregisterClassCout(estrhashof<eclassPropertyBase>);
  epregisterClassInheritance(estrhashof<ebasicarray<eclassMethodBase*> >,ebasearrayof);
//  epregisterClassCout(estrhashof<eclassMethodBase>);
  epregisterClassInheritance(estrhashof<eclassBase*>,ebasearrayof);
//  epregisterClassCout(estrhashof<eclassBase*>);

  epregister2(getClasses(),"classes");
  epregisterClassProperty(eclassBase,properties);
  epregisterClassProperty(eclassBase,methods);
  epregisterClassProperty(eclassBase,parents);
 
  epregisterClass(estrarrayof<evararray>);
  epregisterClassInheritance(estrarrayof<evararray>,ebasearrayof);
  epregisterClassSerializeMethod(estrarrayof<evararray>);

  epregisterClass(estrarrayof<int>);
  epregisterClassInheritance(estrarrayof<int>,ebasearrayof);
  epregisterClassSerializeMethod(estrarrayof<int>);

  epregisterClass(estrarray);
  epregisterClassConstructor(estrarray,(const estr&));
  epregisterClassInheritance(estrarray,ebasearrayof);
  epregisterClassSerializeMethod(estrarray);

  epregisterClassMethod2(estrarray,add,estr&,(const estr&));
  epregisterClassMethod2(estrarray,add,estr&,(const estr&,const estr&));
//  epregisterClassMethod2(estrarray,size,int,());
  epregisterClassMethod(estrarray,load);
  epregisterClassMethod(estrarray,save);
  epregisterClassMethod(estrarray,join);
//  epregisterClassMethod2D(estrarray,find,int,(const estr&,int,bool (*)(const estr&,const estr&)),evararray(0,&estrarray::equal));
//  epregisterClassMethod2D(estrarray,findkey,int,(const estr&,int,bool (*)(const estr&,const estr&)),evararray(0,&estrarray::equalkey));
  epregisterClassMethod2(estrarray,keys,const estr&,(int) const);
  epregisterClassMethod2(estrarray,values,estr&,(int));
  epregisterClassMethod2(estrarray,operator[],estr&,(int));
  epregisterClassMethod2(estrarray,operator[],estr&,(const estr&));
//  epregisterClassMethod2(estrarray,operator=,estrarray&,(const estrarray&));
  epregisterClassMethod2(estrarray,operator-=,estrarray&,(const estrarray&));
  epregisterClassMethod2(estrarray,operator-,estrarray,(const estrarray&));
  epregisterClassMethod3<estrarray>((estr& (estrarray::*)(const estr&)) &estrarray::add,"operator+=");
//  epregisterClassMethod2(estrarray,operator+=,estrarray&,(const estr&));


  epregisterClassCout(ebasearray);
  epregisterClassSerializeMethod(ebasearray);
  epregisterClassMethod2(ebasearray,addvar,void,(evar&));
  epregisterClassMethod2(ebasearray,getvar,evar,(int));
  epregisterClassMethod2(ebasearray,size,int,());
  epregisterClassMethod2A(ebasearray,addvar,"add",void,(evar&));
  epregisterClassMethod2A(ebasearray,getvar,"operator[]",evar,(int));


  epregisterClass(etimer);
  epregisterClassMethod(etimer,lap);
  epregisterClassMethod(etimer,check);
  epregisterClassMethod(etimer,reset);

  epregisterClass(earray<esocket>);
  epregisterClassInheritance(earray<esocket>,ebasearray);

  epregisterClass(edoublearray);
  epregisterClassInheritance(edoublearray,ebasearray);
  epregisterClassSerializeMethod(edoublearray);

  epregisterClass(efloatarray);
  epregisterClassInheritance(efloatarray,ebasearray);
  epregisterClassSerializeMethod(efloatarray);

  epregisterClass(eintarray);
  epregisterClassInheritance(eintarray,ebasearray);
  epregisterClassSerializeMethod(eintarray);

  epregisterClass(evararray);
  epregisterClassSerializeMethod(evararray);
  epregisterClassInheritance(evararray,ebasearray);
  epregisterClassMethod2(evararray,add,evar&,(const evar&));
//  epregisterClassMethod2(evararray,size,int,());
  epregisterClassMethod2(evararray,operator[],evar&,(int));


  epregisterClass(efile);
  epregisterClassCout(efile);
  epregisterClassConstructor(efile,(const estr&));
  epregisterClassConstructor(efile,(const estr&,const estr&));
  epregisterClassMethod2(efile,open,bool,() const);
  epregisterClassMethod2(efile,open,bool,(const estr&,const estr&));
  epregisterClassMethod(efile,read);
  epregisterClassMethod(efile,readln);
  epregisterClassMethod2(efile,write,void,(const estr&) const);
  epregisterClassMethod2(efile,write,void,(const evar&) const);
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
  epregisterClassMethod2(estrarrayof<efile>,operator[],efile&,(const estr&));
  epregisterClassMethod2(estrarrayof<efile>,operator[],efile&,(int));

  epregisterClass(estrarrayof<edir>);
  epregisterClassInheritance(estrarrayof<edir>,ebasearrayof);
  epregisterClassMethod(estrarrayof<edir>,size);
  epregisterClassMethod2(estrarrayof<edir>,keys,estr&,(int));
  epregisterClassMethod2(estrarrayof<edir>,values,edir&,(int));
  epregisterClassMethod2(estrarrayof<edir>,operator[],edir&,(const estr&));
  epregisterClassMethod2(estrarrayof<edir>,operator[],edir&,(int));


  epregisterFuncD(ls,evararray("."));
  epregisterFuncD(cd,evararray("~"));
  epregisterFunc(pwd);
  epregisterFunc2(env,estrarray,());
  

  epregisterClass(etable);
  epregisterClassCout(etable);
  epregisterClassSerializeMethod(etable);
  epregisterClassMethod2(etable,operator[],evararray&,(int));
  epregisterClassMethod2(etable,operator[],evararray&,(const estr&));
  epregisterClassMethod(etable,row);
  epregisterClassMethod(etable,filter);
  epregisterClassMethod(etable,addfield);
  epregisterClassMethod2(etable,add,void,(const estrarray&));
  epregisterClassMethod2(etable,add,void,(const estr&,const evar&));
  epregisterClassMethod(etable,size);
  epregisterClassMethod(etable,clear);
  epregisterClassMethod(etable,load);
  epregisterClassMethod(etable,save);
  epregisterClassProperty(etable,cols);
  epregisterClassMethod2(erow,operator[],evar&,(int));
  epregisterClassMethod2(erow,operator[],evar&,(const estr&));
  epregisterClassMethod(erow,size);

  epregisterFunc(loadWSV);


  epregisterClass(evector2);
  epregisterClassCout(evector2);
  epregisterClassConstructor(evector2,(float,float));
  epregisterClassProperty(evector2,x);
  epregisterClassProperty(evector2,y);
  epregisterClassMethod(evector2,len);
  epregisterClassMethod(evector2,unit);
  epregisterClassMethod(evector2,perp);
  epregisterClassMethod(evector2,operator+);
  epregisterClassMethod2(evector2,operator-,evector2,(const evector2&) const);
  epregisterClassMethod(evector2,operator+=);
  epregisterClassMethod(evector2,operator-=);
  epregisterClassMethod2(evector2,operator*,float,(const evector2&) const);
  epregisterClassMethod2(evector2,operator*,evector2,(float) const);
  epregisterClassMethod(evector2,operator/);

  epregisterClass(evector3);
  epregisterClassCout(evector3);

  epregisterClass(ematrix);
  epregisterClassCout(ematrix);
  epregisterClassMethod(ematrix,load);
  epregisterClassMethod(ematrix,save);
  epregisterClassMethod(ematrix,create);
  epregisterClassMethod(ematrix,mulrow);
  epregisterClassMethod(ematrix,addmulrow);
  epregisterClassMethod(ematrix,swap);
  epregisterClassMethod2(ematrix,operator(),double&,(int,int));
  epregisterClassMethod2(ematrix,row,evector,(int) const);
  epregisterClassMethod2(ematrix,col,evector,(int) const);
  epregisterClassMethod2(ematrix,operator*,evector,(const evector&) const);
  epregisterClassMethod2(ematrix,copytranspose,void,(const ematrix&));
  epregisterClassProperty(ematrix,w);
  epregisterClassProperty(ematrix,h);

  epregisterClass(evector);
  epregisterClassCout(evector);
  epregisterClassMethod(evector,create);
  epregisterClassMethod2(evector,operator(),double&,(int));
  epregisterClassMethod2(evector,operator[],double&,(int));
  epregisterClassMethod2(evector,operator*,evector,(const ematrix&) const);
  epregisterClassMethod(evector,length);
  epregisterClassProperty(evector,w);

  epregisterClassConstructor(emysql,());
  epregisterClassMethodD(emysql,connect,evararray("localhost","root","","",3306));
  epregisterClassMethod(emysql,query);

  epregisterClassMethod(ernd,uniform);
#ifdef EUTILS_HAVE_LIBGSL
  epregisterClassMethod(ernd,gamma);
  epregisterClassMethod(ernd,gaussian);
  epregisterClassMethod(ernd,exponential);
  epregisterClassMethod(ernd,geometric);
#endif

  epregisterClassMethod(emresult,size);
  epregisterClassMethod(emresult,clear);
  epregisterClassMethod(emresult,load);
  epregisterClassMethod(emresult,save);

  epregisterFunc2(grep,estr,(estr&,const eregexp&,int));

  epregisterFunc(udl_load);

  epregisterFunc(eigenvalues);
  epregisterFunc(svd);
  epregisterFunc(rref);
  epregisterFunc(nullspace);
  epregisterFunc(nullspacer);

  epregisterFunc(sin);
  epregisterFunc(cos);
  epregisterFunc(tan);
  epregisterFunc(asin);
  epregisterFunc(acos);
  epregisterFunc(atan);
  epregisterFunc(sqrt);
  epregisterFunc(log);
  epregisterFunc(log2);
  epregisterFunc(log10);
  epregisterFunc(exp);
  epregisterFunc(sleep);
  epregisterFunc2(popen,efile,(const estr&));
  epregisterFunc2(system,void,(const estr&));

  epregisterFunc3(getParser(),epinterpret,"eval");

  epregisterFunc3(getParser(),varprint,"print");
  epregisterFunc3(getParser(),vartype,"type");
  epregisterFunc3(getParser(),varaddress,"addr");
  epregisterFunc2(serial,void,(const evar&,estr&));
  epregisterFunc2(unserial,evar,(const estr&));

#ifdef WITH_DL
  epregisterFunc(emodule_load);
#endif

  epregister(rnd);
#endif
}

