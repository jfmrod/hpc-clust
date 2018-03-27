#ifndef ESTRARRAY_H
#define ESTRARRAY_H

#include "eutils.h"

//#include "estr.h"
#include "earrayof_dec.h"

#include "eregexp.h"

class estr;
class erow;

class estrarray : public earrayof<estr,estr>
{
 public:
  estrarray();
  estrarray(const estrarray& strarr);
//  estrarray(const ebasicarray<estr>& strarr);
  estrarray(const estr& str);
  estrarray(const char *str);
  estrarray(char **str,size_t n);

  estrarray operator[](const eintarray& iarr) const;

  estr& operator[](const estr& key);
  const estr& operator[](const estr& key) const;
 
  estr& operator[](size_t i);
  const estr& operator[](size_t i) const;

  estr join(const estr& fseparator,const estr& vseparator) const;
  void split(const estr& str,const estr& fseparator,const estr& vseparator);

  void load(const estr& filename);
  void save(const estr& filename);

  size_t ifind(const estr& value,size_t i=0);
  size_t ifindkey(const estr& key,size_t i=0);
  
  size_t refind(const eregexp& re,size_t i=0);
  size_t refindkey(const eregexp& re,size_t i=0);

  eintarray ifindall(const estr& value);
  eintarray ifindallkey(const estr& key);
  eintarray refindall(const estr& value);
  eintarray refindallkey(const estr& key);

  estrarray subset(long i,long l=-1) const;
 
  estr implode(const estr& separator,size_t i=0,size_t len=-1) const;
  estrarray ffind(const estrarray& needle,size_t i=0) const;
};


#endif

