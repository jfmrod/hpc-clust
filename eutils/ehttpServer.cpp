#include "ehttpServer.h"


#include "efile.h"

estr makeURI(const estr& host,const estr& location,estrarray& vars)
{
  estr tmpstr;
  tmpstr = "http://" + host + location;
  if (vars.size()){
    tmpstr+="?";
    tmpstr+=vars.join("&","=");
  }
  return(tmpstr);
}



ehttpServerConnection::ehttpServerConnection(): protstate(0)
{
//       "Date: Wed, 27 Feb 2008 10:46:15 GMT\r\n"
  sendHeaders["Server"]="konceptFX 1.0";
  sendHeaders["Cache-Control"]="no-store, no-cache, must-revalidate";
//  sendHeaders["Cache-Control"]="post-check=0, pre-check=0";
  sendHeaders["Keep-Alive"]="timeout=65535, max=15";
  sendHeaders["Connection"]="Keep-Alive";
}



bool ehttpServerConnection::send(const estr& data,int status,const estr& mimetype)
{
  if (protstate!=3){ lerror("tried to send without request: location: "+location); return(false); }
  estr headerstr;
  estr statstr;
  switch (status){
    case 200: statstr="OK"; break;
    case 301: statstr="Moved Permanently"; break;
    case 404: statstr="Not found"; break;
   default:
    statstr="none";
  }

  sendHeaders["Content-Length"]=estr(data.len());
  sendHeaders["Content-Type"]=mimetype;

  headerstr=sendHeaders.join("\r\n",": ");

//  cout << "sendHeaders: "+headerstr<<endl;
  estr tmpstr;

  tmpstr+="HTTP/1.1 "+estr(status)+" "+statstr+"\r\n";
  tmpstr+=headerstr;
  tmpstr+="\r\n\r\n";
  tmpstr+=data;

  int len;
  len=esocket::send(tmpstr);
  
  if (len==0){lerror("connection was closed"); close(); doClose(); return(false);}
  
  estr sendbuffer;
  if (len < tmpstr.len()) {
    if (len <= 0){
      close();
      doClose();
      lerror("connection closed! Unable to send data: "+estr(len)+" --- "+estr(tmpstr.len())+" --- "+estr(data.len()));
      return(false);
    }else{
      lwarn("did not send whole data: "+estr(len)+" --- "+estr(tmpstr.len()));
      sendbuffer += tmpstr.substr(len);
      lwarn("sendbuffer: "+estr(sendbuffer.len()));
      do {
        sleep(1);
        len=esocket::send(sendbuffer);
	lwarn("sent rest: "+estr(len)+" of "+estr(tmpstr.len()));
	if (len<=0) { lerror("connection closed! Unable to send data: "+estr(len)+" --- "+estr(tmpstr.len())+" --- "+estr(data.len())); return(false); }
        sendbuffer.del(0,len);
	lwarn("remaining rest: "+estr(sendbuffer.len()));
      } while (sendbuffer.len());
    }
  }
  protstate=0;
  return(true);
}



void ehttpServerConnection::sendRedirect(const estr& newurl)
{
  sendHeaders["Location"]=newurl;
  send("<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">"
       "<html><head>"
       "<title>301 Moved</title>"
       "</head><body>"
       "<h1>Moved</h1>"
       "<p>This page has moved to <a href=\""+newurl+"\">"+newurl+"</a>.</p>"
       "<hr>"
       "<address>KonceptFX 1.0</address>"
       "</body></html>",301);
  linfo("Redirected to: "+newurl);
}

void ehttpServerConnection::sendNotFound()
{
  send("<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">"
       "<html><head>"
       "<title>404 Not Found</title>"
       "</head><body>"
       "<h1>Not Found</h1>"
       "<p>The requested URL "+location+" was not found on this server.</p>"
       "<hr>"
       "<address>KonceptFX 1.0</address>"
       "</body></html>",404);
  lerror("sent not found for location: "+location);
}

estrarray mimetypes="gif=image/gif,png=image/png,jpg=image/jpg,swf=application/x-shockwave-flash,html=text/html,js=text/javascript,css=text/css";

void ehttpServerConnection::sendFile(const estr& filename)
{
  efile file(filename);
  estr data;
  estr mimetype;

  if (file.exists()){
    file.read(data);
    if (mimetypes.findkey(file.extension())!=-1)
      send(data,200,mimetypes[file.extension()]);
    else{
      send(data,200);
      lwarn("no mimetype defined for extension: "+file.extension()+" using default: text/html");
    }
  }else{
    lerror("Tried to send file: "+filename+" but it was not found");
    sendNotFound();
  }
}

void ehttpServerConnection::parseLocation(estr& location,estrarray& vars)
{
  int i,j;
  i=location.find("?");
  if (i==-1) return;

  estrarray tmparr=location.substr(i+1).explode("&");
  location=location.substr(0,i);
  for (i=0; i<tmparr.size(); ++i){
    j=tmparr[i].find("=");
    if (j==-1)
      vars[tmparr[i]]="";
    else
      vars[tmparr[i].substr(0,j)]=tmparr[i].substr(j+1);
  }
}

void ehttpServerConnection::doRecv()
{
  int i;
  estr line;
  estr tmpdata;
  estrarray tmparr;

  recv(tmpdata);

  data+=tmpdata;

  if (protstate==0){
    if (!data.getline(line)) return;

    requestHeaders.clear();
    tmparr = line.explode(" ");
    if (tmparr.size() < 3) { lerror("malformed request: "+line); close(); doClose(); return; }
    cmd = tmparr[0];
    getVars.clear();
    location = tmparr[1];
    request_uri = tmparr[1];
    parseLocation(location,getVars);
    protocol = tmparr[2];
    ++protstate;
  }

  while (protstate==1){
    if (!data.getline(line)) return;
    cout << "HTTP REQUEST line: " << line << endl;

    if (line.len()==0)
      { ++protstate; break; }

    i = line.find(":");
    if (i==-1)
      { lerror("missing header separator: "+line); cout << request_uri << endl; cout << requestHeaders << endl; continue; }

    requestHeaders.add(line.substr(0,i),line.substr(i+1).trim());
  }

  if (protstate==2){
    if (requestHeaders.findkey("Host")!=-1)
      request_uri="http://"+requestHeaders["Host"]+request_uri;

    linfo("got request: "+cmd+" "+location);
  }

//  if (protstate>=2&&requestHeaders.findkey("User-Agent")!=-1 && requestHeaders["User-Agent"].find("Firefox")!=-1)
//    cout << "RECVDATA:"<<endl<<tmpdata<<endl;

  if (cmd=="GET" && protstate==2){
    ++protstate;
    doHandleGet(location);
  } else if (cmd=="POST" && protstate==2){
    int postlen;
    if (requestHeaders.findkey("Content-Length")==-1) { lerror("Malformed POST request"); close(); doClose(); return; }

    postlen = requestHeaders["Content-Length"].i();
    if (data.len()<postlen) return;

    postdata=data.substr(0,postlen);
    data.del(0,postlen);
    ++protstate;
    doHandlePost(location);
  }
}

