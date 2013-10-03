#ifndef ESYSTEM_OSX_H
#define ESYSTEM_OSX_H

#include "eutils-config.h"

#include "earray.h"
#include "efunc.h"

class esystemTimer
{
 public:
  CFRunLoopTimerRef tmref;
  efunc callback;
  evararray data;
};

class esystemCallback
{
 public:
  int fd;
  void *fdref;
  CFRunLoopSourceRef source;
  CFOptionFlags flags;
  evararray readData;
  efunc readCallback;

  evararray writeData;
  efunc writeCallback;
};


class esystem
{
 public:
  int waitfd;
  int waiting;

  earrayof<esystemCallback,int> callbacks;
  earrayof<esystemTimer,long int> timers;

  int getTotalRam();
  int getFreeRam();
  int getBufferRam();

  long int getMemLimit();

/*
  earray<int> fds;
  vector<void*> fdrefs;
  vector<CFRunLoopSourceRef> fdsources;
  ebasicarray<efunc*> funcs;
  ebasicarray<evar*> datas;

  vector<CFRunLoopTimerRef> tmrefs;
  ebasicarray<efunc*> tmfuncs;
*/

  long int addTimer(const efunc& callback,const evararray& data,double secs,double repeat);
  void removeTimer(long int tid);

  static esystem *cursystem;
  ProcessSerialNumber PSN;

  esystem();

//  bool processMessages();
//  bool processMessagesWait();
  void process();
  void run();

  static void handleTimerCallback(CFRunLoopTimerRef tmref, void *info);
  static void handleSocketCallback(CFSocketRef sref, CFSocketCallBackType callBackTypes, CFDataRef address, const void *data, void *info);
  static void handleFileCallback(CFFileDescriptorRef fdref, CFOptionFlags callBackTypes, void *info);
  
  void addReadWriteCallback(int fd,const efunc& readCallback,const evararray& readData,const efunc& writeCallback,const evararray& writeData);
  void addReadCallback(int fd,const efunc& func,const evararray& data);
  void addWriteCallback(int fd,const efunc& func,const evararray& data);

  void addSocketReadWriteCallback(int fd,const efunc& readCallback,const evararray& readData,const efunc& writeCallback,const evararray& writeData);
  void addSocketReadCallback(int fd,const efunc& func,const evararray& data);
  void addSocketWriteCallback(int fd,const efunc& func,const evararray& data);

  void enableReadWriteCallback(int fd);
  void enableReadCallback(int fd);
  void enableWriteCallback(int fd);
  void disableReadWriteCallback(int fd);
  void disableReadCallback(int fd);
  void disableWriteCallback(int fd);

  void enableSocketReadWriteCallback(int fd);
  void enableSocketReadCallback(int fd);
  void enableSocketWriteCallback(int fd);
  void disableSocketReadWriteCallback(int fd);
  void disableSocketReadCallback(int fd);
  void disableSocketWriteCallback(int fd);


/* deprecated */
  void addSocket(int fd,CFOptionFlags flags,efunc *func,evar *data=0x00);
  void removeSocket(int fd);

  void add(int fd,const efunc& func,evar *data=0x00);

  void addfunc(int fd,efunc *func);
  void remove(int fd);
/* end of deprecated */

  void wait(int fd=-1);  
};

//extern esystem fdwatch;

esystem *getSystem();

#endif

