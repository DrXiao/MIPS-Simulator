#ifndef __PIPELINE_REG_H__
#define __PIPELINE_REG_H__
#include <cstdint>
#include <string>
#include <iostream>
using namespace std;

// Slide p.85
typedef struct control_EX {
    bool RegDst;
    uint16_t ALUOp;
    bool ALUSrc;
} control_EX;

typedef struct control_M {
    bool Branch;
    bool Mem_Read;
    bool Mem_Write;
} control_M;

typedef struct control_WB {
    bool Reg_Write;
    bool MemToReg;
} control_WB;

typedef struct IF_ID_Pipeline_Reg {
    uint16_t RegRs;
    uint16_t RegRt;
    uint16_t RegRd;
    int16_t Immediate;
} IF_ID_Pipeline_Reg;

typedef struct ID_EX_Pipeline_Reg {
    control_WB Ctl_WB;
    control_M Ctl_M;
    control_EX Ctl_Ex;
    int32_t ReadData1;
    int32_t ReadData2;
    int32_t Immediate;
    uint16_t RegRs;
    uint16_t RegRt;
    uint16_t RegRd;
} ID_EX_Pipeline_Reg;

typedef struct EX_MEM_Pipeline_Reg {
    control_WB Ctl_WB;
    control_M Ctl_M;
    bool Zero;
    int32_t ALU_Result;
    int32_t ReadData;
    uint16_t RegRd;
} EX_MEM_Pipeline_Reg;

typedef struct MEM_WB_Pipeline_Reg {
    control_WB Ctl_WB;
    int32_t DataOfMem;
    int32_t ALU_Result;
    uint16_t RegRd;
} MEM_WB_Pipeline_Reg;

extern IF_ID_Pipeline_Reg IF_ID_Reg;
extern ID_EX_Pipeline_Reg ID_EX_Reg;
extern EX_MEM_Pipeline_Reg EX_MEM_Reg;
extern MEM_WB_Pipeline_Reg MEM_WB_Reg;

#endif