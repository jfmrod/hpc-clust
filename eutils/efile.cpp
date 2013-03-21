#include "efile.h"

#define _WIN32_WCE 0x0400

#include "logger.h"
#include "esystem.h"
#include "eregexp.h"

#include <sys/types.h>
#include <sys/stat.h>

#include <stdio.h>

#ifndef __MINGW32CE__
#include <errno.h>
#endif

#ifndef _MSC_VER
 #include <unistd.h>
#else
 #include <direct.h>
#endif

#ifdef _WIN32
 #include <windows.h>
 #include <windowsx.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#include <zlib.h>

#include <libgen.h>

#undef basename
#undef dirname

egzfile::egzfile(): f(0x00),opened(false){}
egzfile::egzfile(const estr& _name,const estr& _mode): name(_name),mode(_mode) {}
egzfile::~egzfile()
{
// close();
}

void egzfile::close()
{
  if (f && opened)
    gzclose(f);
  f=0x00;
  opened=false;
}
bool egzfile::open()
{
  lerrorifr(name.len()==0 || mode.len()==0,"no filename specified",(false));
  return(open(name,mode));
}

bool egzfile::open(const estr& _name,const estr& _mode)
{
  ldinfo("openning gzfile: "+_name+" mode: "+_mode);
//  close();

  name=_name;
  mode=_mode;

  f=gzopen(name._str,mode._str);
  if (f==0x00){
    lwarn("efile: unable to open file \""+name+"\"");
    return(false);
  }
  return(true);
}

/*
bool egzfile::open(const estr& mode)
{
  ldinfo("openning gzfile: "+name+" mode: "+mode);
  close();

  f=gzopen(name._str,mode._str);
  if (f==0x00){
    lwarn("efile: unable to open file \""+name+"\"");
    return(false);
  }
  return(true);
}
*/
void egzfile::write(const estr& str)
{
  if (f==0x00 && !open(name,"w")) return;
  gzwrite(f,str._str,str.len());
}
bool egzfile::read(estr& str,long int len)
{
  if (f==0x00 && !open(name,"r")) return(false);

  if (len==-1){
    len=0;
    long int tmplen;
    do {
      str.reserve(len+1024);
      tmplen=gzread(f,&str._str[len],1024);
      if (tmplen<=0) break;
      len+=tmplen;
    } while (tmplen>0);
  }else{
    str.reserve(len);
    len=gzread(f,str._str,len);
  }
  if (len){
    str._strlen=len;
    str._str[len]=0x00;
    return(true);
  }
  str._str[0]=0x00;
  str._strlen=0;
  return(false);
}
void egzfile::flush()
{
  gzflush(f,Z_SYNC_FLUSH);
}
bool egzfile::readln(estr& str)
{
  if (f==0x00 && !open(name,"r")) return(false);

  long int len;
  int p;
  char *res;
  len=1024;
   

/*
  if (!blocking && fileno(f) == fileno(stdin)){
#ifdef _MSC_VER
    getSystem()->wait(GetStdHandle(STD_INPUT_HANDLE));
#else
    getSystem()->wait(fileno(stdin));
#endif
  }
*/

  str._str[0]=0x00;
  str._strlen=0;

  p=0;
  do {
    str.reserve(len);
    linfo("getting string");
    res=gzgets(f,&str._str[p],len-p);
    p=strlen(str._str);
    linfo("got string");
    if (!res) {
      if (gzeof(f))
        return(false);
      str._str[0]=0x00;
      str._strlen=0;
      return(false);
    }

    str._strlen=strlen(str._str);
    if (str._strlen>=1 && str[str._strlen-1]=='\n')
      { str[str._strlen-1]=0x00; --str._strlen; return(true); }
    else if (str._strlen>=2 && str[str._strlen-2]=='\r' && str[str._strlen-1]=='\n')
      { str[str._strlen-2]=0x00; str._strlen-=2; return(true); }
    len=len*2;
  }while (1);
  return(false);
}



ostream& operator<<(ostream& stream,const efile& file)
{
  if (file.f!=0x00)
    stream << "[efile name: "<<file.name<<" fd: " << file.fileno() << "]";
  else
    stream << "[efile name: "<<file.name<<"]";
  return(stream);
}


efile::~efile()
{
//  close();
}


efile::efile(const efile& file): f(file.f),opened(file.opened),blocking(file.blocking),name(file.name),mode(file.mode) {}

efile::efile(): f(0x00),opened(false),blocking(false) {}
efile::efile(const estr& _name): f(0x00),opened(false),name(_name),mode("r"),blocking(false) {}
efile::efile(const char* _name): f(0x00),opened(false),name(_name),mode("r"),blocking(false) {}
efile::efile(char* _name): f(0x00),opened(false),name(_name),mode("r"),blocking(false) {}
efile::efile(const estr& _name,const estr& _mode): f(0x00),opened(false),name(_name),mode(_mode),blocking(false) {}
efile::efile(int fd,const estr& mode): blocking(false)
{
  open(fd,mode);
//  fcntl(fileno(file),F_SETFL,O_NONBLOCK);
}

efile::efile(FILE* file): f(file),opened(true),blocking(false)
{
//  fcntl(fileno(file),F_SETFL,O_NONBLOCK);
}

bool efile::eof() const
{
  if (f==0x00){ lerror("file handle is null"); return(-1); }
  return(::feof(f));
}

int efile::fileno() const
{
  if (f==0x00){ lerror("file handle is null"); return(-1); }
  return(::fileno(f));
}

estr efile::extension() const
{
  int i;
  for (i=name.len()-1; i>=0; --i){
    if (name[i]=='.') break;
  }
  if (i<=0) return("");
  return(name.substr(i+1));
}
estr efile::basename() const
{
  estr tmpname(name);
  return(::basename(tmpname._str));
}
estr efile::dirname() const
{
  estr tmpname(name);
  return(::dirname(tmpname._str));
}

void efile::disableBuffer() const
{
  setvbuf(f,0x00,_IONBF,0);
}

void efile::close() const
{
  if (f && opened)
    fclose(f);
  f=0x00;
  opened=false;
}

bool efile::open(FILE *_file)
{
  ldinfo("openning file descritor");
//  close();
  opened=true;
  f=_file;
  if (f==0x00){
    opened=false;
    lwarn("efile: invalid file handle");
    return(false);
  }
  return(true);
}

bool efile::open(int fd,const estr& mode)
{
  ldinfo("openning file descritor");
  opened=true;
  f=fdopen(fd,mode._str);
  if (f==0x00){
    opened=false;
    lwarn("efile: invalid file handle");
    return(false);
  }
  return(true); 
}

bool efile::open() const
{
  lerrorifr(name.len()==0 || mode.len()==0,"no filename specified",(false));

  f=fopen(name._str,mode._str);
  if (f==0x00){
    lwarn("efile: unable to open file \""+name+"\"");
    return(false);
  }
  opened=true;
  return(true);
}

bool efile::open(const estr& _name,const estr& _mode)
{
  ldinfo("openning file: "+_name+" mode: "+_mode);
//  close();

  name=_name;
  mode=_mode;

  return(open());
}

bool efile::create() const
{
  bool res;
  mode="w";
  res=open();
  if (res){
    close();
    return(true);
  }
  return(false);
}

/*
bool efile::open(const estr& mode)
{
  ldinfo("openning file: "+name+" mode: "+mode);
  close();

  f=fopen(name._str,mode._str);
  if (f==0x00){
    lwarn("efile: unable to open file \""+name+"\"");
    return(false);
  }
  return(true);
}
*/
bool efile::exists() const
{
  struct stat sb;
  long int r;

  r=stat(name._str,&sb);

  if (!r) return(true);
  return(false);
}

long int efile::size() const
{
  struct stat sb;
  long int r;

  r=stat(name._str,&sb);

  if (r==0) return(sb.st_size);
  return(-1);
}

void efile::flush() const
{
  fflush(f);
}

evar efile::read() const
{
  if (f==0x00){
    mode="r";
    if (!open()) return(false);
  }

  char tmpsz[4];
  int l=fread(tmpsz,sizeof(char),4,f);
  int i;

  if (l<4) {
    for (i=l-1; i>=0; --l)
      ungetc(tmpsz[i],f);
    return(evar());
  }

  estr data;
  int size=*(uint32_t*)tmpsz;
  l=read(data,size);
  if (l!=size) { lerror("reading variable: expected length: "+estr(size)+" got: "+estr(l)); return(evar()); }
  evar var;
  var.unserial(data);
  return(var);
}

int efile::write(const evar& var) const
{
  estr data;
  // reserving space for unsigned int length specifier
  data.reserve(4);
  data._strlen=4;

  var.serial(data); 
  *(uint32_t*)data._str=(data.len()-sizeof(uint32_t)); // data packet length
  return(write(data));
}

int efile::write(const estr& str) const
{
  if (f==0x00){
    mode="w";
    if (!open()) return(0);
  }

  int len=fwrite(str._str,sizeof(char),str.len(),f);
  return(len);
}

int efile::read(estr& str,long int len) const
{
  if (f==0x00){
    mode="r";
    if (!open()) return(false);
  }

  if (len==-1)
    len=size();

  if (len==-1)
    len=1024;
   
  str.reserve(str.len()+len);
  len=fread(&str._str[str.len()],sizeof(char),len,f);
  if (len>0){
    str._strlen+=len;
    str._str[str._strlen]=0x00;
    return(len);
  }
  str._str[0]=0x00;
  str._strlen=0;
  return(0);
}

/*
bool efile::readstr(estr& str,const estr& limit)
{
  if (f==0x00 && !open("r")) return(false);

  int len;

  char *res;
  len=1023;
   
  str.reserve(len);
  res=fgets(str._str,len,f);
  if (res){
    str._strlen=strlen(str._str);
    if (str.len()>1 && str[str.len()-1]=='\n')
      { str[str.len()-1]=0x00; --str._strlen; }
    else if (str.len()>2 && str[str.len()-2]=='\r' && str[str.len()-1]=='\n')
      { str[str.len()-2]=0x00; str._strlen-=2; }
    return(true);
  }
  str._str[0]=0x00;
  str._strlen=0;
  return(false);
}
*/

/*
extern BOOL GetStdioPathW(
  int id,
  wchar_t *pwszBuf,
  DWORD *lpdwLen
);
*/

bool efile::readln(estr& str) const
{
  if (f==0x00){
    mode="r";
    if (!open()) return(false);
  }

  long int len;
  char *res;
  int p;

  len=1024;
   
  str.reserve(len);
  str._str[0]=0x00;
  str._strlen=0;

  p=0;
  do {
    str.reserve(len);
    linfo("getting string");
    if (!blocking && fileno() == ::fileno(stdin)){
      #ifdef _MSC_VER
       getSystem()->wait(GetStdHandle(STD_INPUT_HANDLE));
      #else
       getSystem()->wait(::fileno(stdin));
      #endif
    }
    res=fgets(&str._str[p],len-p,f);
    p=strlen(str._str);
    linfo("got string");
    if (!res) {
      if (feof(f))
        return(false);
      str._str[0]=0x00;
      str._strlen=0;
      return(false);
    }

    str._strlen=strlen(str._str);
    if (str._strlen>=1 && str[str._strlen-1]=='\n')
      { str[str._strlen-1]=0x00; --str._strlen; return(true); }
    else if (str._strlen>=2 && str[str._strlen-2]=='\r' && str[str._strlen-1]=='\n')
      { str[str._strlen-2]=0x00; str._strlen-=2; return(true); }
    len=len*2;
  }while (1);

/*
  linfo("getting string");
  res=fgets(str._str,len,f);
  linfo("got string");
  if (res){
    str._strlen=strlen(str._str);
    if (str.len()>=1 && str[str.len()-1]=='\n')
      { str[str.len()-1]=0x00; --str._strlen; }
    else if (str.len()>=2 && str[str.len()-2]=='\r' && str[str.len()-1]=='\n')
      { str[str.len()-2]=0x00; str._strlen-=2; }
    return(true);
  }
  str._str[0]=0x00;
  str._strlen=0;
  lwarn("error reading: "+estr(ferror(f))+" errno: "+estr(errno));
*/
  return(false);
}


void efile::setNonBlocking() const
{
  fcntl(fileno(),F_SETFL,O_NONBLOCK);
}


efile popen(const estr& cmd)
{
  FILE* res=popen(cmd._str,"r+");
  lwarnif(res==0x00,"error in popen command: "+cmd);
  return(res);
}

void pclose(efile& file)
{
  pclose(file.f);
}

int system(const estr& cmd)
{
  return(system(cmd._str));
}







/*
int efile_size(const estr &filename)
{
  struct stat sb;
  int r;

  r=stat(filename._str,&sb);

  if (!r) return(sb.st_size);
  return(-1);
}

bool efile_exists(const estr &filename)
{
  struct stat sb;
  int r;

  r=stat(filename._str,&sb);

  if (!r) return(true);
  return(false);
}


estr efile_load(const estr &filename)
{
  estr data;
  ldtip("efile_load(filename,result) is the recommended way to load all data from a file");
  efile_load(filename,data);
  return(data);
}

int efile_load(const estr& filename, estr& buffer)
{
  FILE *f;

//  if (filename == "-")
//    return(load_pipe(stdin,buffer));

  int len;
  int szlen;
  
  len=efile_size(filename);

  f=fopen(filename._str,"r");

  dieif(f==0x00,"error: unable to open file: "+filename);
  
  buffer._checkNewSize(len+1);
  buffer._strlen = len;
  buffer._str[len]=0x00;

  szlen = fread(buffer._str,sizeof(char),len,f);

  dieif(szlen!=len,"load_file: size mismatch reading file");

  fclose(f);

  return(szlen);
}

void efile_save(const estr& filename,const estr& buffer)
{
  FILE *f;

  int szlen;
  
  f=fopen(filename._str,"w");

  dieif(f==0x00,"error: unable to open file: "+filename);
  
  szlen = fwrite(buffer._str,sizeof(char),buffer.len(),f);

  dieif(szlen!=buffer.len(),"efile_save: size mismatch writing file");

  fclose(f);
}

void efile_read(FILE *f,estr& buffer)
{
  int len;
  int szlen;

  len=1023;
  buffer._checkNewSize(len+1);

  szlen = fread(buffer._str,sizeof(char),len,f);

  buffer._strlen = szlen;
  buffer._str[szlen]=0x00;
}

void efile_gets(FILE *f,estr& buffer)
{
  int len;
  char* szres;

  len=1023;
  buffer._checkNewSize(len+1);

  szres = fgets(buffer._str,len,f);

  if (szres){
    buffer._strlen = strlen(buffer._str);
    return;
  }

  buffer._str[0]=0x00;
  buffer._strlen = 0;
}
*/




/*

eafileptr::eafileptr(): fd(0),buffer(0x00),bi(0),be(0),bsize(0)
{
}

eafileptr::eafileptr(estr filename)
{
  open(filename);
}

#ifndef O_LARGEFILE
#define O_LARGEFILE 0x00
#endif

void eafileptr::open(estr filename)
{
  fd=::open(filename._str,O_RDONLY | O_LARGEFILE);
  buffer=new char[BUFFER_SIZE];
  bsize =BUFFER_SIZE;
  bi    =READ_SIZE;
  be    =READ_SIZE;    
  _eof  =false;

  int rlen;

  rlen = read(fd,buffer + bi,READ_SIZE);
  if (rlen > 0) be=(bi+rlen)%bsize;
  else if (rlen==0) _eof=true;
  ldieif(rlen < 0,"eafileptr, error reading first bytes of file");
}

eafileptr::~eafileptr()
{
  close(fd);
  delete buffer;
}

bool eafileptr::operator++()
{
  int rlen;

  ++bi;
  bi=bi%bsize;
  if (bi==be){
    rlen = read(fd,buffer+bi,READ_SIZE);
    if (rlen > 0) be=(bi+rlen)%bsize; else if (rlen==0) {_eof=true;return(false);}
    ldieif(be != 0 && be != READ_SIZE && read(fd,buffer+bi,1),"eafileptr::operator++, \"be\" out of bounds");
    return(true);
  }
  return(true);  
}

estr eafileptr::getline()
{
  estr tmp;

  while (!eof() && 0x0A != *(*this) && 0x0D != *(*this) )
    { tmp+=*(*this); ++(*this); }

  if (!eof() && *(*this) == 0x0D) ++(*this);
  if (!eof() && *(*this) == 0x0A) ++(*this);

  return(tmp); 
}

*/
