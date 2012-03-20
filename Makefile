LDLIBS=`eutils-config --libs`
LDFLAGS:=${LDFLAGS} -pthread
CC=g++
CXXFLAGS:=${CXXFLAGS} -pthread -march=nocona -mtune=nocona -O3 # -g # -pg 

.PHONY: all clean

targets=cluster-short cluster-partition2 cluster cluster-profile cluster-fast cluster-cooc cluster2 cluster-uniq cluster-sorted cluster-cmp cluster-check cluster-partition cluster-dist2 cluster-evolve cluster-regen

all: $(targets)

clean:
	rm -f $(targets) *.o

cluster-evolve: cluster-evolve.o seq-profile.o
seq-profile.o : seq-profile.h seq-profile.cpp


cluster-regen: cluster-regen.o seq-profile.o ../sali/etree.o

cluster-regen.o: cluster-regen.cpp seq-profile.h ../sali/etree.h




cluster : cluster.o cluster-common.o eseqcluster.o eseqclustercount.o eseqclustersingle.o eseqclusteravg.o
cluster-short : cluster-short.o cluster-common.o

cluster-dist : cluster-dist.o cluster-common.o eseqcluster.o eseqclustercount.o
cluster-dist2 : cluster-dist2.o cluster-common.o eseqcluster.o eseqclustercount.o eseqclustersingle.o eseqclusteravg.o


cluster-partition : cluster-partition.o cluster-common.o eseqcluster.o eseqclustercount.o
cluster-partition2 : cluster-partition2.o cluster-common.o eseqcluster.o eseqclustercount.o

cluster2 : cluster2.o cluster-common.o eseqcluster.o eseqclustercount.o

cluster-sorted : cluster-sorted.o cluster-common.o eseqcluster.o eseqclustercount.o eseqclustersingle.o eseqclusteravg.o
cluster-uniq : cluster-uniq.o cluster-common.o eseqcluster.o eseqclustercount.o


cluster-cmp: cluster-cmp.o cluster-common.o eseqcluster.o eseqclustercount.o
cluster-check: cluster-check.o cluster-common.o eseqcluster.o eseqclustercount.o
cluster-nj: cluster-nj.o cluster-common.o eseqcluster.o eseqclustercount.o

cluster-mpi : cluster-mpi.cpp cluster-common.o eseqcluster.o eseqclustercount.o eseqclustersingle.o eseqclusteravg.o
	g++ `mpic++ --showme:compile` $(CXXFLAGS) -o $@ $^ $(LDLIBS) `mpic++ --showme:link`

cluster-cooc : cluster-cooc.o cluster-common.o eseqcluster.o eseqclustercount.o

cluster-profile : cluster-profile.o
cluster-fast : cluster-fast.o


cluster-cooc.o : cluster-cooc.cpp cluster-common.h eseqcluster.h eseqclustercount.h
cluster-common.o : eseqcluster.h eseqclustercount.h eseqclustersingle.h cluster-common.h cluster-common.cpp
eseqcluster.o : eseqcluster.h eseqcluster.cpp
eseqclustercount.o : eseqclustercount.h eseqclustercount.cpp
eseqclustersingle.o : eseqcluster.h eseqclustersingle.h eseqclustersingle.cpp
eseqclusteravg.o : eseqcluster.h eseqclusteravg.h eseqclusteravg.cpp

