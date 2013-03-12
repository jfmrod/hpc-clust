#lib_LIBRARIES=libenet.a


#libenet_a_SOURCES=enet.cpp enet.h

SUBDIRS=$(subdirs)
DIST_SUBDIRS=$(subdirs)

if HAVE_MPILIB
  MPIBIN=hpc-clust-mpi
endif

dist_man_MANS=man/hpc-clust.1 man/hpc-clust-mpi.1 man/hpc-clust-dist.1

bin_PROGRAMS=hpc-clust hpc-clust-dist $(MPIBIN) cluster-uniq cluster2 cluster-sorted cluster-partition cluster-fast cluster-matchshort cluster-chimera cluster-profile-siteevorate cluster-short
# cluster-cmp cluster-check cluster-evolve cluster-regen 

LIBS=`eutils/eutils-config --libs`
#EXTRA_PROGRAMS=
EXTRA_DIST=make-otus.sh examples/aligned-archaea-seqs.sto

hpc_clust_SOURCES=hpc-clust.cpp cluster-common.h cluster-common.cpp eseqcluster.h eseqcluster.cpp eseqclusterdata.h eseqclusterdata.cpp eseq.h eseq.cpp eseqclustercount.h eseqclustercount.cpp eseqclustersingle.h eseqclustersingle.cpp eseqclusteravg.h eseqclusteravg.cpp eseqclusterstep.h eseqclusterstep.cpp
#cluster_LDADD= libhpc-clust.a

cluster_short_SOURCES=cluster-short.cpp cluster-common.cpp eseqcluster.cpp eseqclustercount.cpp eseqclustersingle.cpp eseqclusteravg.cpp eseqclusterstep.cpp eseqclusterdata.h eseqclusterdata.cpp eseq.h eseq.cpp
cluster_matchshort_SOURCES=cluster-matchshort.cpp cluster-common.cpp eseqcluster.cpp eseqclusterdata.h eseqclusterdata.cpp eseq.h eseq.cpp
#cluster_match_SOURCES=cluster-match.cpp cluster-common.cpp eseqcluster.cpp eseqclusterdata.cpp
cluster_chimera_SOURCES=cluster-chimera.cpp cluster-common.cpp eseqcluster.cpp eseqclusterdata.h eseqclusterdata.cpp eseq.h eseq.cpp
cluster_profile_siteevorate_SOURCES=cluster-profile-siteevorate.cpp cluster-common.cpp eseqcluster.cpp eseqclusterdata.h eseqclusterdata.cpp eseq.h eseq.cpp

#cluster_dist_SOURCES=cluster-dist.cpp cluster-common.cpp eseqcluster.cpp eseqclustercount.cpp
hpc_clust_dist_SOURCES=hpc-clust-dist.cpp cluster-common.cpp eseqcluster.cpp eseqclusterdata.h eseqclusterdata.cpp eseq.h eseq.cpp eseqclustercount.cpp eseqclustersingle.cpp eseqclusteravg.cpp

cluster_partition_SOURCES=cluster-partition.cpp cluster-common.cpp eseqcluster.cpp eseqclustercount.cpp eseqclusterdata.h eseqclusterdata.cpp eseq.h eseq.cpp
#cluster_partition2_SOURCES=cluster-partition2.cpp cluster-common.cpp eseqcluster.cpp eseqclustercount.cpp

cluster2_SOURCES=cluster2.cpp cluster-common.cpp eseqcluster.cpp eseqclustercount.cpp eseqclusterdata.h eseqclusterdata.cpp eseq.h eseq.cpp

cluster_sorted_SOURCES=cluster-sorted.cpp cluster-common.cpp eseqcluster.cpp eseqclustercount.cpp eseqclustersingle.cpp eseqclusteravg.cpp eseqclusterdata.h eseqclusterdata.cpp eseq.h eseq.cpp
cluster_uniq_SOURCES=cluster-uniq.cpp cluster-common.cpp eseqcluster.cpp eseqclustercount.cpp eseqclusterdata.h eseqclusterdata.cpp eseq.h eseq.cpp

#cluster_cmp_SOURCES=cluster-cmp.cpp cluster-common.cpp eseqcluster.cpp eseqclustercount.cpp
#cluster_check_SOURCES=cluster-check.cpp cluster-common.cpp eseqcluster.cpp eseqclustercount.cpp
#cluster_nj_SOURCES=cluster-nj.cpp cluster-common.cpp eseqcluster.cpp eseqclustercount.cpp

hpc_clust_mpi_SOURCES=hpc-clust-mpi.cpp cluster-common.cpp eseqcluster.cpp eseqclustercount.cpp eseqclustersingle.cpp eseqclusteravg.cpp eseqclusterdata.h eseqclusterdata.cpp eseq.h eseq.cpp

#cluster_cooc_SOURCES=cluster-cooc.cpp cluster-common.cpp eseqcluster.cpp eseqclustercount.cpp

#cluster_profile_SOURCES=cluster-profile.cpp
cluster_fast_SOURCES=cluster-fast.cpp



