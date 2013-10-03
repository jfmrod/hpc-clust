#ifndef EBASICHASHMAP_H
#define EBASICHASHMAP_H

#include "eutils.h"

#include "ebasichashmap_dec.h"
#include "ehashmap_dec.h"
#include "evar_dec.h"




template <class K,class T,unsigned int (*hashfunc)(const K&)>
void ebasichashmap<K,T,hashfunc>::addvar(evar& evarkey,evar& var)
{
  addref(evarkey.get<K>(),&var.get<T>());
}
template <class K,class T,unsigned int (*hashfunc)(const K&)>
evar ebasichashmap<K,T,hashfunc>::getvar(int i) const
{
//  return(evar((T*)&values(i)));
  return(evar());
}
template <class K,class T,unsigned int (*hashfunc)(const K&)>
evar ebasichashmap<K,T,hashfunc>::getvarkey(int i) const
{
  return(evar());
//  return(evar((K*)&keys(i)));
}
template <class K,class T,unsigned int (*hashfunc)(const K&)>
evar ebasichashmap<K,T,hashfunc>::getvarByKey(const evar& var) const
{
  if (var.getTypeid()==typeid(K))
    return(evar((T&)values(var.get<K>())));
  return(evar());
}


#include "logger.h"

const unsigned int BASICHASH_INIT_MASK=0xFF;

template <class K,class T,unsigned int (*hashfunc)(const K&)>
ebasichashmap<K,T,hashfunc>::iter::iter(): hashmap(0x00), hitem(0x00), bucket(0) {}

template <class K,class T,unsigned int (*hashfunc)(const K&)>
typename ebasichashmap<K,T,hashfunc>::iter& ebasichashmap<K,T,hashfunc>::iter::operator++()
{
  hitem=hitem->next;
  
  if (hitem) return(*this);

  ++bucket;
  while (hashmap->_hashitems[bucket]==0x00 && bucket < hashmap->_hashmask+1) ++bucket;

  if (bucket < hashmap->_hashmask+1)
    hitem=hashmap->_hashitems[bucket];
  return(*this); 
}

template <class K,class T,unsigned int (*hashfunc)(const K&)>
const K& ebasichashmap<K,T,hashfunc>::iter::key() const
{
  lddieif(hitem==0x00,"trying to access end iterator");
  return(hitem->key);
}

template <class K,class T,unsigned int (*hashfunc)(const K&)>
T& ebasichashmap<K,T,hashfunc>::iter::value() const
{
  lddieif(hitem==0x00,"trying to access end iterator");
  return(*hitem->value);
}

template <class K,class T,unsigned int (*hashfunc)(const K&)>
T& ebasichashmap<K,T,hashfunc>::iter::operator*() const
{
  lddieif(hitem==0x00,"trying to access end iterator");
  return(*hitem->value);
}

template <class K,class T,unsigned int (*hashfunc)(const K&)>
T* ebasichashmap<K,T,hashfunc>::iter::operator->() const
{
  lddieif(hitem==0x00,"trying to access end iterator");
  return(hitem->value);
}

template <class K,class T,unsigned int (*hashfunc)(const K&)>
bool ebasichashmap<K,T,hashfunc>::iter::operator==(const ebasichashmap<K,T,hashfunc>::iter& i) const
{
  return(hashmap==i.hashmap && hitem==i.hitem);
}

template <class K,class T,unsigned int (*hashfunc)(const K&)>
bool ebasichashmap<K,T,hashfunc>::iter::operator!=(const ebasichashmap<K,T,hashfunc>::iter& i) const
{
  return(hashmap!=i.hashmap || hitem!=i.hitem);
}

template <class K,class T,unsigned int (*hashfunc)(const K&)>
typename ebasichashmap<K,T,hashfunc>::iter& ebasichashmap<K,T,hashfunc>::iter::operator=(const ebasichashmap<K,T,hashfunc>::iter& i)
{
  hashmap=i.hashmap;
  bucket=i.bucket;
  hitem=i.hitem;
  return(*this);
}




template <class K,class T,unsigned int (*hashfunc)(const K&)>
ebasichashmap<K,T,hashfunc>::ebasichashmap(): count(0)
{
  _hashmask = BASICHASH_INIT_MASK;
  _hashitems=new ehashitem<K,T>*[_hashmask+1];
  unsigned int i;
  for (i=0; i<_hashmask+1; ++i)
    _hashitems[i]=0x00;
}

template <class K,class T,unsigned int (*hashfunc)(const K&)>
ebasichashmap<K,T,hashfunc>::ebasichashmap(const ebasichashmap<K,T,hashfunc>& oldhm): count(oldhm.count)
{
  _hashmask = oldhm._hashmask;
  _hashitems=new ehashitem<K,T>*[_hashmask+1];

  ehashitem<K,T> *oldhmitem;
  unsigned int i;
  for (i=0; i<_hashmask+1; ++i){
    _hashitems[i]=0x00;
    for (oldhmitem=oldhm._hashitems[i]; oldhmitem!=0x00; oldhmitem=oldhmitem->next)
      _hashitems[i]=new ehashitem<K,T>(oldhmitem->key,oldhmitem->value,_hashitems[i]);
  }
}




template <class K,class T,unsigned int (*hashfunc)(const K&)>
ebasichashmap<K,T,hashfunc>::~ebasichashmap()
{
  clear();
  delete[] _hashitems;
}

template <class K,class T,unsigned int (*hashfunc)(const K&)>
typename ebasichashmap<K,T,hashfunc>::iter ebasichashmap<K,T,hashfunc>::get(const K& key) const
{
  typename ebasichashmap<K,T,hashfunc>::iter it;

  it.hashmap=this;
  it.bucket=hash(key);
  it.hitem=_hashitems[it.bucket];

  while (it.hitem!=0x00){
    if (key == it.hitem->key)   // there is no collision
      return(it);
    it.hitem=it.hitem->next;
  }

  return(end());
}

template <class K,class T,unsigned int (*hashfunc)(const K&)>
typename ebasichashmap<K,T,hashfunc>::iter ebasichashmap<K,T,hashfunc>::begin() const
{
  typename ebasichashmap<K,T,hashfunc>::iter i;
  i.hashmap=this;
  i.bucket=0;
  i.hitem=0x00;
  while (_hashitems[i.bucket]==0x00 && i.bucket<_hashmask+1) ++i.bucket;

  if (i.bucket==_hashmask+1)
    return(end());

  i.hitem=_hashitems[i.bucket];
  return(i);
}

template <class K,class T,unsigned int (*hashfunc)(const K&)>
typename ebasichashmap<K,T,hashfunc>::iter ebasichashmap<K,T,hashfunc>::end() const
{
  typename ebasichashmap<K,T,hashfunc>::iter i;
  i.hashmap=this;
  i.bucket=0;
  i.hitem=0x00;
  return(i);
}

template <class K,class T,unsigned int (*hashfunc)(const K&)>
void ebasichashmap<K,T,hashfunc>::reserve(unsigned int i)
{
  unsigned int a;
  unsigned int c=1;
  a=0x01;
  while (i>0){
    i=i>>1;
    a=(a<<1)|0x01;
    ++c;
  }

  if (c>=sizeof(unsigned int)*8){
    lwarn("reached limit of hash table index size");
    a=0x80000000u-0x01u;
  }
  
  resizehash(a);
}

template <class K,class T,unsigned int (*hashfunc)(const K&)>
void ebasichashmap<K,T,hashfunc>::resizehash(unsigned int newmask) const
{
  unsigned int thashmask;
  ehashitem<K,T> **thashitems;

  if (newmask < _hashmask) return;

  ldinfo("resizing hash table");

  if (newmask==0) 
    thashmask = (_hashmask << 1)|0x01;
  else
    thashmask = newmask;

  thashitems=new ehashitem<K,T>*[thashmask+1u];
  ldieif(thashitems==0x00,"unable to allocate memory for hashmap");
  unsigned int i;
  for (i=0; i<thashmask+1u; ++i)
    thashitems[i]=0x00;

  unsigned int khash;
  ehashitem<K,T>* hitem;

  typename ebasichashmap<K,T,hashfunc>::iter it;
  for (it=begin(); it!=end(); ++it){
    khash = hashfunc(it.key()) & thashmask;
    hitem = gethashitem(khash & _hashmask,it.key());

    if (hitem->prev)
      hitem->prev->next=hitem->next;
    else
      _hashitems[khash&_hashmask]=hitem->next;
    if (hitem->next)
      hitem->next->prev=hitem->prev;

    hitem->prev=0x00;
    hitem->next=thashitems[khash];
    thashitems[khash]=hitem;
    if (hitem->next)
      hitem->next->prev=hitem;
  } 

  delete[] _hashitems;
  _hashmask=thashmask;
  _hashitems=thashitems;
  ldinfo("finished resize");
}

template <class K,class T,unsigned int (*hashfunc)(const K&)>
ehashitem<K,T>* ebasichashmap<K,T,hashfunc>::gethashitem(unsigned int khash,const K& key) const
{
  ehashitem<K,T>* hitem;
  hitem = _hashitems[khash];
  while (hitem != 0x00){
    if (hitem->key == key)
      return(hitem);
    hitem=hitem->next;
  }
  lderror("ebasichashmap: did not find key");
  return(0x00); 
}


template <class K,class T,unsigned int (*hashfunc)(const K&)>
void ebasichashmap<K,T,hashfunc>::clear()
{
  int i;
  ehashitem<K,T> *hitem;
  for (i=0; i<_hashmask+1; ++i){
    while (_hashitems[i]){
      hitem=_hashitems[i];
      _hashitems[i]=hitem->next;
      delete hitem->value;
      delete hitem;
    }
  }
  count=0;
}

template <class K,class T,unsigned int (*hashfunc)(const K&)>
bool ebasichashmap<K,T,hashfunc>::exists(const K& key) const
{
  int i;
  ehashitem<K,T>* hitem;

  i=hash(key);
  hitem=_hashitems[i];

  while (hitem!=0x00){
    if (key == hitem->key)
      return(true);
    hitem=hitem->next;
  }
  // non existent value
  return(false);
}

template <class K,class T,unsigned int (*hashfunc)(const K&)>
void ebasichashmap<K,T,hashfunc>::erase(const typename ebasichashmap<K,T,hashfunc>::iter& it)
{
  ldieif(it.hitem==0x00,"trying to delete empty iterator");
  if (it.hitem->prev) it.hitem->prev->next=it.hitem->next;
  else _hashitems[it.bucket]=it.hitem->next;
  if (it.hitem->next) it.hitem->next->prev=it.hitem->prev;
  delete it.hitem->value;
  delete it.hitem;
//  it.hitem=0x00;
  --count;
}

template <class K,class T,unsigned int (*hashfunc)(const K&)>
void ebasichashmap<K,T,hashfunc>::erase(const K& key)
{
  int i;
  ehashitem<K,T>* hitem;

  i=hash(key);
  hitem=_hashitems[i];

  int j;
  while (hitem!=0x00){
    if (key == hitem->key){
      if (hitem->prev) hitem->prev->next=hitem->next;
      else _hashitems[i]=hitem->next;
      if (hitem->next) hitem->next->prev=hitem->prev;
      delete hitem->value;
      delete hitem;
      --count;
      return;
    }
    hitem=hitem->next;
  }
  lddie("tried to delete key from hashmap that does not exist");
  // non existent value
}

template <class K,class T,unsigned int (*hashfunc)(const K&)>
ebasichashmap<K,T,hashfunc>& ebasichashmap<K,T,hashfunc>::operator+=(const ebasichashmap<K,T,hashfunc>& hm)
{
  int i;
  for (i=0; i<hm.size(); ++i)
    add(hm.keys(i),hm.values(hm.keys(i)));
  return(*this);
}


template <class K,class T,unsigned int (*hashfunc)(const K&)>
T& ebasichashmap<K,T,hashfunc>::addref(const K& key,T* value)
{
  int i;
  ehashitem<K,T>* hitem;

  if (size() > (3u*(_hashmask+1))>>2u) resizehash();

  i=hash(key);
  hitem=_hashitems[i];

  while (hitem!=0x00){
    if (key == hitem->key){   // there is no collision
      hitem->value = value;
      return(*hitem->value);
    }
    hitem=hitem->next;
  }

  // non existent value
  ++count;
  _hashitems[i]=new ehashitem<K,T>(key,value,_hashitems[i]);
  return(*_hashitems[i]->value);
}

template <class K,class T,unsigned int (*hashfunc)(const K&)>
T& ebasichashmap<K,T,hashfunc>::add(const K& key,const T& value)
{
  int i;
  ehashitem<K,T>* hitem;

  if (size() > (3u*(_hashmask+1))>>2u) resizehash();

  i=hash(key);
  hitem=_hashitems[i];

  while (hitem!=0x00){
    if (key == hitem->key){   // there is no collision
      hitem->value=new T(value);
      return(*hitem->value);
    }
    hitem=hitem->next;
  }

  // non existent value
  ++count;
  _hashitems[i]=new ehashitem<K,T>(key,new T(value),_hashitems[i]);
  return(*_hashitems[i]->value);
/*
  operator[](key) = value;
  return(operator[](key));
*/
}

template <class K,class T,unsigned int (*hashfunc)(const K&)>
const T& ebasichashmap<K,T,hashfunc>::operator[](const K& key) const
{
  int i;
  ehashitem<K,T>* hitem;

  if (size() > (3u*(_hashmask+1))>>2u) resizehash();

  i=hash(key);
  hitem=_hashitems[i];

  while (hitem!=0x00){
    if (key == hitem->key)   // there is no collision
      return(*hitem->value);
    hitem=hitem->next;
  }

  // non existent value
//  ++count;
  _hashitems[i]=new ehashitem<K,T>(key,new T,_hashitems[i]);
  return(*_hashitems[i]->value);
}

/*
template <class K,class T>
const T& ebasichashmap<K,T,hashfunc>::operator[](int ind) const
{
  int i;
  ehashitem<K,T>* hitem;

  lddieif(ind > _keys.size(),"ebasichashmap: index out of bounds");
  i=hash(_keys.at(ind));
  hitem=_hashitems[i];

  while (hitem!=0x00){
    if (_keys.at(ind) == hitem->key)   // there is no collision
      return(*hitem->value);
    hitem=hitem->next;
  }

  ldie("ebasichashmap: index out of bounds: "+estr(ind));
//  return(*(T*)0x00);
}
*/

template <class K,class T,unsigned int (*hashfunc)(const K&)>
T& ebasichashmap<K,T,hashfunc>::operator[](const K& key)
{
  int i;
  ehashitem<K,T>* hitem;

  if (size() > (3u*(_hashmask+1))>>2u) resizehash();

  i=hash(key);
  hitem=_hashitems[i];

  while (hitem!=0x00){
    if (key == hitem->key)   // there is no collision
      return(*hitem->value);
    hitem=hitem->next;
  }

//  lerror("key not found");
//  throw "key not found";

  // non existent value
  ++count;
  _hashitems[i]=new ehashitem<K,T>(key,new T,_hashitems[i]);
  return(*_hashitems[i]->value);
}

//#pragma GCC diagnostic ignored "-Wreturn-type"
template <class K,class T,unsigned int (*hashfunc)(const K&)>
const T& ebasichashmap<K,T,hashfunc>::values(const K& key) const
{
  int i;
  ehashitem<K,T>* hitem;

  if (size() > (3u*(_hashmask+1))>>2u) resizehash();

  i=hash(key);
  hitem=_hashitems[i];

  while (hitem!=0x00){
    if (key == hitem->key)   // there is no collision
      return(*hitem->value);
    hitem=hitem->next;
  }

  lerror("ebasichashmap: key not found");
  throw "ebasichashmap: key not found";
//  return(*(T*)0x00);
}

template <class K,class T,unsigned int (*hashfunc)(const K&)>
T& ebasichashmap<K,T,hashfunc>::values(const K& key)
{
  int i;
  ehashitem<K,T>* hitem;

  if (size() > (3u*(_hashmask+1))>>2u) resizehash();

  i=hash(key);
  hitem=_hashitems[i];

  while (hitem!=0x00){
    if (key == hitem->key)   // there is no collision
      return(*hitem->value);
    hitem=hitem->next;
  }
  
  lerror("ebasichashmap: key not found");
  throw "ebasichashmap: key not found";
//  return(*(T*)0x00);
}

/*
template <class K,class T>
T& ebasichashmap<K,T,hashfunc>::operator[](int ind)
{
  int i;
  ehashitem<K,T>* hitem;

  i=hash(_keys.at(ind));
  hitem=_hashitems[i];

  while (hitem!=0x00){
    if (_keys.at(ind) == hitem->key)   // there is no collision
      return(*hitem->value);
    hitem=hitem->next;
  }

  ldie("ebasichashmap: index out of bounds: "+estr(ind));
  return(*(T*)0x00);
}
//#pragma GCC diagnostic warning "-Wreturn-type"
*/

/*
template <unsigned int (*hashfunc)(const evar&)>
void ebasichashmap<evar,evar,hashfunc>::addvar(evar& key,evar& var)
{
  add(key,var);
}

template <unsigned int (*hashfunc)(const evar&)>
evar ebasichashmap<evar,evar,hashfunc>::getvar(int i) const
{
  return(evar());
//  return(values(i));
}

template <unsigned int (*hashfunc)(const evar&)>
evar ebasichashmap<evar,evar,hashfunc>::getvarkey(int i) const
{
  return(evar());
//  return(keys(i));
}
*/

#endif

