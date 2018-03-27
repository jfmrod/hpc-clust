#ifndef ELOGGER_H
#define ELOGGER_H


class elogger;
class estr;

elogger *getLogger();

#include "eutils.h"

#include <stdlib.h>

#define  lwarnif( condition, warnmsg  ) { if ( condition ) lwarn(warnmsg); }
#define lerrorif( condition, errormsg ) { if ( condition ) lerror(errormsg); }
#define   ldieif( condition, errormsg ) { if ( condition ) ldie(errormsg); }

#define  lwarnifr( condition, warnmsg  , ret ) { if ( condition ) { lwarn(warnmsg);   return ret; } }
#define lerrorifr( condition, errormsg , ret ) { if ( condition ) { lerror(errormsg); return ret; } }

#ifdef _LNODEBUG
  #define lassert( cond )      {}

  #define ldinfo2( infomsg , function ) {}
  #define ldwarn2( infomsg , module )   {}
  #define lderror2( infomsg , module )  {}
  #define lddie2( infomsg , module )    {}


  #define  ldtodo( todomsg )   {}
  #define   ldtip( tipmsg  )   {}
  #define  ldinfo( infomsg )   {}
  #define  ldwarn( warnmsg )   {}
  #define lderror( errormsg )  {}
  #define   lddie( errormsg )  {}

  #define  ldwarnif( condition, warnmsg  ) {}
  #define lderrorif( condition, warnmsg  ) {}
  #define   lddieif( condition, errormsg ) {}

  #define  ldwarnifr( condition, warnmsg  , ret ) {}
  #define lderrorifr( condition, warnmsg  , ret ) {}
  #define   lddieifr( condition, errormsg , ret ) {}
#else
  #define lassert( cond ) ldieif( cond, estr(#cond))

  #define ldinfo2( infomsg , module )  linfo2(infomsg,estr("devel,")+module)
  #define ldwarn2( infomsg , module )  lwarn2(infomsg,estr("devel,")+module)
  #define lderror2( infomsg , module )  lerror2(infomsg,estr("devel,")+module)
  #define lddie2( infomsg , module )  ldie2(infomsg,estr("devel,")+module)

  #define  ldtodo( tipmsg   )  lwarn2(estr("todo: ")+tipmsg,"devel")
  #define   ldtip( tipmsg   )  lwarn2(estr("tip: ")+tipmsg,"devel")
  #define  ldinfo( infomsg  )  linfo2(infomsg,"devel")
  #define  ldwarn( warnmsg  )  lwarn2(warnmsg,"devel")
  #define lderror( errormsg )  lerror2(errormsg,"devel");
  #define   lddie( errormsg )  ldie2(errormsg,"devel");

  #define   ldtipif( condition, tipmsg   ) { if ( condition ) lwarn2(estr("tip: ")+tipmsg,"devel"); }
  #define  ldwarnif( condition, warnmsg  ) { if ( condition ) lwarn2(estr("warnif: ")+warnmsg,"devel"); }
  #define lderrorif( condition, errormsg ) { if ( condition ) lerror2(estr("errorif: ")+errormsg,"devel"); }
  #define   lddieif( condition, errormsg ) { if ( condition ) ldie2(estr("dieif: ")+errormsg,"devel"); }

  #define   ldtipifr( condition, tipmsg   , ret) { if ( condition ) { lwarn2(estr("tip: ")+tipmsg,"devel"); return(ret); } }
  #define  ldwarnifr( condition, warnmsg  , ret) { if ( condition ) { lwarn2(estr("warnif: ")+warnmsg,"devel"); return(ret); } }
  #define lderrorifr( condition, errormsg , ret) { if ( condition ) { lerror2(estr("errorif: ")+errormsg,"devel"); return(ret); } }
  #define   lddieifr( condition, errormsg , ret) { if ( condition ) { ldie2(estr("dieif: ")+errormsg,"devel"); return(ret); } }
#endif


#ifdef _LOVERRIDE_PRINTF
  #define printf     logger.printf
#endif



#define linfo(infomsg)  getLogger()->debug(2,__FILE__,__PRETTY_FUNCTION__,__LINE__,infomsg,"")
#define lwarn(infomsg)  getLogger()->debug(5,__FILE__,__PRETTY_FUNCTION__,__LINE__,infomsg,"")
#define lerror(infomsg) getLogger()->error(__FILE__,__PRETTY_FUNCTION__,__LINE__,infomsg,"")
#define ldie(infomsg) { getLogger()->error(__FILE__,__PRETTY_FUNCTION__,__LINE__,infomsg,""); exit(-1); }

#define linfo2(infomsg,module)  getLogger()->debug(2,__FILE__,__PRETTY_FUNCTION__,__LINE__,infomsg,module)
#define lwarn2(infomsg,module)  getLogger()->debug(5,__FILE__,__PRETTY_FUNCTION__,__LINE__,infomsg,module)
#define lerror2(infomsg,module) getLogger()->error(__FILE__,__PRETTY_FUNCTION__,__LINE__,infomsg,module)
#define ldie2(infomsg,module) { getLogger()->error(__FILE__,__PRETTY_FUNCTION__,__LINE__,infomsg,module); exit(-1); }

#define lfatal(infomsg) { getLogger()->error(__FILE__,__PRETTY_FUNCTION__,__LINE__,infomsg,""); exit(-1); }

void print(const estr& str);


#include <list>

#include "estr.h"
#include "estrarrayof.h"




using namespace std;

class CDebugLine
{
 public:
  CDebugLine();
  CDebugLine(time_t time,int level,const estr& file,const estr& function,int line,const estr& message,const estr& module);

  time_t time;
  int level;
  estr file;
  estr function;
  int line;
  estr message;
  estr module;

  estr text() const;
};

class elogger
{
 public:
  int logfd;
  estr section;
  estr function;

//  efunc onOutput;

  estrarrayof<int> modules;
  
  bool trace;
  bool logtofile;
   
  elogger();
  ~elogger();

//  void (*output_func)(estr msg);

  int dbpos;
  int depos;
  ebasicarray<CDebugLine> debuglines;
//  list<CDebugLine*> debuglines;
  list<estr> sections;
  list<estr> functions;
  unsigned int maxlines;
  int level;

  bool inModules(const CDebugLine& dline);

  virtual void showOutput(const CDebugLine& dline);
//  void setOutput(void (*aoutput)(estr msg));
  
  void debug(int level,const estr& file,const estr& function,int line,const estr& message,const estr& module="");
  void error(const estr& file,const estr& function,int line,const estr& message,const estr& module="");
  void print(const estr& msg);
};

/*
#include "efunc.h"

class elogger2  : public elogger
{
 public:
  efunc onOutput;
  virtual void showOutput(const estr& msg);
};
*/


#endif

