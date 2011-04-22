LDLIBS=`eutils-config --libs`
CXXFLAGS:=${CXXFLAGS} -pthread -march=nocona -mtune=nocona -O3 # -g # -pg 
#CXXFLAGS:=${CXXFLAGS} -pthread -pg # -g
.PHONY: all clean

all: cluster cluster-fast cluster-dist cluster-cooc cluster2 cluster-uniq cluster-sorted cluster-cmp cluster-nj cluster-check

clean:
	rm -f cluster cluster-fast cluster-dist cluster-cooc cluster2 cluster-uniq cluster-sorted cluster-cmp cluster-nj cluster-check *.o

cluster : cluster.cpp cluster-common.o eseqcluster.o
cluster-dist : cluster-dist.cpp cluster-common.o eseqcluster.o

cluster2 : cluster2.cpp cluster-common.o eseqcluster.o

cluster-sorted : cluster-sorted.cpp cluster-common.o eseqcluster.o
cluster-uniq : cluster-uniq.cpp cluster-common.o eseqcluster.o

cluster-cmp: cluster-cmp.cpp cluster-common.o eseqcluster.o
cluster-check: cluster-check.cpp cluster-common.o eseqcluster.o
cluster-nj: cluster-nj.cpp cluster-common.o eseqcluster.o

cluster-cooc : cluster-cooc.cpp
cluster-fast : cluster-fast.cpp

cluster-common.o : cluster-common.h cluster-common.cpp
eseqcluster.o : eseqcluster.h eseqcluster.h


