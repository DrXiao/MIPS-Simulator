#include <string>
#include <sstream>
#include <cstdio>
#include "pipelineReg.h"
#include "forwarding/util.h"
using namespace std;

IF_ID_Pipeline_Reg IF_ID_Reg = {
    .OpCode = "", .RegRs = 0, .RegRt = 0, .RegRd = 0, .Immediate = 0};

ID_EX_Pipeline_Reg ID_EX_Reg = {
    .Ctl_WB = {.Reg_Write = 0, .MemToReg = 0},
    .Ctl_M = {.Branch = 0, .Mem_Read = 0, .Mem_Write = 0},
    .Ctl_Ex = {.RegDst = 0, .ALUOp = 0, .ALUSrc = 0},
    .ReadData1 = 0,
    .ReadData2 = 0,
    .Immediate = 0,
    .RegRs = 0,
    .RegRt = 0,
    .RegRd = 0};

EX_MEM_Pipeline_Reg EX_MEM_Reg = {
    .Ctl_WB = {.Reg_Write = 0, .MemToReg = 0},
    .Ctl_M = {.Branch = 0, .Mem_Read = 0, .Mem_Write = 0},
    .ALU_Result = 0,
    .RegRd = 0};

MEM_WB_Pipeline_Reg MEM_WB_Reg = {.Ctl_WB = {.Reg_Write = 0, .MemToReg = 0},
                                  .DataOfMem = 0,
                                  .ALU_Result = 0,
                                  .RegRd = 0};

bool stages[5] = {false};
string stage_ins[5];

void Write_Back(void) {
    if (MEM_WB_Reg.Ctl_WB.Reg_Write) {
        mipsRegisters[MEM_WB_Reg.RegRd] = MEM_WB_Reg.Ctl_WB.MemToReg
                                              ? MEM_WB_Reg.DataOfMem
                                              : MEM_WB_Reg.ALU_Result;
    }
}

void Memory_Read_Write(void) {
    MEM_WB_Reg.Ctl_WB = EX_MEM_Reg.Ctl_WB;
    if (EX_MEM_Reg.Ctl_M.Mem_Read) {
        MEM_WB_Reg.DataOfMem = memory[EX_MEM_Reg.ALU_Result];
    }
    else if (EX_MEM_Reg.Ctl_M.Mem_Write) {
        memory[EX_MEM_Reg.ALU_Result] = mipsRegisters[EX_MEM_Reg.RegRd];
    }
    MEM_WB_Reg.ALU_Result = EX_MEM_Reg.ALU_Result;
    MEM_WB_Reg.RegRd = EX_MEM_Reg.RegRd;
}

void Execute(void) {
    return;
}

void Instruction_Decode(void) {
    return;
}
void Instruction_Fetch(string &instruction, string insToken[4]) {
    for (string::iterator it = instruction.begin(); it != instruction.end();
         it++) {
        if (*it == ',') *it = ' ';
    }
    stringstream strStream(instruction);
    strStream >> insToken[0] >> insToken[1] >> insToken[2] >> insToken[3];

    IF_ID_Reg.OpCode = insToken[0];
    if (insToken[0] == "lw" || insToken[0] == "sw") {
        sscanf(insToken[1].c_str(), "$%hhu", &IF_ID_Reg.RegRs);
        sscanf(insToken[2].c_str(), "%hd($%hhu)", &IF_ID_Reg.Immediate,
               &IF_ID_Reg.RegRt);
    }
    else if (insToken[0] == "beq") {
        sscanf(insToken[1].c_str(), "$%hhu", &IF_ID_Reg.RegRs);
        sscanf(insToken[2].c_str(), "$%hhu", &IF_ID_Reg.RegRt);
        IF_ID_Reg.Immediate = stoul(insToken[3]);
    }
    else {
        sscanf(insToken[1].c_str(), "$%hhu", &IF_ID_Reg.RegRd);
        sscanf(insToken[2].c_str(), "$%hhu", &IF_ID_Reg.RegRs);
        sscanf(insToken[3].c_str(), "$%hhu", &IF_ID_Reg.RegRt);
    }
}
