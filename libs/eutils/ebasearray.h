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
   virtual int unserial(const estr& data,int i=0);

   virtual unsigned int size() const=0;
   virtual void addvar(evar& var)=0;
   virtual evar getvar(int i) const=0;
};

ostream& operator<<(ostream& stream,const ebasearray& arr);

#endif

