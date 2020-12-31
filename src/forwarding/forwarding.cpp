#include <iostream>
#include <fstream>
#include "forwarding.h"
#include "pipelineReg.h"
#include "pipelineUnit.h"
#include "util.h"

bool Load_Use_Hazard = 0;
char Load_Use_Hazard_Forwarding_In = '\0';
bool Branch_Hazard = 0;

#define EX_HAZARD(reg)                                                         \
    EX_MEM_Reg.Ctl_WB.Reg_Write == 1 && EX_MEM_Reg.RegRd != 0 &&               \
        EX_MEM_Reg.RegRd != ID_EX_Reg.Reg##reg

#define MEM_HAZARD(reg)                                                        \
    MEM_WB_Reg.Ctl_WB.Reg_Write == 1 && MEM_WB_Reg.RegRd != 0 &&               \
        MEM_WB_Reg.RegRd != ID_EX_Reg.Reg##reg

#define LOAD_USE_HAZARD(reg)                                                   \
    ID_EX_Reg.Ctl_M.Mem_Read == 1 && ID_EX_Reg.RegRt == IF_ID_Reg.Reg##reg

using namespace std;

void CheckHazard(void) {
    // EX hazard
    if (EX_HAZARD(Rs)) { ID_EX_Reg.ReadData1 = EX_MEM_Reg.ALU_Result; }
    else if (EX_HAZARD(Rt)) {
        ID_EX_Reg.ReadData2 = EX_MEM_Reg.ALU_Result;
    }
    // Mem hazard
    else if (MEM_HAZARD(Rs)) {
        ID_EX_Reg.ReadData1 = MEM_WB_Reg.ALU_Result;
    }
    else if (MEM_HAZARD(Rt)) {
        ID_EX_Reg.ReadData2 = MEM_WB_Reg.ALU_Result;
    }
    // Load-Use hazard
    if (LOAD_USE_HAZARD(Rs)) {
        Load_Use_Hazard = 1;
        Load_Use_Hazard_Forwarding_In = 's';
        Instruction_Backtrack(-1);
    }
    else if (LOAD_USE_HAZARD(Rt)) {
        Load_Use_Hazard = 1;
        Load_Use_Hazard_Forwarding_In = 't';
        Instruction_Backtrack(-1);
    }
    else {
        Load_Use_Hazard = 0;
    }
    // Branch hazard
}

void Load_Use_Forwarding(void) {
    if (Load_Use_Hazard) {
        switch (Load_Use_Hazard_Forwarding_In) {
        case 's': ID_EX_Reg.ReadData1 = MEM_WB_Reg.ALU_Result;
        case 't': ID_EX_Reg.ReadData2 = MEM_WB_Reg.ALU_Result;
        }
        Load_Use_Hazard_Forwarding_In = '\0';
        Load_Use_Hazard = 0;
    }
}