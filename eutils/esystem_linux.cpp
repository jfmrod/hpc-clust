#include "esystem_linux.h"

#include <stdio.h>
#include <signal.h>


esystemCallback::esystemCallback(): readEnabled(false), writeEnabled(false) {}



esystem *esystem::cursystem=0x00;

esystem *getSystem()
{
  if (!esystem::cursystem)
    esystem::cursystem=new esystem;
  return(esystem::cursystem);
}

void esystem::handleTimerSignal(int signal,siginfo_t *si,void *data)
{
  esystemTimer *stimer=(esystemTimer*)si->_sifields._rt.si_sigval.sival_ptr;
  stimer->callback.call(stimer->data);
}

esystem::esystem()
{
  struct sigaction sa;
  sigemptyset(&sa.sa_mask);
  sigaddset(&sa.sa_mask,SIGALRM);
  sa.sa_flags=SA_SIGINFO;
  sa.sa_sigaction=&esystem::handleTimerSignal;
  sigaction(SIGALRM,&sa,0x00);
}

void esystem::addTimer(const efunc& callback,const evararray& data,double secs,double repeat)
{
  esystemTimer *t=new esystemTimer;

  struct sigevent se;
  se.sigev_notify=SIGEV_SIGNAL;
  se.sigev_signo=SIGALRM;
  se.sigev_value.sival_ptr=(void*)t;

  int retval;
  retval=timer_create(CLOCK_REALTIME,&se,&t->timer);
  lassert(retval==-1);
  t->callback=callback;
  t->data=data;
  timers.addref(0,t);
}

void esystem::addReadWriteCallback(int fd,const efunc& readCallback,const evararray& readData,const efunc& writeCallback,const evararray& writeData)
{
  esystemCallback cb;
  cb.fd=fd;
  cb.readEnabled=true;
  cb.readCallback=readCallback;
  cb.readData=readData;
  cb.writeEnabled=true;
  cb.writeCallback=writeCallback;
  cb.writeData=writeData;
  callbacks.add(fd,cb);
}

void esystem::addWriteCallback(int fd,const efunc& func,const evararray& data)
{
  esystemCallback cb;
  cb.fd=fd;
  cb.writeEnabled=true;
  cb.writeCallback=func;
  cb.writeData=data;
  callbacks.add(fd,cb);
}

void esystem::addReadCallback(int fd,const efunc& func,const evararray& data)
{
  esystemCallback cb;
  cb.fd=fd;
  cb.readEnabled=true;
  cb.readCallback=func;
  cb.readData=data;
  callbacks.add(fd,cb);
}

void esystem::disableReadCallback(int fd)
{
  int i;
  i=callbacks.findkey(fd);
  lerrorifr(i==-1,"file descriptor not found",);
  callbacks.values(i).readEnabled=false;
}

void esystem::disableWriteCallback(int fd)
{
  int i;
  i=callbacks.findkey(fd);
  lerrorifr(i==-1,"file descriptor not found",);
  callbacks.values(i).writeEnabled=false;
}

void esystem::disableReadWriteCallback(int fd)
{
  int i;
  i=callbacks.findkey(fd);
  lerrorifr(i==-1,"file descriptor not found",);
  callbacks.values(i).readEnabled=false;
  callbacks.values(i).writeEnabled=false;
}

void esystem::enableReadCallback(int fd)
{
  int i;
  i=callbacks.findkey(fd);
  lerrorifr(i==-1,"file descriptor not found",);
  callbacks.values(i).readEnabled=true;
}

void esystem::enableWriteCallback(int fd)
{
  int i;
  i=callbacks.findkey(fd);
  lerrorifr(i==-1,"file descriptor not found",);
  callbacks.values(i).writeEnabled=true;
}

void esystem::enableReadWriteCallback(int fd)
{
  int i;
  i=callbacks.findkey(fd);
  lerrorifr(i==-1,"file descriptor not found",);
  callbacks.values(i).readEnabled=true;
  callbacks.values(i).writeEnabled=true;
}

void esystem::add(int fd,const efunc& func,const evar& data)
{
  addReadCallback(fd,func,data);
//  fds.add(fd);
//  funcs.addref(new efunc(func));
//  datas.add(data);
}

void esystem::addfunc(int fd,efunc *func)
{
  addReadCallback(fd,*func,evararray());
//  fds.add(fd);
//  funcs.addref(func);
//  datas.add(evar());
}

void esystem::remove(int fd)
{
  int i;
  i=callbacks.findkey(fd);
  lerrorifr(i==-1,"file descriptor not found",);
  callbacks.erase(i);
  
/*
  int i;
  i=fds.find(fd);
  lerrorifr(i==-1,"file descriptor not found",);

  fds.erase(i);
  funcs.erase(i);
  datas.erase(i);
*/
}

void esystem::run()
{
  while (1) wait();
}

void esystem::wait(int fd)
{
//  struct timeval tv;
  fd_set rd,wd;
  int i;
  int fdmax;

  do{
//    ldinfo("waiting for activity on fds. callbacks.size: "+estr(callbacks.size())+" fd: "+estr(fd));
    FD_ZERO(&rd);
    FD_ZERO(&wd);
    fdmax=-1;
//    if (callbacks.size())
//      fdmax=callbacks.values(0).fd;
    for (i=0; i<callbacks.size(); ++i){
      if (callbacks.values(i).readEnabled){
        if (callbacks.values(i).fd>fdmax) fdmax=callbacks.values(i).fd;
        FD_SET(callbacks.values(i).fd,&rd);
      }
      if (callbacks.values(i).writeEnabled){
        if (callbacks.values(i).fd>fdmax) fdmax=callbacks.values(i).fd;
        FD_SET(callbacks.values(i).fd,&wd);
      }
    }

    if (fd!=-1){
      FD_SET(fd,&rd);
      if (!callbacks.size() || fd > fdmax) fdmax=fd;
    }

    int res;
    res=select(fdmax+1,&rd,&wd,NULL,NULL);

    lerrorif(res==-1,"error in select");

    for (i=0; i<callbacks.size(); ++i){
      if (callbacks.values(i).readEnabled && FD_ISSET(callbacks.values(i).fd,&rd))
        callbacks.values(i).readCallback.call(callbacks.values(i).readData);
      if (callbacks.values(i).writeEnabled && FD_ISSET(callbacks.values(i).fd,&wd))
        callbacks.values(i).writeCallback.call(callbacks.values(i).writeData);
    }
  } while (callbacks.size() && fd!=-1 && !FD_ISSET(fd,&rd));
}



