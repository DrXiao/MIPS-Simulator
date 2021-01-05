/*
** 有hazard的地方，會多輸入一組產生無窮迴圈
** branch 還沒做
** hazard_MEM_WB還沒完成
** hazard_EXE_MEM 理論上應該算完成了
*/
#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>

#include "pipelineReg.h"
#include "util.h"
#include "forwarding/forwarding.h"
using namespace std;

#define PAUSE                                                                  \
    printf("Press any key to continue...");                                    \
    cin.get();

#define OUTPUT_FILE_OPEN 0

int main(void) {

    Init_Reg_Mem();

#if (OUTPUT_FILE_OPEN == 1)
    outputFilePtr = fopen("result.txt", "w");
#endif

    string instruction = "";
    string insToken[4] = {"", "", "", ""};

    fstream mipsIns;
    mipsIns.open("memory.txt", ios::in);

    cycle = 0;
    
    while (!(mipsIns.eof() && CheckEnding())) {
        printf("hazard_EX_MEM %d\n",hazard_EX_MEM);
        printf("hazard_MEM_WB %d\n",hazard_MEM_WB);
        if (!hazard_EX_MEM && !hazard_MEM_WB)
        {
            if (!mipsIns.eof()) {
                getline(mipsIns, instruction);
                Parse_Instruction(instruction, insToken);
            }
            else
                instruction = insToken[0] = "";
        }
        else { 
            instruction = insToken[0] = "";
        }
        
        // printf("imme = %d\n",IF_ID_Reg.Immediate);
        if(cycle>15) break;

        cycle += 1;
        fprintf(outputFilePtr, "Cycle %d : \n", cycle);
        Move_Stages_Instruction(insToken[0]);
        
        hazard_EX_MEM = 0,hazard_MEM_WB = 0;
        /* EX data hazard */
       if(((EX_MEM_Reg.Ctl_WB.Reg_Write == 1) && (EX_MEM_Reg.RegRd != 0) && (EX_MEM_Reg.RegRd == ID_EX_Reg.RegRs))
            || ((EX_MEM_Reg.Ctl_WB.Reg_Write == 1) && (EX_MEM_Reg.RegRd != 0) && (EX_MEM_Reg.RegRd == ID_EX_Reg.RegRt)))    
        {    
            hazard_EX_MEM = 1;
            instruction = insToken[0] = stage_ins[0] =  "";            
        }
        /* MEM data hazard */
        if(((MEM_WB_Reg.Ctl_WB.Reg_Write == 1) && (MEM_WB_Reg.RegRd != 0) && (MEM_WB_Reg.RegRd == ID_EX_Reg.RegRs)
            || (MEM_WB_Reg.Ctl_WB.Reg_Write == 1) && (MEM_WB_Reg.RegRd != 0) && (MEM_WB_Reg.RegRd == ID_EX_Reg.RegRt))
            && (!hazard_EX_MEM))
        {
            hazard_MEM_WB = 1;
            instruction = insToken[0] = stage_ins[0] = "";
        }

        Write_Back();
        Memory_Read_Write();
        Execute();
        Instruction_Decode();
        Instruction_Fetch(insToken);
        
    }
    mipsIns.close();

    fprintf(outputFilePtr, "MIPS code needs %d cycles\n", cycle);
    Print_Reg_Mem(outputFilePtr);

#if (OUTPUT_FILE_OPEN == 1)
    fclose(outputFilePtr);
#endif
    // PAUSE;
    return 0;
}
