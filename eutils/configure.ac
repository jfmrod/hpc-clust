AC_INIT(eutils,1.1.0,joao.rodrigues@imls.uzh.ch)

LT_INIT

: ${CXXFLAGS=-O3}

AC_LANG([C++])
AC_PROG_CXX
AC_CANONICAL_HOST

AX_PTHREAD()

LIBS="$PTHREAD_LIBS $LIBS"
CXXFLAGS="$CXXFLAGS $PTHREAD_CFLAGS"

case "${host_os}" in
  darwin*)
    echo ===========================================================
    echo Setting up build environment for ${host_os}  # ${target_cpu}${target_os}
    echo ===========================================================
    CXXFLAGS="${CXXFLAGS} -I/System/Library/Frameworks/CoreServices.framework/Frameworks/CarbonCore.framework/Headers"
    LIBS="${LIBS} -framework Carbon -framework Foundation -framework AppKit"
    os=macosx
    ;;
  *linux*)
    echo ===========================================================
    echo Setting up build environment for ${host_os}
    echo ===========================================================

    # in linux we only have this option
    os=linux
    ;;
  *win32*|*mingw*)
    echo ===========================================================
    echo Setting up build environment for ${host_os}
    echo ===========================================================

    os=win32
    ;;
  *)
    echo ===========================================================
    echo Setting up build environment for ${host_os}
    echo ===========================================================
    AC_MSG_ERROR([ "Unsupported target operating system" ])
esac
AM_CONDITIONAL(WIN32, test "x$os" = "xwin32")
AM_CONDITIONAL(LINUX, test "x$os" = "xlinux")
AM_CONDITIONAL(OSX, test "x$os" = "xmacosx")


AC_CHECK_TOOL(AR, ar)
AC_PROG_INSTALL

AM_INIT_AUTOMAKE

AC_CHECK_LIB(rt,main)

AC_ARG_WITH(mpi,[AS_HELP_STRING([--with-mpi@<:@=path@:>@],[Specify path to MPI library installation.])],,with_mpi=yes)
if test "x$with_mpi" != "xyes" -a "x$with_mpi" != "xno"; then
  CXXFLAGS="${CXXFLAGS} -I${with_mpi}/include"
  LDFLAGS="${LDFLAGS} -L${with_mpi}/lib"
fi
if test "x$with_mpi" != "xno"; then
  AC_CHECK_PROG([MPICPP],[mpic++],[/usr/bin/mpic++],[no])
  if test "x$MPICPP" != "xno"; then
    LDFLAGS="${LDFLAGS} `mpic++ --showme:links`"
    CXXFLAGS="${CXXFLAGS} `mpic++ --showme:compile`"
  else
    if test "x$no_mpi" = "xyes"; then
      PKG_CHECK_MODULES(MPI,"openmpi-1.4-gcc",no_mpi="")
      LIBS="${LIBS} $MPI_LIBS"
      CXXFLAGS="${CXXFLAGS} $MPI_CFLAGS"
    else
      AC_CHECK_LIB(mpi,main,,no_mpi=yes)
      if test "x$no_mpi" != "xyes"; then
        AC_CHECK_HEADER([mpi.h],,no_mpi=yes)
        if test "x$no_mpi" = "xyes"; then
          no_mpi=""
          AC_CHECK_HEADER([mpi/mpi.h],,no_mpi=yes)
          if test "x$no_mpi" != "xyes"; then
            CXXFLAGS="${CXXFLAGS} -I/usr/include/mpi"
          fi
        fi
      fi
    fi
  fi
fi
if test "x$no_mpi" = "xyes" -a "x$with_mpi" != "xno"; then
  AC_MSG_ERROR([
*** MPI library not found.
*** The MPI library is needed to compile the MPI version of hpc-clust ("hpc-clust-mpi") program.
*** If you do *not* need the MPI version of hpc-clust, you can explicitly disable it with ./configure --without-mpi.
*** If you want the MPI version to be compiled, you can try installing the MPI library using the package manager in your system or by downloading and installing manually (http://www.open-mpi.org/).
*** Some examples of commands for packet managers are: apt-get install libopenmpi (on Debian, Ubuntu, ...), yum install openmpi (on Redhat, Rocks, ...).
*** If you know that the MPI library is installed, please make sure that the libmpi.a (or libmpi.so) library can be found in /usr/lib, and that the mpi.h header can be found in /usr/include.
*** If the files exist elsewhere please use ./configure CXXFLAGS="-I/include/path/to/mpi.h_directory/" LDFLAGS="-L/library/path/to/libmpi.a_directory/", or create symbolic links to the correct files in the /usr/include and /usr/lib directories.])
fi

if test "x$no_mpi" != "xyes" -a "x$with_mpi" != "xno"; then
  echo "Compiling MPI library support"
  AC_DEFINE_UNQUOTED(HAVE_MPILIB,1,[define when MPI library exists])
  LIBS="$LIBS -lmpi"
  AC_CHECK_LIB(mpi_cxx,main)
  AC_CHECK_LIB(mpi++,main)
fi
AM_CONDITIONAL(HAVE_MPILIB, test "x$no_mpi" != "xyes" -a "x$with_mpi" != "xno")

AC_ARG_ENABLE(regcommon, [  --disable-regcommon    disable registration of all eutils classes and methods. Useful when developing to reduce compiling time. ], ,enable_regcommon="yes" )
if test "x$enable_regcommon" = "xyes"; then
  AC_DEFINE_UNQUOTED(REGISTER_COMMON,1,[register all eutils classes and methods])
fi

AC_CHECK_HEADER(regex.h,,no_regex=1)
if test "x$no_regex" = "x"; then
AC_DEFINE_UNQUOTED(HAVE_REGEX,1,[define when regex exists])
fi


EUTILS_CXXFLAGS="-I$includedir $CXXFLAGS"
EUTILS_LDFLAGS="$LDFLAGS"
EUTILS_LIBS="eutils/libeutils.a $LIBS"

AC_SUBST(EUTILS_CXXFLAGS)
AC_SUBST(EUTILS_LDFLAGS)
AC_SUBST(EUTILS_LIBS)

AM_CONFIG_HEADER(config.h)
AX_PREFIX_CONFIG_H

AC_CONFIG_FILES([eutils.pc Makefile])
AC_CONFIG_FILES([eutils-config],[chmod +x eutils-config])
AC_CONFIG_MACRO_DIR([m4])

AC_OUTPUT
