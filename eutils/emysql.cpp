#include "emysql.h"

#include <stdio.h>

#ifdef WIN32
  #include <windows.h>
#endif


emresult::emresult()
{
}

emresult::~emresult()
{
}

emresult::emresult(emysql *mysql,MYSQL_RES *res): _mysql(mysql)
{
#ifdef EUTILS_HAVE_LIBMYSQL
  if (res){
    fetch_fields(res);
    fetch_rows(res);
    mysql_free_result(res);
  } else {
    if (mysql->errnumber())
      lerror("("+estr(mysql->errnumber())+") "+mysql->error());
    else if (mysql->field_count())
      lerror("empty result");
  }
#else
  lerror("mysql support was not compiled with library");
#endif
}

/*
erow emresult::operator[](int i)
{
  return(row(i));
}
*/



void emresult::fetch_rows(MYSQL_RES *res)
{
#ifdef EUTILS_HAVE_LIBMYSQL
  int i,j,rowcount;
  char **row;
  rowcount=mysql_num_rows(res);
  for (i=0; i<rowcount; ++i){
    row=mysql_fetch_row(res);
    for (j=0; j<cols.size(); ++j)
      add(cols.keys(j),new estr(row[j]));
  }
#else
  lerror("mysql support was not compiled with library");
#endif
}

void emresult::fetch_fields(MYSQL_RES *res)
{
#ifdef EUTILS_HAVE_LIBMYSQL
  int i;  
  int fieldsize;
  MYSQL_FIELD *tfields;

  fieldsize = mysql_num_fields(res);
  tfields   = mysql_fetch_fields(res);
  for (i=0; i<fieldsize; ++i)
    addfield(tfields[i].name);
#else
  lerror("mysql support was not compiled with library");
#endif
}




unsigned int emysql::errnumber()
{
#ifdef EUTILS_HAVE_LIBMYSQL
  return(mysql_errno(&_msql));
#else
  lerror("mysql support was not compiled with library");
  return(0);
#endif
}

estr emysql::error()
{
#ifdef EUTILS_HAVE_LIBMYSQL
  return(mysql_error(&_msql));
#else
  lerror("mysql support was not compiled with library");
  return("");
#endif
}


estrarray emysql::tables()
{
  estrarray tables;
#ifdef EUTILS_HAVE_LIBMYSQL
  emresult res;
  res = query("show tables");

  int i;
  for (i=0; i<res[0].size(); ++i)
    tables.add(res[0][i].get<estr>());
#else
  lerror("mysql support was not compiled with library");
#endif
  return(tables);
}


bool emysql::connect(const estr& server, const estr& user, const estr& pass, const estr& dbase,int port)
{
#ifdef EUTILS_HAVE_LIBMYSQL
  MYSQL *res;
  if (server.len() && server[0]==':')
    res=mysql_real_connect(&_msql,NULL,user._str, pass._str, dbase._str,0,&server._str[1],0);
  else
    res=mysql_real_connect(&_msql,server._str,user._str, pass._str, dbase._str,port,NULL,0);
  if (res==NULL){
    lwarn(mysql_error(&_msql));
    return(false);
  }
  return(true);
#else
  lerror("mysql support was not compiled with library");
  return(false);
#endif
}

bool emysql::select_db(const estr& database)
{
#ifdef EUTILS_HAVE_LIBMYSQL
  return(mysql_select_db(&_msql,database._str)==0);
#else
  lerror("mysql support was not compiled with library");
  return(false);
#endif
}

emresult emysql::query(const estr& querystr)
{
#ifdef EUTILS_HAVE_LIBMYSQL
  int res;
  res=mysql_real_query(&_msql, querystr._str, querystr.len());
  if (res && errnumber()){
    lerror("("+estr(errnumber())+") "+error()+" in query: \""+querystr+"\"");
    return(emresult());
  }
  return(emresult(this,mysql_store_result(&_msql)));
#else
  lerror("mysql support was not compiled with library");
  return(emresult());
#endif
}

unsigned int emysql::insert_id()
{
#ifdef EUTILS_HAVE_LIBMYSQL
  return(mysql_insert_id(&_msql));
#else
  lerror("mysql support was not compiled with library");
  return(0);
#endif
}

int emysql::field_count()
{
#ifdef EUTILS_HAVE_LIBMYSQL
  return(mysql_field_count(&_msql));
#else
  lerror("mysql support was not compiled with library");
  return(0);
#endif
}

estr emysql::escape_string(const estr& str)
{
#ifdef EUTILS_HAVE_LIBMYSQL
  estr tmpstr;
  tmpstr.reserve(str.len()*2+1);
  tmpstr._strlen=mysql_real_escape_string(&_msql, tmpstr._str, str._str, str.len());
  return(tmpstr);
#else
  lerror("mysql support was not compiled with library");
  return("");
#endif
}

emysql::emysql()
{
#ifdef EUTILS_HAVE_LIBMYSQL
  if(mysql_init(&_msql)==NULL)
    lwarn("failed to initialize MySQL library\n");
#else
  lerror("mysql support was not compiled with library");
#endif
}

emysql::~emysql()
{
}

