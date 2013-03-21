#ifndef EBASEARRAY_H
#define EBASEARRAY_H

#include <iostream>
using namespace std;

class estr;
class evar;
class efile;

class ebasearray
{
 public:
   void save(const estr& file) const;
   void load(const estr& file);

   virtual void serial(estr& data) const;
   virtual size_t unserial(const estr& data,size_t i=0);

   virtual size_t size() const=0;
   virtual void addvar(evar& var)=0;
   virtual evar getvar(size_t i) const=0;
};

ostream& operator<<(ostream& stream,const ebasearray& arr);

#endif

