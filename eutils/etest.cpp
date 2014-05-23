#include "etest.h"

etest *globalTest=0x00;

etest& getTest()
{
  if (globalTest==0x00) globalTest=new etest;
  return(*globalTest);
}

etestItem::etestItem(const efunc& _func,const estr& _name,const estr& _file): func(_func),name(_name),file(_file)
{
  etest& test(getTest());
  test.tests.add(*this);
}

void etest::run()
{
  int i;
  for (i=0; i<tests.size(); ++i){
    cout << "# testing: " << tests[i].name << " in " << tests[i].file << endl;
    tests[i].func();
  }
}


