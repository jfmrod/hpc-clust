SUBDIRS=$(subdirs)
DIST_SUBDIRS=$(subdirs)

if HAVE_MPILIB
  MPIBIN=hpc-clust-mpi
endif

#man_MANS=man/hpc-clust.1 man/hpc-clust-mpi.1
dist_man_MANS=man/hpc-clust.1 man/hpc-clust-mpi.1

bin_PROGRAMS=hpc-clust $(MPIBIN)

LIBS=`eutils/eutils-config --libs`
EXTRA_DIST=make-otus.sh examples/aligned-archaea-seqs.sto

hpc_clust_SOURCES=hpc-clust.cpp cluster-common.h cluster-common.cpp eseqcluster.h eseqcluster.cpp eseqclusterdata.h eseqclusterdata.cpp eseq.h eseq.cpp eseqclustercount.h eseqclustercount.cpp eseqclustersingle.h eseqclustersingle.cpp eseqclusteravg.h eseqclusteravg.cpp

hpc_clust_mpi_SOURCES=hpc-clust-mpi.cpp cluster-common.h cluster-common.cpp eseqcluster.h eseqcluster.cpp eseqclustercount.h eseqclustercount.cpp eseqclustersingle.h eseqclustersingle.cpp eseqclusteravg.h eseqclusteravg.cpp eseqclusterdata.h eseqclusterdata.cpp eseq.h eseq.cpp

