# Configure path for the GNU Scientific Library
# Christopher R. Gabriel <cgabriel@linux.it>, April 2000
# Modified by: Joao F. Matias Rodrigues <jfmrod@konceptfx.com>, Sep 2012


AC_DEFUN([AM_PATH_GSL],
[
AC_ARG_WITH(gsl-prefix,[  --with-gsl-prefix=PFX   Prefix where GSL is installed (optional)],
            gsl_prefix="$withval", gsl_prefix="")
AC_ARG_WITH(gsl-exec-prefix,[  --with-gsl-exec-prefix=PFX Exec prefix where GSL is installed (optional)],
            gsl_exec_prefix="$withval", gsl_exec_prefix="")
AC_ARG_ENABLE(gsltest,[  --disable-gsltest       Do not try to compile and run a test GSL program],
		    , enable_gsltest=yes)

  if test "x${GSL_CONFIG+set}" != xset ; then
    if test "x$gsl_prefix" != x ; then
      GSL_CONFIG="$gsl_prefix/bin/gsl-config"
    fi
    if test "x$gsl_exec_prefix" != x ; then
      GSL_CONFIG="$gsl_exec_prefix/bin/gsl-config"
    fi
  fi

  AC_PATH_PROG(GSL_CONFIG, gsl-config, no)
  min_gsl_version=ifelse([$1], ,0.2.5,$1)
  AC_MSG_CHECKING(for GSL - version >= $min_gsl_version)
  no_gsl=""

  GSL_CFLAGS=""
  GSL_LIBS="-lgsl -lgslcblas -lm"
  gsl_major_version=0
  gsl_minor_version=0
  gsl_micro_version=0

  if test "$GSL_CONFIG" != "no" ; then
    GSL_CFLAGS=`$GSL_CONFIG --cflags`
    GSL_LIBS=`$GSL_CONFIG --libs`

    gsl_major_version=`$GSL_CONFIG --version | \
           sed 's/^\([[0-9]]*\).*/\1/'`
    if test "x${gsl_major_version}" = "x" ; then
       gsl_major_version=0
    fi

    gsl_minor_version=`$GSL_CONFIG --version | \
           sed 's/^\([[0-9]]*\)\.\{0,1\}\([[0-9]]*\).*/\2/'`
    if test "x${gsl_minor_version}" = "x" ; then
       gsl_minor_version=0
    fi

    gsl_micro_version=`$GSL_CONFIG --version | \
           sed 's/^\([[0-9]]*\)\.\{0,1\}\([[0-9]]*\)\.\{0,1\}\([[0-9]]*\).*/\3/'`
    if test "x${gsl_micro_version}" = "x" ; then
       gsl_micro_version=0
    fi
  else
    PKG_CHECK_MODULES(GSL,"gsl",[ GSL_PKGCONFIG="yes" ],[ GSL_PKGCONFIG="no" ])
  fi

  if test "x$enable_gsltest" = "xyes" ; then
    ac_save_CPPFLAGS="$CPPFLAGS"
    ac_save_LIBS="$LIBS"
    CPPFLAGS="$CPPFLAGS $GSL_CFLAGS"
    LIBS="$LIBS $GSL_LIBS"

    rm -f conf.gsltest
    AC_TRY_RUN([
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  int major = 0, minor = 0, micro = 0;
  int n;
  char *tmp_version;

  system ("touch conf.gsltest");

  /* HP/UX 9 (%@#!) writes to sscanf strings */
  tmp_version = my_strdup("$min_gsl_version");

  n = sscanf(tmp_version, "%d.%d.%d", &major, &minor, &micro) ;

  if (n != 2 && n != 3) {
     printf("\n%s, bad version string\n", "$min_gsl_version");
     exit(1);
   }

   if (($gsl_major_version > major) ||
      (($gsl_major_version == major) && ($gsl_minor_version > minor)) ||
      (($gsl_major_version == major) && ($gsl_minor_version == minor) && ($gsl_micro_version >= micro)))
    {
      exit(0);
    }
  else
    {
      printf("no\n*** 'gsl-config --version' returned %d.%d.%d, but the minimum version\n", $gsl_major_version, $gsl_minor_version, $gsl_micro_version);
      printf("*** of GSL required is %d.%d.%d. If gsl-config is correct, then it is\n", major, minor, micro);
      printf("*** best to upgrade to the required version.\n");
      printf("*** If gsl-config was wrong, set the environment variable GSL_CONFIG\n");
      printf("*** to point to the correct copy of gsl-config, and remove the file\n");
      printf("*** config.cache before re-running configure\n");
      exit(1);
    }
}

],, no_gsl=yes,[ echo $ac_n "cross compiling; assumed OK... $ac_c" ])
    CPPFLAGS="$ac_save_CPPFLAGS"
    LIBS="$ac_save_LIBS"
  fi

  if test "x$no_gsl" = x ; then
    AC_MSG_RESULT(yes)
    ifelse([$2], , :, [$2])     
  else
    AC_MSG_RESULT(no)
    if test "$GSL_CONFIG" != "no" -o "$GSL_PKGCONFIG" = "yes"; then
      if test -f conf.gsltest ; then
        :
      else
        echo "*** Could not run GSL test program, checking why..."
        CPPFLAGS="$CPPFLAGS $GSL_CFLAGS"
        LIBS="$LIBS $GSL_LIBS"
        AC_TRY_LINK([
#include <stdio.h>
],      [ return 0; ],
        [ echo "*** The test program compiled, but did not run. This usually means"
          echo "*** that the run-time linker is not finding GSL or finding the wrong"
          echo "*** version of GSL. If it is not finding GSL, you'll need to set your"
          echo "*** LD_LIBRARY_PATH environment variable, or edit /etc/ld.so.conf to point"
          echo "*** to the installed location. Also, make sure you have run ldconfig if that"
          echo "*** is required on your system"
	  echo "***"
          echo "*** If you have an old version installed, it is best to remove it, although"
          echo "*** you may also be able to get things to work by modifying LD_LIBRARY_PATH"],
        [ echo "*** The test program failed to compile or link. See the file config.log for the"
          echo "*** exact error that occured. This usually means GSL was incorrectly installed"
          echo "*** or that you have moved GSL since it was installed. In the latter case, you"
          echo "*** may want to edit the gsl-config script: $GSL_CONFIG or the gsl.pc pkgconfig file" ])
          CPPFLAGS="$ac_save_CPPFLAGS"
          LIBS="$ac_save_LIBS"
      fi
    fi
    ifelse([$3], , :, [$3])
  fi
  rm -f conf.gsltest
])


