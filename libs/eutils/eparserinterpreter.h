#ifndef EPARSERINTERPRETER_H
#define EPARSERINTERPRETER_H

#include "eutils.h"

class evar;
class estr;

#include "estrhashof.h"
//template <class T,unsigned int (*)(const estr&)>
//class estrhashof;

//void code_interpret(estrarrayof<evar*>& env,const estr& str);

evar code_interpret(estrhashof<evar>& env,const estr& str);
evar epinterpret(const estr& str);
void epruninterpret(int argvc,char **argv);

void epinterpretfile(const estr& file);


#endif
