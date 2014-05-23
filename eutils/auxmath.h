#ifndef AUXMATH_H
#define AUXMATH_H

#include "eutils.h"

#include <math.h>

const float MPI  = acos(-1.0);
const float MPI2 = MPI/2.0;
const float MRADTODEG = 180.0/MPI;
const float MDEGTORAD = MPI/180.0;

#define DEGTORAD(a) a*MDEGTORAD
#define RADTODEG(a) a*MRADTODEG

#endif
