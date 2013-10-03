#ifndef ETABLE_H
#define ETABLE_H

#include "eutils.h"

#include "evar.h"
#include "evararray.h"
#include "evarclass.h"
#include "estrarrayof.h"

class etable;
class efile;
class evar;
template <class T>
class earray;
typedef earray<evar> evararray;

class erow : public evararray
{
 public:
  int index;
  etable *table;
  
  erow();
  erow(etable *table,int index);

  unsigned size() const;

  evar& operator[](int i);
  evar& operator[](const estr &field);
  const evar& operator[](int i) const;
  const evar &operator[](const estr &field) const;
};

class etable
{
 public:
  estrarrayof<evararray> cols;

  etable();
  etable(const estrarray &fields);
  etable(const etable &table);
  ~etable();
 
  inline int size() const { if(cols.size()==0) return(0); else return(cols[0].size()); }

  etable& operator=(const etable& table);

  void addfield(const estr &field);  
  void setfields(const estrarray &fields);

  evararray& operator[](int i);
  const evararray& operator[](int i) const;
  evararray& operator[](const estr& field);
  const evararray& operator[](const estr& field) const;

  erow row(int i);

  void clear();

  void save(efile file) const;
  void load(efile file);

  void add(int i,const evar& var);
  void add(const estr& field,const evar& var);
  void add(const estrarray& strarr);

  void serial(estr& data) const;
  int unserial(const estr& data,int i);

  etable filter(const estr& expression);
};

#include "eregexp.h"

etable loadWSV(const estr& filename,const eregexp& re=estr("[ \t]"));

ostream& operator<<(ostream& stream,const etable& table);
ostream& operator<<(ostream& stream,const erow& row);

#endif

