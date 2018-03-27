#ifndef ETIME_H
#define ETIME_H

#include "estr.h"
#include <time.h>

estr date();
time_t unixtime();

estr utime2date(time_t time);

#endif

