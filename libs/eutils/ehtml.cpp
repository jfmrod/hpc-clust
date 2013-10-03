#include "ehtml.h"

#include "logger.h"
#include "efile.h"
#include "eregexp.h"

inline bool isURLUnreserved(char c)
{
  return( c>='a'&&c<='z' || 
          c>='A'&&c<='Z' ||
          c>='0'&&c<='9' || 
          c=='-'||c=='_' ||
          c=='.'||c=='~' );
}

inline estr chartohex(char c)
{
  estr res;
  res.reserve(256);
  sprintf(res._str,"%hhx",c);
  res._strlen=strlen(res._str);
  return(res);
}

#define H2C(c)  if (c >= '0' && c <= '9')      \
                  res=res|(c-'0');             \
                else if (c >= 'a' && c <= 'f') \
                  res=res|(c-'a'+10u);         \
                else if (c >= 'A' && c <= 'Z') \
                  res=res|(c-'A'+10u);

inline char hextochar(char c,char c2)
{
  char res;
  res=0x00;
  H2C(c);
  res=res<<4;
  H2C(c2);
  return(res);
}

estr urldecode(const estr& str)
{
  estr res;

  int i;
  for (i=0; i<str.len(); ++i){
    if (str[i]=='%'){
      if (i+2 < str.len())
        res+=hextochar(str[i+1],str[i+2]);
      i+=2;
    }else if (str[i]=='+')
      res+=' ';
    else
      res+=str[i];
  }
  return(res);
}

estr urlencode(const estr& str)
{
  estr res;

  int i;
  for (i=0; i<str.len(); ++i){
    if (isURLUnreserved(str[i]))
      res+=str[i];
    else
      { res+='%'; res+=chartohex(str[i]); }
  }
  return(res);
}

estrarray htmlchars("\"=&quot;,'=&apos;,<=&lt;,>=&gt;,&=&amp;");


estr html_entities(const estr& str)
{
  estr tmpstr(str);
  int i;
  for (i=0; i<htmlchars.size(); ++i)
    tmpstr.replace(htmlchars.keys(i),htmlchars.values(i));
  return(tmpstr);
}

eregexp entre("&[a-z]+;");
estr html_entities_decode(const estr& str)
{
  int i=0;
  int ihe;
  estr tmpstr;
  while (entre.match(str,i)>0){
    tmpstr+=str.substr(i,entre.b-i);
    ihe=htmlchars.find(str.substr(entre.b,entre.e-entre.b));
    if (ihe==-1) { lwarn("unknown html entity char: "+str.substr(entre.b,entre.e-entre.b)); tmpstr+=str.substr(entre.b,entre.e-entre.b); }
    else tmpstr+=htmlchars.keys(ihe);
    i=entre.e;
  }
  tmpstr+=str.substr(i);
  return(tmpstr);
}




inline void skip_until2(const estr& str,const char *c,int &i){
  const char *p;
  for(;i<str.len(); ++i){
    for (p=c; *p!=0x00; ++p){
      if (str._str[i]==*p) return;
    }
  }
}
inline void skip_until(const estr& str,char c,int &i){
  while(i<str.len() && str._str[i]!=c) ++i;
}

inline void skip_blank(const estr& str,int &i){
  while(i<str.len() && str._str[i]==' ') ++i;
}

void tag_get(const estr& str,int& i,ehtmltag& tag)
{
  int s;

  estr argname;
  estr argvalue;

  tag.single=false;

  ++i;

  skip_blank(str,i);
  s=i;
  if (str._str[i]=='/') ++i;

  skip_until2(str,"> /",i);
  tag.type=str.substr(s,i-s).lowercase();
  if (i>=str.len()){ lerror("unexpected EOF: tag missing closing >"); return; }
  if (tag.type.substr(0,3)=="!--"){ tag.type="!--"; i=s+3; return; }
  if (tag.type.substr(0,8)=="!doctype"){ tag.type="!doctype"; i=s+8; return; }

  while (i<str.len()){
    skip_blank(str,i);
    if (i>=str.len()) { lerror("unexpected EOF: tag missing closing >"); return; }
    s=i;
    skip_until2(str," =>/",i);
    argname=str.substr(s,i-s).lowercase();
//    if (tag.type=="body") cout << "argname: "<<argname<<endl;
    skip_blank(str,i);
    if (i>=str.len()){ lerror("unexpected EOF: tag missing closing >"); return; }

    if (str._str[i]=='>') break;

    if (str._str[i]=='/') {
      ++i;
      tag.single=true;
      skip_blank(str,i);
      if (str._str[i]!='>') lerror("Missing closing > in tag: "+tag.type); 
      break;
    }

    if (str._str[i]=='>') { tag.args.add(argname,""); lwarn("tagarg missing =value part"); break; }
    if (str._str[i]!='=') { tag.args.add(argname,""); lwarn("tagarg missing =value part"); continue; }

    ++i;
    skip_blank(str,i);
    if (i>=str.len()) { tag.args.add(argname,""); lerror("unexpected EOF: tag missing closing >"); break; }
    if (str._str[i]=='>') { tag.args.add(argname,""); lwarn("tagarg missing =value part"); break; }
    s=i;
    if (str._str[i]=='"' || str._str[i]=='\''){
      char delim=str._str[i];
      ++s; ++i;
      for (;i<str.len(); ++i){
        if (str._str[i]=='\\') ++i;
        if (str._str[i]==delim) break;
      }
      lerrorif(i==str.len(),"unexpected EOF: tagarg string missing closing delimiter: "+estr(delim));
      argvalue=str.substr(s,i-s);
      ++i;
    }else{
      skip_until2(str," >",i);
      argvalue=str.substr(s,i-s);
    }
//    if (tag.type=="body") cout << "argval: "<<argvalue<<endl;
    tag.args.add(argname,argvalue);
  }

  if (!tag.single && tag.type.len()>0 && tag.type[0]!='/' && str.ifind("</"+tag.type+">",i)==-1){
    lwarn("tag: "+tag.type+" missing closing /tag, assuming single tag");
    tag.single=true;
  }
}
/*
ehtmltag::~ehtmltag()
{
  int i;
  for (i=0; i<parts.size(); ++i)
    delete &parts[i];
}
*/

void ehtmltag::addText(const estr& text,int i)
{
  ehtmltag *ntag=new ehtmltag;
  ntag->text=text;
  if (i==-1)
    parts.addref(ntag);
  else
    parts.insertref(i,ntag);
}

void ehtmltag::addTag(const ehtmltag& tag)
{
  ehtmltag *ntag=new ehtmltag;
  ntag->single=tag.single;
  ntag->text=tag.text;
  ntag->type=tag.type;
  ntag->args=tag.args;
  int i;
  for (i=0; i<tag.parts.size(); ++i)
    ntag->addTag(tag.parts[i]);
  parts.addref(ntag);
}

estrarray containingTypes("html,body,div,center,p,span,h1,h2,h3,h4,h5,h6,table,tr,td,ul,ol,li,b,u,i,em,iframe,blockquote,form,font,small,strong");

void ehtmltag::parse(ehtml& html,const estr& data,int &i)
{
  ehtmltag *tag;
  int s,ts;

  ldinfo(" >> "+type);

  if (type=="script" || type=="style"){
    s=i;
    i=data.ifind("</"+type+">",i);
    if (i==-1) { lerror("end </"+type+"> not found in document"); return; }
    addText(data.substr(s,i-s));
    tag=new ehtmltag;
    tag_get(data,i,*tag);
    delete tag;
    ++i; // skip the > character
    return;
  }
  if (type=="!--"){
    s=i;
    i=data.find("-->",i);
    if (i==-1) { lerror("closing --> not found in document"); return; }
    addText(data.substr(s,i-s));
    i+=3; // skip the --> closing tag
    return;
  }
  if (type=="!doctype"){
    s=i;
    i=data.find(">",i);
    if (i==-1) { lerror("closing > for !doctype tag not found in document"); return; }
    addText(data.substr(s,i-s));
    i+=1; // skip the > closing tag
    cout << data.substr(s,i-s) << endl;
    return;
  }
  if (type.len()>0 && type[0]=='/'){ lerror("problem in parsing, found unhandled closing tag: "+type); }

  while (i<data.len()){
    s=i;
    i=data.find('<',i);
    if (i==-1) { i=s; break; }
    if (s!=i)
      addText(data.substr(s,i-s));

    ts=i;
    tag=new ehtmltag;
    tag_get(data,i,*tag);
    if (tag->type=="a")
      html.links.add(tag);
    else if (tag->type=="form")
      html.forms.add(tag);
    else if (tag->type=="script")
      html.scripts.add(tag);
    else if (tag->type=="link")
      html.css.add(tag);
    else if (tag->type=="img")
      html.images.add(tag);
    else if (tag->type=="head")
      html.head=tag;
    else if (tag->type=="body")
      html.body=tag;

    if (tag->args.findkey("id")!=-1)
      html.tags.add(tag->args["id"],tag);

    ++i;
    if (tag->type.len() && tag->type[0]=='/' && tag->type.substr(1) == type)
      { ldinfo(" << "+type); delete tag; return; } 
    if (single && tag->type.len() && tag->type[0]=='/')
      { ldinfo(" << "+type); i=ts; delete tag; return; }
    if (tag->type.len() && tag->type[0]=='/')
      { lerror("Unexpected end tag: "+tag->type+" in tag: "+type); i=ts; delete tag; return; }

    if (!tag->single || containingTypes.find(tag->type)!=-1)
      tag->parse(html,data,i);

    parts.addref(tag);
  }
  if (i<data.len()){ 
    addText(data.substr(i));
    i=data.len();
  }
}

void ehtmltag::make(estr& odata)
{
  int i;
  if (!type.len())
    { odata+=text; return; }

  if (type=="!--"){
    odata+="<!--";
    for (i=0; i<parts.size(); ++i)
      parts[i].make(odata);
    odata+="-->";
    return;
  }
    
  if (type=="!doctype"){
    odata+="<!doctype ";
    for (i=0; i<parts.size(); ++i)
      parts[i].make(odata);
    odata+=">";
    return;
  }
   odata+="<"+type;
  for (i=0; i<args.size(); ++i)
    odata+=" "+args.keys(i)+"=\""+args.values(i)+"\"";
  
  if (single && containingTypes.find(type)==-1){
    odata+=" />";
  }else
    odata+=">";

  for (i=0; i<parts.size(); ++i)
    parts[i].make(odata);

  if (!single)
    odata+="</"+type+">";
}

void ehtml::clear()
{
  images.clear();
  links.clear();
  scripts.clear();
  css.clear();
  head=0x00;
  body=0x00;
  parts.clear();
}

void ehtml::parse(const estr& data)
{
  int i;
  clear();
  cout << "ehtml::parse, data.len=" << data.len() << endl;
  i=0;
  ehtmltag::parse(*this,data,i);
}

void ehtml::load(const estr& filename)
{
  efile file(filename,"r");

  if (!file.exists()) { lerror("html file not found: "+filename); return; }
  
  estr data;
  file.read(data);
  parse(data);
}

void ehtml::save(const estr& filename)
{
  efile file(filename,"w");
  estr data(make());
  file.write(data);
  file.close();
}

estr ehtml::make()
{
  int i;
  estr odata;
  for (i=0; i<parts.size(); ++i)
    parts[i].make(odata);
  return(odata);
}


