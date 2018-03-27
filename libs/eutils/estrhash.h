#ifndef ESTRHASH_H
#define ESTRHASH_H

#include "eutils.h"

#include "estrhashof.h"
#include "estr.h"

class estrhash : public estrhashof<estr>
{
 public:
  estr join(const estr& vdelm=";",const estr& fdelm="=");
  void split(const estr& str,const estr& fseparator,const estr& vseparator);

  void load(const estr& filename);
  void save(const estr& filename);
};

#endif

