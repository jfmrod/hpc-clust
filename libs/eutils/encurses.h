#ifndef __ENCURSES__
#define __ENCURSES__

#include "eutils.h"

#include "estr.h"

#ifdef EUTILS_HAVE_LIBNCURSES
 #include <ncurses.h>
#endif

class encurses
{
 public:
  encurses();
  ~encurses();
  void raw();
  void noecho();
  void printw(estr str);
  char getch();
  void setbold();
  void unsetbold();
};

extern encurses ncurses;

#endif

