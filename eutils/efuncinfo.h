#ifndef EFUNCINFO_H
#define EFUNCINFO_H

#include "eutils.h"

#include "efunc.h"

void efuncinfo(void (*pfunc)(),efunc& func);

template <class A1>
void efuncinfo(void (*pfunc)(A1),efunc& func)
{
  func.fReturn = 0x00;
  func.fArgs.add(&typeid(A1));
}

template <class A1,class A2>
void efuncinfo(void (*pfunc)(A1,A2),efunc& func)
{
  func.fReturn = 0x00;
  func.fArgs.add(&typeid(A1));
  func.fArgs.add(&typeid(A2));
}

template <class A1,class A2,class A3>
void efuncinfo(void (*pfunc)(A1,A2,A3),efunc& func)
{
  func.fReturn = 0x00;
  func.fArgs.add(&typeid(A1));
  func.fArgs.add(&typeid(A2));
  func.fArgs.add(&typeid(A3));
}

template <class A1,class A2,class A3,class A4>
void efuncinfo(void (*pfunc)(A1,A2,A3,A4),efunc& func)
{
  func.fReturn = 0x00;
  func.fArgs.add(&typeid(A1));
  func.fArgs.add(&typeid(A2));
  func.fArgs.add(&typeid(A3));
  func.fArgs.add(&typeid(A4));
}

template <class A1,class A2,class A3,class A4,class A5>
void efuncinfo(void (*pfunc)(A1,A2,A3,A4,A5),efunc& func)
{
  func.fReturn = 0x00;
  func.fArgs.add(&typeid(A1));
  func.fArgs.add(&typeid(A2));
  func.fArgs.add(&typeid(A3));
  func.fArgs.add(&typeid(A4));
  func.fArgs.add(&typeid(A5));
}

template <class A1,class A2,class A3,class A4,class A5,class A6>
void efuncinfo(void (*pfunc)(A1,A2,A3,A4,A5,A6),efunc& func)
{
  func.fReturn = 0x00;
  func.fArgs.add(&typeid(A1));
  func.fArgs.add(&typeid(A2));
  func.fArgs.add(&typeid(A3));
  func.fArgs.add(&typeid(A4));
  func.fArgs.add(&typeid(A5));
  func.fArgs.add(&typeid(A6));
}

template <class A1,class A2,class A3,class A4,class A5,class A6,class A7>
void efuncinfo(void (*pfunc)(A1,A2,A3,A4,A5,A6,A7),efunc& func)
{
  func.fReturn = 0x00;
  func.fArgs.add(&typeid(A1));
  func.fArgs.add(&typeid(A2));
  func.fArgs.add(&typeid(A3));
  func.fArgs.add(&typeid(A4));
  func.fArgs.add(&typeid(A5));
  func.fArgs.add(&typeid(A6));
  func.fArgs.add(&typeid(A7));
}


// Functions with return value


template <class R>
void efuncinfo(R (*pfunc)(),efunc& func)
{
  func.fReturn = &typeid(R); 
}

template <class R,class A1>
void efuncinfo(R (*pfunc)(A1),efunc& func)
{
  func.fReturn = &typeid(R); 
  func.fArgs.add(&typeid(A1));
}

template <class R,class A1,class A2>
void efuncinfo(R (*pfunc)(A1,A2),efunc& func)
{
  func.fReturn = &typeid(R); 
  func.fArgs.add(&typeid(A1));
  func.fArgs.add(&typeid(A2));
}

template <class R,class A1,class A2,class A3>
void efuncinfo(R (*pfunc)(A1,A2,A3),efunc& func)
{
  func.fReturn = &typeid(R); 
  func.fArgs.add(&typeid(A1));
  func.fArgs.add(&typeid(A2));
  func.fArgs.add(&typeid(A3));
}

template <class R,class A1,class A2,class A3,class A4>
void efuncinfo(R (*pfunc)(A1,A2,A3,A4),efunc& func)
{
  func.fReturn = &typeid(R); 
  func.fArgs.add(&typeid(A1));
  func.fArgs.add(&typeid(A2));
  func.fArgs.add(&typeid(A3));
  func.fArgs.add(&typeid(A4));
}

template <class R,class A1,class A2,class A3,class A4,class A5>
void efuncinfo(R (*pfunc)(A1,A2,A3,A4,A5),efunc& func)
{
  func.fReturn = &typeid(R); 
  func.fArgs.add(&typeid(A1));
  func.fArgs.add(&typeid(A2));
  func.fArgs.add(&typeid(A3));
  func.fArgs.add(&typeid(A4));
  func.fArgs.add(&typeid(A5));
}

template <class R,class A1,class A2,class A3,class A4,class A5,class A6>
void efuncinfo(R (*pfunc)(A1,A2,A3,A4,A5,A6),efunc& func)
{
  func.fReturn = &typeid(R); 
  func.fArgs.add(&typeid(A1));
  func.fArgs.add(&typeid(A2));
  func.fArgs.add(&typeid(A3));
  func.fArgs.add(&typeid(A4));
  func.fArgs.add(&typeid(A5));
  func.fArgs.add(&typeid(A6));
}

template <class R,class A1,class A2,class A3,class A4,class A5,class A6,class A7>
void efuncinfo(R (*pfunc)(A1,A2,A3,A4,A5,A6,A7),efunc& func)
{
  func.fReturn = &typeid(R); 
  func.fArgs.add(&typeid(A1));
  func.fArgs.add(&typeid(A2));
  func.fArgs.add(&typeid(A3));
  func.fArgs.add(&typeid(A4));
  func.fArgs.add(&typeid(A5));
  func.fArgs.add(&typeid(A6));
  func.fArgs.add(&typeid(A7));
}

template <class R,class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8>
void efuncinfo(R (*pfunc)(A1,A2,A3,A4,A5,A6,A7,A8),efunc& func)
{
  func.fReturn = &typeid(R); 
  func.fArgs.add(&typeid(A1));
  func.fArgs.add(&typeid(A2));
  func.fArgs.add(&typeid(A3));
  func.fArgs.add(&typeid(A4));
  func.fArgs.add(&typeid(A5));
  func.fArgs.add(&typeid(A6));
  func.fArgs.add(&typeid(A7));
  func.fArgs.add(&typeid(A8));
}

#endif

