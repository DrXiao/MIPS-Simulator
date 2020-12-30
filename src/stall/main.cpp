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
        // if(((EX_MEM_Reg.Ctl_WB.Reg_Write == 1) && (EX_MEM_Reg.RegRd != 0)
        //         && (EX_MEM_Reg.RegRd == ID_EX_Reg.RegRs))
        //     || ((EX_MEM_Reg.Ctl_WB.Reg_Write == 1) && (EX_MEM_Reg.RegRd != 0)
        //         && (EX_MEM_Reg.RegRd == ID_EX_Reg.RegRt)))    
            
            
        
        if (!mipsIns.eof()) {
            getline(mipsIns, instruction);
            Parse_Instruction(instruction, insToken);
        }
        else
            instruction = insToken[0] = "";

        /*印出來抗抗*/
        printf("%s rs = %d\n",insToken[0].c_str(),IF_ID_Reg.RegRs);
        printf("%s rt = %d\n",insToken[0].c_str(),IF_ID_Reg.RegRt);
        printf("%s rd = %d\n",insToken[0].c_str(),IF_ID_Reg.RegRd);
        printf("imme = %d\n",IF_ID_Reg.Immediate);
 
 
        cycle += 1;
        fprintf(outputFilePtr, "Cycle %d : \n", cycle);
        Move_Stages_Instruction(insToken[0]);
        
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
