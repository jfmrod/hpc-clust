#ifndef EHTTPSERVER_H
#define EHTTPSERVER_H

#include "esocket.h"

class ehttpServerConnection : public esocket
{
 public:
  int protstate;

  estr cmd;
  estr location;
  estr request_uri;
  estr protocol;

  estrarray sendHeaders;
  estrarray requestHeaders;
  estr header;
  estr data;
  estr postdata;

  estrarray getVars;

  ehttpServerConnection();

  virtual void doRecv();

  virtual void doHandleGet(const estr& location)=0;
  virtual void doHandlePost(const estr& location)=0;

  bool send(const estr& data,int status=200,const estr& mimetype="text/html");

  void parseLocation(estr& location,estrarray& vars);

  void sendRedirect(const estr& newurl);
  void sendNotFound();
  void sendFile(const estr& filename);
};

estr makeURI(const estr& host,const estr& location,estrarray& vars);

/*
template <class T,class K>
class ehttpServer : public eserver<T,K>
{
 public:
  estr htmldir;
  void doAccept(); 
};
*/

#endif

