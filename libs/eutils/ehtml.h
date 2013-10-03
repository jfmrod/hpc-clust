#ifndef EHTML_H
#define EHTML_H

#include "evar.h"
#include "earray.h"
#include "estr.h"
#include "estrarrayof.h"

class ehtml;

class ehtmltag
{
 public:
  earray<ehtmltag> parts;
  estr text;

  bool single;
  estr type;
  estrarray args;
  void addText(const estr& text,int i=-1);
  void addTag(const ehtmltag& tag);
  void parse(ehtml& html,const estr& data,int& i);
  void make(estr& odata);

//  ~ehtmltag();
};

class ehtml : public ehtmltag
{
 public:
  ehtmltag *head;
  ehtmltag *body;
  ebasicarray<ehtmltag*> links;
  ebasicarray<ehtmltag*> forms;
  ebasicarray<ehtmltag*> images;
  ebasicarray<ehtmltag*> scripts;
  ebasicarray<ehtmltag*> css;
  estrarrayof<ehtmltag*> tags;

  void parse(const estr& data);
  void load(const estr& filename);
  void save(const estr& filename);
  estr make();

  void clear();
};

estr urldecode(const estr& str);
estr urlencode(const estr& str);

estr html_entities(const estr& str);
estr html_entities_decode(const estr& str);

#endif


