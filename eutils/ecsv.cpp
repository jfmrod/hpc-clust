
#include "ecsv.h"

#include "efile.h"
#include "eregexp.h"

#include "logger.h"
#include "eiostream.h"

void ecsv::load(estr filename)
{
  estr data;

  linfo("loading data from file: "+filename);
  efile(filename,"r").read(data);
  linfo("finished load, parsing...");
  parse(data);
}

void ecsv::parse(estr &data)
{
  int i;
  estr line;
  estrarray ffields;

  if (!data.getline(line)) return;

  emcsv_tokenize_first(line._str,line.len(),ffields);
  setfields(ffields);

  while (data.getline(line)){
    emcsv_tokenize_next(line._str,line.len(),ffields);
    for (i=0; i<cols.size() && i<ffields.size(); ++i)
      add(i,new estr(ffields.values(i)));
  }
  data.trim();
  if (data.len()){
    line=data;
    emcsv_tokenize_next(line._str,line.len(),ffields);
    add(ffields);
    for (i=0; i<cols.size() && i<ffields.size(); ++i)
      add(i,new estr(ffields.values(i)));
  }
  linfo("finished parsing");
}


void ecsv_tokenize(const estr &str, estrarray &tokens)
{
  estr tmps;

  int itoken;

  int i;

  itoken=0;

  bool inside_comma;
  bool inside_quotes;
  bool ignore_next_char;

  inside_comma=false;
  inside_quotes=false;
  ignore_next_char=false;
  for (i=0; i<str.len(); ++i){
    if (inside_quotes){
      if (ignore_next_char){
        switch(str[i]){
          case '\\': tmps+='\\'; break;
          case '"': tmps+='"'; break;
          case 'n': tmps+=(char)0x0A; break;
          case 'r': tmps+=(char)0x0D; break;
        }
        ignore_next_char=false;
      } else {
        if (str[i] == '\\') ignore_next_char=true; 
        else if (str[i] == '"'){
	  if (i+1 < str.len() && str[i+1] == '"')
	    { tmps+='"'; tmps+='"'; ++i; }
	  else 
            { inside_quotes=false; }
	}
        else {
          tmps+=str[i]; 
        }
      }
    } else if (inside_comma){
      if (str[i] == ',')
        { inside_comma=false; if (itoken>=tokens.size()) tokens.add(tmps); else tokens.values(itoken) = tmps; tmps=""; }
      else
	tmps += str[i];
    } else {
      if (str[i] == ',')
        { if (itoken >= tokens.size()) tokens.add(tmps); else tokens.values(itoken) = tmps;
	  ++itoken; tmps = ""; }
      else if (str[i] == '"')
        inside_quotes=true;
      else if (str[i] != ' ')
        { inside_comma=true; tmps+=str[i]; }
    }
  }
  if (!inside_quotes) {
    if (itoken>=tokens.size())
      tokens.add(tmps);
    else
      tokens.values(itoken) = tmps;
    ++itoken;
    tmps = "";
  }
}

void emcsv_tokenize_first(const char *str, int strlen, estrarray &tokens)
{
  estr tmps;

  int i;

  bool inside_quotes;
  bool inside_comma;
  bool ignore_next_char;

  inside_comma=false;
  inside_quotes=false;
  ignore_next_char=false;
  for (i=0; i<strlen; ++i){
    if (inside_quotes){
      if (ignore_next_char){
        switch(str[i]){
          case '\\': tmps+='\\'; break;
          case '"': tmps+='"'; break;
          case 'n': tmps+=(char)0x0A; break;
          case 'r': tmps+=(char)0x0D; break;
        }
        ignore_next_char=false;
      } else {
        if (str[i] == '\\') ignore_next_char=true; 
        else if (str[i] == '"'){
	  if (i+1 < strlen && str[i+1] == '"')
	    { tmps+='"'; tmps+='"'; ++i; }
	  else 
            { inside_quotes=false; }
	} else {
          tmps+=str[i]; 
        }
      }
    } else if (inside_comma){
      if (str[i] == ',')
        { inside_comma=false; tokens.add(tmps); tmps=""; }
      else
	tmps += str[i];
    } else {
      if (str[i] == ',')
        { tokens.add(tmps); tmps = ""; }
      else if (str[i] == '"')
        inside_quotes=true;
      else if (str[i] != ' ')
        { inside_comma=true; tmps+=str[i]; }
    }
  }
  if (!inside_quotes) { tokens.add(tmps); tmps = ""; }
}

void emcsv_tokenize_next(const char *str, int strlen, estrarray &tokens)
{
  estr *tmps;

  int itoken;

  int i;

  itoken=0;

  bool inside_quotes;
  bool inside_comma;
  bool ignore_next_char;

  inside_comma=false;
  inside_quotes=false;
  ignore_next_char=false;

  lddieif(!tokens.size(),"emcsv_tokenize_next: tokens is empty, cannot write next CSV in empty array");
  tmps = &tokens[0];
  tmps->clear();
  
  for (i=0; i<strlen; ++i){
    if (inside_quotes){
      if (ignore_next_char){
        switch(str[i]){
          case '\\': *tmps+='\\'; break;
          case '"': *tmps+='"'; break;
          case 'n': *tmps+=(char)0x0A; break;
          case 'r': *tmps+=(char)0x0D; break;
        }
        ignore_next_char=false;
      } else {
        switch(str[i]){
          case '\\': ignore_next_char=true; break;
          case '"':
            if (i+1 < strlen && str[i+1] == '"')
              { *tmps+='"'; *tmps+='"'; ++i; }
            else 
              { inside_quotes=false; }
           break;
          default:
            *tmps+=str[i]; 
        }
      }
    } else if (inside_comma){
      if (str[i] == ',')
        { inside_comma=false; ++itoken; if(itoken>=tokens.size()) return; tmps=&tokens[itoken]; tmps->clear(); }
      else
	*tmps += str[i];
    } else {
      switch(str[i]){
        case ',':
          ++itoken;
          if(itoken>=tokens.size()) return;
          tmps=&tokens[itoken];
          tmps->clear();
         break;
        case '"':
          inside_quotes=true; break;
        default:
          if (str[i] != ' ')
            { inside_comma=true; *tmps+=str[i]; }
      }
    }
  }
  if (!inside_quotes) {
    ++itoken; if(itoken>=tokens.size()) return; tmps=&tokens[itoken]; tmps->clear();
  }
}


