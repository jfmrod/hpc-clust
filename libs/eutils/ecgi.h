#ifndef ECGI_H
#define ECGI_H

#include "eutils.h"
#include "estrhash.h"

#include <stdio.h>

class ecgi
{
 private:
  bool sentHeaders;
 public:
  estrhash GET;
  estrhash POST;
  estrhash FILES;
  estrhash SERVER;

  estrhash headers;

  ecgi();
  ~ecgi();

  void init();

  void sendHeaders();
  template <class T>
  ostream& operator<<(const T& v);
};

template <class T>
ostream& ecgi::operator<<(const T& v)
{
  if(!sentHeaders)
    sendHeaders();
  return(cout << v);
}


#endif

