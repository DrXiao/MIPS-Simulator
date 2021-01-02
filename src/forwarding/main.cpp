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
    prevIns.open("memory.txt", ios::in);

    cycle = 0;

    while (mipsIns.eof() == false || CheckEnding() == false) {
        cycle += 1;
        if (mipsIns.eof() == false) {
            if (Load_Use_count - 1 != 1) {
                prevIns.seekg(mipsIns.tellg());
                getline(mipsIns, instruction);
                Parse_Instruction(instruction, insToken);
            }
        }
        else
            instruction = insToken[0] = "";
        if (cycle > 15) break;
        fprintf(outputFilePtr, "Cycle %d : \n", cycle);
        Move_Stages_Instruction(insToken[0]);
        Check_EX_And_MEM_Hazard();
        Load_Use_Forwarding();
        Check_Load_Use_Hazard();
        Write_Back();
        Memory_Read_Write();
        Execute();
        Instruction_Decode();
        Instruction_Fetch(insToken);
        Load_Use_Hazard_Flush();
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


/*

0
0000 0000  0000 0000  0000 0000  0000 0001

1
0000 0000  0000 0000  0000 0000  0000 0001

2
0000 0000  0000 0000  0000 0000  0000 0001

3
0000 0000  0000 0000  0000 0000  0000 0001

4
0000 0000  0000 0000  0000 0000  0000 0001

5
0000 0000  0000 0000  0000 0000  0000 0001


*/