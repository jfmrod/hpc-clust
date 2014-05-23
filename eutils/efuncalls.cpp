#include "efuncalls.h"

evar efuncall(void (*pfunc)(),const evararray& arr)
{
  (*pfunc)();
  return(evar());
}

evar efuncall(evar (*pfunc)(),const evararray& arr)
{
  return(evar((*pfunc)()));
}

