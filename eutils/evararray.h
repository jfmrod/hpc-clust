#ifndef EVARARRAY_H
#define EVARARRAY_H

#include "eutils.h"

#include "earray_dec.h"
#include "estrarrayof.h"

//#include "evar.h"

class evar;

typedef earray<evar> evararray;
typedef estrarrayof<evar> esvararray;

/*
class evararray : public earray<evar>
{
};
*/

#endif

