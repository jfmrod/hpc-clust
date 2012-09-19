#include "evarclass.h"

estrhashof<eclassBase>* pclasses=0x00;

estrhashof<eclassBase>& getClasses()
{
  if (pclasses==0x00) pclasses=new estrhashof<eclassBase>;
  return(*pclasses);
}

estrhashof<eclassBase*>* pclassNames=0x00;

estrhashof<eclassBase*>& getClassNames()
{
  if (pclassNames==0x00) pclassNames=new estrhashof<eclassBase*>;
  return(*pclassNames);
}

