SUBDIRS=$(subdirs)
DIST_SUBDIRS=$(subdirs)

if HAVE_MPILIB
  MPIBIN=hpc-clust-mpi
endif

TESTS=tests/single-al.sh tests/single-sl.sh tests/mpi-sl.sh


#man_MANS=man/hpc-clust.1 man/hpc-clust-mpi.1
dist_man_MANS=man/hpc-clust.1 man/hpc-clust-mpi.1

bin_PROGRAMS=hpc-clust $(MPIBIN)

LIBS=`eutils/eutils-config --libs`
EXTRA_DIST=make-otus-mothur.sh make-otus.sh examples/aligned-archaea-seqs.sto manual/hpc-clust-manual.tex manual/hpc-clust-manual.pdf

hpc_clust_SOURCES=hpc-clust.cpp cluster-common.h cluster-common.cpp eseqcluster.h eseqcluster.cpp eseqclusterdata.h eseqclusterdata.cpp eseq.h eseq.cpp eseqclustercount.h eseqclustercount.cpp eseqclustersingle.h eseqclustersingle.cpp eseqclusteravg.h eseqclusteravg.cpp lookuptables.h lookuptables.cpp

hpc_clust_mpi_SOURCES=hpc-clust-mpi.cpp cluster-common.h cluster-common.cpp eseqcluster.h eseqcluster.cpp eseqclustercount.h eseqclustercount.cpp eseqclustersingle.h eseqclustersingle.cpp eseqclusteravg.h eseqclusteravg.cpp eseqclusterdata.h eseqclusterdata.cpp eseq.h eseq.cpp lookuptables.h lookuptables.cpp

#nodist_make_lookuptables_SOURCES=make-lookuptables.cpp

# lt_gap_count lt_nogap_count lt_nogap_len : make-lookuptables
#	./make-lookuptables

#lookuptables.cpp : lt_gap_count lt_nogap_count lt_nogap_len
#	for i in $^; do xxd -i $$i; done > $@
