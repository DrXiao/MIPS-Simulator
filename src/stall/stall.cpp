#include <cstring>
#include <iostream>
#include "stall.h"
#include "util.h"
#include "pipelineReg.h"
using namespace std;

bool hazard = false;
bool taken = false;
bool branch_stall = false;
int EOF_count = 0;
bool branch_taken = false;
void Bubble() {
    if (hazard) {
        // EXE被沖刷
        stage_ins[2] = "";
    }
}

void Check_hazard() {
    hazard = 0;
    /* EX data hazard */

    if (((ID_EX_Reg.Ctl_WB.Reg_Write == 1) && (ID_EX_Reg.RegRd != 0) &&
         (ID_EX_Reg.RegRd == IF_ID_Reg.RegRs)) ||
        ((ID_EX_Reg.Ctl_WB.Reg_Write == 1) && (ID_EX_Reg.RegRd != 0) &&
         (ID_EX_Reg.RegRd == IF_ID_Reg.RegRt))) {
        hazard = 1;
        if (EOF_count < 2) Instruction_Backtrack(-1);
    }

    /* MEM data hazard */
    else if (((EX_MEM_Reg.Ctl_WB.Reg_Write == 1) && (EX_MEM_Reg.RegRd != 0) &&
              (EX_MEM_Reg.RegRd == IF_ID_Reg.RegRs)) ||
             ((EX_MEM_Reg.Ctl_WB.Reg_Write == 1) && (EX_MEM_Reg.RegRd != 0) &&
              (EX_MEM_Reg.RegRd == IF_ID_Reg.RegRt))) {
        hazard = 1;
        if (EOF_count < 2) Instruction_Backtrack(-1);
    }

    /* Load use data hazard*/
    else if ((ID_EX_Reg.Ctl_M.Mem_Read == 1) &&
             ((ID_EX_Reg.RegRt == IF_ID_Reg.RegRs) ||
              (ID_EX_Reg.RegRt == IF_ID_Reg.RegRt))) {
        hazard = 1;
        if (EOF_count < 2) Instruction_Backtrack(-1);
    }
}