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
void Bubble()
{    
    if(hazard)
    {
        //EXE被沖刷
        // memset(&ID_EX_Reg,0,sizeof(ID_EX_Reg));    
        stage_ins[2] = "";
    }

}

void Check_Branch()
{
    
    if(stage_ins[1] == BEQ && branch == 0)
    {
        // printf("!!!!!!Branch now!!!!!!Branch = %d\n", branch);
        branch = 2;
    }

    cout<< "branch " << branch <<endl;
    // if(branch != 0)
    // {
    //     branch = branch - 1;
    //     if(EOF_count < 2 && branch != 1)
    //         Instruction_Backtrack(-1);
    // }

}

void Branch_bubble()
{
    if(branch == 0) return;
    stage_ins[2] = "";
    memset(&IF_ID_Reg,0,sizeof(IF_ID_Reg));
    if(branch != 0)
    {
        branch = branch - 1;
        // cout<<"branch = "<<branch<<"???????"<<endl;
        if(EOF_count < 2 && branch != 0)
            Instruction_Backtrack(-1);
    }

} 
