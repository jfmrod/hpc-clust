#include "ethread.h"



// Find out number of processors: sysconf(_SC_NPROCESSORS_ONLN); 

emutex::emutex(int type)
{
 
  pthread_mutexattr_t attr;

  pthread_mutexattr_init(&attr);
  if (type!=EMUTEX_RECURSIVE)
    ldie("unknown type: "+estr(type));

  pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

  pthread_mutex_init(&_mutex, &attr);
  pthread_mutexattr_destroy(&attr);
}

emutex::emutex()
{
  pthread_mutex_init(&_mutex,NULL);
}

emutex::~emutex()
{
  pthread_mutex_destroy(&_mutex);
}

void emutex::lock()
{
  pthread_mutex_lock(&_mutex);
}

void emutex::unlock()
{
  pthread_mutex_unlock(&_mutex);
}

bool emutex::trylock()
{
  if (pthread_mutex_trylock(&_mutex)==0) return(true);
  return(false);
}

/*
class econdsig
{
 private:
  _pthread_mutex_t _waitmutex;
  _pthread_cond_t _waitcond;
 public:
  econdsig();
  ~econdsig();

  void wait();
  void signal();
  bool isWaiting();
};
*/

econdsig::econdsig()
{
  pthread_cond_init(&_cond,NULL);
}

econdsig::~econdsig()
{
  pthread_cond_destroy(&_cond);
}

void econdsig::wait(emutex& mutex)
{
  pthread_cond_wait(&_cond,&mutex._mutex);
}

void econdsig::signal()
{
  pthread_cond_signal(&_cond);
}

void econdsig::broadcast()
{
  pthread_cond_broadcast(&_cond);
}

class ethread_info
{
 public:
  efunc func;
  evararray args;
};

pthread_t ethread_create(const efunc& func,const evararray& args)
{
  pthread_t pthread;
  ethread_info *tinfo=new ethread_info;
  tinfo->func=func;
  tinfo->args=args;
  pthread_create(&pthread,NULL,ethread_run,tinfo);
  return(pthread);
}

void* ethread_run(void *ptinfo)
{
  ethread_info *tinfo=static_cast<ethread_info*>(ptinfo);
  tinfo->func.call(tinfo->args);
  delete tinfo;
  return(0x00);
}

void ethreads::run(const efunc& func,const evararray& args,int nthreads)
{
  int i;
  finish();

  for (i=0; i<nthreads; ++i)
    threads.add(new ethread);
  for (i=0; i<threads.size(); ++i)
    threads[i]->run(func,args);
}

void ethreads::finish()
{
  int i;
  for (i=0; i<threads.size(); ++i){
    threads[i]->finish();
    delete threads[i];
  }
  threads.clear();
}

ethread::ethread(): active(true), _isBusy(false), _pthread(0x00)
{
}

ethread::~ethread()
{
}

void ethread::finish()
{
  active=false;
  runMutex.lock();
  runCond.signal();
  runMutex.unlock();
  pthread_join(_pthread,0x00);
}

void ethread::waitrun(const efunc& func,const evararray& args)
{
  if (_pthread==0x00)
    pthread_create(&_pthread,NULL,ethread::entrypoint, this);

  runMutex.lock();
  if (_isBusy)
    finishedCond.wait(runMutex);
  _func=func;
  _args=args;
  _isBusy=true;
  runCond.signal();
  runMutex.unlock();
}

bool ethread::run(const efunc& func,const evararray& args)
{
  if (_pthread==0x00)
    pthread_create(&_pthread,NULL,ethread::entrypoint, this);

  runMutex.lock();
  if (_isBusy){ runMutex.unlock(); return(false); }
  _func=func;
  _args=args;
  _isBusy=true;
  runCond.signal();
  runMutex.unlock();
  return(true);
}

evar ethread::_runJob()
{
  return(_func.call(_args));
}

int ethread::_runThread()
{
  evar tmpresult;
  // should not lock mutexes in different threads than they will be unlocked
  while (1) {
    runMutex.lock();
    while (!_isBusy && active)
      runCond.wait(runMutex);
    if (!active) { runMutex.unlock(); break; }

    runMutex.unlock();
    tmpresult.set(_runJob());
    runMutex.lock();
    result.set(tmpresult);
    _isBusy=false;
    // announce that job finished (in case another thread is waiting on this one)
    finishedCond.signal();
    runMutex.unlock();
  }
  return(0);
}

bool ethread::isBusy()
{
  runMutex.lock();
  if (_isBusy) { runMutex.unlock(); return(true); }
  runMutex.unlock();
  return(false);
}

void ethread::wait()
{
  runMutex.lock();
  while (_isBusy)  // only wait if something is runnning
    finishedCond.wait(runMutex);
  runMutex.unlock();
}

void *ethread::entrypoint(void *pthis)
{
 static_cast<ethread*>(pthis)->_runThread();
 return(0x00);
}

#ifndef EUTILS_NOTEST

#include "etest.h"

void ethread_test_sfunc(int i)
{
  cout << "thread running function: " << i << endl;
  sleep(3);
  cout << "thread ending function: " << i << endl;
}

void ethread_test()
{
  ethread t;
  cout << "is thread busy?: " << t.isBusy() << endl;
  cout << "running thread: " << t.run(ethread_test_sfunc,evararray(0)) << endl;
  cout << "is thread busy?: " << t.isBusy() << endl;
  cout << "testing immediate submission of new function: " << t.run(ethread_test_sfunc,evararray(0)) << endl;
  cout << "waiting for thread to be free" << endl;
  t.wait();
  cout << "submitting series of jobs to thread" << endl;
  t.waitrun(ethread_test_sfunc,evararray(0));
  t.waitrun(ethread_test_sfunc,evararray(1));
  t.wait();
}

//etestAdd(ethread_test);

#endif




etaskthread::etaskthread(etaskman& _taskman): taskman(_taskman)
{
  pthread_create(&_pthread,NULL,etaskthread::entrypoint, this);
}

etaskthread::~etaskthread()
{
}

evar etaskthread::_runJob(etask& task)
{
  return(task.func.call(task.args));
}

int etaskthread::_runThread()
{
  while(1){
    // get a task from the taskmanager
    etask *ptask=taskman.getTask(*this);
    if (ptask==0x00) return(0);
    ptask->result.set(_runJob(*ptask));
    ptask->setDone();
    taskman.onTaskDone.call(evararray(taskman,*ptask));
  }
  return(0);
}

void *etaskthread::entrypoint(void *pthis)
{
 static_cast<etaskthread*>(pthis)->_runThread();
 return(0x00);
}






etask::etask(const efunc& _func,const evararray& _args): status(0),func(_func),args(_args) {}


etaskman::etaskman(): runningThreads(0),firstPendingTask(0)
{
}

etaskman::~etaskman()
{
}

void etaskman::createThread(int n)
{
  runThreadsMutex.lock();
  int i;
  for (i=0; i<n; ++i){
    threads.addref(new etaskthread(*this));
    ++runningThreads;
  }
  runThreadsMutex.unlock();
}

etask& etaskman::addTask(const efunc& func,const evararray& args)
{
  runThreadsMutex.lock();
  etask &task(tasks.add(etask(func,args)));

/*
  while (min(tasks.size()-firstPendingTask-runningThreads,threads.size()-runningThreads)>0){
    runThreadsCond.signal();
    runThreadsMutex.unlock();
    runThreadsMutex.lock();
  }
*/
  runThreadsCond.broadcast();

  runThreadsMutex.unlock();
  return(task);
}

etask* etaskman::getTask(etaskthread& thread)
{
  int tmpi;
  runThreadsMutex.lock();
//  cout << pthread_self() << " [finished] run: " << runningThreads << " pending: " << firstPendingTask << " tasks: " << tasks.size() << endl;
  while (1) {
    if (tasks.size()-firstPendingTask>0){
//      cout << pthread_self() << " [running] job: " << firstPendingTask << " run: " << runningThreads << " pending: " << firstPendingTask << " tasks: " << tasks.size() << endl;
      tmpi=firstPendingTask;
      ++firstPendingTask;
      lassert(!tasks[tmpi].isPending());
      tasks[tmpi].setRunning();
      runThreadsMutex.unlock();
      return(&tasks[tmpi]);
    }
    if (runningThreads==1){
      runThreadsMutex.unlock();
      onAllDone.call(evararray(*this));
      runThreadsMutex.lock();
      if (tasks.size()-firstPendingTask>0) continue;
    }
    --runningThreads;
    if (runningThreads==0)
      finishedThreadsCond.signal();
//    cout << pthread_self() << " [waiting] run: " << runningThreads << " pending: " << firstPendingTask << " tasks: " << tasks.size() << endl;
    runThreadsCond.wait(runThreadsMutex);
//    cout << pthread_self() << " [waking] run: " << runningThreads << " pending: " << firstPendingTask << " tasks: " << tasks.size() << endl;
    ++runningThreads;
  }
//  cout << pthread_self() << " [exiting] run: " << runningThreads << " pending: " << firstPendingTask << " tasks: " << tasks.size() << endl;
  runThreadsMutex.unlock();
  return(0x00);
}

void etaskman::wait()
{
  runThreadsMutex.lock();
//  cout << "tasks.size: " << tasks.size() << " firstPending: " << firstPendingTask << endl;
  while (runningThreads>0 || firstPendingTask<tasks.size())
    finishedThreadsCond.wait(runThreadsMutex);
  runThreadsMutex.unlock();
}

#ifndef EUTILS_NOTEST

#include "etest.h"

void etaskman_test_sfunc(int i)
{
  cout << "begin thread test task: " << i << endl;
  sleep(2);
  cout << "end thread test task: " << i << endl;
}

void etaskman_test()
{
  etaskman t;
  cout << "creating 2 threads" << endl;
  t.createThread(2);
  cout << "adding 10 test tasks" << endl;
  int i;
  for (i=0; i<10; ++i)
    t.addTask(etaskman_test_sfunc,evararray((const int&)i));

  cout << "waiting for tasks to finish" << endl;
  t.wait();
  cout << "all tasks finished" << endl;

  sleep(2);

  cout << "submitting another round of tasks" << endl;
  for (i=0; i<10; ++i)
    t.addTask(etaskman_test_sfunc,evararray((const int&)i));

  cout << "waiting for tasks to finish" << endl;
  t.wait();
  cout << "all tasks finished" << endl;
}

//etestAdd(etaskman_test);

#endif



