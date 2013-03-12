ACLOCAL=@ACLOCAL@ -I m4

noinst_LIBRARIES=libeutils.a

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

headers=eutils.h earray.h earray_dec.h earrayof.h earrayof_dec.h efile.h     \
  emain.h eintarray.h auxmath.h  \
  eregexp.h ernd.h estr.h ebasearray.h ebasearrayof.h estrarray.h etimer.h etypes.h   \
  evar.h evar_dec.h evararray.h logger.h eparser.h   \
  ebasicarray.h ebasicarray_dec.h ehashmap.h ehashmap_dec.h ebasichashmap.h ebasichashmap_dec.h estrhash.h ehashfunc.h eterm.h \
  evarclass.h evarclass_dec.h evartype.h evarbase.h evarclassmethod.h evarclassproperty.h \
  evarclassconverter.h efunc.h efunc_dec.h efuncbase.h efunctype.h efuncalls.h evarmethodcalls.h \
  estrhashof.h evarcommon.h eparserinterpreter.h evarclassconstructor.h \
  evarclasscout.h eheap.h efuncinfo.h evarmethodinfo.h edir.h esocket.h \
  eutils-config.h estrarrayof.h esystem.h eiostream.h etime.h \
  eblockarray.h eblockarray_dec.h \
  eoption.h etest.h \
  ethread.h edcserver.h \
  $(DLHDR) $(WIN32HDR) $(LINUXHDR) $(OSXHDR) $(GMPLIBHDR) $(MPFRLIBHDR)

libeutils_a_SOURCES=$(headers) eutils.cpp ernd.cpp logger.cpp eparser.cpp \
  estr.cpp estr.h \
  evar.cpp evar.h evar_dec.h evarclass.h evarclass_dec.h evartype.h evartype.cpp evarbase.h evarclassmethod.cpp evarclassmethod.h evarclassproperty.h \
  efunc.cpp efunc.h efunc_dec.h efuncbase.h efunctype.h efunctype.cpp efuncalls.h \
  evarmethodcalls.h evarclassconverter.h ebasearray.h ebasearray.cpp ebasearrayof.h ebasearrayof.cpp earrayof.cpp estrarray.cpp \
  eregexp.cpp efile.cpp etimer.cpp \
  eterm.cpp ebasichashmap.h ebasichashmap_dec.h ebasichashmap.cpp ehashmap.h ehashmap_dec.h ehashmap.cpp ehashfunc.h estrhash.h evarclass.cpp evarclassconverter.cpp \
  efuncalls.cpp estrhashof.h evarcommon.cpp evarcommon.h eparserinterpreter.cpp \
  eparserinterpreter.h estrhashof.cpp efuncbase.cpp eparserinterpreter.h \
  evarclassconstructor.h evarclasscout.h efuncinfo.h efuncinfo.cpp \
  evarmethodinfo.h edir.h edir.cpp esocket.cpp esocket.h  \
  estrarrayof.h estrhash.cpp esystem.h \
  ethread.h ethread.cpp etime.h etime.cpp earray.h earray_dec.h earray.cpp \
  ebasicarray.h ebasicarray_dec.h ebasicarray.cpp \
  edcserver.cpp edcserver.h \
  eblockarray.h eblockarray_dec.h \
  eoption.h etest.cpp etest.h \
  $(DLSRC) $(WIN32SRC) $(LINUXSRC) $(OSXSRC) $(GMPLIBSRC) $(MPFRLIBSRC)

EXTRA_DIST = m4 bootstrap eutils-config.in eutils.pc.in configure.in eutils.m4

dist-hook:
	rm -rf `find $(distdir)/m4 -name .svn`


