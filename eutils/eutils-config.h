#ifndef _EUTILS_CONFIG_H
#define _EUTILS_CONFIG_H 1
 
/* eutils-config.h. Generated automatically at end of configure. */
/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.in by autoheader.  */

/* define when apophenia library exists */
#ifndef EUTILS_HAVE_APOLIB 
#define EUTILS_HAVE_APOLIB  1 
#endif

/* define when gnu mp library exists */
#ifndef EUTILS_HAVE_GMPLIB 
#define EUTILS_HAVE_GMPLIB  1 
#endif

/* Define to 1 if you have the <inttypes.h> header file. */
#ifndef EUTILS_HAVE_INTTYPES_H 
#define EUTILS_HAVE_INTTYPES_H  1 
#endif

/* define when libgsl exists */
#ifndef EUTILS_HAVE_LIBGSL 
#define EUTILS_HAVE_LIBGSL  1 
#endif

/* Define to 1 if you have the `mpi' library (-lmpi). */
#ifndef EUTILS_HAVE_LIBMPI 
#define EUTILS_HAVE_LIBMPI  1 
#endif

/* Define to 1 if you have the `mpi_cxx' library (-lmpi_cxx). */
#ifndef EUTILS_HAVE_LIBMPI_CXX 
#define EUTILS_HAVE_LIBMPI_CXX  1 
#endif

/* Define to 1 if you have the `mpi++' library (-lmpi++). */
/* #undef HAVE_LIBMPI__ */

/* define when libmysql exists */
/* #undef HAVE_LIBMYSQL */

/* define when ncurses exists */
#ifndef EUTILS_HAVE_LIBNCURSES 
#define EUTILS_HAVE_LIBNCURSES  1 
#endif

/* Define to 1 if you have the `readline' library (-lreadline). */
#ifndef EUTILS_HAVE_LIBREADLINE 
#define EUTILS_HAVE_LIBREADLINE  1 
#endif

/* Define to 1 if you have the `rt' library (-lrt). */
#ifndef EUTILS_HAVE_LIBRT 
#define EUTILS_HAVE_LIBRT  1 
#endif

/* Define to 1 if you have the <memory.h> header file. */
#ifndef EUTILS_HAVE_MEMORY_H 
#define EUTILS_HAVE_MEMORY_H  1 
#endif

/* define when mpfr library exists */
/* #undef HAVE_MPFRLIB */

/* define when MPI library exists */
#ifndef EUTILS_HAVE_MPILIB 
#define EUTILS_HAVE_MPILIB  1 
#endif

/* Define if you have POSIX threads libraries and header files. */
#ifndef EUTILS_HAVE_PTHREAD 
#define EUTILS_HAVE_PTHREAD  1 
#endif

/* Have PTHREAD_PRIO_INHERIT. */
/* #undef HAVE_PTHREAD_PRIO_INHERIT */

/* define when readline.h exists */
#ifndef EUTILS_HAVE_READLINE_H 
#define EUTILS_HAVE_READLINE_H  1 
#endif

/* define when regex exists */
#ifndef EUTILS_HAVE_REGEX 
#define EUTILS_HAVE_REGEX  1 
#endif

/* Define to 1 if you have the <stdint.h> header file. */
#ifndef EUTILS_HAVE_STDINT_H 
#define EUTILS_HAVE_STDINT_H  1 
#endif

/* Define to 1 if you have the <stdlib.h> header file. */
#ifndef EUTILS_HAVE_STDLIB_H 
#define EUTILS_HAVE_STDLIB_H  1 
#endif

/* Define to 1 if you have the <strings.h> header file. */
#ifndef EUTILS_HAVE_STRINGS_H 
#define EUTILS_HAVE_STRINGS_H  1 
#endif

/* Define to 1 if you have the <string.h> header file. */
#ifndef EUTILS_HAVE_STRING_H 
#define EUTILS_HAVE_STRING_H  1 
#endif

/* Define to 1 if you have the <sys/stat.h> header file. */
#ifndef EUTILS_HAVE_SYS_STAT_H 
#define EUTILS_HAVE_SYS_STAT_H  1 
#endif

/* Define to 1 if you have the <sys/types.h> header file. */
#ifndef EUTILS_HAVE_SYS_TYPES_H 
#define EUTILS_HAVE_SYS_TYPES_H  1 
#endif

/* Define to 1 if you have the <unistd.h> header file. */
#ifndef EUTILS_HAVE_UNISTD_H 
#define EUTILS_HAVE_UNISTD_H  1 
#endif

/* define when zlib exists */
#ifndef EUTILS_HAVE_ZLIB 
#define EUTILS_HAVE_ZLIB  1 
#endif

/* Name of package */
#ifndef EUTILS_PACKAGE 
#define EUTILS_PACKAGE  "eutils" 
#endif

/* Define to the address where bug reports for this package should be sent. */
#ifndef EUTILS_PACKAGE_BUGREPORT 
#define EUTILS_PACKAGE_BUGREPORT  "" 
#endif

/* Define to the full name of this package. */
#ifndef EUTILS_PACKAGE_NAME 
#define EUTILS_PACKAGE_NAME  "" 
#endif

/* Define to the full name and version of this package. */
#ifndef EUTILS_PACKAGE_STRING 
#define EUTILS_PACKAGE_STRING  "" 
#endif

/* Define to the one symbol short name of this package. */
#ifndef EUTILS_PACKAGE_TARNAME 
#define EUTILS_PACKAGE_TARNAME  "" 
#endif

/* Define to the home page for this package. */
#ifndef EUTILS_PACKAGE_URL 
#define EUTILS_PACKAGE_URL  "" 
#endif

/* Define to the version of this package. */
#ifndef EUTILS_PACKAGE_VERSION 
#define EUTILS_PACKAGE_VERSION  "" 
#endif

/* Define to necessary symbol if this constant uses a non-standard name on
   your system. */
/* #undef PTHREAD_CREATE_JOINABLE */

/* register all eutils classes and methods */
#ifndef EUTILS_REGISTER_COMMON 
#define EUTILS_REGISTER_COMMON  1 
#endif

/* Define to 1 if you have the ANSI C header files. */
#ifndef EUTILS_STDC_HEADERS 
#define EUTILS_STDC_HEADERS  1 
#endif

/* Version number of package */
#ifndef EUTILS_VERSION 
#define EUTILS_VERSION  "1.0.0" 
#endif

/* include dynamic linking routines */
/* #undef WITH_DL */
 
/* once: _EUTILS_CONFIG_H */
#endif
