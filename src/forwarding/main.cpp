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

    string instruction = "";
    string insToken[4] = {"", "", "", ""};

    fstream mipsIns;
    mipsIns.open("memory.txt", ios::in);

    int cycle = 0;
    while (!(mipsIns.eof() && CheckEnding())) {
        if(!mipsIns.eof())
            getline(mipsIns, instruction);
        else
            instruction = insToken[0] = "";
        cycle += 1;
        Write_Back();
        Memory_Read_Write();
        Execute();
        Instruction_Decode();
        Instruction_Fetch(instruction ,insToken);
        Move_Stages_Instruction(insToken[0]);
    }
    mipsIns.close();

    FILE *outputFilePtr = stdout;
#if (OUTPUT_FILE_OPEN == 1)
    outputFilePtr = fopen("result.txt", "a");
#endif

    fprintf(outputFilePtr ,"MIPS code needs %d cycles\n", cycle);
    Print_Reg_Mem(outputFilePtr);

#if (OUTPUT_FILE_OPEN == 1)    
    fclose(outputFilePtr);
#endif
    PAUSE;
    return 0;
}