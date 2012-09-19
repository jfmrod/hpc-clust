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
   int unserial(const estr& data,int i=0);

   virtual unsigned int size() const=0;
   virtual void addvar(evar& key,evar& var)=0;
   virtual evar getvarByKey(const evar& key) const=0;
   virtual evar getvar(int i) const=0;
   virtual evar getvarkey(int i) const=0;
};

ostream& operator<<(ostream& stream,const ebasearrayof& arr);

#endif

