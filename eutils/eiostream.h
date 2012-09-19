#ifndef EIOSTREAM_H
#define EIOSTREAM_H

#include "eutils.h"


class _ecout;

/*
extern _ecout *ecout;
extern _ecout *ecerr;

_ecout *getCout();
_ecout *getCerr();
*/

#include <ostream>
#include <streambuf>
using namespace std;
#include "evar.h"
#include "efunc_dec.h"
#include "estr.h"

class eredirbuf : public streambuf
{
  ostream *s;
  streambuf *sbuf;
  estr line;
  void full_line();
 protected:
  int overflow (int c);
  int sync();
 public:
  efunc onOutput;
  eredirbuf(ostream& stream);
  ~eredirbuf();
};

#endif

