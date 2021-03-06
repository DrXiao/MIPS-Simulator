#include <string>
#include <sstream>
#include <cstdio>
#include <cstring>
#include "pipelineReg.h"
#include "pipelineUnit.h"
#include "stall.h"
#include "util.h"
#include <iostream>
using namespace std;

IF_ID_Pipeline_Reg IF_ID_Reg = {
    .RegRs = 0, .RegRt = 0, .RegRd = 0, .Immediate = 0};

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
    .Zero = 0,
    .ALU_Result = 0,
    .ReadData = 0,
    .RegRd = 0};

MEM_WB_Pipeline_Reg MEM_WB_Reg = {.Ctl_WB = {.Reg_Write = 0, .MemToReg = 0},
                                  .DataOfMem = 0,
                                  .ALU_Result = 0,
                                  .RegRd = 0};

Pipeline_ID_Stage ID_Stage = {
    .ReadReg1 = 0, .ReadReg2 = 0, .ReadData1 = 0, .ReadData2 = 0};

Pipeline_EX_Stage EX_Stage = {
    .Operand_1 = 0, .Operand_2 = 0, .Zero = 0, .ALU_Result = 0};

Pipeline_MEM_Stage Mem_Stage = {.Address = 0, .WriteData = 0, .ReadData = 0};

Pipeline_WB_Stage WB_Stage = {
    .DataOfMem = 0, .ALU_Result = 0, .WriteBackData = 0};

string stage_ins[5];

void Write_Back(void) {
    if (stage_ins[4] == "") return;
    fprintf(outputFilePtr, "\t%s : WB", stage_ins[4].c_str());
    if (stage_ins[4] == SW || stage_ins[4] == BEQ) {
        fprintf(outputFilePtr, " %dX\n", MEM_WB_Reg.Ctl_WB.Reg_Write);
    }
    else
        fprintf(outputFilePtr, " %d%d\n", MEM_WB_Reg.Ctl_WB.Reg_Write,
                MEM_WB_Reg.Ctl_WB.MemToReg);

    WB_Stage.DataOfMem = MEM_WB_Reg.DataOfMem;
    WB_Stage.ALU_Result = MEM_WB_Reg.ALU_Result;
    WB_Stage.WriteBackData = MEM_WB_Reg.Ctl_WB.MemToReg ? MEM_WB_Reg.DataOfMem
                                                        : MEM_WB_Reg.ALU_Result;

    if (MEM_WB_Reg.Ctl_WB.Reg_Write && MEM_WB_Reg.RegRd != 0) {
        mipsRegisters[MEM_WB_Reg.RegRd] = WB_Stage.WriteBackData;
    }
}

void Memory_Read_Write(void) {
    static uint8_t *memoryPtr = (uint8_t *)memory;

    if (stage_ins[3] == "") {
        memset(&MEM_WB_Reg, 0, sizeof(MEM_WB_Reg));
        return;
    }
    fprintf(outputFilePtr, "\t%s : MEM", stage_ins[3].c_str());
    if (stage_ins[3] == SW || stage_ins[3] == BEQ) {
        fprintf(outputFilePtr, " %d%d%d %dX\n", EX_MEM_Reg.Ctl_M.Branch,
                EX_MEM_Reg.Ctl_M.Mem_Read, EX_MEM_Reg.Ctl_M.Mem_Write,
                EX_MEM_Reg.Ctl_WB.Reg_Write);
    }
    else {
        fprintf(outputFilePtr, " %d%d%d %d%d\n", EX_MEM_Reg.Ctl_M.Branch,
                EX_MEM_Reg.Ctl_M.Mem_Read, EX_MEM_Reg.Ctl_M.Mem_Write,
                EX_MEM_Reg.Ctl_WB.Reg_Write, EX_MEM_Reg.Ctl_WB.MemToReg);
    }

    MEM_WB_Reg.Ctl_WB = EX_MEM_Reg.Ctl_WB;
    Mem_Stage.Address = EX_MEM_Reg.ALU_Result;
    Mem_Stage.WriteData = EX_MEM_Reg.ReadData;
    if (EX_MEM_Reg.Ctl_M.Mem_Read) {
        MEM_WB_Reg.DataOfMem = *(int32_t *)(memoryPtr + Mem_Stage.Address);
    }
    else if (EX_MEM_Reg.Ctl_M.Mem_Write) {
        // 檢查一下
        *(int32_t *)(memoryPtr + Mem_Stage.Address) = Mem_Stage.WriteData;
    }
    MEM_WB_Reg.ALU_Result = EX_MEM_Reg.ALU_Result;
    MEM_WB_Reg.RegRd = EX_MEM_Reg.RegRd;
}

void Execute(void) {

    if (stage_ins[2] == "") {
        memset(&EX_MEM_Reg, 0, sizeof(EX_MEM_Reg));
        return;
    }
    fprintf(outputFilePtr, "\t%s : EX", stage_ins[2].c_str());

    if (stage_ins[2] == SW || stage_ins[2] == BEQ) {
        fprintf(outputFilePtr, " X%d %d%d%d %dX\n", ID_EX_Reg.Ctl_Ex.ALUSrc,
                ID_EX_Reg.Ctl_M.Branch, ID_EX_Reg.Ctl_M.Mem_Read,
                ID_EX_Reg.Ctl_M.Mem_Write, ID_EX_Reg.Ctl_WB.Reg_Write);
    }
    else {
        fprintf(outputFilePtr, " %d%d %d%d%d %d%d\n", ID_EX_Reg.Ctl_Ex.RegDst,
                ID_EX_Reg.Ctl_Ex.ALUSrc, ID_EX_Reg.Ctl_M.Branch,
                ID_EX_Reg.Ctl_M.Mem_Read, ID_EX_Reg.Ctl_M.Mem_Write,
                ID_EX_Reg.Ctl_WB.Reg_Write, ID_EX_Reg.Ctl_WB.MemToReg);
    }
    /* data hazard */
    EX_Stage.Operand_1 = ID_EX_Reg.ReadData1;
    if (ID_EX_Reg.Ctl_Ex.ALUSrc) { EX_Stage.Operand_2 = ID_EX_Reg.Immediate; }
    else {
        EX_Stage.Operand_2 = ID_EX_Reg.ReadData2;
    }

    if (stage_ins[2] == SUB || stage_ins[2] == BEQ) {
        EX_Stage.ALU_Result = EX_Stage.Operand_1 - EX_Stage.Operand_2;
    }
    else {
        EX_Stage.ALU_Result = EX_Stage.Operand_1 + EX_Stage.Operand_2;
    }
    if (EX_Stage.ALU_Result == 0) EX_Stage.Zero = 1;

    EX_MEM_Reg.Ctl_WB = ID_EX_Reg.Ctl_WB;
    EX_MEM_Reg.Ctl_M = ID_EX_Reg.Ctl_M;
    EX_MEM_Reg.ALU_Result = EX_Stage.ALU_Result;
    EX_MEM_Reg.ReadData = ID_EX_Reg.ReadData2;
    EX_MEM_Reg.Zero = EX_Stage.Zero;
    if (ID_EX_Reg.Ctl_Ex.RegDst)
        EX_MEM_Reg.RegRd = ID_EX_Reg.RegRd;
    else
        EX_MEM_Reg.RegRd = ID_EX_Reg.RegRt;
}

void Instruction_Decode(void) {

    if (stage_ins[1] == "") {
        memset(&ID_EX_Reg, 0, sizeof(ID_EX_Reg));
        return;
    }
    fprintf(outputFilePtr, "\t%s : ID\n", stage_ins[1].c_str());

    // Skip if stalled
    if (hazard) return;

    ID_Stage.ReadReg1 = IF_ID_Reg.RegRs;
    ID_Stage.ReadReg2 = IF_ID_Reg.RegRt;

    ID_Stage.ReadData1 = mipsRegisters[ID_Stage.ReadReg1];
    ID_Stage.ReadData2 = mipsRegisters[ID_Stage.ReadReg2];

    if (stage_ins[1] == ADD || stage_ins[1] == SUB) {
        ID_EX_Reg.Ctl_WB = {.Reg_Write = 1, .MemToReg = 0};
        ID_EX_Reg.Ctl_M = {.Branch = 0, .Mem_Read = 0, .Mem_Write = 0};
        ID_EX_Reg.Ctl_Ex = {.RegDst = 1, .ALUOp = 0x02, .ALUSrc = 0};
        ID_EX_Reg.ReadData1 = ID_Stage.ReadData1;
        ID_EX_Reg.ReadData2 = ID_Stage.ReadData2;
        ID_EX_Reg.Immediate = (int32_t)IF_ID_Reg.Immediate;
        ID_EX_Reg.RegRs = IF_ID_Reg.RegRs;
        ID_EX_Reg.RegRt = IF_ID_Reg.RegRt;
        ID_EX_Reg.RegRd = IF_ID_Reg.RegRd;
    }
    else if (stage_ins[1] == LW) {
        ID_EX_Reg.Ctl_WB = {.Reg_Write = 1, .MemToReg = 1};
        ID_EX_Reg.Ctl_M = {.Branch = 0, .Mem_Read = 1, .Mem_Write = 0};
        ID_EX_Reg.Ctl_Ex = {.RegDst = 0, .ALUOp = 0x00, .ALUSrc = 1};
        ID_EX_Reg.RegRs = IF_ID_Reg.RegRs;
        ID_EX_Reg.RegRt = IF_ID_Reg.RegRt;
        ID_EX_Reg.RegRd = IF_ID_Reg.RegRd; // Needs to check
        ID_EX_Reg.Immediate = (int32_t)IF_ID_Reg.Immediate;
        ID_EX_Reg.ReadData1 = ID_Stage.ReadData1;
        ID_EX_Reg.ReadData2 = ID_Stage.ReadData2;
    }
    else if (stage_ins[1] == SW) {
        ID_EX_Reg.Ctl_WB = {.Reg_Write = 0, .MemToReg = 0};
        ID_EX_Reg.Ctl_M = {.Branch = 0, .Mem_Read = 0, .Mem_Write = 1};
        ID_EX_Reg.Ctl_Ex = {.RegDst = 0, .ALUOp = 0x00, .ALUSrc = 1};
        ID_EX_Reg.ReadData1 = ID_Stage.ReadData1;
        ID_EX_Reg.ReadData2 = ID_Stage.ReadData2;
        ID_EX_Reg.Immediate = (int32_t)IF_ID_Reg.Immediate;
        ID_EX_Reg.RegRs = IF_ID_Reg.RegRs;
        ID_EX_Reg.RegRt = IF_ID_Reg.RegRt;
        ID_EX_Reg.RegRd = IF_ID_Reg.RegRd; // Needs to check
    }
    else {
        ID_EX_Reg.Ctl_WB = {.Reg_Write = 0, .MemToReg = 0};
        ID_EX_Reg.Ctl_M = {.Branch = 1, .Mem_Read = 0, .Mem_Write = 0};
        ID_EX_Reg.Ctl_Ex = {.RegDst = 0, .ALUOp = 0x01, .ALUSrc = 0};
        ID_EX_Reg.ReadData1 = ID_Stage.ReadData1;
        ID_EX_Reg.ReadData2 = ID_Stage.ReadData2;
        ID_EX_Reg.Immediate = (int32_t)IF_ID_Reg.Immediate;
        ID_EX_Reg.RegRs = IF_ID_Reg.RegRs;
        ID_EX_Reg.RegRt = IF_ID_Reg.RegRt;
        ID_EX_Reg.RegRd = IF_ID_Reg.RegRd; // Needs to check
    }
}

void Instruction_Fetch(string *insToken) {

    if (stage_ins[0] == "") return;

    fprintf(outputFilePtr, "\t%s : IF\n", stage_ins[0].c_str());

    // If stalled, do not flush the pipeline register and not fetch new
    // instruction
    if (!hazard) { memset(&IF_ID_Reg, 0, sizeof(IF_ID_Reg)); }
    else {
        return;
    }

    if (insToken[0] == LW || insToken[0] == SW) {
        sscanf(insToken[1].c_str(), "$%hu", &IF_ID_Reg.RegRt);
        sscanf(insToken[2].c_str(), "%hd($%hu)", &IF_ID_Reg.Immediate,
               &IF_ID_Reg.RegRs);
    }
    else if (insToken[0] == BEQ) {
        sscanf(insToken[1].c_str(), "$%hu", &IF_ID_Reg.RegRs);
        sscanf(insToken[2].c_str(), "$%hu", &IF_ID_Reg.RegRt);
        IF_ID_Reg.Immediate = stol(insToken[3]);
    }
    else {
        sscanf(insToken[1].c_str(), "$%hu", &IF_ID_Reg.RegRd);
        sscanf(insToken[2].c_str(), "$%hu", &IF_ID_Reg.RegRs);
        sscanf(insToken[3].c_str(), "$%hu", &IF_ID_Reg.RegRt);
    }
}
