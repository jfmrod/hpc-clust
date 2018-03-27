#include "evar.h"
#include "evararray.h"
#include "evarclass.h"
#include "evartype.h"


/*
evarType<evar>::evarType(evar& _object): object(&_object) {}

evarType<evar>::~evarType() {}

evar evarType<evar>::call(const estr& methodname,evararray& args)
{
  return(object->call(methodname,args).var);
}

evar evarType<evar>::get(const estr& propertyname)
{
  return(object->get(propertyname).var);
}

void evarType<evar>::set(const estr& propertyname,const evar& value)
{
  object->set(propertyname,value);
}

void evarType<evar>::set(const evar& value)
{
  if (!value.var) return;

  if (getClassTypeid() == value.var->getClassTypeid()){
    get() = ((evarType<evar>*)value.var)->get();
    return;
  }

  *object = value;
}

ostream& evarType<evar>::outstream(ostream& stream)
{
  return(stream << *object);
}
*/

