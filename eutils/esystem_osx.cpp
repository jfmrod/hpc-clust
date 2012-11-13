#include "esystem_osx.h"

#include "ebasicarray.h"
#include "evar.h"

#include <stdio.h>
#include <MacMemory.h>

#import <Cocoa/Cocoa.h>



#import <sys/sysctl.h>
#import <mach/host_info.h>
#import <mach/mach_host.h>
#import <mach/task_info.h>
#import <mach/task.h>

long int esystem::getMemLimit()
{
  return(-1); // unlimited
}

int esystem::getTotalRam()
{
  int mib[6]; 
  mib[0] = CTL_HW;
  mib[1] = HW_PAGESIZE;

  int pagesize;
  size_t length;
  length = sizeof (pagesize);
  if (sysctl (mib, 2, &pagesize, &length, NULL, 0) < 0)
  {
    fprintf (stderr, "getting page size");
  }

  mach_msg_type_number_t count = HOST_VM_INFO_COUNT;

  vm_statistics_data_t vmstat;
  if (host_statistics (mach_host_self (), HOST_VM_INFO, (host_info_t) &vmstat, &count) != KERN_SUCCESS)
  {
    fprintf (stderr, "Failed to get VM statistics.");
  }

//  double total = vmstat.wire_count + vmstat.active_count + vmstat.inactive_count + vmstat.free_count;
//   double wired = vmstat.wire_count / total;
//  double active = vmstat.active_count / total;
//  double inactive = vmstat.inactive_count / total;
//  double free = vmstat.free_count / total;

//  task_basic_info_64_data_t info;
//  unsigned size = sizeof (info);
//  task_info (mach_task_self (), TASK_BASIC_INFO_64, (task_info_t) &info, &size);

//  double unit = 1024 * 1024;
//  memLabel.text = [NSString stringWithFormat: @"% 3.1f MB\n% 3.1f MB\n% 3.1f MB", vmstat.free_count * pagesize / unit, (vmstat.free_count + vmstat.inactive_count) * pagesize / unit, info.resident_size / unit];
  return(((double)vmstat.wire_count+(double)vmstat.active_count+(double)vmstat.inactive_count+(double)vmstat.free_count)*(double)pagesize/1024);
}

int esystem::getFreeRam()
{
  int mib[6]; 
  mib[0] = CTL_HW;
  mib[1] = HW_PAGESIZE;

  int pagesize;
  size_t length;
  length = sizeof (pagesize);
  if (sysctl (mib, 2, &pagesize, &length, NULL, 0) < 0)
  {
    fprintf (stderr, "getting page size");
  }

  mach_msg_type_number_t count = HOST_VM_INFO_COUNT;

  vm_statistics_data_t vmstat;
  if (host_statistics (mach_host_self (), HOST_VM_INFO, (host_info_t) &vmstat, &count) != KERN_SUCCESS)
  {
    fprintf (stderr, "Failed to get VM statistics.");
  }

  return((double)vmstat.free_count*(double)pagesize/1024);
}

int esystem::getBufferRam()
{
  int mib[6]; 
  mib[0] = CTL_HW;
  mib[1] = HW_PAGESIZE;

  int pagesize;
  size_t length;
  length = sizeof (pagesize);
  if (sysctl (mib, 2, &pagesize, &length, NULL, 0) < 0)
  {
    fprintf (stderr, "getting page size");
  }

  mach_msg_type_number_t count = HOST_VM_INFO_COUNT;

  vm_statistics_data_t vmstat;
  if (host_statistics (mach_host_self (), HOST_VM_INFO, (host_info_t) &vmstat, &count) != KERN_SUCCESS)
  {
    fprintf (stderr, "Failed to get VM statistics.");
  }

  return((double)vmstat.inactive_count*(double)pagesize/1024);
}



esystem *esystem::cursystem=0x00;

esystem *getSystem()
{
  if (!esystem::cursystem)
    esystem::cursystem=new esystem;
  return(esystem::cursystem);
}

esystem::esystem(): waiting(0),waitfd(-1)
{
  GetCurrentProcess(&PSN);
  SetFrontProcess(&PSN);
  [NSApplication sharedApplication];
}

void esystem::handleSocketCallback(CFSocketRef sref, CFSocketCallBackType callbackTypes, CFDataRef address, const void *data, void *info)
{
//  cout << "handleSocketCallback"<<endl;
  esystemCallback *cb=(esystemCallback*)info;
  if ((callbackTypes & kCFSocketReadCallBack) && cb->readCallback.isSet())
    cb->readCallback.call(cb->readData);
  if ((callbackTypes & kCFSocketWriteCallBack) && cb->writeCallback.isSet())
    cb->writeCallback.call(cb->writeData);

  // stops the loop from running and returns if the user called the ::wait function
  esystem *psystem=getSystem();
  if (psystem->waiting){
    if (psystem->waitfd==-1 || psystem->waitfd==cb->fd)
      { --psystem->waiting; CFRunLoopStop(CFRunLoopGetMain()); }
  }
}

void esystem::handleFileCallback(CFFileDescriptorRef fdref, CFOptionFlags callBackTypes, void *info)
{
  int i;
//  esystem *psystem=(esystem*)info;
  esystemCallback *cb=(esystemCallback*)info;



  if ((callBackTypes & kCFFileDescriptorReadCallBack) && cb->readCallback.isSet())
    cb->readCallback.call(cb->readData);
  if ((callBackTypes & kCFFileDescriptorWriteCallBack) && cb->writeCallback.isSet())
    cb->writeCallback.call(cb->writeData);
//  cout << "Got PIPE read: " << cb->fd << " " << cb->flags << " " << (cb->flags&kCFFileDescriptorReadCallBack) <<endl;
  CFFileDescriptorEnableCallBacks(fdref, cb->flags);

  // stops the loop from running and returns if the user called the ::wait function
//  ldinfo("got handle file callback: "+estr(i)+" fd: "+estr(psystem->callbacks.values(i).fd)+" system: "+estr(psystem->waiting)+" sysfd: "+estr(psystem->waitfd));

  esystem *psystem=getSystem();
  if (psystem->waiting){
    if (psystem->waitfd==-1 || psystem->waitfd==cb->fd)
      { --psystem->waiting; CFRunLoopStop(CFRunLoopGetMain()); }
  }
}

void esystem::handleTimerCallback(CFRunLoopTimerRef tmref, void *info)
{
  esystemTimer *t=(esystemTimer*)info;
  t->callback.call(t->data);
}

void esystem::addSocketReadWriteCallback(int fd,const efunc& readCallback,const evararray& readData,const efunc& writeCallback,const evararray& writeData)
{
  esystemCallback *cb=new esystemCallback;

  cb->fd=fd;
  cb->readCallback=readCallback;
  cb->readData=readData;
  cb->writeCallback=writeCallback;
  cb->writeData=writeData;
  cb->flags=kCFSocketReadCallBack | kCFSocketWriteCallBack;

  CFSocketContext sContext;
  bzero(&sContext,sizeof(sContext));
  sContext.version=0;
  sContext.info=(void*)cb;

  CFSocketRef sref = CFSocketCreateWithNative(kCFAllocatorDefault,fd,cb->flags,esystem::handleSocketCallback,&sContext);
  ldieif(sref==NULL,"sref is NULL");

  cb->fdref=sref;

  CFRunLoopSourceRef source = CFSocketCreateRunLoopSource(kCFAllocatorDefault, sref, 0);
  CFRunLoopAddSource(CFRunLoopGetMain(), source, kCFRunLoopDefaultMode);
  cb->source=source;
  callbacks.addref(fd,cb);
}

void esystem::addSocketWriteCallback(int fd,const efunc& writeCallback,const evararray& writeData)
{
  esystemCallback *cb=new esystemCallback;

  cb->fd=fd;
  cb->writeCallback=writeCallback;
  cb->writeData=writeData;
  cb->flags=kCFSocketWriteCallBack;

  CFSocketContext sContext;
  bzero(&sContext,sizeof(sContext));
  sContext.version=0;
  sContext.info=(void*)cb;

  CFSocketRef sref = CFSocketCreateWithNative(kCFAllocatorDefault,fd,cb->flags,esystem::handleSocketCallback,&sContext);
  ldieif(sref==NULL,"sref is NULL");

  cb->fdref=sref;

  CFRunLoopSourceRef source = CFSocketCreateRunLoopSource(kCFAllocatorDefault, sref, 0);
  CFRunLoopAddSource(CFRunLoopGetMain(), source, kCFRunLoopDefaultMode);
  cb->source=source;
  callbacks.addref(fd,cb);
}

void esystem::addSocketReadCallback(int fd,const efunc& readCallback,const evararray& readData)
{
  esystemCallback *cb=new esystemCallback;

  cb->fd=fd;
  cb->readCallback=readCallback;
  cb->readData=readData;
  cb->flags=kCFSocketReadCallBack;

  CFSocketContext sContext;
  bzero(&sContext,sizeof(sContext));
  sContext.version=0;
  sContext.info=(void*)cb;

  CFSocketRef sref = CFSocketCreateWithNative(kCFAllocatorDefault,fd,cb->flags,esystem::handleSocketCallback,&sContext);
  ldieif(sref==NULL,"sref is NULL");

  cb->fdref=sref;

  CFRunLoopSourceRef source = CFSocketCreateRunLoopSource(kCFAllocatorDefault, sref, 0);
  CFRunLoopAddSource(CFRunLoopGetMain(), source, kCFRunLoopDefaultMode);
  cb->source=source;
  callbacks.addref(fd,cb);
}

void esystem::addSocket(int fd,CFOptionFlags flags,efunc *func,evar *data)
{
  esystemCallback *cb=new esystemCallback;

  cb->fd=fd;
  cb->readCallback=*func;
  cb->readData=*data;

  CFSocketContext sContext;
  bzero(&sContext,sizeof(sContext));
  sContext.version=0;
  sContext.info=(void*)cb;

/*
   kCFSocketNoCallBack = 0,
   kCFSocketReadCallBack = 1,
   kCFSocketAcceptCallBack = 2,
   kCFSocketDataCallBack = 3,
   kCFSocketConnectCallBack = 4,
   kCFSocketWriteCallBack = 8
*/

  CFSocketRef sref = CFSocketCreateWithNative(kCFAllocatorDefault,fd,flags,esystem::handleSocketCallback,&sContext);
  ldieif(sref==NULL,"sref is NULL");

  cb->fdref=sref;
//  fdrefs.push_back(sref);

//  CFFileDescriptorEnableCallBacks(fdref, kCFFileDescriptorReadCallBack);
  CFRunLoopSourceRef source = CFSocketCreateRunLoopSource(kCFAllocatorDefault, sref, 0);
  CFRunLoopAddSource(CFRunLoopGetMain(), source, kCFRunLoopDefaultMode);
  cb->source=source;
  callbacks.addref(fd,cb);
//  fdsources.push_back(source);
}

void esystem::disableSocketReadWriteCallback(int fd)
{
  int i;
  i=callbacks.findkey(fd);
  lddieif(i==-1,"socket not found with fd: "+estr(fd));

  esystemCallback *cb=&callbacks.values(i);
  cb->flags=(cb->flags ^ kCFSocketReadCallBack) ^ kCFSocketWriteCallBack;
  CFSocketDisableCallBacks((CFSocketRef)cb->fdref, kCFSocketReadCallBack | kCFSocketWriteCallBack);
}

void esystem::disableSocketReadCallback(int fd)
{
  int i;
  i=callbacks.findkey(fd);
  lddieif(i==-1,"socket not found with fd: "+estr(fd));

  esystemCallback *cb=&callbacks.values(i);
  cb->flags=cb->flags ^ kCFSocketReadCallBack;
  CFSocketDisableCallBacks((CFSocketRef)cb->fdref, kCFSocketReadCallBack);
}

void esystem::disableSocketWriteCallback(int fd)
{
  int i;
  i=callbacks.findkey(fd);
  lddieif(i==-1,"socket not found with fd: "+estr(fd));

  esystemCallback *cb=&callbacks.values(i);
  cb->flags=cb->flags ^ kCFSocketWriteCallBack;
  CFSocketDisableCallBacks((CFSocketRef)cb->fdref, kCFSocketWriteCallBack);
}

void esystem::enableSocketReadWriteCallback(int fd)
{
  int i;
  i=callbacks.findkey(fd);
  lddieif(i==-1,"socket not found with fd: "+estr(fd));

  esystemCallback *cb=&callbacks.values(i);
  cb->flags=cb->flags | kCFSocketReadCallBack | kCFSocketWriteCallBack;
  CFSocketEnableCallBacks((CFSocketRef)cb->fdref, kCFSocketReadCallBack | kCFSocketWriteCallBack);
}

void esystem::enableSocketReadCallback(int fd)
{
  int i;
  i=callbacks.findkey(fd);
  lddieif(i==-1,"socket not found with fd: "+estr(fd));

  esystemCallback *cb=&callbacks.values(i);
  cb->flags=cb->flags | kCFSocketReadCallBack;
  CFSocketEnableCallBacks((CFSocketRef)cb->fdref, kCFSocketReadCallBack);
}

void esystem::enableSocketWriteCallback(int fd)
{
  int i;
  i=callbacks.findkey(fd);
  lddieif(i==-1,"socket not found with fd: "+estr(fd));

  esystemCallback *cb=&callbacks.values(i);
  cb->flags=cb->flags | kCFSocketWriteCallBack;
  CFSocketEnableCallBacks((CFSocketRef)cb->fdref, kCFSocketWriteCallBack);
}

void esystem::disableReadWriteCallback(int fd)
{
  int i;
  i=callbacks.findkey(fd);
  lddieif(i==-1,"file not found with fd: "+estr(fd));

  esystemCallback *cb=&callbacks.values(i);
  cb->flags=(cb->flags ^ kCFFileDescriptorReadCallBack) ^ kCFFileDescriptorWriteCallBack;
  CFFileDescriptorDisableCallBacks((CFFileDescriptorRef)cb->fdref, kCFFileDescriptorReadCallBack | kCFFileDescriptorWriteCallBack);
}

void esystem::disableReadCallback(int fd)
{
  int i;
  i=callbacks.findkey(fd);
  lddieif(i==-1,"file not found with fd: "+estr(fd));

  esystemCallback *cb=&callbacks.values(i);
  cb->flags=cb->flags ^ kCFFileDescriptorReadCallBack;
  CFFileDescriptorDisableCallBacks((CFFileDescriptorRef)cb->fdref, kCFFileDescriptorReadCallBack);
}

void esystem::disableWriteCallback(int fd)
{
  int i;
  i=callbacks.findkey(fd);
  lddieif(i==-1,"file not found with fd: "+estr(fd));

  esystemCallback *cb=&callbacks.values(i);
  cb->flags=cb->flags ^ kCFFileDescriptorWriteCallBack;
  CFFileDescriptorDisableCallBacks((CFFileDescriptorRef)cb->fdref, kCFFileDescriptorWriteCallBack);
}

void esystem::enableReadWriteCallback(int fd)
{
  int i;
  i=callbacks.findkey(fd);
  lddieif(i==-1,"file not found with fd: "+estr(fd));

  esystemCallback *cb=&callbacks.values(i);
  cb->flags=cb->flags | kCFFileDescriptorReadCallBack | kCFFileDescriptorWriteCallBack;
  CFFileDescriptorEnableCallBacks((CFFileDescriptorRef)cb->fdref, kCFFileDescriptorReadCallBack | kCFFileDescriptorWriteCallBack);
}

void esystem::enableReadCallback(int fd)
{
  int i;
  i=callbacks.findkey(fd);
  lddieif(i==-1,"file not found with fd: "+estr(fd));

  esystemCallback *cb=&callbacks.values(i);
  cb->flags=cb->flags | kCFFileDescriptorReadCallBack;
  CFFileDescriptorEnableCallBacks((CFFileDescriptorRef)cb->fdref, kCFFileDescriptorReadCallBack);
}

void esystem::enableWriteCallback(int fd)
{
  int i;
  i=callbacks.findkey(fd);
  lddieif(i==-1,"file not found with fd: "+estr(fd));

  esystemCallback *cb=&callbacks.values(i);
  cb->flags=cb->flags | kCFFileDescriptorWriteCallBack;
  CFFileDescriptorEnableCallBacks((CFFileDescriptorRef)cb->fdref, kCFFileDescriptorWriteCallBack);
}

void esystem::removeSocket(int fd)
{
  int i;
  i=callbacks.findkey(fd);
  ldieif(i==-1,"socket not found with fd: "+estr(fd));

  CFRunLoopRemoveSource(CFRunLoopGetMain(), callbacks.values(i).source, kCFRunLoopDefaultMode);
  CFRelease(callbacks.values(i).source);
  CFSocketInvalidate((CFSocketRef)callbacks.values(i).fdref);
  CFRelease((CFSocketRef)callbacks.values(i).fdref);
  callbacks.erase(i);
}

long int esystem::addTimer(const efunc& callback,const evararray& data,double secs,double repeat)
{
  esystemTimer *t=new esystemTimer;
  t->callback=callback;
  t->data=data;

  CFRunLoopTimerContext tmContext;
  bzero(&tmContext,sizeof(tmContext));
  tmContext.version=0;
  tmContext.info=(void*)t;

  t->tmref = CFRunLoopTimerCreate(kCFAllocatorDefault, CFAbsoluteTimeGetCurrent()+secs, repeat , 0, 0, esystem::handleTimerCallback, &tmContext);
  lassert(t->tmref==NULL);

  CFRunLoopAddTimer(CFRunLoopGetMain(), t->tmref, kCFRunLoopDefaultMode);
  timers.addref((long int)t->tmref,t);
  return((long int)t->tmref);
}

void esystem::removeTimer(long int tid)
{
  int i;
  i=timers.findkey(tid);
  ldieif(i==-1,"timer not found with id: "+estr(tid));

  CFRunLoopTimerInvalidate(timers.values(i).tmref);
  timers.erase(i);
}

void esystem::addReadWriteCallback(int fd,const efunc& readCallback,const evararray& readData,const efunc& writeCallback,const evararray& writeData)
{
  esystemCallback *cb=new esystemCallback;
  cb->fd=fd;
  cb->readCallback=readCallback;
  cb->readData=readData;
  cb->writeCallback=writeCallback;
  cb->writeData=writeData;
  cb->flags=kCFFileDescriptorReadCallBack | kCFFileDescriptorWriteCallBack;

  CFFileDescriptorContext fdContext;
  bzero(&fdContext,sizeof(fdContext));
  fdContext.version=0;
  fdContext.info=(void*)cb;

  CFFileDescriptorRef fdref = CFFileDescriptorCreate(kCFAllocatorDefault, fd, false, esystem::handleFileCallback, &fdContext);
  ldieif(fdref==NULL,"fdref is NULL");
  cb->fdref=fdref;

  CFFileDescriptorEnableCallBacks(fdref, cb->flags);
  cb->source = CFFileDescriptorCreateRunLoopSource(kCFAllocatorDefault, fdref, 0);
  CFRunLoopAddSource(CFRunLoopGetMain(), cb->source, kCFRunLoopDefaultMode);
  callbacks.addref(fd,cb);
  CFFileDescriptorEnableCallBacks(fdref, cb->flags);
}

void esystem::addWriteCallback(int fd,const efunc& writeCallback,const evararray& writeData)
{
  esystemCallback *cb=new esystemCallback;
  cb->fd=fd;
  cb->writeCallback=writeCallback;
  cb->writeData=writeData;
  cb->flags=kCFFileDescriptorWriteCallBack;

  CFFileDescriptorContext fdContext;
  bzero(&fdContext,sizeof(fdContext));
  fdContext.version=0;
  fdContext.info=(void*)cb;

  CFFileDescriptorRef fdref = CFFileDescriptorCreate(kCFAllocatorDefault, fd, false, esystem::handleFileCallback, &fdContext);
  ldieif(fdref==NULL,"fdref is NULL");
  cb->fdref=fdref;

  CFFileDescriptorEnableCallBacks(fdref, cb->flags);
  cb->source = CFFileDescriptorCreateRunLoopSource(kCFAllocatorDefault, fdref, 0);
  CFRunLoopAddSource(CFRunLoopGetMain(), cb->source, kCFRunLoopDefaultMode);
  callbacks.addref(fd,cb);
  CFFileDescriptorEnableCallBacks(fdref, cb->flags);
}

void esystem::addReadCallback(int fd,const efunc& readCallback,const evararray& readData)
{
  esystemCallback *cb=new esystemCallback;
  cb->fd=fd;
  cb->readCallback=readCallback;
  cb->readData=readData;
  cb->flags=kCFFileDescriptorReadCallBack;

  CFFileDescriptorContext fdContext;
  bzero(&fdContext,sizeof(fdContext));
  fdContext.version=0;
  fdContext.info=(void*)cb;

  CFFileDescriptorRef fdref = CFFileDescriptorCreate(kCFAllocatorDefault, fd, false, esystem::handleFileCallback, &fdContext);
  ldieif(fdref==NULL,"fdref is NULL");

  cb->fdref=fdref;

  CFFileDescriptorEnableCallBacks(fdref, cb->flags);
  cb->source = CFFileDescriptorCreateRunLoopSource(kCFAllocatorDefault, fdref, 0);
  CFRunLoopAddSource(CFRunLoopGetMain(), cb->source, kCFRunLoopDefaultMode);
  callbacks.addref(fd,cb);
  CFFileDescriptorEnableCallBacks(fdref, cb->flags);
}

void esystem::add(int fd,const efunc& func,evar *data)
{
  addReadCallback(fd,func,evararray(*data));
}

void esystem::addfunc(int fd,efunc *func)
{
  addReadCallback(fd,*func,evararray());
}

void esystem::remove(int fd)
{
  int i;
  for (i=0; i<callbacks.size(); ++i){
    if (callbacks.values(i).fd==fd) break;
  }
  ldieif(i==callbacks.size(),"file descriptor not found");

  CFRunLoopRemoveSource(CFRunLoopGetMain(), callbacks.values(i).source, kCFRunLoopDefaultMode);
  CFRelease(callbacks.values(i).source);
  CFFileDescriptorInvalidate((CFFileDescriptorRef)callbacks.values(i).fdref);
  CFRelease((CFFileDescriptorRef)callbacks.values(i).fdref);
  callbacks.erase(i);
}

void esystem::run()
{
  [NSApp run];
/*
  EventRef theEvent;
  EventTargetRef theTarget;

  theTarget = GetEventDispatcherTarget();
  while  (ReceiveNextEvent(0, NULL,kEventDurationForever,true,&theEvent) == noErr){
    SendEventToEventTarget (theEvent, theTarget);
    ReleaseEvent(theEvent);
  }
*/
}

void esystem::process()
{
  EventRef theEvent;
  EventTargetRef theTarget;

  theTarget = GetEventDispatcherTarget();
  while ( ReceiveNextEvent(0, NULL,0,true, &theEvent) == noErr ){
    SendEventToEventTarget (theEvent, theTarget);
    ReleaseEvent(theEvent);
  }

//  return(true);
//  CFRunLoopRunInMode(kCFRunLoopDefaultMode,0,false);
}

void esystem::wait(int fd)
{
  ++waiting;
  if (fd!=-1){
    waitfd=fd;
    addReadCallback(fd,efunc(),evararray());
//    addfunc(fd,0x00);
    linfo("waiting: "+estr(fd)+", runnning loop");
  }

//  run();

//  RunCurrentEventLoop(kEventDurationForever);
  CFRunLoopRun();

  linfo("ended loop");

  if (fd!=-1){
    remove(fd);
    waitfd=-1;
  }
}
