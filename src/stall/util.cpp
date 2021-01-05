#include "util.h"
#include <cstdio>
#include <iomanip>
#include <string>
using namespace std;
#define OUTPUT_FIELD 3

int mipsRegisters[32] = {0};
int memory[32] = {0};
bool hazard_EX_MEM = 0;
bool hazard_MEM_WB = 0;

FILE *outputFilePtr = stdout;
int cycle = 0;

void Init_Reg_Mem(void) {
    for (int idxOfReg_Mem = 0; idxOfReg_Mem < 32; idxOfReg_Mem++) {
        mipsRegisters[idxOfReg_Mem] = memory[idxOfReg_Mem] = 1;
    }
    mipsRegisters[0] = 0;
}

void Print_Reg_Mem(FILE *outputFilePtr) {
    for (int idxOfReg = 0; idxOfReg < 32; idxOfReg++) {
        fprintf(outputFilePtr, " $%-*d", OUTPUT_FIELD, idxOfReg);
    }
    fprintf(outputFilePtr, "\n");
    for (int idxOfReg = 0; idxOfReg < 32; idxOfReg++) {
        fprintf(outputFilePtr, "  %-*d", OUTPUT_FIELD, mipsRegisters[idxOfReg]);
    }
    fprintf(outputFilePtr, "\n\n");
    for (int idxOfMem = 0; idxOfMem < 32; idxOfMem++) {
        fprintf(outputFilePtr, " W%-*d", OUTPUT_FIELD, idxOfMem);
    }
    fprintf(outputFilePtr, "\n");
    for (int idxOfMem = 0; idxOfMem < 32; idxOfMem++) {
        fprintf(outputFilePtr, "  %-*d", OUTPUT_FIELD, memory[idxOfMem]);
    }
    fprintf(outputFilePtr, "\n");
}

void Parse_Instruction(string &instruction, string insToken[4]) {
    for (string::iterator it = instruction.begin(); it != instruction.end();
         it++) {
        if (*it == ',') *it = ' ';
    }
    stringstream strStream(instruction);
    strStream >> insToken[0] >> insToken[1] >> insToken[2] >> insToken[3];
}

// void Move_Stages_Instruction(string &Next_New_Instruction) {
//     for (int stages_ins_idx = 4; stages_ins_idx > 0; stages_ins_idx--) {
//         stage_ins[stages_ins_idx] = stage_ins[stages_ins_idx - 1];
//     }
//     stage_ins[0] = Next_New_Instruction;
// }

void Move_Stages_Instruction(string &Next_New_Instruction) {
    if(hazard_EX_MEM){
        stage_ins[4] = stage_ins[3];
        stage_ins[3] = stage_ins[2];
        stage_ins[2] = "";
        return;
    }
    if(hazard_MEM_WB)
    {
        stage_ins[4] = stage_ins[3];
        stage_ins[3] = "";
        return;
    }
    for (int stages_ins_idx = 4; stages_ins_idx > 0; stages_ins_idx--) {
        stage_ins[stages_ins_idx] = stage_ins[stages_ins_idx - 1];
    }
    stage_ins[0] = Next_New_Instruction;
}

bool CheckEnding(void) {
    for (int stages_ins_idx = 0; stages_ins_idx < 4; stages_ins_idx++) {
        if (stage_ins[stages_ins_idx] != "") return false;
    }
    return true;
}