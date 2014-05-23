#ifndef ESTRARRAYOF_H
#define ESTRARRAYOF_H

#include "eutils.h"

//#include "estr.h"
#include "earrayof_dec.h"

class estr;

template <class T>
class estrarrayof : public earrayof<T,estr>
{
};

#endif

