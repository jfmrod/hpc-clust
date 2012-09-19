#include "estr.h"
#include "earrayof.h"

template <>
void earrayof<evar,evar>::addvar(evar& key,evar& var)
{
  add(key,var);
}

template <>
evar earrayof<evar,evar>::getvar(int i) const
{
  return(values(i));
}

template <>
evar earrayof<evar,evar>::getvarByKey(const evar& var) const
{
  return(earrayof<evar,evar>::at(var));
}

template <>
evar earrayof<evar,evar>::getvarkey(int i) const
{
  return(keys(i));
}
