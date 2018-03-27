#include "eparser.h"
#include "logger.h"

#include "eiostream.h"

#include "efile.h"

//#include "evarcommon.h"

#include "eparserinterpreter.h"



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
  epregisterFuncMethod(uint,uintinc,"++");
  epregisterFuncMethod(uint,uintdec,"--");
  epregisterFuncMethod(uint,uintincpre,"++prefix");
  epregisterFuncMethod(uint,uintdecpre,"--prefix");
  epregisterFuncMethod(uint,uintssum,"+=");
  epregisterFuncMethod(uint,uintssub,"-=");
  epregisterFuncMethod(uint,uintsum,"+");  // the order between uintsum and uintstrsum is important, when a sum is occurring between int + float, if uintstrsum is selected first, then the float will be converted to a string and then concatenated, which is an undesired case. having uintsum first, makes the float be converted to an int and then summed
  epregisterFuncMethod(uint,uintstrsum,"+"); 
  epregisterFuncMethod(uint,uintsub,"-");
  epregisterFuncMethod(uint,uintmul,"*");
  epregisterFuncMethod(uint,uintdiv,"/");
  epregisterFuncMethod(uint,uintmod,"%");
  epregisterFuncMethod(uint,uintgt,">");
  epregisterFuncMethod(uint,uintlt,"<");
  epregisterFuncMethod(uint,uintgteq,"<=");
  epregisterFuncMethod(uint,uintlteq,">=");
  epregisterFuncMethod(uint,uinteq,"==");
  epregisterFuncMethod(uint,uintneq,"!=");

  epregisterClass(int);
  epregisterClassCout(int);
  epregisterClassSerializeType(int);
  epregisterFuncMethod(int,intinc,"++");
  epregisterFuncMethod(int,intdec,"--");
  epregisterFuncMethod(int,intincpre,"++prefix");
  epregisterFuncMethod(int,intdecpre,"--prefix");
  epregisterFuncMethod(int,intssum,"+=");
  epregisterFuncMethod(int,intssub,"-=");
  epregisterFuncMethod(int,intsum,"+");
  epregisterFuncMethod(int,intstrsum,"+");
  epregisterFuncMethod(int,intsub,"-");
  epregisterFuncMethod(int,intmul,"*");
  epregisterFuncMethod(int,intdiv,"/");
  epregisterFuncMethod(int,intmod,"%");
  epregisterFuncMethod(int,intgt,">");
  epregisterFuncMethod(int,intlt,"<");
  epregisterFuncMethod(int,intgteq,"<=");
  epregisterFuncMethod(int,intlteq,">=");
  epregisterFuncMethod(int,inteq,"==");
  epregisterFuncMethod(int,intneq,"!=");

  epregisterClass(long);
  epregisterClassCout(long);
  epregisterClassSerializeType(long);
  epregisterFuncMethod(long,longintinc,"++");
  epregisterFuncMethod(long,longintdec,"--");
  epregisterFuncMethod(long,longintincpre,"++prefix");
  epregisterFuncMethod(long,longintdecpre,"--prefix");
  epregisterFuncMethod(long,longintssum,"+=");
  epregisterFuncMethod(long,longintssub,"-=");
  epregisterFuncMethod(long,longintsum,"+");
  epregisterFuncMethod(long,longintstrsum,"+");
  epregisterFuncMethod(long,longintsub,"-");
  epregisterFuncMethod(long,longintmul,"*");
  epregisterFuncMethod(long,longintdiv,"/");
  epregisterFuncMethod(long,longintmod,"%");
  epregisterFuncMethod(long,longintgt,">");
  epregisterFuncMethod(long,longintlt,"<");
  epregisterFuncMethod(long,longintgteq,"<=");
  epregisterFuncMethod(long,longintlteq,">=");
  epregisterFuncMethod(long,longinteq,"==");
  epregisterFuncMethod(long,longintneq,"!=");

  epregisterClass(long int);
  epregisterClassCout(long int);
  epregisterClassSerializeType(long int);
  epregisterFuncMethod(long int,longintinc,"++");
  epregisterFuncMethod(long int,longintdec,"--");
  epregisterFuncMethod(long int,longintincpre,"++prefix");
  epregisterFuncMethod(long int,longintdecpre,"--prefix");
  epregisterFuncMethod(long int,longintssum,"+=");
  epregisterFuncMethod(long int,longintssub,"-=");
  epregisterFuncMethod(long int,longintsum,"+");
  epregisterFuncMethod(long int,longintstrsum,"+");
  epregisterFuncMethod(long int,longintsub,"-");
  epregisterFuncMethod(long int,longintmul,"*");
  epregisterFuncMethod(long int,longintdiv,"/");
  epregisterFuncMethod(long int,longintmod,"%");
  epregisterFuncMethod(long int,longintgt,">");
  epregisterFuncMethod(long int,longintlt,"<");
  epregisterFuncMethod(long int,longintgteq,"<=");
  epregisterFuncMethod(long int,longintlteq,">=");
  epregisterFuncMethod(long int,longinteq,"==");
  epregisterFuncMethod(long int,longintneq,"!=");

  epregisterClass(bool);
  epregisterClassCout(bool);
  epregisterClassSerializeType(bool);
  epregisterClassConstructor(bool,(int));
  epregisterFuncMethod(bool,booln,"!");
  epregisterFuncMethod(bool,booland,"&&");
  epregisterFuncMethod(bool,boolor,"||");
  epregisterFuncMethod(bool,boolxor,"^");

  epregisterClass(char);
  epregisterClassCout(char);
  epregisterClassSerializeType(char);

  epregisterClass(float);
  epregisterClassCout(float);
  epregisterClassSerializeType(float);
  epregisterFuncMethod(float,floatinc,"++");
  epregisterFuncMethod(float,floatdec,"--");
  epregisterFuncMethod(float,floatincpre,"++prefix");
  epregisterFuncMethod(float,floatdecpre,"--prefix");
  epregisterFuncMethod(float,floatssum,"+=");
  epregisterFuncMethod(float,floatssub,"-=");
  epregisterFuncMethod(float,floatsum,"+");
  epregisterFuncMethod(float,floatstrsum,"+");
  epregisterFuncMethod(float,floatsub,"-");
  epregisterFuncMethod(float,floatmul,"*");
  epregisterFuncMethod(float,floatdiv,"/");
  epregisterFuncMethod(float,floatgt,">");
  epregisterFuncMethod(float,floatlt,"<");
  epregisterFuncMethod(float,floatgteq,"<=");
  epregisterFuncMethod(float,floatlteq,">=");
  epregisterFuncMethod(float,floateq,"==");
  epregisterFuncMethod(float,floatneq,"!=");

  epregisterClass(double);
  epregisterClassCout(double);
  epregisterClassSerializeType(double);
  epregisterFuncMethod(double,doubleinc,"++");
  epregisterFuncMethod(double,doubledec,"--");
  epregisterFuncMethod(double,doubleincpre,"++prefix");
  epregisterFuncMethod(double,doubledecpre,"--prefix");
  epregisterFuncMethod(double,doublessum,"+=");
  epregisterFuncMethod(double,doublessub,"-=");
  epregisterFuncMethod(double,doublesum,"+");
  epregisterFuncMethod(double,doublestrsum,"+");
  epregisterFuncMethod(double,doublesub,"-");
  epregisterFuncMethod(double,doublemul,"*");
  epregisterFuncMethod(double,doublediv,"/");
  epregisterFuncMethod(double,doublegt,">");
  epregisterFuncMethod(double,doublelt,"<");
  epregisterFuncMethod(double,doublegteq,"<=");
  epregisterFuncMethod(double,doublelteq,">=");
  epregisterFuncMethod(double,doubleeq,"==");
  epregisterFuncMethod(double,doubleneq,"!=");

  epregisterClass(efunc);
  epregisterClassMethod4(efunc,operator=,efunc&,(const efunc&),"=");
  epregisterClassMethod4(efunc,operator(),evar,() const,"()");
  epregisterClassMethod4(efunc,operator(),evar,(const evararray& args) const,"()");
  epregisterClassMethod2(efunc,call,evar,(const estr&) const);

//  epregisterClass(ethread);
//  epregisterClassConstructor(ethread,());
//  epregisterClassMethod(ethread,run);
//  epregisterClassMethod(ethread,isBusy);
//  epregisterClassMethod(ethread,wait);

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
  epregisterClassMethod4(estr,operator<,bool,(const estr&) const,"<");
  epregisterClassMethod4(estr,operator<=,bool,(const estr&) const,"<=");
  epregisterClassMethod4(estr,operator>,bool,(const estr&) const,">");
  epregisterClassMethod4(estr,operator>=,bool,(const estr&) const,">=");
  epregisterClassMethod4(estr,operator==,bool,(const estr&) const,"==");
  epregisterClassMethod4(estr,operator!=,bool,(const estr&) const,"!=");
  epregisterClassMethod4(estr,operator+,estr,(const estr&) const,"+");
  epregisterClassMethod4(estr,operator+=,estr&,(const estr&),"+=");
  epregisterClassMethod4(estr,operator[],char&,(long int),"[]");

  epregisterClassConverterCast(estrarray,estr);

  epregisterClassCout(ebasearrayof);
  epregisterClassSerializeMethod(ebasearrayof);


  epregisterClass(estrhashof<evar>);
  epregisterClassCout(estrhashof<evar>);
  epregisterClassInheritance(estrhashof<evar>,ebasearrayof);
  epregisterClassSerializeMethod(estrhashof<evar>);

  epregisterClassInheritance(estrhashof<eclassBase>,ebasearrayof);
  epregisterClassInheritance(estrhashof<eclassPropertyBase>,ebasearrayof);
  epregisterClassInheritance(estrhashof<ebasicarray<eclassMethodBase*> >,ebasearrayof);
  epregisterClassInheritance(estrhashof<eclassBase*>,ebasearrayof);

  epregisterClass(estrarrayof<evararray>);
  epregisterClassInheritance(estrarrayof<evararray>,ebasearrayof);
  epregisterClassSerializeMethod(estrarrayof<evararray>);

  epregisterClass(estrarrayof<int>);
  epregisterClassInheritance(estrarrayof<int>,ebasearrayof);
  epregisterClassSerializeMethod(estrarrayof<int>);

  epregisterClass(estrarray);
  epregisterClassInheritance(estrarray,ebasearrayof);
  epregisterClassSerializeMethod(estrarray);


  epregisterClassCout(ebasearray);
  epregisterClassSerializeMethod(ebasearray);


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

#endif
}

eparser* parser=0x00;

eparser* getParser()
{
  if (!parser){
    getLogger();
    parser = new eparser();
    parser->registerParser();
  }
  return(parser);
}

void epregisterAlias(const estr& alias,const estr& var)
{
  getParser()->argAlias.add(alias,var);
}
void epregisterEnd()
{
  getParser()->registerEnd();
}

void eparseArgs(int& _argvc,char** &_argv)
{
  getParser()->parseArgs(_argvc,_argv);
}

bool epfuncExists(const estr& funcname)
{
  if (parser->funcs.exists(funcname))
    return(true);
  
  return(false);
}

bool epvarExists(const estr& varname)
{
  if (parser->objects.exists(varname))
    return(true);
  return(false);
}

eparser::eparser(): showHelp(false),userRegister(false),parsing(false),showRegister(false)
{
  // WARNING: dont put any class or method registering here
  //  classes use eparser to register
}

eparser::~eparser()
{
  if (showHelp){
//    if (userRegister) cout << "you should call parser->registerEnd() after you registered all your functions" << endl;  // we dont use ldwarn here because the logger object might no longer exist, as we have no control over the order of destruction of objects at the end of a program
    registerEnd();
  }
}


void eparser::registerEnd()
{
/*
  if (showHelp){
    help();
    exit(0);
  }

  int i;
  for (i=0; i<initvalues.size(); ++i){
//    cout << initvalues.keys(i)+"="+initvalues.values(i) << endl;
    if (initvalues.values(i).len())
      epinterpret(initvalues.keys(i)+"="+initvalues.values(i)+";");
    else
      epinterpret(initvalues.keys(i)+"()"+";");
  }
*/

/*
  int i,j;
  estrarray infostr;
  estr section;
  printf("\n");
  for (i=0; i<cmds.size(); ++i){
      if (section!=cmds[i].section){
        section=cmds[i].section;
        printf("\n");
        printf(" %s\n",section._str);
      }
      infostr = cmds[i].info.explode("\n");
      
      printf("  -%s --%-15s %s\n",cmds[i].argname._str,cmds[i].name._str,infostr[0]._str);
      for (j=1; j<infostr.size(); ++j)
        printf("                       %s\n",infostr[j]._str);
  }
  printf("\n");
*/
}

void eparser::help(const estr& line)
{
  int i;
  cout << endl;
  cout << "eparser.help():" << endl;
  cout << " - registered objects:"<<endl;
  for (i=0; i<objects.size(); ++i)
    cout << objects.keys(i) << endl;
  cout << " - registered functions:"<<endl;
  for (i=0; i<funcs.size(); ++i)
    cout << funcs.keys(i) << endl;

  if (parsing)
    showRegister=true;
}

void eparser::save(const estr& filename)
{
  int i;
  estr data;

  for (i=0; i<objects.size(); ++i){
    objects.keys(i).serial(data);
    objects[i].serial(data);
  }

  efile file(filename,"w");
  file.write(data);
  file.close();
}

void eparser::load(const estr& filename)
{
  estr data;
  efile file(filename,"r");
  file.read(data);
  file.close();

  int i=0;
  while (i < data.len()){
    estr varname;
    i=varname.unserial(data,i);
    if (objects.exists(varname)){
      evar var;
      i=var.unserial(data,i);
      objects[varname]=var;
    }else{
      evar *var=new evar;
      i=var->unserial(data,i);
      objects.add(varname,var);
//      lwarn("object: "+varname+" does not exist while loading environment: "+filename);
    }
    cout << "loaded varname: "<<varname<<endl;
  }
}


void eparser::registerParser()
{
  epregisterClassPropertyI(elogger,level,"verbose level");
  epregisterClassPropertyI(elogger,modules,"verbose modules");
  epregister2(getLogger(),"logger");
  epregisterClassMethodI(eparser,help,"show command help");
  epregisterClassMethodI(eparser,load,"load registered variables from file");
  epregisterClassMethodI(eparser,save,"save registered variables to file");
  epregisterClassPropertyI(eparser,objects,"global variables");
  epregisterCommon();
  epregister(parser);
//  epregisterAlias("h","parser.help");
//  epregisterAlias("help","parser.help");
  epregisterAlias("save","parser.save");
  epregisterAlias("load","parser.load");
//  getLogger()->level=1;
}

void eparser::execArg(estr cmd,const estr& args)
{
//  int i;
//  cout << argAlias << endl;
  if (argAlias.exists(cmd))
    cmd=argAlias[cmd];

  evar var=epinterpret(cmd+";");
  ldieif(var.getClass() == "empty","unknown argument: "+cmd);

  if (args.len() && args[0]=='{'){
    epinterpret(cmd+"="+args.substr(1,-2)+";");
    return;
  }

//  cout << cmd << " --- " << var.getClass() << endl;
  if (var.getTypeid()==typeid(efunc))
    var.get<efunc>().call(evararray(args));
  else if (var.getTypeid()==typeid(estr) || var.getTypeid()==typeid(estrarray))
    epinterpret(cmd+"=\""+args+"\";");
  else if (var.getTypeid()==typeid(int))
    epinterpret(cmd+"="+args.i()+";");
  else if (var.getTypeid()==typeid(double) || var.getTypeid()==typeid(double))
    epinterpret(cmd+"="+args.f()+";");
  else if (var.getTypeid()==typeid(bool)){
    if (args=="false" || args=="0")
      epinterpret(cmd+"=0;");
    else if (args=="true" || args=="1")
      epinterpret(cmd+"=1;");
    else
      ldie("invalid value for boolean argument: "+cmd);
  } else
    epinterpret(cmd+"=\""+args+"\";");

/*
  int i;
  for (i=0; i<initvalues.size(); ++i){
//    cout << initvalues.keys(i)+"="+initvalues.values(i) << endl;
    if (initvalues.values(i).len())
      epinterpret(initvalues.keys(i)+"="+initvalues.values(i)+";");
    else
      epinterpret(initvalues.keys(i)+"()"+";");
  }
*/

//  initvalues.add(cmd,args);

/*
  ldinfo("executing cmd: "+cmd+", args: "+args);
  int i;
  i=cmd.find(".");
  if (i>0){
    estr prop=cmd.substr(i+1);
    cmd.del(i);
    if (epvarExists(cmd)){
      ldinfo("found var: "+cmd);
      evar obj(epget(cmd).var);
      estr objclass = obj.getClass();
      if (classes.exists(objclass)){
        if (classes.values(objclass).method_exists(prop)){
          ldinfo("calling method: "+prop);
          obj.call(prop,args);
        }else if (classes.values(objclass).property_exists(prop)){
          ldinfo("setting property: "+prop);
          obj.set(prop,args);
        }else
          lwarn("Method or property: "+prop+" not found for obj: "+cmd);
      }else
        lwarn("Object class not registered");
    } else {
      initpropvalues.add(cmd,args);
      ldinfo("stored init prop value "+cmd);
    }
    return;
  }

  if (epvarExists(cmd)){
    ldinfo("found var: "+cmd);
    epget(cmd).set(args);
    return;
  }
  if (epfuncExists(cmd)){
    ldinfo("found function: "+cmd);
    epcall(cmd,args);
    return;
  }
  initvalues.add(cmd,args);
  ldinfo("stored init value "+cmd);
*/
}

void eparser::parseArgs(int& _argvc,char** &_argv)
{
  argv=&_argv;
  argvc=&_argvc;

  ebasicarray<char*> newargs;
  estr fargs;
  estr fcmd;

  parsing=true;
  estr setaction;

  newargs+=(*argv)[0]; 
 
  int i;
  bool process=true;
  for (i=1; i<*argvc; ++i){
    if (!process || (*argv)[i][0]!='-' && (*argv)[i][0]!='+')
      { newargs+=(*argv)[i]; continue; }

    if (!strcmp("--",(*argv)[i])) { process=false; continue; } // do not parse arguments after a "--" argument

    fcmd=estr((*argv)[i]);
    if (fcmd.len() && fcmd[0]=='-') fcmd.del(0,1);
    if (fcmd.len() && fcmd[0]=='-') fcmd.del(0,1);

    if (actions.exists(fcmd)){
      setaction=fcmd;
      process=false;
      continue;
    }else{
      fargs="";
      if (i+1<(*argvc) && (*argv)[i+1][0]!='-' && (*argv)[i+1][0]!='+'){
        fargs+=(*argv)[i+1];
        ++i;
      }
      execArg(fcmd,fargs);
    }
  }

  if (*argvc > newargs.size()){
    char **tmpargv;
    tmpargv = new char*[newargs.size()];
    for (i=0; i<newargs.size(); ++i)
      tmpargv[i] = newargs[i];
  
    *argv = tmpargv;
    *argvc = newargs.size();
  }

  parsing=false;
  if (setaction.len()>0)
    actions[setaction].call();
}

evar apply(const efunc& func,const evararray& args)
{
  eintarray argarr;
  ebasicarray<ebasearray*> argvar;
  evararray tmparr;
  int i;
  for (i=0; i<func.fArgs.size() && i<args.size(); ++i){
    if (*func.fArgs[i]!=args[i].getTypeid() && !args[i].isConvertible(*func.fArgs[i])){
      if (args[i].getTypeid()==typeid(evararray) || args[i].isConvertible(typeid(ebasearray))){
        argarr.add(i);
        argvar.add(&args[i].convert(typeid(ebasearray)).get<ebasearray>());
        tmparr.add(evar());
      }else{
        lwarn("Apply failed, argument "+estr(i)+" of type: "+args[i].getClass()+" does not match function argument: "+func.fArgs[i]->name());
        return(evar());
      }
    }else{
      tmparr.add(args[i]);
    }
  }
  if (argarr.size()==0)
    return(func.call(args));

  eintarray ind;
  ind.init(argarr.size(),0);
  evvararray results;

  while (ind[0] < argvar[0]->size()){
    for (i=0; i<ind.size(); ++i){
      tmparr[argarr[i]].set(argvar[0][ind[0]]);
      if (*func.fArgs[argarr[i]]!=args[argarr[i]].getTypeid() && !args[argarr[i]].isConvertible(*func.fArgs[argarr[i]])){
        lwarn("Apply failed, argument "+estr(argarr[i])+" of type: "+args[argarr[i]].getClass()+" does not match function argument: "+func.fArgs[argarr[i]]->name());
        return(evar());
      }
    }
    results.add(tmparr,func.call(tmparr));
    for (i=ind.size()-1; i>=0; --i){
      ++ind[i];
      if (ind[i]==argvar[i]->size() && i>0) ind[i]=0;
      else break;
    }
  }
  return(results); 
}



