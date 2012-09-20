#ifndef EOPTION_H
#define EOPTION_H

#include "estrarrayof.h"
template <class T>
class eoption : public estrarrayof<T>
{
 public:
  int choice;
  int operator=(const estr&);

  T& value();
  estr key();

  estr choices();
  
  eoption();
};

template <class T>
eoption<T>::eoption(): choice(-1) {}


template <class T>
estr eoption<T>::key()
{
  lddieif(choice==-1 || choice>estrarrayof<T>::size(),"no choice or out of bounds: "+estr(choice));
  return(estrarrayof<T>::keys(choice));
}

template <class T>
T& eoption<T>::value()
{
  lddieif(choice==-1 || choice>estrarrayof<T>::size(),"no choice or out of bounds: "+estr(choice));
  return(estrarrayof<T>::values(choice));
}

template <class T>
estr eoption<T>::choices()
{
  if (estrarrayof<T>::size()==0) return("{}");

  estr tmpstr="{";
  int i;
  for (i=0; i<estrarrayof<T>::size(); ++i)
    tmpstr+=estrarrayof<T>::keys(i)+",";
  tmpstr.del(-1);
  tmpstr+="}";
  return(tmpstr);
}

template <class T>
int eoption<T>::operator=(const estr& val)
{
  int i;
  i=estrarrayof<T>::findkey(val);
  if (i!=-1)
    choice=i;
  else
    ldie("option: "+estr(val)+" not found. Valid options are: "+choices());
  return(i);
}


#endif

