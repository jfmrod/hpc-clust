#include "evarclass.h"
#include "evarclassmethod.h"

eclassMethodBase::eclassMethodBase(): fReturn(0x00) {}
eclassMethodBase::eclassMethodBase(const evararray& args): fReturn(0x00),defargs(args) {}

void eclassMethodBase::setDefaultArgs(evararray& args)
{
  int i;
  if (!defargs.size() || args.size()+defargs.size()<fArgs.size()) return;

  for (i=args.size()+defargs.size()-fArgs.size(); i<defargs.size(); ++i)
    args.add(defargs[i]);
}

ostream& operator<<(ostream& stream,const eclassMethodBase& method)
{
  if (method.fReturn==0x00)
    stream << "void ";
  else
    stream << method.fReturn->name()<<" ";

  stream << "(*)(";
  int i;
  for (i=0; i<method.fArgs.size()-1; ++i)
    stream << method.fArgs[i]->name() <<",";
  if (i<method.fArgs.size())
    stream << method.fArgs[i]->name() <<")";
  return(stream);
}


