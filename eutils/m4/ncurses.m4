# Configure path for the GNU Scientific Library
# Christopher R. Gabriel <cgabriel@linux.it>, April 2000


AC_DEFUN([AM_PATH_NCURSES],
[
AC_ARG_WITH(ncurses-prefix,[  --with-ncurses-prefix=PFX   Prefix where NCURSES is installed (optional)],
            ncurses_prefix="$withval", ncurses_prefix="")
AC_ARG_WITH(ncurses-exec-prefix,[  --with-ncurses-exec-prefix=PFX Exec prefix where NCURSES is installed (optional)],
            ncurses_exec_prefix="$withval", ncurses_exec_prefix="")
AC_ARG_ENABLE(ncursestest, [  --disable-ncursestest       Do not try to compile and run a test NCURSES program],
		    , enable_ncursestest=yes)

  if test "x${NCURSES_CONFIG+set}" != xset ; then
     if test "x$ncurses_prefix" != x ; then
         NCURSES_CONFIG="$ncurses_prefix/bin/ncurses-config"
     fi
     if test "x$ncurses_exec_prefix" != x ; then
        NCURSES_CONFIG="$ncurses_exec_prefix/bin/ncurses-config"
     fi
  fi

  AC_PATH_PROG(NCURSES_CONFIG, ncurses-config, no)
  if test "$NCURSES_CONFIG" = "no" ; then
    AC_PATH_PROG(NCURSES5_CONFIG, ncurses5-config, no)
    NCURSES_CONFIG="$NCURSES5_CONFIG"
  fi

  min_ncurses_version=ifelse([$1], ,0.2.5,$1)
  AC_MSG_CHECKING(for ncurses - version >= $min_ncurses_version)
  no_ncurses=""

  NCURSES_CFLAGS="-I/usr/include/ncurses"
  NCURSES_LIBS="-lncurses -ldl"
  ncurses_major_version=0
  ncurses_minor_version=0
  ncurses_micro_version=0

  if test "$NCURSES_CONFIG" != "no" ; then
#    no_ncurses=yes
#  else
    NCURSES_CFLAGS=`$NCURSES_CONFIG --cflags`
    NCURSES_LIBS=`$NCURSES_CONFIG --libs`

    ncurses_major_version=`$NCURSES_CONFIG --version | \
           sed 's/^\([[0-9]]*\).*/\1/'`
    if test "x${ncurses_major_version}" = "x" ; then
       ncurses_major_version=0
    fi

    ncurses_minor_version=`$NCURSES_CONFIG --version | \
           sed 's/^\([[0-9]]*\)\.\{0,1\}\([[0-9]]*\).*/\2/'`
    if test "x${ncurses_minor_version}" = "x" ; then
       ncurses_minor_version=0
    fi

    ncurses_micro_version=`$NCURSES_CONFIG --version | \
           sed 's/^\([[0-9]]*\)\.\{0,1\}\([[0-9]]*\)\.\{0,1\}\([[0-9]]*\).*/\3/'`
    if test "x${ncurses_micro_version}" = "x" ; then
       ncurses_micro_version=0
    fi

  fi

    if test "x$enable_ncursestest" = "xyes" ; then
      ac_save_CFLAGS="$CFLAGS"
      ac_save_LIBS="$LIBS"
      CFLAGS="$CFLAGS $NCURSES_CFLAGS"
      LIBS="$LIBS $NCURSES_LIBS"

      rm -f conf.ncursestest
      AC_TRY_RUN([
#include <ncurses.h>

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

  system ("touch conf.ncursestest");

/*
  if (initscr()==NULL){
    printf("error initializing ncurses\n");
    exit(1);
  }
*/

  /* HP/UX 9 (%@#!) writes to sscanf strings */
  tmp_version = my_strdup("$min_ncurses_version");

  n = sscanf(tmp_version, "%d.%d.%d", &major, &minor, &micro) ;

  if (n != 2 && n != 3) {
     printf("%s, bad version string\n", "$min_ncurses_version");
     exit(1);
  }

  if ((NCURSES_VERSION_MAJOR > major) ||
     ((NCURSES_VERSION_MAJOR == major) && (NCURSES_VERSION_MINOR > minor)))
  {
    exit(0);
  }
  else
  {
    printf("\n*** NCURSES_VERSION is %s, but the minimum version\n", NCURSES_VERSION);
    printf("*** of NCURSES required is %d.%d. If ncurses-config is correct, then it is\n", major, minor);
    printf("*** best to upgrade to the required version.\n");
    printf("*** If ncurses-config was wrong, set the environment variable NCURSES_CONFIG\n");
    printf("*** to point to the correct copy of ncurses-config, and remove the file\n");
    printf("*** config.cache before re-running configure\n");
    exit(1);
  }
  return(0);
}

],, no_ncurses=yes,[echo $ac_n "cross compiling; assumed OK... $ac_c"])
       CFLAGS="$ac_save_CFLAGS"
       LIBS="$ac_save_LIBS"
     fi

#  fi

  if test "x$no_ncurses" = x ; then
     AC_MSG_RESULT(yes)
     ifelse([$2], , :, [$2])     
  else
     AC_MSG_RESULT(no)
     if test "$NCURSES_CONFIG" = "no" ; then
       echo "*** The ncurses-config script installed by NCURSES could not be found"
       echo "*** If NCURSES was installed in PREFIX, make sure PREFIX/bin is in"
       echo "*** your path, or set the NCURSES_CONFIG environment variable to the"
       echo "*** full path to ncurses-config."
     else
       if test -f conf.ncursestest ; then
        :
       else
          echo "*** Could not run NCURSES test program, checking why..."
          CFLAGS="$CFLAGS $NCURSES_CFLAGS"
          LIBS="$LIBS $NCURSES_LIBS"
          AC_TRY_LINK([
#include <stdio.h>
],      [ return 0; ],
        [ echo "*** The test program compiled, but did not run. This usually means"
          echo "*** that the run-time linker is not finding NCURSES or finding the wrong"
          echo "*** version of NCURSES. If it is not finding NCURSES, you'll need to set your"
          echo "*** LD_LIBRARY_PATH environment variable, or edit /etc/ld.so.conf to point"
          echo "*** to the installed location  Also, make sure you have run ldconfig if that"
          echo "*** is required on your system"
	  echo "***"
          echo "*** If you have an old version installed, it is best to remove it, although"
          echo "*** you may also be able to get things to work by modifying LD_LIBRARY_PATH"],
        [ echo "*** The test program failed to compile or link. See the file config.log for the"
          echo "*** exact error that occured. This usually means NCURSES was incorrectly installed"
          echo "*** or that you have moved NCURSES since it was installed. In the latter case, you"
          echo "*** may want to edit the ncurses-config script: $NCURSES_CONFIG" ])
          CFLAGS="$ac_save_CFLAGS"
          LIBS="$ac_save_LIBS"
       fi
     fi
#     NCURSES_CFLAGS=""
#     NCURSES_LIBS=""
     ifelse([$3], , :, [$3])
  fi
  AC_SUBST(NCURSES_CFLAGS)
  AC_SUBST(NCURSES_LIBS)
  rm -f conf.ncursestest
])


