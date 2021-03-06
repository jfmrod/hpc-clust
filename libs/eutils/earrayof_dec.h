#ifndef EARRAYOF_DEC_H
#define EARRAYOF_DEC_H

#include "eutils.h"

#include <iostream>

#include "ebasicarray_dec.h"

#include "ebasearrayof.h"
//#include "estr.h"
class estr;

using namespace std;

typedef ebasicarray<int> eintarray;

template <class T, class K>
class earrayof;

template <class T,class K>
ostream &operator<<(ostream &stream,const earrayof<T,K> &var);

template <class T, class K>
class earrayof : public ebasearrayof
{
 protected:
  ebasicarray<T*> _values;
  ebasicarray<K*> _keys;
 public:
  earrayof();
  earrayof(const earrayof<T,K> &a);
//  earrayof(const ebasicarray<T> &a);
  earrayof(const T &a);
  ~earrayof();
  
  T &add(const T& value);
  T &add(const K& name,const T& value);
  T &addref(const K& name,T *value);

//  void add(const ebasicarray<T> &value);

  T &add_keysorted(const K& key,const T& value);
  
  earrayof<T,K> operator[](const eintarray& i) const;

  T& operator[](const K& key);
  const T& operator[](const K& key) const;
  
  T& operator[](size_t i);
  const T& operator[](size_t i) const;

//  earrayof<T,K> operator[](const eintarray& arr) const;

  earrayof<T,K>  operator+(const earrayof<T,K> &a) const;
  earrayof<T,K>  operator-(const earrayof<T,K> &a) const;
  earrayof<T,K> &operator=(const earrayof<T,K> &a);
  earrayof<T,K> &operator+=(const earrayof<T,K> &a);
  earrayof<T,K> &operator-=(const earrayof<T,K> &a);

  inline earrayof<T,K> &operator=(const T &a) { clear(); add(a); return(*this); }
//  inline earrayof<T,K> &operator=(const ebasicarray<T> &a) { clear(); add(a); return(*this); }
  earrayof<T,K> &operator+=(const T &a) { add(a); return(*this); }
//  earrayof<T,K> &operator+=(const ebasicarray<T> &a) { add(a); return(*this); }

  inline earrayof<T,K>  operator+(const T &a) const { earrayof<T,K> tmp; tmp += *this; tmp.add(a); return(tmp); }
//  inline earrayof<T,K>  operator+(const ebasicarray<T> &a) const { earrayof<T,K> tmp; tmp += *this; tmp.add(a); return(tmp); }

  T& insert(size_t i,const T& value);
  T& insert(size_t i,const K& key,const T& value);

  inline void swap(size_t i,size_t j){ _values.swap(i,j); _keys.swap(i,j); }

  inline K& keys(size_t i) { return(*_keys[i]); }
  inline const K& keys(size_t i) const  { return(*_keys[i]); }

  T& at(const K& key);
  const T& at(const K& key) const;

  inline T& values(size_t i) { return(*_values[i]); }
  inline const T& values(size_t i) const { return(*_values[i]); }

//  inline T& at(int i) { return(*_values[i]); }
//  inline const T& at(int i) const { return(*_values[i]); }

  inline size_t size() const { return(_values.size()); }

//  inline const earray<T> &values() const { return(_values); }
//  inline const ebasicarray<T>& values() const { return(_values); }


  earrayof<T,K> subset(long i,long l=-1) const;


  static bool equal(T* const &a, T* const &b);
  static bool equalkey(K* const &a, K* const &b);

  long findkey(const K& value,size_t i=0,bool (*matchkey)(K* const &a,K* const &b)=equalkey) const
    { return( _keys.find((K*)&value,i,matchkey) ); }
  long find(const T& value,size_t i=0,bool (*match)(T* const &a,T* const &b)=equal) const
    { return( _values.find((T*)&value,i,match) ); }

  eintarray findkeyall(const K& key,size_t i=0,bool (*match)(K* const &a,K* const &b)=equalkey) const;
  eintarray findkeyall(const earrayof<T,K>& keys,size_t i=0,bool (*match)(K* const &a,K* const &b)=equalkey) const;

  eintarray findall(const T& value,size_t i=0,bool (*match)(T* const &a,T* const &b)=equal) const;
  eintarray findall(const earrayof<T,K>& value,size_t i=0,bool (*match)(T* const &a,T* const &b)=equal) const;

  earrayof<T,K> afindall(const earrayof<T,K>& values,size_t i=0,bool (*match)(T* const& a,T* const& b)=equal) const
    { return( operator[](findall(values,i,match))); }
  earrayof<T,K> afindkeyall(const earrayof<T,K>& keys,size_t i=0,bool (*match)(K* const& a,K* const& b)=equalkey) const
    { return( operator[](findkeyall(keys,i,match)) ); }

  void remove(size_t i);
  inline void erase(size_t i) { remove(i); }
  inline void rem(size_t i)   { remove(i); }
  inline void del(long int i,long int l=-1)   { if (i<0) i+=size(); if (l<0) l+=size()-i+1; if (i>=size() || i<0l || l<=0l) return; if (i+l>size()) l=size()-i; for (; l>0; --l) remove(i); }

  void clear();

  friend ostream &operator<< <>(ostream &stream,const earrayof<T,K> &var);

  void addvar(evar& key,evar& var);
  evar getvar(size_t i) const;
  evar getvarByKey(const evar& var) const;
  evar getvarkey(size_t i) const;
};

#endif

