#ifndef EBASICARRAY_H
#define EBASICARRAY_H

#include "eutils.h"

#include<vector>
#include<iostream>

using namespace std;

#include "ebasearray.h"
#include "ebasicarray_dec.h"

#include "evar_dec.h"
#include "estr.h"
#include "evarclass_dec.h"
#include "evartype.h"

#include "logger.h"

/*
template <>
void ebasicarray<evar*>::addvar(evar& var);
template <>
evar ebasicarray<evar*>::getvar(int i) const;
*/

template <class T>
void ebasicarray<T>::init(int count)
{
  int i;
  clear();
  vector<T>::reserve(count);
  for (i=0; i<count; ++i)
    push_back(T());
}

template <class T>
void ebasicarray<T>::init(int count,const T& initval)
{
  int i;
  clear();
  vector<T>::reserve(count);
  for (i=0; i<count; ++i)
    push_back(initval);
}

template <class T>
void ebasicarray<T>::serial(estr& data) const
{
  serialint(size(),data);
  if (size()==0) return;

  if (!getClasses().exists(typeid(T).name())) { lerror(estr("class not registered: ")+typeid(T).name()); return; }

  int ilen=data.len();

  eclassBase *pClass=&getClasses().values(typeid(T).name());
  evarType<T> *pType=new evarType<T>((T&)at(0));
  pType->cleanup=false;
  pClass->fserial(pType,data);

  if (data.len()-ilen>0){ data.reserve(ilen+ (data.len()-ilen)*size()); }

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
int ebasicarray<T>::unserial(const estr& data,int i)
{
  clear();
  unsigned int count;
  T var;
  i=unserialint(count,data,i);
  if (i==-1) return(-1);

  eclassBase *pClass=&getClasses().values(typeid(T).name());
  evarType<T> *pType=new evarType<T>((T&)var);
  pType->cleanup=false;

  while(count && i<data.len()){
    i=pClass->funserial(pType,data,i);
    if (i==-1) { delete pType; return(-1); }
    add(var);
    --count;
  }
  delete pType;

  if (count) return(-1);

  return(i);
}

template <class T>
void ebasicarray<T>::addvar(evar& var)
{
  add(var.get<T>());
}
template <class T>
evar ebasicarray<T>::getvar(int i) const
{
  return(evar((T&)at(i)));
}

template <>
void ebasicarray<evar>::addvar(evar& var);
template <>
evar ebasicarray<evar>::getvar(int i) const;



template <class T>
ostream &operator<<(ostream &stream,const ebasicarray<T> &var)
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
ebasicarray<T>::ebasicarray(): vector<T>()
{
}

template <class T>
ebasicarray<T>::ebasicarray(const ebasicarray<T> &arr): vector<T>()
{
  *this += arr;
}

template <class T>
ebasicarray<T>::ebasicarray(const T& value): vector<T>()
{
  push_back(value);
}

template <class T>
ebasicarray<T>::ebasicarray(const T& v1,const T& v2): vector<T>()
{
  push_back(v1);
  push_back(v2);
}

template <class T>
ebasicarray<T>::ebasicarray(const T& v1,const T& v2,const T& v3): vector<T>()
{
  push_back(v1);
  push_back(v2);
  push_back(v3);
}

template <class T>
ebasicarray<T>::ebasicarray(const T& v1,const T& v2,const T& v3,const T& v4): vector<T>()
{
  push_back(v1);
  push_back(v2);
  push_back(v3);
  push_back(v4);
}

template <class T>
ebasicarray<T>::ebasicarray(const T& v1,const T& v2,const T& v3,const T& v4,const T& v5): vector<T>()
{
  push_back(v1);
  push_back(v2);
  push_back(v3);
  push_back(v4);
  push_back(v5);
}

template <class T>
ebasicarray<T>::ebasicarray(const T& v1,const T& v2,const T& v3,const T& v4,const T& v5,const T& v6): vector<T>()
{
  push_back(v1);
  push_back(v2);
  push_back(v3);
  push_back(v4);
  push_back(v5);
  push_back(v6);
}

template <class T>
ebasicarray<T>::ebasicarray(const T& v1,const T& v2,const T& v3,const T& v4,const T& v5,const T& v6,const T& v7): vector<T>()
{
  push_back(v1);
  push_back(v2);
  push_back(v3);
  push_back(v4);
  push_back(v5);
  push_back(v6);
  push_back(v7);
}

template <class T>
ebasicarray<T>::~ebasicarray()
{
}

template <class T>
ebasicarray<T> &ebasicarray<T>::operator=(const ebasicarray<T> &arr)
{
  clear();
  *this += arr;
  return(*this);
}

template <class T>
ebasicarray<T> &ebasicarray<T>::operator+=(const ebasicarray<T> &arr)
{
  int i;
  for (i=0; i<arr.size(); ++i)
    push_back(arr[i]);
  return(*this);
}

template <class T>
ebasicarray<T> ebasicarray<T>::operator[](const eintarray& arr) const
{
  ebasicarray<T> res;
  int i;
  for (i=0; i<arr.size(); ++i){
    if (arr[i]<0 || arr[i]>=size()) { ldwarn("element in array list out of bounds: "+estr(arr[i])); continue; }
    res.add(operator[](arr[i]));
  }
  return(res); 
}

template <class T>
ebasicarray<T> &ebasicarray<T>::operator-=(const ebasicarray<T> &arr)
{
  int ind,j;
  for (j=0; j<arr.size(); ++j){
    if (-1 != ind = find(arr[j]))
      vector<T>::erase(vector<T>::begin()+ind);
  }
  return(*this);
}

template <class T>
ebasicarray<T> ebasicarray<T>::operator+(const ebasicarray<T> &arr) const
{
  ebasicarray<T> tmpa;

  tmpa+=*this;
  tmpa+=arr;
  return(tmpa);
}

template <class T>
ebasicarray<T> ebasicarray<T>::operator-(const ebasicarray<T> &arr) const
{
  ebasicarray<T> tmpa;

  tmpa=*this;
  tmpa-=arr;
  return(tmpa);
}

/*
template <class T>
ebasicarray<T> ebasicarray<T>::operator[](const ebasicarray<int> &iarr)
{
  ebasicarray<T> tmpa;

  int i;
  for (i=0; i<iarr.size(); ++i)
    tmpa += iarr[i];

  return(tmpa);
}
*/

template <class T>
ebasicarray<T> ebasicarray<T>::subset(int i,int l) const
{
  ebasicarray<T> tmpa;
 
  if (i<0) i+=vector<T>::size();
  if (l<0) l+=vector<T>::size()-i+1;

  if (i<0 || l<0 || !vector<T>::size()) return(tmpa);

  l+=i;
  if (l>vector<T>::size()) l=vector<T>::size();
  for(;i<l;++i)
    tmpa+=(*this)[i];
}

template <class T>
int ebasicarray<T>::find(const T &value,int i,bool (*match)(const T &a, const T &b)) const
{
  if (i<0) i+=vector<T>::size();

  if (i<0) return(-1);

  for (;i<vector<T>::size(); ++i){
    if ( match( at(i) , value) )
      return(i);
  }
  return(-1);
}

/*
template <class T>
ebasicarray<T> ebasicarray<T>::afindall(const T &value,int i,bool (*match)(const T &a, const T &b)) const
{
  ebasicarray<T> tmpai;

  if (i<0) i+=vector<T>::size();

  if (i<0) return(tmpai);

  for (;i<vector<T>::size(); ++i){
    if ( match( (*this)[i] , value) )
      tmpai += (*this)[i];
  }
  return(tmpai);
}
*/

template <class T>
ebasicarray<T> ebasicarray<T>::afindall(const ebasicarray<T> &arr,int i,bool (*match)(const T &a, const T &b)) const
{
  ebasicarray<T> tmpai;

  if (i<0) i+=vector<T>::size();

  if (i<0){ ldwarn("starting index below 0: "+estr(i)); return(tmpai); }
  for (;i<vector<T>::size(); ++i){
    if ( -1 != arr.find( (*this)[i] , 0 , match) )
      tmpai += (*this)[i];
  }
  return(tmpai);
}

template <class T>
eintarray ebasicarray<T>::findall(const T &value,int i,bool (*match)(const T &a, const T &b)) const
{
  eintarray tmpai;

  if (i<0) i+=vector<T>::size();
  if (i<0) { ldwarn("starting index below 0: "+estr(i)); return(tmpai); }

  for (;i<vector<T>::size(); ++i){
    if ( match( (*this)[i] , value) )
      tmpai += i;
  }
  return(tmpai);
}

template <class T>
eintarray ebasicarray<T>::findall(const ebasicarray<T> &arr,int i,bool (*match)(const T &a, const T &b)) const
{
  eintarray tmpai;

  if (i<0) i+=vector<T>::size();
  if (i<0) { ldwarn("starting index below 0: "+estr(i)); return(tmpai); }

  for (;i<vector<T>::size(); ++i){
    if ( -1 != arr.find( (*this)[i] , 0 , match) )
      tmpai += i;
  }
  return(tmpai);
}

#endif

