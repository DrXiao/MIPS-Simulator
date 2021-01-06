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
    
    // cout << "EX_MEM_Reg.RegRd " << EX_MEM_Reg.RegRd << endl
    //      << "ID_EX_Reg.RegRd " << ID_EX_Reg.RegRd << endl
    //      << "IF_ID_Reg.RegRs " << IF_ID_Reg.RegRs << endl
    //      << "IF_ID_Reg.RegRt " << IF_ID_Reg.RegRt << endl;

    if(((ID_EX_Reg.Ctl_WB.Reg_Write == 1) && (ID_EX_Reg.RegRd != 0) 
            && (ID_EX_Reg.RegRd == IF_ID_Reg.RegRs))
        || ((ID_EX_Reg.Ctl_WB.Reg_Write == 1) && (ID_EX_Reg.RegRd != 0) 
            && (ID_EX_Reg.RegRd == IF_ID_Reg.RegRt)))    
    {   
        hazard = 1;
        if(EOF_count < 2)
            Instruction_Backtrack(-1);
        printf("EXE hazard\n");       
    }
                
        /* MEM data hazard */
    else if(((EX_MEM_Reg.Ctl_WB.Reg_Write == 1) && (EX_MEM_Reg.RegRd != 0) 
            && (EX_MEM_Reg.RegRd == IF_ID_Reg.RegRs))
        || ((EX_MEM_Reg.Ctl_WB.Reg_Write == 1) && (EX_MEM_Reg.RegRd != 0) 
            && (EX_MEM_Reg.RegRd == IF_ID_Reg.RegRt)))    
    { 
    /*else if((((MEM_WB_Reg.Ctl_WB.Reg_Write == 1) && (MEM_WB_Reg.RegRd != 0) 
                && (MEM_WB_Reg.RegRd == IF_ID_Reg.RegRs))
        || ((MEM_WB_Reg.Ctl_WB.Reg_Write == 1) && (MEM_WB_Reg.RegRd != 0) 
            && (MEM_WB_Reg.RegRd == IF_ID_Reg.RegRt))))
    {*/
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


