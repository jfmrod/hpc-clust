#include "files.h"

#include <stdio.h>

#ifndef __MINGW32CE__
#include <errno.h>
#endif

#ifndef _MSC_VER
#include <unistd.h>
#endif

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "logger.h"


void skipcomments(FILE *f)
{
  int r;
  char tmpsz[1024];

  do{
    r=fscanf(f,"%[^0-9\n]\n",tmpsz);
    ldieif(r==EOF,"error: no data");
  } while (tmpsz[0]=='#'); 
}


int load_pipe(FILE *file, char **buffer)
{
  int szlen;
  int tmplen;
  int buffer_len;

  szlen=0;
  buffer_len=1024*4;

  *buffer=(char*)malloc(buffer_len*sizeof(char));

  tmplen = fread(*buffer,sizeof(char),buffer_len,file);
  szlen  = tmplen;

  while (szlen==buffer_len && tmplen > 0){
    buffer_len = buffer_len*4;
    *buffer=(char*)realloc(*buffer,buffer_len*sizeof(char));

    tmplen=fread(&(*buffer)[szlen],sizeof(char),buffer_len-szlen,file);
    ldieif(tmplen==-1,"load_pipe: reading from file descriptor");
    szlen+=tmplen;
  }

  return(szlen);
}

int load_file(char *filename, char **buffer)
{
  FILE *f;

  if (!strcmp(filename,"-"))
    return(load_pipe(stdin,buffer));

  f=fopen(filename,"r");

  ldieif(f==0x00,"error: unable to open file");
  
  int len;
  int szlen;
  //try and get size of file
  ldieif(fseek(f,0L,SEEK_END)==-1,"Unable to seek end of file");
  len = ftell(f);

  ldieif(len==-1,"load_file: invalid file descriptor");

  *buffer=(char*)malloc(len*sizeof(char));

  ldieif(*buffer==0x00,"load_file: unable to allocate memory for file");

  fseek(f,0L,SEEK_SET);
  szlen = fread(*buffer,sizeof(char),len,f);

  ldieif(szlen!=len,"load_file: size mismatch reading file");

  fclose(f);

  return(szlen);
}


int load_file_xy(char *filename, double **buffer, double **ybuffer, XYINFO *xyi)
{
  FILE *f;

  if (!strcmp(filename,"-"))
    f=stdin;
  else
    f=fopen(filename,"r");

  ldieif(f==0x00,"error: unable to open file");

  skipcomments(f);

  double xmin,xmax;
  double ymin,ymax;

  int blen;
  int bsize;

  blen=0;
  bsize=1024;

  //TODO: improvement tip, estimate necessary memory size, using lseek
  
  *buffer=(double*)malloc(bsize*sizeof(double));
  *ybuffer=(double*)malloc(bsize*sizeof(double));
  ldieif(*buffer==0x00 || *ybuffer==0x00,"unable to allocate memory");
  
  if (fscanf(f,"%lf",&(*buffer)[0])==EOF || fscanf(f,"%lf",&(*ybuffer)[0])==EOF) { printf("error: no data\n"); return(0x00); }
  
  xmin = (*buffer)[blen]; xmax = xmin;
  ymin = (*ybuffer)[blen]; ymax = ymin;
  blen=1;
  while (fscanf(f,"%lf",&(*buffer)[blen]) != EOF && fscanf(f,"%lf",&(*ybuffer)[blen]) != EOF){
    if ((*buffer)[blen] < xmin) xmin = (*buffer)[blen];
    if ((*buffer)[blen] > xmax) xmax = (*buffer)[blen];
    if ((*ybuffer)[blen] < ymin) ymin = (*ybuffer)[blen];
    if ((*ybuffer)[blen] > ymax) ymax = (*ybuffer)[blen];
    ++blen;

    if (blen>=bsize){
      bsize=2*bsize;
      *buffer=(double*)realloc(*buffer,bsize*sizeof(double));
      *ybuffer=(double*)realloc(*ybuffer,bsize*sizeof(double));
      ldieif(*buffer==0x00 || *ybuffer==0x00,"unable to allocate memory");
    }
  }

  xyi->xmin=xmin;
  xyi->xmax=xmax;
  xyi->ymin=ymin;
  xyi->ymax=ymax;

  if (f!=stdin)
    fclose(f);

  return(blen);
}


int load_file_x(char *filename, double **buffer)
{
  FILE *f;

  if (!strcmp(filename,"-"))
    f=stdin;
  else
    f=fopen(filename,"r");

  ldieif(f==0x00,"error: unable to open file");

  skipcomments(f);

  double xmin,xmax;

  int blen;
  int bsize;

  blen=0;
  bsize=1024;

  //TODO: improvement tip, estimate necessary memory size, using lseek
  
  *buffer=(double*)malloc(bsize*sizeof(double));
  
  if (fscanf(f,"%lf",&(*buffer)[0])==EOF) { printf("error: no data\n"); return(0x00); }
  
  blen=1;
  xmin = (*buffer)[blen]; xmax = xmin;
  while (fscanf(f,"%lf",&(*buffer)[blen]) != EOF){
    if ((*buffer)[blen] < xmin) xmin = (*buffer)[blen];
    if ((*buffer)[blen] > xmax) xmax = (*buffer)[blen];
    ++blen;

    if (blen>=bsize){
      bsize=2*bsize;
      *buffer=(double*)realloc(*buffer,bsize*sizeof(double));
      ldieif(*buffer==0x00,"unable to allocate memory");
    }
  }

  if (f!=stdin)
    fclose(f);

  return(blen);
}


