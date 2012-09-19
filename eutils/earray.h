#ifndef EARRAY_H
#define EARRAY_H

#include "eutils.h"
#include "ebasicarray_dec.h"
#include "earray_dec.h"
#include "evar_dec.h"


#include "efunc_dec.h"
#include "estr.h"
#include "evarclass_dec.h"
#include "evartype.h"

#include "logger.h"



using namespace std;
class estr;

template <>
void earray<evar>::serial(estr& data) const;
template <>
int earray<evar>::unserial(const estr& data,int i);



template <class T>
void earray<T>::serial(estr& data) const
{
  serialint(size(),data);
  if (size()==0) return;

  if (!getClasses().exists(typeid(T).name())) { lerror(estr("class not registered: ")+typeid(T).name()); return; }

  int ilen=data.len();

  eclassBase *pClass=&getClasses().values(typeid(T).name());
  evarType<T> *pType=new evarType<T>((T&)at(0));
  pType->cleanup=false;
  pClass->fserial(pType,data);

  if (data.len()-ilen>0){ data.reserve(ilen + (data.len()-ilen)*size()); }

  int i;
  for (i=1; i<size(); ++i){
    pType->object=(T*)&at(i);
    pClass->fserial(pType,data);
  }
  delete pType;
//    ::serial(at(i),data);
}


//  evar var(value);
//  var.var->cleanup=false;
//  if (!getClasses().exists(var.getClass())) { lerror(estr("class not registered: ")+var.getClass()); return(i); }
//  return(getClasses().values(var.getClass()).funserial(var.var,data,i));


template <class T>
int earray<T>::unserial(const estr& data,int i)
{
  clear();
  unsigned int count;
  evar var;
//  T var;
  i=unserialint(count,data,i);
  if (i==-1) return(-1);

  eclassBase *pClass=&getClasses().values(typeid(T).name());
//  evarType<T> *pType=new evarType<T>((T&)var);
//  pType->cleanup=false;

  var.create(typeid(T).name());
  var.var->cleanup=false;
//  cout << "var.count: " << var.var->pcount << endl;


  if (var.isNull()) { ldie("unregistered create class"); }


  while(count && i<data.len()){
    i=pClass->funserial(var.var,data,i);
    if (i==-1) { var.var->cleanup=true; var.clear(); return(-1); }
    addref(&var.get<T>());
    var.clear();
    var.create(typeid(T).name());
    var.var->cleanup=false;
    --count;
  }
  var.var->cleanup=true;
  var.clear();

  if (count) return(-1);

  return(i);
}

/*
template <class T>
void earray<T>::serial(estr& data) const
{
  serialint(size(),data);
  int i;
  for (i=0; i<size(); ++i)
    ::serial(at(i),data);
}

template <class T>
int earray<T>::unserial(const estr& data,int i)
{
  return(ebasearray::unserial(data,i));
  clear();
  unsigned int count;
  T var;
  i=unserialint(count,data,i);
  while(count-- && i<data.len()){
    i=::unserial(var,data,i);
    add(var);
  }
  return(i);
}
*/

template <class T>
void earray<T>::addvar(evar& var)
{
  add(var.get<T>());
}
template <class T>
evar earray<T>::getvar(int i) const
{
  return(evar((T&)at(i)));
}

template <>
void earray<evar>::addvar(evar& var);
template <>
evar earray<evar>::getvar(int i) const;


template <class T>
void earray<T>::init(int count)
{
  int i;
  clear();
  reserve(count);
  for (i=0; i<count; ++i)
    push_back(new T());
}

template <class T>
void earray<T>::init(int count,const T& initval)
{
  int i;
  clear();
  reserve(count);
  for (i=0; i<count; ++i)
    push_back(new T(initval));
}

template <class T>
inline void earray<T>::erase(int i) { delete ebasicarray<T*>::at(i); ebasicarray<T*>::erase(i); }

template <class T>
ostream &operator<<(ostream &stream,const earray<T> &var)
{
  stream<<"{ "<<endl;
  int i;

  if (var.size()){
    for (i=0; i<var.size()-1; ++i){
      stream<<var[i]<<", "<<endl;
    }
    stream<<var[i]<<endl;
  }
  stream<<" }";
  return(stream);
}


template <class T>
earray<T>::earray(): ebasicarray<T*>()
{
}

/*
template <class T>
earray<T>::earray(const T* arr,int n): ebasicarray<T*>()
{
  int i;
  for (i=0; i<n; ++i)
    *this += arr[i];
}
*/

template <class T>
earray<T>::earray(const earray<T> &arr): ebasicarray<T*>()
{
  *this += arr;
}

template <class T>
earray<T>::earray(const T &value): ebasicarray<T*>()
{
  push_back(new T(value));
}

template <class T>
earray<T>::earray(const T &value1,const T &value2): ebasicarray<T*>()
{
  push_back(new T(value1));
  push_back(new T(value2));
}

template <class T>
earray<T>::earray(const T &value1,const T &value2,const T &value3): ebasicarray<T*>()
{
  push_back(new T(value1));
  push_back(new T(value2));
  push_back(new T(value3));
}

template <class T>
earray<T>::earray(const T &value1,const T &value2,const T &value3,const T& value4): ebasicarray<T*>()
{
  push_back(new T(value1));
  push_back(new T(value2));
  push_back(new T(value3));
  push_back(new T(value4));
}

template <class T>
earray<T>::earray(const T &value1,const T &value2,const T &value3,const T& value4,const T& value5): ebasicarray<T*>()
{
  push_back(new T(value1));
  push_back(new T(value2));
  push_back(new T(value3));
  push_back(new T(value4));
  push_back(new T(value5));
}

template <class T>
earray<T>::earray(const T &value1,const T &value2,const T &value3,const T& value4,const T& value5,const T& value6): ebasicarray<T*>()
{
  push_back(new T(value1));
  push_back(new T(value2));
  push_back(new T(value3));
  push_back(new T(value4));
  push_back(new T(value5));
  push_back(new T(value6));
}

template <class T>
earray<T>::earray(const T &value1,const T &value2,const T &value3,const T& value4,const T& value5,const T& value6,const T& value7): ebasicarray<T*>()
{
  push_back(new T(value1));
  push_back(new T(value2));
  push_back(new T(value3));
  push_back(new T(value4));
  push_back(new T(value5));
  push_back(new T(value6));
  push_back(new T(value7));
}

template <class T>
earray<T>::earray(const T &value1,const T &value2,const T &value3,const T& value4,const T& value5,const T& value6,const T& value7,const T& value8): ebasicarray<T*>()
{
  push_back(new T(value1));
  push_back(new T(value2));
  push_back(new T(value3));
  push_back(new T(value4));
  push_back(new T(value5));
  push_back(new T(value6));
  push_back(new T(value7));
  push_back(new T(value8));
}

template <class T>
earray<T>::earray(const T &value1,const T &value2,const T &value3,const T& value4,const T& value5,const T& value6,const T& value7,const T& value8,const T& value9): ebasicarray<T*>()
{
  push_back(new T(value1));
  push_back(new T(value2));
  push_back(new T(value3));
  push_back(new T(value4));
  push_back(new T(value5));
  push_back(new T(value6));
  push_back(new T(value7));
  push_back(new T(value8));
  push_back(new T(value9));
}

template <class T>
earray<T>::~earray()
{
  clear();
}

template <class T>
void earray<T>::clear()
{
  int i;
  for (i=0; i<size(); ++i)
    delete ebasicarray<T*>::at(i);

  ebasicarray<T*>::clear();
}

template <class T>
earray<T> earray<T>::operator[](const eintarray& arr) const
{
  earray<T> res;
  int i;
  for (i=0; i<arr.size(); ++i){
    if (arr[i]<0 || arr[i]>=size()) { ldwarn("element in array list out of bounds: "+estr(arr[i])); continue; }
    res.add(operator[](arr[i]));
  }
  return(res); 
}


template <class T>
earray<T> &earray<T>::operator=(const earray<T> &arr)
{
  clear();
  *this += arr;
  return(*this);
}

template <class T>
earray<T> &earray<T>::operator+=(const earray<T> &arr)
{
  unsigned int i;
  for (i=0; i<arr.size(); ++i)
    add(arr[i]);
  return(*this);
}

template <class T>
earray<T> &earray<T>::operator-=(const earray<T> &arr)
{
  int ind,j;
  for (j=0; j<arr.size(); ++j){
    if (-1 != ind = find(arr[j]))
      ebasicarray<T*>::erase(ebasicarray<T*>::begin()+ind);
  }
  return(*this);
}

template <class T>
earray<T> earray<T>::operator+(const earray<T> &arr) const
{
  earray<T> tmpa;

  tmpa+=*this;
  tmpa+=arr;
  return(tmpa);
}

template <class T>
earray<T> earray<T>::operator-(const earray<T> &arr) const
{
  earray<T> tmpa;

  tmpa=*this;
  tmpa-=arr;
  return(tmpa);
}

/*
template <class T>
earray<T> earray<T>::operator[](const earray<int> &iarr)
{
  earray<T> tmpa;

  int i;
  for (i=0; i<iarr.size(); ++i)
    tmpa += iarr[i];

  return(tmpa);
}
*/

template <class T>
earray<T> earray<T>::subset(int i,int l) const
{
  earray<T> tmpa;
 
  if (i<0) i+=ebasicarray<T*>::size();
  if (l<0) l+=ebasicarray<T*>::size()-i+1;

  if (i<0 || l<0 || !ebasicarray<T*>::size()) return(tmpa);

  l+=i;
  if (l>ebasicarray<T*>::size()) l=ebasicarray<T*>::size();
  for(;i<l;++i)
    tmpa+=(*this)[i];
  return(tmpa);
}

template <class T>
int earray<T>::find(const T &value,int i) const
{
  if (i<0) i+=size();

  if (i<0) return(-1);

  for (;i<size(); ++i){
    if (at(i) == value)
      return(i);
  }
  return(-1);
}

template <class T>
int earray<T>::find(const T &value,int i,bool (*match)(const T &a, const T &b)) const
{
  if (i<0) i+=size();

  if (i<0) return(-1);

  for (;i<size(); ++i){
    if ( match( at(i) , value) )
      return(i);
  }
  return(-1);
}

/*
template <class T>
earray<T> earray<T>::afindall(const T &value,int i,bool (*match)(const T &a, const T &b)) const
{
  earray<T> tmpai;

  if (i<0) i+=ebasicarray<T*>::size();

  if (i<0) return(tmpai);

  for (;i<ebasicarray<T*>::size(); ++i){
    if ( match( (*this)[i] , value) )
      tmpai += (*this)[i];
  }
  return(tmpai);
}
*/

template <class T>
earray<T> earray<T>::afindall(const earray<T> &arr,int i,bool (*match)(const T &a, const T &b)) const
{
  earray<T> tmpai;

  if (i<0) i+=ebasicarray<T*>::size();
  if (i<0) return(tmpai);

  for (;i<ebasicarray<T*>::size(); ++i){
    if ( -1 != arr.find( (*this)[i] , 0 , match) )
      tmpai += (*this)[i];
  }
  return(tmpai);
}

template <class T>
eintarray earray<T>::findall(const T &value,int i,bool (*match)(const T &a, const T &b)) const
{
  eintarray tmpai;

  if (i<0) i+=ebasicarray<T*>::size();

  if (i<0) return(tmpai);

  for (;i<ebasicarray<T*>::size(); ++i){
    if ( match( (*this)[i] , value) )
      tmpai += i;
  }
  return(tmpai);
}

template <class T>
eintarray earray<T>::findall(const earray<T> &arr,int i,bool (*match)(const T &a, const T &b)) const
{
  eintarray tmpai;

  if (i<0) i+=ebasicarray<T*>::size();

  if (i<0) return(tmpai);

  for (;i<ebasicarray<T*>::size(); ++i){
    if ( -1 != arr.find( (*this)[i] , 0 , match) )
      tmpai += i;
  }
  return(tmpai);
}


/*
void erase(const earray<int> &iarr)
{
  int i;

  for (i=0; i<iarr.size(); ++i){
    vector::erase(begin() + iarr[i]);
  }

}
*/

#endif

