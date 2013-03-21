#include "ebasicarray.h"

template <>
void ebasicarray<evar>::addvar(evar& var)
{
  add(var);
}

template <>
evar ebasicarray<evar>::getvar(size_t i) const
{
  return(at(i));
}
