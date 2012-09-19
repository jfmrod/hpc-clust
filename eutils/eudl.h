#ifndef EUDL_H
#define EUDL_H

#include "eutils.h"

#include "estr.h"
#include "emysql.h"
#include "ecsv.h"


class eudl
{
 public:
  eudl(const estr& udl);

  estr location;
  estr protocol;
  estr resource;
  estr fields;
};


etable &udl_load(const estr& udl);

// abstract interface class
//   what all data interfaces *must* have
class edata
{
 public:
  virtual etable& load(const eudl& udl)=0;
  virtual ~edata() {}
// this also *must* be declared and added to the array at the bottom for each class
//  static  edataif *accept_dl(const estr& udl)=0;
};


class edata_mysql : public emresult,public edata
{
  emysql mysql;
 public:
  etable& load(const eudl& udl);
  static edata *accept_dl(const eudl& udl);

  virtual ~edata_mysql() {}
};
  
class edata_csv : public ecsv,public edata
{
 public:
  etable& load(const eudl& udl);
  static edata *accept_dl(const eudl& udl);

  virtual ~edata_csv() {}
};

typedef edata*(*dataif_accept_func_t)(const eudl&);

const dataif_accept_func_t dataif_accept_funcs[] = {
  edata_mysql::accept_dl ,
  edata_csv::accept_dl   ,
  0x00
};

#endif

