#include "ehttp.h"

#include "esystem.h"

estr escape_string(const estr& str)
{
  estr res(str);
  res.replace("\\","\\\\");
  res.replace("\"","\\\"");
  res.replace("\n","\\n");
  res.replace("\r","\\r");
  res.replace("\t","\\t");
  return(res);
}

estr json_serialize(const evar& var)
{
  if (!getClasses().exists(var.getClass())) return(estr());

  estr res="{";
  evar pvar;
  int i;
  for (i=0; i<var.properties().size(); ++i){
    pvar.set(var.property(var.properties()[i]));
    res+=var.properties()[i];
    res+=":";
    if (pvar.getTypeid()==typeid(int))
      res+=pvar.get<int>();
    else if (pvar.getTypeid()==typeid(float))
      res+=pvar.get<float>();
    else if (pvar.getTypeid()==typeid(estr))
      res+="\""+escape_string(pvar.get<estr>())+"\"";
    else
      res+=json_serialize(pvar);
    res+=",";
  }
  res.del(-1);
  res+="}";
  return(res);
}

void json_getString(evar& var,const estr& data,int& i)
{
  int j;
  estr tmpstr;
  for (j=i+1; j<data.len() && data[j]!=data[i]; ++j){ if (data[j]=='\\') ++j; }
  if (j>=data.len()) { i=j; return; }
  if (var.getTypeid()==typeid(estr)){
    tmpstr=data.substr(i+1,j-i-1);
    tmpstr.replace("\\\\","\\");
    tmpstr.replace("\\\"","\"");
    tmpstr.replace("\\r","\r");
    tmpstr.replace("\\n","\n");
    tmpstr.replace("\\t","\t");
    var=evar(tmpstr);
  }
  i=j+1;
}
void json_getNumber(evar& var,const estr& data,int& i)
{
  int decimal=-1;
  int exponent=-1;
  int j;
  for (j=i; j<data.len(); ++j){
    if (!isdigit(data[j])){
      if (data[j]=='.') { if (decimal==-1 && exponent==-1) decimal=j; else break; }
      else if (tolower(data[j])=='e') { if (exponent==-1) exponent=j; else break; }
      else if (data[j]=='-'){ if (i!=j || j!=exponent+1) break; }
      else break;
    }
  }
  lerrorif(i==j,"should never happen");

  if (var.getTypeid()==typeid(int))
    var=evar(data.substr(i,j-i).i());
  else if (var.getTypeid()==typeid(float))
    var=evar(data.substr(i,j-i).f());
  i=j;
}
inline void json_skipBlanks(const estr& data,int& i)
{
  while(i<data.len() && data[i]==' ') ++i;
}
void json_getProperty(const estr& data,int& i,estr& pname)
{
  pname.clear();
  if (data[i]=='\'' || data[i]=='"'){
    int j;
    for (j=i+1; j<data.len() && data[j]!=data[i]; ++j);
    if (j==data.len()) { i=j; return; }
    pname=data.substr(i+1,j-i-1);
    i=j+1;
    return;
  }
  int j;
  for (j=i; j<data.len() && data[j]!=':' && data[j]!=' '; ++j);
  if (j==data.len()) { i=j; return; }
  pname=data.substr(i,j-i);
  i=j;
}
int json_unserialize(evar var,const estr& data,int i)
{
  json_skipBlanks(data,i);
  if (i==data.len()) return(i);
  if (isdigit(data[i]) || data[i]=='-')
    json_getNumber(var,data,i);
  switch (data[i]){
    case '{':{
      ++i;
      estr pname;
      evar pvalue;
      do {
        json_skipBlanks(data,i);
        if (i==data.len() || data[i]=='}') break;
        json_getProperty(data,i,pname);
        json_skipBlanks(data,i); 
        if (i==data.len() || data[i]!=':') { lerror("unexepected end of string. missing value"); break; }
        ++i;
        pvalue.clear();
        if (var.hasProperty(pname))
          pvalue.set(var.property(pname));
        i=json_unserialize(pvalue,data,i);
        json_skipBlanks(data,i); 
        if (data[i]==',') ++i;
      } while (i<data.len());
     } break;
    case '\'':
    case '"':
      json_getString(var,data,i);
     break;
  }
  return(i);
}


void json2_getString(evar& var,const estr& data,int& i)
{
  int j;
  estr tmpstr;
  for (j=i+1; j<data.len() && data[j]!=data[i]; ++j){ if (data[j]=='\\') ++j; }
  if (j>=data.len()) { i=j; return; }
  tmpstr=data.substr(i+1,j-i-1);
  tmpstr.replace("\\\\","\\");
  tmpstr.replace("\\\"","\"");
  tmpstr.replace("\\r","\r");
  tmpstr.replace("\\n","\n");
  tmpstr.replace("\\t","\t");
  var.set(new estr(tmpstr));
  i=j+1;
}
void json2_getNumber(evar& var,const estr& data,int& i)
{
  int isFloat=false;
  int decimal=-1;
  int exponent=-1;
  int j;
  for (j=i; j<data.len(); ++j){
    if (!isdigit(data[j])){
      if (data[j]=='.') { isFloat=true; if (decimal==-1 && exponent==-1) decimal=j; else break; }
      else if (tolower(data[j])=='e') { isFloat=true; if (exponent==-1) exponent=j; else break; }
      else if (data[j]=='-'){ if (i!=j || j!=exponent+1) break; }
      else break;
    }
  }
  lerrorif(i==j,"should never happen");

  if (isFloat)
    var.set(data.substr(i,j-i).f());
  else
    var.set(data.substr(i,j-i).i());

  i=j;
}


int json2_unserialize(evar& var,const estr& data,int i)
{
  json_skipBlanks(data,i);
  if (i==data.len()) return(i);
  if (isdigit(data[i]) || data[i]=='-')
    json2_getNumber(var,data,i);
  switch (data[i]){
    case '{':{
      ++i;
      var.set(new esvararray());
      estr pname;
      evar pvalue;
      do {
        json_skipBlanks(data,i);
        if (i==data.len() || data[i]=='}') break;
        json_getProperty(data,i,pname);
//        cout << "property: " << pname << endl;
        json_skipBlanks(data,i); 
        if (i==data.len() || data[i]!=':') { lerror("unexpected end of string. missing value"); break; }
        ++i;
        pvalue.clear();
        i=json2_unserialize(pvalue,data,i);
        var.get<esvararray>().add(pname,pvalue);
        json_skipBlanks(data,i); 
        if (data[i]==',') ++i;
      } while (i<data.len());
     } break;
    case '\'':
    case '"':
      json2_getString(var,data,i);
     break;
  }
  return(i);
}


estr json2_serialize(const evar& var)
{
  if (var.getTypeid()==typeid(int))
    return(var.get<int>());
  else if (var.getTypeid()==typeid(float))
    return(var.get<float>());
  else if (var.getTypeid()==typeid(estr))
    return("\""+escape_string(var.get<estr>())+"\"");
  else if (var.getTypeid()!=typeid(esvararray))
    { lerror("unexpected type: "+var.getClass()); return(estr()); }


  esvararray& arr(var.get<esvararray>());
  if (arr.size()==0) return("{}");

  estr res="{";
  evar pvar;
  int i;
  for (i=0; i<arr.size(); ++i){
    pvar.set(arr.values(i));
    res+="\""+arr.keys(i)+"\"";
    res+=":";
    if (pvar.getTypeid()==typeid(int))
      res+=pvar.get<int>();
    else if (pvar.getTypeid()==typeid(float))
      res+=pvar.get<float>();
    else if (pvar.getTypeid()==typeid(estr))
      res+="\""+escape_string(pvar.get<estr>())+"\"";
    else if (pvar.getTypeid()==typeid(esvararray))
      res+=json2_serialize(pvar);
    else
      { lerror("unexpected type: "+pvar.getClass()); return(estr()); }
    res+=",";
  }
  res.del(-1);
  res+="}";
  return(res);
}





void ehttp::clear()
{
  data.clear();
  headers.clear();
  retcode=0;
  retversion.clear();
  retstrcode.clear();
  datalen=-1;
  chunklen=-1;
  protstate=0;
}

void ehttp::doRequestComplete()
{
  sendHeaders.clear();
  cout << "Request complete: " << url << endl;
  onDone();
}

void ehttp::doClose()
{
  cout << "Connection closed: " << url << endl;
}

void ehttp::doRecv()
{
  int i;
  estr line;
  estr tmpdata;
  estrarray tmparr;

//  cout << "ehttp::doRecv: ";
  socket.recv(tmpdata);
  if (tmpdata.len()==0) return;

//  cout << "url: " << url << " received: " << tmpdata << endl;
  data+=tmpdata;

  if (protstate==0){
    if (!data.getline(line)) return;

    headers.clear();

    tmparr = line.explode(" ");
    if (tmparr.size() < 3) { lerror("malformed request: "+line); socket.close(); doClose(); return; }

    retversion = tmparr[0];
    retcode = tmparr[1].i();
    retstrcode = tmparr[2];
    lwarnif(retcode != 200,"ehttp retcode: "+estr(retcode));
    ++protstate;
  }

  while (protstate==1){
    if (!data.getline(line)) return;

    if (line.len()==0){
      protstate=2;
      if (headers.findkey("Content-Length")!=-1){
        datalen=headers["Content-Length"].i();
        if (datalen<0) { lerror("Content-Length is negative"); socket.close(); doClose(); return; }
      }else if (retversion=="HTTP/1.1" && (headers.findkey("Transfer-Encoding")==-1 || headers["Transfer-Encoding"]!="chunked")){
        lerror("Missing Content-Length and Transfer-Enconding with http protocol 1.1");
        socket.close(); doClose(); return;
      }else if (retversion=="HTTP/1.1"){
        protstate=3; // data transfer in chunk mode
        chunklen=0;
      }
      break;
    }

    i = line.find(":");
    if (i==-1)
      { lerror("missing header separator: "+line); continue; }

    estr key,val;
    key=line.substr(0,i);
    val=line.substr(i+2);
    headers.add(key,val);
  }

  if (protstate>2){
    while (data.len()>chunklen) {
      if (protstate==3){
        tmpdata=data.substr(chunklen);
        if (!tmpdata.getline(line)) return;
        data.del(chunklen); data+=tmpdata;
        nextchunk=line.h();
        cout << ">> url: " << url << " next chunk: " << nextchunk << endl;
        if (nextchunk<0) { lerror("negative chunk size?"); socket.close(); doClose(); return; } 
        chunklen+=nextchunk;
        protstate=4;
      }
      if (protstate==4 && data.len()>chunklen){
        tmpdata=data.substr(chunklen);
        if (!tmpdata.getline(line)) return;
        data.del(chunklen); data+=tmpdata;
        if (nextchunk==0) { protstate=-1; doRequestComplete(); return; }
        protstate=3;
      }
    }
  }else if (protstate==2){
    if (datalen>=0 && data.len()>=datalen){
      lwarnif(data.len()>datalen,"Too many bytes received");
      protstate=-1;
      doRequestComplete();
    }
  }
}

bool ehttp::get(const estr& _url)
{
  estr newhostname;
  clear();
  url=_url;
  newhostname=_url;
  protstate=0;

  newhostname.del(0,7);
  int i;
  i=newhostname.find("/");
  if (i==-1) location="/";
  else {
    location=newhostname.substr(i);
    newhostname.del(i);
  }
  if (socket.fhsock==0 || hostname==newhostname){
    socket.close();
    hostname=newhostname;
    cout << " connecting to: "<< hostname <<endl;
    socket.onClose   = efunc(*this,&ehttp::doClose);
    socket.onReceive = efunc(*this,&ehttp::doRecv);
    if (!socket.connect(hostname,80)) return(false);
  }else{
    cout << " already connected to: "<< hostname <<endl;
  }

  sendHeaders["Host"]=hostname;

  cout << " getting page: "<< location <<endl;
  cout << sendHeaders << endl;

  estr headerstr;
  headerstr=sendHeaders.join("\r\n",": ");

//  cout << "GET "+url.substr(i)+" HTTP/1.0\r\n"+headerstr+"\r\n"<<endl;
  socket.send("GET "+location+" HTTP/1.1\r\n"+headerstr+"\r\n\r\n");
//  socket.send("GET "+url.substr(i)+" HTTP/1.0\r\n\r\n");
  

//  while(socket.fhsock){
//  socket.wait();
//    getSystem()->wait();
//  }

  return(true);
}

void ehttp::wait()
{
  while(socket.fhsock){
    getSystem()->wait();
  }
}


