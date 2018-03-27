#ifndef EBASICHASHMAP_DEC_H
#define EBASICHASHMAP_DEC_H

#include "eutils.h"
#include "earray_dec.h"
#include "ebasearrayof.h"
#include "ehashmap_dec.h"

template <class K,class T,size_t (*hashfunc)(const K&)>
class ebasichashmap : public ebasearrayof
{
  mutable ehashitem<K,T> **_hashitems;
  size_t count;
 public:
  mutable size_t _hashmask;
//  unsigned int (*hashfunc)(const K&);

  ebasichashmap();
  ebasichashmap(const ebasichashmap<K,T,hashfunc>& oldhm);
  ~ebasichashmap();

  inline size_t size() const { return(count); }

//  void add(const T& value);
  T& add(const K& key,const T& value);
  T& addref(const K& key,T* value);

  inline const T& operator[](const K& key) const;
//  const T& operator[](int i) const;
  T& operator[](const K& key);
//  T& operator[](int i);

  ebasichashmap<K,T,hashfunc>& operator+=(const ebasichashmap<K,T,hashfunc>& hm);


//  inline T& at(int i)   { return(values( _keys.at(i) ) ); }
//  inline T& values(int i) { return(values( _keys.at(i) ) ); }
//  inline K& keys(int i) { return( _keys.at(i)); }
  T& values(const K& key);
//  inline const T& at(int i) const  { return(values( _keys.at(i) ) ); }
//  inline const T& values(int i) const { return(values( _keys.at(i) ) ); }
//  inline const K& keys(int i) const { return( _keys.at(i)); }
  const T& values(const K& key) const;

  bool exists(const K& key) const ;
  long findkey(const K& key,size_t pos=0) const;

  void clear();
  void resizehash(size_t i=0) const;

  void reserve(size_t i);
  void erase(const K& key);
//  void erase(int i);

  ehashitem<K,T>* gethashitem(size_t khash,const K& key) const;

  inline size_t hash(const K& key) const { return(hashfunc(key) & _hashmask); }

  void addvar(evar& key,evar& var);
  evar getvar(size_t i) const;
  evar getvarkey(size_t i) const;
  evar getvarByKey(const evar& var) const;

//  template <typename K,typename T>
  class iter
  {
   public:
    const ebasichashmap<K,T,hashfunc> *hashmap;
    ehashitem<K,T> *hitem;
    size_t  bucket;

    iter();

    iter& operator++();
 
    const K& key() const;
    T& value() const;

    T& operator*() const;
    T* operator->() const;

    bool operator==(const iter& i) const;
    bool operator!=(const iter& i) const;

    iter& operator=(const iter& i);
  };

  void erase(const typename ebasichashmap<K,T,hashfunc>::iter&);
  typename ebasichashmap<K,T,hashfunc>::iter get(const K& key) const;
  typename ebasichashmap<K,T,hashfunc>::iter begin() const;
  typename ebasichashmap<K,T,hashfunc>::iter end() const;
};

#endif

