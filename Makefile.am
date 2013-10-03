ACLOCAL_AMFLAGS=-I m4

lib_LTLIBRARIES=libhpc-clust.la
libhpc_clust_la_SOURCES=eseqcluster.h eseqcluster.cpp eseqclustersingle.h eseqclustersingle.cpp eseqclusteravg.h eseqclusteravg.cpp eseqclusterstep.h eseqclusterstep.cpp eseqclusterdata.h eseqclusterdata.cpp eseq.h eseq.cpp cluster-common.h cluster-common.cpp eseqclustercount.h eseqclustercount.cpp
libhpc_clust_la_include_HEADERS=eseqcluster.h eseqclustersingle.h eseqclusteravg.h eseqclusterstep.h eseqclusterdata.h eseq.h cluster-common.h eseqclustercount.h
libhpc_clust_la_includedir=$(includedir)/hpc-clust

SUBDIRS=$(subdirs)
DIST_SUBDIRS=$(subdirs)

if HAVE_MPILIB
  MPIBIN=hpc-clust-mpi
endif

dist_man_MANS=man/hpc-clust.1 man/hpc-clust-mpi.1 man/hpc-clust-dist.1

bin_PROGRAMS=hpc-clust # hpc-clust-dist $(MPIBIN) cluster-uniq cluster2 cluster-sorted cluster-partition cluster-fast cluster-matchshort cluster-chimera cluster-profile-siteevorate cluster-short
# cluster-cmp cluster-check cluster-evolve cluster-regen 

#LIBS=`eutils/eutils-config --libs`
#EXTRA_PROGRAMS=
EXTRA_DIST=make-otus.sh examples/aligned-archaea-seqs.sto

hpc_clust_SOURCES=hpc-clust.cpp cluster-common.h eseqcluster.h eseqclusterdata.h eseq.h eseqclustercount.h eseqclustersingle.h eseqclusteravg.h eseqclusterstep.h
hpc_clust_LDADD=libhpc-clust.la

cluster_short_SOURCES=cluster-short.cpp eseqclusterdata.h eseq.h
cluster_short_LDADD=libhpc-clust.la

cluster_matchshort_SOURCES=cluster-matchshort.cpp eseqclusterdata.h eseq.h
cluster_matchshort_LDADD=libhpc-clust.la

#cluster_match_SOURCES=cluster-match.cpp cluster-common.cpp eseqcluster.cpp eseqclusterdata.cpp
cluster_chimera_SOURCES=cluster-chimera.cpp eseqclusterdata.h eseq.h
cluster_chimera_LDADD=libhpc-clust.la

cluster_profile_siteevorate_SOURCES=cluster-profile-siteevorate.cpp eseqclusterdata.h eseq.h
cluster_profile_siteevorate_LDADD=libhpc-clust.la

#cluster_dist_SOURCES=cluster-dist.cpp cluster-common.cpp eseqcluster.cpp eseqclustercount.cpp
hpc_clust_dist_SOURCES=hpc-clust-dist.cpp eseqclusterdata.h eseq.h
hpc_clust_dist_LDADD=libhpc-clust.la

cluster_partition_SOURCES=cluster-partition.cpp eseqclusterdata.h eseq.h
cluster_partition_LDADD=libhpc-clust.la
#cluster_partition2_SOURCES=cluster-partition2.cpp cluster-common.cpp eseqcluster.cpp eseqclustercount.cpp

cluster2_SOURCES=cluster2.cpp eseqclusterdata.h eseq.h
cluster2_LDADD=libhpc-clust.la

cluster_sorted_SOURCES=cluster-sorted.cpp eseqclusterdata.h eseq.h
cluster_sorted_LDADD=libhpc-clust.la

cluster_uniq_SOURCES=cluster-uniq.cpp eseqclusterdata.h eseq.h
cluster_uniq_LDADD=libhpc-clust.la

#cluster_cmp_SOURCES=cluster-cmp.cpp cluster-common.cpp eseqcluster.cpp eseqclustercount.cpp
#cluster_check_SOURCES=cluster-check.cpp cluster-common.cpp eseqcluster.cpp eseqclustercount.cpp
#cluster_nj_SOURCES=cluster-nj.cpp cluster-common.cpp eseqcluster.cpp eseqclustercount.cpp

hpc_clust_mpi_SOURCES=hpc-clust-mpi.cpp eseqclusterdata.h eseq.h
hpc_clust_mpi_LDADD=libhpc-clust.la

#cluster_cooc_SOURCES=cluster-cooc.cpp cluster-common.cpp eseqcluster.cpp eseqclustercount.cpp

#cluster_profile_SOURCES=cluster-profile.cpp
cluster_fast_SOURCES=cluster-fast.cpp
cluster_fast_LDADD=libhpc-clust.la



