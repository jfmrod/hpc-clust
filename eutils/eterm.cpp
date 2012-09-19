#include "eterm.h"

#include <iostream>
using namespace std;

#ifdef _WIN32
bool useColor=false;
#else
bool useColor=true;
#endif

void etermUseColor(bool color)
{
  useColor=color;
}

estr etermSetColor(int color,bool bold)
{
  if (!useColor) return("");

  int i1;
  i1=0;
  if (bold) i1=1;

  return("\033["+estr(i1)+";"+estr(color)+"m");
}

estr etermUnsetColor()
{
  if (!useColor) return("");
  return("\033[0m");
}
