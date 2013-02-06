#include "eparser.h"
#include "logger.h"

#include "eiostream.h"

#include "efile.h"

#include "evarcommon.h"


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
/*
evar epcall(const estr& funcname,const estr& args)
{
  if (parser->funcs.exists(funcname))
  {
    if (parser->funcs[funcname]->size()!=1)
      lwarn("overloaded function using first available");

    return( parser->funcs[funcname]->operator[](0)->call(args) );
  }

  lwarn("parser: function \""+funcname+"\" not found");
  return(evar());
}

evar epget(const estr& objname)
{
  if (parser->objects.exists(objname))
    return(parser->objects[objname]);
  lwarn("parser: var \""+objname+"\" not found, while getting value");
  return(evar());
}

void epset(const estr& objname,const evar& value)
{
  int i;
  i=objname.find(".");
  if (i!=-1)
    { epset(objname.substr(0,i),objname.substr(i+1),value); return; }

  if (parser->objects.exists(objname))
    { parser->objects[objname]->set(value); return; }

  lwarn("creating variable \""+objname+"\" (type: "+value.getClass()+")");
  parser->objects.add(objname,new evar(value.var));
}


evar epcall(const estr& objname,const estr& methodname,const estr& args)
{
  evararray arr;
  evarSplitArgs(arr,args);
  if (parser->objects.exists(objname))
    { return( parser->objects[objname]->call(methodname,arr) ); }

  lwarn("parser: object \""+objname+"\" not found, while calling method: "+methodname+" args: "+args);
  return(evar());
}

evar epget(const estr& objname,const estr& propertyname)
{
  if (parser->objects.exists(objname))
    { return(parser->objects[objname]->get(propertyname) ); }
  lwarn("parser: object \""+objname+"\" not found, while getting property: "+propertyname);
  return(evar());
}

void epset(const estr& objname,const estr& propertyname,const evar& value)
{
  if (parser->objects.exists(objname)){
    parser->objects[objname]->set(propertyname,value);
    return;
  }
  lwarn("parser: object \""+objname+"\" not found, while setting property: "+propertyname);
}
*/


/*
estr find_sarg(const estr &name)
{
  int i,j;
  bool found;
  estr t;

  for(j=0; j<name.size(); ++j){
    t=name.substr(j,1);

    found=false;
    for (i=0;i<parser->cmds.size(); ++i)
      if (t==parser->cmds[i].argname) { found=true; break; }
    if (!found) return(t);
  }
  
  for (j='a';j<='z';++j){
    t[0]=j;

    found=false;
    for (i=0;i<parser->cmds.size(); ++i)
      if (t==parser->cmds[i].argname) { found=true; break; }
    if (!found) return(t);
  }
 
  for (j='A';j<='Z';++j){
    t[0]=j;

    found=false;
    for (i=0;i<parser->cmds.size(); ++i)
      if (t==parser->cmds[i].argname) { found=true; break; }
    if (!found) return(t);
  }
  return("");
}
*/

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
  epregisterAlias("h","parser.help");
  epregisterAlias("help","parser.help");
  epregisterAlias("save","parser.save");
  epregisterAlias("load","parser.load");
//  getLogger()->level=1;
}

void eparser::execArg(estr cmd,const estr& args)
{
//  int i;
  if (cmd[0]=='-') cmd.del(0,1);
  if (cmd[0]=='-') cmd.del(0,1);

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
  if (var.getTypeid()==typeid(estr) || var.getTypeid()==typeid(estrarray))
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
  char* fcmd;

  parsing=true;

  newargs+=(*argv)[0]; 
 
  int i;

  bool process=true;
  for (i=1; i<*argvc; ++i){
    if (!process || ((*argv)[i][0]!='-' && (*argv)[i][0]!='+'))
      { newargs+=(*argv)[i]; continue; }

    if (!strcmp("--",(*argv)[i])) { process=false; continue; } // do not parse arguments after a -- command

    fcmd=(*argv)[i];
    fargs="";
    if (i+1<(*argvc) && (*argv)[i+1][0]!='-' && (*argv)[i+1][0]!='+'){
      fargs+=(*argv)[i+1];
      ++i;
    }
    execArg(fcmd,fargs);
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
}

