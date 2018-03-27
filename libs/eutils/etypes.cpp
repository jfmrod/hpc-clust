#include "etypes.h"

#include "logger.h"

#include "estrarray.h"

//int operator=(int &a,etype &var)
//{ return(a = var.i());}
etype operator+(int a,const etype &var)
{ return(etype(a)+var);}
etype operator-(int a,const etype &var)
{ return(etype(a)-var);}
etype operator*(int a,const etype &var)
{ return(etype(a)/var);}
etype operator/(int a,const etype &var)
{ return(etype(a)/var);}


//float operator=(float &a,etype &var)
//{ return(a = var.f());}
etype operator+(float a,const etype &var)
{ return(etype(a)+var);}
etype operator-(float a,const etype &var)
{ return(etype(a)-var);}
etype operator*(float a,const etype &var)
{ return(etype(a)*var);}
etype operator/(float a,const etype &var)
{ return(etype(a)/var);}

//double operator=(double &a,etype &var)
//{ return(a = var.d());}
etype operator+(double a,const etype &var)
{ return(etype(a)+var);}
etype operator-(double a,const etype &var)
{ return(etype(a)-var);}
etype operator*(double a,const etype &var)
{ return(etype(a)*var);}
etype operator/(double a,const etype &var)
{ return(etype(a)/var);}

//estr operator=(estr &a,etype &var)
//{ return(a = var.s());}
etype operator+(estr a,const etype &var)
{ return(etype(a)+var);}

etype operator+(char *a,const etype &var)
{ return(etype(a)+var);}

etype operator+(const estrarray &a,const etype &var)
{ return(etype(a)+var); }


etype::etype(): type(ET_UNDEFINED), value(0x00)
{
 // ldebug("creating undefined etype");
}

etype::~etype()
{
//  ldebug("destroying etype");
  free_type();
}


etype::etype(const etype &var): type(ET_UNDEFINED), value(0x00)
{ set_type(var.type); set_value(var.value); }

etype::etype(int    var): type(ET_UNDEFINED), value(0x00)
{ //ldebug("creating int etype");
  set_type(ET_INT);    *(int*)   value=var; }

etype::etype(float  var): type(ET_UNDEFINED), value(0x00)
{ set_type(ET_FLOAT);  *(float*) value=var; }

etype::etype(double var): type(ET_UNDEFINED), value(0x00)
{ set_type(ET_DOUBLE); *(double*)value=var; }

etype::etype(char   var): type(ET_UNDEFINED), value(0x00)
{ set_type(ET_CHAR);   *(char*)  value=var; }

etype::etype(char  *var): type(ET_UNDEFINED), value(0x00)
{ set_type(ET_ESTR);   *(estr*)  value=var; }

etype::etype(const estr   &var): type(ET_UNDEFINED), value(0x00)
{ set_type(ET_ESTR);   *(estr*)  value=var; }

etype::etype(const estrarray &var): type(ET_UNDEFINED), value(0x00)
{ set_type(ET_EARRAY); *(estrarray*)value=var; }


etype &etype::operator[](const etype &i)
{
  if (type == ET_EARRAY) return((*(estrarray*)value)[i]);

  else if (i==0) return(*this);
  else {
    if (type!=ET_UNDEFINED){
      etype tmpv;
      tmpv = *this;
      set_type(ET_EARRAY);
      (*(estrarray*)value)[0] = tmpv;
    }else
      set_type(ET_EARRAY);

    return( (*(estrarray*)value)[i] );
  }
}

etype &etype::operator=(const etype &var)
{
  set_type(var.type);
  set_value(var.value);
  return(*this);
}

bool etype::operator==(const etype &var) const
{
  if (type == var.type && cmp_value(var.value))
    return(true);
  else if (type == ET_ESTR)
    return( *(estr*)value == var.s() );
  else if (var.type == ET_ESTR)
    return( s() == *(estr*)var.value );
  else if (type == ET_EARRAY || var.type == ET_EARRAY)
    return(false);
  else{
    switch(type){
      case ET_INT: return( s() == var.s() );
      case ET_FLOAT: return( *(float*)value == var.f() );
      case ET_DOUBLE: return( *(double*)value == var.d() );
      case ET_CHAR: return( *(char*)value == var.c() );
    }
  }

  return(false);
}

etype etype::operator+(const etype &var)
{
  switch(type){
    case ET_INT: return(etype( *(int*)value + var.i() ));
    case ET_FLOAT: return(etype( *(float*)value + var.f() ));
    case ET_DOUBLE: return(etype( *(double*)value + var.d() ));
    case ET_CHAR: return(etype( *(char*)value + var.c() ));
    case ET_ESTR: return(etype( *(estr*)value + var.s() ));
    case ET_EARRAY: return(etype( *(estrarray*)value + var ));
  }
  lwarn("undefined value");
  return(estr());
}
 
etype etype::operator-(const etype &var)
{
  switch(type){
    case ET_INT: return(etype( *(int*)value - var.i() ));
    case ET_FLOAT: return(etype( *(float*)value - var.f() ));
    case ET_DOUBLE: return(etype( *(double*)value - var.d() ));
    case ET_CHAR: return(etype( *(char*)value - var.c() ));
    case ET_ESTR: lwarn("subtraction not defined for string type"); return(estr());
    case ET_EARRAY: lwarn("subtraction not defined for array type"); return(estrarray());
  }
  lwarn("undefined value");
  return(estr());
}

etype etype::operator*(const etype &var)
{
  switch(type){
    case ET_INT: return(etype( *(int*)value * var.i() ));
    case ET_FLOAT: return(etype( *(float*)value * var.f() ));
    case ET_DOUBLE: return(etype( *(double*)value * var.d() ));
    case ET_CHAR: return(etype( *(char*)value * var.c() ));
    case ET_ESTR: lwarn("multiplication not defined for string type"); return(estr());
    case ET_EARRAY: lwarn("multiplication not defined for array type"); return(estrarray());
  }
  lwarn("undefined value");
  return(estr());
}

etype etype::operator/(const etype &var)
{
  switch(type){
    case ET_INT: return(etype( *(int*)value / var.i() ));
    case ET_FLOAT: return(etype( *(float*)value / var.f() ));
    case ET_DOUBLE: return(etype( *(double*)value / var.d() ));
    case ET_CHAR: return(etype( *(char*)value / var.c() ));
    case ET_ESTR: lwarn("division not defined for string type"); return(estr());
    case ET_EARRAY: lwarn("division not defined for array type"); return(estrarray());
  }
  lwarn("undefined value");
  return(estr());
}

void etype::free_type()
{
  //ldebug("freeing type");
  switch(type){
    case ET_INT:    delete (int*)value; break;
    case ET_FLOAT:  delete (float*)value; break;
    case ET_DOUBLE: delete (double*)value; break;
    case ET_CHAR:   delete (char*)value; break;
    case ET_ESTR:   delete (estr*)value; break;
    case ET_EARRAY:  delete (estrarray*)value; break;
  }
}

void etype::set_type(evartype _type)
{
  if (type == _type) return;

  free_type();
  
  //ldebug("setting type");
  type=_type;
  switch(type){
    case ET_INT:    value = new int; break;
    case ET_FLOAT:  value = new float; break;
    case ET_DOUBLE: value = new double; break;
    case ET_CHAR:   value = new char; break;
    case ET_ESTR:   value = new estr; break;
    case ET_EARRAY:  value = new estrarray; break;
//   default:
//    lwarn("undefined value");
  }
}

#define COMPARE(a,b,t) *(t*)a == *(t*)b

bool etype::cmp_value(void *_value) const
{
  switch(type){
    case ET_INT: return(COMPARE(value,_value,int));
    case ET_FLOAT: return(COMPARE(value,_value,float));
    case ET_DOUBLE: return(COMPARE(value,_value,double));
    case ET_CHAR: return(COMPARE(value,_value,char));
    case ET_ESTR: return(COMPARE(value,_value,estr));
//    case ET_EARRAY: return(COMPARE(value,_value,estrarray));
   default:
    lwarn("undefined value");	
  }
  return(false);
}

#define ASSIGN(a,b,t) *(t*)a = *(t*)b

void etype::set_value(void *_value)
{
  switch(type){
    case ET_INT: ASSIGN(value,_value,int); break;
    case ET_FLOAT: ASSIGN(value,_value,float); break;
    case ET_DOUBLE: ASSIGN(value,_value,double); break;
    case ET_CHAR: ASSIGN(value,_value,char); break;
    case ET_ESTR: ASSIGN(value,_value,estr); break;
    case ET_EARRAY: ASSIGN(value,_value,estrarray); break;
//   default:
//    lwarn("undefined value");	
  }
}


int etype::i() const
{
  switch(type){
    case ET_INT:    return(*(int*)value);
    case ET_FLOAT:  return((int)*(float*)value);
    case ET_DOUBLE: return((int)*(double*)value);
    case ET_CHAR:   return((int)*(char*)value);
    case ET_ESTR:   return((*(estr*)value).i());
    case ET_EARRAY:  return( (*(estrarray*)value)[0].i() );
  }
  lwarn("undefined value");
  return(0);
}

float etype::f() const
{
  switch(type){
    case ET_INT: return((float)*(int*)value);
    case ET_FLOAT: return(*(float*)value);
    case ET_DOUBLE: return((float)*(double*)value);
    case ET_CHAR:   return((float)*(char*)value);
    case ET_ESTR:   return((*(estr*)value).f());
    case ET_EARRAY:  return( (*(estrarray*)value)[0].f() );
  }
  lwarn("undefined value");
  return(0.0);
}

double etype::d() const
{
  switch(type){
    case ET_INT: return((double)*(int*)value);
    case ET_FLOAT: return((double)*(float*)value);
    case ET_DOUBLE: return(*(double*)value);
    case ET_CHAR:   return((double)*(char*)value);
    case ET_ESTR:   return((*(estr*)value).d());
    case ET_EARRAY:  return( (*(estrarray*)value)[0].d() );
  }
  lwarn("undefined value");
  return(0.0);
}

char etype::c() const
{
  switch(type){
    case ET_INT: return((char)*(int*)value);
    case ET_FLOAT: return((char)*(float*)value);
    case ET_DOUBLE: return((char)*(double*)value);
    case ET_CHAR:   return(*(char*)value);
    case ET_ESTR:   return((*(estr*)value)._str[0]);
    case ET_EARRAY:  return( (*(estrarray*)value)[0].c() );
  }

  lwarn("undefined value");
  return(0x00);
}

estr etype::s() const
{
  switch(type){
    case ET_INT:    return(estr(*(int*)value));
    case ET_FLOAT:  return(estr(*(float*)value));
    case ET_DOUBLE: return(estr(*(double*)value));
    case ET_CHAR:   return(estr(*(char*)value));
    case ET_ESTR:   return(*(estr*)value);
    case ET_EARRAY:  return( (*(estrarray*)value)[0].s() );
  }

  lwarn("undefined value");
  return(0x00);
}

estrarray etype::a() const
{
  switch(type){
    case ET_INT:
    case ET_FLOAT:
    case ET_DOUBLE:
    case ET_CHAR:
    case ET_ESTR:
      return(estrarray(*this));
    case ET_EARRAY:  return(*(estrarray*)value);
  }

  lwarn("undefined value");
  return(estrarray());
}

ostream &operator<<(ostream &stream,const etype &var)
{
  switch(var.type){
    case ET_INT:    return(stream<<(*(int*)var.value));
    case ET_FLOAT:  return(stream<<(*(float*)var.value));
    case ET_DOUBLE: return(stream<<(*(double*)var.value));
    case ET_CHAR:   return(stream<<(*(char*)var.value));
    case ET_ESTR:   return(stream<<(*(estr*)var.value));
    case ET_EARRAY: return(stream<<(*(estrarray*)var.value));
  }

  stream<<" (etype: undefined) ";
  return(stream);
}


/*
etype load_cvs(fstream &f)
{

}

etype load(etype &filename)
{
  fstream f;
  etype result;

  if (!f.open(filename.s()._str,"r"))
    { lwarn("unable to load file"); return(result); }

  // detect file type, text file, comma seperated values, ...
  // then call apropriate function
  result = load_cvs(f);


  f.close();
  return(result);
}


*/





