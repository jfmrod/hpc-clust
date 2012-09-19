#include "ebasicarray.h"

template <>
void ebasicarray<evar>::addvar(evar& var)
{
  add(var);
}

template <>
evar ebasicarray<evar>::getvar(int i) const
{
  return(at(i));
}
