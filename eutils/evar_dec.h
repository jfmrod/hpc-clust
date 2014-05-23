#ifndef EVAR_DEC_H
#define EVAR_DEC_H

#include <typeinfo>
#include <iostream>
using namespace std;


class estr;
class evarBase;

template <class T>
class earray;
class evar;
typedef earray<evar> evararray;


class evar
{
 public:
  evarBase* var;

  evar();
  evar(evarBase* value);
  evar(evar* value);
  evar(const evar* value);
  evar(const evar& value);
  evar(evar& value);

  evar(const char* value);

  evar(double& value);
  evar(float& value);
  evar(unsigned char& value);
  evar(unsigned int& value);
  evar(unsigned long int& value);
  evar(char& value);
  evar(int& value);
  evar(long int& value);

  evar(const double& value);
  evar(const float& value);
  evar(const unsigned char& value);
  evar(const unsigned int& value);
  evar(const unsigned long int& value);
  evar(const char& value);
  evar(const int& value);
  evar(const long int& value);

  template <class T>
  evar(T* value);
  template <class T>
  evar(const T* value);
  template <class T>
  evar(const T& value);
  template <class T>
  evar(T& value);

  evar(void (*)());
  template <class T1>
  evar(void (*)(T1));
  template <class T1,class T2>
  evar(void (*)(T1,T2));
  template <class T1,class T2,class T3>
  evar(void (*)(T1,T2,T3));
  template <class T1,class T2,class T3,class T4>
  evar(void (*)(T1,T2,T3,T4));
  template <class T1,class T2,class T3,class T4,class T5>
  evar(void (*)(T1,T2,T3,T4,T5));
  template <class T1,class T2,class T3,class T4,class T5,class T6>
  evar(void (*)(T1,T2,T3,T4,T5,T6));

  template <class R>
  evar(R (*)());
  template <class R,class T1>
  evar(R (*)(T1));
  template <class R,class T1,class T2>
  evar(R (*)(T1,T2));
  template <class R,class T1,class T2,class T3>
  evar(R (*)(T1,T2,T3));
  template <class R,class T1,class T2,class T3,class T4>
  evar(R (*)(T1,T2,T3,T4));
  template <class R,class T1,class T2,class T3,class T4,class T5>
  evar(R (*)(T1,T2,T3,T4,T5));
  template <class R,class T1,class T2,class T3,class T4,class T5,class T6>
  evar(R (*)(T1,T2,T3,T4,T5,T6));

  ~evar();
  void clear();

  const char *getClass() const;
  const type_info& getTypeid() const;

  bool isNull() const;

  void create(const estr& type);
  void serial(estr& data) const;
  size_t unserial(const estr& data,size_t i=0);


  evar& get() const;
  evar& getarg(void (*pfunc)(evar)) const;
  evar& getarg(void (*pfunc)(const evar&)) const;
  evar& getarg(void (*pfunc)(evar&)) const;

  template <class T>
  T& get() const;
  template <class T>
  T& getarg(void (*pfunc)(T)) const;
  template <class T>
  T& getarg(void (*pfunc)(const T&)) const;
  template <class T>
  T& getarg(void (*pfunc)(T&)) const;

//  template <class T>
//  evar convert() const;
  evar convert(const type_info& ti) const;

//  template <class T>
//  bool isConvertible() const;

  bool isConvertible(const type_info& ti) const;

/*
  template <class T>
  evar& operator=(T* value);
  template <class T>
  evar& operator=(const T& value);
*/

  void retain();
  void release();

  evar& set(const evar& value);
  evar& copy(const evar& value);
  evar& operator=(evarBase* value);
  evar& operator=(const evar& value);
  evar& operator=(const char* value);

  earray<estr> properties() const;
  earray<estr> methods() const;

  bool hasProperty(const estr& pname);
  bool hasMethod(const estr& mname);

  evar  property(const estr& pname) const;
  evar  call(const estr& mname,const evararray& args) const;
  void  call(evar& rvar,const estr& mname,const evararray& args) const;


  evar operator()(const evararray& args);

  evar& operator-=(const evar& value);
  evar& operator+=(const evar& value);

  evar  operator+(const evar& value);
  evar  operator-(const evar& value);
  evar  operator*(const evar& value);
  evar  operator/(const evar& value);
  evar  operator%(const evar& value);

  evar&  operator++();
  evar&  operator--();

  evar  operator[](const evar& value);

  bool  operator==(const evar& value) const;
  inline bool operator!=(const evar& value) const { return(!(*this==value)); }

  bool  operator<(const evar& value) const;
  bool  operator<=(const evar& value) const;
  bool  operator>(const evar& value) const;
  bool  operator>=(const evar& value) const;


/*
  template <class T>
  void setref(T& value);

  inline void set(const evar& value);

  inline evar call(const estr& method,evararray& args);
  inline evar get(const estr& property) const;
  inline void set(const estr& property,const evar& value);

  evar call(const estr& method,const estr& args="");
*/
};

void evarSplitArgs(evararray &arr,const estr& args);
ostream& operator<<(ostream& stream,const evar& variable);

size_t unserialint(unsigned int& v,const estr& data,size_t i);
void serialint(unsigned int v,estr &data);

void serial(const evar& var,estr& data);
evar unserial(const estr& data);

template <class T>
void serial(const T& var,estr& data);

template <class T>
size_t unserial(T& var,const estr& data,size_t i);


#endif

