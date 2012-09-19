#ifndef __ESYSTEM_WIN32__
#define __ESYSTEM_WIN32__

#include "eutils.h"

#include "earray.h"
#include "efunc.h"

class esystem
{
 public:
  earray<HANDLE> fds;
  earray<efunc> funcs;
  ebasicarray<evar*> datas;
  HINSTANCE hInstance;

  static esystem *cursystem;
  static int (*handleEvent)(HWND hWnd,UINT uMsg,DWORD wParam,DWORD lParam);

  bool processMessages();
  bool processMessagesWait();
  void run();

  void addSocket(int fd,const efunc& func,evar *data=0x00);
  void removeSocket(int fd);

  void addSocket(SOCKET fHandle,const efunc& func,evar *data=0x00);
  void removeSocket(SOCKET fHandle);

  void add(HANDLE fHandle,const efunc& func,evar *data=0x00);
  void addfunc(HANDLE fHandle,efunc *func);
  void remove(HANDLE fHandle);
  void wait(HANDLE fHandle=0x00); 

  esystem();
  ~esystem();
};

esystem *getSystem();

#endif

