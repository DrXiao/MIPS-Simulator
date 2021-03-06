#include "util.h"
#include "stall.h"
#include "pipelineReg.h"
#include <cstdio>
#include <iomanip>
#include <string>
#include <iostream>
#include <cstring>
using namespace std;
#define OUTPUT_FIELD 5

int mipsRegisters[32] = {0};
int memory[32] = {0};

fstream mipsIns;
fstream headIns;
uint32_t insLine = 0;
FILE *outputFilePtr = stdout;
int cycle = 0;
void Instruction_Backtrack(int lines) {
    if (lines == 0)
        return;
    else if (lines < 0) {
        mipsIns.setstate(headIns.rdstate());
        mipsIns.seekg(headIns.tellg());
        int count = insLine + lines;
        while (count--) {
            string prevIns;
            getline(mipsIns, prevIns);
        }
        insLine += lines;
    }
    else {
        while (lines--) {
            string prevIns;
            getline(mipsIns, prevIns);
            insLine++;
        }
    }
}

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

void Move_Stages_Instruction(string &Next_New_Instruction) {
    int move_lb = 0;
    if (hazard) move_lb = 2;
    // if (branch_stall) move_lb = 1;
    for (int stages_ins_idx = 4; stages_ins_idx > move_lb; stages_ins_idx--) {
        stage_ins[stages_ins_idx] = stage_ins[stages_ins_idx - 1];
    }

    // Insert bubble in to EX stage and flush ID/EX pipeline register
    if (hazard) {
        stage_ins[2] = "";

        memset(&ID_EX_Reg, 0, sizeof(ID_EX_Reg));
    }
    if (!hazard) stage_ins[0] = Next_New_Instruction;
    if (taken) {
        stage_ins[1] = "";
        taken = false;
    }
}

bool CheckEnding(void) {
    for (int stages_ins_idx = 0; stages_ins_idx < 4; stages_ins_idx++) {
        if (stage_ins[stages_ins_idx] != "") return false;
    }
    return true;
}