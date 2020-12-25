#include "forwarding/util.h"
#include <cstdio>
#include <iomanip>
#include <sstream>
using namespace std;
#define OUTPUT_FIELD 3

int mipsRegisters[32] = {0};
int memory[32] = {0};

void Init_Reg_Mem(void) {
    for (int idxOfReg_Mem = 0; idxOfReg_Mem < 32; idxOfReg_Mem++) {
        mipsRegisters[idxOfReg_Mem] = memory[idxOfReg_Mem] = 1;
    }
    mipsRegisters[0] = 0;
}

void Parse_MPIS_Ins(string &instruction, string insToken[4]) {
    for(string::iterator it = instruction.begin(); it != instruction.end() ; it++) {
        if(*it == ',')
            *it = ' ';
    }
    stringstream strStream(instruction);
    strStream >> insToken[0] >> insToken[1] >> insToken[2] >> insToken[3];
}

void Print_Reg_Mem(FILE *outputFilePtr) {
    for(int idxOfReg = 0; idxOfReg < 32 ; idxOfReg++) {
        fprintf(outputFilePtr ," $%-*d", OUTPUT_FIELD, idxOfReg);
    }
    fprintf(outputFilePtr ,"\n");
    for(int idxOfReg = 0; idxOfReg < 32 ; idxOfReg++) {
        fprintf(outputFilePtr ,"  %-*d", OUTPUT_FIELD, mipsRegisters[idxOfReg]);
    }
    fprintf(outputFilePtr ,"\n\n");
    for(int idxOfMem = 0; idxOfMem < 32 ; idxOfMem++) {
        fprintf(outputFilePtr ," W%-*d", OUTPUT_FIELD, idxOfMem);
    }
    fprintf(outputFilePtr ,"\n");
    for(int idxOfMem = 0; idxOfMem < 32 ; idxOfMem++) {
        fprintf(outputFilePtr ,"  %-*d", OUTPUT_FIELD, memory[idxOfMem]);
    }
    fprintf(outputFilePtr ,"\n");
}