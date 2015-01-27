#ifndef EPARSER_H
#define EPARSER_H

#include "eutils.h"
#include "estrhash.h"
#include "estrhashof.h"
#include "evararray.h"


class estr;
class evar;
class efunc;
class eparser;

#define epregister( obj )  epregister2(obj, #obj)
#define epregisterI( obj , info )  epregister2(obj, #obj,info)
template <class T>
void epregister2(T* object,const estr& objname,const estr& info="");

#define epregisterFunc( func )  epregisterFunc3(getParser(),func,#func)
#define epregisterFuncD( func , defargs )  epregisterFunc3(getParser(),func,#func,defargs)
#define epregisterFunc2( func , ret , args )  epregisterFunc3(getParser(),(ret (*)args)&func,#func)
#define epregisterFunc2D( func , defargs, ret , args )  epregisterFunc3(getParser(),(ret (*)args)&func,#func,defargs)
template <class F>
void epregisterFunc3(eparser* parser,F func,const estr& funcname,const evararray& args=evararray());

class eparser
{
 public:
  int *argvc;
  char ***argv;

  estrhash   initvalues;
  estrhash   argAlias;

  estrhashof<efunc> actions;
  estrhashof<evar>  objects;
  estrhashof< earray<efunc> > funcs;
//  estrhashof<epclassBase> classes;

  bool parsing;
  bool showRegister;

  bool initiated;

  bool showHelp;
  bool userRegister;

  eparser();
  ~eparser();

  void registerParser();
  void registerEnd();

  void help(const estr& line="");

  void save(const estr& filename);
  void load(const estr& filename);

  void execArg(estr argname,const estr& values);
  void parseArgs(int& _argvc,char** &_argv);
};

extern eparser* parser;
eparser* getParser();

#include "estr.h"

#include "earray.h"
#include "logger.h"
#include "evar.h"
#include "efunc.h"
#include "evararray.h"
#include "evarclass.h"

//#include "eparserinterpreter.h"

// epregisterClassHeader(eparser);
// epregisterClassHeader(elogger);

bool epvarExists(const estr& varname);
bool epfuncExists(const estr& funcname);

/*
void epset(const estr& objname,const evar& value);
evar epget(const estr& objname);
evar epcall(const estr& funcname,const estr& args="");

void epset(const estr& objname,const estr& propertyname,const evar& value);
evar epget(const estr& objname,const estr& propertyname);
evar epcall(const estr& objname,const estr& methodname,const estr& args);
*/

template <class T>
void epregister2(T* object,const estr& objname,const estr& info)
{
  getParser();

  ldinfo("registering object: "+objname);
  
  parser->objects.add(objname,new evar(object));

/*
  // At the moment of registering a variable, assign it the value
  // found in "initvalues"
  if (parser->initvalues.exists(objname)){
    ldinfo("setting initial value: "+objname);
    epset(objname,parser->initvalues[objname]);
  }

  // if help command was called during parsing, we show any new registered variables
  if (parser->showRegister){
    cout << "  --"<<objname<<"   "<<info<<endl;
  }

  if (getClasses().exists(typeid(T).name())){
    eclass<T> *c;
    GETCLASS(c,T);
    int i;
    ldinfo("object has registered class: "+objname);
    for (i=0; i<c->methods.size(); ++i){
      if (parser->initvalues.exists(objname+"."+c->methods.keys(i)))
        epcall(objname,c->methods.keys(i),parser->initvalues[objname+"."+c->methods.keys(i)]);
    }
    for (i=0; i<c->properties.size(); ++i){
      ldinfo("searching properties: "+c->properties.keys(i));
      if (parser->initvalues.exists(objname+"."+c->properties.keys(i)))
        epset(objname,c->properties.keys(i),parser->initvalues[objname+"."+c->properties.keys(i)]);
    }
  }
*/
}

template <class T>
void epregister2(T& object,const estr& objname,const estr& info="")
{
//  epclassAssignId<T>();
  getParser();
  
  ldinfo("registering object: "+objname);

  parser->objects.add(objname,new evar(object));

/*
  // At the moment of registering a variable, assign it the value
  // found in "initvalues"
  if (parser->initvalues.exists(objname))
    epset(objname,parser->initvalues[objname]);

  if (parser->showRegister)
    cout << "  --"<<objname<<"    "<<info<<endl;

  if (parser->classes.exists(typeid(T).name())){
    epclass<T> *c;
    GETCLASS(c,T);
    int i;
    for (i=0; i<c->methods.size(); ++i){
      if (parser->initvalues.exists(objname+"."+c->methods.keys(i)))
        epcall(objname,c->methods.keys(i),parser->initvalues[objname+"."+c->methods.keys(i)]);
    }
    for (i=0; i<c->properties.size(); ++i){
      if (parser->initvalues.exists(objname+"."+c->properties.keys(i)))
        epset(objname,c->properties.keys(i),parser->initvalues[objname+"."+c->properties.keys(i)]);
    }
  }
*/
}

template <class F>
void epregisterFunc3(eparser* parser,F func,const estr& funcname,const evararray& args)
{
  if (!parser->funcs.exists(funcname))
    parser->funcs.add(funcname,earray<efunc>());

  parser->funcs[funcname].add(efunc(func,args));

  // At the moment of registering a function, call it with the value
  // found in "initvalues"
  
//  if (parser->initvalues.exists(funcname))
//    epcall(funcname,parser->initvalues[funcname]);
}

void epregisterAlias(const estr& alias,const estr& var);
void epregisterEnd();
void eparseArgs(int& _argvc,char** &_argv);

evar apply(const efunc& func,const evararray& args);



/*
template <class P>
void epset(const estr& objname,const estr& propertyname,P value)
{
  if (parser.objects.exists(objname)){
    parser.objects[objname]->set(propertyname,value);
    return;
  }
  lwarn("parser: object \""+objname+"\" not found, while setting property: "+propertyname);
}
*/


#endif

