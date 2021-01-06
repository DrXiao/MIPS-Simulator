#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>

#include "pipelineReg.h"
#include "util.h"
#include "stall.h"
using namespace std;

#define PAUSE                                                                  \
    printf("Press any key to continue...");                                    \
    cin.get();

#define OUTPUT_FILE_OPEN 0

string instruction = "";
string insToken[4] = {"", "", "", ""};

int main(void) {

    Init_Reg_Mem();

#if (OUTPUT_FILE_OPEN == 1)
    outputFilePtr = fopen("result.txt", "w");
#endif
 
    mipsIns.open("memory.txt", ios::in);
    headIns.open("memory.txt", ios::in);

    cycle = 0;
    
    while (!(mipsIns.tellg()==-1 && CheckEnding()))  {
        cycle += 1;
        // cout << prevIns.tellg() << " " <<  mipsIns.tellg() << endl;
        if (!mipsIns.eof()) {
            insLine++;
            getline(mipsIns, instruction);
            Parse_Instruction(instruction, insToken);
        }
        else{
            cout << "EOF!!!" << endl;
            instruction = insToken[0] = "";
            EOF_count++;
        }
        if(cycle>15) break;
        fprintf(outputFilePtr, "Cycle %d : \n", cycle);

        Move_Stages_Instruction(insToken[0]);

        Write_Back();

        Bubble();
        
        Memory_Read_Write();
        Bubble();
        Execute();
        // Branch_bubble(); 
        Instruction_Decode();
        Instruction_Fetch(insToken);
        
        
        // Check_Branch();

        /* 檢查hazard */
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
        // cout<< "MEM_WB_Reg.RegRd: "<<MEM_WB_Reg.RegRd<<endl
        //         << "EX_MEM_Reg.RegRd: "<<EX_MEM_Reg.RegRd<<endl
        //         << "ID_EX_Reg.RegRs: "<<ID_EX_Reg.RegRs<<endl
        //         << "IF_ID_Reg.RegRs: "<<IF_ID_Reg.RegRs<<endl
        //         << "IF_ID_Reg.RegRt: "<<IF_ID_Reg.RegRt<<endl
        //         << "ID_EX_Reg.RegRt: "<<ID_EX_Reg.RegRt<<endl
        //         << "EX_Hazard: "<<hazard_EX_MEM<<endl;
                
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
        // for(int i = 0;i<5;i++)
        //     cout<<"stage "<< i << " " <<stage_ins[i] << endl;
        
    }
    mipsIns.close();
    headIns.close();

    fprintf(outputFilePtr, "MIPS code needs %d cycles\n", cycle);
    Print_Reg_Mem(outputFilePtr);

#if (OUTPUT_FILE_OPEN == 1)
    fclose(outputFilePtr);
#endif
    // PAUSE;
    return 0;
}

/*
* beq 只寫ㄌstall
* 還沒判斷跳躍
* 給值還沒寫
*/