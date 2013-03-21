#include "esystem_linux.h"

#include <stdio.h>
#include <signal.h>


#include <sys/sysinfo.h>
#include <sys/time.h>
#include <sys/resource.h>


#include <stdio.h>      
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h> 
#include <string.h> 
#include <arpa/inet.h>

estrarray esystem::getLocalAddresses()
{
  estrarray tmpstr;
  struct ifaddrs * ifAddrStruct=NULL;
  struct ifaddrs * ifa=NULL;
  void * tmpAddrPtr=NULL;

  getifaddrs(&ifAddrStruct);

  for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
    if (ifa ->ifa_addr->sa_family==AF_INET) { // check it is IP4
      // is a valid IP4 Address
      tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
      char addressBuffer[INET_ADDRSTRLEN];
      inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
      printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer); 
      tmpstr.add(ifa->ifa_name,addressBuffer);
    }
/*
    else if (ifa->ifa_addr->sa_family==AF_INET6) { // check it is IP6
      // is a valid IP6 Address
      tmpAddrPtr=&((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
      char addressBuffer[INET6_ADDRSTRLEN];
      inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
      printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer); 
    }
*/
  }
  if (ifAddrStruct!=NULL) freeifaddrs(ifAddrStruct);
}

long int esystem::getMemLimit()
{
  struct rlimit rlim;
  getrlimit(RLIMIT_AS,&rlim);
  return(rlim.rlim_cur/1024);
}

int esystem::getTotalRam()
{
  struct sysinfo info;
  lerrorif(sysinfo(&info)!=0,"getting sysinfo");
  return(info.totalram*info.mem_unit/1024);
}

int esystem::getFreeRam()
{
  struct sysinfo info;
  lerrorif(sysinfo(&info)!=0,"getting sysinfo");
  return(info.freeram*info.mem_unit/1024);
}

int esystem::getBufferRam()
{
  struct sysinfo info;
  lerrorif(sysinfo(&info)!=0,"getting sysinfo");
  return(info.bufferram*info.mem_unit/1024);
}

int esystem::getSharedRam()
{
  struct sysinfo info;
  lerrorif(sysinfo(&info)!=0,"getting sysinfo");
  return(info.sharedram*info.mem_unit/1024);
}

int esystem::getTotalSwap()
{
  struct sysinfo info;
  lerrorif(sysinfo(&info)!=0,"getting sysinfo");
  return(info.totalswap*info.mem_unit/1024);
}

int esystem::getFreeSwap()
{
  struct sysinfo info;
  lerrorif(sysinfo(&info)!=0,"getting sysinfo");
  return(info.freeswap*info.mem_unit/1024);
}



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



