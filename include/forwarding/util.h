#ifndef __INIT_H__
#define __INIT_H__
#include <string>
using namespace std;

extern int mipsRegisters[32];
extern int memory[32];

void Init_Reg_Mem(void);

void Parse_MPIS_Ins(string &, string[4]);

#endif