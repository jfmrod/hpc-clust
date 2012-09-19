#ifndef ECLASS_H
#define ECLASS_H

#include "evarclass_dec.h"
#include "earray.h"
#include "estr.h"

template <class T,class T2>
void epregisterClassInheritance2()
{
  eclass<T> *c;
  eclass<T2> *c2;
  GETCLASS(c,T);
  GETCLASS(c2,T2);
  c->parents.add(typeid(T2).name(),c2);
}

#include "evarclassmethod.h"
#include "evarclassproperty.h"

template <class T,class P>
void epregisterClassProperty2(P prop,const estr& propname,const estr& info="")
{
  eclass<T> *c;
  GETCLASS(c,T);
  c->properties.addref(propname,new eclassProperty<T,P>(prop,propname,info)); 
}

template <class T,class M>
void epregisterClassMethod3(M method,const estr& methodname,const evararray& args=evararray(),const estr& info="")
{
  eclass<T> *c;
  GETCLASS(c,T);

  if (!c->methods.exists(methodname))
    c->methods.addref(methodname,new ebasicarray<eclassMethodBase*>);
  c->methods[methodname].add(new eclassMethod<T,M>(method,methodname,args,info)); 
//  c->methods.addref(methodname,new eclassMethod<T,M>(method,methodname,args,info));
}

template <class T,class F>
void epregisterFuncMethod2(F func,const estr& methodname,const evararray& args=evararray(),const estr& info="")
{
  eclass<T> *c;
  GETCLASS(c,T);
  if (!c->methods.exists(methodname))
    c->methods.addref(methodname,new ebasicarray<eclassMethodBase*>);
  c->methods[methodname].add(new eclassMethodFunc<T,F>(func,methodname,args,info)); 
//  c->methods.addref(methodname,new eclassMethodFunc<T,F>(func,methodname,args,info));
}



template <class T>
ostream& eclassCout(ostream& stream, const T& val)
{
  return(stream<<(const T&)val);
}

template <class T>
void epregisterClassCout2(ostream& (*func)(ostream&, const T&))
{
  eclass<T> *c;
  GETCLASS(c,T);
  c->pfcout=func;
}

#include "evarclassconstructor.h"

template <class T>
void eclassAssign(T& to, const T& from) { to=from; }

template <class T>
void epregisterClassAssign2(void (*func)(T&, const T&))
{
  eclass<T> *c;
  GETCLASS(c,T);
  c->pfassign=func;
}

template <class T>
T* eclassCopy(const T& from) { return(new T(from)); }

template <class T>
void epregisterClassCopy2(T* (*func)(const T&))
{
  eclass<T> *c;
  GETCLASS(c,T);
  c->pfcopy=func;
}

template <class T>
void eclassSerialMethod(const T& obj,estr& data) { obj.serial(data); }
template <class T>
void eclassSerialType(const T& obj,estr& data)
{
  data.reserve(data.len()+sizeof(T)+1);
  *((T*)&data[data.len()])=obj;
  data._strlen+=sizeof(T);
  data._str[data._strlen]=0x00;
}

template <class T>
void epregisterClassSerial2(void (*func)(const T&,estr&))
{
  eclass<T> *c;
  GETCLASS(c,T);
  c->pfserial=func;
}

template <class T>
int eclassUnserialMethod(T& obj,const estr& data,int i) { return(obj.unserial(data,i)); }

template <class T>
int eclassUnserialType(T& val,const estr& data,int i)
{
  if (i+sizeof(T)>data.len()) return(-1);
  val = *((T*)(data._str+i));
  return(i+sizeof(T));
}

template <class T>
void epregisterClassUnserial2(int (*func)(T&,const estr&,int i))
{
  eclass<T> *c;
  GETCLASS(c,T);
  c->pfunserial=func;
}



/*
#define epregisterClassSerial(c) epregisterFunc3(epclassSerial<c>,typeid(c).name()+estr("serial"));

template <class T>
void epclassSerial(const T& var,estr& data)
{
  data.reserve(data.len()+sizeof(T));
  *((T*)&data[data.len()])=var;
  data._strlen+=sizeof(T);
}

#define epregisterClassSerial2(c) epregisterFunc3(epclassSerial2<c>,typeid(c).name()+estr("serial"));
template <class T>
void epclassSerial2(const T& var,estr& data)
{
  var.serial(data);
}

#define epregisterClassUnserial(c) epregisterFunc3(epclassUnserial<c>,typeid(c).name()+estr("unserial"));
template <class T>
int epclassUnserial(T &var,estr& data,int i)
{
  var = *((T*)&data[i]);
  return(i+sizeof(T));
}
#define epregisterClassUnserial2(c) epregisterFunc3(epclassUnserial2<c>,typeid(c).name()+estr("unserial"));
template <class T>
int epclassUnserial2(T &var,estr& data,int i)
{
  return(var.unserial(data,i));
}

*/

#endif

