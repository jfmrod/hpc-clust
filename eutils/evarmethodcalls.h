#ifndef EVARMETHODCALLS_H
#define EVARMETHODCALLS_H

#include "eutils.h"

//////////////////////////////
//
//  Method Call templates 
//


//TODO: Make sure that if arguments cannot be converted to the right types, then do not call the function!!



#include "logger.h"
#include "evar_dec.h"

#define checkArg2(d)  evar a ## d; \
  if ( args[d-1].isNull() ) { lwarn("argument "+estr(d+1)+" is null"); return(evar()); }  \
  if (args[d-1].getTypeid()==typeid(A ## d))\
    a ## d = args[d-1].var; \
  else \
    a ## d = args[d-1].convert(typeid(A ## d)).var;        \
  if (a ## d.isNull() ){ \
    lwarn("argument "+estr(d+1)+" not convertible to type: "+estr(typeid(A ## d).name())+" necessary by function"); \
    return(evar()); \
  }


#define MGET(a,b) a.getarg( (void(*)(b))0x00 )


template <class T,class T2>
evar eclassMethodCall(T* obj,void (T2::*method)(),const evararray& args)
{
  (obj->*method)();
  return(evar());
}
template <class T,class T2,class A1>
evar eclassMethodCall(T* obj,void (T2::*method)(A1),const evararray& args)
{
  if (args.size()>=1){
    checkArg2(1);
    (obj->*method)( MGET(a1,A1) );
    return(evar());
  }
  lwarn("not enough arguments to call function");
  return(evar());
}
template <class T,class T2,class A1,class A2>
evar eclassMethodCall(T* obj,void (T2::*method)(A1,A2),const evararray& args)
{
  if (args.size()>=2){
    checkArg2(1);
    checkArg2(2);
    (obj->*method)( MGET(a1,A1) , MGET(a2,A2) );
    return(evar());
  }
  lwarn("not enough arguments to call function");
  return(evar());
}
template <class T,class T2,class A1,class A2,class A3>
evar eclassMethodCall(T* obj,void (T2::*method)(A1,A2,A3),const evararray& args)
{
  if (args.size()>=3){
    checkArg2(1);
    checkArg2(2);
    checkArg2(3);
    (obj->*method)( MGET(a1,A1) , MGET(a2,A2) , MGET(a3,A3) );
    return(evar());
  }
  lwarn("not enough arguments to call function");
  return(evar());
}
template <class T,class T2,class A1,class A2,class A3,class A4>
evar eclassMethodCall(T* obj,void (T2::*method)(A1,A2,A3,A4),const evararray& args)
{
  if (args.size()>=4){
    checkArg2(1);
    checkArg2(2);
    checkArg2(3);
    checkArg2(4);
    (obj->*method)( MGET(a1,A1) , MGET(a2,A2) , MGET(a3,A3) , MGET(a4,A4) );
    return(evar());
  }
  lwarn("not enough arguments to call function");
  return(evar());
}
template <class T,class T2,class A1,class A2,class A3,class A4,class A5>
evar eclassMethodCall(T* obj,void (T2::*method)(A1,A2,A3,A4,A5),const evararray& args)
{
  if (args.size()>=5){
    checkArg2(1);
    checkArg2(2);
    checkArg2(3);
    checkArg2(4);
    checkArg2(5);
    (obj->*method)( MGET(a1,A1) , MGET(a2,A2) , MGET(a3,A3) , MGET(a4,A4) , MGET(a5,A5) );
    return(evar());
  }
  lwarn("not enough arguments to call function");
  return(evar());
}

template <class T,class T2,class A1,class A2,class A3,class A4,class A5,class A6,class A7>
evar eclassMethodCall(T* obj,void (T2::*method)(A1,A2,A3,A4,A5,A6,A7),const evararray& args)
{
  if (args.size()>=7){
    checkArg2(1);
    checkArg2(2);
    checkArg2(3);
    checkArg2(4);
    checkArg2(5);
    checkArg2(6);
    checkArg2(7);
    (obj->*method)( MGET(a1,A1) , MGET(a2,A2) , MGET(a3,A3) , MGET(a4,A4) , MGET(a5,A5) , MGET(a6,A6) , MGET(a7,A7));
    return(evar());
  }
  lwarn("not enough arguments to call function");
  return(evar());
}

template <class T,class T2>
evar eclassMethodCall(T* obj,void (T2::*method)() const,const evararray& args)
{
  (obj->*method)();
  return(evar());
}
template <class T,class T2,class A1>
evar eclassMethodCall(T* obj,void (T2::*method)(A1) const,const evararray& args)
{
  if (args.size()>=1){
    checkArg2(1);
    (obj->*method)( MGET(a1,A1) );
    return(evar());
  }
  lwarn("not enough arguments to call function");
  return(evar());
}
template <class T,class T2,class A1,class A2>
evar eclassMethodCall(T* obj,void (T2::*method)(A1,A2) const,const evararray& args)
{
  if (args.size()>=2){
    checkArg2(1);
    checkArg2(2);
    (obj->*method)( MGET(a1,A1) , MGET(a2,A2) );
    return(evar());
  }
  lwarn("not enough arguments to call function");
  return(evar());
}
template <class T,class T2,class A1,class A2,class A3>
evar eclassMethodCall(T* obj,void (T2::*method)(A1,A2,A3) const,const evararray& args)
{
  if (args.size()>=3){
    checkArg2(1);
    checkArg2(2);
    checkArg2(3);
    (obj->*method)( MGET(a1,A1) , MGET(a2,A2) , MGET(a3,A3) );
    return(evar());
  }
  lwarn("not enough arguments to call function");
  return(evar());
}
template <class T,class T2,class A1,class A2,class A3,class A4>
evar eclassMethodCall(T* obj,void (T2::*method)(A1,A2,A3,A4) const,const evararray& args)
{
  if (args.size()>=4){
    checkArg2(1);
    checkArg2(2);
    checkArg2(3);
    checkArg2(4);
    (obj->*method)( MGET(a1,A1) , MGET(a2,A2) , MGET(a3,A3) , MGET(a4,A4) );
    return(evar());
  }
  lwarn("not enough arguments to call function");
  return(evar());
}
template <class T,class T2,class A1,class A2,class A3,class A4,class A5>
evar eclassMethodCall(T* obj,void (T2::*method)(A1,A2,A3,A4,A5) const,const evararray& args)
{
  if (args.size()>=5){
    checkArg2(1);
    checkArg2(2);
    checkArg2(3);
    checkArg2(4);
    checkArg2(5);
    (obj->*method)( MGET(a1,A1) , MGET(a2,A2) , MGET(a3,A3) , MGET(a4,A4) , MGET(a5,A5) );
    return(evar());
  }
  lwarn("not enough arguments to call function");
  return(evar());
}

template <class T,class T2,class A1,class A2,class A3,class A4,class A5,class A6,class A7>
evar eclassMethodCall(T* obj,void (T2::*method)(A1,A2,A3,A4,A5,A6,A7) const,const evararray& args)
{
  if (args.size()>=7){
    checkArg2(1);
    checkArg2(2);
    checkArg2(3);
    checkArg2(4);
    checkArg2(5);
    checkArg2(6);
    checkArg2(7);
    (obj->*method)( MGET(a1,A1) , MGET(a2,A2) , MGET(a3,A3) , MGET(a4,A4) , MGET(a5,A5) , MGET(a6,A6) , MGET(a7,A7));
    return(evar());
  }
  lwarn("not enough arguments to call function");
  return(evar());
}


template <class T,class T2,class R>
evar eclassMethodCall(T* obj,R (T2::*method)(),const evararray& args)
{
  return( (obj->*method)() );
}
template <class T,class T2,class R,class A1>
evar eclassMethodCall(T* obj,R (T2::*method)(A1),const evararray& args)
{
  if (args.size()>=1){
    checkArg2(1);
    return((obj->*method)( MGET(a1,A1) ) );
  }
  lwarn("not enough arguments to call function");
  return(evar());
}
template <class T,class T2,class R,class A1,class A2>
evar eclassMethodCall(T* obj,R (T2::*method)(A1,A2),const evararray& args)
{
  if (args.size()>=2){
    checkArg2(1);
    checkArg2(2);
    return((obj->*method)( MGET(a1,A1) , MGET(a2,A2) ) );
  }
  lwarn("not enough arguments to call function");
  return(evar());
}
template <class T,class T2,class R,class A1,class A2,class A3>
evar eclassMethodCall(T* obj,R (T2::*method)(A1,A2,A3),const evararray& args)
{
  if (args.size()>=3){
    checkArg2(1);
    checkArg2(2);
    checkArg2(3);
    return((obj->*method)( MGET(a1,A1) , MGET(a2,A2) , MGET(a3,A3)) );
  }
  lwarn("not enough arguments to call function");
  return(evar());
}
template <class T,class T2,class R,class A1,class A2,class A3,class A4>
evar eclassMethodCall(T* obj,R (T2::*method)(A1,A2,A3,A4),const evararray& args)
{
  if (args.size()>=4){
    checkArg2(1);
    checkArg2(2);
    checkArg2(3);
    checkArg2(4);
    return(new R( (obj->*method)( MGET(a1,A1) , MGET(a2,A2) , MGET(a3,A3) , MGET(a4,A4)) ));
  }
  lwarn("not enough arguments to call function");
  return(evar());
}
template <class T,class T2,class R,class A1,class A2,class A3,class A4,class A5>
evar eclassMethodCall(T* obj,R (T2::*method)(A1,A2,A3,A4,A5),const evararray& args)
{
  if (args.size()>=5){
    checkArg2(1);
    checkArg2(2);
    checkArg2(3);
    checkArg2(4);
    checkArg2(5);
    return( (obj->*method)( MGET(a1,A1) , MGET(a2,A2) , MGET(a3,A3) , MGET(a4,A4), MGET(a5,A5) ));
  }
  lwarn("not enough arguments to call function");
  return(evar());
}





template <class T,class T2,class R>
evar eclassMethodCall(T* obj,R& (T2::*method)(),const evararray& args)
{
  return( (obj->*method)() );
}
template <class T,class T2,class R,class A1>
evar eclassMethodCall(T* obj,R& (T2::*method)(A1),const evararray& args)
{
  if (args.size()>=1){
    checkArg2(1);
    return( (obj->*method)( MGET(a1,A1) ) );
  }
  lwarn("not enough arguments to call function");
  return(evar());
}
template <class T,class T2,class R,class A1,class A2>
evar eclassMethodCall(T* obj,R& (T2::*method)(A1,A2),const evararray& args)
{
  if (args.size()>=2){
    checkArg2(1);
    checkArg2(2);
    return( (obj->*method)( MGET(a1,A1) , MGET(a2,A2) ) );
  }
  lwarn("not enough arguments to call function");
  return(evar());
}
template <class T,class T2,class R,class A1,class A2,class A3>
evar eclassMethodCall(T* obj,R& (T2::*method)(A1,A2,A3),const evararray& args)
{
  if (args.size()>=3){
    checkArg2(1);
    checkArg2(2);
    checkArg2(3);
    return( (obj->*method)( MGET(a1,A1) , MGET(a2,A2) , MGET(a3,A3)) );
  }
  lwarn("not enough arguments to call function");
  return(evar());
}
template <class T,class T2,class R,class A1,class A2,class A3,class A4>
evar eclassMethodCall(T* obj,R& (T2::*method)(A1,A2,A3,A4),const evararray& args)
{
  if (args.size()>=4){
    checkArg2(1);
    checkArg2(2);
    checkArg2(3);
    checkArg2(4);
    return( (obj->*method)( MGET(a1,A1) , MGET(a2,A2) , MGET(a3,A3) , MGET(a4,A4)) );
  }
  lwarn("not enough arguments to call function");
  return(evar());
}

template <class T,class T2>
evar eclassMethodCall(T* obj,evar (T2::*method)() const,const evararray& args)
{
  return((obj->*method)());
}
template <class T,class T2,class A1>
evar eclassMethodCall(T* obj,evar (T2::*method)(A1) const,const evararray& args)
{
  if (args.size()>=1){
    checkArg2(1);
    return((obj->*method)( MGET(a1,A1)));
  }
  lwarn("not enough arguments to call function");
  return(evar());
}
template <class T,class T2,class A1,class A2>
evar eclassMethodCall(T* obj,evar (T2::*method)(A1,A2) const,const evararray& args)
{
  if (args.size()>=2){
    checkArg2(1);
    checkArg2(2);
    return((obj->*method)( MGET(a1,A1) , MGET(a2,A2)));
  }
  lwarn("not enough arguments to call function");
  return(evar());
}
template <class T,class T2,class A1,class A2,class A3>
evar eclassMethodCall(T* obj,evar (T2::*method)(A1,A2,A3) const,const evararray& args)
{
  if (args.size()>=3){
    checkArg2(1);
    checkArg2(2);
    checkArg2(3);
    return((obj->*method)( MGET(a1,A1) , MGET(a2,A2) , MGET(a3,A3)));
  }
  lwarn("not enough arguments to call function");
  return(evar());
}
template <class T,class T2,class A1,class A2,class A3,class A4>
evar eclassMethodCall(T* obj,evar (T2::*method)(A1,A2,A3,A4) const,const evararray& args)
{
  if (args.size()>=4){
    checkArg2(1);
    checkArg2(2);
    checkArg2(3);
    checkArg2(4);
    return((obj->*method)( MGET(a1,A1) , MGET(a2,A2) , MGET(a3,A3) , MGET(a4,A4) ));
  }
  lwarn("not enough arguments to call function");
  return(evar());
}


template <class T,class T2,class R>
evar eclassMethodCall(T* obj,R (T2::*method)() const,const evararray& args)
{
  return( new R((obj->*method)()) );
}
template <class T,class T2,class R,class A1>
evar eclassMethodCall(T* obj,R (T2::*method)(A1) const,const evararray& args)
{
  if (args.size()>=1){
    checkArg2(1);
    return( new R( (obj->*method)( MGET(a1,A1) ) ));
  }
  lwarn("not enough arguments to call function");
  return(evar());
}
template <class T,class T2,class R,class A1,class A2>
evar eclassMethodCall(T* obj,R (T2::*method)(A1,A2) const,const evararray& args)
{
  if (args.size()>=2){
    checkArg2(1);
    checkArg2(2);
    return(new R( (obj->*method)( MGET(a1,A1) , MGET(a2,A2) ) ));
  }
  lwarn("not enough arguments to call function");
  return(evar());
}
template <class T,class T2,class R,class A1,class A2,class A3>
evar eclassMethodCall(T* obj,R (T2::*method)(A1,A2,A3) const,const evararray& args)
{
  if (args.size()>=3){
    checkArg2(1);
    checkArg2(2);
    checkArg2(3);
    return( new R((obj->*method)( MGET(a1,A1) , MGET(a2,A2) , MGET(a3,A3)) ));
  }
  lwarn("not enough arguments to call function");
  return(evar());
}
template <class T,class T2,class R,class A1,class A2,class A3,class A4>
evar eclassMethodCall(T* obj,R (T2::*method)(A1,A2,A3,A4) const,const evararray& args)
{
  if (args.size()>=4){
    checkArg2(1);
    checkArg2(2);
    checkArg2(3);
    checkArg2(4);
    return( new R((obj->*method)( MGET(a1,A1) , MGET(a2,A2) , MGET(a3,A3) , MGET(a4,A4) )));
  }
  lwarn("not enough arguments to call function");
  return(evar());
}
template <class T,class T2,class R,class A1,class A2,class A3,class A4,class A5>
evar eclassMethodCall(T* obj,R (T2::*method)(A1,A2,A3,A4,A5) const,const evararray& args)
{
  if (args.size()>=5){
    checkArg2(1);
    checkArg2(2);
    checkArg2(3);
    checkArg2(4);
    checkArg2(5);
    return( new R((obj->*method)( MGET(a1,A1) , MGET(a2,A2) , MGET(a3,A3) , MGET(a4,A4) , MGET(a5,A5))));
  }
  lwarn("not enough arguments to call function");
  return(evar());
}





template <class T,class T2,class R>
evar eclassMethodCall(T* obj,R& (T2::*method)() const,const evararray& args)
{
  return( (obj->*method)() );
}
template <class T,class T2,class R,class A1>
evar eclassMethodCall(T* obj,R& (T2::*method)(A1) const,const evararray& args)
{
  if (args.size()>=1){
    checkArg2(1);
    return( (obj->*method)( MGET(a1,A1) ) );
  }
  lwarn("not enough arguments to call function");
  return(evar());
}
template <class T,class T2,class R,class A1,class A2>
evar eclassMethodCall(T* obj,R& (T2::*method)(A1,A2) const,const evararray& args)
{
  if (args.size()>=2){
    checkArg2(1);
    checkArg2(2);
    return( (obj->*method)( MGET(a1,A1) , MGET(a2,A2) ) );
  }
  lwarn("not enough arguments to call function");
  return(evar());
}
template <class T,class T2,class R,class A1,class A2,class A3>
evar eclassMethodCall(T* obj,R& (T2::*method)(A1,A2,A3) const,const evararray& args)
{
  if (args.size()>=3){
    checkArg2(1);
    checkArg2(2);
    checkArg2(3);
    return( (obj->*method)( MGET(a1,A1) , MGET(a2,A2) , MGET(a3,A3)) );
  }
  lwarn("not enough arguments to call function");
  return(evar());
}
template <class T,class T2,class R,class A1,class A2,class A3,class A4>
evar eclassMethodCall(T* obj,R& (T2::*method)(A1,A2,A3,A4) const,const evararray& args)
{
  if (args.size()>=4){
    checkArg2(1);
    checkArg2(2);
    checkArg2(3);
    checkArg2(4);
    return( (obj->*method)( MGET(a1,A1) , MGET(a2,A2) , MGET(a3,A3) , MGET(a4,A4)) );
  }
  lwarn("not enough arguments to call function");
  return(evar());
}
template <class T,class T2,class R,class A1,class A2,class A3,class A4,class A5>
evar eclassMethodCall(T* obj,R& (T2::*method)(A1,A2,A3,A4,A5) const,const evararray& args)
{
  if (args.size()>=5){
    checkArg2(1);
    checkArg2(2);
    checkArg2(3);
    checkArg2(4);
    checkArg2(5);
    return( (obj->*method)( MGET(a1,A1) , MGET(a2,A2) , MGET(a3,A3) , MGET(a4,A4)) , MGET(a5,A5));
  }
  lwarn("not enough arguments to call function");
  return(evar());
}




//////////////////////////////////////////////
//
//  End of method call templates
//

#endif

