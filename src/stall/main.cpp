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
            // cout << "EOF!!!" << endl;
            instruction = insToken[0] = "";
            EOF_count++;
        }
        if(cycle>15) break;
        fprintf(outputFilePtr, "Cycle %d : \n", cycle);
        Move_Stages_Instruction(insToken[0]);


        Write_Back();
        Bubble();           //
        Memory_Read_Write();
        Bubble();           //
        Execute();
        Instruction_Decode();
        Instruction_Fetch(insToken);
        
        Check_hazard();
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