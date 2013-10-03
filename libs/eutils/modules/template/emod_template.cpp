#include <eutils/evar.h>
#include <eutils/eparser.h>

void sayhello()
{
  cout << "Hello world" << endl;
}

extern "C" void eutils_register(eparser* parser)
{
  epregisterFunc3(parser,sayhello,"sayhello");
}
