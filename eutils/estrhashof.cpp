
#include "estrhashof.h"

#include "estr.h"
#include "ehashfunc.h"

template <>
void estrhashof<evar>::addvar(evar& evarkey,evar& var)
{
  add(evarkey.get<estr>(),var);
}
