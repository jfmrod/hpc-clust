#include <eutils/emain.h>
#include <eutils/etest.h>

int emain()
{
  etest& test(getTest());
  test.run();
  return(0);
}
