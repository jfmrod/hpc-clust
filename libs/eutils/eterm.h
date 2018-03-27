#ifndef __ETERM__
#define __ETERM__

#include "eutils.h"

#include "estr.h"

#define tcBlack       30
#define tcRed         31
#define tcGreen       32
#define tcYellow      33
#define tcBlue        34
#define tcPurple      35
#define tcCyan        36
#define tcWhite       37

/*
#define tcBoldGray    1,30
#define tcBoldRed     1,31
#define tcBoldGreen   1,32
#define tcBoldYellow  1,33
#define tcBoldBlue    1,34
#define tcBoldPurple  1,35
#define tcBoldCyan    1,36
#define tcBoldWhite   1,37
*/

estr etermSetColor(int color,bool bold=false);
estr etermUnsetColor();

void etermUseColor(bool color);


#endif

