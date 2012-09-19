#ifndef ECSV_H
#define ECSV_H

#include "eutils.h"

#include "estr.h"
#include "estrarray.h"
#include "etable.h"

class ecsv : public etable
{
//  etable table;
  void parse(estr &data);
 public:
  void load(estr filename);
//  erow &operator[](int i);
//  inline int size() const { return(table.size()); }
};

void ecsv_tokenize(const estr &str, estrarray &tokens);
void emcsv_tokenize_first(const char *str, int strlen, estrarray &tokens);
void emcsv_tokenize_next(const char *str, int strlen, estrarray &tokens);



#endif

