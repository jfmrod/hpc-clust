
#include "enet.h"

#include "../efile.h"
#include "../eregexp.h"
#include "../logger.h"

#include <fstream>


ostream &operator<<(ostream &stream,enet &net)
{
  int i,j;

  for (i=0; i<net.links.size(); ++i){
    if (net.links[i].info.size() && net.links[i].info[0].len())
      stream << estr(net.links[i].info) << ": ";
    stream << net.links[i].src->index;
    stream << " -> ";
    stream << net.links[i].dst->index;
    stream << endl;
  }
  return(stream);
}


/*
enet::enet(){}
enet::~enet(){}

void enet::load_fast(estr &data)
{
  estr line,left;

  elink* link;
  enode* node;
  estr nodestr;

  int i;
  double rrate;
  estr rratestr;
  estrarray part;
  estrarray infoarr;

  estr info;
  total_elements=0;

  lstart("enet::load_fast");

  lwarn("loading fast function");
  while (data.len()){
    line = data.getline();

    if (!line.len() || line[0]=='#') continue;

    i=line.find(':');
    if (i!=-1){
      info=line.substr(0,i);
      line.del(0,i+2);
    }

    infoarr = info;
    if (infoarr.size() && infoarr[0].len())
      link = &links.add(infoarr[0],elink());
    else
      link = &links.add("REACTION"+estr(links.size()),elink());
    link->info=infoarr;

    i=line.find("-->");
    if (i!=-1){ // irreversible right
      link->directed=true;
    } else {
      i=line.find("<=>");
      if (i==-1) { lerror("missing reaction direction --> or <=>"); continue; }
      link->directed=false;
    }

    left = line.substr(0,i-1);
    line.del(0,i+4);


    do {
      i=left.find("+");
      if (i!=-1){
        nodestr=left.substr(0,i-1);
        left.del(0,i+2);
      }else{
        nodestr=left;
        left="";
      }

      rrate = 1.0;
      i=nodestr.find("(");
      if (i!=-1){
        nodestr.del(0,i+1);
        i=nodestr.find(")");
        if (i==-1) { lerror("missing \")\" in reaction stoichiometric factor"); continue; }
        rrate=nodestr.d();
        nodestr.del(0,i+2);
      }
      node = getnode(nodestr);

      link->src.add(elinkelem(node,rrate));
      node->links.add(link);
      for (i=0; i<link->src.size(); ++i){
        if (node->nodes.find(link->src[i].node)==-1){
          node->nodes.add(link->src[i].node);
          link->src[i].node->nodes.add(node);
        }
      }
    } while(left.len());

    left=line;
    do {
      i=left.find("+");
      if (i!=-1){
        nodestr=left.substr(0,i-1);
        left.del(0,i+2);
      }else{
        nodestr=left;
        left="";
      }

      rrate = 1.0;
      i=nodestr.find("(");
      if (i!=-1){
        nodestr.del(0,i+1);
        i=nodestr.find(")");
        if (i==-1) { lerror("missing \")\" in reaction stoichiometric factor"); continue; }
        rrate=nodestr.d();
        nodestr.del(0,i+2);
      }
      node = getnode(nodestr);

      link->dst.add(elinkelem(node,rrate));
      node->links.add(link);
      for (i=0; i<link->src.size(); ++i){
        if (node->nodes.find(link->src[i].node)==-1){
          node->nodes.add(link->src[i].node);
          link->src[i].node->nodes.add(node);
        }
      }
      for (i=0; i<link->dst.size(); ++i){
        if (node->nodes.find(link->dst[i].node)==-1){
          node->nodes.add(link->dst[i].node);
          link->dst[i].node->nodes.add(node);
        }
      }
    } while(left.len());

    total_elements+=link->src.size() + link->dst.size();
  }
}

void enet::save(estr filename)
{
  ofstream file;

  file.open(filename._str);
  file << (*this);
  file.close();
}

void enet::load(estr filename)
{
  int predicted_no_reactions;
  estr data;
  efile file(filename);

  predicted_no_reactions = file.size()/42;    // based on some examples, the average chars per reaction is between 47 and 38, we use this to allocate the array

  links.reserve(predicted_no_reactions);
  nodes.reserve((int)(predicted_no_reactions));

  file.read(data);

  if (data.substr(0,8) == "#emetnet")
    load_fast(data);
  else
    load_robust(data);

  int i;
  for (i=0; i<links.size(); ++i){
    links[i].i = i;
  }
}

void enet::addLink(estr line)
{
  elink* link;
  enode* node;
  estr nodestr;

  
  int i;
  double rrate;
  estr rratestr;

  estrarray part;
  eregexp re("^ *((\\([0-9]+(\\.[0-9]+)?([Ee]-?[0-9]+)?\\))? *|[0-9]+(\\.[0-9]+)?([Ee]-?[0-9]+)? +)[^+ ]+ *\\+");
  eregexp rerate("^ *(\\([0-9]+(\\.[0-9]+)?([Ee]-?[0-9]+)?\\) *|[0-9]+(\\.[0-9]+)?([Ee]-?[0-9]+)? +)");

  eregexp re_irrev_right("[^-<=](-|=)+>");
  eregexp re_irrev_left("<(-|=)+[^->=]");
  eregexp re_rev("<(-|=)+>");

  eregexp re_sepeq("(<?(-|=)+>|<(-|=)+>?)");

  eregexp re_info("^[^:]+:");

  estr info;
  estrarray infoarr;

  earray<elinkelem> *src;
  earray<elinkelem> *dst;

  line.trim();
  if (!line.len()) return;

  info = re_match(line,re_info);
  line.del(0,info.len());
  info.del(-1);

  infoarr = info;

  if (infoarr.size() && infoarr[0].len())
    link = &links.add(infoarr[0],elink());
  else
    link = &links.add("REACTION"+estr(links.size()),elink());
  link->info = info;

  src     = &link->src;
  dst     = &link->dst;
  if (re_strpos(line,re_irrev_right)!=-1) // directed right
    link->directed=true;
  else if (re_strpos(line,re_irrev_left)!=-1) // directed left
    { link->directed=true; src = &link->dst; dst=&link->src; }
  else
    link->directed=false;

  part = re_explode(line,re_sepeq);
  lddieif(part.size()!=2," reaction separator \"->\" not found, parsing file on line: "+estr((int)links.size()));

  line = part.values(0);
  do {
    nodestr = re_match(line,re);
    if (nodestr.len()){
      line.del(0,nodestr.len());
      nodestr.del(-1);
    }else{
      nodestr=line;
      line="";
    }
    rrate = 1.0;
    rratestr = re_match(nodestr,rerate);
    nodestr.del(0,rratestr.len());
    rratestr.trim();
    if (rratestr.len() && rratestr[0]=='(')
      { rratestr.del(0,1); rratestr.del(-1); } // remove (,)'s from (rate)
    if (rratestr.len())
      rrate=rratestr.d();

    nodestr.trim();
    node = getnode(nodestr);

    src->add(elinkelem(node,rrate));
    node->links.add(link);
    for (i=0; i<src->size(); ++i){
      if (node->nodes.find(src->at(i).node)==-1){
        node->nodes.add(src->at(i).node);
        src->at(i).node->nodes.add(node);
      }
    }
    linfo("srcnode: "+nodestr);
  } while(line.len());

  line = part.values(1);
  do {
    nodestr = re_match(line,re);
    if (nodestr.len()){
      line.del(0,nodestr.len());
      nodestr.del(-1);
    }else{
      nodestr=line;
      line="";
    }

    rrate = 1.0;
    rratestr = re_match(nodestr,rerate);
    nodestr.del(0,rratestr.len());
    rratestr.trim();
    if (rratestr.len() && rratestr[0]=='(')
      { rratestr.del(0,1); rratestr.del(-1); } // remove (,)'s from (rate)
    if (rratestr.len())
      rrate=rratestr.d();

    nodestr.trim();
    node = getnode(nodestr);

    dst->add(elinkelem(node,rrate));
    node->links.add(link);
    for (i=0; i<src->size(); ++i){
      if (node->nodes.find(src->at(i).node)==-1){
        node->nodes.add(src->at(i).node);
        src->at(i).node->nodes.add(node);
      }
    }
    for (i=0; i<dst->size(); ++i){
      if (node->nodes.find(dst->at(i).node)==-1){
        node->nodes.add(dst->at(i).node);
        dst->at(i).node->nodes.add(node);
      }
    }
    linfo("dstnode: "+nodestr);
  } while(line.len());
}

void enet::load_robust(estr &data)
{
  linfo("loading robust function");

  data=re_replace(data,"#[^\n]*\n","\n");

  estr line;

  elink* link;
  enode* node;
  estr nodestr;

  
  int i;
  double rrate;
  estr rratestr;

  estrarray part;

//  eregexp re("^( *\\([0-9]*\\.?[0-9]+\\))? *[^+ ]+ *\\+");
  eregexp re("^ *((\\([0-9]+(\\.[0-9]+)?([Ee]-?[0-9]+)?\\))? *|[0-9]+(\\.[0-9]+)?([Ee]-?[0-9]+)? +)[^+ ]+ *\\+");
  eregexp rerate("^ *(\\([0-9]+(\\.[0-9]+)?([Ee]-?[0-9]+)?\\) *|[0-9]+(\\.[0-9]+)?([Ee]-?[0-9]+)? +)");

  eregexp re_irrev_right("[^-<=](-|=)+>");
  eregexp re_irrev_left("<(-|=)+[^->=]");
  eregexp re_rev("<(-|=)+>");

  eregexp re_sepeq("(<?(-|=)+>|<(-|=)+>?)");

  eregexp re_info("^[^:]+:");

  estr info;
  estrarray infoarr;
  earray<elinkelem> *src;
  earray<elinkelem> *dst;


  total_elements=0;
  lstart("enet::load_robust");
  lwarn("loading with robust (slower) function, if you want to have faster loading times, please convert this file with metnet-conv");
  while (data.len()){
    line = data.getline();

    line.trim();
    if (!line.len()) continue;


    info = re_match(line,re_info);
    line.del(0,info.len());
    info.del(-1);
    infoarr = info;

    if (infoarr.size() && infoarr[0].len())
      link = &links.add(infoarr[0],elink());
    else
      link = &links.add("REACTION"+estr(links.size()),elink());
//    link = &links.add(elink());
    link->info = infoarr;

    src     = &link->src;
    dst     = &link->dst;
    if (re_strpos(line,re_irrev_right)!=-1) // directed right
      link->directed=true;
    else if (re_strpos(line,re_irrev_left)!=-1) // directed left
      { link->directed=true; src = &link->dst; dst=&link->src; }
    else
      link->directed=false;

    part = re_explode(line,re_sepeq);
    lddieif(part.size()!=2," reaction separator \"->\" not found, parsing file on line: "+estr((int)links.size()));

    line = part.values(0);
    do {
      nodestr = re_match(line,re);
      if (nodestr.len()){
        line.del(0,nodestr.len());
        nodestr.del(-1);
      }else{
        nodestr=line;
        line="";
      }
      rrate = 1.0;
      rratestr = re_match(nodestr,rerate);
      nodestr.del(0,rratestr.len());
      rratestr.trim();
      if (rratestr.len() && rratestr[0]=='(')
        { rratestr.del(0,1); rratestr.del(-1); } // remove (,)'s from (rate)
      if (rratestr.len())
        rrate=rratestr.d();

      nodestr.trim();
      node = getnode(nodestr);

      src->add(elinkelem(node,rrate));
      node->links.add(link);
      for (i=0; i<src->size(); ++i){
        if (node->nodes.find(src->at(i).node)==-1){
          node->nodes.add(src->at(i).node);
          src->at(i).node->nodes.add(node);
        }
      }
      linfo("srcnode: "+nodestr);
    } while(line.len());

    line = part.values(1);
    do {
      nodestr = re_match(line,re);
      if (nodestr.len()){
        line.del(0,nodestr.len());
        nodestr.del(-1);
      }else{
        nodestr=line;
        line="";
      }

      rrate = 1.0;
      rratestr = re_match(nodestr,rerate);
      nodestr.del(0,rratestr.len());
      rratestr.trim();
      if (rratestr.len() && rratestr[0]=='(')
        { rratestr.del(0,1); rratestr.del(-1); } // remove (,)'s from (rate)
      if (rratestr.len())
        rrate=rratestr.d();

      nodestr.trim();
      node = getnode(nodestr);

      dst->add(elinkelem(node,rrate));
      node->links.add(link);
      for (i=0; i<src->size(); ++i){
        if (node->nodes.find(src->at(i).node)==-1){
          node->nodes.add(src->at(i).node);
          src->at(i).node->nodes.add(node);
        }
      }
      for (i=0; i<dst->size(); ++i){
        if (node->nodes.find(dst->at(i).node)==-1){
          node->nodes.add(dst->at(i).node);
          dst->at(i).node->nodes.add(node);
        }
      }
      linfo("dstnode: "+nodestr);
    } while(line.len());

    total_elements+=link->src.size() + link->dst.size();
  }
}

// get node with id "nodeid", if it is not in the list, create a new node with id
enode* enet::getnode(const estr &nodeid)
{
  enode *node;
  node = &nodes[nodeid];
  if (!node->id.len()){
    node->i=nodes.size()-1;
    node->id=nodeid;
  }
  return(node);
}

void enet::addLink(const elink& newlink)
{
  elink* link;

  if (newlink.info.size() && newlink.info[0].len())
    link = &links.add(newlink.info[0],elink());
  else
    link = &links.add("REACTION"+estr(links.size()),elink());
  link->info   = newlink.info;
  link->directed=newlink.directed;

  enode* node;
  int i;
  for (i=0; i<newlink.src.size(); ++i){
    node = getnode(newlink.src[i].node->id);
    link->src.add(elinkelem(node,newlink.src[i].rate)); 
    node->links.add(link);
  }
  for (i=0; i<newlink.dst.size(); ++i){
    node = getnode(newlink.dst[i].node->id);
    link->dst.add(elinkelem(node,newlink.dst[i].rate)); 
    node->links.add(link);
  }
}

float getRate(elink& link, enode& node)
{
  int i;
  for (i=0; i<link.src.size(); ++i){
    if (link.src[i].node == &node)
      return(-link.src[i].rate);
  }
  for (i=0; i<link.dst.size(); ++i){
    if (link.dst[i].node == &node)
      return(link.dst[i].rate);
  }
  return(0.0);
}



ostream &operator<<(ostream &stream,enet &net)
{
  int i,j;

  stream << "#emetnet" << endl;

  for (i=0; i<net.links.size(); ++i){
    if (!net.links[i].src.size() || !net.links[i].dst.size()) continue;

    if (net.links[i].info.size() && net.links[i].info[0].len())
      stream << estr(net.links[i].info) << ": ";
    for (j=0; j<net.links[i].src.size()-1; ++j){
      if (net.links[i].src[j].rate!=1.0)
        stream << "("<< net.links[i].src[j].rate <<") ";
      stream << net.links[i].src[j].node->id << " + ";
    }
    if (net.links[i].src.size()){
      if (net.links[i].src[j].rate!=1.0)
        stream << "("<< net.links[i].src[j].rate <<") ";
      stream << net.links[i].src[j].node->id;
    }

    if (net.links[i].directed)
      stream << " --> ";
    else
      stream << " <=> ";

    for (j=0; j<net.links[i].dst.size()-1; ++j){
      if (net.links[i].dst[j].rate!=1.0)
        stream << "("<< net.links[i].dst[j].rate <<") ";
      stream << net.links[i].dst[j].node->id << " + ";
    }
    if (net.links[i].dst.size()){
      if (net.links[i].dst[j].rate!=1.0)
        stream << "("<< net.links[i].dst[j].rate <<") ";
      stream << net.links[i].dst[j].node->id;
    }

    stream << endl;
  }
  return(stream);
}

ostream &operator<<(ostream &stream,elink &link)
{
  int j;

  if (link.info.size() && link.info[0].len())
    stream << estr(link.info) << ": ";
  for (j=0; j<link.src.size()-1; ++j){
    if (link.src[j].rate!=1.0)
      stream << "("<< link.src[j].rate <<") ";
    stream << link.src[j].node->id << " + ";
  }
  if (link.src.size()){
    if (link.src[j].rate!=1.0)
      stream << "("<< link.src[j].rate <<") ";
    stream << link.src[j].node->id;
  }

  if (link.directed)
    stream << " --> ";
  else
    stream << " <=> ";

  for (j=0; j<link.dst.size()-1; ++j){
    if (link.dst[j].rate!=1.0)
      stream << "("<< link.dst[j].rate <<") ";
    stream << link.dst[j].node->id << " + ";
  }
  if (link.dst.size()){
    if (link.dst[j].rate!=1.0)
      stream << "("<< link.dst[j].rate <<") ";
    stream << link.dst[j].node->id;
  }

  return(stream);
}

enode::enode(): i(-1) {}
enode::enode(const estr &_id): id(_id) {}

enode &enode::operator=(const enode &node)
{ id = node.id; return(*this); }

bool enode::operator==(const enode &node) const
{ return(id==node.id); }


elinkelem::elinkelem() {}
elinkelem::elinkelem(enode *_node,double _rate): node(_node),rate(_rate){}
*/

