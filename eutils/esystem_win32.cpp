#include "esystem_win32.h"

#include "logger.h"
#include "eiostream.h"
#include <windowsx.h>
#include <math.h>

#ifdef __MINGW32CE__
  #define WSTR(s) L ## s
#else
  #define WSTR(s) s
#endif

esystem* esystem::cursystem=0x00;

esystem *getSystem()
{
  if (!esystem::cursystem)
    esystem::cursystem=new esystem;
  return(esystem::cursystem);
}

/*
LRESULT CALLBACK WndProc(       HWND    hWnd,
                                UINT    uMsg,
                                WPARAM  wParam,
                                LPARAM  lParam)
{
//  if (uMsg == WM_SOCK){  
//  }else
  if (esystem::handleEvent)
    return(esystem::handleEvent(hWnd,uMsg,wParam,lParam));
  return(DefWindowProc(hWnd,uMsg,wParam,lParam));
}
*/

esystem::esystem()
{
  cursystem=this;

/*
  cout << " Getting Module Handle"<<endl;
  hInstance = GetModuleHandle(NULL);

  WNDCLASS wc;
#ifdef __MINGW32CE__
  wc.style       = CS_HREDRAW | CS_VREDRAW;
  wc.hIcon       = 0; //LoadIcon(NULL, IDI_WINLOGO);
  wc.hCursor     = 0; //LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
#else
  wc.style       = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  wc.hIcon       = LoadIcon(NULL, IDI_WINLOGO);
  wc.hCursor     = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = NULL;
#endif
  wc.lpfnWndProc = (WNDPROC) WndProc;
  wc.cbClsExtra  = 0;
  wc.cbWndExtra  = 0;
  wc.hInstance   = hInstance;
  wc.lpszMenuName  = NULL;
  wc.lpszClassName = WSTR("easycpp.edlib"); //szClassName;

  ecout << " Creating esystem!"<<endl;
  ldieif(!RegisterClass(&wc),"Failed to register The Window Class.\n");
  ecout << " Done creating esystem!"<<endl;
*/
}

esystem::~esystem()
{
/*
  if (!UnregisterClass(WSTR("easycpp.edlib"),hInstance))  //szClassName,hInstance))
    MessageBox(NULL,WSTR("Could Not Unregister Class."),WSTR("SHUTDOWN ERROR"),MB_OK | MB_ICONINFORMATION);
*/
}

bool esystem::processMessages()
{
  MSG  msg;
  while (PeekMessage(&msg,0,0,0,PM_REMOVE) != 0){
    if (msg.message == WM_QUIT)
      return(false);
    else{
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }
  return(true);
}

bool esystem::processMessagesWait()
{
  MSG  msg;
  if (GetMessage(&msg,0,0,0)){
    if (msg.message == WM_QUIT)
      return (false);
    else{
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
	return(true);
  }
  return(false);
}

void esystem::run()
{
  while(processMessagesWait());
}

void esystem::addSocket(SOCKET fd,const efunc& func,evar *data)
{
  return;
}

void esystem::removeSocket(int fd)
{
  return;
}

void esystem::add(HANDLE fHandle,const efunc& func,evar *data)
{
  fds.add(fHandle);
  funcs.addref(new efunc(func));
  datas.add(data);
}

void esystem::addfunc(HANDLE fHandle,efunc *func)
{
  fds.add(fHandle);
  funcs.addref(func);
  datas.add(NULL);
}


void esystem::remove(HANDLE fHandle)
{
  int i;
  i=fds.find(fHandle);
  lerrorifr(i==-1,"file handle not found",);

  fds.erase(i);
  funcs.erase(i);
  datas.erase(i);
}



void printError(const estr& lpszFunction) 
{ 
    // Retrieve the system error message for the last-error code

    LPVOID lpMsgBuf;
    DWORD dw = GetLastError(); 

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0, NULL );

    estr errmsg = (wchar_t*)lpMsgBuf;

    // Display the error message and exit the process

    printf("%s failed with error %d:\n",lpszFunction._str, dw);
    printf("%s\n", errmsg._str); 
//    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 

    LocalFree(lpMsgBuf);
}

void esystem::wait(HANDLE fHandle)
{
  if (fHandle == 0x00 && !fds.size()){
    processMessagesWait();
    return;
  }

  int i;
  int nCount;
  int ret;
  HANDLE *pHandles;
  do{ 
    nCount = fds.size();
    if (fHandle!=0x00) ++nCount;
    ldinfo("waiting on fds: "+estr(nCount)+" fd: "+estr((int)fHandle));

    pHandles = new HANDLE[nCount];

    for (i=0; i<fds.size(); ++i)
      pHandles[i]=fds[i];
    if (fHandle!=0x00)
      pHandles[i]=fHandle;
    ret=MsgWaitForMultipleObjects(nCount,pHandles,FALSE,INFINITE,QS_ALLEVENTS);
    delete[] pHandles;
    if (ret >= WAIT_OBJECT_0 && ret < WAIT_OBJECT_0 + nCount){
      ldinfo("Got msg on fd");
      i=ret-WAIT_OBJECT_0;
      if (fHandle!=0x00 && i==nCount-1){
        return;
      }
      evararray arr;
      if (datas[i]){
        arr.add(datas[i]->var);
        funcs[i].call(arr);
      }else
        funcs[i].call(arr);
    }else if (ret == WAIT_OBJECT_0 + nCount) {
      ldinfo("Got system MSG event");
      processMessages();
    }else if (ret == WAIT_TIMEOUT)
      lwarn("got timeout event");
    else if (ret == WAIT_FAILED){
      printError("MsgWaitForMultipleObjects");
      lwarn("got error event");
    }
  } while (fHandle!=0x00);
}

