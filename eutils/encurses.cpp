
#include "encurses.h"
#include "logger.h"



encurses::encurses()
{
#ifdef EUTILS_HAVE_LIBNCURSES
  initscr();
#else
  lerror("ncurses support was not compiled for library");
#endif
}

encurses::~encurses()
{
#ifdef EUTILS_HAVE_LIBNCURSES
  endwin();
#else
  lerror("ncurses support was not compiled for library");
#endif
}

void encurses::raw()
{
#ifdef EUTILS_HAVE_LIBNCURSES
  ::raw();
#else
  lerror("ncurses support was not compiled for library");
#endif
}

void encurses::noecho()
{
#ifdef EUTILS_HAVE_LIBNCURSES
  ::noecho();
#else
  lerror("ncurses support was not compiled for library");
#endif
}

void encurses::printw(estr str)
{
#ifdef EUTILS_HAVE_LIBNCURSES
  ::printw(str._str);
  ::refresh();
#else
  lerror("ncurses support was not compiled for library");
#endif
}

char encurses::getch()
{
#ifdef EUTILS_HAVE_LIBNCURSES
  return(getch());
#else
  lerror("ncurses support was not compiled for library");
  return(0x00);
#endif
}

void encurses::setbold()
{
#ifdef EUTILS_HAVE_LIBNCURSES
  attron(A_BOLD);
#else
  lerror("ncurses support was not compiled for library");
#endif
}

void encurses::unsetbold()
{
#ifdef EUTILS_HAVE_LIBNCURSES
  attroff(A_BOLD);
#else
  lerror("ncurses support was not compiled for library");
#endif
}

/*
int main()
{	int ch;

	initscr();			
	raw();				
	keypad(stdscr, TRUE);		
	noecho();			

    	printw("Type any character to see it in bold\n");
	ch = getch();
		
	
	if(ch == KEY_F(1))		
		printw("F1 Key pressed");



	else
	{	printw("The pressed key is ");
		attron(A_BOLD);
		printw("%c", ch);
		attroff(A_BOLD);
	}
	refresh();			
    	getch();			
	endwin();			

	return 0;
}

*/

