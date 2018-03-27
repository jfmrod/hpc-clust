#ifndef ESYSTEM_LINUX_H
#define ESYSTEM_LINUX_H

#include "eutils-config.h"

#include "evar.h"
#include "earray.h"
#include "efunc.h"

#include <signal.h>

class esystemCallback
{
 public:
  int fd;
  bool readEnabled;
  evararray readData;
  efunc readCallback;

  bool writeEnabled;
  evararray writeData;
  efunc writeCallback;

  esystemCallback();
};

class esystemTimer
{
 public:
  timer_t timer;
  efunc callback;
  evararray data;
};

class esystem
{
 public:
  earrayof<esystemCallback,int> callbacks;
  earrayof<esystemTimer,int> timers;

//  earray<int> fds;
//  earray<efunc> funcs;
//  ebasicarray<evar> datas;

  static esystem *cursystem;

  esystem();
//  bool processMessages();
//  bool processMessagesWait();
  void run();

  estrarray getLocalAddresses();

  long int getMemLimit();

  int getTotalRam();
  int getFreeRam();
  int getBufferRam();
  int getSharedRam();
  int getTotalSwap();
  int getFreeSwap();

  void addTimer(const efunc& func,const evararray& args,double secs,double repeat);
  static void handleTimerSignal(int signal,siginfo_t *si,void *data);

  void enableReadWriteCallback(int fd);
  void enableReadCallback(int fd);
  void enableWriteCallback(int fd);
  void disableReadWriteCallback(int fd);
  void disableReadCallback(int fd);
  void disableWriteCallback(int fd);

  void addReadWriteCallback(int fd,const efunc& readCallback,const evararray& readData,const efunc& writeCallback,const evararray& writeData);
  void addWriteCallback(int fd,const efunc& func,const evararray& data);
  void addReadCallback(int fd,const efunc& func,const evararray& data);

  inline void enableSocketReadWriteCallback(int fd) { enableReadWriteCallback(fd); }
  inline void enableSocketReadCallback(int fd) { enableReadCallback(fd); }
  inline void enableSocketWriteCallback(int fd) { enableWriteCallback(fd); }
  inline void disableSocketReadWriteCallback(int fd) { disableReadWriteCallback(fd); }
  inline void disableSocketReadCallback(int fd) { disableReadCallback(fd); }
  inline void disableSocketWriteCallback(int fd) { disableWriteCallback(fd); }

  inline void addSocketReadWriteCallback(int fd,const efunc& readCallback,const evararray& readData,const efunc& writeCallback,const evararray& writeData) { addReadWriteCallback(fd,readCallback,readData,writeCallback,writeData); }
  inline void addSocketWriteCallback(int fd,const efunc& func,const evararray& data) { addWriteCallback(fd,func,data); }
  inline void addSocketReadCallback(int fd,const efunc& func,const evararray& data) { addReadCallback(fd,func,data); }


  inline void addSocket(int fd,const efunc& func,const evar& data=evar()) { add(fd,func,data); }
  inline void removeSocket(int fd) { remove(fd); }

  void add(int fd,const efunc& func,const evar& data=evar());

  void addfunc(int fd,efunc *func);

  void remove(int fd);
  void wait(int fd=-1);  
};

//extern esystem fdwatch;

esystem *getSystem();

#endif

