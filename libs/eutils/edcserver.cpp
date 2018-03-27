#include "edcserver.h"
#include "esystem.h"
#include "eparser.h"
#include "eparserinterpreter.h"

#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

edcBaseServerNode::edcBaseServerNode(edcBaseServer& _server): server(_server) {}

void edcBaseClient::init() {}
void edcBaseClient::final() {}

void edcBaseServer::init() {}
void edcBaseServer::final() {}


void edcclient::doRecv()
{
  estr tmpdata;
  if (0==recv(tmpdata)){ lwarn("client disconnected"); return; }

//  cerr << "# received: " << tmpdata.len() << " bytes" << endl;
  recvbuffer+=tmpdata;

  unsigned int msgcode,datalen;
  int i;
  do {
    i=0;
    i=unserialint(msgcode,recvbuffer,i);
    if (i==-1) return;
    i=unserialint(datalen,recvbuffer,i);
    if (i==-1) return;
//    cerr << "# received msg. code: " << msgcode << " size: "<< datalen << endl;
    if (recvbuffer.len()<i+datalen) { recvbuffer.reserve(i+datalen); return; }
  
    switch (msgcode){
      case 0: 
        doHandleEval(recvbuffer.substr(i,datalen));
       break;
      case 1: 
        doHandleCall(recvbuffer.substr(i,datalen));
       break;
      default:
        lerror("unknown command: "+estr(msgcode));
    }
    recvbuffer.del(0,i+datalen);
  } while(recvbuffer.len()>0);
}

void edcclient::doSend()
{
  if (!sendbuffer.len()) return;

  int len;
  len=send(sendbuffer);
  if (len==-1) {
    switch (errno){
      case ENOBUFS: lwarn("Socket outgoing buffer is full"); break;
      case EAGAIN: lwarn("Socket would block"); break;
      case ECONNRESET: lwarn("Connection reset by peer"); break;
     default:
       lerror("Socket error: "+estr(errno));
    }
    enableWriteCallback();
    return;
  }
  sendbuffer.del(0,len);

  if (!sendbuffer.len())
    disableWriteCallback();
}

void edcclient::doClose()
{
  exit(0);
}

void edcclient::doHandleCall(const estr& evaldata)
{
  estr cmd;
  int i=0;
  i=cmd.unserial(evaldata,i);
  ldieif(i==-1,"malformed call message");

  evararray args;
  i=args.unserial(evaldata,i);
  ldieif(i==-1,"malformed call message");
  
  evar var;
  evar varfunc(epinterpret(cmd));
  if (varfunc.getTypeid()==typeid(efunc)){
    efunc func(varfunc.get<efunc>());
    var.set(func(args));
  }else{
    lerror("call is not of type efunc");
  }
//  if (getParser()->funcs.exists(cmd))
//    var.set(getParser()->funcs[cmd].at(0).call(arr));

  sendResult(var);
}

void edcclient::doHandleEval(const estr& evaldata)
{
  estr cmd;
  int i=0;
  i=cmd.unserial(evaldata,i);
  ldieif(i==-1,"malformed eval message");

  evar var(epinterpret(cmd));
  sendResult(var);
}


bool edcclient::sendMsg(int code,const estr& msg)
{
  estr sendstr;
  serialint(code,sendstr);
  serialint(msg.len(),sendstr);
  sendstr+=msg;
  sendbuffer+=sendstr;
  doSend();
  return(!sendbuffer.len());
}

void edcclient::sendResult(const evar& var)
{
  estr msg;
  var.serial(msg);
  sendMsg(1,msg);
}

void edcclient::sendOutput()
{
  int len;
  estr output;
  output.reserve(1024);
  do{
    len=read(outpipe,&output._str[output.len()],1024);
    output._strlen+=len;
    output.reserve(output.len()+1024);
  } while (len==1024);
  ldieif(len==-1,"error reading from outpipe");

  estr msg;
  output.serial(msg);
  sendMsg(0,msg);
}



edcserverClient::edcserverClient(edcserver& _server): edcBaseServerNode(_server)
{
  isChoked=false;
  isBusy=false;
  recvbuffer.reserve(26000);
}

void edcserverClient::doSend()
{
  if (!sendbuffer.len()) return;

  int len;
  len=send(sendbuffer);
  if (len==-1) {
    switch (errno){
      case ENOBUFS: lwarn("Socket outgoing buffer is full"); break;
      case EAGAIN: lwarn("Socket would block"); break;
      case ECONNRESET: lwarn("Connection reset by peer"); break;
     default:
       lerror("Socket error: "+estr(errno));
    }
    enableWriteCallback();
    return;
  }

  sendbuffer.del(0,len);
  if (!sendbuffer.len())
    disableWriteCallback();
}



bool edcserverClient::sendMsg(int code,const estr& msg)
{
  estr sendstr;
  serialint(code,sendstr);
  serialint(msg.len(),sendstr);
  sendstr+=msg;
  sendbuffer+=sendstr;
  doSend();
  return(!sendbuffer.len());
}

void edcserverClient::call(const estr& cmd,const evararray& arr)
{
  if (isBusy) { lwarn("dclient is busy"); return; }
  isBusy=true;
  estr msg;
  cmd.serial(msg);
  arr.serial(msg);
  sendMsg(1,msg);
}

void edcserverClient::eval(const estr& cmd)
{
  if (isBusy) { lwarn("dclient is busy"); return; }
  isBusy=true;
  estr msg;
  cmd.serial(msg);
  sendMsg(0,msg);
}

void edcserverClient::choke()
{
  isChoked=true;
  disableReadCallback();
}

void edcserverClient::unchoke()
{
  isChoked=false;
  enableReadCallback();
//  doProcess();
}

void edcserverClient::doProcess()
{
  if (!recvbuffer.len()) return;

/*
  int k=-1;
  for (k=0; k<server->sockets.size(); ++k)
    if (&server->getClient(k)==this)
      { break; }
*/

  unsigned int msgcode,datalen;
  int i,msgpos;
  do {
    i=0;
    i=unserialint(msgcode,recvbuffer,i);
    if (i==-1) return;
    i=unserialint(datalen,recvbuffer,i);
    if (i==-1) return;
//    cout << "# got msg: " << k << " msgcode: " << msgcode << " msglen: " << datalen << " recvbuffer: " << recvbuffer.len() << endl;
    
    if (recvbuffer.len()<i+datalen) { recvbuffer.reserve(i+datalen); return; }
    ldieif(msgcode>=server.callbacks.size(),"unregistered callback: "+estr(msgcode)+" callbacks.size: "+estr(server.callbacks.size()));
    server.callbacks.at(msgcode).call(evararray(*this,recvbuffer.substr(i,datalen)));
    recvbuffer.del(0,i+datalen);
    
//    i=server->callbacks.at(v).call(evararray(*this,recvbuffer,i)).get<int>();
//    ldieif(i==-1,"not supposed to happen: "+estr(v)+" mlen: "+estr(mlen));
//    if (i==-1) return;
/*
    switch(v){
      case 50: 
        i=doHandleOutput(data,i);
        ldieif(i==-1,"not supposed to happen: "+estr(v)+" mlen: "+estr(mlen));
        if (i==-1) return;
       break;
      case 51:
        i=doHandleResult(data,i);
        ldieif(i==-1,"not supposed to happen: "+estr(v)+" mlen: "+estr(mlen));
        if (i==-1) return;
       break;
      default:
       lerror("unknown code: "+estr(v)+" data.len: "+data.len()+" i: "+i);
       exit(0);
    }
*/
//    recvbuffer.del(0,i);
  } while (recvbuffer.len() && !isChoked);
}

void edcserverClient::doRecv()
{
  lassert(isChoked);

  estr tmpdata;
  if (0==recv(tmpdata,13000)){ lwarn("client disconnected"); return; }
  recvbuffer+=tmpdata;

  doProcess();
}




int edcserver::nodeCount()
{
  return(sockets.size());
}


void edcserver::doHandleOutput(edcserverClient& client,const estr& msgdata)
{
  estr msg;
  int i;
  i=msg.unserial(msgdata,0);
  if (i==-1) return;

  if (showResult)
    cout << "[remote] " << msg << endl;
}

void edcserver::doHandleResult(edcserverClient& client,const estr& resdata)
{
  client.result.clear();
  int i;
  i=client.result.unserial(resdata,0);
  if (i==-1) return;

  if (showResult)
    cout << "[remote] result: " << client.result << endl;
  client.isBusy=false;
  doReady(client);
}


edcserver::edcserver(): showResult(false)
{
  callbacks.add(efunc(*this,&edcserver::doHandleOutput));
  callbacks.add(efunc(*this,&edcserver::doHandleResult));
}

edcBaseServerNode& edcserver::getClient(int i)
{
  return(dynamic_cast<edcBaseServerNode&>(sockets.at(i)));
}

void edcserver::doIncoming()
{
  edcserverClient *client=new edcserverClient(*this);
  sockets.addref(client);
  accept(*client);
  cout << "client connected: "<< client->faddress << ":" <<client->fport << endl;
}

/*
esocket& edcserver::accept()
{
  edcserverClient *socket=new edcserverClient(*this);
  sockets.addref(socket); 
  esocket::accept(*socket);
  return(*socket);
}
*/

void edcserver::doAllReady()
{
}

void edcserver::doReady(edcserverClient& dclient)
{
  int i;
  bool allReady=true;
  for (i=0; i<sockets.size(); ++i)
    if (getClient(i).isBusy) allReady=false;
  if (allReady)
    { doAllReady(); if (onAllReady.isSet()) onAllReady.call(evararray(*this)); }
}


void registerServer()
{
  epregisterClassMethod(edcserverClient,call);
  epregisterClassMethod(edcserverClient,eval);
  epregisterClassProperty(edcserverClient,isBusy);
  epregisterClassProperty(edcserverClient,result);

  epregisterClass(edcserver);
//  epregisterClassInheritance(edcserver,eserver<edcserver,edcserverClient>);
  epregisterClassMethodA(edcserver,getClient,"operator[]");
  epregisterClassMethod(edcserver,getClient);
  epregisterClassProperty(edcserver,showResult);
}

void startServer(int port)
{
  edcserver* server=new edcserver;
  server->listen(port);

  registerServer();
//  epregisterClass(edcserverClient);
  epregister(server);
//  epruninterpret();
}

#include <unistd.h>

void startClient(const estr& host,int port)
{
  edcclient client;

  int pipefd[2];
  pipe(pipefd);
  dup2(pipefd[1],1);

  client.outpipe=pipefd[0];
  getSystem()->addReadCallback(client.outpipe,efunc(client,&edcclient::sendOutput),evararray());

  client.connect(host,port);

  getSystem()->run();
}


#ifdef EUTILS_HAVE_MPILIB

/*
edcmpi::edcmpi()
{

}
*/

int edcmpi::nodeCount()
{
  return(nodes.size());
}


void empiParseArgs(int& argvc,char ***argv){
//  int numprocs, rank, namelen;
//  char processor_name[MPI_MAX_PROCESSOR_NAME];
  MPI_Init(&argvc,argv);
  eparseArgs(argvc,*argv);
}

void edcmpi::init()
{
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Get_processor_name(processor_name, &namelen);
  if (rank==0){
    int i;
    for (i=1; i<numprocs; ++i)
      nodes.add(edcmpiServerNode(*this,i));
    onStartServer.call(evararray(*this));
    runServer();
  }else{ 
    onStartClient.call(evararray(*this));
    runClient();
  }
}

void edcmpi::final()
{
  MPI_Finalize();
}

void edcmpi::doHandleOutput(edcmpiServerNode& client,const estr& msgdata)
{
  estr msg;
  int i;
  i=msg.unserial(msgdata,0);
  if (i==-1) return;

  if (showResult)
    cout << "[remote] " << msg << endl;
}

void edcmpi::doHandleResult(edcmpiServerNode& client,const estr& resdata)
{
  client.result.clear();
  int i;
  i=client.result.unserial(resdata,0);
  if (i==-1) return;

  if (showResult)
    cout << "[remote] result: " << client.result << endl;
  client.isBusy=false;
  doReady(client);
}


edcmpi::edcmpi(): showResult(false)
{
  callbacks.add(efunc(*this,&edcmpi::doHandleOutput));
  callbacks.add(efunc(*this,&edcmpi::doHandleResult));
}

edcBaseServerNode& edcmpi::getClient(int i)
{
  return(nodes.at(i));
}

void edcmpi::doAllReady()
{
}

void edcmpi::doReady(edcmpiServerNode& dclient)
{
  int i;
  bool allReady=true;
  for (i=0; i<nodes.size(); ++i)
    if (getClient(i).isBusy) allReady=false;
  if (allReady)
    { doAllReady(); if (onAllReady.isSet()) onAllReady.call(evararray(*this)); }
}

void edcmpi::runServer()
{
  MPI_Status stat;
  int i;
  bool gotdata=true;
  while (1){
    if (!gotdata){
//      cout << rank << ": waiting for message" << endl;
      MPI_Probe(MPI_ANY_SOURCE,0,MPI_COMM_WORLD,&stat);
//      cout << rank << ": got message from: "<< stat.MPI_SOURCE << " tag: " << stat.MPI_TAG << endl;
    }

    gotdata=false;
    for (i=0; i<nodes.size(); ++i){
      if (!nodes[i].isChoked && nodes[i].probe()){
        gotdata=true;
        nodes[i].doRecv();
      }
    }
  }
}

void edcmpi::runClient()
{
  MPI_Status stat;
  while (1){
//    cout << rank << ": waiting for message" << endl;
    MPI_Probe(0,0,MPI_COMM_WORLD,&stat);
//    cout << rank << ": got message" << endl;
    doNodeRecv();
  }
}








edcmpiServerNode::edcmpiServerNode(edcmpi& _server,int _rank): edcBaseServerNode(_server),rank(_rank)
{
  isBusy=false;
  isChoked=false;
  recvbuffer.reserve(26000);
}

void edcmpiServerNode::doSend()
{
  if (!sendbuffer.len()) return;

  int res;
  res=send(sendbuffer);
  if (res!=MPI_SUCCESS) {
    lerror("MPI_Send error: "+estr(res));
    return;
  }

  sendbuffer.clear();
//  sendbuffer.del(0,len);
//  if (!sendbuffer.len())
//    disableWriteCallback();
}

bool edcmpiServerNode::sendMsg(int code,const estr& msg)
{
  estr sendstr;
  serialint(code,sendstr);
  serialint(msg.len(),sendstr);
  sendstr+=msg;
  sendbuffer+=sendstr;
  doSend();
  return(!sendbuffer.len());
}

void edcmpiServerNode::call(const estr& cmd,const evararray& arr)
{
  if (isBusy) { lwarn("dclient is busy"); return; }
  isBusy=true;
  estr msg;
  cmd.serial(msg);
  arr.serial(msg);
  sendMsg(1,msg);
}

void edcmpiServerNode::eval(const estr& cmd)
{
  if (isBusy) { lwarn("dclient is busy"); return; }
  isBusy=true;
  estr msg;
  cmd.serial(msg);
  sendMsg(0,msg);
}

void edcmpiServerNode::choke()
{
  isChoked=true;
}

void edcmpiServerNode::unchoke()
{
  isChoked=false;
//  doProcess();
}

void edcmpiServerNode::doProcess()
{
  if (!recvbuffer.len()) return;

/*
  int k=-1;
  for (k=0; k<server->sockets.size(); ++k)
    if (&server->getClient(k)==this)
      { break; }
*/

  unsigned int msgcode,datalen;
  int i,msgpos;
  do {
    i=0;
    i=unserialint(msgcode,recvbuffer,i);
    if (i==-1) return;
    i=unserialint(datalen,recvbuffer,i);
    if (i==-1) return;
//    cout << "# got msg: " << k << " msgcode: " << msgcode << " msglen: " << datalen << " recvbuffer: " << recvbuffer.len() << endl;
    
    if (recvbuffer.len()<i+datalen) { recvbuffer.reserve(i+datalen); return; }
    ldieif(msgcode>=server.callbacks.size(),"unregistered callback: "+estr(msgcode)+" callbacks.size: "+estr(server.callbacks.size()));
    server.callbacks.at(msgcode).call(evararray(*this,recvbuffer.substr(i,datalen)));
    recvbuffer.del(0,i+datalen);
    
//    i=server->callbacks.at(v).call(evararray(*this,recvbuffer,i)).get<int>();
//    ldieif(i==-1,"not supposed to happen: "+estr(v)+" mlen: "+estr(mlen));
//    if (i==-1) return;
/*
    switch(v){
      case 50: 
        i=doHandleOutput(data,i);
        ldieif(i==-1,"not supposed to happen: "+estr(v)+" mlen: "+estr(mlen));
        if (i==-1) return;
       break;
      case 51:
        i=doHandleResult(data,i);
        ldieif(i==-1,"not supposed to happen: "+estr(v)+" mlen: "+estr(mlen));
        if (i==-1) return;
       break;
      default:
       lerror("unknown code: "+estr(v)+" data.len: "+data.len()+" i: "+i);
       exit(0);
    }
*/
//    recvbuffer.del(0,i);
  } while (recvbuffer.len() && !isChoked);
}

bool edcmpiServerNode::probe()
{
  MPI_Status stat;
  int flag;
  MPI_Iprobe(rank,0,MPI_COMM_WORLD,&flag,&stat);
  return(flag);
}

int edcmpiServerNode::send(const estr& data)
{
  return(MPI_Ssend(data._str,data.len(),MPI_BYTE,rank,0,MPI_COMM_WORLD));
}

int edcmpiServerNode::recv(estr& data)
{
  MPI_Status stat;
  int recvlen;
  MPI_Probe(rank,0,MPI_COMM_WORLD,&stat);
  MPI_Get_count(&stat,MPI_BYTE,&recvlen);
  data.reserve(recvlen);
  MPI_Recv(data._str,recvlen,MPI_BYTE,rank,0,MPI_COMM_WORLD,&stat);
  MPI_Get_count(&stat,MPI_BYTE,&recvlen);
  data._strlen=recvlen;
  return(recvlen);
}

void edcmpiServerNode::doRecv()
{
  lassert(isChoked);

  estr tmpdata;
  if (0==recv(tmpdata)){ lwarn("client disconnected"); return; }
  recvbuffer+=tmpdata;

  doProcess();
}



int edcmpi::nodeSend(const estr& data)
{
  return(MPI_Ssend(data._str,data.len(),MPI_BYTE,0,0,MPI_COMM_WORLD));
}

int edcmpi::nodeRecv(estr& data)
{
  MPI_Status stat;
  int recvlen;
  MPI_Probe(0,0,MPI_COMM_WORLD,&stat);
  MPI_Get_count(&stat,MPI_BYTE,&recvlen);
  data.reserve(recvlen);
  MPI_Recv(data._str,recvlen,MPI_BYTE,0,0,MPI_COMM_WORLD,&stat);
  MPI_Get_count(&stat,MPI_BYTE,&recvlen);
  data._strlen=recvlen;
  return(recvlen);
}

void edcmpi::doNodeRecv()
{
  estr tmpdata;
  if (0==nodeRecv(tmpdata)){ lwarn("client disconnected"); return; }

//  cerr << "# received: " << tmpdata.len() << " bytes" << endl;
  recvbuffer+=tmpdata;

  unsigned int msgcode,datalen;
  int i;
  do {
    i=0;
    i=unserialint(msgcode,recvbuffer,i);
    if (i==-1) return;
    i=unserialint(datalen,recvbuffer,i);
    if (i==-1) return;
//    cerr << "# received msg. code: " << msgcode << " size: "<< datalen << endl;
    if (recvbuffer.len()<i+datalen) { recvbuffer.reserve(i+datalen); return; }
  
    switch (msgcode){
      case 0: 
        doNodeHandleEval(recvbuffer.substr(i,datalen));
       break;
      case 1: 
        doNodeHandleCall(recvbuffer.substr(i,datalen));
       break;
      default:
        lerror("unknown command: "+estr(msgcode));
    }
    recvbuffer.del(0,i+datalen);
  } while(recvbuffer.len()>0);
}

void edcmpi::doNodeSend()
{
  if (!sendbuffer.len()) return;

  int res;
  res=nodeSend(sendbuffer);
  if (res==-1) {
    lerror("MPI_Send error: "+estr(res));
    return;
  }

  sendbuffer.clear();
/*
  sendbuffer.del(0,len);
  if (!sendbuffer.len())
    disableWriteCallback();
*/
}

void edcmpi::doNodeClose()
{
  exit(0);
}

void edcmpi::doNodeHandleCall(const estr& evaldata)
{
  estr cmd;
  int i=0;
  i=cmd.unserial(evaldata,i);
  ldieif(i==-1,"malformed call message");

  cout << "# got call to function: " << cmd << endl;

  evararray args;
  i=args.unserial(evaldata,i);
  ldieif(i==-1,"malformed call message");
  cout << "# args.count: " << args.size() << endl;

  for (i=0; i<args.size(); ++i)
    cout << "#   " << i << ": "<< args[i].getClass() << endl;
  
  evar var;
  evar varfunc(epinterpret(cmd));
  if (varfunc.getTypeid()==typeid(efunc)){
    efunc func(varfunc.get<efunc>());
    var.set(func(args));
  }else{
    lerror("call is not of type efunc");
  }
//  if (getParser()->funcs.exists(cmd))
//    var.set(getParser()->funcs[cmd].at(0).call(arr));

  nodeSendResult(var);
}

void edcmpi::doNodeHandleEval(const estr& evaldata)
{
  estr cmd;
  int i=0;
  i=cmd.unserial(evaldata,i);
  ldieif(i==-1,"malformed eval message");

  evar var(epinterpret(cmd));
  nodeSendResult(var);
}


bool edcmpi::sendMsg(int code,const estr& msg)
{
  estr sendstr;
  serialint(code,sendstr);
  serialint(msg.len(),sendstr);
  sendstr+=msg;
  sendbuffer+=sendstr;
  doNodeSend();
  return(!sendbuffer.len());
}

void edcmpi::nodeSendResult(const evar& var)
{
  estr msg;
  var.serial(msg);
  sendMsg(1,msg);
}

void edcmpi::nodeSendOutput()
{
  int len;
  estr output;
  output.reserve(1024);
  do{
    len=read(outpipe,&output._str[output.len()],1024);
    output._strlen+=len;
    output.reserve(output.len()+1024);
  } while (len==1024);
  ldieif(len==-1,"error reading from outpipe");

  estr msg;
  output.serial(msg);
  sendMsg(0,msg);
}





#endif


