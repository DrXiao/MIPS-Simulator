#include "forwarding/pipelineReg.h"

IF_ID_Pipeline_Reg IF_ID_Reg = {
    .OpCode = 0, .RegRs = 0, .RegRt = 0, .RegRd = 0, .Immediate = 0};

ID_EX_Pipeline_Reg ID_EX_Reg = {
    .Ctl_WB = {.Reg_Write = 0, .MemToReg = 0},
    .Ctl_M = {.Branch = 0, .Mem_Read = 0, .Mem_Write = 0},
    .Ctl_Ex = {.RegDst = 0, .ALUOp = 0, .ALUSrc = false},
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
    .RegRd = 0
};

MEM_WB_Pipeline_Reg MEM_WB_Reg = {
    .Ctl_WB = {.Reg_Write = 0, .MemToReg = 0},
    .DataOfMem = 0,
    .ALU_Result = 0,
    .RegRd = 0
};