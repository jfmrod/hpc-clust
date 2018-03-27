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


template <class K,class T,size_t (*hashfunc)(const K&)>
class ehashmap : public ebasearrayof
{
  earray<K> _keys;
  mutable ehashitem<K,T> **_hashitems;
 public:
  mutable size_t _hashmask;
//  unsigned int (*hashfunc)(const K&);

  ehashmap();
  ehashmap(const ehashmap<K,T,hashfunc>& oldhm);
  ~ehashmap();

  inline size_t size() const { return(_keys.size()); }

//  void add(const T& value);
  T& add(const K& key,const T& value);
  T& addref(const K& key,T* value);

  inline const T& operator[](const K& key) const;
  const T& operator[](size_t i) const;
  T& operator[](const K& key);
  T& operator[](size_t i);

  ehashmap<K,T,hashfunc>& operator+=(const ehashmap<K,T,hashfunc>& hm);


  inline T& at(size_t i)   { return(values( _keys.at(i) ) ); }
  inline T& values(size_t i) { return(values( _keys.at(i) ) ); }
  inline K& keys(size_t i) { return( _keys.at(i)); }
  T& values(const K& key);
  inline const T& at(size_t i) const  { return(values( _keys.at(i) ) ); }
  inline const T& values(size_t i) const { return(values( _keys.at(i) ) ); }
  inline const K& keys(size_t i) const { return( _keys.at(i)); }
  const T& values(const K& key) const;

  bool exists(const K& key) const ;
  long findkey(const K& key,size_t pos=0) const;

  void clear();
  void resizehash(size_t i=0) const;

  void reserve(size_t i);
  void erase(const K& key);
  void erase(size_t i);

  ehashitem<K,T>* gethashitem(size_t khash,const K& key) const;

  inline size_t hash(const K& key) const { return(hashfunc(key) & _hashmask); }

  void addvar(evar& key,evar& var);
  evar getvar(size_t i) const;
  evar getvarByKey(const evar& var) const;
  evar getvarkey(size_t i) const;
};

#endif

