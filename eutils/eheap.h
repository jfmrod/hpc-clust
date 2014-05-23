#ifndef EHEAP_H
#define EHEAP_H

#include "eutils.h"

#include "earray.h"
#include "eintarray.h"

#undef check

/*
template <class T>
class eheap : public earray<T>
{
 private:
  int _size;
//  earray<T> *_arr;
//  bool internal;
//  int _size;
 public:
  eheap();
  ~eheap();
  
  void add(const T& value);
  void delroot();
  void check(int i);

  bool exists(const T& value);

//  inline T& operator[](int i){ return(_arr->operator[](i)); }
  inline T& root() { return(earray<T>::operator[](0)); }
  inline int size() { return(_size); }
};
*/

//#define SWAP(arr,i,j) { int tmpi; tmpi=arr[i]; arr[i]=arr[j]; arr[j]=tmpi; }


template <class T>
eintarray mergesort(T& arr)
{
  eintarray res;
  res.init(arr.size());
  int i;
  for (i=0; i<res.size(); ++i)
    res[i]=i;

  for (i=0; i<arr.size(); i+=2){
    if (arr[res[i]]>arr[res[i+1]])
      res.swap(i,i+1);
  }

/*
  for (i=0; i<arr.size(); ++i)
    cout << arr[res[i]] << ", ";
  cout << endl;
*/

  eintarray tmparr;
  int s,l,j,r,k,e; 
  for (s=4; (s>>1)<arr.size(); s=(s<<1)){
    for (i=0; i+(s>>1)<arr.size(); i+=s){
//      cout << "i: " << i << " s: " << s << " (s>>1):" << (s>>1) <<endl;
      l=i; r=i+(s>>1);
      e=MIN(i+s,arr.size());
      for (j=i; j<e && r<e && l!=r; ++j){
/*
        for (k=0; k<arr.size(); ++k){
          if (k==l) cout << "l";
          if (k==r) cout << "r";
          if (k==j) cout << "j";
          cout << arr[res[k]] << ", ";
        }
        cout << endl;
*/
//        cout << "j: " << j << " l: " << l << "("<<arr[res[l]]<<") "<< " r: " << r << "("<<arr[res[r]]<<")"<< endl;
        if (arr[res[l]]>arr[res[r]]){
//          cout << j << "("<<arr[res[j]]<< ") <-> " << r <<"("<<arr[res[r]]<<")"<< endl;
          res.swap(j,r);
          if (l==j) l=r;
          ++r;
        }else{
          if (l!=j) {
//            cout << j << "("<<arr[res[j]]<< ") <-> " << l <<"("<<arr[res[l]]<<")"<< endl;
            res.swap(l,j);
            int tmp=res[l];
            for (k=r-1; k>l; --k)
              res[k-1]=res[k]; // move this item up
            res[r-1]=tmp;
//            for (k=l; k<r-1; ++k)
//              res.swap(k,k+1); // move this item up
//            if (l+1==r) l=j;
          }else
            ++l;
        }
      }
/*
      for (k=0; k<arr.size(); ++k){
        if (k==l) cout << "l";
        if (k==r) cout << "r";
        if (k==j) cout << "j";
        cout << arr[res[k]] << ", ";
      }
      cout << endl;
*/

      if (l!=j && l<r){
        tmparr.clear();
        tmparr.reserve(r-l);
        for (k=l; k<r; ++k)
          tmparr.add(res[k]);
        for (k=r-1; k-tmparr.size()>=j; --k)
          res[k]=res[k-tmparr.size()];
        for (k=j; k<j+tmparr.size(); ++k)
          res[k]=tmparr[k-j];
      }
/*
      if (l!=j){
        for (; l<r; ++l,++j)
          for (k=l; k>j; --k)
            res.swap(k,k-1);
      }
*/
    }
/*
    for (i=0; i<arr.size(); ++i)
      cout << arr[res[i]] << ", ";
    cout << endl;
*/
  }
  return(res);
}

template <class T>
void mergesorted(const earray<T>& arr1,const earray<T>& arr2,earray<T>& res)
{
  int i=0;
  int j=0;
  res.reserve(arr1.size()+arr2.size());
  while (i<arr1.size() && j<arr2.size()){
    if (arr1[i]<arr2[j])
      { res.add(arr1[i]); ++i; }
    else
      { res.add(arr2[j]); ++j; }
  }
  while (i<arr1.size())
    { res.add(arr1[i]); ++i; }
  while (j<arr2.size())
    { res.add(arr2[j]); ++j; }
}

template <class T>
ebasicarray<T> mergesorted(const ebasicarray<T>& arr1,const ebasicarray<T>& arr2,ebasicarray<T>& res)
{
  int i=0;
  int j=0;
  res.reserve(arr1.size()+arr2.size());
  while (i<arr1.size() && j<arr2.size()){
    if (arr1[i]<arr2[j])
      { res.add(arr1[i]); ++i; }
    else
      { res.add(arr2[j]); ++j; }
  }
  while (i<arr1.size())
    { res.add(arr1[i]); ++i; }
  while (j<arr2.size())
    { res.add(arr2[j]); ++j; }
}


template <class T>
earray<T> mergesorted(const earray<T>& arr1,const earray<T>& arr2)
{
  earray<T> res;
  int i=0;
  int j=0;
  res.reserve(arr1.size()+arr2.size());
  while (i<arr1.size() && j<arr2.size()){
    if (arr1[i]<arr2[j])
      { res.add(arr1[i]); ++i; }
    else
      { res.add(arr2[j]); ++j; }
  }
  while (i<arr1.size())
    { res.add(arr1[i]); ++i; }
  while (j<arr2.size())
    { res.add(arr2[j]); ++j; }
  return(res);
}

template <class T>
ebasicarray<T> mergesorted(const ebasicarray<T>& arr1,const ebasicarray<T>& arr2)
{
  ebasicarray<T> res;
  int i=0;
  int j=0;
  res.reserve(arr1.size()+arr2.size());
  while (i<arr1.size() && j<arr2.size()){
    if (arr1[i]<arr2[j])
      { res.add(arr1[i]); ++i; }
    else
      { res.add(arr2[j]); ++j; }
  }
  while (i<arr1.size())
    { res.add(arr1[i]); ++i; }
  while (j<arr2.size())
    { res.add(arr2[j]); ++j; }
  return(res);
}


template <class T,unsigned int (*F)(T&,long int)>
void radix256sort(T& array)
{
  if (array.size()==0) return;
  ldieif(sizeof(int)!=sizeof(F(array,0)),"size type mismatch");

  int ksize=sizeof(int);
  long int bincount[256*ksize];
  long int binipos[257*ksize];
  long int binpos[256*ksize];
  long int i;

  long int *pBinCount;
  long int *pBinPos;
  long int *pBinIPos;

  long int start,end;
  int binlevel[ksize];
  int l;
  int shift;

  l=0;
  binlevel[0]=0;
  start=0; end=array.size();

  while (1){
    shift=(ksize-1-l)*8;
//    printf("level: %i mask: %x  shift: %i\n",l,mask,shift);
    pBinCount=&bincount[256*l];
    pBinPos=&binpos[256*l];
    pBinIPos=&binipos[257*l];

    for (i=0; i<256; ++i)
      pBinCount[i]=0;

    for (i=start; i<end; ++i)
      ++pBinCount[ (F(array,i)>>shift)&0x00FFu ];

    long int tmpcount=0;
    for (i=0; i<256; ++i)
      { pBinIPos[i]=start+tmpcount; pBinPos[i]=pBinIPos[i]; tmpcount+=pBinCount[i]; }
    pBinIPos[i]=start+tmpcount;

    int k,b;
    for (b=0; b<255; ++b){
//      while (b<255 && pBinPos[b]==pBinIPos[b+1]) ++b;
      for (i=pBinPos[b]; i<pBinIPos[b+1]; ++i){
        do {
          k=(F(array,i)>>shift)&0x00FFu;
          if (k!=b)
            array.swap(i,pBinPos[k]);
          ++pBinPos[k];
        } while (k!=b);
      }
    }

    if (l==ksize-1)
      --l;

    do {
      while (binlevel[l]<256 && bincount[ binlevel[l] + 256*l ]<=1)
        ++binlevel[l];

      while (l>=0 && binlevel[l]==256)
        --l;
    } while (l>=0 && bincount[ binlevel[l] + 256*l ]<=1);

    if (l==-1) break;

    start=binipos[ binlevel[l]   + 257*l ];
    end  =binipos[ binlevel[l]+1 + 257*l ];
//    cout << "start: " << start << " end: " << end << endl;

    if (l<ksize-1) {
      ++binlevel[l];
      ++l;
      binlevel[l]=0;
    }
  } 
}

template <class T>
void lfradix256sort(T& array)
{
  if (array.size()==0) return;
  ldieif(sizeof(double)!=sizeof(array.keys(0)),"size type mismatch");

  int ksize=sizeof(double);

  long int bincount[256*ksize];
  long int binepos[256*ksize];
  long int binipos[256*ksize];
  long int binpos[256*ksize];
  long int i;

  long int *pBinCount;
  long int *pBinPos;
  long int *pBinIPos;
  long int *pBinEPos;

  long int start,end;
  int binlevel[ksize];
  int l;
  int shift;

  l=0;
  binlevel[0]=0;
  start=0; end=array.size();

  long int tmpcount;
  bool negative;

  while (1){
    shift=(ksize-1-l)*8;
//    printf("level: %i mask: %x  shift: %i\n",l,mask,shift);
    pBinCount=&bincount[256*l];
    pBinPos=&binpos[256*l];
    pBinIPos=&binipos[256*l];
    pBinEPos=&binepos[256*l];

    for (i=0; i<256; ++i)
      pBinCount[i]=0;

    for (i=start; i<end; ++i)
      ++pBinCount[ ((*reinterpret_cast<unsigned long*>(&array.keys(i)))>>shift)&0x00FFul ];

    tmpcount=0;

    if (l==0) {  // if this is the most significant byte, then we need to handle the sign bit
      for (i=255; i>=128; --i)
        { pBinPos[i]=start+tmpcount; pBinIPos[i]=pBinPos[i]; tmpcount+=pBinCount[i]; pBinEPos[i]=start+tmpcount; }
      for (i=0; i<128; ++i)
        { pBinPos[i]=start+tmpcount; pBinIPos[i]=pBinPos[i]; tmpcount+=pBinCount[i]; pBinEPos[i]=start+tmpcount;  }
    }else{
      if (binlevel[0]<128){
        for (i=0; i<256; ++i)
          { pBinPos[i]=start+tmpcount; pBinIPos[i]=pBinPos[i]; tmpcount+=pBinCount[i]; pBinEPos[i]=start+tmpcount;  }
      }else{
        for (i=255; i>=0; --i)
          { pBinPos[i]=start+tmpcount; pBinIPos[i]=pBinPos[i]; tmpcount+=pBinCount[i]; pBinEPos[i]=start+tmpcount; }
      }
    }

    int k,b;
    for (b=0; b<255; ++b){
//      while (b<255 && pBinPos[b]==pBinIPos[b+1]) ++b;
//      if (pBinIPos[b+1]==pBinIPos[256]) break;
      for (i=pBinPos[b]; i<pBinEPos[b]; ++i){
        do {
          k=((*reinterpret_cast<unsigned long*>(&array.keys(i)))>>shift)&0x00FFul;
          if (k!=b)
            array.swap(i,pBinPos[k]);
          ++pBinPos[k];
        } while (k!=b);
      }
    }

    if (l==ksize-1)
      --l;

    do {
      while (binlevel[l]<256 && bincount[ binlevel[l] + 256*l ]<=1)
        ++binlevel[l];

      while (l>=0 && binlevel[l]==256)
        --l;
    } while (l>=0 && bincount[ binlevel[l] + 256*l ]<=1);

    if (l==-1) break;

    start=binipos[ binlevel[l]  + 256*l ];
    end  =binepos[ binlevel[l]  + 256*l ];
//    cout << "l: " << l << "start: " << start << " end: " << end << endl;

    if (l<ksize-1) {
      ++binlevel[l];
      ++l;
      binlevel[l]=0;
    }
  } 
}
template <class T,double (*F)(T&,long int)>
void lfradix256sort(T& array)
{
  if (array.size()==0) return;
  ldieif(sizeof(double)!=sizeof(F(array,0)),"size type mismatch");

  int ksize=sizeof(double);

  long int bincount[256*ksize];
  long int binepos[256*ksize];
  long int binipos[256*ksize];
  long int binpos[256*ksize];
  long int i;

  long int *pBinCount;
  long int *pBinPos;
  long int *pBinIPos;
  long int *pBinEPos;

  long int start,end;
  int binlevel[ksize];
  int l;
  int shift;

  union {
    uint64_t i;
    double f;
  } u;


  l=0;
  binlevel[0]=0;
  start=0; end=array.size();

  long int tmpcount;
  bool negative;

  while (1){
    shift=(ksize-1-l)*8;
//    printf("level: %i mask: %x  shift: %i\n",l,mask,shift);
    pBinCount=&bincount[256*l];
    pBinPos=&binpos[256*l];
    pBinIPos=&binipos[256*l];
    pBinEPos=&binepos[256*l];

    for (i=0; i<256; ++i)
      pBinCount[i]=0;

    for (i=start; i<end; ++i){
      u.f=F(array,i);
      ++pBinCount[ (u.i>>shift)&0x00FFul ];
    }

    tmpcount=0;

    if (l==0) {  // if this is the most significant byte, then we need to handle the sign bit
      for (i=255; i>=128; --i)
        { pBinPos[i]=start+tmpcount; pBinIPos[i]=pBinPos[i]; tmpcount+=pBinCount[i]; pBinEPos[i]=start+tmpcount; }
      for (i=0; i<128; ++i)
        { pBinPos[i]=start+tmpcount; pBinIPos[i]=pBinPos[i]; tmpcount+=pBinCount[i]; pBinEPos[i]=start+tmpcount;  }
    }else{
      if (binlevel[0]<128){
        for (i=0; i<256; ++i)
          { pBinPos[i]=start+tmpcount; pBinIPos[i]=pBinPos[i]; tmpcount+=pBinCount[i]; pBinEPos[i]=start+tmpcount;  }
      }else{
        for (i=255; i>=0; --i)
          { pBinPos[i]=start+tmpcount; pBinIPos[i]=pBinPos[i]; tmpcount+=pBinCount[i]; pBinEPos[i]=start+tmpcount; }
      }
    }

    int k,b;
    for (b=0; b<255; ++b){
//      while (b<255 && pBinPos[b]==pBinIPos[b+1]) ++b;
//      if (pBinIPos[b+1]==pBinIPos[256]) break;
      for (i=pBinPos[b]; i<pBinEPos[b]; ++i){
        do {
          u.f=F(array,i);
          k=(u.i>>shift)&0x00FFul;
          if (k!=b)
            array.swap(i,pBinPos[k]);
          ++pBinPos[k];
        } while (k!=b);
      }
    }

    if (l==ksize-1)
      --l;

    do {
      while (binlevel[l]<256 && bincount[ binlevel[l] + 256*l ]<=1)
        ++binlevel[l];

      while (l>=0 && binlevel[l]==256)
        --l;
    } while (l>=0 && bincount[ binlevel[l] + 256*l ]<=1);

    if (l==-1) break;

    start=binipos[ binlevel[l]  + 256*l ];
    end  =binepos[ binlevel[l]  + 256*l ];
//    cout << "l: " << l << "start: " << start << " end: " << end << endl;

    if (l<ksize-1) {
      ++binlevel[l];
      ++l;
      binlevel[l]=0;
    }
  } 
}
template <class T>
void fradix256sort(T& array)
{
  if (array.size()==0) return;
  ldieif(sizeof(float)!=sizeof(array.keys(0)),"size type mismatch");

  int ksize=sizeof(float);

  long int bincount[256*ksize];
  long int binepos[256*ksize];
  long int binipos[256*ksize];
  long int binpos[256*ksize];
  long int i;

  long int *pBinCount;
  long int *pBinPos;
  long int *pBinIPos;
  long int *pBinEPos;

  long int start,end;
  int binlevel[ksize];
  int l;
  int shift;

  l=0;
  binlevel[0]=0;
  start=0; end=array.size();

  long int tmpcount;
  bool negative;

  while (1){
    shift=(ksize-1-l)*8;
//    printf("level: %i mask: %x  shift: %i\n",l,mask,shift);
    pBinCount=&bincount[256*l];
    pBinPos=&binpos[256*l];
    pBinIPos=&binipos[256*l];
    pBinEPos=&binepos[256*l];

    for (i=0; i<256; ++i)
      pBinCount[i]=0;

    for (i=start; i<end; ++i)
      ++pBinCount[ ((*reinterpret_cast<unsigned int*>(&array.keys(i)))>>shift)&0x00FFu ];

    tmpcount=0;

    if (l==0) {  // if this is the most significant byte, then we need to handle the sign bit
      for (i=255; i>=128; --i)
        { pBinPos[i]=start+tmpcount; pBinIPos[i]=pBinPos[i]; tmpcount+=pBinCount[i]; pBinEPos[i]=start+tmpcount; }
      for (i=0; i<128; ++i)
        { pBinPos[i]=start+tmpcount; pBinIPos[i]=pBinPos[i]; tmpcount+=pBinCount[i]; pBinEPos[i]=start+tmpcount;  }
    }else{
      if (binlevel[0]<128){
        for (i=0; i<256; ++i)
          { pBinPos[i]=start+tmpcount; pBinIPos[i]=pBinPos[i]; tmpcount+=pBinCount[i]; pBinEPos[i]=start+tmpcount;  }
      }else{
        for (i=255; i>=0; --i)
          { pBinPos[i]=start+tmpcount; pBinIPos[i]=pBinPos[i]; tmpcount+=pBinCount[i]; pBinEPos[i]=start+tmpcount; }
      }
    }

    int k,b;
    for (b=0; b<255; ++b){
//      while (b<255 && pBinPos[b]==pBinIPos[b+1]) ++b;
//      if (pBinIPos[b+1]==pBinIPos[256]) break;
      for (i=pBinPos[b]; i<pBinEPos[b]; ++i){
        do {
          k=((*reinterpret_cast<unsigned int*>(&array.keys(i)))>>shift)&0x00FFu;
          if (k!=b)
            array.swap(i,pBinPos[k]);
          ++pBinPos[k];
        } while (k!=b);
      }
    }

    if (l==ksize-1)
      --l;

    do {
      while (binlevel[l]<256 && bincount[ binlevel[l] + 256*l ]<=1)
        ++binlevel[l];

      while (l>=0 && binlevel[l]==256)
        --l;
    } while (l>=0 && bincount[ binlevel[l] + 256*l ]<=1);

    if (l==-1) break;

    start=binipos[ binlevel[l]  + 256*l ];
    end  =binepos[ binlevel[l]  + 256*l ];
//    cout << "l: " << l << "start: " << start << " end: " << end << endl;

    if (l<ksize-1) {
      ++binlevel[l];
      ++l;
      binlevel[l]=0;
    }
  } 
}
template <class T,float (*F)(T&,long int)>
void fradix256sort(T& array)
{
  if (array.size()==0) return;
  ldieif(sizeof(float)!=sizeof(F(array,0)),"size type mismatch");

  int ksize=sizeof(float);

  long int bincount[256*ksize];
  long int binepos[256*ksize];
  long int binipos[256*ksize];
  long int binpos[256*ksize];
  long int i;

  long int *pBinCount;
  long int *pBinPos;
  long int *pBinIPos;
  long int *pBinEPos;

  long int start,end;
  int binlevel[ksize];
  int l;
  int shift;

  l=0;
  binlevel[0]=0;
  start=0; end=array.size();

  long int tmpcount;
  bool negative;
  union {
    uint32_t i;
    float f;
  } u;

  while (1){
    shift=(ksize-1-l)*8;
//    printf("level: %i mask: %x  shift: %i\n",l,mask,shift);
    cout << "l: " << l << "start: " << start << " end: " << end << endl;

    pBinCount=&bincount[256*l];
    pBinPos=&binpos[256*l];
    pBinIPos=&binipos[256*l];
    pBinEPos=&binepos[256*l];

    for (i=0; i<256; ++i)
      pBinCount[i]=0;


    for (i=start; i<end; ++i){
      u.f=F(array,i); // this is needed because casting does not work with -O2 optimization
      ++pBinCount[ (u.i>>shift)&0x00FFu ];
    }

    tmpcount=0;

    if (l==0) {  // if this is the most significant byte, then we need to handle the sign bit
      for (i=255; i>=128; --i) {
        pBinPos[i]=start+tmpcount; pBinIPos[i]=pBinPos[i]; tmpcount+=pBinCount[i]; pBinEPos[i]=start+tmpcount;
      }
      for (i=0; i<128; ++i) {
        pBinPos[i]=start+tmpcount; pBinIPos[i]=pBinPos[i]; tmpcount+=pBinCount[i]; pBinEPos[i]=start+tmpcount;
      }
    }else{
      if (binlevel[0]<128){
        for (i=0; i<256; ++i)
          { pBinPos[i]=start+tmpcount; pBinIPos[i]=pBinPos[i]; tmpcount+=pBinCount[i]; pBinEPos[i]=start+tmpcount;  }
      }else{
        for (i=255; i>=0; --i)
          { pBinPos[i]=start+tmpcount; pBinIPos[i]=pBinPos[i]; tmpcount+=pBinCount[i]; pBinEPos[i]=start+tmpcount; }
      }
    }

    int k,b;
    for (b=0; b<255; ++b){
//      while (b<255 && pBinPos[b]==pBinIPos[b+1]) ++b;
//      if (pBinIPos[b+1]==pBinIPos[256]) break;
      for (i=pBinPos[b]; i<pBinEPos[b]; ++i){
        do {
          u.f=F(array,i); // this is needed because casting does not work with -O2 optimization
          k=(u.i>>shift)&0x00FFu;
          if (k!=b)
            array.swap(i,pBinPos[k]);
          ++pBinPos[k];
        } while (k!=b);
      }
    }

    if (l==ksize-1)
      --l;

    do {
      while (binlevel[l]<256 && bincount[ binlevel[l] + 256*l ]<=1)
        ++binlevel[l];

      while (l>=0 && binlevel[l]==256)
        --l;
    } while (l>=0 && bincount[ binlevel[l] + 256*l ]<=1);

    if (l==-1) break;

    start=binipos[ binlevel[l]  + 256*l ];
    end  =binepos[ binlevel[l]  + 256*l ];
//    cout << "l: " << l << "start: " << start << " end: " << end << endl;

    if (l<ksize-1) {
      ++binlevel[l];
      ++l;
      binlevel[l]=0;
    }
  } 
}

template <class T>
void radix256sort(T& array)
{
  if (array.size()==0) return;
  ldieif(sizeof(int)!=sizeof(array.keys(0)),"size type mismatch");

  int ksize=sizeof(int);
  long int bincount[256*ksize];
  long int binipos[257*ksize];
  long int binpos[256*ksize];
  long int i;

  long int *pBinCount;
  long int *pBinPos;
  long int *pBinIPos;

  long int start,end;
  int binlevel[ksize];
  int l;
  int shift;

  l=0;
  binlevel[0]=0;
  start=0; end=array.size();

  while (1){
    shift=(ksize-1-l)*8;
//    printf("level: %i mask: %x  shift: %i\n",l,mask,shift);
    pBinCount=&bincount[256*l];
    pBinPos=&binpos[256*l];
    pBinIPos=&binipos[257*l];

    for (i=0; i<256; ++i)
      pBinCount[i]=0;

    for (i=start; i<end; ++i)
      ++pBinCount[ (array.keys(i)>>shift)&0x00FFu ];

    long int tmpcount=0;
    for (i=0; i<256; ++i)
      { pBinIPos[i]=start+tmpcount; pBinPos[i]=pBinIPos[i]; tmpcount+=pBinCount[i]; }
    pBinIPos[i]=start+tmpcount;

    int k,b;
    for (b=0; b<255; ++b){
//      while (b<255 && pBinPos[b]==pBinIPos[b+1]) ++b;
//      if (pBinIPos[b+1]==pBinIPos[256]) break;
      for (i=pBinPos[b]; i<pBinIPos[b+1]; ++i){
        do {
          k=(array.keys(i)>>shift)&0x00FFu;
          if (k!=b)
            array.swap(i,pBinPos[k]);
          ++pBinPos[k];
        } while (k!=b);
      }
    }

    if (l==ksize-1)
      --l;

    do {
      while (binlevel[l]<256 && bincount[ binlevel[l] + 256*l ]<=1)
        ++binlevel[l];

      while (l>=0 && binlevel[l]==256)
        --l;
    } while (l>=0 && bincount[ binlevel[l] + 256*l ]<=1);

    if (l==-1) break;

    start=binipos[ binlevel[l]   + 257*l ];
    end  =binipos[ binlevel[l]+1 + 257*l ];
//    cout << "start: " << start << " end: " << end << endl;

    if (l<ksize-1) {
      ++binlevel[l];
      ++l;
      binlevel[l]=0;
    }
  } 
}

template <class T>
eintarray iheapsort(T& array)
{
  int i;
  eintarray ind;
  ind.reserve(array.size());
  for (i=0; i<array.size(); ++i)
    ind.add(i);

  int root,child;
  int tmpsize;

  // heapifuy
  tmpsize=array.size();
  for (i=array.size()/2; i>=0; --i){
    root=i;
    while (root<<1 < tmpsize){
      child=root<<1;
      if (child+1<tmpsize && array[ind[child]] < array[ind[child+1]])
        ++child;
      if (array[ind[root]] < array[ind[child]]){
        ind.swap(root,child);
        root=child;
      } else
        break;
    }
  }

  // build sorted array
  for (tmpsize=array.size()-1; tmpsize>0; --tmpsize){
    ind.swap(0,tmpsize);
    root=0;
    while (root<<1 < tmpsize){
      child=root<<1;
      if (child+1<tmpsize && array[ind[child]] < array[ind[child+1]])
        ++child;
      if (array[ind[root]] < array[ind[child]]){
        ind.swap(root,child);
        root=child;
      } else
        break;
    }
  }
  return(ind);
}

template <class T,class V,bool F(const V&,const V&)>
eintarray iheapsort(T& array)
{
  int i;
  eintarray ind;
  ind.reserve(array.size());
  for (i=0; i<array.size(); ++i)
    ind.add(i);

  int root,child;
  int tmpsize;

  // heapify
  tmpsize=array.size();
  for (i=array.size()/2; i>=0; --i){
    root=i;
    while (root<<1 < tmpsize){
      child=root<<1;
      if (child+1<tmpsize && F(array[ind[child]],array[ind[child+1]]))
        ++child;
      if (F(array[ind[root]],array[ind[child]])){
        ind.swap(root,child);
        root=child;
      } else
        break;
    }
  }

  // build sorted array
  for (tmpsize=array.size()-1; tmpsize>0; --tmpsize){
    ind.swap(0,tmpsize);
    root=0;
    while (root<<1 < tmpsize){
      child=root<<1;
      if (child+1<tmpsize && F(array[ind[child]],array[ind[child+1]]))
        ++child;
      if (F(array[ind[root]],array[ind[child]])){
        ind.swap(root,child);
        root=child;
      } else
        break;
    }
  }
  return(ind);
}

template <class T,class V,bool F(const V&,const V&)>
void heapsort(T& array)
{
  int i;
  int root,child;
  int tmpsize;

  // heapify
  tmpsize=array.size();
  for (i=array.size()/2; i>=0; --i){
    root=i;
    while (root<<1 < tmpsize){
      child=root<<1;
      if (child+1<tmpsize && F(array[child],array[child+1]))
        ++child;
      if (F(array[root],array[child])){
        array.swap(root,child);
        root=child;
      } else
        break;
    }
  }

  // build sorted array
  for (tmpsize=array.size()-1; tmpsize>0; --tmpsize){
    array.swap(0,tmpsize);
    root=0;
    while (root<<1 < tmpsize){
      child=root<<1;
      if (child+1<tmpsize && F(array[child],array[child+1]))
        ++child;
      if (F(array[root],array[child])){
        array.swap(root,child);
        root=child;
      } else
        break;
    }
  }
}

template <class T>
void heapsort(T& array)
{
  int i;
  int root,child;
  int tmpsize;

  // heapify
  tmpsize=array.size();
  for (i=array.size()/2; i>=0; --i){
    root=i;
    while (root<<1 < tmpsize){
      child=root<<1;
      if (child+1<tmpsize && array[child] < array[child+1])
        ++child;
      if (array[root] < array[child]){
        array.swap(root,child);
        root=child;
      } else
        break;
    }
  }

  // build sorted array
  for (tmpsize=array.size()-1; tmpsize>0; --tmpsize){
    array.swap(0,tmpsize);
    root=0;
    while (root<<1 < tmpsize){
      child=root<<1;
      if (child+1<tmpsize && array[child] < array[child+1])
        ++child;
      if (array[root] < array[child]){
        array.swap(root,child);
        root=child;
      } else
        break;
    }
  }
}

template <class T,bool F(const T&,const T&)>
void heapsort(earray<T>& array)
{
  int i;
  int root,child;
  int tmpsize;

  tmpsize=array.size();
  for (i=array.size()/2; i>=0; --i){
    root=i;
    while (root<<1 < tmpsize){
      child=root<<1;
      if (child+1<tmpsize && F(array[child],array[child+1]))
        ++child;
      if (F(array[root],array[child])){
        array.swap(root,child);
        root=child;
      } else
        break;
    }
  }

  for (tmpsize=array.size()-1; tmpsize>0; --tmpsize){
    array.swap(0,tmpsize);
    root=0;
    while (root<<1 < tmpsize){
      child=root<<1;
      if (child+1<tmpsize && F(array[child],array[child+1]))
        ++child;
      if (F(array[root],array[child])){
        array.swap(root,child);
        root=child;
      } else
        break;
    }
  }
}

template <class T>
void heapsort(earray<T>& array)
{
  int i;
  int root,child;
  int tmpsize;

  tmpsize=array.size();
  for (i=array.size()/2; i>=0; --i){
    root=i;
    while (root<<1 < tmpsize){
      child=root<<1;
      if (child+1<tmpsize && array[child] < array[child+1])
        ++child;
      if (array[root] < array[child]){
        array.swap(root,child);
        root=child;
      } else
        break;
    }
  }

  for (tmpsize=array.size()-1; tmpsize>0; --tmpsize){
    array.swap(0,tmpsize);
    root=0;
    while (root<<1 < tmpsize){
      child=root<<1;
      if (child+1<tmpsize && array[child] < array[child+1])
        ++child;
      if (array[root] < array[child]){
        array.swap(root,child);
        root=child;
      } else
        break;
    }
  }
}

template <class T>
eintarray mergesort(const earray<T>& arr1,const earray<T>& arr2)
{
  eintarray res;
  int i=0;
  int j=0;
  res.reserve(arr1.size()+arr2.size());
  while (i<arr1.size() && j<arr2.size()){
    if (arr1[i]<arr2[j])
      { res.add(i); ++i; }
    else
      { res.add(arr1.size()+j); ++j; }
  }
  while (i<arr1.size())
    { res.add(i); ++i; }
  while (j<arr2.size())
    { res.add(arr1.size()+j); ++j; }
  return(res);
}

template <class T>
eintarray mergesort(const ebasicarray<T>& arr1,const ebasicarray<T>& arr2)
{
  eintarray res;
  int i=0;
  int j=0;
  res.reserve(arr1.size()+arr2.size());
  while (i<arr1.size() && j<arr2.size()){
    if (arr1[i]<arr2[j])
      { res.add(i); ++i; }
    else
      { res.add(arr1.size()+j); ++j; }
  }
  while (i<arr1.size())
    { res.add(i); ++i; }
  while (j<arr2.size())
    { res.add(arr1.size()+j); ++j; }
  return(res);
}



template <class T>
eintarray sort(const ebasicarray<T>& array)
{
  int i;
  eintarray sorti;
  for (i=0; i<array.size(); ++i)
    sorti.add(i);

  int root,child;
  int tmpsize;

  tmpsize=array.size();
  for (i=array.size()/2; i>=0; --i){
    root=i;
    while (root<<1 < tmpsize){
      child=root<<1;
      if (child+1<tmpsize && array[sorti[child]] < array[sorti[child+1]])
        ++child;
      if (array[sorti[root]] < array[sorti[child]]){
        sorti.swap(root,child);
        root=child;
      } else
        break;
    }
  }

  for (tmpsize=array.size()-1; tmpsize>0; --tmpsize){
    sorti.swap(0,tmpsize);
    root=0;
    while (root<<1 < tmpsize){
      child=root<<1;
      if (child+1<tmpsize && array[sorti[child]] < array[sorti[child+1]])
        ++child;
      if (array[sorti[root]] < array[sorti[child]]){
        sorti.swap(root,child);
        root=child;
      } else
        break;
    }
  }
  return(sorti);
}

template <class T>
eintarray sort(const earray<T>& array)
{
/*
  eheap<T> heap(array);
  int i;
  for (i=array.size()-1; i>0; --i)
    { array[i]=heap.root(); array.delroot(); }
*/
  int i;
  eintarray sorti;
  for (i=0; i<array.size(); ++i)
    sorti.add(i);

  int root,child;
  int tmpsize;

  tmpsize=array.size();
  for (i=array.size()/2; i>=0; --i){
    root=i;
    while (root<<1 < tmpsize){
      child=root<<1;
      if (child+1<tmpsize && array[sorti[child]] < array[sorti[child+1]])
        ++child;
      if (array[sorti[root]] < array[sorti[child]]){
        sorti.swap(root,child);
        root=child;
      } else
        break;
    }
    
  }

  for (tmpsize=array.size()-1; tmpsize>0; --tmpsize){
    sorti.swap(0,tmpsize);
    root=0;
    while (root<<1 < tmpsize){
      child=root<<1;
      if (child+1<tmpsize && array[sorti[child]] < array[sorti[child+1]])
        ++child;
      if (array[sorti[root]] < array[sorti[child]]){
        sorti.swap(root,child);
        root=child;
      } else
        break;
    }
  }
  return(sorti);
}



/*
template <class T>
eheap<T>::eheap(): _size(0) {} //: _size(0),_arr(new earray<T>()),internal(true) {}

template <class T>
eheap<T>::eheap(earray<T>* arr): _size(1),_arr(arr),internal(false)
{
  int i;
  for (i=1; i<_arr->size(); ++i)
    { ++_size; check(i); }
  if (!_arr->size()) _size=0;
}

template <class T>
eheap<T>::~eheap(){
//  if (internal)
//    delete _arr;
}

template <class T>
void eheap<T>::check(int i)
{
//  ++i;
  while ((i>>1)-1>=0 && earray<T>::operator[]((i>>1)-1)<earray<T>::operator[](i-1)){
    earray<T>::swap(i-1,(i>>1)-1);
    i=i>>1;
  }
}

template <class T>
bool eheap<T>::exists(const T& value)
{
  for (i=0; i<earray<T>::size(); ++i){
    if (earray<T>::operator[](i) == value) return(true);
    if (earray<T>::operator[](i) <
  }

  int i;
  i=earray<T>::size();
  while ((i>>1)-1>=0 && earray<T>::operator[]((i>>1)-1)<value){
    i=i>>1;
  }

  
  if ((i>>1)-1<0) return(false);
  return(earray<T>::operator[]((i>>1)-1)==value || earray<T>::operator[](i>>1)==value);
  return(false);
}

template <class T>
void eheap<T>::add(const T& value)
{
  if (_size==earray<T>::size())
    earray<T>::add(value);
  else
    earray<T>::operator[](_size)=value;
  ++_size;
  check(_size);
}

template <class T>
void eheap<T>::delroot()
{
  int i;

  earray<T>::operator[](0)=earray<T>::operator[](_size-1);
  i=1;
  while (1){
    if ((i<<1)-1<_size && earray<T>::operator[](i-1)<earray<T>::operator[]((i<<1)-1) && ((i<<1)>=_size || earray<T>::operator[](i<<1) < earray<T>::operator[]((i<<1)-1) )){
      earray<T>::swap(i-1,(i<<1)-1);
      i=i<<1;
    }else if ((i<<1)<_size && earray<T>::operator[](i-1)<earray<T>::operator[](i<<1)){
      earray<T>::swap(i-1,i<<1);
      i=(i<<1)+1;
    }else
      break;
  }
  --_size;
//  earray<T>::erase(earray<T>::size()-1);
}
*/

#endif

