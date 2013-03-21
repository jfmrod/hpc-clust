#ifndef EHASHMAP_H
#define EHASHMAP_H

#include "eutils.h"

#include "ehashmap_dec.h"
#include "evar_dec.h"




template <class K,class T,size_t (*hashfunc)(const K&)>
void ehashmap<K,T,hashfunc>::addvar(evar& evarkey,evar& var)
{
  var.retain();
  addref(evarkey.get<K>(),&var.get<T>());
}
template <class K,class T,size_t (*hashfunc)(const K&)>
evar ehashmap<K,T,hashfunc>::getvar(size_t i) const
{
  return(evar((T&)values(i)));
}
template <class K,class T,size_t (*hashfunc)(const K&)>
evar ehashmap<K,T,hashfunc>::getvarByKey(const evar& var) const
{
  if (var.getTypeid()==typeid(K))
    return(evar((T&)values(var.get<K>())));
  return(evar());
}
template <class K,class T,size_t (*hashfunc)(const K&)>
evar ehashmap<K,T,hashfunc>::getvarkey(size_t i) const
{
  return(evar((K&)keys(i)));
}


#include "logger.h"

template <class K,class T>
ehashitem<K,T>::ehashitem(const K& _key,T* _value,ehashitem<K,T>* _next):key(_key),value(_value),next(_next),prev(0x00)
{
  if (next)
    next->prev=this;
}

template <class K,class T>
ehashitem<K,T>::~ehashitem()
{
/*
  if (next)
    next->prev=prev;
  if (prev)
    prev->next=next;
*/
}

const size_t HASH_INIT_MASK=0xFF;

template <class K,class T,size_t (*hashfunc)(const K&)>
ehashmap<K,T,hashfunc>::ehashmap()
{
  _hashmask = HASH_INIT_MASK;
  _hashitems=new ehashitem<K,T>*[_hashmask+1];
  size_t i;
  for (i=0; i<_hashmask+1; ++i)
    _hashitems[i]=0x00;
}

template <class K,class T,size_t (*hashfunc)(const K&)>
ehashmap<K,T,hashfunc>::ehashmap(const ehashmap<K,T,hashfunc>& oldhm)
{
  _hashmask = oldhm._hashmask;
  _keys = oldhm._keys;
  _hashitems=new ehashitem<K,T>*[_hashmask+1];

  ehashitem<K,T> *oldhmitem;
  size_t i;
  for (i=0; i<_hashmask+1; ++i){
    _hashitems[i]=0x00;
    for (oldhmitem=oldhm._hashitems[i]; oldhmitem!=0x00; oldhmitem=oldhmitem->next)
      _hashitems[i]=new ehashitem<K,T>(oldhmitem->key,oldhmitem->value,_hashitems[i]);
  }
}

template <class K,class T,size_t (*hashfunc)(const K&)>
ehashmap<K,T,hashfunc>::~ehashmap()
{
  clear();
  delete[] _hashitems;
}

template <class K,class T,size_t (*hashfunc)(const K&)>
void ehashmap<K,T,hashfunc>::reserve(size_t i)
{
  _keys.reserve(i);
  
  size_t a;
  a=0x01;
  while (i>0){
    i=i>>1;a=(a<<1)|0x01;
  }
  
  resizehash(a);
}

template <class K,class T,size_t (*hashfunc)(const K&)>
void ehashmap<K,T,hashfunc>::resizehash(size_t newmask) const
{
  size_t thashmask;
  ehashitem<K,T> **thashitems;

  ldinfo("resizing hash table");

  if (newmask==0 && newmask < _hashmask) 
    thashmask = (_hashmask << 1)|0x01;
  else
    thashmask = newmask;

  thashitems=new ehashitem<K,T>*[thashmask+1];
  size_t i;
  for (i=0; i<thashmask+1; ++i)
    thashitems[i]=0x00;

  size_t khash;
  ehashitem<K,T>* hitem;

  for (i=0; i<_keys.size(); ++i){
    khash = hashfunc(_keys.at(i)) & thashmask;
    hitem = gethashitem(khash & _hashmask,_keys.at(i));

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

template <class K,class T,size_t (*hashfunc)(const K&)>
ehashitem<K,T>* ehashmap<K,T,hashfunc>::gethashitem(size_t khash,const K& key) const
{
  ehashitem<K,T>* hitem;
  hitem = _hashitems[khash];
  while (hitem != 0x00){
    if (hitem->key == key)
      return(hitem);
    hitem=hitem->next;
  }
  lderror("ehashmap: did not find key");
  return(0x00); 
}


template <class K,class T,size_t (*hashfunc)(const K&)>
void ehashmap<K,T,hashfunc>::clear()
{
  size_t i;
  ehashitem<K,T> *hitem;
  for (i=0; i<_hashmask+1; ++i){
    while (_hashitems[i]){
      hitem=_hashitems[i];
      _hashitems[i]=hitem->next;
      delete hitem->value;
      delete hitem;
    }
  }
  _keys.clear();
    
/*
  while (_keys.size()){
    i=hash(_keys.at(0));
    if (_hashitems[i]){
      hitem=_hashitems[i];
      _hashitems[i]=hitem->next;
      delete hitem->value;
      delete hitem;
    }
    _keys.erase(0);
  }
*/
}

template <class K,class T,size_t (*hashfunc)(const K&)>
bool ehashmap<K,T,hashfunc>::exists(const K& key) const
{
  size_t i;
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

template <class K,class T,size_t (*hashfunc)(const K&)>
void ehashmap<K,T,hashfunc>::erase(const K& key)
{
  size_t i;
  ehashitem<K,T>* hitem;

  i=hash(key);
  hitem=_hashitems[i];

  long j;
  while (hitem!=0x00){
    if (key == hitem->key){
      j=_keys.find(key);
      if (j!=-1) _keys.erase(j);
      if (hitem->prev) hitem->prev->next=hitem->next;
      else _hashitems[i]=hitem->next;
      if (hitem->next) hitem->next->prev=hitem->prev;
      delete hitem->value;
      delete hitem;
      return;
    }
    hitem=hitem->next;
  }
  lddie("tried to delete key from hashmap that does not exist");
  // non existent value
}

template <class K,class T,size_t (*hashfunc)(const K&)>
void ehashmap<K,T,hashfunc>::erase(size_t i)
{
  erase(_keys.at(i));
}

template <class K,class T,size_t (*hashfunc)(const K&)>
long ehashmap<K,T,hashfunc>::findkey(const K& key,size_t pos) const
{
  return(_keys.find(key,pos));
}

template <class K,class T,size_t (*hashfunc)(const K&)>
ehashmap<K,T,hashfunc>& ehashmap<K,T,hashfunc>::operator+=(const ehashmap<K,T,hashfunc>& hm)
{
  size_t i;
  for (i=0; i<hm.size(); ++i)
    add(hm.keys(i),hm.values(hm.keys(i)));
  return(*this);
}


template <class K,class T,size_t (*hashfunc)(const K&)>
T& ehashmap<K,T,hashfunc>::addref(const K& key,T* value)
{
  size_t i;
  ehashitem<K,T>* hitem;

  if (_keys.size() > (3u*(_hashmask+1))>>2u) resizehash();

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
  _keys.add(key);
  _hashitems[i]=new ehashitem<K,T>(key,value,_hashitems[i]);
  return(*_hashitems[i]->value);
}

template <class K,class T,size_t (*hashfunc)(const K&)>
T& ehashmap<K,T,hashfunc>::add(const K& key,const T& value)
{
  size_t i;
  ehashitem<K,T>* hitem;

  if (_keys.size() > (3u*(_hashmask+1))>>2u) resizehash();

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
  _keys.add(key);
  _hashitems[i]=new ehashitem<K,T>(key,new T(value),_hashitems[i]);
  return(*_hashitems[i]->value);
/*
  operator[](key) = value;
  return(operator[](key));
*/
}

template <class K,class T,size_t (*hashfunc)(const K&)>
const T& ehashmap<K,T,hashfunc>::operator[](const K& key) const
{
  size_t i;
  ehashitem<K,T>* hitem;

  if (_keys.size() > (3u*(_hashmask+1))>>2u) resizehash();

  i=hash(key);
  hitem=_hashitems[i];

  while (hitem!=0x00){
    if (key == hitem->key)   // there is no collision
      return(*hitem->value);
    hitem=hitem->next;
  }

  // non existent value
  _keys.add(key);
  _hashitems[i]=new ehashitem<K,T>(key,new T,_hashitems[i]);
  return(*_hashitems[i]->value);
}

template <class K,class T,size_t (*hashfunc)(const K&)>
const T& ehashmap<K,T,hashfunc>::operator[](size_t ind) const
{
  size_t i;
  ehashitem<K,T>* hitem;

  lddieif(ind > _keys.size(),"ehashmap: index out of bounds");
  i=hash(_keys.at(ind));
  hitem=_hashitems[i];

  while (hitem!=0x00){
    if (_keys.at(ind) == hitem->key)   // there is no collision
      return(*hitem->value);
    hitem=hitem->next;
  }

  ldie("ehashmap: index out of bounds: "+estr(ind));
//  return(*(T*)0x00);
}

template <class K,class T,size_t (*hashfunc)(const K&)>
T& ehashmap<K,T,hashfunc>::operator[](const K& key)
{
  size_t i;
  ehashitem<K,T>* hitem;

  if (_keys.size() > (3u*(_hashmask+1))>>2u) resizehash();

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
  _keys.add(key);
  _hashitems[i]=new ehashitem<K,T>(key,new T,_hashitems[i]);
  return(*_hashitems[i]->value);
}

//#pragma GCC diagnostic ignored "-Wreturn-type"
template <class K,class T,size_t (*hashfunc)(const K&)>
const T& ehashmap<K,T,hashfunc>::values(const K& key) const
{
  size_t i;
  ehashitem<K,T>* hitem;

  if (_keys.size() > (3u*(_hashmask+1))>>2u) resizehash();

  i=hash(key);
  hitem=_hashitems[i];

  while (hitem!=0x00){
    if (key == hitem->key)   // there is no collision
      return(*hitem->value);
    hitem=hitem->next;
  }

  lerror("ehashmap: key not found");
  throw "ehashmap: key not found";
//  return(*(T*)0x00);
}

template <class K,class T,size_t (*hashfunc)(const K&)>
T& ehashmap<K,T,hashfunc>::values(const K& key)
{
  size_t i;
  ehashitem<K,T>* hitem;

  if (_keys.size() > (3u*(_hashmask+1))>>2u) resizehash();

  i=hash(key);
  hitem=_hashitems[i];

  while (hitem!=0x00){
    if (key == hitem->key)   // there is no collision
      return(*hitem->value);
    hitem=hitem->next;
  }

  lerror("ehashmap: key not found");
  throw "ehashmap: key not found";
//  return(*(T*)0x00);
}

template <class K,class T,size_t (*hashfunc)(const K&)>
T& ehashmap<K,T,hashfunc>::operator[](size_t ind)
{
  size_t i;
  ehashitem<K,T>* hitem;

  i=hash(_keys.at(ind));
  hitem=_hashitems[i];

  while (hitem!=0x00){
    if (_keys.at(ind) == hitem->key)   // there is no collision
      return(*hitem->value);
    hitem=hitem->next;
  }

  ldie("ehashmap: index out of bounds: "+estr(ind));
  return(*(T*)0x00);
}
//#pragma GCC diagnostic warning "-Wreturn-type"

/*
template <unsigned int (*hashfunc)(const evar&)>
void ehashmap<evar,evar,hashfunc>::addvar(evar& key,evar& var)
{
  add(key,var);
}

template <unsigned int (*hashfunc)(const evar&)>
evar ehashmap<evar,evar,hashfunc>::getvar(int i) const
{
  return(values(i));
}

template <unsigned int (*hashfunc)(const evar&)>
evar ehashmap<evar,evar,hashfunc>::getvarkey(int i) const
{
  return(keys(i));
}
*/

#endif

