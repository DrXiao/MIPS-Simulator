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
    prevIns.open("memory.txt", ios::in);

    cycle = 0;
    int EOF_count = 0;
    while (!(mipsIns.eof() && CheckEnding()))  {
        cycle += 1;
        cout << prevIns.tellg() << " " <<  mipsIns.tellg() << endl;
        if (!mipsIns.eof()) {
            prevIns.seekg(mipsIns.tellg());
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

        Bubble_MEM();
        
        Memory_Read_Write();

        Bubble_EXE();

        Execute();
        Instruction_Decode();
        Instruction_Fetch(insToken);


        /* 檢查hazard */
        hazard_EX_MEM = 0,hazard_MEM_WB = 0;
        /* EX data hazard */
        
       if(((EX_MEM_Reg.Ctl_WB.Reg_Write == 1) && (EX_MEM_Reg.RegRd != 0) 
            && (EX_MEM_Reg.RegRd == ID_EX_Reg.RegRs))
            || ((EX_MEM_Reg.Ctl_WB.Reg_Write == 1) && (EX_MEM_Reg.RegRd != 0) 
            && (EX_MEM_Reg.RegRd == ID_EX_Reg.RegRt)))    
        {    
            hazard_EX_MEM = 1;
            if(EOF_count < 2)
                Instruction_Backtrack(-1);
            printf("EXE hazard\n");       
        }
        /* MEM data hazard */
        if((((MEM_WB_Reg.Ctl_WB.Reg_Write == 1) && (MEM_WB_Reg.RegRd != 0) && (MEM_WB_Reg.RegRd == ID_EX_Reg.RegRs))
            || ((MEM_WB_Reg.Ctl_WB.Reg_Write == 1) && (MEM_WB_Reg.RegRd != 0) && (MEM_WB_Reg.RegRd == ID_EX_Reg.RegRt)))
            && !hazard_EX_MEM)
        {
            hazard_MEM_WB = 1;
            Instruction_Backtrack(-1);
            printf("MEM hazard\n");  
        }
        for(int i = 0;i<5;i++)
            cout<<"stage "<< i << " " <<stage_ins[i] << endl;
        
    }
    mipsIns.close();
    prevIns.close();

    fprintf(outputFilePtr, "MIPS code needs %d cycles\n", cycle);
    Print_Reg_Mem(outputFilePtr);

#if (OUTPUT_FILE_OPEN == 1)
    fclose(outputFilePtr);
#endif
    // PAUSE;
    return 0;
}
