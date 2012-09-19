#ifndef EHTTP_H
#define EHTTP_H

#include "esocket.h"
#include "estr.h"


class ehttp //: public esocket
{
 public: 
  esocket socket;

  efunc onDone;

  int protstate;

  estr url;
  estr location;
  estr hostname;

  estrarray sendHeaders;
  estrarray headers;
  estr header;
  estr data;
  int datalen;
  int chunklen;
  int nextchunk;

  estr retversion;
  estr retstrcode;
  int  retcode;
  
  bool get(const estr& url);
  void wait();

  void clear();

  void doRecv();
  void doClose();
  void doRequestComplete();
};

estr json_serialize(const evar& var);
int json_unserialize(evar var,const estr& data,int i=0);

estr json2_serialize(const evar& var);
int json2_unserialize(evar& var,const estr& data,int i=0);

#endif

