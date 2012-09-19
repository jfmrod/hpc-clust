#ifndef EFUNCALLS_H
#define EFUNCALLS_H

#include "eutils.h"

#include "logger.h"
#include "evar_dec.h"
#include "evararray.h"

//#define checkArg(d)  evar a ## d; \
  if ( arr[d-1].isNull() ) { lwarn("argument "+estr(d+1)+" is null"); return(evar()); }  \
  if (arr[d-1].getTypeid()==typeid(A ## d)){ \
    ldinfo("no conversion necessary for arg: "+estr(d)+" of type: "+typeid(A ## d).name()); \
    a ## d.set(arr[d-1]); \
  } else { \
    ldinfo("converting arg: "+estr(d)+" to: "+typeid(A ## d).name()); \
    a ## d.set(arr[d-1].convert(typeid(A ## d)));        \
  }; \
  if (a ## d.isNull() ){ \
    lwarn("argument "+estr(d+1)+" not convertible to type: "+estr(typeid(A ## d).name())+" necessary by function"); \
    return(evar()); \
  }

#define checkArg(d)  evar a ## d; \
  if ( arr[d-1].isNull() ) { lwarn("argument "+estr(d+1)+" is null"); return(evar()); }  \
  if (arr[d-1].getTypeid()==typeid(A ## d)){ \
    a ## d.set(arr[d-1]); \
  } else { \
    a ## d.set(arr[d-1].convert(typeid(A ## d)));        \
  }; \
  if (a ## d.isNull() ){ \
    lwarn("argument "+estr(d+1)+" not convertible to type: "+estr(typeid(A ## d).name())+" necessary by function"); \
    return(evar()); \
  }

evar efuncall(void (*pfunc)(),evararray& arr);

#define FGET(a,b) a.getarg( (void(*)(b))0x00 )

/*
template <class A1,class A2>
inline evar efuncall(void (*pfunc)(A1),A2 arg)
{
  if (typeid(A1)==typeid(A2))
    { (*pfunc)(arg); return(evar()); }
  
  evar a1(arg);
  if (typeid(A1)==a1.getClassTypeid())
    { (*pfunc)(FGET(a1,A1)); return(evar()); }
  
  a1=a1.convert<A1>().var;
  if (a1.isNull())
    { lwarn("argument 1 not convertible to type: "+estr(typeid(A1).name())+" necessary by function"); return(evar()); }

  (*pfunc)(FGET(a1,A1));
  return(evar()); 
}
*/

template <class A1>
evar efuncall(void (*pfunc)(A1),evararray& arr)
{
  if (arr.size()>=1){
    checkArg(1);
    (*pfunc)(FGET(a1,A1));
    return(evar());
  }
  lwarn("not enough arguments to call function");
  return(evar());
}

template <class A1,class A2>
evar efuncall(void (*pfunc)(A1,A2),evararray& arr)
{
  if (arr.size()>=2){
    checkArg(1);
    checkArg(2);
    (*pfunc)(FGET(a1,A1),FGET(a2,A2));
    return(evar());
  }
  lwarn("not enough arguments to call function");
  return(evar());
}


template <class A1,class A2,class A3>
evar efuncall(void (*pfunc)(A1,A2,A3),evararray& arr)
{
  if (arr.size()>=3){
    checkArg(1);
    checkArg(2);
    checkArg(3);
    (*pfunc)(FGET(a1,A1),FGET(a2,A2),FGET(a3,A3));
    return(evar());
  }
  lwarn("not enough arguments to call function");
  return(evar());
}

template <class A1,class A2,class A3,class A4>
evar efuncall(void (*pfunc)(A1,A2,A3,A4),evararray& arr)
{
  if (arr.size()>=4){
    checkArg(1);
    checkArg(2);
    checkArg(3);
    checkArg(4);
    (*pfunc)(FGET(a1,A1),FGET(a2,A2),FGET(a3,A3),FGET(a4,A4));
    return(evar());
  }
  lwarn("not enough arguments to call function");
  return(evar());
}

template <class A1,class A2,class A3,class A4,class A5>
evar efuncall(void (*pfunc)(A1,A2,A3,A4,A5),evararray& arr)
{
  if (arr.size()>=5){
    checkArg(1);
    checkArg(2);
    checkArg(3);
    checkArg(4);
    checkArg(5);
    (*pfunc)(FGET(a1,A1),FGET(a2,A2),FGET(a3,A3),FGET(a4,A4),FGET(a5,A5));
    return(evar());
  }
  lwarn("not enough arguments to call function");
  return(evar());
}

template <class A1,class A2,class A3,class A4,class A5,class A6>
evar efuncall(void (*pfunc)(A1,A2,A3,A4,A5,A6),evararray& arr)
{
  if (arr.size()>=6){
    checkArg(1);
    checkArg(2);
    checkArg(3);
    checkArg(4);
    checkArg(5);
    checkArg(6);
    (*pfunc)(FGET(a1,A1),FGET(a2,A2),FGET(a3,A3),FGET(a4,A4),FGET(a5,A5),FGET(a6,A6));
    return(evar());
  }
  lwarn("not enough arguments to call function");
  return(evar());
}

template <class A1,class A2,class A3,class A4,class A5,class A6,class A7>
evar efuncall(void (*pfunc)(A1,A2,A3,A4,A5,A6,A7),evararray& arr)
{
  if (arr.size()>=7){
    checkArg(1);
    checkArg(2);
    checkArg(3);
    checkArg(4);
    checkArg(5);
    checkArg(6);
    checkArg(7);
    (*pfunc)(FGET(a1,A1),FGET(a2,A2),FGET(a3,A3),FGET(a4,A4),FGET(a5,A5),FGET(a6,A6),FGET(a7,A7));
    return(evar());
  }
  lwarn("not enough arguments to call function");
  return(evar());
}

template <class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8>
evar efuncall(void (*pfunc)(A1,A2,A3,A4,A5,A6,A7,A8),evararray& arr)
{
  if (arr.size()>=7){
    checkArg(1);
    checkArg(2);
    checkArg(3);
    checkArg(4);
    checkArg(5);
    checkArg(6);
    checkArg(7);
    checkArg(8);
    (*pfunc)(FGET(a1,A1),FGET(a2,A2),FGET(a3,A3),FGET(a4,A4),FGET(a5,A5),FGET(a6,A6),FGET(a7,A7),FGET(a8,A8));
    return(evar());
  }
  lwarn("not enough arguments to call function");
  return(evar());
}


// Functions with return value
template <class R>
evar efuncall(R& (*pfunc)(),evararray& arr)
{
  return((*pfunc)());
}

template <class R,class A1>
evar efuncall(R& (*pfunc)(A1),evararray& arr)
{
  if (arr.size()>=1){
    checkArg(1);
    return((*pfunc)(FGET(a1,A1)));
  }
  lwarn("not enough arguments to call function");
  return(evar());
}

template <class R,class A1,class A2>
evar efuncall(R& (*pfunc)(A1,A2),evararray& arr)
{
  if (arr.size()>=2){
    checkArg(1);
    checkArg(2);
    return((*pfunc)(FGET(a1,A1),FGET(a2,A2)));
  }
  lwarn("not enough arguments to call function");
  return(evar());
}

template <class R,class A1,class A2,class A3>
evar efuncall(R& (*pfunc)(A1,A2,A3),evararray& arr)
{
  if (arr.size()>=3){
    checkArg(1);
    checkArg(2);
    checkArg(3);
    return((*pfunc)(FGET(a1,A1),FGET(a2,A2),FGET(a3,A3)));
  }
  lwarn("not enough arguments to call function");
  return(evar());
}

template <class R,class A1,class A2,class A3,class A4>
evar efuncall(R& (*pfunc)(A1,A2,A3,A4),evararray& arr)
{
  if (arr.size()>=4){
    checkArg(1);
    checkArg(2);
    checkArg(3);
    checkArg(4);
    return((*pfunc)(FGET(a1,A1),FGET(a2,A2),FGET(a3,A3),FGET(a4,A4)));
  }
  lwarn("not enough arguments to call function");
  return(evar());
}

evar efuncall(evar (*pfunc)(),evararray& arr);

template <class A1>
evar efuncall(evar (*pfunc)(A1),evararray& arr)
{
  if (arr.size()>=1){
    checkArg(1);
    return((*pfunc)(FGET(a1,A1)));
  }
  lwarn("not enough arguments to call function");
  return(evar());
}

template <class A1,class A2>
evar efuncall(evar (*pfunc)(A1,A2),evararray& arr)
{
  if (arr.size()>=2){
    checkArg(1);
    checkArg(2);
    return((*pfunc)(FGET(a1,A1),FGET(a2,A2)));
  }
  lwarn("not enough arguments to call function");
  return(evar());
}

template <class A1,class A2,class A3>
evar efuncall(evar (*pfunc)(A1,A2,A3),evararray& arr)
{
  if (arr.size()>=3){
    checkArg(1);
    checkArg(2);
    checkArg(3);
    return((*pfunc)(FGET(a1,A1),FGET(a2,A2),FGET(a3,A3)));
  }
  lwarn("not enough arguments to call function");
  return(evar());
}

template <class A1,class A2,class A3,class A4>
evar efuncall(evar (*pfunc)(A1,A2,A3,A4),evararray& arr)
{
  if (arr.size()>=4){
    checkArg(1);
    checkArg(2);
    checkArg(3);
    checkArg(4);
    return((*pfunc)(FGET(a1,A1),FGET(a2,A2),FGET(a3,A3),FGET(a4,A4)));
  }
  lwarn("not enough arguments to call function");
  return(evar());
}

template <class A1,class A2,class A3,class A4,class A5>
evar efuncall(evar (*pfunc)(A1,A2,A3,A4,A5),evararray& arr)
{
  if (arr.size()>=4){
    checkArg(1);
    checkArg(2);
    checkArg(3);
    checkArg(4);
    checkArg(5);
    return((*pfunc)(FGET(a1,A1),FGET(a2,A2),FGET(a3,A3),FGET(a4,A4),FGET(a5,A5)));
  }
  lwarn("not enough arguments to call function");
  return(evar());
}



template <class R>
evar efuncall(R (*pfunc)(),evararray& arr)
{
  return(new R((*pfunc)()));
}

template <class R,class A1>
evar efuncall(R (*pfunc)(A1),evararray& arr)
{
  if (arr.size()>=1){
    checkArg(1);
    return(new R((*pfunc)(FGET(a1,A1))));
  }
  lwarn("not enough arguments to call function");
  return(evar());
}

template <class R,class A1,class A2>
evar efuncall(R (*pfunc)(A1,A2),evararray& arr)
{
  if (arr.size()>=2){
    checkArg(1);
    checkArg(2);
    return(new R((*pfunc)(FGET(a1,A1),FGET(a2,A2))));
  }
  lwarn("not enough arguments to call function");
  return(evar());
}

template <class R,class A1,class A2,class A3>
evar efuncall(R (*pfunc)(A1,A2,A3),evararray& arr)
{
  if (arr.size()>=3){
    checkArg(1);
    checkArg(2);
    checkArg(3);
    return(new R((*pfunc)(FGET(a1,A1),FGET(a2,A2),FGET(a3,A3))));
  }
  lwarn("not enough arguments to call function");
  return(evar());
}

template <class R,class A1,class A2,class A3,class A4>
evar efuncall(R (*pfunc)(A1,A2,A3,A4),evararray& arr)
{
  if (arr.size()>=4){
    checkArg(1);
    checkArg(2);
    checkArg(3);
    checkArg(4);
    return(new R((*pfunc)(FGET(a1,A1),FGET(a2,A2),FGET(a3,A3),FGET(a4,A4))));
  }
  lwarn("not enough arguments to call function");
  return(evar());
}

template <class R,class A1,class A2,class A3,class A4,class A5>
evar efuncall(R (*pfunc)(A1,A2,A3,A4,A5),evararray& arr)
{
  if (arr.size()>=5){
    checkArg(1);
    checkArg(2);
    checkArg(3);
    checkArg(4);
    checkArg(5);
    return(new R((*pfunc)(FGET(a1,A1),FGET(a2,A2),FGET(a3,A3),FGET(a4,A4),FGET(a5,A5))));
  }
  lwarn("not enough arguments to call function: "+estr(arr.size()));
  return(evar());
}

template <class R,class A1,class A2,class A3,class A4,class A5,class A6>
evar efuncall(R (*pfunc)(A1,A2,A3,A4,A5,A6),evararray& arr)
{
  if (arr.size()>=6){
    checkArg(1);
    checkArg(2);
    checkArg(3);
    checkArg(4);
    checkArg(5);
    checkArg(6);
    return(new R((*pfunc)(FGET(a1,A1),FGET(a2,A2),FGET(a3,A3),FGET(a4,A4),FGET(a5,A5),FGET(a6,A6))));
  }
  lwarn("not enough arguments to call function: "+estr(arr.size()));
  return(evar());
}

template <class R,class A1,class A2,class A3,class A4,class A5,class A6,class A7>
evar efuncall(R (*pfunc)(A1,A2,A3,A4,A5,A6,A7),evararray& arr)
{
  if (arr.size()>=7){
    checkArg(1);
    checkArg(2);
    checkArg(3);
    checkArg(4);
    checkArg(5);
    checkArg(6);
    checkArg(7);
    return(new R((*pfunc)(FGET(a1,A1),FGET(a2,A2),FGET(a3,A3),FGET(a4,A4),FGET(a5,A5),FGET(a6,A6),FGET(a7,A7))));
  }
  lwarn("not enough arguments to call function: "+estr(arr.size()));
  return(evar());
}

template <class R,class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8>
evar efuncall(R (*pfunc)(A1,A2,A3,A4,A5,A6,A7,A8),evararray& arr)
{
  if (arr.size()>=7){
    checkArg(1);
    checkArg(2);
    checkArg(3);
    checkArg(4);
    checkArg(5);
    checkArg(6);
    checkArg(7);
    checkArg(8);
    return(new R((*pfunc)(FGET(a1,A1),FGET(a2,A2),FGET(a3,A3),FGET(a4,A4),FGET(a5,A5),FGET(a6,A6),FGET(a7,A7),FGET(a8,A8))));
  }
  lwarn("not enough arguments to call function: "+estr(arr.size()));
  return(evar());
}

#endif

