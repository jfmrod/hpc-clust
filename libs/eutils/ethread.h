#ifndef ETHREAD_H
#define ETHREAD_H

#include <pthread.h>
#include "efunc.h"

class econdsig;

class emutex
{
 private:
  pthread_mutex_t _mutex;
 public:
  emutex();
  ~emutex();

  void lock();
  void unlock();
  bool trylock();

  friend class econdsig;
};

class econdsig
{
 private:
  pthread_cond_t _cond;
 public:
  econdsig();
  ~econdsig();

  void wait(emutex& mutex);
  void broadcast();
  void signal();
};

void* ethread_run(void *ptinfo);
pthread_t ethread_create(const efunc& func,const evararray& args);

class ethread
{
 protected:
  static void *entrypoint(void*);
 private:
  pthread_t _pthread;

  bool active;
  bool _isBusy;
  efunc _func;
  evararray _args;

  emutex runMutex;
  econdsig runCond;
  emutex finishedMutex;
  econdsig finishedCond;

  evar _runJob();
  int _runThread();
 public:
  evar result;

  ethread();
  ~ethread();
  void waitrun(const efunc& func,const evararray& args);
  bool run(const efunc& func,const evararray& args);

  void wait();
  bool isBusy();
};



class etask
{
 public:
  int status;
  efunc func;
  evararray args;
  evar result;

  inline void setRunning(){ status=1; }
  inline void setDone(){ status=2; }

  inline bool isPending(){ return(status==0); }
  inline bool isRunning(){ return(status==1); }
  inline bool isDone(){ return(status==2); }
  
  etask(const efunc& func,const evararray& args);
};

class etaskman;

class etaskthread
{
 protected:
  static void *entrypoint(void*);
 private:
  etaskman& taskman;

  pthread_t _pthread;

  evar _runJob(etask& task);
  int _runThread();
 public:
  etaskthread(etaskman& taskman);
  ~etaskthread();

  friend class etaskman;
};

class etaskman
{
 private:
  emutex runThreadsMutex;
  econdsig finishedThreadsCond;
  econdsig runThreadsCond;

  int runningThreads;
  int firstPendingTask;
 public:
  earray<etaskthread> threads;
  earray<etask>   tasks;

  etaskman();
  ~etaskman();

  efunc onTaskDone;
  efunc onAllDone;

  void doAllTasksDone();

  void createThread(int n=1);
  etask& addTask(const efunc& func,const evararray& args);
  etask* getTask(etaskthread& thread);
  void wait();
};

#endif

