#ifndef _FILES_
#define _FILES_

#include "eutils.h"

#include <stdio.h>

typedef struct _XYINFO {
  double xmin;
  double xmax;
  double ymin;
  double ymax;
} XYINFO;

void skipcomments(FILE *f);

int load_file_x(char *filename, double **buffer);
int load_file_xy(char *filename, double **buffer, double **ybuffer, XYINFO *xyi);


int load_file(char *filename, char **buffer);

#endif

