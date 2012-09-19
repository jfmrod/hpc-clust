#ifndef ESTRARRAY_H
#define ESTRARRAY_H

#include "eutils.h"

//#include "estr.h"
#include "earrayof_dec.h"

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
  estrarray(char **str,int n);

  estr& operator[](const estr& key);
  const estr& operator[](const estr& key) const;
 
  estr& operator[](int i);
  const estr& operator[](int i) const;

  estr join(const estr& fseparator,const estr& vseparator) const;
  void split(const estr& str,const estr& fseparator,const estr& vseparator);

  void load(const estr& filename);
  void save(const estr& filename);

  estrarray subset(int i,int l=-1) const;
 
  estr implode(const estr& seperator,int i=0,int len=-1) const;
  estrarray ffind(const estrarray& needle,int i=0) const;
};


#endif

