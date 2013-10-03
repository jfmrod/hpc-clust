#include "ecgi.h"
#include "edir.h"
#include "logger.h"
#include "ehtml.h"


void ecgi::sendHeaders()
{
  cout << headers.join("\r\n",": ") << "\r\n\r\n";
  sentHeaders=true;
}

void ecgi::init()
{
  estrarray sarr=env();
  int i;
  for (i=0; i<sarr.size(); ++i)
    SERVER.add(sarr.keys(i),sarr.values(i));

  lerrorif(!SERVER.exists("QUERY_STRING"),"missing QUERY_STRING");
  sarr.clear();
  sarr.split(SERVER["QUERY_STRING"],"&","=");
  for (i=0; i<sarr.size(); ++i)
    GET.add(urldecode(sarr.keys(i)),urldecode(sarr.values(i)));

  headers["Content-type"] = "text/html";
}

ecgi::ecgi(): sentHeaders(false) {}
ecgi::~ecgi()
{
  if (!sentHeaders) sendHeaders();
}


