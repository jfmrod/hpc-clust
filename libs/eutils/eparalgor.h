#ifndef EPARALGOR_H
#define EPARALGOR_H

#include "ethread.h"

template <class T>
void parApplyTask(evararray& res,const earray<T>& arr,const efunc& func,int task,int totalTasks)
{
  int start,end;
  start=task*arr.size()/totalTasks;
  end=(task+1)*arr.size()/totalTasks;
  int i;
  for (i=start; i<end; ++i)
    res[i].set(func.call(arr[i]));
}

template <class T>
evararray parApply(const earray<T>& arr,const efunc& func,etaskman& taskman)
{
  evararray res;
  res.init(arr.size());
  int i;
  ldieif(taskman.threads.size()==0,"No threads created in taskman!");
  for (i=0; i<taskman.threads.size(); ++i)
    taskman.addTask(parApplyTask<T>,evararray(res,arr,func,(const int&)i,(const int&)taskman.threads.size()));
  taskman.wait();
  return(res);
}

template <class T>
void parHeapRemoveTask(eintarray& res,const T& arr,earray<emutex>& mutexs,int &tmpsize)
{
  int root,child;
  int l;
  mutexs[0].lock();
  while (tmpsize>0){
    --tmpsize;
    res.swap(0,tmpsize);
    root=0;
    l=0;
    while ((root<<1)+1 < tmpsize){
      child=(root<<1)+1;
      if (child+1<tmpsize && arr[res[child]] < arr[res[child+1]])
        ++child;
      if (arr[res[root]] < arr[res[child]]){
        res.swap(root,child);
        mutexs[l+1].lock();
        mutexs[l].unlock();
        root=child;
        ++l;
      } else
        break;
    }
    if (l!=0) {
      mutexs[l].unlock();
      mutexs[0].lock();
    }
  }
  mutexs[0].unlock();
}

template <class T>
void parHeapifyTask(eintarray& res,const T& arr,int heapLevel,int task,int totalTasks)
{
  int width=0x01<<heapLevel;
  int start,end;
  start=width-1+task*width/totalTasks;
  end=width-1+(task+1)*width/totalTasks;
//  cout << "l: "<< heapLevel << " start: " << start << " end: " << end << " width: " << width << endl;

  int i;
  int root,child;
  int tmpsize;

  // heapify
  tmpsize=arr.size();
  for (i=start; i<end; ++i){
//  for (i=arr.size()>>1; i>=0; --i){
    root=i;
    while ((root<<1)+1 < tmpsize){
      child=(root<<1)+1;
      if (child+1<tmpsize && arr[res[child]] < arr[res[child+1]])
        ++child;
      if (arr[res[root]] < arr[res[child]]){
        res.swap(root,child);
        root=child;
      } else
        break;
    }
  }
}


template <class T>
eintarray parHeapSort(const T& arr,etaskman& taskman)
{
  int jobcount;
  eintarray res;
  res.init(arr.size());

  int i,l;

  for (i=0; i<res.size(); ++i)
    res[i]=i;

  for (l=log2(arr.size()); l>0; --l){
    jobcount=MIN(taskman.threads.size(),(0x01<<l));
    cout << "l: "<< l << " jobcount: " << jobcount << endl;
    for (i=0; i<jobcount; ++i)
      taskman.addTask(parHeapifyTask<T>,evararray(res,arr,(const int&)l,(const int&)i,(const int&)jobcount));
    taskman.wait();
  }

  cout << "l: "<< 0 << " jobcount: " << 1 << endl;
  parHeapifyTask<T>(res,arr,0,0,1);

  cout << "building ordered array" << endl;
  int tmpsize;
  tmpsize=arr.size();
  earray<emutex> mutexs;
  for (i=0; i<log2(arr.size())+1; ++i)
    mutexs.addref(new emutex());
  cout << "finished initing mutexs" << endl;
  for (i=0; i<taskman.threads.size(); ++i)
    taskman.addTask(parHeapRemoveTask<T>,evararray(res,arr,mutexs,tmpsize));
  taskman.wait();

/*
  // build sorted array from heap
  for (tmpsize=arr.size()-1; tmpsize>0; --tmpsize){
    res.swap(0,tmpsize);
    root=0;
    while ((root<<1)+1 < tmpsize){
      child=(root<<1)+1;
      if (child+1<tmpsize && arr[res[child]] < arr[res[child+1]])
        ++child;
      if (arr[res[root]] < arr[res[child]]){
        res.swap(root,child);
        root=child;
      } else
        break;
    }
  }
*/
  return(res);
}

template <class T>
void parRadixSortTask(T& array,long int start,long int end)
{
  int ksize=4;
  long int bincount[256*ksize];
  long int binipos[257*ksize];
  long int binpos[256*ksize];
  long int i;
  int mask;

  long int *pBinCount;
  long int *pBinPos;
  long int *pBinIPos;

  int binlevel[ksize];
  int l;
  int shift;

  l=1;
  binlevel[1]=0;

  while (1){
    shift=(ksize-1-l)*8;
    mask=0x00FF<<shift;

    pBinCount=&bincount[256*l];
    pBinPos=&binpos[256*l];
    pBinIPos=&binipos[257*l];

    for (i=0; i<256; ++i)
      pBinCount[i]=0;

    for (i=start; i<end; ++i)
      ++pBinCount[ (array.keys(i)&mask)>>shift ];

    long int tmpcount=0;
    for (i=0; i<256; ++i)
      { pBinIPos[i]=start+tmpcount; pBinPos[i]=pBinIPos[i]; tmpcount+=pBinCount[i]; }
    pBinIPos[i]=start+tmpcount;

    int k,b;
    for (b=0; b<255; ++b){
//      while (b<255 && pBinPos[b]==pBinIPos[b+1]) ++b;
      for (i=pBinPos[b]; i<pBinIPos[b+1]; ++i){
        do {
          k=(array.keys(i)&mask)>>shift;
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

      while (l>0 && binlevel[l]==256)
        --l;
    } while (l>0 && bincount[ binlevel[l] + 256*l ]<=1);

    if (l==0) break;

    start=binipos[ binlevel[l]   + 257*l ];
    end  =binipos[ binlevel[l]+1 + 257*l ];

    if (l<ksize-1) {
      ++binlevel[l];
      ++l;
      binlevel[l]=0;
    }
  } 
}
template <class T>
void parRadixSort(T& array,etaskman& taskman)
{
  int ksize=4;
  long int bincount[256];
  long int binipos[257];
  long int binpos[256];
  long int i;
  int mask;

  long int *pBinCount;
  long int *pBinPos;
  long int *pBinIPos;

  long int start,end;
  int shift;
  int l;

  l=0;
  start=0; end=array.size();

  shift=(ksize-1-l)*8;
  mask=0x00FF<<shift;
  pBinCount=&bincount[0];
  pBinPos=&binpos[0];
  pBinIPos=&binipos[0];

  for (i=0; i<256; ++i)
    pBinCount[i]=0;

  for (i=start; i<end; ++i)
    ++pBinCount[ (array.keys(i)&mask)>>shift ];

  long int tmpcount=0;
  for (i=0; i<256; ++i)
    { pBinIPos[i]=start+tmpcount; pBinPos[i]=pBinIPos[i]; tmpcount+=pBinCount[i]; }
  pBinIPos[i]=start+tmpcount;

  int k,b;
  for (b=0; b<255; ++b){
    for (i=pBinPos[b]; i<pBinIPos[b+1]; ++i){
      do {
        k=(array.keys(i)&mask)>>shift;
        if (k!=b)
          array.swap(i,pBinPos[k]);
        ++pBinPos[k];
        if (pBinPos[k]==pBinIPos[k+1] && pBinPos[k]>pBinIPos[k])
          taskman.addTask(parRadixSortTask<T>,evararray(array,(const int&)pBinIPos[k],(const int&)pBinIPos[k+1]));
      } while (k!=b);
    }
  }
  taskman.wait();
}

template <class T,unsigned int (*F)(T&,int)>
void parRadixSortTask(T& array,long int start,long int end)
{
  int ksize=4;
  long int bincount[256*ksize];
  long int binipos[257*ksize];
  long int binpos[256*ksize];
  long int i;
  int mask;

  long int *pBinCount;
  long int *pBinPos;
  long int *pBinIPos;

  int binlevel[ksize];
  int l;
  int shift;

  l=1;
  binlevel[1]=0;

  while (1){
    shift=(ksize-1-l)*8;
    mask=0x00FF<<shift;

    pBinCount=&bincount[256*l];
    pBinPos=&binpos[256*l];
    pBinIPos=&binipos[257*l];

    for (i=0; i<256; ++i)
      pBinCount[i]=0;

    for (i=start; i<end; ++i)
      ++pBinCount[ (F(array,i)&mask)>>shift ];

    long int tmpcount=0;
    for (i=0; i<256; ++i)
      { pBinIPos[i]=start+tmpcount; pBinPos[i]=pBinIPos[i]; tmpcount+=pBinCount[i]; }
    pBinIPos[i]=start+tmpcount;

    int k,b;
    for (b=0; b<255; ++b){
//      while (b<255 && pBinPos[b]==pBinIPos[b+1]) ++b;
      for (i=pBinPos[b]; i<pBinIPos[b+1]; ++i){
        do {
          k=(F(array,i)&mask)>>shift;
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

      while (l>0 && binlevel[l]==256)
        --l;
    } while (l>0 && bincount[ binlevel[l] + 256*l ]<=1);

    if (l==0) break;

    start=binipos[ binlevel[l]   + 257*l ];
    end  =binipos[ binlevel[l]+1 + 257*l ];

    if (l<ksize-1) {
      ++binlevel[l];
      ++l;
      binlevel[l]=0;
    }
  } 
}
template <class T,unsigned int (*F)(T&,int)>
void parRadixSort(T& array,etaskman& taskman)
{
  int ksize=4;
  long int bincount[256];
  long int binipos[257];
  long int binpos[256];
  long int i;
  int mask;

  long int *pBinCount;
  long int *pBinPos;
  long int *pBinIPos;

  long int start,end;
  int shift;
  int l;

  l=0;
  start=0; end=array.size();

  shift=(ksize-1-l)*8;
  mask=0x00FF<<shift;
  pBinCount=&bincount[0];
  pBinPos=&binpos[0];
  pBinIPos=&binipos[0];

  for (i=0; i<256; ++i)
    pBinCount[i]=0;

  for (i=start; i<end; ++i)
    ++pBinCount[ (F(array,i)&mask)>>shift ];

  long int tmpcount=0;
  for (i=0; i<256; ++i)
    { pBinIPos[i]=start+tmpcount; pBinPos[i]=pBinIPos[i]; tmpcount+=pBinCount[i]; }
  pBinIPos[i]=start+tmpcount;

  int k,b;
  for (b=0; b<255; ++b){
    for (i=pBinPos[b]; i<pBinIPos[b+1]; ++i){
      do {
        k=(F(array,i)&mask)>>shift;
        if (k!=b)
          array.swap(i,pBinPos[k]);
        ++pBinPos[k];
        if (pBinPos[k]==pBinIPos[k+1] && pBinPos[k]>pBinIPos[k])
          taskman.addTask(parRadixSortTask<T,F>,evararray(array,(const int&)pBinIPos[k],(const int&)pBinIPos[k+1]));
      } while (k!=b);
    }
  }
  taskman.wait();
}




#endif

