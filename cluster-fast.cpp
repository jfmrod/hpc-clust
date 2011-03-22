#include <eutils/emain.h>
#include <eutils/efile.h>
#include <eutils/eheap.h>

inline int mindist(const estrhash& arr,int ind,int len,eintarray tmp)
{
  int count=0;
  int maxlen=0;
  int i,j;
  for (j=0; j<tmp.size(); ++j) tmp[j]=0;

  for (i=0; i<len; ++i){
    for (j=0; j<arr.size(); ++j){
      if (ind==j || tmp[j]<0) continue;
      if (maxlen>tmp[j]+(len-i)) { tmp[j]=-i; } // conservative approach, worst-case: all next nucleotides will match
//      if (i>60 && maxlen>tmp[j]+(len-i)*tmp[j]/i) { tmp[j]=-i; } // heuristic, we expect next nucleotides to match based on the average until now
//      if (i>100 && (float)maxlen*0.9/i>(float)tmp[j]/i) { tmp[j]=-i; } // heuristic, if the rate of matches of the seq is 90% lower than the best stop checking it

      if (arr.values(ind)[i]==arr.values(j)[i] && arr.values(ind)[i]!='-') ++tmp[j];
      if (maxlen<tmp[j]) maxlen=tmp[j];
      ++count;
    }
  }
  cout << ind << " "<< maxlen << " (" << (float)maxlen/len << ") " << (float)count/((arr.size()-1)*len) << endl;
  return(maxlen);
}

inline float dist(const estr& s1,const estr& s2)
{
  int i;
  int count=0;
  for (i=0; i<s1.len(); ++i){
    if (s1[i]==s2[i]) ++count;
  }
  return((float)count/(float)s1.len());
}

void consensus(const estrhash& arr,eintarray& cons,int len)
{
  int i,j;
  estrhashof<int> tmp;
  for (i=0; i<len; ++i){
    tmp.clear();
    ldieif(tmp.size()>0,"tmp.size");
    for (j=0; j<arr.size(); ++j){
      if (tmp.exists(arr.values(j)[i]))
        ++tmp[estr(arr.values(j)[i])];
      else if (arr.values(j)[i]!='-')
        tmp.add(arr.values(j)[i],0);
    }
    ldieif(tmp.size()==0,"");
    cons[i]=tmp.values(0);
    for (j=1; j<tmp.size(); ++j)
      if (cons[i]<tmp.values(j)) cons[i]=tmp.values(j);
  }
}

void sortstr(estr& str,eintarray sorti)
{
  ldieif(str.len()!=sorti.size(),"size mismatch");
  int i;
  estr tmp;
//  for (i=0; i<sorti.size(); ++i)
  for (i=sorti.size()-1; i>0; --i)
    tmp+=str[sorti[i]];
  str=tmp; 
}

int emain()
{
  eparseArgs(argvc,argv);

  ldieif(argvc<2,"syntax: "+efile(argv[0]).basename()+" <file>");


  estr line;
  estr name;
  efile f(argv[1]);

  int i,i2,j;
  estrhash arr;

  while (f.readln(line)){
    if (line.len()==0 || line[0]=='#') continue;
    
    i=line.find(" ");
    if (i==-1) continue;
    name=line.substr(0,i);
    line.del(0,i);
    line.trim();
    if (!arr.exists(name))
      arr.add(name,line);
    else
      arr[name]+=line;
  }

  eintarray duplicateflag;
  int duplicates=0;
  for (i=0; i<arr.size(); ++i)
    duplicateflag.add(0);

  for (i=0; i<arr.size(); ++i){
    if (duplicateflag[i]==1) continue;
    for (j=i+1; j<arr.size(); ++j){
      if (duplicateflag[j]==1) continue;
      if (arr.values(i)==arr.values(j))
        { ++duplicates; duplicateflag[j]=1; }
    }
  }

  cout << "# duplicates: " << duplicates << endl;



  eintarray cons;
  for (i=0; i<arr[0].len(); ++i)
    cons.add(0);

  consensus(arr,cons,arr[0].len());

  eintarray sorti(sort(cons));

  for (i=0; i<cons.size(); ++i){
    if (cons[sorti[i]]==arr.size())
      cout << "*";
    else
      cout << (int)(cons[sorti[i]]*10/arr.size());
  }
  cout << endl;

  // we sort by most variable to most conserved
  for (i=0; i<arr.size(); ++i)
    sortstr(arr.values(i),sorti);

  cout << "# seqs: " << arr.size() << endl;
  cout << "# seq length: " << arr[0].len() << endl;

  estrhashof<int> tmphash;
  estr tmpstr="12";
  int tmpmax;
  for (i=0; i<arr[0].len(); ++i){
    for (i2=i+1; i2<arr[0].len(); ++i2){
      tmphash.clear();
      for (j=0; j<arr.size(); ++j){
        tmpstr[0]=arr.values(j)[i]; tmpstr[1]=arr.values(j)[i2];
        if (!tmphash.exists(tmpstr))
          tmphash.add(tmpstr,0);
        else
          ++tmphash[tmpstr];
      }
      tmpmax=0;
      for (j=0; j<tmphash.size(); ++j)
        if (tmphash.values(j)>tmpmax) tmpmax=tmphash.values(j);

      cout << "i: "<< i << " i2: "<<i2<<" tmpmax: "<<tmpmax << " tmphash: "<<tmphash.size() << endl;
    }
  }
  
  eintarray tmpint;
  for (i=0; i<arr.size(); ++i)
    tmpint.add(0);

  for (i=0; i<arr.size(); ++i)
    mindist(arr,i,arr[0].len(),tmpint);


  return(0);
}

