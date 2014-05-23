#ifndef EDCSERVER_H
#define EDCSERVER_H

#include "esocket.h"

class edcBaseClient
{
 public:
  virtual void init();
  virtual void final();

  virtual bool sendMsg(int code,const estr& msg)=0;
};

class edcBaseServer;
class edcBaseServerNode
{
 public:
  edcBaseServer& server;
  bool isBusy;
  bool isChoked;
  evar result;

  edcBaseServerNode(edcBaseServer& server);
  virtual bool sendMsg(int code,const estr& msg)=0;

  virtual void choke()=0;
  virtual void unchoke()=0;
  virtual void doProcess()=0;
  virtual void call(const estr& cmd,const evararray& arr)=0; 
};

class edcBaseServer
{
 public:
  earray<efunc> callbacks;

  efunc onAllReady;

  virtual void init();
  virtual void final();

  virtual int nodeCount()=0;
  virtual edcBaseServerNode& getClient(int i)=0;
};

class edcclient : public esocket, public edcBaseClient
{
 public:
  estr recvbuffer;
  estr sendbuffer;
  int outpipe;

  void doSend();
  void doRecv();
  void doClose();

  virtual bool sendMsg(int code,const estr& msg);

  void doHandleCall(const estr& data);
  void doHandleEval(const estr& data);
  void sendResult(const evar& var);
  void sendOutput();
};

class edcserver;

class edcserverClient : public esocket,public edcBaseServerNode
{
 public:
//  edcserver& server;
  estr recvbuffer;
  estr sendbuffer;

//  bool isChoked;
//  bool isBusy;

  edcserverClient(edcserver& server);

  void choke();
  void unchoke();

  bool sendMsg(int code,const estr& msg);

  void doSend();
  void call(const estr& cmd,const evararray& arr); 
  void eval(const estr& cmd); 

  void doProcess();
  void doRecv();
};

class edcserver : public esocket, public edcBaseServer
{
 public:
  bool showResult;

  edcserver();

  earray<edcserverClient> sockets;

  void doIncoming();

  void doAllReady();
  void doReady(edcserverClient& dclient);
  virtual edcBaseServerNode& getClient(int i);

  void doHandleOutput(edcserverClient& client,const estr& data);
  void doHandleResult(edcserverClient& client,const estr& data);

  virtual int nodeCount();
};

void registerServer();
void startServer(int port);
void startClient(const estr& hostname,int port);


#ifdef EUTILS_HAVE_MPILIB
#include <mpi.h>

class edcmpiServerNode;

void empiParseArgs(int& argvc,char ***argv);

class edcmpi: public edcBaseServer, public edcBaseClient
{
 public:
  int numprocs;
  int rank;
  int namelen;
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  
  efunc onStartServer;
  efunc onStartClient;

  void init();
  void final();

  void runServer();
  void runClient();

  bool showResult;

  edcmpi();

  earray<edcmpiServerNode> nodes;
//  earray<efunc> callbacks;

  void doIncoming();

  void doAllReady();
  void doReady(edcmpiServerNode& node);
  edcBaseServerNode& getClient(int i);

  void doHandleOutput(edcmpiServerNode& node,const estr& data);
  void doHandleResult(edcmpiServerNode& node,const estr& data);

  estr recvbuffer;
  estr sendbuffer;
  int outpipe;

  void doNodeSend();
  void doNodeRecv();
  void doNodeClose();

  bool sendMsg(int code,const estr& msg);

  int nodeRecv(estr& data);
  int nodeSend(const estr& data);

  void doNodeHandleCall(const estr& data);
  void doNodeHandleEval(const estr& data);
  void nodeSendResult(const evar& var);
  void nodeSendOutput();

  virtual int nodeCount();
};

class edcmpiServerNode: public edcBaseServerNode
{
 public:
//  edcmpi& server;
  estr recvbuffer;
  estr sendbuffer;

  int rank;

//  bool isChoked;
//  bool isBusy;

  edcmpiServerNode(edcmpi& server,int rank);

  bool probe();
  int recv(estr& data);
  int send(const estr& data);

  void choke();
  void unchoke();

  bool sendMsg(int code,const estr& msg);

  void doSend();
  void call(const estr& cmd,const evararray& arr); 
  void eval(const estr& cmd); 

  void doProcess();
  void doRecv();
};

#endif /* EUTILS_HAVE_MPILIB */



#endif /* EDCSERVER_H */

