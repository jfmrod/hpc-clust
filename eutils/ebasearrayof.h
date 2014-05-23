#ifndef EBASEARRAYOF_H
#define EBASEARRAYOF_H

#include <iostream>
using namespace std;

class estr;
class evar;

class ebasearrayof
{
 public:
   void serial(estr& data) const;
   size_t unserial(const estr& data,size_t i=0);

   virtual size_t size() const=0;
   virtual void addvar(evar& key,evar& var)=0;
   virtual evar getvarByKey(const evar& key) const=0;
   virtual evar getvar(size_t i) const=0;
   virtual evar getvarkey(size_t i) const=0;
};

ostream& operator<<(ostream& stream,const ebasearrayof& arr);

#endif

