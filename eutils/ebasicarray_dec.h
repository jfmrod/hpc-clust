#ifndef EBASICARRAY_DEC_H
#define EBASICARRAY_DEC_H

#include "eutils.h"

#include<vector>
#include<iostream>

using namespace std;

#include "ebasearray.h"

template <class T>
class ebasicarray;

typedef ebasicarray<int> eintarray;
typedef ebasicarray<float> efloatarray;
typedef ebasicarray<double> edoublearray;

template <class T>
ostream &operator<<(ostream &stream,const ebasicarray<T> &var);

template <class T>
class ebasicarray : public vector<T>,public ebasearray
{
 public:
   ebasicarray();
   ebasicarray(const ebasicarray<T> &arr);
   ebasicarray(const T& value);
   ebasicarray(const T& val1,const T& val2);
   ebasicarray(const T& val1,const T& val2,const T& val3);
   ebasicarray(const T& val1,const T& val2,const T& val3,const T& val4);
   ebasicarray(const T& val1,const T& val2,const T& val3,const T& val4,const T& val5);
   ebasicarray(const T& val1,const T& val2,const T& val3,const T& val4,const T& val5,const T& val6);
   ebasicarray(const T& val1,const T& val2,const T& val3,const T& val4,const T& val5,const T& val6,const T& val7);
   ~ebasicarray();

   void init(size_t count);
   void init(size_t count,const T& initval);

   ebasicarray<T> &operator=(const ebasicarray<T> &arr);
   inline size_t size() const { return(vector<T>::size()); }

   inline ebasicarray<T>& operator+=(const T &value) { push_back(value); return(*this); }

   inline T& add(const T& value) { push_back(value); return(vector<T>::back()); }

   inline void erase(size_t i) { vector<T>::erase(vector<T>::begin()+i); }
   inline void insert(size_t i,const T& value) { vector<T>::insert(vector<T>::begin()+i,value); }

   inline T& operator[](size_t i) { return(vector<T>::operator[](i)); }
   inline const T& operator[](size_t i) const { return(vector<T>::operator[](i)); }

   ebasicarray<T> operator[](const eintarray& i) const;

   inline T& at(size_t i) { return(operator[](i)); }
   inline const T& at(size_t i) const { return(operator[](i)); }

   inline void swap(size_t i,size_t j){ T tmp; tmp=at(j); at(j)=at(i); at(i)=tmp; }

   ebasicarray<T>& operator+=(const ebasicarray<T>& arr);
   ebasicarray<T>& operator-=(const ebasicarray<T>& arr);

   ebasicarray<T> operator+(const ebasicarray<T>& arr) const;
   ebasicarray<T> operator-(const ebasicarray<T>& arr) const;

//   inline const T operator[](int i) const { return( ((vector<T>)(*this))[i] ); }
//   inline T &operator[](int i){ return( ((vector<T>)(*this))[i] ); }

//   ebasicarray<T> operator[](const ebasicarray<int> &iarr);
   ebasicarray<T> subset(size_t i,size_t l) const;
 
   inline void clear() { vector<T>::clear(); }

   static bool equal(const T& a,const T& b){ return(a==b); }

   long find(const T& value,size_t i=0,bool (*match)(const T&,const T&)=equal) const;   
   eintarray findall(const T &value,size_t i=0,bool (*match)(const T&,const T&)=equal) const;
   eintarray findall(const ebasicarray<T> &value,size_t i=0,bool (*match)(const T&,const T&)=equal) const;
   
//   ebasicarray<T> afindall(const T &value,int i=0,bool (*match)(const T&,const T&)=equal) const;
   ebasicarray<T> afindall(const ebasicarray<T> &value,size_t i=0,bool (*match)(const T&,const T&)=equal) const;

//   void erase(const ebasicarray<int> &iarr);
   friend ostream &operator<< <>(ostream &stream,const ebasicarray<T> &var);

   virtual void addvar(evar& var);
   virtual evar getvar(size_t i) const;

   void serial(estr& data) const;
   size_t unserial(const estr& data,size_t i=0);
};

#endif

