#include <iostream>
#include <fstream>
#include <cstring>
#include "forwarding.h"
#include "pipelineReg.h"
#include "pipelineUnit.h"
#include "util.h"

bool Load_Use_Hazard = 0;
bool Load_Use_Hazard_Forwarding_Rs = 0;
bool Load_Use_Hazard_Forwarding_Rt = 0;
bool Branch_Hazard = 0;

#define EX_HAZARD(reg)                                                         \
    EX_MEM_Reg.Ctl_WB.Reg_Write == 1 && EX_MEM_Reg.RegRd != 0 &&               \
        EX_MEM_Reg.RegRd == ID_EX_Reg.Reg##reg

#define MEM_HAZARD(reg)                                                        \
    MEM_WB_Reg.Ctl_WB.Reg_Write == 1 && MEM_WB_Reg.RegRd != 0 &&               \
        MEM_WB_Reg.RegRd == ID_EX_Reg.Reg##reg

#define LOAD_USE_HAZARD(reg)                                                   \
    ID_EX_Reg.Ctl_M.Mem_Read == 1 && ID_EX_Reg.RegRt == IF_ID_Reg.Reg##reg

using namespace std;

void CheckHazard(void) {
    // EX hazard
    if (EX_HAZARD(Rs)) { ID_EX_Reg.ReadData1 = EX_MEM_Reg.ALU_Result; }
    // Mem hazard
    else if (MEM_HAZARD(Rs)) {
        ID_EX_Reg.ReadData1 = MEM_WB_Reg.ALU_Result;
    }

    // EX hazard
    if (EX_HAZARD(Rt)) { ID_EX_Reg.ReadData2 = EX_MEM_Reg.ALU_Result; }
    // Mem hazard
    else if (MEM_HAZARD(Rt)) {
        ID_EX_Reg.ReadData2 = MEM_WB_Reg.ALU_Result;
    }

    // Load-Use hazard
    if (LOAD_USE_HAZARD(Rs)) {
        Load_Use_Hazard = 1;
        Load_Use_Hazard_Forwarding_Rs = 1;
    }
    if (LOAD_USE_HAZARD(Rt)) {
        Load_Use_Hazard = 1;
        Load_Use_Hazard_Forwarding_Rt = 1;
    }

    // Branch hazard
}

void Load_Use_Forwarding(void) {
    if (Load_Use_Hazard) {
        if (Load_Use_Hazard_Forwarding_Rs)
            ID_EX_Reg.ReadData1 = WB_Stage.WriteBackData;
        if (Load_Use_Hazard_Forwarding_Rt)
            ID_EX_Reg.ReadData2 = WB_Stage.WriteBackData;

        Load_Use_Hazard_Forwarding_Rs = Load_Use_Hazard_Forwarding_Rt = 0;
        memset(&ID_EX_Reg, 0, sizeof(ID_EX_Reg));
        Load_Use_Hazard = 0;
    }
}