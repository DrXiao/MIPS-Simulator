#include "stall.h"
#include "util.h"
#include "pipelineReg.h"
using namespace std;


void Bubble_EXE()
{    
    if(hazard_EX_MEM)
    {
        ID_EX_Reg.Ctl_WB = {.Reg_Write = 0, .MemToReg = 0};
        ID_EX_Reg.Ctl_M = {.Branch = 0, .Mem_Read = 0, .Mem_Write = 0};
        ID_EX_Reg.Ctl_Ex = {.RegDst = 0, .ALUOp = 0, .ALUSrc = 0};
        ID_EX_Reg.ReadData1 = 0;
        ID_EX_Reg.ReadData2 = 0;
        ID_EX_Reg.Immediate = 0;
        ID_EX_Reg.RegRs = 0;
        ID_EX_Reg.RegRt = 0;
        ID_EX_Reg.RegRd = 0;
        stage_ins[2] = "";
    }

}
#include "stall.h"
#include "util.h"
#include "pipelineReg.h"
using namespace std;


void Bubble_MEM()
{        
    if(hazard_MEM_WB)
    {   
        stage_ins[2] = "";
    }
}