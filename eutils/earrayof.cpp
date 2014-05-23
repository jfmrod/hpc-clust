#include "estr.h"
#include "earrayof.h"

template <>
void earrayof<evar,evar>::addvar(evar& key,evar& var)
{
  add(key,var);
}

template <>
evar earrayof<evar,evar>::getvar(size_t i) const
{
  return(values(i));
}

template <>
evar earrayof<evar,evar>::getvarByKey(const evar& var) const
{
  return(earrayof<evar,evar>::at(var));
}

template <>
evar earrayof<evar,evar>::getvarkey(size_t i) const
{
  return(keys(i));
}
