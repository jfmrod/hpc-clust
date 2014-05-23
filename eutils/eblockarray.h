#ifndef EBLOCKARRAY_H
#define EBLOCKARRAY_H

#include "eutils.h"

#include<vector>
#include<iostream>

using namespace std;

#include "ebasearray.h"
#include "eblockarray_dec.h"

#include "evar_dec.h"
#include "estr.h"
#include "evarclass_dec.h"
#include "evartype.h"

#include "logger.h"
#include "ebasicarray.h"

/*
template <>
void ebasicarray<evar*>::addvar(evar& var);
template <>
evar ebasicarray<evar*>::getvar(int i) const;
*/

template <class T>
eblockarray<T>::eblockarray(): blocksize(100000), count(0) { blocks.add(new T[blocksize]); }

template <class T>
eblockarray<T>::~eblockarray()
{
  long int i;
  for (i=0; i<blocks.size(); ++i)
    if (blocks[i]) delete[] blocks[i];
}

template <class T>
void eblockarray<T>::add(const T& sdist)
{
  lastblock()[count%blocksize]=sdist;
  ++count;
  if (count%blocksize==0)
    blocks.add(new T[blocksize]);
}

template <class T>
const T& eblockarray<T>::operator[](long int i) const
{
  ldieif(i/blocksize>=blocks.size() || i>=size() || i<0,"out of range: blocks.size: "+estr(blocks.size())+" i/blocksize: "+estr(i/blocksize)+" i: "+estr(i)+" size: "+size());
  return(blocks[i/blocksize][i%blocksize]);
}

template <class T>
T& eblockarray<T>::operator[](long int i)
{
  ldieif(i/blocksize>=blocks.size() || i>=size() || i<0,"out of range: blocks.size: "+estr(blocks.size())+" i/blocksize: "+estr(i/blocksize)+" i: "+estr(i)+" size: "+size());
//  ldieif(i/blocksize>=blocks.size(),"out of range: blocks.size: "+estr(blocks.size())+" i/blocksize: "+estr(i/blocksize));
  return(blocks[i/blocksize][i%blocksize]);
}

template <class T>
inline void addarray(T *arr1,long int& i1,long int n1,T *arr2,long int& i2,long int n2)
{
  for (; i1<n1 && i2<n2; ++i1,++i2)
    arr1[i1]=arr2[i2];
}

template <class T>
eblockarray<T>& eblockarray<T>::merge(eblockarray& barr)
{
  if (barr.size()==0) return(*this);

  long int i,j,k;
  T *tmparr;
  if (blocksize-(count%blocksize) >= barr.count%blocksize){
    i=count%blocksize; j=0;
    addarray(lastblock(),i,blocksize,barr.lastblock(),j,barr.count%blocksize);    
    count+=barr.count;
    if (barr.blocks.size()>1){
      tmparr=lastblock();
      blocks[blocks.size()-1]=barr.blocks[0];
      barr.blocks[0]=0x00;
      for (j=1; j<barr.blocks.size()-1; ++j){
        blocks.add(barr.blocks[j]);
        barr.blocks[j]=0x00;
      }
      blocks.add(tmparr);
    }
    if (count%blocksize==0)
      blocks.add(new T[blocksize]);
  }else{
    i=count%blocksize; j=0;
    addarray(lastblock(),i,blocksize,barr.lastblock(),j,barr.count%blocksize);
    for (k=0; k<barr.blocks.size()-1; ++k){
      blocks.add(barr.blocks[k]);
      barr.blocks[k]=0x00;
    }

    blocks.add(new T[blocksize]);
    i=0;
    addarray(lastblock(),i,blocksize,barr.lastblock(),j,barr.count%blocksize);
    count+=barr.count;
  }

  ldieif (count/blocksize>=blocks.size() || count/blocksize+1<blocks.size(),"error in merge: "+estr(count/blocksize)+" "+estr(blocks.size()));

  barr.clear();
  return(*this);
}

template <class T>
void eblockarray<T>::clear()
{
  long int i;
  for (i=0; i<blocks.size(); ++i)
    if (blocks[i]) delete[] blocks[i];
  blocks.clear();
  count=0;
  blocks.add(new T[blocksize]);
}

template <class T>
void eblockarray<T>::swap(long int i,long int j)
{
  ldieif(i/blocksize>=blocks.size() || i>=size() || i<0,"out of range: blocks.size: "+estr(blocks.size())+" i/blocksize: "+estr(i/blocksize)+" i: "+estr(i)+" size: "+size());
  ldieif(j/blocksize>=blocks.size() || j>=size() || j<0,"out of range: blocks.size: "+estr(blocks.size())+" i/blocksize: "+estr(j/blocksize)+" j: "+estr(j)+" size: "+size());
  ldieif(i==j,"swaping same item: "+estr(i)+" == "+estr(j));

  T tmp;
  tmp=operator[](i);
  operator[](i)=operator[](j);
  operator[](j)=tmp;
}

template <class T>
void eblockarray<T>::sort()
{
  long int i;
  long int root,child;
  long int tmpsize;

  // heapify
  tmpsize=size();
  for (i=size()/2; i>=0; --i){
    root=i;
    while (root<<1 < tmpsize){
      child=root<<1;
      if (child+1<tmpsize && operator[](child) < operator[](child+1))
        ++child;
      if (operator[](root) < operator[](child)){
        swap(root,child);
        root=child;
      } else
        break;
    }
  }

  // build sorted array
  for (tmpsize=size()-1; tmpsize>0; --tmpsize){
    swap(0,tmpsize);
    root=0;
    while (root<<1 < tmpsize){
      child=root<<1;
      if (child+1<tmpsize && operator[](child) < operator[](child+1))
        ++child;
      if (operator[](root) < operator[](child)){
        swap(root,child);
        root=child;
      } else
        break;
    }
  }
}



template <class T>
ostream &operator<<(ostream &stream,const eblockarray<T> &var)
{
  stream<<"{ "<<endl;
  int i;

  if (var.size()){
    for (i=0; i<var.size()-1; ++i){
      stream<<var[i]<<", "<<endl;
    }
    stream<<var[i]<<endl;
  }
  stream<<" }";
  return(stream);
}

#endif

