# Configure path for the GMP library
# Joao F. Matias Rodrigues <jfmrod@konceptfx.com>, Sep 2012

AC_DEFUN([AM_PATH_GMP],
[
AC_ARG_ENABLE(gmptest, [  --disable-gmptest       Do not try to compile and run a test GMP program],
		    , enable_gmptest=yes)

  min_gmp_version=ifelse([$1], ,0,$1)
  AC_MSG_CHECKING(for gmp - version >= $min_gmp_version)
  no_gmp=""

  GMP_LIBS="-lgmp -lgmpxx"
  if test "x$enable_gmptest" = "xyes"; then
    ac_save_CPPFLAGS="$CPPFLAGS"
    ac_save_LIBS="$LIBS"
dnl    CFLAGS="$CFLAGS $NCURSES_CFLAGS"
    LIBS="$LIBS $GMP_LIBS"

    rm -f conf.gmptest
    AC_TRY_RUN([
#include <gmp.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __GNU_MP_VERSION
 #define TEST_GMP_VERSION __GNU_MP_VERSION 
 #define TEST_GMP_VERSION_MINOR   __GNU_MP_VERSION_MINOR
#else
 #define TEST_GMP_VERSION __GNU_MP__ 
 #define TEST_GMP_VERSION_MINOR   0
#endif
 
 

char* my_strdup (const char *str);

char*
my_strdup (const char *str)
{
  char *new_str;
  
  if (str)
    {
      new_str = (char *)malloc ((strlen (str) + 1) * sizeof(char));
      strcpy (new_str, str);
    }
  else
    new_str = NULL;
  
  return new_str;
}

int main (void)
{
  int major = 0;
  int minor = 0;
  int n;
  char *tmp_version;

  system ("touch conf.gmptest");

  /* HP/UX 9 (%@#!) writes to sscanf strings */
  tmp_version = my_strdup("$min_gmp_version");

  n = sscanf(tmp_version, "%d.%d", &major,&minor) ;

  if (n != 2) {
     printf("error: %s, bad version string\n", "$min_gmp_version");
     exit(1);
  }

  if ((TEST_GMP_VERSION > major) || (TEST_GMP_VERSION==major && TEST_GMP_VERSION_MINOR >=minor))
  {
    exit(0);
  }
  else
  {
    printf("no\n*** GNU MP version is %d.%d, but the minimum version\n", TEST_GMP_VERSION,TEST_GMP_VERSION_MINOR);
    printf("*** of GMP required is %d.%d. To use GMP with this software you need\n", major,minor);
    printf("*** to upgrade to the required version.\n");
    exit(1);
  }
  return(0);
}

],, no_gmp=yes,[echo $ac_n "cross compiling; assumed OK... $ac_c"])
    CPPFLAGS="$ac_save_CPPFLAGS"
    LIBS="$ac_save_LIBS"
  fi


  if test "x$no_gmp" = "x"; then
    AC_MSG_RESULT(yes)
    ifelse([$2], , :, [$2])     
  else
    if test -f conf.gmptest ; then
      :
    else
      AC_MSG_RESULT(no)
    fi
    ifelse([$3], , :, [$3])
  fi
  rm -f conf.gmptest
])
