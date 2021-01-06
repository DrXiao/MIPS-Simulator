#include <cstring>
#include <iostream>
#include "stall.h"
#include "util.h"
#include "pipelineReg.h"
using namespace std;

bool hazard = 0;
int branch = 0;
//0 原本沒有發生
//1 還有發生一次
//2 還要發生兩次
int EOF_count = 0;
bool branch_taken = false;
void Bubble()
{    
    if(hazard)
    {
        //EXE被沖刷
        stage_ins[2] = "";
    }

}

void Check_hazard()
{
    hazard = 0;
        /* EX data hazard */
    
    if(((EX_MEM_Reg.Ctl_WB.Reg_Write == 1) && (EX_MEM_Reg.RegRd != 0) 
            && (EX_MEM_Reg.RegRd == ID_EX_Reg.RegRs))
        || ((EX_MEM_Reg.Ctl_WB.Reg_Write == 1) && (EX_MEM_Reg.RegRd != 0) 
            && (EX_MEM_Reg.RegRd == ID_EX_Reg.RegRt))
        && !branch)    
    {   
        hazard = 1;
        if(EOF_count < 2)
            Instruction_Backtrack(-1);
        printf("EXE hazard\n");       
    }
                
        /* MEM data hazard */
    else if((((MEM_WB_Reg.Ctl_WB.Reg_Write == 1) && (MEM_WB_Reg.RegRd != 0) 
                && (MEM_WB_Reg.RegRd == ID_EX_Reg.RegRs))
        || ((MEM_WB_Reg.Ctl_WB.Reg_Write == 1) && (MEM_WB_Reg.RegRd != 0) 
            && (MEM_WB_Reg.RegRd == ID_EX_Reg.RegRt)))
        && !branch)
    {
        hazard = 1;
        if(EOF_count < 2)
            Instruction_Backtrack(-1);
        printf("MEM hazard\n");  
    }

}


void Check_BEQ_Taken()
{
    branch_taken = true;
}

void Branch_Flush()
{
    
    if(branch_taken)
    {
        cout<<"flush!!!"<<endl;
    }

} 


