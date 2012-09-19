#ifndef EHASHMAP_DEC_H
#define EHASHMAP_DEC_H

#include "eutils.h"
#include "earray_dec.h"
#include "ebasearrayof.h"

template <class K,class T>
class ehashitem
{
 public:
  K key;
  T *value;
  ehashitem<K,T>* next;
  ehashitem<K,T>* prev;

  ehashitem(const K& key,T* value,ehashitem<K,T>* next);
  ~ehashitem();
};


template <class K,class T,unsigned int (*hashfunc)(const K&)>
class ehashmap : public ebasearrayof
{
  earray<K> _keys;
  mutable ehashitem<K,T> **_hashitems;
 public:
  mutable unsigned int _hashmask;
//  unsigned int (*hashfunc)(const K&);

  ehashmap();
  ehashmap(const ehashmap<K,T,hashfunc>& oldhm);
  ~ehashmap();

  inline unsigned int size() const { return(_keys.size()); }

//  void add(const T& value);
  T& add(const K& key,const T& value);
  T& addref(const K& key,T* value);

  inline const T& operator[](const K& key) const;
  const T& operator[](int i) const;
  T& operator[](const K& key);
  T& operator[](int i);

  ehashmap<K,T,hashfunc>& operator+=(const ehashmap<K,T,hashfunc>& hm);


  inline T& at(int i)   { return(values( _keys.at(i) ) ); }
  inline T& values(int i) { return(values( _keys.at(i) ) ); }
  inline K& keys(int i) { return( _keys.at(i)); }
  T& values(const K& key);
  inline const T& at(int i) const  { return(values( _keys.at(i) ) ); }
  inline const T& values(int i) const { return(values( _keys.at(i) ) ); }
  inline const K& keys(int i) const { return( _keys.at(i)); }
  const T& values(const K& key) const;

  bool exists(const K& key) const ;
  int findkey(const K& key,int pos=0) const;

  void clear();
  void resizehash(unsigned int i=0) const;

  void reserve(unsigned int i);
  void erase(const K& key);
  void erase(int i);

  ehashitem<K,T>* gethashitem(unsigned int khash,const K& key) const;

  inline unsigned int hash(const K& key) const { return(hashfunc(key) & _hashmask); }

  void addvar(evar& key,evar& var);
  evar getvar(int i) const;
  evar getvarByKey(const evar& var) const;
  evar getvarkey(int i) const;
};

#endif

