#ifndef EDIR_H
#define EDIR_H

#include "eutils.h"

#include "estr.h"
#include "earray.h"
#include "efile.h"

#ifdef _WIN32
 #include <windows.h>
#endif

/*
class ediritem
{
 public:
  estr name;

#ifdef _WIN32
  DWORD type;
#else
  int type;
#endif

  bool isDir() const;
  bool isArchive() const;

  ediritem();
  ediritem(const ediritem& diritem);
#ifdef _WIN32
  ediritem(WIN32_FIND_DATA& ditem);
#else
  ediritem(struct dirent* ditem);
#endif
};
*/

class edir
{
 public:
  estr name;
  estrarrayof<edir> dirs;
  estrarrayof<efile> files;

  edir();
  edir(const estr& dirname);

  void open(const estr& dirname="");
};

ostream& operator<<(ostream& stream,const edir& dir);


edir ls(const estr& dirname=".");

void cd(const estr& dirname="~");
estr pwd();

estrarray env();
estr env(const estr& varname);


#endif

