#ifndef EVARMETHODINFO_H
#define EVARMETHODINFO_H

#include "eutils.h"
#include "evar_dec.h"

//////////////////////////////
//
//  Method Info templates 
//



///////////////////////////////////////////////////////////////////////////////
//
//     methods with no return value
//

template <class T>
void eclassMethodInfo(void (T::*pmethod)(),eclassMethodBase& method)
{
  method.fReturn=0x00;
}
template <class T,class A1>
void eclassMethodInfo(void (T::*pmethod)(A1),eclassMethodBase& method)
{
  method.fReturn=0x00;
  method.fArgs.add( &typeid(A1) ); 
}
template <class T,class A1,class A2>
void eclassMethodInfo(void (T::*pmethod)(A1,A2),eclassMethodBase& method)
{
  method.fReturn=0x00;
  method.fArgs.add( &typeid(A1) ); 
  method.fArgs.add( &typeid(A2) ); 
}
template <class T,class A1,class A2,class A3>
void eclassMethodInfo(void (T::*pmethod)(A1,A2,A3),eclassMethodBase& method)
{
  method.fReturn=0x00;
  method.fArgs.add( &typeid(A1) ); 
  method.fArgs.add( &typeid(A2) ); 
  method.fArgs.add( &typeid(A3) ); 
}
template <class T,class A1,class A2,class A3,class A4>
void eclassMethodInfo(void (T::*pmethod)(A1,A2,A3,A4),eclassMethodBase& method)
{
  method.fReturn=0x00;
  method.fArgs.add( &typeid(A1) ); 
  method.fArgs.add( &typeid(A2) ); 
  method.fArgs.add( &typeid(A3) ); 
  method.fArgs.add( &typeid(A4) ); 
}
template <class T,class A1,class A2,class A3,class A4,class A5>
void eclassMethodInfo(void (T::*pmethod)(A1,A2,A3,A4,A5),eclassMethodBase& method)
{
  method.fReturn=0x00;
  method.fArgs.add( &typeid(A1) ); 
  method.fArgs.add( &typeid(A2) ); 
  method.fArgs.add( &typeid(A3) ); 
  method.fArgs.add( &typeid(A4) ); 
  method.fArgs.add( &typeid(A5) ); 
}

template <class T,class A1,class A2,class A3,class A4,class A5,class A6,class A7>
void eclassMethodInfo(void (T::*pmethod)(A1,A2,A3,A4,A5,A6,A7),eclassMethodBase& method)
{
  method.fReturn=0x00;
  method.fArgs.add( &typeid(A1) ); 
  method.fArgs.add( &typeid(A2) ); 
  method.fArgs.add( &typeid(A3) ); 
  method.fArgs.add( &typeid(A4) ); 
  method.fArgs.add( &typeid(A5) ); 
  method.fArgs.add( &typeid(A6) ); 
  method.fArgs.add( &typeid(A7) ); 
}



///////////////////////////////////////////////////////////////////////////////
//
//     methods with return value
//

template <class T,class R>
void eclassMethodInfo(R (T::*pmethod)(),eclassMethodBase& method)
{
  method.fReturn=&typeid(R);
}
template <class T,class R,class A1>
void eclassMethodInfo(R (T::*pmethod)(A1),eclassMethodBase& method)
{
  method.fReturn=&typeid(R);
  method.fArgs.add( &typeid(A1) ); 
}
template <class T,class R,class A1,class A2>
void eclassMethodInfo(R (T::*pmethod)(A1,A2),eclassMethodBase& method)
{
  method.fReturn=&typeid(R);
  method.fArgs.add( &typeid(A1) ); 
  method.fArgs.add( &typeid(A2) ); 
}
template <class T,class R,class A1,class A2,class A3>
void eclassMethodInfo(R (T::*pmethod)(A1,A2,A3),eclassMethodBase& method)
{
  method.fReturn=&typeid(R);
  method.fArgs.add( &typeid(A1) ); 
  method.fArgs.add( &typeid(A2) ); 
  method.fArgs.add( &typeid(A3) ); 
}
template <class T,class R,class A1,class A2,class A3,class A4>
void eclassMethodInfo(R (T::*pmethod)(A1,A2,A3,A4),eclassMethodBase& method)
{
  method.fReturn=&typeid(R);
  method.fArgs.add( &typeid(A1) ); 
  method.fArgs.add( &typeid(A2) ); 
  method.fArgs.add( &typeid(A3) ); 
  method.fArgs.add( &typeid(A4) ); 
}
template <class T,class R,class A1,class A2,class A3,class A4,class A5>
void eclassMethodInfo(R (T::*pmethod)(A1,A2,A3,A4,A5),eclassMethodBase& method)
{
  method.fReturn=&typeid(R);
  method.fArgs.add( &typeid(A1) ); 
  method.fArgs.add( &typeid(A2) ); 
  method.fArgs.add( &typeid(A3) ); 
  method.fArgs.add( &typeid(A4) ); 
  method.fArgs.add( &typeid(A5) ); 
}



//////////////////////////////////////////////////////////////////
//
//    return by reference methods
//

template <class T,class R>
void eclassMethodInfo(R& (T::*pmethod)(),eclassMethodBase& method)
{
  method.fReturn=&typeid(R);
}
template <class T,class R,class A1>
void eclassMethodInfo(R& (T::*pmethod)(A1),eclassMethodBase& method)
{
  method.fReturn=&typeid(R);
  method.fArgs.add( &typeid(A1) ); 
}
template <class T,class R,class A1,class A2>
void eclassMethodInfo(R& (T::*pmethod)(A1,A2),eclassMethodBase& method)
{
  method.fReturn=&typeid(R);
  method.fArgs.add( &typeid(A1) ); 
  method.fArgs.add( &typeid(A2) ); 
}
template <class T,class R,class A1,class A2,class A3>
void eclassMethodInfo(R& (T::*pmethod)(A1,A2,A3),eclassMethodBase& method)
{
  method.fReturn=&typeid(R);
  method.fArgs.add( &typeid(A1) ); 
  method.fArgs.add( &typeid(A2) ); 
  method.fArgs.add( &typeid(A3) ); 
}
template <class T,class R,class A1,class A2,class A3,class A4>
void eclassMethodInfo(R& (T::*pmethod)(A1,A2,A3,A4),eclassMethodBase& method)
{
  method.fReturn=&typeid(R);
  method.fArgs.add( &typeid(A1) ); 
  method.fArgs.add( &typeid(A2) ); 
  method.fArgs.add( &typeid(A3) ); 
  method.fArgs.add( &typeid(A4) ); 
}

/////////////////////////////////////////////////////////////////
//
//     const methods

template <class T>
void eclassMethodInfo(void (T::*pmethod)() const,eclassMethodBase& method)
{
  method.fReturn=0x00;
}
template <class T,class A1>
void eclassMethodInfo(void (T::*pmethod)(A1) const,eclassMethodBase& method)
{
  method.fReturn=0x00;
  method.fArgs.add( &typeid(A1) ); 
}
template <class T,class A1,class A2>
void eclassMethodInfo(void (T::*pmethod)(A1,A2) const,eclassMethodBase& method)
{
  method.fReturn=0x00;
  method.fArgs.add( &typeid(A1) ); 
  method.fArgs.add( &typeid(A2) ); 
}
template <class T,class A1,class A2,class A3>
void eclassMethodInfo(void (T::*pmethod)(A1,A2,A3) const,eclassMethodBase& method)
{
  method.fReturn=0x00;
  method.fArgs.add( &typeid(A1) ); 
  method.fArgs.add( &typeid(A2) ); 
  method.fArgs.add( &typeid(A3) ); 
}
template <class T,class A1,class A2,class A3,class A4>
void eclassMethodInfo(void (T::*pmethod)(A1,A2,A3,A4) const,eclassMethodBase& method)
{
  method.fReturn=0x00;
  method.fArgs.add( &typeid(A1) ); 
  method.fArgs.add( &typeid(A2) ); 
  method.fArgs.add( &typeid(A3) ); 
  method.fArgs.add( &typeid(A4) ); 
}
template <class T,class A1,class A2,class A3,class A4,class A5>
void eclassMethodInfo(void (T::*pmethod)(A1,A2,A3,A4,A5) const,eclassMethodBase& method)
{
  method.fReturn=0x00;
  method.fArgs.add( &typeid(A1) ); 
  method.fArgs.add( &typeid(A2) ); 
  method.fArgs.add( &typeid(A3) ); 
  method.fArgs.add( &typeid(A4) ); 
  method.fArgs.add( &typeid(A5) ); 
}

template <class T,class A1,class A2,class A3,class A4,class A5,class A6,class A7>
void eclassMethodInfo(void (T::*pmethod)(A1,A2,A3,A4,A5,A6,A7) const,eclassMethodBase& method)
{
  method.fReturn=0x00;
  method.fArgs.add( &typeid(A1) ); 
  method.fArgs.add( &typeid(A2) ); 
  method.fArgs.add( &typeid(A3) ); 
  method.fArgs.add( &typeid(A4) ); 
  method.fArgs.add( &typeid(A5) ); 
  method.fArgs.add( &typeid(A6) ); 
  method.fArgs.add( &typeid(A7) ); 
}




template <class T,class R>
void eclassMethodInfo(R (T::*pmethod)() const,eclassMethodBase& method)
{
  method.fReturn=&typeid(R);
}
template <class T,class R,class A1>
void eclassMethodInfo(R (T::*pmethod)(A1) const,eclassMethodBase& method)
{
  method.fReturn=&typeid(R);
  method.fArgs.add( &typeid(A1) ); 
}
template <class T,class R,class A1,class A2>
void eclassMethodInfo(R (T::*pmethod)(A1,A2) const,eclassMethodBase& method)
{
  method.fReturn=&typeid(R);
  method.fArgs.add( &typeid(A1) ); 
  method.fArgs.add( &typeid(A2) ); 
}
template <class T,class R,class A1,class A2,class A3>
void eclassMethodInfo(R (T::*pmethod)(A1,A2,A3) const,eclassMethodBase& method)
{
  method.fReturn=&typeid(R);
  method.fArgs.add( &typeid(A1) ); 
  method.fArgs.add( &typeid(A2) ); 
  method.fArgs.add( &typeid(A3) ); 
}
template <class T,class R,class A1,class A2,class A3,class A4>
void eclassMethodInfo(R (T::*pmethod)(A1,A2,A3,A4) const,eclassMethodBase& method)
{
  method.fReturn=&typeid(R);
  method.fArgs.add( &typeid(A1) ); 
  method.fArgs.add( &typeid(A2) ); 
  method.fArgs.add( &typeid(A3) ); 
  method.fArgs.add( &typeid(A4) ); 
}
template <class T,class R,class A1,class A2,class A3,class A4,class A5>
void eclassMethodInfo(R (T::*pmethod)(A1,A2,A3,A4,A5) const,eclassMethodBase& method)
{
  method.fReturn=&typeid(R);
  method.fArgs.add( &typeid(A1) ); 
  method.fArgs.add( &typeid(A2) ); 
  method.fArgs.add( &typeid(A3) ); 
  method.fArgs.add( &typeid(A4) ); 
  method.fArgs.add( &typeid(A5) ); 
}

template <class T,class R>
void eclassMethodInfo(R& (T::*pmethod)() const,eclassMethodBase& method)
{
  method.fReturn=&typeid(R);
}
template <class T,class R,class A1>
void eclassMethodInfo(R& (T::*pmethod)(A1) const,eclassMethodBase& method)
{
  method.fReturn=&typeid(R);
  method.fArgs.add( &typeid(A1) ); 
}
template <class T,class R,class A1,class A2>
void eclassMethodInfo(R& (T::*pmethod)(A1,A2) const,eclassMethodBase& method)
{
  method.fReturn=&typeid(R);
  method.fArgs.add( &typeid(A1) ); 
  method.fArgs.add( &typeid(A2) ); 
}
template <class T,class R,class A1,class A2,class A3>
void eclassMethodInfo(R& (T::*pmethod)(A1,A2,A3) const,eclassMethodBase& method)
{
  method.fReturn=&typeid(R);
  method.fArgs.add( &typeid(A1) ); 
  method.fArgs.add( &typeid(A2) ); 
  method.fArgs.add( &typeid(A3) ); 
}
template <class T,class R,class A1,class A2,class A3,class A4>
void eclassMethodInfo(R& (T::*pmethod)(A1,A2,A3,A4) const,eclassMethodBase& method)
{
  method.fReturn=&typeid(R);
  method.fArgs.add( &typeid(A1) ); 
  method.fArgs.add( &typeid(A2) ); 
  method.fArgs.add( &typeid(A3) ); 
  method.fArgs.add( &typeid(A4) ); 
}
template <class T,class R,class A1,class A2,class A3,class A4,class A5>
void eclassMethodInfo(R& (T::*pmethod)(A1,A2,A3,A4,A5) const,eclassMethodBase& method)
{
  method.fReturn=&typeid(R);
  method.fArgs.add( &typeid(A1) ); 
  method.fArgs.add( &typeid(A2) ); 
  method.fArgs.add( &typeid(A3) ); 
  method.fArgs.add( &typeid(A4) ); 
  method.fArgs.add( &typeid(A5) ); 
}


//////////////////////////////////////////////
//
//  End of method call templates
//

#endif

