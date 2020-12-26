#ifndef __INIT_H__
#define __INIT_H__
#include <string>
#define ADD "add"
#define SUB "sub"
#define LW "lw"
#define SW "sw"
#define BEQ "beq"
using namespace std;

extern FILE *outputFilePtr;

extern int mipsRegisters[32];
extern int memory[32];

extern bool stages_bubble[5];
extern string stage_ins[5];

void Init_Reg_Mem(void);

void Print_Reg_Mem(FILE *);

void Instruction_Fetch(string[4]);

void Instruction_Decode(void);

void Execute(void);

void Memory_Read_Write(void);

void Write_Back(void);

void Parse_Instruction(string &, string[4]);

void Move_Stages_Instruction(string &);

bool CheckEnding(void);

#endif