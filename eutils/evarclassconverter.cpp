#include "evarclassconverter.h"
#include "estrhashof.h"

estrhashof< estrhashof< eclassConverterBase > > *pclassConverters=0x00;

estrhashof< estrhashof< eclassConverterBase > >& getConverters()
{
  if (pclassConverters==0x00)
    pclassConverters = new estrhashof< estrhashof< eclassConverterBase > >;
  return(*pclassConverters);
}

void addConverter(const type_info& t1,const type_info& t2,eclassConverterBase* converter)
{
  getConverters();
  if (!pclassConverters->exists(t1.name()))
    pclassConverters->addref(t1.name(), new estrhashof< eclassConverterBase >);
  (*pclassConverters)[t1.name()].addref(t2.name(), converter);
}

eclassConverterBase* getConverter(const type_info& to,const type_info& from)
{
  getConverters();
  if (!pclassConverters->exists(to.name())) return(0x00);
  if (!(*pclassConverters)[to.name()].exists(from.name())) return(0x00);
  return(&(*pclassConverters).values(to.name()).values(from.name()));
}


