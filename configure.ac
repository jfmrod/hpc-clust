AC_INIT(hpc-clust,1.2.1,joao.rodrigues@imls.uzh.ch)

: ${CXXFLAGS=-O3}

AC_LANG([C++])
AC_PROG_CXX
AC_PROG_CC
AC_PROG_RANLIB
AC_PROG_INSTALL

AX_PTHREAD()

LIBS="$PTHREAD_LIBS $LIBS"
CXXFLAGS="$CXXFLAGS $PTHREAD_CFLAGS"

AC_CANONICAL_SYSTEM

case "${target_os}" in
  darwin*)
    echo ===========================================================
    echo Setting up build environment for ${target_cpu}${target_os}
    echo ===========================================================
    target_os=osx
    CXXFLAGS="${CXXFLAGS} -DOS_MACOSX"
    LDLIBS="${LDLIBS} -framework Carbon"
    AC_SUBST(EXTRA_PROGS)
    ;;
  *linux*)
    echo ===========================================================
    echo Setting up build environment for ${target_cpu}${target_os}
    echo ===========================================================
    target_os=xlib
    CXXFLAGS="${CXXFLAGS} -DOS_LINUX"
    ;;
  *)
    echo ===========================================================
    echo Setting up build environment for ${target_cpu}${target_os}
    echo ===========================================================
    AC_MSG_ERROR([ "Unsupported target operating system, please correct" ])
  esac


AM_INIT_AUTOMAKE

#AC_CHECK_LIB(z,main)

AC_CHECK_LIB(pthread,main)

AM_CONFIG_HEADER(config.h)
AX_PREFIX_CONFIG_H

AC_CONFIG_SUBDIRS([eutils])

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
          if test "$x$no_mpi" != "xyes"; then
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


AC_ARG_ENABLE(longind,[AS_HELP_STRING([--enable-longind],[Use 64bit indices. Allows clustering more than 2million sequences but requires almost double the memory per stored distance.])],,enable_longind=yes)
if test "x$enable_longind" != "xno"; then
  echo "Compiling support for more than 2 million sequences"
  AC_DEFINE_UNQUOTED(USE_LONGIND,1,[define when using 64bit indices])
fi


#AC_SUBST(HPC_CLUST_CXXFLAGS)
#AC_SUBST(HPC_CLUST_LDFLAGS)
#AC_SUBST(HPC_CLUST_LIBS)

AC_CONFIG_FILES(Makefile)

AC_OUTPUT
