#include "efunc.h"
#include "logger.h"

efunc::efunc(evar& obj,eclassMethodBase *pmethod,const evararray& args): fReturn(0x00), defargs(args)
{
  func = new efuncObjMethod(obj,pmethod);
  ++func->pcount;

  updateInfo();
}

efunc::efunc(const evar& obj,eclassMethodBase *pmethod,const evararray& args): fReturn(0x00), defargs(args)
{
  func = new efuncObjMethod(obj,pmethod);
  ++func->pcount;

  updateInfo();
}

void efunc::setDefaultArgs(evararray& args)
{
  int i;
  if (!defargs.size() || args.size()+defargs.size()<fArgs.size()) return;

  for (i=args.size()+defargs.size()-fArgs.size(); i<defargs.size(); ++i)
    args.add(defargs[i]);
}

evar efunc::call(evararray& args)
{
  if (!func) { ldinfo("efunc: attempting to call unassigned function"); return(evar()); }
  setDefaultArgs(args);
  return(func->call(args));
}

evar efunc::call(const evararray& args)
{
  if (!func) { ldinfo("efunc: attempting to call unassigned function"); return(evar()); }
  evararray tmpargs(args);
  setDefaultArgs(tmpargs);
  return(func->call(tmpargs));
}

evar efunc::call(const estr& args)
{
  if (!func) { ldinfo("efunc: attempting to call unassigned function"); return(evar()); }

  evararray arr;
  evarSplitArgs(arr,args);
  setDefaultArgs(arr);
  return(call(arr));
}

evar efunc::operator()()
{
  return(call(evararray()));
}

evar efunc::operator()(const evararray& _args)
{
  return(call(_args));
}

evar efunc::operator()(evararray& _args)
{
  return(call(_args));
}

efunc& efunc::setFunc(efuncBase* _func)
{
  clear();
  func = _func;
  if (func)
    ++func->pcount;
  updateInfo();

  return(*this);
}

efunc& efunc::operator=(const efunc& _func)
{
  clear();
  func = _func.func;
  defargs=_func.defargs;
  if (func)
    ++func->pcount;
  updateInfo();

  return(*this);
}

void efunc::clear()
{
  if (func){
    --func->pcount;
    if (!func->pcount)
      delete func;
    func=0x00;
  }
}

efunc::efunc(): func(0x00),fReturn(0x00) {}

efunc::efunc(const efunc& _func): func(0x00),fReturn(0x00)
{
  operator=(_func);
}

efunc::~efunc()
{
  clear();
}


void efunc::updateInfo()
{
  fArgs.clear();
  if (func)
    func->updateInfo(this);
}

ostream& operator<<(ostream& stream,const efunc& func)
{
  if (func.fReturn==0x00)
    stream << "void ";
  else
    stream << func.fReturn->name() <<" ";

  stream << "(*)(";
  int i;
  for (i=0; i<func.fArgs.size()-1; ++i)
    stream << func.fArgs[i]->name() <<",";
  if (i<func.fArgs.size())
    stream << func.fArgs[i]->name();
  stream <<")";
  return(stream);
}
