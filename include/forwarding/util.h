#ifndef __INIT_H__
#define __INIT_H__
#include <string>
using namespace std;

extern int mipsRegisters[32];
extern int memory[32];

void Init_Reg_Mem(void);

void Print_Reg_Mem(FILE *);

void Instruction_Fetch(string &,string[4]);

void Instruction_Decode(void);

void Execute(void);

void Memory_Read_Write(void);

void Write_Back(void);

#endif