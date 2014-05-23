#ifndef ESOCKET_H
#define ESOCKET_H

#include "eutils.h"

#ifdef _WIN32
 #include <winsock2.h>
 #include <windows.h>
#else
 #include <sys/socket.h>
 #include <sys/types.h>
 #include <netinet/in.h>
 #include <netdb.h>
#endif

#include "estr.h"
#include "earray.h"

#include "efunc.h"


class eudpsocket
{
 public:
  int                fhsock;
  struct sockaddr_in faddr_in;
  estr               faddress;
  int                fport;
#ifdef _WIN32
  WSAEVENT wsaEvent;
#endif

  efunc onSend;
  efunc onReceive;
  efunc onClose;

  eudpsocket();
  eudpsocket(int hsock,struct sockaddr_in addr_in);
  virtual ~eudpsocket();

  void setCallback();

//  bool connect(const estr& address,int port);
  bool listen(int port,const estr& address="0.0.0.0",bool reuseAddress=false);
  void close();
  void shutdown();

  void setBroadcast(bool value);
  void setReuseAddress(bool value);
  void setBlocking(bool value);

  // user defined functions
  virtual void doClose();
  virtual void doRecv();
  virtual void doSend();

  // internal functions
  void _doRecv();
  void _doClose();
  void _doSend();

  bool sendto(const estr& data,const estr& address,int port);
  int recvfrom(estr& data,estr& address,int& port);
};

class esocket
{
 public:
  int                fhsock;
  struct sockaddr_in faddr_in;
  estr               faddress;
  int                fport;
#ifdef _WIN32
  WSAEVENT wsaEvent;
#endif

  efunc onSend;
  efunc onReceive;
  efunc onClose;

  esocket();
  esocket(int hsock,struct sockaddr_in addr_in);
  virtual ~esocket();

  void init();
  void setCallback();
  void enableReadCallback();
  void enableWriteCallback();
  void disableReadCallback();
  void disableWriteCallback();

  bool connect(const estr& address,int port);
  void close();
  void shutdown();

  efunc onIncoming;
  virtual void doIncoming();

  bool listen(int port,const estr& address="0.0.0.0");
  bool accept(esocket &socket);

  void setBlocking(bool value);

  // user subclassed functions
  virtual void doClose();
  virtual void doRecv();
  virtual void doSend();

  // internal functions
  void _doIncoming();

  void _doSend();
  void _doRecv();
  void _doClose();

  int recv(estr& buffer,int maxlen=1024);

  inline int recv(char *buffer, int len,int flags=0){
    return(::recv(fhsock,buffer,len,flags));
  }

  inline int send(const estr& buffer){
    return(send(buffer._str,buffer.len()));
  }

  inline int send(char *buffer, int len, int flags=0){
    return(::send(fhsock,buffer,len,flags));
  }
};

/*
template <class K>
class eserverClient : public esocket
{
 public:
  T& server;
  eserverClient(T& server);
};

template <class K>
eserverClient<K>::eserverClient(T& _server): server(_server) {}
*/

/*
class eserver : public esocket
{
 public:
  earray<esocket> sockets;

  efunc onAccept;

  virtual void doAccept(esocket& socket);
  void doIncoming();
};
*/

#endif
