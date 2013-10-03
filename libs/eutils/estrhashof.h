#ifndef ESTRHASHOF_H
#define ESTRHASHOF_H

#include "eutils.h"

#include "ehashmap_dec.h"
#include "ebasichashmap_dec.h"
#include "evar_dec.h"

#include "ehashfunc.h"

#include "estr.h"

inline unsigned int hash_oaat_estr(const estr& str)
{ return(hash_oaat(str._str,str.len())); }

inline unsigned int hash_lookup3_estr(const estr& str)
{ return(hash_lookup3(str._str,str.len(),0x75483842)); }

inline unsigned int hash_sf_estr(const estr& str)
{ return(hash_sf(str._str,str.len())); }


template <class T,unsigned int (*hashfunc)(const estr& str)=hash_oaat_estr>
class estrhashof : public ehashmap<estr,T,hashfunc>
{
 public:
  void addvar(evar& evarkey,evar& var);
};

template <class T,unsigned int (*hashfunc)(const estr& str)=hash_lookup3_estr>
class ebasicstrhashof : public ebasichashmap<estr,T,hashfunc>
{
 public:
//  void addvar(evar& evarkey,evar& var);
};

//#include "ehashfunc.h"


template <class T,unsigned int (*hashfunc)(const estr& str)>
void estrhashof<T,hashfunc>::addvar(evar& evarkey,evar& var)
{
  addref(evarkey.get<estr>(),&var.get<T>());
}

//template <class T,unsigned int (*hashfunc)(const estr& str)>
//void estrhashof<evar,hashfunc>::addvar(evar& evarkey,evar& var);




template <class T,unsigned int (*hashfunc)(const estr& str)>
ostream& operator<<(ostream& stream,const estrhashof<T,hashfunc>& strhash)
{
  int i;
  stream <<"{"<<endl;
  for (i=0; i<strhash.size()-1; ++i)
    stream << " "<<strhash.keys(i)<<"="<<(T&)strhash.values(i)<<","<<endl;
  if (strhash.size())
    stream << " "<<strhash.keys(i)<<"="<<(T&)strhash.values(i)<<endl;
  stream << "}"<<endl;
  return(stream);
}   

template <class T,unsigned int (*hashfunc)(const estr& str)>
ostream& operator<<(ostream& stream,const ebasicstrhashof<T,hashfunc>& strhash)
{
  typename ebasicstrhashof<T,hashfunc>::iter i;
  stream <<"{"<<endl;
  for (i=strhash.begin(); i!=strhash.end(); ++i)
    stream << " "<<i.key()<<"="<<(T&)i.value()<<","<<endl;
  stream << "}"<<endl;
  return(stream);
}   


#endif

