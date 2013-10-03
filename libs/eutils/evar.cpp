#include "evar_dec.h"

#include "logger.h"
#include "evarclassconverter.h"
#include "evarclass.h"

#include "evar.h"




void serial(const evar& var,estr& data)
{
  var.serial(data);
}
evar unserial(const estr& data)
{
  evar var;
  var.unserial(data);
  return(var);
}


estr evar::getClass() const
{
  if (!var) return("empty");
  return(var->getClass());
}
const type_info& evar::getTypeid() const
{
  if (!isNull())
    return(var->getTypeid());
  return(typeid(void));
}
bool evar::isNull() const
{
  if (!var) return(true);
  return(false);
}

#include <inttypes.h>

int unserialint(unsigned int& v,const estr& data,int i)
{
  if (data.len()<i+sizeof(uint32_t)) return(-1);
  v=*((uint32_t*)&data._str[i]);
  return(i+sizeof(uint32_t));
}

void serialint(unsigned int v,estr &data)
{
  data.reserve(data.len()+sizeof(uint32_t));
  *((uint32_t*)&data[data._strlen])=(uint32_t)v;
  data._strlen+=sizeof(uint32_t);
  data[data._strlen]=0x00;
}

void addpstr(estr &data,const estr& str)
{
  data.reserve(data.len()+sizeof(uint32_t)+str.len());
  *((uint32_t*)&data[data._strlen])=(uint32_t)str.len();
  strcpy(&data[data._strlen+sizeof(uint32_t)],str._str);
  data._strlen+=str.len()+sizeof(uint32_t);
}

int readpstr(estr &value, const estr& data,int i)
{
  if (data.len()<i+sizeof(uint32_t)) { lerror("truncated serial string?"); return(-1); }
  unsigned long len;
  len=*((uint32_t*)&data._str[i]);
  if (data.len()<i+sizeof(uint32_t)+len){ lerror("truncated serial string?"); return(-1); }

  value.reserve(len);
  strncpy(value._str,&data._str[i+sizeof(uint32_t)],len);
  value[len]=0x00;
  value._strlen=len;
  return(i+len+sizeof(uint32_t));
}

void evar::create(const estr& type)
{
  if (!getClasses().exists(type) || !getClasses().values(type).create.size()) { lwarn("unable to create object of class: "+type); return; }

  set(getClasses().values(type).create[0]());
}

int evar::unserial(const estr& data,int i)
{
  estr type;
  ldinfo("unserial: data.len: "+estr(data.len())+" i: "+estr(i));
  i=readpstr(type,data,i);
  if (i==-1) return(-1);
  if (!getClasses().exists(type)) return(i);

  if (getClass()!=type)
    create(type);

  if(var==0x00){ lwarn("unable to create object of type: "+type); return(i); }

  ldinfo("unserial type: "+type);
  if (getClasses().exists(type))
    return(getClasses().values(type).funserial(var,data,i));
  
  return(i);
}

void evar::serial(estr& data) const
{
  addpstr(data,getClass());
  if (!getClasses().exists(getClass())) return;
  getClasses().values(getClass()).fserial(var,data);

/*  
  int i;
  if (getParser()->classes[getClass()].getProperties().size()){
    for (i=0; i<getParser()->classes[getClass()].getProperties().size(); ++i){
      addpstr(data,getParser()->classes[getClass()].getProperties().at(i));
      get(getParser()->classes[getClass()].getProperties().at(i)).serial(data);
    }
  }
*/
}


evar& evar::get() const { return((evar&)*this); }
evar& evar::getarg(void (*pfunc)(evar)) const { return((evar&)*this); }
evar& evar::getarg(void (*pfunc)(const evar&)) const { return((evar&)*this); }
evar& evar::getarg(void (*pfunc)(evar&)) const { return((evar&)*this); }


evar evar::convert(const type_info& ti) const
{
  if (var==0x00) { lwarn("empty var"); throw("trying to get from empty envar"); }

  if (ti==var->getTypeid() || ti == typeid(evar))
    return(*this);

  // if typeid T is one of the parent classes, you can return the value directly

  eclassConverterBase *converter=getConverter(ti,var->getTypeid());
  if (converter==0x00) { lwarn("unable to convert variable from type: "+estr(var->getClass())+" to type: "+estr(ti.name())); return(evar()); }

  return((*converter)( var )); 
}

bool evar::isConvertible(const type_info& ti) const
{
  if (var==0x00) { lwarn("empty var"); throw("trying to get from empty envar"); }

  if (ti==var->getTypeid() || ti==typeid(evar))
    return(true);

  eclassConverterBase *converter=getConverter(ti,var->getTypeid());
  if (converter) return(true);
  return(false);
}




void evarSplitArgs(evararray &arr,const estr& args)
{
  estrarray strarr;
  if (args.len())
    strarr = args.explode(",");
  int i;
  for (i=0; i<strarr.size(); ++i)
    arr.add(new estr(strarr[i]));
}

/*
evar evar::call(const estr& method,const estr& args)
{
  if (!var){ ldinfo("error: evar is not set"); return(evar()); }

  if (var->getClassTypeid() == typeid(evar))
    return( ((evarType<evar>*)var)->object->call(method,args).var );

  evararray arr;
  evarSplitArgs(arr,args); 
  return(var->call(method,arr).var);
}
*/




evar::evar(): var(0x00) {}

evar::evar(evarBase* value): var(0x00)
{
  var=value;
  retain();
}

evar::evar(evar* value): var(0x00)
{
  var=value->var;
  retain();
}

evar::evar(const evar* value): var(0x00)
{
  var=value->var;
  retain();
}

evar::evar(const evar& value): var(0x00)
{
  var=value.var;
  retain();
}

evar::evar(evar& value): var(0x00)
{
  var=value.var;
  retain();
}

evar::evar(const char* value)
{
  var = new evarType<estr>(new estr(value));
  retain();
}


evar::evar(double& value)
{
  var = new evarType<double>(value);
  retain();
}

evar::evar(float& value)
{
  var = new evarType<float>(value);
  retain();
}

evar::evar(unsigned long int& value)
{
  var = new evarType<unsigned long int>(value);
  retain();
}

evar::evar(unsigned int& value)
{
  var = new evarType<unsigned int>(value);
  retain();
}

evar::evar(unsigned char& value)
{
  var = new evarType<unsigned char>(value);
  retain();
}

evar::evar(long int& value)
{
  var = new evarType<long int>(value);
  retain();
}

evar::evar(int& value)
{
  var = new evarType<int>(value);
  retain();
}

evar::evar(char& value)
{
  var = new evarType<char>(value);
  retain();
}

evar::evar(const double& value)
{
  var = new evarType<double>(new double(value));
  retain();
}

evar::evar(const float& value)
{
  var = new evarType<float>(new float(value));
  retain();
}

evar::evar(const unsigned long int& value)
{
  var = new evarType<unsigned long int>(new unsigned long int(value));
  retain();
}

evar::evar(const unsigned int& value)
{
  var = new evarType<unsigned int>(new unsigned int(value));
  retain();
}

evar::evar(const unsigned char& value)
{
  var = new evarType<unsigned char>(new unsigned char(value));
  retain();
}

evar::evar(const long int& value)
{
  var = new evarType<long int>(new long int(value));
  retain();
}

evar::evar(const int& value)
{
  var = new evarType<int>(new int(value));
  retain();
}

evar::evar(const char& value)
{
  var = new evarType<char>(new char(value));
  retain();
}

evar::~evar()
{
  clear();
}

void evar::clear()
{
  release();
  var=0x00;
}

evar& evar::set(const evar& value)
{
  clear();
  var=value.var;
  retain();
  return(*this);
}
evar& evar::copy(const evar& value)
{
  clear();
  if (!getClasses().exists(value.getClass())) return(*this);

  var=getClasses().values(value.getClass()).fcopy(value.var);
  retain();
  return(*this);
}
evar& evar::operator=(evarBase* _var)
{
  clear();
  var=_var;
  retain();
  return(*this);
}
evar& evar::operator=(const char* value)
{
  clear();
  var = new evarType<estr>(*new estr(value));
  retain();
  return(*this);
}

void evar::retain()
{
  if (var)
    ++var->pcount;
}

void evar::release()
{
  if (var){
    --var->pcount;
    if (var->pcount==0)
      delete var;
    var=0x00;
  }
}


eclassMethodBase* findMethod(ebasicarray<eclassMethodBase*>& methods,const evararray& args)
{
  unsigned int i,j;

  if (methods.size()==1)
    return(methods.operator[](0));

  eclassMethodBase *m;

  int score;

  int bestscore;
  int bestmatch;

  bestscore=0;
  bestmatch=-1;
  // TODO: Improve overloaded function matching. we can check if conversion is possible,
  //   and also implement default values for arguments
  for (i=0; i<methods.size(); ++i){
    m=methods.operator[](i);
    score=0;

    ldinfo("evaluating methods: "+estr(i)+"/"+estr((int)methods.size())+" args: "+estr((int)args.size())+" m->fArgs: "+estr((int)m->fArgs.size()));

    if (!args.size() && !m->fArgs.size())
      return(methods.operator[](i));

    if (!args.size() && m->fArgs.size() && m->fArgs.size()-m->defargs.size() == 0)  // there should only be one function with enough default arguments in this case
      return(methods.operator[](i));

    if (m->fArgs.size() && m->fArgs.size() - m->defargs.size() > args.size()) // not enough arguments to call function
      continue;
    
    for (j=0; j<args.size() && j<m->fArgs.size(); ++j){
//      cout << args[j].getClassTypeid().name() << " --- "<<m->fArgs[j]->name()<<endl;
      if ((args[j].getTypeid() == *m->fArgs[j]) || *m->fArgs[j]==typeid(evar)){
        score+=2;
      }else{
        if (!args[j].isConvertible(*m->fArgs[j])){
          score=-1;
          break;
        }else
         ++score;
      }
    }

    ldinfo("evaluating methods: "+estr(i)+"/"+estr((int)methods.size())+" args: "+estr((int)args.size())+" m->fArgs: "+estr((int)m->fArgs.size())+" score: "+estr(score));
    // if function args matches number and exact classes then use this function
    if (max(args.size()*2,(unsigned int)m->fArgs.size()) == score)
      return(methods.operator[](i));

    if (score>bestscore){ bestscore=score; bestmatch=i; }
  }

  if (bestmatch!=-1)
    return(methods.operator[](bestmatch));

  lwarn("did not find any compatible method, using first overloaded method");
  return(methods.operator[](0));
}


evar& evar::operator=(const evar& value)
{
  if (!getClasses().exists(value.getClass())) return(*this);

  if (getTypeid()==value.getTypeid()){
    getClasses().values(getClass()).fassign(var,value.var);
    return(*this);
  }

  if (value.isConvertible(getTypeid())){
    evar conv(value.convert(getTypeid()));
    getClasses().values(getClass()).fassign(var,conv.var);
    return(*this);
  }

  evararray args(value);
  // look for defined "operator=" methods

  if (getClasses().values(getClass()).methods.exists("operator=")){
    eclassMethodBase *bestmatch=findMethod(getClasses().values(getClass()).methods.values("operator="),args);
    (*bestmatch)(var,args);
    return(*this);
  }

  int i;
  for (i=0; i<getClasses().values(var->getClass()).parents.size(); ++i){
    if (getClasses().values(var->getClass()).parents[i]->methods.exists("operator=")){
      evar tmpvar;
      tmpvar=convert(getClasses().values(var->getClass()).parents[i]->getTypeid()).var;
      ldieif(tmpvar.var==0x00,"class is parent but conversion failed?");
      eclassMethodBase *bestmatch=findMethod(getClasses().values(var->getClass()).parents[i]->methods.values("operator="),args);
      (*bestmatch)(tmpvar.var,args);
      return(*this);
    }
  }
  lwarn("operator= does not exist in object of class: "+getClass());
  return(*this);
}


earray<estr> evar::methods() const
{
  if (var==0x00 || !getClasses().exists(var->getClass())) return(earray<estr>());

  earray<estr> arr;
  int i;
  for (i=0; i<getClasses().values(var->getClass()).methods.size(); ++i)
    arr.add(getClasses().values(var->getClass()).methods.keys(i));
  return(arr);
}

earray<estr> evar::properties() const
{
  if (var==0x00 || !getClasses().exists(var->getClass())) return(earray<estr>());

  earray<estr> arr;
  int i;
  for (i=0; i<getClasses().values(var->getClass()).properties.size(); ++i)
    arr.add(getClasses().values(var->getClass()).properties.keys(i));
  return(arr);
}

bool evar::hasProperty(const estr& pname)
{
  if (var==0x00) { lwarn("evar is null"); return(false); }

  if (!getClasses().exists(var->getClass())) { lwarn("class: "+var->getClass()+" not registered"); return(false); }

  if (getClasses().values(var->getClass()).properties.exists(pname))
    return(true);

  int i;
  for (i=0; i<getClasses().values(var->getClass()).parents.size(); ++i){
    if (getClasses().values(var->getClass()).parents[i]->properties.exists(pname))
      return(true);
  }

  return(false);
}

bool evar::hasMethod(const estr& mname)
{
  if (var==0x00) { lwarn("evar is null"); return(false); }

  if (!getClasses().exists(getClass())) { lwarn("class: "+getClass()+" not registered"); return(false); }

  if (getClasses().values(getClass()).methods.exists(mname))
    return(true);

  int i;
  for (i=0; i<getClasses().values(var->getClass()).parents.size(); ++i){
    if (getClasses().values(var->getClass()).parents[i]->methods.exists(mname))
      return(true);
  }
  lwarn("method: \""+mname+"\" does not exist in object of class: "+getClass());
  return(false);
}


evar evar::call(const estr& mname,evararray& args) const
{
  if (var==0x00) { lwarn("evar is null"); return(evar()); }

  if (!getClasses().exists(getClass())) { lwarn("class: "+getClass()+" not registered"); return(evar()); }

  if (getClasses().values(getClass()).methods.exists(mname)){
    eclassMethodBase *bestmatch=findMethod(getClasses().values(getClass()).methods.values(mname),args);
    return((*bestmatch)(var,args));
  }

  int i;
  for (i=0; i<getClasses().values(var->getClass()).parents.size(); ++i){
    if (getClasses().values(var->getClass()).parents[i]->methods.exists(mname)){
      evar tmpvar;
      tmpvar=convert(getClasses().values(var->getClass()).parents[i]->getTypeid()).var;
      ldieif(tmpvar.var==0x00,"class is parent but conversion failed?");
      eclassMethodBase *bestmatch=findMethod(getClasses().values(var->getClass()).parents[i]->methods.values(mname),args);
      return((*bestmatch)(tmpvar.var,args));
    }
  }
  lwarn("method: \""+mname+"\" does not exist in object of class: "+getClass());
  return(evar());
}

evar evar::property(const estr& pname) const
{
  if (var==0x00) { lwarn("evar is null"); return(evar()); }

  if (!getClasses().exists(var->getClass())) { lwarn("class: "+var->getClass()+" not registered"); return(evar()); }

  if (getClasses().values(var->getClass()).properties.exists(pname))
    return(getClasses().values(var->getClass()).properties.values(pname)(var));

  int i;
  for (i=0; i<getClasses().values(var->getClass()).parents.size(); ++i){
    if (getClasses().values(var->getClass()).parents[i]->properties.exists(pname)){
      evar tmpvar;
      tmpvar=convert(getClasses().values(var->getClass()).parents[i]->getTypeid()).var;
      ldieif(tmpvar.var==0x00,"class is parent but conversion failed?");
      return(getClasses().values(var->getClass()).parents[i]->properties.values(pname)(tmpvar.var));
    }
  }

  if (getClasses().values(getClass()).methods.exists(pname))
    return(new efunc(*this,getClasses().values(getClass()).methods.values(pname).at(0)));

  for (i=0; i<getClasses().values(var->getClass()).parents.size(); ++i){
    if (getClasses().values(var->getClass()).parents[i]->methods.exists(pname)){
      evar tmpvar;
      tmpvar=convert(getClasses().values(var->getClass()).parents[i]->getTypeid()).var;
      ldieif(tmpvar.var==0x00,"class is parent but conversion failed?");
      return(new efunc(tmpvar,getClasses().values(var->getClass()).parents[i]->methods.values(pname).at(0)));
    }
  }

  lwarn("property: \""+pname+"\" does not exist in object of class: "+var->getClass());
  return(evar());
}






evar evar::operator()(const evararray& _args)
{
  evararray args(_args);
  return(call("operator()",args));
}

evar& evar::operator-=(const evar& value)
{
  evararray arr;
  arr.add(value);
  call("operator-=",arr);
  return(*this);
}

evar& evar::operator+=(const evar& value)
{
  evararray arr;
  arr.add(value);
  call("operator+=",arr);
  return(*this);
}

evar evar::operator+(const evar& value)
{
  evararray arr;
  arr.add(value);
  return(call("operator+",arr));
}

evar evar::operator-(const evar& value)
{
  evararray arr;
  arr.add(value);
  return(call("operator-",arr));
}

evar evar::operator*(const evar& value)
{
  evararray arr;
  arr.add(value);
  return(call("operator*",arr));
}

evar evar::operator/(const evar& value)
{
  evararray arr;
  arr.add(value);
  return(call("operator/",arr));
}

evar evar::operator%(const evar& value)
{
  evararray arr;
  arr.add(value);
  return(call("operator%",arr));
}

evar& evar::operator++()
{
  evararray arr;
  call("operator++",arr); 
  return(*this);
}

evar& evar::operator--()
{
  evararray arr;
  call("operator--",arr);
  return(*this);
}

bool evar::operator==(const evar& value) const
{
  evararray arr;
  arr.add(value);
  return(((evar*)this)->call("operator==",arr).get<bool>());
}

bool evar::operator<(const evar& value) const
{
  evararray arr;
  arr.add(value);
  return(((evar*)this)->call("operator<",arr).get<bool>());
}
bool evar::operator<=(const evar& value) const
{
  evararray arr;
  arr.add(value);
  return(((evar*)this)->call("operator<=",arr).get<bool>());
}

bool evar::operator>(const evar& value) const
{
  evararray arr;
  arr.add(value);
  return(((evar*)this)->call("operator>",arr).get<bool>());
}
bool evar::operator>=(const evar& value) const
{
  evararray arr;
  arr.add(value);
  return(((evar*)this)->call("operator>=",arr).get<bool>());
}





evar evar::operator[](const evar& value)
{
  evararray arr;
  arr.add(value);
  return(call("operator[]",arr));
}

ostream& operator<<(ostream& stream,const evar& val)
{
  if (val.isNull()) { stream << "[null]"; return(stream); }

  if (!getClasses().exists(val.getClass())) { stream << "<"<<val.getClass()<<">"; return(stream); }

  if (getClasses().values(val.getClass()).hasCout())
    return(getClasses().values(val.getClass()).fcout(stream,val.var));

  int i;
  for (i=0; i<getClasses().values(val.getClass()).parents.size(); ++i){
    if (getClasses().values(val.getClass()).parents[i]->hasCout()){
      evar tmpvar(val.convert(getClasses().values(val.getClass()).parents[i]->getTypeid()));
      ldieif(tmpvar.var==0x00,"class is parent but conversion failed?");
      return(getClasses().values(val.getClass()).parents[i]->fcout(stream,tmpvar.var));
    }
  }
  stream << "<"<<val.getClass()<<">";
  return(stream);
}


