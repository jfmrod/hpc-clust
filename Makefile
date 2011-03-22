LDLIBS=`eutils-config --libs`
CXXFLAGS:=${CXXFLAGS} -pthread -march=nocona -mtune=nocona -O3 # -pg # -g
#CXXFLAGS:=${CXXFLAGS} -pthread -pg # -g

all: cluster cluster-fast cluster-dist cluster-cooc cluster2 cluster-uniq cluster-sorted cluster-cmp cluster-nj

cluster : cluster.cpp cluster-common.o
cluster-common.o : cluster-common.cpp cluster-common.h
cluster2 : cluster2.cpp
cluster-cooc : cluster-cooc.cpp
cluster-dist : cluster-dist.cpp cluster-common.o
cluster-sorted : cluster-sorted.cpp cluster-common.o
cluster-uniq : cluster-uniq.cpp cluster-common.o
cluster-fast : cluster-fast.cpp

cluster-cmp: cluster-cmp.cpp cluster-common.o
cluster-nj: cluster-nj.cpp cluster-common.o

