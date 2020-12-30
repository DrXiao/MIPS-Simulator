#ifndef __PIPELINE_UNIT_H__
#define __PIPELINE_UNIT_H__
#include <cstdint>

typedef struct Pipeline_ID_Stage {
    // ID inputs
    uint8_t ReadReg1;
    uint8_t ReadReg2;

    // ID outputs
    int32_t ReadData1;
    int32_t ReadData2;
} Pipeline_ID_Stage;

typedef struct Pipeline_EX_Stage {
    // EX inputs
    int32_t Operand_1;
    int32_t Operand_2;

    // EX outputs
    bool Zero;
    int32_t ALU_Result;
} Pipeline_EX_Stage;

typedef struct Pipeline_MEM_Stage {
    // MEM inputs
    int32_t Address;
    int32_t WriteData;

    // MEM outputs
    int32_t ReadData;
} Pipeline_MEM_Stage;

extern Pipeline_ID_Stage ID_Stage;
extern Pipeline_EX_Stage EX_Stage;
extern Pipeline_MEM_Stage Mem_Stage;

#endif