#ifndef EARRAYOF_H
#define EARRAYOF_H

#include "eutils.h"

#include "earrayof_dec.h"
#include "evar_dec.h"

template <class T,class K>
void earrayof<T,K>::addvar(evar& evarkey,evar& var)
{
  var.retain();
  addref(evarkey.get<K>(),&var.get<T>());
}
template <class T,class K>
evar earrayof<T,K>::getvar(size_t i) const
{
  return(evar((T&)values(i)));
//  return(evar(values(i)));
}
template <class T,class K>
evar earrayof<T,K>::getvarByKey(const evar& var) const
{
  if (var.getTypeid()==typeid(K))
    return(evar((T&)earrayof<T,K>::at(var.get<K>())));
  return(evar());
}
template <class T,class K>
evar earrayof<T,K>::getvarkey(size_t i) const
{
  return(evar((K&)keys(i)));
}

template <>
void earrayof<evar,evar>::addvar(evar& key,evar& var);
template <>
evar earrayof<evar,evar>::getvar(size_t i) const;
template <>
evar earrayof<evar,evar>::getvarByKey(const evar& key) const;
template <>
evar earrayof<evar,evar>::getvarkey(size_t i) const;


#include "eintarray.h"

template <class T,class K>
eintarray earrayof<T,K>::findall(const T& value,size_t i,bool (*match)(T* const &a,T* const &b)) const
    { return( _values.findall((T*)&value,i,match) ); }

template <class T,class K>
eintarray earrayof<T,K>::findall(const earrayof<T,K>& value,size_t i,bool (*match)(T* const &a,T* const &b)) const
    { return( _values.findall(value._values,i,match) ); }

template <class T,class K>
eintarray earrayof<T,K>::findkeyall(const K& key,size_t i,bool (*match)(K* const &a,K* const &b)) const
    { return( _keys.findall((K*)&key,i,match) ); }

template <class T,class K>
eintarray earrayof<T,K>::findkeyall(const earrayof<T,K>& keys,size_t i,bool (*match)(K* const &a,K* const &b)) const
    { return( _keys.findall(keys._keys,i,match) ); }


template <class T,class K>
bool earrayof<T,K>::equal(T* const &a, T* const &b)
  { return(*a==*b); }

template <class T,class K>
bool earrayof<T,K>::equalkey(K* const &a,K* const &b)
  { if (!a || !b) return(false); return(*a == *b); }



template <class T,class K>
ostream& operator<<(ostream& stream,const earrayof<T,K>& var)
{
  stream<<"{ "<<endl;
  size_t i;

  if (var.size()){
    for (i=0; i<var.size()-1; ++i){
      if (var._keys[i])
        stream<<var.keys(i)<<" = ";
      stream<<var.values(i)<<", "<<endl;
    }
    if (var._keys[i])
      stream<<var.keys(i)<<" = ";
    stream<<var.values(i)<<endl;
  }
  stream<<" }";
  return(stream);
}

template <class T,class K>
earrayof<T,K>::earrayof()
{
}

template <class T,class K>
earrayof<T,K>::earrayof(const earrayof<T,K>& a)
{
  *this = a;
}

/*
template <class T,class K>
earrayof<T,K>::earrayof(const ebasicarray<T> &a)
{
  add(a);
}
*/

template <class T,class K>
earrayof<T,K>::earrayof(const T& a)
{
  add(a);
}

template <class T,class K>
earrayof<T,K>::~earrayof()
{
  clear();
}

template <class T,class K>
T& earrayof<T,K>::add_keysorted(const K& key,const T& value)
{
  size_t i;

  for (i=0; i<size() && *_keys[i]<key; ++i);

  _keys.insert(i,new K(key));
  _values.insert(i,new T(value));
  return(*_values[i]);
}

template <class T,class K>
T& earrayof<T,K>::addref(const K& key,T *value)
{
  _keys.add(new K(key));  
  _values.add(value);
  return(*_values.back());
}

template <class T,class K>
T& earrayof<T,K>::add(const K& key,const T& value)
{
  _keys.add(new K(key));
  _values.add(new T(value));
  return(*_values.back());
}

template <class T,class K>
T& earrayof<T,K>::insert(size_t i,const T& value)
{
  _keys.insert(i,new K());
  _values.insert(i,new T(value));
  return(*_values.back());
}

template <class T,class K>
T& earrayof<T,K>::insert(size_t i,const K& key,const T& value)
{
  _keys.insert(i,new K(key));
  _values.insert(i,new T(value));
  return(*_values.back());
}

/*
template <class T,class K>
void earrayof<T,K>::add(const ebasicarray<T> &value)
{
  for (i=0; i<value.size(); ++i
  _values += value;
  int i;
  for (i=0; i<value.size(); ++i) _keys.add(0x00);
}
*/
	
template <class T,class K>
T& earrayof<T,K>::add(const T& value)
{
  _keys.add(new K());
  _values.add(new T(value));
  return(*_values.back());
}

/*
template <class T,class K>
T &earrayof<T,K>::operator[](const int &i)
{
  return(_values[i]);
}
*/

/*
// removed because causes problems when the key of an array is an eintarray
// gives an overloading conflict with: operator[](const K& arr) const where K=eintarray
template <class T,class K>
earrayof<T,K> earrayof<T,K>::operator[](const eintarray& arr) const
{
  earrayof<T,K> res;
  int i;
  for (i=0; i<arr.size(); ++i){
    if (arr[i]<0 || arr[i]>=size()) continue;
    res.add(keys(arr[i]),values(arr[i]));
  }
  return(res); 
}
*/


template <class T,class K>
earrayof<T,K> earrayof<T,K>::operator[](const eintarray& iarr) const
{
  earrayof<T,K> tmparr;
  size_t i;
  for (i=0; i<iarr.size(); ++i){
//    if (iarr[i]<0 || iarr[i]>=size()) { lerror("index out of founds: "+estr(iarr[i])); return(tmparr); }
    tmparr.add(keys(iarr[i]),values(iarr[i]));
  }
  return(tmparr);
}


template <class T,class K>
const T& earrayof<T,K>::operator[](size_t i) const
{
  return(*_values[i]);
}

template <class T,class K>
T& earrayof<T,K>::operator[](size_t i)
{
  return(*_values[i]);
}

template <class T,class K>
const T& earrayof<T,K>::operator[](const K& key) const
{
  size_t i;
  for (i=0; i<size(); ++i){
    if (_keys[i] && *_keys[i] == key)
      return(*_values[i]);
  }

//  return(add(key,T()));
  throw "key not found"; 
}

template <class T,class K>
T& earrayof<T,K>::operator[](const K& key)
{
  size_t i;
  for (i=0; i<size(); ++i){
    if (_keys[i] && *_keys[i] == key)
      return(*_values[i]);
  }
 
  // add a new entry on the list if this one does not exist 
  return(add(key,T()));
}

template <class T,class K>
const T& earrayof<T,K>::at(const K& key) const
{
  size_t i;
  for (i=0; i<size(); ++i){
    if (_keys[i] && *_keys[i] == key)
      return(*_values[i]);
  }

//  lerror("value not found");
  throw "value not found";
}

template <class T,class K>
T& earrayof<T,K>::at(const K& key)
{
  size_t i;
  for (i=0; i<size(); ++i){
    if (_keys[i] && *_keys[i] == key)
      return(*_values[i]);
  }
 
  // add a new entry on the list if this one does not exist 
//  lerror("value not found");
  throw "value not found";
}




template <class T,class K>
earrayof<T,K>& earrayof<T,K>::operator-=(const earrayof<T,K>& a)
{
  size_t i,j;
  for (i=0; i<size(); ++i){
    for (j=0; j<a.size(); ++j){
      if ((!_keys[i] && !a._keys[j] || _keys[i] && a._keys[j] && *_keys[i] == *a._keys[j]) &&
          *_values[i] == *a._values[j])
      { erase(i); --i; break; }
    }
  }
  return(*this);
}

template <class T,class K>
earrayof<T,K>& earrayof<T,K>::operator+=(const earrayof<T,K>& a)
{
  size_t i;
  for (i=0; i<a.size(); ++i){
    if (a._keys[i])
      _keys.add(new K(*a._keys[i]));
    else
      _keys.add(0x00);
    _values.add(new T(*a._values[i]));
  }
  return(*this);
}

template <class T,class K>
earrayof<T,K> earrayof<T,K>::subset(long i,long l) const
{
  earrayof<T,K> tmpa;
 
  if (i<0) i+=size();
  if (l<0) l+=size()-i+1;

  if (i<0 || l<0 || !size()) return(tmpa);

  l+=i;
  for(;i<l;++i){
    if (_keys[i])
      tmpa.add(*_keys[i],*_values[i]);
    else
      tmpa.add(*_values[i]);
  }
}



template <class T,class K>
earrayof<T,K>  earrayof<T,K>::operator+(const earrayof<T,K>& a) const
{
  earrayof<T,K> tmp;
  
  tmp  = *this;
  tmp += a;
  return(tmp);
}

template <class T,class K>
earrayof<T,K>  earrayof<T,K>::operator-(const earrayof<T,K>& a) const
{
  earrayof<T,K> tmp;
  
  tmp  = *this;
  tmp -= a;
  return(tmp);
}

template <class T,class K>
earrayof<T,K> &earrayof<T,K>::operator=(const earrayof<T,K>& a)
{
  if (this==&a) return(*this);

  clear();
  *this += a;
  return(*this);
}


template <class T,class K>
void earrayof<T,K>::remove(size_t i)
{
  if (_keys[i]) delete _keys[i];
  delete _values[i];
  _keys.erase(i);
  _values.erase(i);
}

template <class T,class K>
void earrayof<T,K>::clear()
{
  size_t i;
  for (i=0; i<_keys.size(); ++i)
    if(_keys[i]) delete _keys[i];
  for (i=0; i<_values.size(); ++i)
    delete _values[i];
  _keys.clear();
  _values.clear();
}

#endif

