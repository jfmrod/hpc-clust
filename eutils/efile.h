#ifndef EFILE_H
#define EFILE_H

#include "eutils.h"

#include "estr.h"
#include <stdio.h>


/*
int efile_size(const estr &filename);

bool efile_exists(const estr &filename);

estr efile_load(const estr &filename);
// More efficient form of passing back all the data
int  efile_load(const estr& filename,estr& buffer);

void efile_save(const estr& filename,const estr& buffer);


void efile_read(FILE *f,estr& buffer);
void efile_gets(FILE *f,estr& buffer);

*/

class evar;


#ifdef EUTILS_HAVE_ZLIB

#include <zlib.h>

class egzfile
{
  gzFile f;
  bool opened;
 public:
  estr name;
  mutable estr mode;
  bool blocking;

  egzfile();
  egzfile(const estr& name,const estr& mode);
  ~egzfile();

  bool open(const estr& filename,const estr& mode);
  bool open();
//  bool open(const estr& mode);
  void close();
  void flush();

  void write(const estr& str);

  bool read(estr& str,long int len=-1);
  bool readln(estr& str);
};

#endif

class ehttpfile
{

};


class efile
{
  mutable FILE* f;
  mutable bool opened;
 public:
  estr name;
  mutable estr mode;
  bool blocking;

  efile();
  ~efile();

  efile(const efile& file);
  efile(const estr& filename);
  efile(const char* filename);
  efile(char* filename);
  efile(const estr& filename,const estr& mode);
  efile(FILE *file);
  efile(int fd,const estr& mode);

  bool open(FILE *file);
  bool open(int fd,const estr& mode);
  bool open() const;
  bool open(const estr& filename,const estr& mode);
//  bool open(const estr& mode);
  void close() const;
  void flush() const;

  void disableBuffer() const;
  void setNonBlocking() const;

  bool create() const;

  estr extension() const;
  estr basename() const;
  estr dirname() const;

  int fileno() const;
  bool eof() const;

  int write(const estr& str) const;
  inline int write(const char *str) const { return(write(estr(str))); }
  inline int write(char *str) const { return(write(estr(str))); }
  inline int write(char chr) const { return(write(estr(chr))); }
  int write(const evar& var) const;

  evar read() const;
  int read(estr& str,long int len=-1) const;
  bool readln(estr& str) const;

  long int size() const;

  bool exists() const;
  friend void pclose(efile& pfile);
  friend ostream& operator<<(ostream& stream,const efile& file);
};

ostream& operator<<(ostream& stream,const efile& file);


efile popen(const estr& cmd);
void pclose(efile& pfile);

int system(const estr& cmd);


/*

// Method for reading really large data
const int BUFFER_SIZE=8192;
const int READ_SIZE=4096;

class eafileptr
{
 private:
  int fd;
  char *buffer;
  int   bi,be;
  int   bsize;
  bool  _eof;

  eafileptr();
 public:
  ~eafileptr();
  eafileptr(estr filename);
//  efileptr(FILE *file);
  eafileptr(int fd);

  void open(estr filename);

  inline bool eof() { return(_eof); }
  inline char operator*(){ return(*(buffer+bi)); }
  bool operator++();

  estr getline();
};
*/

#endif

