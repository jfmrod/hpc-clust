LDLIBS=`eutils-config --libs`
CXXFLAGS:=${CXXFLAGS} -pthread -march=nocona -mtune=nocona -O3 # -g # -pg 
#CXXFLAGS:=${CXXFLAGS} -pthread -pg # -g
.PHONY: all clean

all: cluster cluster-mpi cluster-fast cluster-dist cluster-cooc cluster2 cluster-uniq cluster-sorted cluster-cmp cluster-nj cluster-check

clean:
	rm -f cluster cluster-fast cluster-dist cluster-cooc cluster2 cluster-uniq cluster-sorted cluster-cmp cluster-nj cluster-check *.o

cluster : cluster.cpp cluster-common.o
cluster-common.o : cluster-common.cpp cluster-common.h
cluster2 : cluster2.cpp
cluster-cooc : cluster-cooc.cpp
cluster-dist : cluster-dist.cpp cluster-common.o
cluster-mpi : cluster-mpi.cpp cluster-common.o
	g++ `mpic++ --showme:compile` $(CXXFLAGS) -o $@ $^ $(LDLIBS) `mpic++ --showme:link`

cluster-sorted : cluster-sorted.cpp cluster-common.o
cluster-uniq : cluster-uniq.cpp cluster-common.o
cluster-fast : cluster-fast.cpp

cluster-cmp: cluster-cmp.cpp cluster-common.o
cluster-check: cluster-check.cpp cluster-common.o
cluster-nj: cluster-nj.cpp cluster-common.o

