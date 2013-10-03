#ifndef EHTTPHUB_H
#define EHTTPHUB_H

#include "ehttpServer.h"

class ehttpHub;

class ehttpHubConnection : public ehttpServerConnection
{
 public:
  ehttpHub& server;

  bool connected;
  estr session;  

  ehttpHubConnection(ehttpHub& server);

  void getSession();

  virtual void doClose();
  bool send(const estr& data,int status=200,const estr& mimetype="text/html");

  void newSession();

  virtual void doGetLocation(const estr& location);

  virtual void doHandleGet(const estr& location);
  virtual void doHandlePost(const estr& location);

  void sendStatus();

  void doHandleWebClientSend(const estr& location);
  void doHandleWebClientRecv(const estr& location);

  virtual void handleCommand(const estr& msg);
  void handleGlobalMsg(esvararray& msg);
  void handleChannelMsg(esvararray& msg);
  void handlePrivateMsg(esvararray& msg);
};

class ehttpHubConnectionData
{
 public:
  ehttpHub& server;
  estr username;

  estrarray sendbuffer;
  estr channel;
  estr session;
  ehttpHubConnection *socket;
  estr ip;

  ehttpHubConnectionData();
  ehttpHubConnectionData(ehttpHub& server);
  ehttpHubConnectionData(ehttpHub& server,ehttpHubConnection* socket);

  void setData(const estr& msg);
  void setChannel(const estr& chan);
  void send(const estr& data);
};

class ehttpHub : public esocket
{
 public:
  earray<ehttpHubConnection> sockets;

  estr wd;
  estrarray allowedExt;

  estrhashof<ehttpHubConnectionData> client;
  estrhashof<ebasicarray<ehttpHubConnectionData*> > channels;

  ehttpHub();

  void doIncoming();

  void sendTo(const estr& session,const estr& data);
  void sendAll(const estr& data,const estr& session);
  void sendChannel(const estr& data,const estr& channel,const estr& session);

  int countClients();

  esvararray usersInChannel(const estr& session,const estr& channel);
};



#endif

