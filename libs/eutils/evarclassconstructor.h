#ifndef EVARCLASSCONSTRUCTOR_H
#define EVARCLASSCONSTRUCTOR_H

#include "eutils.h"


#define epregisterClassConstructor(c,args) epregisterClassConstructor2<c>((evar (*)args)&epclassConstructor<c>,#c)

template <class T,class F>
void epregisterClassConstructor2(F func,const estr& classname)
{
  eclass<T> *c;
  GETCLASS(c,T);
  c->create.add(func);
  getClassNames().add(classname,c);
}

#include "evar.h"

template <class T>
evar epclassConstructor()
{
  return((T*)new T);
}

template <class T,class A1>
evar epclassConstructor(A1 a1)
{
  return((T*)new T(a1));
}

template <class T,class A1,class A2>
evar epclassConstructor(A1 a1,A2 a2)
{
  return((T*)new T(a1,a2));
}

template <class T,class A1,class A2,class A3>
evar epclassConstructor(A1 a1,A2 a2,A3 a3)
{
  return((T*)new T(a1,a2,a3));
}

template <class T,class A1,class A2,class A3,class A4>
evar epclassConstructor(A1 a1,A2 a2,A3 a3,A4 a4)
{
  return((T*)new T(a1,a2,a3,a4));
}

template <class T,class A1,class A2,class A3,class A4,class A5>
evar epclassConstructor(A1 a1,A2 a2,A3 a3,A4 a4,A5 a5)
{
  return((T*)new T(a1,a2,a3,a4,a5));
}

#endif

