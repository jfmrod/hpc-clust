LDLIBS=`eutils-config --libs`
CC=g++
CXXFLAGS:=${CXXFLAGS} -pthread -march=nocona -mtune=nocona -O3 # -g # -pg 
#CXXFLAGS:=${CXXFLAGS} -pthread -pg # -g

.PHONY: all clean

targets=cluster cluster-mpi cluster-fast cluster-dist cluster-cooc cluster2 cluster-uniq cluster-sorted cluster-cmp cluster-nj cluster-check cluster-partition

all: $(targets)

clean:
	rm -f $(targets) *.o

cluster : cluster.o cluster-common.o eseqcluster.o eseqclustercount.o eseqclustersingle.o

cluster-dist : cluster-dist.o cluster-common.o eseqcluster.o eseqclustercount.o


cluster-partition : cluster-partition.o cluster-common.o eseqcluster.o eseqclustercount.o

cluster2 : cluster2.o cluster-common.o eseqcluster.o eseqclustercount.o

cluster-sorted : cluster-sorted.o cluster-common.o eseqcluster.o eseqclustercount.o
cluster-uniq : cluster-uniq.o cluster-common.o eseqcluster.o eseqclustercount.o


cluster-cmp: cluster-cmp.o cluster-common.o eseqcluster.o eseqclustercount.o
cluster-check: cluster-check.o cluster-common.o eseqcluster.o eseqclustercount.o
cluster-nj: cluster-nj.o cluster-common.o eseqcluster.o eseqclustercount.o

cluster-mpi : cluster-mpi.o cluster-common.o eseqcluster.o eseqclustercount.o
	g++ `mpic++ --showme:compile` $(CXXFLAGS) -o $@ $^ $(LDLIBS) `mpic++ --showme:link`

cluster-cooc : cluster-cooc.o cluster-common.o eseqcluster.o eseqclustercount.o

cluster-fast : cluster-fast.o


cluster-cooc.o : cluster-cooc.cpp cluster-common.h eseqcluster.h eseqclustercount.h
cluster-common.o : eseqcluster.h eseqclustercount.h eseqclustersingle.h cluster-common.h cluster-common.cpp
eseqcluster.o : eseqcluster.h eseqcluster.cpp
eseqclustercount.o : eseqclustercount.h eseqclustercount.cpp
eseqclustersingle.o : eseqcluster.h eseqclustersingle.h eseqclustersingle.cpp

