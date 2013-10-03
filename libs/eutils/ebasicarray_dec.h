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

   void init(int count);
   void init(int count,const T& initval);

   ebasicarray<T> &operator=(const ebasicarray<T> &arr);
   inline unsigned int size() const { return(vector<T>::size()); }

   inline ebasicarray<T>& operator+=(const T &value) { push_back(value); return(*this); }

   inline T& add(const T& value) { push_back(value); return(vector<T>::back()); }

   inline void erase(int i) { vector<T>::erase(vector<T>::begin()+i); }
   inline void insert(int i,const T& value) { vector<T>::insert(vector<T>::begin()+i,value); }

   inline T& operator[](int i) { return(vector<T>::operator[](i)); }
   inline const T& operator[](int i) const { return(vector<T>::operator[](i)); }

   ebasicarray<T> operator[](const eintarray& i) const;

   inline T& at(int i) { return(operator[](i)); }
   inline const T& at(int i) const { return(operator[](i)); }

   inline void swap(int i,int j){ T tmp; tmp=at(j); at(j)=at(i); at(i)=tmp; }

   ebasicarray<T>& operator+=(const ebasicarray<T>& arr);
   ebasicarray<T>& operator-=(const ebasicarray<T>& arr);

   ebasicarray<T> operator+(const ebasicarray<T>& arr) const;
   ebasicarray<T> operator-(const ebasicarray<T>& arr) const;

//   inline const T operator[](int i) const { return( ((vector<T>)(*this))[i] ); }
//   inline T &operator[](int i){ return( ((vector<T>)(*this))[i] ); }

//   ebasicarray<T> operator[](const ebasicarray<int> &iarr);
   ebasicarray<T> subset(int i,int l) const;
 
   inline void clear() { vector<T>::clear(); }

   static bool equal(const T& a,const T& b){ return(a==b); }

   int find(const T& value,int i=0,bool (*match)(const T&,const T&)=equal) const;   
   eintarray findall(const T &value,int i=0,bool (*match)(const T&,const T&)=equal) const;
   eintarray findall(const ebasicarray<T> &value,int i=0,bool (*match)(const T&,const T&)=equal) const;
   
//   ebasicarray<T> afindall(const T &value,int i=0,bool (*match)(const T&,const T&)=equal) const;
   ebasicarray<T> afindall(const ebasicarray<T> &value,int i=0,bool (*match)(const T&,const T&)=equal) const;

//   void erase(const ebasicarray<int> &iarr);
   friend ostream &operator<< <>(ostream &stream,const ebasicarray<T> &var);

   virtual void addvar(evar& var);
   virtual evar getvar(int i) const;

   void serial(estr& data) const;
   int unserial(const estr& data,int i=0);
};

#endif

