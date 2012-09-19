#include "ehttpHub.h"

#include "ernd.h"
#include "efile.h"
#include "ecgi.h"
#include "ehtml.h"
#include "ehttp.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/tcp.h>

ehttpHubConnectionData::ehttpHubConnectionData(): server(*(ehttpHub*)0x00),socket(0x00) {}
ehttpHubConnectionData::ehttpHubConnectionData(ehttpHub& _server): server(_server),socket(0x00) {}
ehttpHubConnectionData::ehttpHubConnectionData(ehttpHub& _server,ehttpHubConnection* _socket): server(_server),socket(_socket),session(_socket->session),ip(_socket->faddress) {}

void ehttpHubConnectionData::send(const estr& data)
{
  if (socket==0x00 || !socket->connected || socket->protstate!=3 || !socket->send(data,200,"text/plain"))
    sendbuffer.add(data);
}

void ehttpHubConnectionData::setChannel(const estr& _channel)
{
  if (_channel==channel) return;
  channel=_channel;

  esvararray msg;
  msg.add("type",new estr("join"));
  msg.add("uid",session);
  msg.add("user",username);
  msg.add("chan",channel);
  server.sendChannel(json2_serialize(msg),channel,session);
//  server.sendChannel("0+"+session+"+join+"+username,channel,session);
}

void ehttpHubConnectionData::setData(const estr& msg)
{

}



int ehttpHub::countClients(){
  int i,count;
  count=0;
  for (i=0; i<client.size(); ++i)
    if (client.values(i).socket!=0x00) ++count;
  return(count);
}


ehttpHubConnection::ehttpHubConnection(ehttpHub& _server): connected(false), server(_server)
{
}

bool ehttpHubConnection::send(const estr& data,int status,const estr& mimetype)
{
  if (connected || cmd=="POST"){
    sendHeaders.clear();
    return(ehttpServerConnection::send(data,status,"text/plain"));
  }
  return(ehttpServerConnection::send(data,status,mimetype));
}

void ehttpHubConnection::doClose()
{
  if (server.client.exists(session) && server.client[session].socket==this){
    server.client[session].socket=0x00;
    if (connected && protstate==3){
      linfo("client disconnected");
//      sendAll(session+"+"+"un+"+client[session].username,session);
//      clients_username[session]="";
    }
  }
  linfo("disconnected: clients:"+estr(server.countClients())+" sid: "+session+"  cmd: "+cmd+"  loc: "+location);
}

void ehttpHubConnection::doHandleWebClientRecv(const estr& location)
{
  int val=1;
  if (setsockopt(fhsock,IPPROTO_TCP,TCP_NODELAY,&val,sizeof(val))!=0) lerror("setting setsockopt");

//  if (getVars.findkey("channel")!=-1)
//    client[session].channel=getVars["channel"];

/*
  if (!connected && !client.exists(session)){
//    announceClient(session);
    connected=true;
    client.add(session,this);
    client[session].session=session;
    cout << " !) connected: clients:"<<countClients()<<"  conn: "<<connIndex<<"  conns: "<<connCount<<"  sid: "<<session<<"  cmd: "<<cmd<<"  loc: "<<location<< " channel: "<<client[session].channel<<endl;
  }else if (!connected && client.exists(session)){
*/

//  sendChannel(session+"+nc",client[session].channel,session);
  if (server.client[session].socket!=0x00 && server.client[session].socket!=this) { server.client[session].socket->close(); server.client[session].socket->doClose(); }
  connected=true;
  server.client[session].socket=this;
  server.client[session].session=session;
  linfo(" !) reconnected: clients:"+estr(server.countClients())+" sid: "+session+"  cmd: "+cmd+"  loc: "+location+" channel: "+server.client[session].channel);
//  }
    
  if (server.client[session].sendbuffer.size()>0){
    int i;
    estr tmpstr="{\"length\":"+estr(server.client[session].sendbuffer.size())+",";
    for (i=0; i<server.client[session].sendbuffer.size(); ++i)
      tmpstr+="\""+estr(i)+"\":"+server.client[session].sendbuffer[i]+",";
    tmpstr.del(-1);
    tmpstr+="}";
    send(tmpstr,200,"text/plain");
//    send(server.client[session].sendbuffer.implode(";"),200,"text/plain");
    server.client[session].sendbuffer.clear();
  }
}


void ehttpHubConnection::sendStatus()
{
  ldieif(session.length()==0,"session is null!!");

  server.client[session].setChannel(getVars["channel"]);

  // send client information
//  send("hello",200,"text/plain");

  esvararray arr;

  arr.add("users",new esvararray(server.usersInChannel(session,server.client[session].channel)));

  send(json2_serialize(arr));

//  send(session+"+"+cstate+"+"+client[session].clicks+"+"+client[session].totalPayoff+"+"+client[session].infoChoice+"+"+COST_INFO);
}

void ehttpHubConnection::doGetLocation(const estr& location)
{
  if (location == "/favicon.ico" || server.allowedExt.find(efile(location).extension())!=-1)
    { sendFile(server.wd+urldecode(location)); return; }
  else if (location == "/" || location == "/index.html")
    { sendFile(server.wd+"/index.html"); return; }
  sendNotFound();
}

void ehttpHubConnection::newSession()
{
  session=(int)(rnd.uniform()*1000000);
  server.client.add(session,ehttpHubConnectionData(server,this));
    
  sendHeaders["Set-Cookie"]="sid="+session+"; expires=Thu, 28-Feb-2012 10:46:16 GMT; path=/";
  estrarray tmpvars = getVars;
  tmpvars["sid"]=session;
  sendRedirect(makeURI(requestHeaders["Host"],location,tmpvars));
  linfo(" !) connected: clients:"+estr(server.countClients())+" sid: "+session+"  cmd: "+cmd+"  loc: "+location);
}

void ehttpHubConnection::doHandleGet(const estr& location)
{
  if (!location.len())
    sendNotFound();

  getSession();

  if (location!="/" && efile(location).extension()!="html" && efile(location).extension()!="php")
    { doGetLocation(location); return; }
  
//  if (location == "/favicon.ico" || server.allowedExt.find(efile(location).extension())!=-1)
//    { sendFile(server.wd+urldecode(location)); return; }

  if (session==""){
    if (location == "/server.php" || location == "/status.php")
      { lerror("got connection request for: "+location+" without session"); sendNotFound(); return; }

    lerrorif(location!="/","creating new session in location: "+location);

    newSession();
    return;
  }

  if (location == "/status.php")
    sendStatus();
  else if (location == "/server.php")
    doHandleWebClientRecv(location);
  else {
    server.client[session].setChannel(location);
    doGetLocation(location);
// sendFile(server.wd+"/index.html"); }
  }
}

void ehttpHubConnection::handleCommand(const estr& msg)
{
  lddieif(msg.len()==0,"empty command!");

  evar var;
  json2_unserialize(var,msg);

  lerrorifr(var.getTypeid()!=typeid(esvararray),"command from client not json object: "+msg,);

  esvararray& arr(var.get<esvararray>());

  lerrorifr(arr.findkey("type")==-1 || arr["type"].getTypeid()!=typeid(estr),"command from client does not have type or it is not a string: "+msg,);

  estr& type(arr["type"].get<estr>());

  if (type=="msg")
    handleGlobalMsg(arr);
  else if (type=="cmsg")
    handleChannelMsg(arr);
  else if (type=="pmsg")
    handlePrivateMsg(arr);
//  else if (type=="set")
//    server.client[session].setData(arr);
  else
    lerror("unknwown message type: "+type+" in message: "+msg);

/*
  lddieif(msg.length()==0,"empty command!");

  switch (msg[0]){
    case '0': break;
    case '1': handleGlobalMsg(msg); return;
    case '2': handleChannelMsg(msg); return;
    case '3': handlePrivateMsg(msg); return;
   default:
    lerror("unknown msg code: "+msg);
  }

  lerrorifr(msg.length()<=2,"command message too short",);

  switch (msg[2]){
    case '0': server.client[session].setData(msg); return;
   default:
    lerror("unknown command code: "+msg);
  }
*/
}



void ehttpHubConnection::handleGlobalMsg(esvararray& msg)
{
//  lerrorifr(msg.findkey("msg")==-1 || msg["msg"].getTypeid()!=typeid(estr),"Global message sent but no \"msg\" field or it is not a string",);

  msg.add("uid",session);
  server.sendAll(json2_serialize(msg),session);

//  server.sendAll("1+"+session+"+"+&msg._str[2],session);
//  chatGlobal.add(client[session].username+" | "+msg);
}

void ehttpHubConnection::handleChannelMsg(esvararray& msg)
{
//  lerrorifr(msg.findkey("msg")==-1 || msg["msg"].getTypeid()!=typeid(estr),"Channel message sent but no \"msg\" field or it is not a string",);

  msg.add("uid",session);
  server.sendChannel(json2_serialize(msg),server.client[session].channel,session);

//  server.sendChannel("2+"+session+"+"+&msg._str[2],server.client[session].channel,session);

//  estr sendchannel;
//  if (getVars.findkey("channel")!=-1)
//    sendchannel=getVars["channel"];
//  sendChannel("msg+0+&lt;"+client[session].username+"&gt; "+msg,channel,session);
}

void ehttpHubConnection::handlePrivateMsg(esvararray& msg)
{
//  lerrorifr(msg.findkey("msg")==-1 || msg["msg"].getTypeid()!=typeid(estr),"Private message sent but no \"msg\" field or it is not a string",);
  lerrorifr(msg.findkey("to")==-1 || msg["to"].getTypeid()!=typeid(int),"Private message sent but no \"to\" field or it is not an int",);
  estr toid(msg["to"].get<int>());

  msg.add("uid",session);
  lerrorifr(!server.client.exists(toid),"Private message sent but no user with id: "+estr(toid),);

  server.client[toid].send(json2_serialize(msg));
}



void ehttpHubConnection::doHandlePost(const estr& location)
{
  getSession();

  if (connected){
    if (server.client.exists(session) && server.client[session].socket==this){
      server.client[session].socket=0x00;
      connected=false;
    }
  }
  send("h");

  estr sendchannel;
  if (getVars.findkey("channel")!=-1)
    sendchannel=getVars["channel"];

  estrarray cmds=postdata.explode(";");
  int i;

  for (i=0;i<cmds.size();++i){
    if (cmds[i].len()==0) continue;

    handleCommand(cmds[i]);
  }
//  sendChannel(tmpstr,sendchannel,session);
}

void ehttpHubConnection::getSession()
{
  if (getVars.findkey("sid")!=-1){
    session=getVars["sid"];
    if (!server.client.exists(session)){
      session="";
    }
  }else if (requestHeaders.findkey("Cookie")!=-1){
    estrarray tmparr;
    tmparr.split(requestHeaders["Cookie"].trim(),"; ","=");
    if (tmparr.findkey("sid")!=-1){
      session=tmparr["sid"];
      if (!server.client.exists(session)){
        session="";
      }
    }
  }
}





ehttpHub::ehttpHub(): wd("html"), allowedExt("css,png,gif,jpg,html,js,swf") {}

void ehttpHub::sendTo(const estr& session,const estr& data)
{
  if (client.exists(session))
    client[session].send(data);
}

void ehttpHub::doIncoming()
{
  ehttpHubConnection *conn = new ehttpHubConnection(*this);
  sockets.addref(conn);
  lwarnif(!esocket::accept(*conn),"failure in accepting connection");
  linfo("new connection from: "+conn->faddress+":"+conn->fport);
}

void ehttpHub::sendAll(const estr& data,const estr& session)
{
  linfo("sending to all clients");
  int i;
  for (i=0; i<client.size(); ++i){
    if (client.keys(i)==session) continue;
    client.values(i).send(data);
  }
}

void ehttpHub::sendChannel(const estr& data,const estr& channel,const estr& session)
{
  linfo("sending to all on channel: "+channel+" session: "+session);
  int i;
  for (i=0; i<client.size(); ++i){
    if (client.keys(i)==session || client.values(i).channel!=channel) continue;
    client.values(i).send(data);
  }
}

esvararray ehttpHub::usersInChannel(const estr& session,const estr& channel)
{
  int i;
  esvararray tmparr;
  int count=0;
  
  for (i=0; i<client.size(); ++i){
    if (client.values(i).session==session) continue;
//    cout << channel << " --- " <<client.values(i).channel << " --- " << client.values(i).username << endl;
    if (client.values(i).channel==channel){
      esvararray user;
      user.add("uid",client.values(i).session);
      user.add("user",client.values(i).username);
      tmparr.add(estr(count),new esvararray(user));
      ++count;
//      tmparr.add(client.values(i).session+"="+client.values(i).username);  //client.values(i).username);
    }
  }
  tmparr.add("length",(const int&)count);
  return(tmparr);
}


