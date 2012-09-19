#include "etable.h"
#include "logger.h"

#include "estrarray.h"
#include "earray.h"
#include "estr.h"
#include "evar.h"
#include "efile.h"

#include "eparserinterpreter.h"

etable loadWSV(const estr& filename,const eregexp& re)
{
  int i;
  etable t;
  efile f;
  if (!f.open(filename,"r")) { lerror(filename); return(t); }
  
  estr line;
  estrarray arr;
  while (f.readln(line)){
    arr=re_explode(line,re);
    for (i=0; i<arr.size(); ++i)
      t.add(estr(i),new estr(arr[i]));
  }
  return(t);
}

etable::etable()
{
}

etable::etable(const estrarray &fields)
{
  setfields(fields);
}

etable::etable(const etable &table)
{
  operator=(table);
}

etable::~etable()
{
  clear();
}

etable& etable::operator=(const etable& table)
{
  cols=table.cols;
  return(*this);
}

erow etable::row(int i)
{
  return(erow(this,i));
}

etable etable::filter(const estr& expression)
{
  int i,j;
  etable t;
  estrhashof<evar> env;

  for (i=0; i<cols.size(); ++i){
    t.addfield(cols.keys(i));
    env.add(cols.keys(i),0x00);
  }

  for (i=0; i<size(); ++i){
    for (j=0; j<cols.size(); ++j)
      env.values(j)=&cols.values(j).at(i);
    if (code_interpret(env,expression).get<bool>()==true){
      for (j=0; j<cols.size(); ++j)
        t.cols.values(j).add(cols.values(j).at(i));
    }
  }
  return(t);
}

void etable::serial(estr& data) const
{
  int i,j;
  serialint(cols.size(),data);
  for (i=0; i<cols.size(); ++i)
    cols.keys(i).serial(data);
  serialint(size(),data);
  for (i=0; i<size(); ++i)
    for (j=0; j<cols.size(); ++j)
      cols.values(j)[i].serial(data);
}

int etable::unserial(const estr& data,int i)
{
  clear();

  unsigned int count;
  int j;

  estr field;
  i=unserialint(count,data,i);
  if (i==-1) return(-1);
  while(count-- && i<data.len()){
    i=field.unserial(data,i);
    if (i==-1) return(-1);
    addfield(field);
  }
  evar var;
  i=unserialint(count,data,i);
  if (i==-1) return(-1);
  while(count-- && i<data.len()){
    for (j=0; j<cols.size(); ++j){ 
      i=var.unserial(data,i);
      if (i==-1) return(-1);
      cols.values(j).add(var);
    }
  }
  return(i);
}




void etable::add(int i,const evar& var)
{
  lddieif(i>=cols.size(),"adding value to non-existent column: "+estr(i));
  if (cols.values(0).size()==0 || !cols[i][cols[i].size()-1].isNull()){
//    cout << " adding row " << field << endl;
    int i;
    for (i=0; i<cols.size(); ++i)
      cols.values(i).add(evar());
  }
//  cout << " adding var: "<<var<<" to field: " << field << endl;
  cols[i][cols.values(0).size()-1]=var.var;
}


void etable::add(const estr& field,const evar& var)
{
  if (cols.size()==0 || cols.findkey(field)==-1){
//    cout << " adding field: " << field << endl;
    addfield(field);
  }
  if (cols.values(0).size()==0 || !cols[field][cols[field].size()-1].isNull()){
//    cout << " adding row " << field << endl;
    int i;
    for (i=0; i<cols.size(); ++i)
      cols.values(i).add(evar());
  }
//  cout << " adding var: "<<var<<" to field: " << field << endl;
  cols[field][cols.values(0).size()-1]=var.var;
}

void etable::add(const estrarray& arr)
{
  int i;
  if (cols.size()==0){
    for (i=0; i<arr.size(); ++i)
      addfield(arr.keys(i));
  }
  for (i=0; i<cols.size() && i<arr.size(); ++i)
    cols[arr.keys(i)].add(new estr(arr.values(i)));
}

void etable::addfield(const estr& field)
{
  cols.add(field,evararray());
  int i;
  for (i=0; i<cols.values(0).size(); ++i) cols[field].add(evar());
}

void etable::setfields(const estrarray& fields)
{
  int i;
  for (i=0; i<cols.size() && i<fields.size(); ++i)
    cols.keys(i)=fields[i];
  for (; i<fields.size(); ++i)
    addfield(fields[i]);
}

evararray& etable::operator[](int i)
{
  return(cols[i]);
}
const evararray& etable::operator[](int i) const
{
  return(cols[i]);
}

evararray& etable::operator[](const estr& field)
{
  return(cols[field]);
}
const evararray& etable::operator[](const estr& field) const
{
  return(cols.at(field));
}


void etable::save(efile file) const
{
//  if (!file.open("w")) { lwarn("unable to open file: \""+file.name+"\" for writing"); return; }

  if (cols.size()==0) return;

  int i,j;

  for (j=0; j<cols[0].size(); ++j){
    for (i=0; i<cols.size(); ++i){
      file.write(cols[i][j]);
      file.write(" ");
    }
    file.write("\n");
  }
  file.close();
}

void etable::load(efile file)
{
  clear();

//  if (!file.open("r")) { lwarn("unable to open file: \""+file.name+"\""); return; }

  int i;
  estr s;
  estrarray fields;

  if (file.readln(s)){
    fields=s.explode(" ");
    for (i=0; i<fields.size(); ++i)
      addfield(cols.size());

    for (i=0; i<cols.size() && i<fields.size(); ++i)
      cols[i].add(fields[i]);
  }
  while (file.readln(s)){
    fields=s.explode(" ");
    for (i=0; i<cols.size() && i<fields.size(); ++i)
      cols[i].add(fields[i]);
  }

  file.close();
}

void etable::clear()
{
  cols.clear();
}

erow::erow(): table(0x00),index(0) {}
erow::erow(etable *_table,int _index): table(_table),index(_index) {}

unsigned int erow::size() const
{
  return(table->cols.size());
}

const evar& erow::operator[](int i) const
{
  return(table->cols[i][index]);
}

evar& erow::operator[](int i)
{
  return(table->cols[i][index]);
}

const evar &erow::operator[](const estr& field) const
{
  return(table->cols[field][index]);
}

evar& erow::operator[](const estr &field)
{
  return(table->cols[field][index]);
}


ostream& operator<<(ostream& stream,const erow& row)
{
  if (row.size()==0) return(stream);

  int i;
  stream << row[0];
  for (i=1; i<row.size(); ++i)
    stream << " " << row[i];

  return(stream);
}

#include <sstream>

ostream& operator<<(ostream& stream,const etable& table)
{
  int i,c,r,m;
  eintarray maxlen;
  estr line,tmp;
  
  if (table.cols.size()==0){
    stream << "< empty table >" << endl;
    return(stream);
  }

  for (i=0 ; i < table.cols.size() ; i++)
    maxlen.add(table.cols.keys(i).len());

  stringstream ss;
  for (r = 0 ; r < table.size() ; r ++)
    for ( c = 0 ; c < table.cols.size() ; c++){
      ss.str("");
      ss << table[c][r];
      if ( ( m = ss.str().length())  > maxlen[c]) 
        maxlen[c] = m;
    }

  for( c = 0; c < table.cols.size() ; c++){
    line += "+";
    for ( i=0 ; i < maxlen[c]+2 ; i++ )
      line += "-";
  }
    
  line += "+\n";  
  stream << line << "|";
    
  for ( i = 0 ; i < table.cols.size() ; i++ ){ 
    tmp.sprintf(" %-*s|",maxlen[i]+1,table.cols.keys(i)._str);
    stream << tmp;
  }

  stream << endl << line;
  
  for ( r = 0 ; r < table.size() ; r++ ) {
    stream << "|";
    for( c = 0 ; c < table.cols.size() ; c++ ) {
      ss.str("");
//      ss.clear();
      ss << table[c][r];
      
//      tmp.sprintf(" %-*s|",maxlen[c]+1,table[c][r]._str);
      stream << " " <<ss.str().substr(0,maxlen[c]+1);
      for (i=0; i<maxlen[c]+1-ss.str().length(); ++i) stream << " ";
      stream << "|";
    }
    stream << endl;   
  }     
    
  if ( table.size() > 0 )
    stream << line; 
      
  return(stream);
}

