#include <eutils/emain.h>
#include <eutils/efile.h>

void gap_writeLookupTable(const estr& file);
void nogap_writeLookupTable(const estr& file,const estr& file2);

#define lt_gap_init(i,j,lt_count,mask)  { if ((i&mask)==(j&mask)) ++lt_count[i^j]; }

#define lt_nogap_init(i,j,lt_len,lt_count,mask)  \
  {                                              \
    if ((j&i&mask)==mask)                        \
      ++lt_len[i&j];                             \
    if ((i&mask)==(j&mask))                      \
      ++lt_count[i^j];                           \
  }

void gap_writeLookupTable(const estr& fnm)
{
  char tmplt_gap_count[0x1u << 16];
  unsigned int i,j;
  for (i=0u; i<0x1u<<16; ++i){
    for (j=0u; j<=i; ++j){
      tmplt_gap_count[i^j]=0;
      lt_gap_init(i,j,tmplt_gap_count,0xfu);
      lt_gap_init(i,j,tmplt_gap_count,0xf0u);
      lt_gap_init(i,j,tmplt_gap_count,0xf00u);
      lt_gap_init(i,j,tmplt_gap_count,0xf000u);
    }
  }
  efile f(fnm);
  for (i=0u; i<0x1u<<16; ++i)
    f.write(tmplt_gap_count[i]);
  f.close();
}

void nogap_writeLookupTable(const estr& fnm,const estr& fnm2)
{
  char tmplt_nogap_len[0x1u<<16];
  char tmplt_nogap_count[0x1u<<16];

  unsigned int i,j;
  for (i=0u; i<0x1u<<16; ++i){
    for (j=0u; j<=i; ++j){
      tmplt_nogap_len[i&j]=0;
      tmplt_nogap_count[i^j]=0;
      lt_nogap_init(i,j,tmplt_nogap_len,tmplt_nogap_count,0xfu);
      lt_nogap_init(i,j,tmplt_nogap_len,tmplt_nogap_count,0xf0u);
      lt_nogap_init(i,j,tmplt_nogap_len,tmplt_nogap_count,0xf00u);
      lt_nogap_init(i,j,tmplt_nogap_len,tmplt_nogap_count,0xf000u);
    }
  }
  efile f;
  f.open(fnm,"w");
  for (i=0u; i<0x1u<<16; ++i)
    f.write(tmplt_nogap_count[i]);
  f.close();
  f.open(fnm2,"w");
  for (i=0u; i<0x1u<<16; ++i)
    f.write(tmplt_nogap_len[i]);
  f.close();
}



int emain()
{
  gap_writeLookupTable("lt_gap_count");
  nogap_writeLookupTable("lt_nogap_count","lt_nogap_len");
  return(0);
}
