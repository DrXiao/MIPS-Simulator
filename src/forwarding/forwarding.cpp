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
bool Branch_Hazard = 0;
bool BEQ_Taken=0;

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

void Check_EX_And_MEM_Hazard(void) {
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
    // Load-Use hazard
    bool Hazard = false;
    if (LOAD_USE_HAZARD(Rs)) {
        Load_Use_Hazard = 1;
        Load_Use_Hazard_Forwarding_Rs = 1;
        Load_Use_count = 3;
        Hazard = true;
    }
    if (LOAD_USE_HAZARD(Rt)) {
        Load_Use_Hazard = 1;
        Load_Use_Hazard_Forwarding_Rt = 1;
        Load_Use_count = 3;
        Hazard = true;
    }
    if (Hazard) {
        Instruction_Backtrack(-1);
    }
    if (Load_Use_count != 0) {
        Load_Use_count--;
        if (!Load_Use_count) { Load_Use_Forwarding(); }
    }
}

void Load_Use_Hazard_Flush(void) {
    if (Load_Use_Hazard) {
        stage_ins[1] = "";
        IF_ID_Reg.OpCode = insToken[0];
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
        ID_EX_Reg.ReadData1 = WB_Stage.WriteBackData;
        MEM_Hazard_Rs = 0;
    }
    if (MEM_Hazard_Rt) {
        ID_EX_Reg.ReadData2 = WB_Stage.WriteBackData;
        MEM_Hazard_Rt = 0;
    }
}

void Load_Use_Forwarding(void) {
    if (Load_Use_Hazard_Forwarding_Rs)
        ID_EX_Reg.ReadData1 = MEM_WB_Reg.DataOfMem;
    if (Load_Use_Hazard_Forwarding_Rt)
        ID_EX_Reg.ReadData2 = MEM_WB_Reg.DataOfMem;
}

void Check_BEQ_TAKEN(void){
    BEQ_Taken=true;
}

void BEQ_Flush(void){
    if(BEQ_Taken){
        stage_ins[0] = "";
        stage_ins[1] = "";
        IF_ID_Reg.OpCode = "";
        
        sscanf(insToken[1].c_str(), "$%hu", &IF_ID_Reg.RegRs);
        sscanf(insToken[2].c_str(), "$%hu", &IF_ID_Reg.RegRt);
        IF_ID_Reg.Immediate = stol(insToken[3]);
        
        memset(&ID_EX_Reg.Ctl_WB, 0, sizeof(ID_EX_Reg.Ctl_WB));
        memset(&ID_EX_Reg.Ctl_M, 0, sizeof(ID_EX_Reg.Ctl_M));
        memset(&ID_EX_Reg.Ctl_Ex, 0, sizeof(ID_EX_Reg.Ctl_Ex));

        int pc_register = atoi(insToken[3].c_str());
        Instruction_Backtrack(pc_register);
        BEQ_Taken=false;
    }
}