#include "esocket.h"

#include <ctype.h>

#ifdef WIN32
 #include <windows.h>
#else
 #include <netdb.h>
 #include <errno.h>
 #include <netinet/in.h>
 #include <string.h>

 #include <sys/socket.h>
 #include <sys/types.h>
 #include <sys/stat.h>
 #include <sys/time.h>

 #include <arpa/inet.h>

 #include <fcntl.h>
 #include <unistd.h>
#endif

#include "logger.h"
#include "efunc.h"
#include "esystem.h"

//#include <signal.h>

/*
esocket_manager socket_manager;

void esocket_manager::add(esocket *socket)
{
  printf("adding socket %i to list\n",socket->fhsock);
  sockets.push_back(socket);
}

void esocket_manager::rem(esocket *socket)
{
  std::list<esocket*>::iterator iter;
  printf("trying to remove socket %i from list\n",socket->fhsock);

  for (iter=sockets.begin(); iter!=sockets.end(); iter++){
    if (*iter == socket){
      printf("removing socket %i from list\n",socket->fhsock);
      sockets.erase(iter);
      break;
    } 
  }
}

void esocket_manager::check_sockets()
{
  std::list<esocket*>::iterator iter;

  fd_set rdfs;
  int n;

  if (!socket_manager.sockets.size()) return;

  n = (*socket_manager.sockets.begin())->fhsock;

  for (iter=socket_manager.sockets.begin(); iter!=socket_manager.sockets.end(); iter++){
    FD_SET((*iter)->fhsock,&rdfs);
    if (n<(*iter)->fhsock) n = (*iter)->fhsock;
  }

  select(n+1,&rdfs,0x00,0x00,0x00);

  for (iter=socket_manager.sockets.begin(); iter!=socket_manager.sockets.end(); iter++){
    if (FD_ISSET((*iter)->fhsock,&rdfs)){
      (*iter)->doRecv();
    }
  }
}

void esocket_manager::signal_handler(int signal, siginfo_t *siginfo, void *some)
{
  if (siginfo->si_code == SI_SIGIO)
    printf("handling sigio for fd: %i\n",siginfo->si_fd);
  else{
    socket_manager.check_sockets();
  }
}

esocket_manager::esocket_manager()
{
  struct sigaction sa;

  sa.sa_sigaction = &socket_manager.signal_handler;
  sa.sa_flags = SA_SIGINFO;
  if (sigaction(SIGIO,&sa,0x00)) printf("error installing signal handler\n");
}

esocket_manager::~esocket_manager()
{
}
*/


#ifdef _WIN32
bool wsastartup=false;
#endif


eudpsocket::eudpsocket(): fhsock(0)
{
#ifdef _WIN32
  if (!wsastartup){
    WSADATA WSData;
    WSAStartup(MAKEWORD(2,2),&WSData);
    wsastartup=true;
  }
#endif
}

eudpsocket::eudpsocket(int hsock,struct sockaddr_in addr_in): fhsock(hsock), faddr_in(addr_in)
{
  faddress=inet_ntoa(faddr_in.sin_addr);
  fport=ntohs(faddr_in.sin_port);
  setCallback();
}

void eudpsocket::setBroadcast(bool value)
{
  int on=(value?1:0);
  int ret=setsockopt( fhsock, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on) );
  lwarnif(ret!=0,"error setting socket SO_BROADCAST flag");
}

void eudpsocket::setReuseAddress(bool value)
{
  int on=(value?1:0);
  int ret=setsockopt( fhsock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );
  lwarnif(ret!=0,"error setting socket SO_REUSEADDR flag");
}

void eudpsocket::setBlocking(bool value)
{
#ifndef _WIN32
  if (value)
    fcntl(fhsock, F_SETFL, O_NONBLOCK | O_ASYNC);
  else
    fcntl(fhsock, F_SETFL, 0x00 );
#endif
}

void eudpsocket::setCallback()
{
#ifdef _WIN32
  if (!wsastartup){
    WSADATA WSData;
    WSAStartup(MAKEWORD(2,2),&WSData);
    wsastartup=true;
  }
  u_long opt;
  opt = 1;
  ioctlsocket(fhsock,FIONBIO,&opt);
#elif defined(__APPLE__)
    int tmpo=1;
    if(0!=setsockopt(fhsock,SOL_SOCKET,SO_NOSIGPIPE,&tmpo,sizeof(tmpo)))
      lerror("setsockopt: "+estr(errno));
#endif

  setBlocking(false);

#ifdef _WIN32
  wsaEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
  WSAEventSelect(fhsock,wsaEvent,FD_READ);
  getSystem()->addfunc(wsaEvent,new efunc(*this,&eudpsocket::_doRecv));
//#elif defined(__APPLE__)
//  getSystem()->addSocketReadWriteCallback(fhsock,efunc(*this,&eudpsocket::_doRecv),evararray());
//  getSystem()->addSocket(fhsock,kCFSocketReadCallBack,new efunc(*this,&eudpsocket::_doRecv),0x00);
//  getSystem()->addSocketReadWriteCallback(fhsock,efunc(*this,&eudpsocket::_doRecv),evararray(),efunc(*this,&eudpsocket::_doSend),evararray());
#else
  getSystem()->addSocketReadWriteCallback(fhsock,efunc(*this,&eudpsocket::_doRecv),evararray(),efunc(*this,&eudpsocket::_doSend),evararray());
  // TODO: disableWriteCallback();
#endif
}

eudpsocket::~eudpsocket()
{
  close();
}

void eudpsocket::close()
{
  if (!fhsock) return;

#ifdef _WIN32
  closeudpsocket(fhsock);
#else
  ::close(fhsock);
#endif

#ifdef _WIN32
  getSystem()->remove(wsaEvent);
  WSACloseEvent(wsaEvent);
  wsaEvent=0x00;
#elif defined(__APPLE__)
  getSystem()->removeSocket(fhsock);
#else
  getSystem()->removeSocket(fhsock);
#endif
  fhsock = 0;
}

bool eudpsocket::listen(int port,const estr& address,bool reuseaddress)
{
  int constat;
  faddress=address;
  fport=port;

  fhsock = socket(PF_INET,SOCK_DGRAM,0);
  lerrorifr(fhsock == -1,"unable to create socket",false);

  if (reuseaddress) { setReuseAddress(true); setBroadcast(true); }

  faddr_in.sin_family = AF_INET;
  faddr_in.sin_addr.s_addr = inet_addr(faddress._str);
  faddr_in.sin_port = htons(fport);
  constat = bind(fhsock, (struct sockaddr *)&faddr_in,sizeof(faddr_in));
#ifndef __MINGW32CE__
  lerrorifr(constat==-1,"unable to bind socket: "+estr(errno),false);
#else
  lerrorifr(constat==-1,"unable to bind socket: unable to get error",false);
#endif

  setCallback();
 
  linfo("listening on address: "+address+" port: "+estr(port));
  return(true);
}

bool eudpsocket::sendto(const estr& data,const estr& address,int port)
{
  struct hostent     *phe;
  struct sockaddr_in addr_in;

#ifdef _WIN32
  if (isdigit(address[0])){
    addr_in.sin_addr.s_addr = inet_addr(address._str);
  }else{
    phe = (hostent *)gethostbyname(address._str);
    lwarnifr(phe==NULL,"unable to resolve hostname",(false));
//    if (phe == NULL){ esockClose(hsock); logger.error("esocklib.cpp, esockConnect: hostname lookup failed"); return(-1); }
    addr_in.sin_addr.s_addr = *(u_long *)(phe->h_addr_list[0]);
  }
#else
  if (isdigit(address[0])){
    inet_aton(address._str,&addr_in.sin_addr); 
  } else {
    phe = (struct hostent *)gethostbyname(address._str);
    lwarnifr(phe==NULL,"unable to resolve hostname",(false));

    addr_in.sin_addr.s_addr = *(in_addr_t*)phe->h_addr_list[0];
  }
#endif

  addr_in.sin_family = AF_INET;
  addr_in.sin_port = htons(port);

  linfo("sending udp packet to ip "+estr(inet_ntoa(addr_in.sin_addr)));
  if (::sendto(fhsock,data._str,data.len(),0, (struct sockaddr *)&addr_in,sizeof(addr_in)) == -1)
    { lwarn("unable to send udp to server "+address+" on port "+estr(port)); return(false); }

  return(true);
}

void eudpsocket::doClose(){}
void eudpsocket::doRecv(){}
void eudpsocket::doSend(){}

void eudpsocket::_doClose()
{
  doClose();
  if (onClose.isSet())
    onClose.call();
}

void eudpsocket::_doRecv()
{
  doRecv();
  if (onReceive.isSet())
    onReceive.call();
}

void eudpsocket::_doSend()
{
  doSend();
  if (onSend.isSet())
    onSend.call();
}


int eudpsocket::recvfrom(estr& buffer,estr& address,int& port)
{
  struct sockaddr_in addr_in;
  socklen_t slen=sizeof(addr_in); 

  buffer.clear();
  buffer.reserve(2048);
  int rlen;
  rlen=::recvfrom(fhsock,buffer._str,2048,0,(struct sockaddr *)&addr_in,&slen);

  address=estr(inet_ntoa(addr_in.sin_addr));
  port=ntohs(addr_in.sin_port);

#ifndef __MINGW32CE__
  if (rlen<0) {lwarn("receiving data, socket: "+estr(fhsock)+" errno: "+estr(errno)); return(rlen); }
#else
  if (rlen<0) {lwarn("receiving data, error: unable to get error code"); return(rlen); }
#endif
  buffer._strlen=rlen;
  buffer._str[rlen]=0x00;
  if (rlen==0){
    _doClose();
    close();
  }
  return(rlen);
}

void eudpsocket::shutdown()
{
#ifdef _WIN32
  closesocket(fhsock);
#else
  ::shutdown(fhsock,SHUT_WR);
#endif
}






esocket::esocket(): fhsock(0)
{
#ifdef _WIN32
  if (!wsastartup){
    WSADATA WSData;
    WSAStartup(MAKEWORD(2,2),&WSData);
    wsastartup=true;
  }
#endif
}

esocket::esocket(int hsock,struct sockaddr_in addr_in): fhsock(hsock), faddr_in(addr_in)
{
  faddress=inet_ntoa(faddr_in.sin_addr);
  fport=ntohs(faddr_in.sin_port);
  setCallback();
}

void esocket::setBlocking(bool value)
{
#ifndef _WIN32
  if (value){
    fcntl(fhsock, F_SETFL, O_NONBLOCK | O_ASYNC);
  }else{
    fcntl(fhsock, F_SETFL, 0x00 );
  }
#endif
}

void esocket::setCallback()
{
#ifdef _WIN32
  if (!wsastartup){
    WSADATA WSData;
    WSAStartup(MAKEWORD(2,2),&WSData);
    wsastartup=true;
  }
  u_long opt;
  opt = 1;
  ioctlsocket(fhsock,FIONBIO,&opt);
#elif defined(__APPLE__)
  int tmpo=1;
  if(0!=setsockopt(fhsock,SOL_SOCKET,SO_NOSIGPIPE,&tmpo,sizeof(tmpo))){
    lerror("setsockopt: "+estr(errno));
  }
#endif

  setBlocking(false);

#ifdef _WIN32
  wsaEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
  WSAEventSelect(fhsock,wsaEvent,FD_READ);
  getSystem()->addfunc(wsaEvent,new efunc(*this,&esocket::_doRecv));
//#elif defined(__APPLE__)
//  getSystem()->addSocket(fhsock,kCFSocketReadCallBack,new efunc(*this,&esocket::_doRecv),0x00);
//  getSystem()->addSocketReadWriteCallback(fhsock,efunc(*this,&esocket::_doRecv),evararray(),efunc(*this,&esocket::_doSend),evararray());
//  disableWriteCallback();
#else
//  getSystem()->addfunc(fhsock,new efunc(*this,&esocket::_doRecv));
  getSystem()->addSocketReadWriteCallback(fhsock,efunc(*this,&esocket::_doRecv),evararray(),efunc(*this,&esocket::_doSend),evararray());
  disableWriteCallback();
#endif
}

void esocket::enableReadCallback()
{
  getSystem()->enableSocketReadCallback(fhsock);
}

void esocket::enableWriteCallback()
{
  getSystem()->enableSocketWriteCallback(fhsock);
}

void esocket::disableReadCallback()
{
  getSystem()->disableSocketReadCallback(fhsock);
}

void esocket::disableWriteCallback()
{
  getSystem()->disableSocketWriteCallback(fhsock);
}

esocket::~esocket()
{
  close();
}

void esocket::close()
{
  if (!fhsock) return;

#ifdef _WIN32
  closesocket(fhsock);
#else
  ::close(fhsock);
#endif

#ifdef _WIN32
  getSystem()->remove(wsaEvent);
  WSACloseEvent(wsaEvent);
  wsaEvent=0x00;
//#elif defined(__APPLE__)
//  getSystem()->removeSocket(fhsock);
#else
  getSystem()->removeSocket(fhsock);
#endif
  fhsock = 0;
}

void esocket::init()
{
  close();
  fhsock = socket(PF_INET,SOCK_STREAM,0);
  lerrorifr(fhsock == -1,"unable to create socket",);
}

bool esocket::connect(const estr& address, int port)
{
//  struct sockaddr_in addr;
  struct hostent     *phe;
//  int  constat;

  init();

  fport = port;
  faddress = address; 

#ifdef _WIN32
  if (isdigit(faddress[0])){
    faddr_in.sin_addr.s_addr = inet_addr(faddress._str);
  }else{
    phe = (hostent *)gethostbyname(faddress._str);
    lwarnifr(phe==NULL,"unable to resolve hostname",(false));
//    if (phe == NULL){ esockClose(hsock); logger.error("esocklib.cpp, esockConnect: hostname lookup failed"); return(-1); }
    faddr_in.sin_addr.s_addr = *(u_long *)(phe->h_addr_list[0]);
  }
#else
  if (isdigit(faddress[0])){
    inet_aton(faddress._str,&faddr_in.sin_addr); 
  } else {
    phe = (struct hostent *)gethostbyname(faddress._str);
    lwarnifr(phe==NULL,"unable to resolve hostname",(false));

    faddr_in.sin_addr.s_addr = *(in_addr_t*)phe->h_addr_list[0];
  }
#endif

  faddr_in.sin_family = AF_INET;
  faddr_in.sin_port = htons(fport);

  linfo("Connecting to ip "+estr(inet_ntoa(faddr_in.sin_addr)));
  if (::connect(fhsock, (struct sockaddr *)&faddr_in,sizeof(faddr_in)) == -1)
    { lwarn("unable to connect to server "+faddress+" on port "+estr(fport)); return(false); }

  setCallback();

/*
#ifdef _WIN32
  u_long opt;
  opt = 1;
  ioctlsocket(fhsock,FIONBIO,&opt);
#else
  fcntl(fhsock, F_SETFL, O_NONBLOCK | O_ASYNC);
#endif

#ifdef _WIN32
//  wsaEvent = WSACreateEvent();
  wsaEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
  WSAEventSelect(fhsock,wsaEvent,FD_READ);
//  getSystem()->add(wsaEvent,esocket::socketNotify,new evar(this));
  getSystem()->addfunc(wsaEvent,new efunc(*this,&esocket::_doRecv));
#else
  getSystem()->addfunc(fhsock,new efunc(*this,&esocket::_doRecv));
#endif
//  fcntl(fhsock, F_SETOWN, getpid());
//  socket_manager.add(this);
*/

  linfo("connected to server "+faddress+" on port "+estr(fport));
  return(true);
}

void esocket::doClose(){}
void esocket::doRecv(){}
void esocket::doSend(){}

void esocket::_doClose()
{
  doClose();
  if (onClose.isSet())
    onClose.call();
}

void esocket::_doRecv()
{
  doRecv();
  if (onReceive.isSet())
    onReceive.call();
}

void esocket::_doSend()
{
  doSend();
  if (onSend.isSet())
    onSend.call();
}


int esocket::recv(estr& buffer,int maxlen)
{
  int rlen;

  buffer.clear();
  buffer.reserve(maxlen+1);

  rlen=::recv(fhsock,buffer._str,maxlen,0);
/*  
  do {
    buffer.reserve(buffer._strlen+1024+1);
    rlen=::recv(fhsock,&buffer._str[buffer._strlen],1024,0);
    if (rlen>0)
      buffer._strlen+=rlen;
  }while (rlen==1024);
  buffer._str[buffer._strlen]=0x00;
*/
  
  if (rlen<0 && errno != EWOULDBLOCK) { lwarn("receiving data, socket: "+estr(fhsock)+" errno: "+estr(errno)); return(rlen); }
  buffer._strlen=rlen;
  buffer._str[buffer._strlen]=0x00;

  if (rlen==0){
    _doClose();
    close();
  }
  return(buffer.len());
}

void esocket::shutdown()
{
#ifdef _WIN32
  closesocket(fhsock);
#else
  ::shutdown(fhsock,SHUT_WR);
#endif
}

void esocket::doIncoming() {}

void esocket::_doIncoming()
{
  doIncoming();
  linfo("got server connection notification");
  if (onIncoming.isSet())
    onIncoming.call(evararray(*this));
}

bool esocket::listen(int port,const estr& address)
{
  int    constat;

  fport = port;
  faddress = address;

//  init();
//  if (fhsock)
//    reset();
  fhsock = socket(PF_INET,SOCK_STREAM,0);
  lerrorifr(fhsock == -1,"unable to create socket",false);

  faddr_in.sin_family = AF_INET;
  faddr_in.sin_addr.s_addr = inet_addr(faddress._str);
  faddr_in.sin_port = htons(fport);
  constat = bind(fhsock, (struct sockaddr *)&faddr_in,sizeof(faddr_in));
#ifndef __MINGW32CE__
  lerrorifr(constat==-1,"unable to bind socket: "+estr(errno),false);
#else
  lerrorifr(constat==-1,"unable to bind socket: unable to get error",false);
#endif

  constat = ::listen(fhsock,5);
#ifndef __MINGW32CE__
  lerrorifr(constat==-1,"unable to put socket into listening mode"+estr(errno),false);
#else
  lerrorifr(constat==-1,"unable to put socket into listening mode",false);
#endif

#ifdef _WIN32
  u_long opt;
  opt=1;
  ioctlsocket(fhsock,FIONBIO,&opt);
#else
  fcntl(fhsock,F_SETFL,O_NONBLOCK);
#endif

#ifdef _WIN32
//  wsaEvent = WSACreateEvent();
  wsaEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
  WSAEventSelect(fhsock,wsaEvent,FD_ACCEPT);
//  getSystem()->add(wsaEvent,eserver::serverNotify,new evar(this));
  getSystem()->addfunc(wsaEvent,new efunc(*this,&esocket::_doIncoming));
//#elif defined(__APPLE__)
//  getSystem()->addSocketReadCallback(fhsock,efunc(*this,&eserver::doIncoming),evararray());
//  getSystem()->addSocket(fhsock,kCFSocketReadCallBack,new efunc(*this,&eserver::doIncoming));
#else
  getSystem()->addSocketReadCallback(fhsock,efunc(*this,&esocket::_doIncoming),evararray());
//  getSystem()->addfunc(fhsock,new efunc(*this,&eserver::doIncoming));
#endif
//  socket_manager.add(this);
  
  linfo("listening on port "+estr(fport));
  return(true);
}


#ifdef _WIN32
  #define socklen_t int
#endif

bool esocket::accept(esocket &socket)
{
  int constat;
  int addrlen;

  addrlen = sizeof(socket.faddr_in);
  constat = ::accept(fhsock, (struct sockaddr *)&(socket.faddr_in),(socklen_t*)&addrlen);
  if (constat==-1)
    return(false);

  socket.faddress=inet_ntoa(socket.faddr_in.sin_addr);
  socket.fport=ntohs(socket.faddr_in.sin_port);
  socket.fhsock = constat;
  socket.setCallback();
  return(true);
}




/*
void eserver::doAccept(esocket& socket)
{
}

void eserver::doIncoming()
{
  esocket& socket(accept());
  if (onAccept.isSet())
    onAccept(socket);
  doAccept(socket);
}

esocket& eserver::accept()
{
  esocket& socket(sockets.add(esocket()));
  esocket::accept(socket);
  return(socket);
}
*/



