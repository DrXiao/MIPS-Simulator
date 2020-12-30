#include <string>
#include <sstream>
#include <cstdio>
#include "pipelineReg.h"
#include "pipelineUnit.h"
#include "util.h"
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
    .Zero = 0,
    .ALU_Result = 0,
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

bool stages_bubble[5] = {0};
string stage_ins[5];


void Write_Back(void) {
    if (stage_ins[4] == "") return;

    fprintf(outputFilePtr, "\t%s : WB", stage_ins[4].c_str());
    fprintf(outputFilePtr, " %d %d\n", MEM_WB_Reg.Ctl_WB.Reg_Write,
            MEM_WB_Reg.Ctl_WB.MemToReg);

    if (MEM_WB_Reg.Ctl_WB.Reg_Write) {
        mipsRegisters[MEM_WB_Reg.RegRd] = MEM_WB_Reg.Ctl_WB.MemToReg
                                              ? MEM_WB_Reg.DataOfMem
                                              : MEM_WB_Reg.ALU_Result;
    }
}

void Memory_Read_Write(void) {
    if (stage_ins[3] == "") return;
    fprintf(outputFilePtr, "\t%s : MEM", stage_ins[3].c_str());
    fprintf(outputFilePtr, " \n");
    
    MEM_WB_Reg.Ctl_WB = EX_MEM_Reg.Ctl_WB;
    Mem_Stage.Address = EX_MEM_Reg.ALU_Result;
    Mem_Stage.WriteData = EX_MEM_Reg.ReadData;
    if (EX_MEM_Reg.Ctl_M.Mem_Read) {
        MEM_WB_Reg.DataOfMem = memory[EX_MEM_Reg.ALU_Result >> 2];
    }
    else if (EX_MEM_Reg.Ctl_M.Mem_Write) {
        // 檢查一下
        memory[Mem_Stage.Address >> 2] = Mem_Stage.WriteData;
    }
    MEM_WB_Reg.ALU_Result = EX_MEM_Reg.ALU_Result;
    MEM_WB_Reg.RegRd = EX_MEM_Reg.RegRd;
}

void Execute(void) {
    if (stage_ins[2] == "") return;
    fprintf(outputFilePtr, "\t%s : EX", stage_ins[2].c_str());
    fprintf(outputFilePtr, " \n");
    
    EX_Stage.Operand_1 = ID_EX_Reg.ReadData1;
    if (stage_ins[2] == ADD || stage_ins[2] == SUB) {
        EX_Stage.Operand_2 = ID_EX_Reg.ReadData2;
    }
    else {
        EX_Stage.Operand_2 = ID_EX_Reg.Immediate;
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
    EX_MEM_Reg.ReadData = EX_Stage.Operand_2;
    EX_MEM_Reg.Zero = EX_Stage.Zero;
    EX_MEM_Reg.RegRd = ID_EX_Reg.RegRd;
}

void Instruction_Decode(void) {
    if (stage_ins[1] == "") return;
    fprintf(outputFilePtr, "\t%s : ID\n", stage_ins[1].c_str());
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
        ID_EX_Reg.Immediate = IF_ID_Reg.Immediate;
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
        ID_EX_Reg.RegRd = IF_ID_Reg.RegRt; // Needs to check
    }
    else if (stage_ins[1] == SW) {
        ID_EX_Reg.Ctl_WB = {.Reg_Write = 0, .MemToReg = 0};
        ID_EX_Reg.Ctl_M = {.Branch = 0, .Mem_Read = 0, .Mem_Write = 1};
        ID_EX_Reg.Ctl_Ex = {.RegDst = 0, .ALUOp = 0x00, .ALUSrc = 1};
        ID_EX_Reg.ReadData1 = ID_Stage.ReadData1;
        ID_EX_Reg.ReadData2 = ID_Stage.ReadData2;
        ID_EX_Reg.Immediate = IF_ID_Reg.Immediate;
        ID_EX_Reg.RegRs = IF_ID_Reg.RegRs;
        ID_EX_Reg.RegRt = IF_ID_Reg.RegRt;
        ID_EX_Reg.RegRd = IF_ID_Reg.RegRs; // Needs to check
    }
    else {
        ID_EX_Reg.Ctl_WB = {.Reg_Write = 0, .MemToReg = 0};
        ID_EX_Reg.Ctl_M = {.Branch = 1, .Mem_Read = 0, .Mem_Write = 0};
        ID_EX_Reg.Ctl_Ex = {.RegDst = 0,.ALUOp = 0x01, .ALUSrc = 0};
        ID_EX_Reg.ReadData1 = ID_Stage.ReadData1;
        ID_EX_Reg.ReadData2 = ID_Stage.ReadData2;
        ID_EX_Reg.Immediate = IF_ID_Reg.Immediate;
        ID_EX_Reg.RegRs = IF_ID_Reg.RegRs;
        ID_EX_Reg.RegRt = IF_ID_Reg.RegRt;
        ID_EX_Reg.RegRd = IF_ID_Reg.RegRs; // Needs to check
    }
}

void Instruction_Fetch(string insToken[4]) {
    if (insToken[0] == "") return;
    fprintf(outputFilePtr, "\t%s : IF\n", stage_ins[0].c_str());
    IF_ID_Reg.OpCode = insToken[0];
    if (insToken[0] == LW || insToken[0] == SW) {
        sscanf(insToken[1].c_str(), "$%hhu", &IF_ID_Reg.RegRs);
        sscanf(insToken[2].c_str(), "%hd($%hhu)", &IF_ID_Reg.Immediate,
               &IF_ID_Reg.RegRt);
    }
    else if (insToken[0] == BEQ) {
        sscanf(insToken[1].c_str(), "$%hhu", &IF_ID_Reg.RegRs);
        sscanf(insToken[2].c_str(), "$%hhu", &IF_ID_Reg.RegRt);
        IF_ID_Reg.Immediate = stol(insToken[3]);
    }
    else {
        sscanf(insToken[1].c_str(), "$%hhu", &IF_ID_Reg.RegRd);
        sscanf(insToken[2].c_str(), "$%hhu", &IF_ID_Reg.RegRs);
        sscanf(insToken[3].c_str(), "$%hhu", &IF_ID_Reg.RegRt);
    }
}
