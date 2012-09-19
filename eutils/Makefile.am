ACLOCAL=@ACLOCAL@ -I m4

noinst_LIBRARIES=libeutils.a

SUBDIRS=sci net

esystem_osx.o: CXXFLAGS := $(CXXFLAGS) -ObjC++


if WIN32
  WIN32HDR=esystem_win32.h
  WIN32SRC=$(WIN32HDR) esystem_win32.cpp
endif

if LINUX
  LINUXHDR=esystem_linux.h
  LINUXSRC=$(LINUXHDR) esystem_linux.cpp
endif

if OSX
  OSXHDR=esystem_osx.h
  OSXSRC=$(OSXHDR) esystem_osx.cpp
endif

if HAVE_GMPLIB
  GMPLIBHDR=empint.h
  GMPLIBSRC=empint.cpp
endif

if HAVE_MPFRLIB
  MPFRLIBHDR=empfloatr.h
  MPFRLIBSRC=empfloatr.cpp
endif

nodist_libeutils_a_SOURCES=eutils.cpp ernd.cpp logger.cpp eparser.cpp vector2.cpp ematrix2.h ematrix2.cpp \
  vector3.cpp matrix3.cpp ematrix4.h ematrix4.cpp quaternion.cpp files.cpp estr.cpp estr.h \
  evar.cpp evar.h evar_dec.h evarclass.h evarclass_dec.h evartype.h evartype.cpp evarbase.h evarclassmethod.cpp evarclassmethod.h evarclassproperty.h \
  efunc.cpp efunc.h efunc_dec.h efuncbase.h efunctype.h efunctype.cpp efuncalls.h \
  evarmethodcalls.h evarclassconverter.h ebasearray.h ebasearray.cpp ebasearrayof.h ebasearrayof.cpp earrayof.cpp emysql.cpp etable.cpp estrarray.cpp \
  eregexp.cpp ekstract.cpp ecmpstr.cpp efile.cpp ecsv.cpp etimer.cpp eudl.cpp \
  eterm.cpp ebasichashmap.h ebasichashmap_dec.h ebasichashmap.cpp ehashmap.h ehashmap_dec.h ehashmap.cpp ehashfunc.h estrhash.h evarclass.cpp evarclassconverter.cpp \
  efuncalls.cpp estrhashof.h evarcommon.cpp evarcommon.h eparserinterpreter.cpp \
  eparserinterpreter.h estrhashof.cpp efuncbase.cpp eparserinterpreter.h ematrix.h \
  evarclassconstructor.h evarclasscout.h ematrix.cpp efuncinfo.h efuncinfo.cpp \
  evarmethodinfo.h edir.h edir.cpp esocket.cpp esocket.h  \
  estrarrayof.h ecgi.h ecgi.cpp estrhash.cpp esystem.h eiostream.h eiostream.cpp \
  ethread.h ethread.cpp etime.h etime.cpp earray.h earray_dec.h earray.cpp ehttp.h ehttp.cpp \
  ehtml.h ehtml.cpp ebasicarray.h ebasicarray_dec.h ebasicarray.cpp \
  edcserver.cpp edcserver.h emodule.cpp emodule.cpp \
  eblockarray.h eblockarray_dec.h \
  eoption.h etest.cpp etest.h ehttpServer.h ehttpServer.cpp ehttpHub.h ehttpHub.cpp ezlib.h ezlib.cpp \
  $(WIN32SRC) $(LINUXSRC) $(OSXSRC) $(GMPLIBSRC) $(MPFRLIBSRC)

#libeutils_a_include_HEADERS=eutils.h earray.h earray_dec.h earrayof.h earrayof_dec.h ecmpstr.h ecsv.h efile.h     \
#  encurses.h emysql.h emain.h ekstract.h eintarray.h bindata.h auxmath.h  \
#  eregexp.h ernd.h estr.h ebasearray.h ebasearrayof.h estrarray.h etable.h etimer.h etypes.h eudl.h   \
#  evar.h evar_dec.h evararray.h files.h logger.h matrix3.h ematrix4.h eparser.h quaternion.h ematrix2.h    \
#  vector2.h vector3.h ebasicarray.h ebasicarray_dec.h ehashmap.h ehashmap_dec.h ebasichashmap.h ebasichashmap_dec.h estrhash.h ehashfunc.h eterm.h \
#  evarclass.h evarclass_dec.h evartype.h evarbase.h evarclassmethod.h evarclassproperty.h \
#  evarclassconverter.h efunc.h efunc_dec.h efuncbase.h efunctype.h efuncalls.h evarmethodcalls.h \
#  estrhashof.h evarcommon.h eparserinterpreter.h ematrix.h evarclassconstructor.h \
#  evarclasscout.h eheap.h efuncinfo.h evarmethodinfo.h edir.h esocket.h \
#  eutils-config.h estrarrayof.h ecgi.h esystem.h eiostream.h etime.h \
#  eblockarray.h eblockarray_dec.h \
#  eoption.h etest.h eparalgor.h emodule.h \
#  ethread.h ehttp.h ehtml.h edcserver.h ehttpServer.h ehttpHub.h ezlib.h \
#  $(WIN32HDR) $(LINUXHDR) $(OSXHDR) $(GMPLIBHDR) $(MPFRLIBHDR)

#libeutils_a_includedir=$(includedir)/eutils

libeutils_a_LIBADD=sci/ecorr.o sci/etest.o net/enet.o sci/estats.o


#bin_SCRIPTS = eutils-config

#pkgconfigdir = $(libdir)/pkgconfig
#pkgconfig_DATA= eutils.pc

#m4datadir = $(datadir)/aclocal
#m4data_DATA = eutils.m4

EXTRA_DIST = m4 examples bootstrap eutils-config.in eutils.pc.in configure.in eutils.m4

dist-hook:
	rm -rf `find $(distdir)/examples -name .svn`
	rm -rf `find $(distdir)/m4 -name .svn`


