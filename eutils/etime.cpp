#include "etime.h"

estr date()
{
  time_t tloc;
  time(&tloc);

  char tmpsz[26];
  ctime_r(&tloc,tmpsz);
  tmpsz[24]=0x00;
  return(tmpsz);
}

time_t unixtime()
{
  time_t tloc;
  time(&tloc);
  return(tloc);
}

estr utime2date(time_t utime)
{
  char tmpsz[26];
  ctime_r(&utime,tmpsz);
  tmpsz[24]=0x00;
  return(tmpsz);
}
