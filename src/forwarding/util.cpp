#include "forwarding/util.h"
#include <sstream>

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