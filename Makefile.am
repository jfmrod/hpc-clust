SUBDIRS=$(subdirs)
DIST_SUBDIRS=$(subdirs)

if HAVE_MPILIB
  MPIBIN=hpc-clust-mpi
  MPITESTS=tests/mpi-sl.sh
endif

TESTS=tests/single-al.sh tests/single-sl.sh $(MPITESTS)


#man_MANS=man/hpc-clust.1 man/hpc-clust-mpi.1
dist_man_MANS=man/hpc-clust.1 man/hpc-clust-mpi.1

bin_PROGRAMS=hpc-clust $(MPIBIN) # denoiser

AM_CXXFLAGS=
#LIBS=`eutils/eutils-config --libs`
if !USE_SYSEUTILS
  LDADD=libs/eutils/libeutils.la
  AM_CXXFLAGS+= -I$(srcdir)/libs/eutils
#  AM_CXXFLAGS+= -Ilibs
endif



EXTRA_DIST=make-otus-mothur.sh make-otus.sh examples/aligned-archaea-seqs.sto manual/hpc-clust-manual.tex manual/hpc-clust-manual.pdf

# denoiser_SOURCES=denoiser.cpp ekmerhashmap.h ekmerhashmap_dec.h

hpc_clust_SOURCES=hpc-clust.cpp cluster-common.h cluster-common.cpp eseqcluster.h eseqcluster.cpp eseqclusterdata.h eseqclusterdata.cpp eseq.h eseq.cpp eseqclustercount.h eseqclustercount.cpp eseqclustersingle.h eseqclustersingle.cpp eseqclusteravg.h eseqclusteravg.cpp lookuptables.h lookuptables.cpp

hpc_clust_mpi_SOURCES=hpc-clust-mpi.cpp cluster-common.h cluster-common.cpp eseqcluster.h eseqcluster.cpp eseqclustercount.h eseqclustercount.cpp eseqclustersingle.h eseqclustersingle.cpp eseqclusteravg.h eseqclusteravg.cpp eseqclusterdata.h eseqclusterdata.cpp eseq.h eseq.cpp lookuptables.h lookuptables.cpp

#nodist_make_lookuptables_SOURCES=make-lookuptables.cpp

# lt_gap_count lt_nogap_count lt_nogap_len : make-lookuptables
#	./make-lookuptables

#lookuptables.cpp : lt_gap_count lt_nogap_count lt_nogap_len
#	for i in $^; do xxd -i $$i; done > $@
