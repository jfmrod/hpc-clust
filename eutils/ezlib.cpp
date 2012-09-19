#include "ezlib.h"
#include "logger.h"

#include <zlib.h>

estr inflate(estr& str)
{
  estr outstr;

  z_stream strm;
  strm.next_in=(unsigned char*)str._str;
  strm.avail_in=str._strlen;
  strm.zalloc=Z_NULL;
  strm.zfree=Z_NULL;
  lerrorifr(inflateInit2(&strm,MAX_WBITS+32)!=Z_OK,"Error initializing zlib for inflation",(estr()));

  estr tmpstr;
  tmpstr.reserve(1024);
  strm.next_out=(unsigned char*)tmpstr._str;
  strm.avail_out=1024;


  int status;
  while (strm.avail_in>0 && Z_OK==(status=inflate(&strm,Z_NO_FLUSH)) ){
    if (strm.avail_out==0){
      tmpstr._strlen=1024-strm.avail_out;
      outstr+=tmpstr;
      strm.next_out=(unsigned char*)tmpstr._str;
      strm.avail_out=1024;
    }
  }
  if (status!=Z_STREAM_END){
    lerror("error in inflation: "+estr(status));
    inflateEnd(&strm);
    return(estr());
  }

  tmpstr._strlen=1024-strm.avail_out;
  outstr+=tmpstr;
  inflateEnd(&strm);

  return(outstr);
}

