#ifndef EMYSQL_H
#define EMYSQL_H

#include "eutils.h"

#ifdef EUTILS_HAVE_LIBMYSQL
  #include <mysql/mysql.h>
  #include <mysql/mysqld_error.h>
#else
  typedef int MYSQL_RES;
  typedef int MYSQL;
#endif

#include "logger.h"


class emysql;

#include "etable.h"

class emresult : public etable
{
 private:
  emysql *_mysql;
  void fetch_rows(MYSQL_RES *res);
  void fetch_fields(MYSQL_RES *res);

 public:
  emresult();
  ~emresult();
  
  emresult(emysql *mysql,MYSQL_RES *res);

//  erow operator[](int i);
};

class estr;

class emysql
{
 public:
  MYSQL _msql;

  bool connect(const estr& server, const estr& user, const estr& pass, const estr& database,int port=3306);
  
  bool select_db(const estr& database);

  MYSQL_RES select(const estr& tables,const estr& condition,const estr& fields);
  void  insert(const estr& tables,const estrarray& record);
  emresult query(const estr& querystr);

  unsigned int insert_id();
  int field_count();
  estr escape_string(const estr& str);

  estrarray tables();
//  estrarray fields(const estr &table);
  
//  emtable &operator[](char *table);
  unsigned int errnumber();  // for some reason G++ in windows complains about the unsigned part
  estr         error();
  
  emysql();
  ~emysql();
};


#endif

