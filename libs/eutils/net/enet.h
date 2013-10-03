#ifndef ENET_H
#define ENET_H

class eplink;

#include "../estr.h"
#include "../earrayof.h"
#include "../estrhashof.h"

class epnode
{
 public:
  unsigned int index;
  earrayof<eplink*,epnode*> links;
};

class eplink
{
 public:
  epnode* src;
  epnode* dst;
};

template <class N,class L>
class epnet
{
 public:
  estrhashof<N> nodes;
  estrhashof<L> links;

  N& addNode(N* node=0x00);
  L& addLink(epnode& src,epnode& dst);
};

#include "../estrhashof.h"

template <class N,class L>
N& epnet<N,L>::addNode(N* node)
{
  if (node==0x00) node = new N;
  node->index=nodes.size();
  nodes.addref(estr((int)node->index),node);
}

template <class N,class L>
L& epnet<N,L>::addLink(epnode& src,epnode& dst)
{
  L *newlink=new L;
  newlink->src=&src;
  newlink->dst=&dst;
  links.addref(estr(links.size()),newlink);
  src.links.add(&dst,static_cast<eplink*>(newlink));
  dst.links.add(&src,static_cast<eplink*>(newlink));
}

class enode : public epnode
{
 public:
  estrarray info;
};

class elink : public eplink
{
 public:
  estrarray info;
};

typedef epnet<enode,elink> enet;

/*
class ehyperlinknode
{
 public:
  enode* node;
  double rate;

  elinkelem();
  elinkelem(enode *node,double rate);
};
class ehyperlink
{
 public:
  estrarray info;
  earray<ehyperlinknode> src;
  earray<ehyperlinknode> dst;
};
*/

class ehyperlink
{
 public:
  earray<enode*> srcnodes;
  earray<enode*> dstnodes;
};

class ehypernet
{
 public:
  estrhashof<enode>      nodes;
  estrhashof<ehyperlink> links;
};

/*
class enet
{
 public:
  estrhashof<enode> nodes;
  estrhashof<elink> links;
  int total_elements;

  enode* getnode(const estr &nodeid);

  enet();
  ~enet();

  void addLink(estr line);
  void addLink(const elink& link);

  void getActive(eintarray& arr);
  void setActive(eintarray& arr);

  void save(estr filename);
  void load(estr filename);
  void load_fast(estr &data);
  void load_robust(estr &data);
};
*/

ostream &operator<<(ostream &stream,enet &net);
ostream &operator<<(ostream &stream,elink &link);

#endif

