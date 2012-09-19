#include <stdio.h>
#include <fcntl.h>
#include <sstream>

#include "evar.h"
#include "eiostream.h"
#include "logger.h"

#ifndef _MSC_VER
#include <unistd.h>
#endif

#ifdef WIN32
 #include <windows.h>
#endif

#undef logger
#undef printf

#include "eterm.h"

#include "etime.h"



static elogger *logger = 0x00;

//#include "eparser.h"


elogger *getLogger()
{
  if (logger==0x00)
    logger=new elogger;
  return(logger);
}

/*
void ldie(const estr &diemsg,const estr& function)
{
  lerror(diemsg,function);
  exit(1);
}
*/

CDebugLine::CDebugLine()
{
}

CDebugLine::CDebugLine(time_t _time,int _level, const estr& _file, const estr& _function, int _line, const estr& _message,const estr& _module): level(_level),file(_file),function(_function),line(_line),message(_message),module(_module),time(_time)
{
}

estr CDebugLine::text() const
{
  estr msg;
  switch (level){
    case 0:msg = etermSetColor(tcBlue); msg+="   "; break;
    case 1:msg = etermSetColor(tcBlue); msg+=".  "; break;
    case 2:msg = etermSetColor(tcGreen); msg+="i  "; break;
    case 3:msg = etermSetColor(tcGreen); msg+="I  "; break;
    case 4:msg="S  "; break;
    case 5:msg = etermSetColor(tcYellow,true); msg+="!! "; break;
    case 6:msg = etermSetColor(tcRed,true); msg+="E! "; break;
  }
  msg+=utime2date(time)+" ";
  msg+="["+module+"] ";
  msg+=function+"(): ";
  msg+=message;
  msg+=etermUnsetColor();
  return(msg);
}


elogger::elogger():trace(false),logtofile(false),maxlines(200),level(3),dbpos(0),depos(0)
{
  debuglines.init(200);
  // WARNING: logger constructor should not call any functions that
  //   may output debugging info

//  estr msg = "Starting debug log:\n";
  
//  logfd = open("debug.log",O_WRONLY | O_TRUNC | O_CREAT, OF_READWRITE);
//  write(logfd,msg.c_str(),msg.length());
}

elogger::~elogger()
{
//  close(logfd);
}

/*
void elogger::setOutput(void (*aoutput_func)(estr msg))
{
  output_func = aoutput_func;
}
*/


void elogger::showOutput(const CDebugLine& dline)
{
//  if (logtofile){
//    int fd = open("debug.log",O_WRONLY | O_APPEND);
//    msg += "\n";
//    write(logfd,msg.c_str(),msg.length());
//    close(fd); 
//  }  
//  if (onOutput.isSet())
//    onOutput.call(msg);
//  else
  cerr << dline.text() << endl;
}

void elogger::error(const estr& file,const estr& function,int line,const estr& msg,const estr& module)
{
  debug(6,file,function,line,"!error!: "+msg,module);
}

inline estr getname(const estr& filename)
{
  estr str=filename;
  int i=str.find(".");
  if (i==-1) return(str);
  return(str.substr(0,i));
}

//bool elogger::inModules(int mlevel,const estr& _modules,const estr& _function,const estr& _file)
bool elogger::inModules(const CDebugLine& dline)
{
  if (dline.level>=level || modules.findkey("all")!=-1 && dline.level>=modules["all"]) return(true);

  int i;
  estrarray _marr(dline.module);
  for (i=0; i<_marr.size(); ++i){
    if (modules.findkey(_marr[i])!=-1 && dline.level>=modules[_marr[i]] || modules.findkey(getname(dline.file))!=-1 && dline.level>=modules[getname(dline.file)] || modules.findkey(dline.function)!=-1 && dline.level>=modules[dline.function])
      return(true);
  }
  return(false);
}

#include "ethread.h"

emutex mutexDebug;

void elogger::debug(int mlevel,const estr& file,const estr& function,int line,const estr& message,const estr& module)
{
  mutexDebug.lock();
  debuglines[depos]=CDebugLine(unixtime(),mlevel,file,function,line,message,module);

//  if (inModules(mlevel,module,file,function))
  if (inModules(debuglines[depos]))
    showOutput(debuglines[depos]);

  depos=(depos+1)%debuglines.size();
  if (depos==dbpos)
    dbpos=(dbpos+1)%debuglines.size();

  mutexDebug.unlock();
}

void elogger::print(const estr& msg)
{
//  showOutput(msg);
  debug(3,"","",0,msg);
}

void print(const estr& msg)
{
  getLogger()->print(msg);
}
  
