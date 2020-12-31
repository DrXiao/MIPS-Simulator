#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>

#include "pipelineReg.h"
#include "util.h"
#include "forwarding.h"
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

    cycle = 0;

    while (!(mipsIns.eof() && CheckEnding())) {
        if (!mipsIns.eof()) {
            getline(mipsIns, instruction);
            Parse_Instruction(instruction, insToken);
        }
        else
            instruction = insToken[0] = "";
        if (cycle > 15)
            break;
        cycle += 1;
        fprintf(outputFilePtr, "Cycle %d : \n", cycle);
        Move_Stages_Instruction(insToken[0]);
        Load_Use_Forwarding();
        CheckHazard();

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
