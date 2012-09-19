#ifndef ECMPSTR_H
#define ECMPSTR_H

#include "eutils.h"

#include "evar.h"
#include "estr.h"
#include "eintarray.h"
#include "estrarray.h"
#include "eregexp.h"

const float ECW_EXACT=1.0;
const float ECW_CASE=0.9;
const float ECW_TYPO=0.7;

class ecmpword
{
 public:
  // straight compare
  bool scCaseSensitive;

  // typo check
  bool typoCheck;
  
  // we can implement some levels of typo checking
  // and do some finer grained tuning of it also
  bool tcHist;
  bool tcSwap;
  bool tcRemaining;

  int   tcHistMaximumTypos;
  float tcHistMaximumTyposPerLetter;

  int   tcSwapMaximumTypos;
  float tcSwapMaximumTyposPerLetter;
  int   tcSwapMaximumDistance;

  float compare(const estr &word1, const estr &word2, float target=0.0);
};

class ecmpphrase
{
 public:
  ecmpword cmpword;

  estr psChars; // " \t\n\r";
  eregexp psCharsRE;
  
  estr piChars; // ",.:;/?()'\"[]{}*&^%$#@!~`_-<>\\|=+";
  eregexp piCharsRE;

  bool wordOrderCompare;
  int   wocSwapMaximumSwaps;
  float wocSwapMaximumSwapsPerWord;
  int   wocSwapMaximumDistance;


  void setSeparationChars(const estr& chars);
  void setIgnoreChars(const estr& chars);

  ecmpphrase();

  float compare(const estr &str1, const estr &str2, float target=0.0);
};


// Match 2 arrays of strings
class ematchstrings
{
 public:
  ecmpword cmpword;
  eintarray match(const estrarray &arr1, const estrarray &arr2);
};


// Match 2 arrays of records, matching first the records fields
template <class T>
class ematchitems
{
 public:
  ematchstrings matchstrings;

  eintarray match(const ebasicarray<T> &arr1, const ebasicarray<T> &arr2);
};


// Global settings for cmpphrase/cmpword/cmpitems
extern ecmpword    eg_cmpword;
extern ecmpphrase  eg_cmpphrase;
extern ematchstrings eg_matchstrings;

// same as ecmpword but provided because it is easier to remember
float     cmpstr(const estr &str1, const estr &str2,float target=0.0);

float     cmpword(const estr &str1, const estr &str2,float target=0.0);
float     cmpphrase(const estr &str1, const estr &str2,float target=0.0);
eintarray matchstrings(const estrarray &arr1, const estrarray &arr2);


#endif

