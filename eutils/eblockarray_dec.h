#ifndef EBLOCKARRAY_DEC_H
#define EBLOCKARRAY_DEC_H

#include "eutils.h"

#include<vector>
#include<iostream>

using namespace std;

#include "ebasicarray_dec.h"

template <class T>
class eblockarray
{
 private:
  long int blocksize;
  long int count;
 public:
  ebasicarray<T*> blocks;

  eblockarray();
  ~eblockarray();

  void clear();

  void add(const T& sdist);
  inline long int size() const { return(count); }

  void swap(long int i,long int j);
  void sort();

  T& operator[](long int i);
  const T& operator[](long int i) const;
  eblockarray<T>& merge(eblockarray& barr);
  inline eblockarray<T>& operator+=(eblockarray& barr){ merge(barr); return(*this); }

  inline T* lastblock() { return(blocks[blocks.size()-1]); }
  inline const T* lastblock() const { return(blocks[blocks.size()-1]); }
};

template <class T>
ostream &operator<<(ostream &stream,const eblockarray<T> &var);

#endif

