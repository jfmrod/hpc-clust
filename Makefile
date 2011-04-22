LDLIBS=`eutils-config --libs`
CXXFLAGS:=${CXXFLAGS} -pthread -g # -march=nocona -mtune=nocona -O3 # -g # -pg 
#CXXFLAGS:=${CXXFLAGS} -pthread -pg # -g
.PHONY: all clean

all: cluster cluster-fast cluster-dist cluster-cooc cluster2 cluster-uniq cluster-sorted cluster-cmp cluster-nj cluster-check

clean:
	rm -f cluster cluster-fast cluster-dist cluster-cooc cluster2 cluster-uniq cluster-sorted cluster-cmp cluster-nj cluster-check *.o

cluster : cluster.cpp cluster-common.o eseqcluster.o eseqclustercount.o

cluster-dist : cluster-dist.cpp cluster-common.o eseqcluster.o eseqclustercount.o


cluster2 : cluster2.cpp cluster-common.o eseqcluster.o eseqclustercount.o

cluster-sorted : cluster-sorted.cpp cluster-common.o eseqcluster.o eseqclustercount.o
cluster-uniq : cluster-uniq.cpp cluster-common.o eseqcluster.o eseqclustercount.o

cluster-cmp: cluster-cmp.cpp cluster-common.o eseqcluster.o eseqclustercount.o
cluster-check: cluster-check.cpp cluster-common.o eseqcluster.o eseqclustercount.o
cluster-nj: cluster-nj.cpp cluster-common.o eseqcluster.o eseqclustercount.o

cluster-cooc : cluster-cooc.cpp
cluster-fast : cluster-fast.cpp

cluster-common.o : eseqcluster.h eseqclustercount.h cluster-common.h cluster-common.cpp
eseqcluster.o : eseqcluster.h eseqcluster.cpp
eseqclustercount.o : eseqclustercount.h eseqclustercount.cpp


