#include "eiostream.h"
#include <iostream>

using namespace std;



void eredirbuf::full_line ()
{
//  cout << "full_line: " << line << " onOutput.isSet: " << onOutput.isSet() << endl;
  onOutput(line);
  line.clear();
}

int eredirbuf::overflow (int c)
{
  if (!onOutput.isSet())
    return(sbuf->sputc(c));

  if (c == '\n')
    full_line();
  else if (c != streambuf::traits_type::eof ())
    line += (char)c;
  return c; // traits_type::eof() if failed
}

int eredirbuf::sync ()
{
  if (line.len())
    full_line();
  return 0; // -1 if failed
}

eredirbuf::eredirbuf(ostream& _stream): s(&_stream),sbuf(_stream.rdbuf())
{
  _stream.rdbuf(this);
}

eredirbuf::~eredirbuf()
{
  s->rdbuf(sbuf);
}

