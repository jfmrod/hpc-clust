#ifndef EMAIN_H
#define EMAIN_H

#include "eutils.h"

#include "eparser.h"

int emain();

int argvc=0;
char **argv=0x00;

int main(int _argvc, char *_argv[])
{
  getParser();
  argvc=_argvc; argv=_argv;
  return(emain());
}

#ifdef _WIN32
int WINAPI WinMain(  HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine,
                     int nCmdShow )
{
  getParser();
//  getParser()->init(_argvc,_argv);
  argvc=0; argv=0x00;
  return(emain());
}
#endif

#define main emain
#define WinMain emain


#endif

