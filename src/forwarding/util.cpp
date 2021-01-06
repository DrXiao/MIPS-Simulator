#include "util.h"
#include "forwarding.h"
#include "pipelineReg.h"
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <string>
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
        cout << mipsIns.tellg() << endl;
        insLine += lines;
    }
    else {
        while (lines--) {
            string prevIns;
            getline(mipsIns, prevIns);
            insLine++;
        }
    }

    /*
    int offset = lines > 0 ? 1 : -1;
    lines = lines < 0 ? -lines + 1 : lines;
    while (lines) {
        mipsIns.seekg(offset, mipsIns.cur);
        if (mipsIns.peek() == '\n');
            lines--;
    }
    mipsIns.seekg(1, mipsIns.cur);
    */
}
