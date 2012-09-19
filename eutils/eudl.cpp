#include "eudl.h"

#include "eregexp.h"

/*
eclass eudi
{
 public:
  eudi(estr udi);

  estr protocol;
  estr resource;
  estr fields;
};
*/

eudl::eudl(const estr& udl): location(udl)
{
  protocol = re_match(udl,"^[[:alpha:]]+://");

  // remove protocol part from string
  resource=udl.substr(protocol.len());

  // remove :// from end
  protocol=protocol.substr(0,-4);

  // defaults to "file" protocol
  if (protocol=="") protocol = "file";

  lwarnif(!resource.len(),"resource missing in udl");

  // remove : in beginning of fields (if there are any)  
//  fields = udl.substr(1);

  linfo2("protocol: "+protocol,"eudl");
  linfo2("resource: "+resource,"eudl");
//  linfo("fields: "+fields,"eudl");
}

etable& udl_load(const estr& udl)
{
  dataif_accept_func_t *accept_dl;

  ldtodo("IMPORTANT: free edata objects");

  edata *data;

  for(accept_dl = (dataif_accept_func_t*)dataif_accept_funcs; *accept_dl; ++accept_dl){
    data = (*accept_dl)(udl);
    if (data){
      return(data->load(udl));
    }
  } 
  lwarn2("no data interface found for udl: "+udl,"eudl");
  return(*(new etable()));
}



edata *edata_mysql::accept_dl(const eudl& udl)
{
  if (udl.protocol=="mysql")
    return(new edata_mysql());

  return(0x00);
}

etable& edata_mysql::load(const eudl& udl)
{
  estr host="localhost";
  estr user="test";
  estr pass="";
  estr dbase="test";
  estr mtable;
  estr fieldstr;
  estr tmp;

  tmp=udl.resource;
  user = re_match(tmp,"^.+@");
  // remove from udl
  tmp.del(user.len());
//  udl.resource=udl.resource.substr(user.len());
  // remove @ from user
  user=user.substr(0,-2);

  pass = re_match(user,":.+");
  user=user.substr(0,-1-pass.len());
  pass=pass.substr(1);
 
  host = re_match(tmp,"^[^/]+/");
  tmp.del(host.len());
//  udl.resource=tmp.resource.substr(host.len());
  host=host.substr(0,-2);

  dbase = re_match(tmp,"^[^\\.]+\\.");
  tmp.del(dbase.len());
//  udl.resource=udl.resource.substr(dbase.len());
  dbase=dbase.substr(0,-2);

  mtable= re_match(tmp,"^[^:]+:?");
  tmp.del(mtable.len());
//  udl.resource=udl.resource.substr(mtable.len());
  if (mtable.size() && mtable[ mtable.len()-1 ] == ':')
    mtable=mtable.substr(0,-2);

  fieldstr = tmp;

  ldinfo("user: "+user);
  ldinfo("pass: "+pass);
  ldinfo("host: "+host);
  ldinfo("dbase: "+dbase);
  ldinfo("table: "+mtable);
  ldinfo("fields: "+fieldstr);

  mysql.connect(host,user,pass,dbase);

  if (!fieldstr.size()) fieldstr="*";

  lddieif(!mtable.len(),"no table specified, should lookup some existing table on dbase, maybe show list of tables");

  *(emresult*)this = mysql.query("select "+fieldstr+" from "+mtable);

  ldtodo("check if the mysql connection/query worked");
  return(*this);
}
  


edata *edata_csv::accept_dl(const eudl& udl)
{
  if (udl.protocol=="file" || udl.protocol=="http")
    // check if file is really of type csv
    return(new edata_csv());

  return(0x00);
}

etable &edata_csv::load(const eudl& udl)
{
  ecsv::load(udl.resource);

  ldtodo("check result from loading csv file");
  return(*this);
}
