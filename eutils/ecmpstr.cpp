#include "ecmpstr.h"
#include "estr.h"
#include "logger.h"
#include "eregexp.h"

#include "earray.h"

#ifdef _MSC_VER
#include <direct.h>
#endif

/*
class ecmpword
  bool scCaseSensitive=false;

  // typo check
  bool typoCheck=true;
  
  bool tcHist=true;
  bool tcSwap=true;
  bool tcRemaining=true;

  int   tcHistMaximumTypos=1;
  float tcHistMaximumTyposPerLetter=0.1;

  int   tcSwapMaximumTypos=1;
  float tcSwapMaximumTyposPerLetter=0.1;
  int   tcSwapMaximumDistance=1;
*/

#undef MIN
#undef MAX

#define ABS(a) ((a)>0?(a):-(a))
#define MIN(a,b) (a>b?b:a)
#define MAX(a,b) (a<b?b:a)


inline int maxs(double a,double b,double c)
{
  if (a>b){
    if (c<a) return(0);
    return(2);
  }
  if (c<b) return(1);
  return(2);
}
inline double maxv(double a,double b,double c)
{
  if (a>b){
    if (c<a) return(a);
    return(c);
  }
  if (c<b) return(b);
  return(c);
}

template <class T>
class eswalign
{
 public:
  int aligned;
  int gaps;
  int matches;
  int mismatches;
  int maxsize;

  float align(const T& a,const T& b,int match=5,int penalty=2,int gap=3);
};

template <class T>
float eswalign<T>::align(const T& a,const T& b,int match,int penalty,int gap)
{
  aligned=0; gaps=0; matches=0; mismatches=0;
  maxsize=MAX(a.size(),b.size());
  if (a.size()==0 || b.size()==0) return(0.0);

  int i,j;
  earray<ebasicarray<double> > F;
//  F.init(a.size());
  for (i=0; i<a.size(); ++i){
    F.add(ebasicarray<double>());
    F[i].init(b.size(),0.0l);
  }
//  double F[a.size()][b.size()];

  F[0][0]=maxv(-gap,-gap,(a[0]==b[0]?match:-penalty));
  for (i=1; i<a.len(); ++i)
    F[i][0]=maxv(F[i-1][0]-gap,-gap-i*gap,-i*gap);
  for (j=1; j<b.len(); ++j)
    F[0][j]=maxv(-gap-j*gap,F[0][j-1]-gap,-j*gap);
  for (i=1; i<a.len(); ++i){
    for (j=1; j<b.len(); ++j)
      F[i][j]=maxv(F[i-1][j]-gap,F[i][j-1]-gap,F[i-1][j-1]+(a[i]==b[j]?match:-penalty));
  }

  i=a.size()-1; j=b.size()-1;
  while (i>0 && j>0){
    switch (maxs(F[i-1][j],F[i][j-1],F[i-1][j-1])){
      case 0: --i; break;
      case 1: --j; break;
      case 2: ++aligned; if (a[i]==b[j]) ++matches; --j; --i; break;
    }
  }
  while (j>0 && i>=0){
    switch (maxs(-gap-(j-1)*gap,F[i][j-1],-(j-1)*gap)){
      case 0: --i; break;
      case 1: --j; break;
      case 2: ++aligned; if (a[i]==b[j]) ++matches; --j; --i; break;
    }
  }
  while (j>=0 && i>0){
    switch (maxs(F[i-1][j],-gap-(i-1)*gap,-(i-1)*gap)){
      case 0: --i; break;
      case 1: --j; break;
      case 2: ++aligned; if (a[i]==b[j]) ++matches; --j; --i; break;
    }
  }

  if (i==0 && j==0){
    ++aligned; if (a[i]==b[j]) ++matches;
  }
  mismatches=aligned-matches;
  gaps=maxsize-aligned;
/*
  cout << "# matches: " << matches << endl;
  cout << "# mismatches: " << mismatches << endl;
  cout << "# gaps: " << gaps << endl;
  cout << "# aligned: " << aligned << endl;
  cout << "# maxsize: " << maxsize << endl;
*/
  return((float)matches/aligned);
}







// auxiliary functions implemented at the end of the file
int strdist(char *str1, char *str2);







// target is used to tune the ammount of computation that goes into a match
//  i.e.: if you are not interested in fuzzy matching like checking for typos
//        you would just put target = 0.9

//        when the compare function reaches the maximum possible match of 0.9 it
//        stops comparing the strings and returns(0.0)

float ecmpword::compare(const estr &word1, const estr &word2,float target)
{
  int i;
  float res;

  res = 1.0;
  // are they the same size?
  if (word1.len()==word2.len()){ 
    // are they exactly equal?
    if (word1==word2) return(1.0);    //1.0

    // is upper/lower case the only difference?
    if (!strncasecmp(word1._str,word2._str,word1.len())) return(0.99); //0.99

    // is there a typo somewhere?
    int typos=0;
    for (i=0; i<word1.len(); ++i)
      if (word1[i]!=word2[i]) ++typos;

    if (target>=0.99) return(0.0);

    if (typos==2 && word1.len()>=5){ // may be a letter swap
      for (i=0; i<word1.len(); ++i)
        if (word1[i]!=word2[i]) break;
      if (i+1<word1.len() && word1[i]==word2[i+1] && word2[i]==word1[i+1]) // it is a letter swap!
        return(0.98);
    }

    if ((float)typos/word1.len() < 1.0/5.0) return(0.95-0.05*(float)typos/word1.len());
  }

  // best match will give 0.98
  res = 0.98;
  if (target>=0.98) return(0.0);

  eswalign<estr> aln;

  aln.align(word1,word2);

  return((float)aln.matches/aln.maxsize);
}


/*
   // if still not matched, check for insertion typo's
  int chistsum,chistsub;
  int chist[256]; // char distribution difference between words
  for (i=0;i<256; ++i) chist[i]=0;

  char *p1,*p2;

  for (p1=word1._str; *p1; ++p1)
    ++chist[(unsigned char)*p1];
  for (p2=word2._str; *p2; ++p2)
    --chist[(unsigned char)*p2];

  chistsum=0; chistsub=0;
  for (i=0;i<256;++i) if (chist[i]>0) chistsum+=chist[i]; else chistsub-=chist[i];

  if (word1.len() == word2.len()){
    int dist;

    dist=strdist(word1._str,word2._str);

    if (dist==2)
      return(0.9);
    else
      return(0.8*(1.0-(float)dist/(float)(word1.len()*(word1.len()+1))));
  }

  estr tmpsz1,tmpsz2;
  tmpsz1.reserve(word1.len()+1);
  tmpsz2.reserve(word2.len()+1);
  char *pt1,*pt2;

  int histdiff;
  int dlen;

  dlen = ABS(word1.len() - word2.len());

  // histogram real diff factor independent of length diff
  histdiff = (chistsum+chistsub - dlen)/2;

  // copy strings removing chars that do not have equal counts 
  for (p1=word1._str,pt1=tmpsz1._str; *p1; ++p1)
    if (!chist[(unsigned char)*p1]) { *pt1=*p1; ++pt1; }
  *pt1=0x00;
  tmpsz1._strlen=pt1-tmpsz1._str-1;

  for (p2=word2._str,pt2=tmpsz2._str; *p2; ++p2)
    if (!chist[(unsigned char)*p2]) { *pt2=*p2; ++pt2; }
  *pt2=0x00;
  tmpsz2._strlen=pt2-tmpsz2._str-1;

  // strings should now have the same size
  float lostinfo;
  lostinfo = (float)tmpsz1.len()/(float)MAX(word1.len(),word2.len());

  bool are_equal;
  are_equal = (tmpsz1==tmpsz2);

  // if it is not equal after removing all different characters then
  // consider the strings to be different
  if (!are_equal) return(0.0);

//  cout << "dlen: " << dlen << " histdiff: " << histdiff << endl;
  if (are_equal && histdiff==0 && dlen==1){
    // in this case one of the strings matches completely the other
    // and there is only one extra char on one of the strings
//    cout << "dlen: " << dlen << " histdiff: " << histdiff << endl;
    return(0.97-0.05/word1.len());
  }
  dlen += 3;
  return(0.8*lostinfo*16.0/(dlen*dlen));  
}
*/

/*
class ecmpphrase
  ecmpword cmpword;

  bool phraseSpaceSensitive=false;
  estr psChars = " \t\n\r";
  
  bool phrasePunctuationSensitive=false;
  estr ppChars = ",.:;/?()'\"[]{}*&^%$#@!~`_-<>\\|=+";

  bool wordOrderCompare=true;
  int   wocSwapMaximumSwaps=1;
  float wocSwapMaximumSwapsPerWord=0.1;
  int   wocSwapMaximumDistance=1;
*/

ecmpphrase::ecmpphrase()
{
  setSeparationChars(" ");
  setIgnoreChars("");
}


void ecmpphrase::setSeparationChars(const estr& chars)
{
  psChars=chars;
  if (chars.size())
    psCharsRE.compile(psChars);
}

void ecmpphrase::setIgnoreChars(const estr& chars)
{
  piChars=chars;
  if (chars.size())
    piCharsRE.compile(piChars);
}


float ecmpphrase::compare(const estr &str1, const estr &str2, float target)
{
  estr s1,s2;

  if (piChars.size()){
    s1=re_replace(str1,piCharsRE,"");
    s2=re_replace(str2,piCharsRE,"");
  }else{
    s1=str1;
    s2=str2;
  }


//  cout <<"s1: "<<s1<<" ,s2: "<<s2<<endl;
  if (s1==s2) return(1.0);

  if (s1.len()==s2.len() && !strncasecmp(s1._str,s2._str,s1.len())) return(0.99);

  estrarray a1(re_explode(s1,psCharsRE));
  estrarray a2(re_explode(s2,psCharsRE));

  if (a1.join("","")==a2.join("","")) return(0.98); // if for some reason some words are split in one phrase and not on the other then merging all words will yield the same phrase

//  cout << a1 << endl;
//  cout << a2 << endl;

  // we are in fact matching two string lists here!
  if (!a1.size() && !a2.size()) return(1.0);
  if (!a1.size() || !a2.size()) return(0.0);

  int i,j;
  earray<earray<float> > matched;
  earray<float> matched1;
  earray<float> matched2;
/*
  float matched[a1.size()][a2.size()];
  float matched1[a1.size()];
  float matched2[a2.size()];
*/

  for (i=0; i<a1.size(); ++i){
    matched1.add(-1.0);
    matched.add(earray<float>());
    for (j=0;j<a2.size(); ++j){
      matched[i].add(-1.0);
    }
  }
  for (j=0;j<a2.size(); ++j)
    matched2.add(-1.0);
  

  float r;
  for (i=0; i<a1.size(); ++i){
//    if (matched1[i]=1.0) continue;
    for (j=0; j<a2.size(); ++j){
//      if (matched2[j]=1.0) continue;

      //   the reason for the MAX in the target
      // is that we should not spend resources on matching something
      // worse if it has already been matched better
      r=cmpword.compare(a1.values(i),a2.values(j),MAX(matched1[i],matched2[j]));
//      printf("%s,%s - %f\n",a1.values(i)._str,a2.values(j)._str,r);
      matched[i][j]=r;
      matched1[i]=MAX(matched1[i],r);
      matched2[j]=MAX(matched2[j],r);
    }
  }

  if (a1.size()==a2.size()){
    // if order is conserved, but there is a typo somewhere
    float mmax=1.0;
    float smax1=0.0;
    float smax2=0.0;
    float mdiag=1.0;
    float sdiag=0.0;
    for (i=0; i<a1.size(); ++i){
      mdiag=mdiag*matched[i][i];
      sdiag+=matched[i][i];

      mmax=mmax*matched1[i];
      smax1+=matched1[i];
      smax2+=matched2[i];
    }

//    printf("mdiag: %f  mmax: %f\n",mdiag,mmax);
    if (mdiag >= mmax) return(mdiag);

    return(0.95*(smax1+smax2)/(2.0*a1.size()));
  }

  // if order is conserved, but there is a typo somewhere
  float smax1=-0.1;
  float smax2=-0.1;
  float mmin1=1.1;
  float mmin2=1.1;
  int imin1,imin2;
  int tsize1,tsize2;

  tsize1=0;
  for (i=0; i<a1.size(); ++i){
    tsize1+=a1.values(i).size();
    smax1+=matched1[i];
    if (mmin1>matched1[i]){
      mmin1=matched1[i];
      imin1=i;
    }
  }
  tsize2=0;
  for (j=0; j<a2.size(); ++j){
    tsize2+=a2.values(j).size();
    smax2+=matched2[j];
    if (mmin2>matched2[j]){
      mmin2=matched2[j];
      imin2=j;
    }
  }

  int dlen;
  float lostinfo;

  dlen=ABS(a1.size()-a2.size());
  if (dlen==1){
    if (a1.size()<a2.size()){
      mmin1=mmin2;
      lostinfo = 1.0-0.8*(float)a2.values(imin2).size()/(float)tsize2;
    }else
      lostinfo = 1.0-0.8*(float)a1.values(imin1).size()/(float)tsize1;

    return(MIN(0.9,lostinfo*(smax1+smax2-mmin1)/(a1.size()+a2.size()-1)));
  }

  return(0.8*(smax1+smax2)/(a1.size()+a2.size()));
}



/*
class ematchstrings
  ecmpword cmpword;
*/

eintarray ematchstrings::match(const estrarray &arr1, const estrarray &arr2)
{
  return(eintarray());
}




/*
class ematchitems
  ematchstr matchstr;
*/

template <class T>
eintarray ematchitems<T>::match(const ebasicarray<T> &arr1, const ebasicarray<T> &arr2)
{
  ldie("function match is not done yet!");
  return(eintarray());
}


ecmpword      eg_cmpword;
ecmpphrase    eg_cmpphrase;
ematchstrings eg_matchstrings;

float cmpstr(const estr &str1, const estr &str2,float target)
{
  return(eg_cmpword.compare(str1,str2,target));
}

float cmpword(const estr &str1, const estr &str2,float target)
{
  return(eg_cmpword.compare(str1,str2,target));
}

float cmpphrase(const estr &str1, const estr &str2,float target)
{
  return(eg_cmpphrase.compare(str1,str2,target));
}

eintarray cmpstrings(const estrarray &arr1, const estrarray &arr2)
{
  return(eg_matchstrings.match(arr1,arr2));
}









///////////////////////////////////////////////////////////////
//
//  Word auxiliary function
//

int strdist(char *str1, char *str2)
{
  char *pstr1,*pstr2;
  char *pstr1f,*pstr1b;

  int dst;

  dst=0;
  pstr1 = str1;  pstr2 = str2;
  pstr1f = pstr1; pstr1b = pstr1;

  while (*pstr2){
    if(*pstr1f == *pstr2){
      dst += (pstr1f-pstr1);
      ++pstr2; ++pstr1;
      pstr1f = pstr1; pstr1b = pstr1;
    }
    else if (*pstr1b == *pstr2){
      dst += (pstr1-pstr1b);
      ++pstr2; ++pstr1;
      pstr1f = pstr1; pstr1b = pstr1;
    }
    else{
      if (pstr1b <= str1 && !*pstr1f){
        dst += (pstr1f-pstr1b);
        ++pstr2; ++pstr1;
        pstr1f = pstr1; pstr1b = pstr1;
      }

      if (pstr1b > str1)
        --pstr1b;
      if (*pstr1f)
        ++pstr1f;
    }
  }
  return(dst);
}

