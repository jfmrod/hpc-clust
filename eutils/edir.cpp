#include "edir.h"

#include "logger.h"

#include <sys/types.h>

#ifndef _MSC_VER
 #include <unistd.h>
#else
 #include <direct.h>
#endif

#ifndef _WIN32
 #include <dirent.h>
#endif

#include "estrarray.h"

#ifndef _WIN32
extern char **environ;
#endif

#include "evar.h"

ostream& operator<<(ostream& stream,const edir& dir)
{
  int i;
  stream << "[edir name: " << dir.name << endl;
  estr tmpstr;
  for (i=0; i<dir.dirs.size(); ++i)
    tmpstr+=dir.dirs.keys(i)+"/,\n";
  for (i=0; i<dir.files.size(); ++i)
    tmpstr+=dir.files.keys(i)+",\n";
  tmpstr.del(-2);
  stream << tmpstr << endl << "]" << endl;
  return(stream);
}

/*
ediritem::ediritem() {}
ediritem::ediritem(const ediritem& diritem): name(diritem.name), type(diritem.type) {}
#ifdef _WIN32
ediritem::ediritem(WIN32_FIND_DATA& ditem): name(ditem.cFileName),type(ditem.dwFileAttributes)
{}
#else
ediritem::ediritem(struct dirent* ditem): name(ditem->d_name),type(ditem->d_type){ }
#endif

bool ediritem::isDir() const
{
#ifdef _WIN32
  if (type&FILE_ATTRIBUTE_DIRECTORY) return(true);
#else
  if (type==DT_DIR) return(true);
#endif
  return(false);
}

bool ediritem::isArchive() const
{
#ifdef _WIN32
  if (!(type&FILE_ATTRIBUTE_DIRECTORY) && (type&FILE_ATTRIBUTE_ARCHIVE) || (type&&FILE_ATTRIBUTE_NORMAL)) return(true);
#else
  if (type==DT_REG) return(true);
#endif
  return(false);
}
*/


edir::edir() {}
edir::edir(const estr& dirname): name(dirname)
{
//  open(dirname);
}

void edir::open(const estr& dirname)
{
  if (dirname.len())
    name=dirname;

#ifdef _WIN32
  WIN32_FIND_DATA FindFileData;
  HANDLE hFind;
//  char tmpsz[1024];

  bool more;
  int len;

  FILE *teste;

  
  more=true;
  hFind = FindFirstFile(L"*", &FindFileData);
  while (hFind != INVALID_HANDLE_VALUE && more) 
  {
//    len=wcslen(FindFileData.cFileName);
//    wcstombs(tmpsz,FindFileData.cFileName,len);
//    tmpsz[len] =0x00;
//    printf ("The first file found is %s\n", tmpsz);
    add(ediritem(FindFileData));
    more = FindNextFile(hFind,&FindFileData);
  } 
  FindClose(hFind);
#else
  DIR* dhandle;

  dhandle = opendir(name._str);
  if (!dhandle) { lwarn("unable to open dir name: "+dirname); return; }

  struct dirent* ditem;

  while (ditem=readdir(dhandle)){
    if (ditem->d_type&DT_DIR)
      dirs.add(ditem->d_name,edir(ditem->d_name));
    else
      files.add(ditem->d_name,efile(ditem->d_name));
  }
  closedir(dhandle);
#endif
}


edir ls(const estr& dirname)
{
  edir dir;
  dir.open(dirname);
  return(dir);
}

void cd(const estr& dirname)
{
#ifndef __MINGW32CE__
  if (dirname=="~"){
    lerrorif(chdir(env()["HOME"]._str)!=0,"changing directory");
  }else{
    lerrorif(chdir(dirname._str)!=0,"changing directory");
  }
#endif
}

estr pwd()
{
#ifndef __MINGW32CE__
  char tmpsz[1024];
  if (getcwd(tmpsz,1024)==0x00) return(estr());
  return(estr(tmpsz));
#else
  return(estr(""));
#endif
}


#ifdef __APPLE__
 /* environ is not defined for shared libraries on OSX. therefore we need _NSGetEnviron */
 #include <crt_externs.h>
#endif

estrarray env()
{
  estrarray tmpenv;
#ifndef __MINGW32CE__
  #ifdef __APPLE__
    char** penv=*_NSGetEnviron();
  #else
    char** penv=environ;
  #endif
  int i;
  estr tmps;
  while (*penv){
    tmps=*penv;
    i=tmps.find("=");
    tmpenv.add(tmps.substr(0,i),tmps.substr(i+1));
    ++penv;
  }
#endif
  return(tmpenv);
}

