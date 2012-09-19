# Configure path for MYSQL Library
# Joao F. Matias Rodrigues <nyone@enyon.homeip.net>, August 2007


AC_DEFUN([AM_PATH_MYSQL],
[
AC_ARG_WITH(mysql-prefix,[  --with-mysql-prefix=PFX   Prefix where MYSQL is installed (optional)],
            mysql_prefix="$withval", mysql_prefix="")
AC_ARG_WITH(mysql-exec-prefix,[  --with-mysql-exec-prefix=PFX Exec prefix where MYSQL is installed (optional)],
            mysql_exec_prefix="$withval", mysql_exec_prefix="")
AC_ARG_ENABLE(mysqltest, [  --disable-mysqltest       Do not try to compile and run a test MYSQL program],
		    , enable_mysqltest=yes)

  if test "x${MYSQL_CONFIG+set}" != xset ; then
     if test "x$mysql_prefix" != x ; then
        MYSQL_CONFIG="$mysql_prefix/bin/mysql_config"
     fi
     if test "x$mysql_exec_prefix" != x ; then
        MYSQL_CONFIG="$mysql_exec_prefix/bin/mysql_config"
     fi
  fi

  AC_PATH_PROG(MYSQL_CONFIG, mysql_config, no)
  min_mysql_version=ifelse([$1], ,4.0.0,$1)
  AC_MSG_CHECKING(for mysql - version >= $min_mysql_version)
  no_mysql=""
  if test "$MYSQL_CONFIG" = "no" ; then
    no_mysql=yes
  else
    MYSQL_CFLAGS=`$MYSQL_CONFIG --include`
    MYSQL_LIBS=`$MYSQL_CONFIG --libs`

    mysql_major_version=`$MYSQL_CONFIG --version | \
             sed 's/^\([[0-9]]*\).*/\1/'`

    if test "x${mysql_major_version}" = "x" ; then
      mysql_major_version=0
    fi

    mysql_minor_version=`$MYSQL_CONFIG --version | \
             sed 's/^\([[0-9]]*\)\.\{0,1\}\([[0-9]]*\).*/\2/'`

    if test "x${mysql_minor_version}" = "x" ; then
      mysql_minor_version=0
    fi

    mysql_micro_version=`$MYSQL_CONFIG --version | \
             sed 's/^\([[0-9]]*\)\.\{0,1\}\([[0-9]]*\)\.\{0,1\}\([[0-9]]*\).*/\3/'`

    if test "x${mysql_micro_version}" = "x" ; then
       mysql_micro_version=0
    fi

    if test "x$enable_mysqltest" = "xyes" ; then
      ac_save_CFLAGS="$CFLAGS"
      ac_save_LIBS="$LIBS"
      CFLAGS="$CFLAGS $MYSQL_CFLAGS"
      LIBS="$LIBS $MYSQL_LIBS"

      rm -f conf.mysqltest
      AC_TRY_RUN([
#include <mysql/mysql.h> /* Headers for MySQL usage */
#include <mysql/mysqld_error.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* m4test_strdup (const char *str);

char*
m4test_strdup (const char *str)
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

  system ("touch conf.mysqltest");

  MYSQL msql;
  if(mysql_init(&msql)==NULL){
    printf("\n*** unable to initialize mysql library\n");
    exit(1);
  }
 
  /* HP/UX 9 (%@#!) writes to sscanf strings */

  tmp_version = m4test_strdup("$min_mysql_version");

  n = sscanf(tmp_version, "%d.%d.%d", &major, &minor, &micro) ;

  if (n != 2 && n != 3) {
     printf("%s, bad version string\n", "$min_mysql_version");
     exit(1);
   }

   if (($mysql_major_version > major) ||
      (($mysql_major_version == major) && ($mysql_minor_version > minor)) ||
      (($mysql_major_version == major) && ($mysql_minor_version == minor) && ($mysql_micro_version >= micro)))
    {
      exit(0);
    }
  else
    {
      printf("\n*** 'mysql_config --version' returned %d.%d.%d, but the minimum version\n", $mysql_major_version, $mysql_minor_version, $mysql_micro_version);
      printf("*** of MYSQL required is %d.%d.%d. If mysql_config is correct, then it is\n", major, minor, micro);
      printf("*** best to upgrade to the required version.\n");
      printf("*** If mysql_config was wrong, set the environment variable MYSQL_CONFIG\n");
      printf("*** to point to the correct copy of mysql_config, and remove the file\n");
      printf("*** config.cache before re-running configure\n");
      exit(1);
    }
  return(0);
}

],, no_mysql=yes,[echo $ac_n "cross compiling; assumed OK... $ac_c"])
      CFLAGS="$ac_save_CFLAGS"
      LIBS="$ac_save_LIBS"
    fi
  fi
  if test "x$no_mysql" = x ; then
     AC_MSG_RESULT(yes)
     ifelse([$2], , :, [$2])     
  else
     AC_MSG_RESULT(no)
     if test "$MYSQL_CONFIG" = "no" ; then
       echo "*** The mysql_config script installed by MYSQL could not be found"
       echo "*** If MYSQL was installed in PREFIX, make sure PREFIX/bin is in"
       echo "*** your path, or set the MYSQL_CONFIG environment variable to the"
       echo "*** full path to mysql_config."
     else
       if test -f conf.mysqltest ; then
        :
       else
          echo "*** Could not run MYSQL test program, checking why..."
          CFLAGS="$CFLAGS $MYSQL_CFLAGS"
          LIBS="$LIBS $MYSQL_LIBS"
          AC_TRY_LINK([
#include <stdio.h>
],      [ return 0; ],
        [ echo "*** The test program compiled, but did not run. This usually means"
          echo "*** that the run-time linker is not finding MYSQL or finding the wrong"
          echo "*** version of MYSQL. If it is not finding MYSQL, you'll need to set your"
          echo "*** LD_LIBRARY_PATH environment variable, or edit /etc/ld.so.conf to point"
          echo "*** to the installed location  Also, make sure you have run ldconfig if that"
          echo "*** is required on your system"
	  echo "***"
          echo "*** If you have an old version installed, it is best to remove it, although"
          echo "*** you may also be able to get things to work by modifying LD_LIBRARY_PATH"],
        [ echo "*** The test program failed to compile or link. See the file config.log for the"
          echo "*** exact error that occured. This usually means MYSQL include files are not installed"
          echo "*** or that they are not in the compilers search path. In this case you should point"
          echo "*** the CPLUS_INCLUDE_PATH to your MYSQL include directory." ])
          CFLAGS="$ac_save_CFLAGS"
          LIBS="$ac_save_LIBS"
       fi
     fi
     ifelse([$3], , :, [$3])
  fi
  AC_SUBST(MYSQL_CFLAGS)
  AC_SUBST(MYSQL_LIBS)
  rm -f conf.mysqltest
])


