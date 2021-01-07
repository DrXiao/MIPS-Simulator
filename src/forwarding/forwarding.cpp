#include <iostream>
#include <fstream>
#include <cstring>
#include "forwarding.h"
#include "pipelineReg.h"
#include "pipelineUnit.h"
#include "util.h"

bool EX_Hazard_Rs = 0;
bool EX_Hazard_Rt = 0;
bool MEM_Hazard_Rs = 0;
bool MEM_Hazard_Rt = 0;

bool Load_Use_Hazard = 0;
bool Load_Use_Hazard_Forwarding_Rs = 0;
bool Load_Use_Hazard_Forwarding_Rt = 0;
int Branch_Stall = 0;
bool stall_flag = 0;

#define EX_HAZARD(reg)                                                         \
    EX_MEM_Reg.Ctl_WB.Reg_Write == 1 && EX_MEM_Reg.RegRd != 0 &&               \
        EX_MEM_Reg.RegRd == ID_EX_Reg.Reg##reg

#define MEM_HAZARD(reg)                                                        \
    MEM_WB_Reg.Ctl_WB.Reg_Write == 1 && MEM_WB_Reg.RegRd != 0 &&               \
        MEM_WB_Reg.RegRd == ID_EX_Reg.Reg##reg

#define LOAD_USE_HAZARD(reg)                                                   \
    ID_EX_Reg.Ctl_M.Mem_Read == 1 && ID_EX_Reg.RegRt == IF_ID_Reg.Reg##reg

using namespace std;

int Load_Use_count = 0;

void Stall_Flag_Re(void) {
    stall_flag = 0;
}

void Check_EX_And_MEM_Hazard(void) {
    if (stall_flag == true || Load_Use_Hazard || Load_Use_count) { return; }
    // EX hazard
    if (EX_HAZARD(Rs)) {

        EX_Hazard_Rs = 1;
        // ID_EX_Reg.ReadData1 = EX_MEM_Reg.ALU_Result;
    }
    // Mem hazard
    else if (MEM_HAZARD(Rs)) {

        MEM_Hazard_Rs = 1;
        // ID_EX_Reg.ReadData1 = WB_Stage.WriteBackData;
    }

    // EX hazard
    if (EX_HAZARD(Rt)) {

        EX_Hazard_Rt = 1;
        // ID_EX_Reg.ReadData2 = EX_MEM_Reg.ALU_Result;
    }
    // Mem hazard
    else if (MEM_HAZARD(Rt)) {

        MEM_Hazard_Rt = 1;
        // ID_EX_Reg.ReadData2 = WB_Stage.WriteBackData;
    }
    EX_MEM_Forwarding();
}

void Check_Load_Use_Hazard(void) {
    if (stall_flag == true) { return; }
    if (stage_ins[0] == BEQ) { return; }
    // Load-Use hazard
    bool Hazard = false;
    if (LOAD_USE_HAZARD(Rs)) {
        stall_flag = true;
        Load_Use_Hazard = 1;
        Load_Use_Hazard_Forwarding_Rs = 1;
        Load_Use_count = 3;
        Hazard = true;
    }
    if (LOAD_USE_HAZARD(Rt)) {
        stall_flag = true;
        Load_Use_Hazard = 1;
        Load_Use_Hazard_Forwarding_Rt = 1;
        Load_Use_count = 3;
        Hazard = true;
    }
    if (Hazard) { Instruction_Backtrack(-1); }
    if (Load_Use_count != 0) {
        Load_Use_count--;
        if (!Load_Use_count) { Load_Use_Forwarding(); }
    }
}

void Check_Branch_Stall(void) {
    // 前一個是 lw
    Branch_Stall = 0;
    if (stage_ins[1] != BEQ || stall_flag == true) { return; }
    if (stage_ins[2] == LW && IF_ID_Reg.RegRs == EX_MEM_Reg.RegRd) {
        stall_flag = true;
        Branch_Stall = 1;
    }
    else if (stage_ins[2] == LW && IF_ID_Reg.RegRt == EX_MEM_Reg.RegRd) {
        stall_flag = true;
        Branch_Stall = 1;
    }
    else if ((stage_ins[2] == ADD || stage_ins[2] == SUB) && IF_ID_Reg.RegRs == EX_MEM_Reg.RegRd) {
        stall_flag = true;
        Branch_Stall = 1;
    }
    else if ((stage_ins[2] == ADD || stage_ins[2] == SUB) && IF_ID_Reg.RegRt == EX_MEM_Reg.RegRd) {
        stall_flag = true;
        Branch_Stall = 1;
    }
    else if (stage_ins[3] == LW && IF_ID_Reg.RegRs == MEM_WB_Reg.RegRd) {
        stall_flag = true;
        Branch_Stall = 1;
    }
    else if (stage_ins[3] == LW && IF_ID_Reg.RegRt == MEM_WB_Reg.RegRd) {
        stall_flag = true;
        Branch_Stall = 1;
    }
}

void Branch_Data_Hazard_2nd_3nd(void) {

    if (stage_ins[1] != BEQ) return;
    if (ID_Stage.ReadReg1 == EX_MEM_Reg.RegRd) {
        ID_EX_Reg.ReadData1 = EX_MEM_Reg.ALU_Result;
    }
    else if (ID_Stage.ReadReg2 == EX_MEM_Reg.RegRd) {
        ID_EX_Reg.ReadData2 = EX_MEM_Reg.ALU_Result;
    }
    if (ID_Stage.ReadReg1 == MEM_WB_Reg.RegRd) {
        ID_EX_Reg.ReadData1 = MEM_WB_Reg.ALU_Result;
    }
    else if (ID_Stage.ReadReg2 == MEM_WB_Reg.RegRd) {
        ID_EX_Reg.ReadData2 = MEM_WB_Reg.ALU_Result;
    }
}

void Check_Branch_Hazard(void) {
    if (stall_flag == true) { return; }
    if (stage_ins[1] == BEQ && ID_EX_Reg.ReadData1 == ID_EX_Reg.ReadData2) {
        stall_flag = true;
        stage_ins[0] = "";
        memset(&IF_ID_Reg, 0, sizeof(IF_ID_Reg));
        Instruction_Backtrack(ID_EX_Reg.Immediate - 1);
    }
}

void Load_Use_Hazard_Flush(void) {
    if (Load_Use_Hazard) {
        stage_ins[1] = "";
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
        Load_Use_Hazard = 0;
        memset(&ID_EX_Reg.Ctl_WB, 0, sizeof(ID_EX_Reg.Ctl_WB));
        memset(&ID_EX_Reg.Ctl_M, 0, sizeof(ID_EX_Reg.Ctl_M));
        memset(&ID_EX_Reg.Ctl_Ex, 0, sizeof(ID_EX_Reg.Ctl_Ex));
    }
}

void EX_MEM_Forwarding(void) {
    if (EX_Hazard_Rs) {
        ID_EX_Reg.ReadData1 = EX_MEM_Reg.ALU_Result;
        EX_Hazard_Rs = 0;
    }
    if (EX_Hazard_Rt) {
        ID_EX_Reg.ReadData2 = EX_MEM_Reg.ALU_Result;
        EX_Hazard_Rt = 0;
    }
    if (MEM_Hazard_Rs) {
        ID_EX_Reg.ReadData1 = MEM_WB_Reg.ALU_Result;
        MEM_Hazard_Rs = 0;
    }
    if (MEM_Hazard_Rt) {
        ID_EX_Reg.ReadData2 = MEM_WB_Reg.ALU_Result;
        MEM_Hazard_Rt = 0;
    }
}

void Load_Use_Forwarding(void) {
    if (Load_Use_Hazard_Forwarding_Rs)
        ID_EX_Reg.ReadData1 = MEM_WB_Reg.DataOfMem;
    if (Load_Use_Hazard_Forwarding_Rt)
        ID_EX_Reg.ReadData2 = MEM_WB_Reg.DataOfMem;
}