#include "efuncalls.h"

evar efuncall(void (*pfunc)(),evararray& arr)
{
  (*pfunc)();
  return(evar());
}

evar efuncall(evar (*pfunc)(),evararray& arr)
{
  return(evar((*pfunc)()));
}

