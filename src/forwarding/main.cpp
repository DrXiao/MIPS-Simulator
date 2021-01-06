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
    headIns.open("memory.txt", ios::in);

    cycle = 0;

    while (mipsIns.eof() == false || CheckEnding() == false) {
        cycle += 1;
        Stall_Flag_Re();

        if (mipsIns.eof() == false) {
            insLine++;
            getline(mipsIns, instruction);
            Parse_Instruction(instruction, insToken);
        }
        else
            instruction = insToken[0] = "";
        if (cycle > 15) break;
        fprintf(outputFilePtr, "Cycle %d : \n", cycle);
        Move_Stages_Instruction(insToken[0]);

        Write_Back();
        Memory_Read_Write();
        Execute();
        Check_Branch_Stall();
        // BEQ_Flush();
        Load_Use_Hazard_Flush();
        Instruction_Decode();
        Instruction_Fetch(insToken);
        if (Branch_Stall) {
            Instruction_Backtrack(-1);
            continue;
        }

        Check_EX_And_MEM_Hazard();
        Check_Load_Use_Hazard();
        Branch_Data_Hazard_2nd_3nd();
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

0
0000 0001  0000 0000  0000 0000  0000 0000

1
0000 0001  0000 0000  0000 0000  0000 0000

2
0000 0001  0000 0000  0000 0000  0000 0000

3
0000 0001  0000 0000  0000 0000  0000 0000

4
0000 0001  0000 0000  0000 0000  0000 0000

5
0000 0001  0000 0000  0000 0000  0000 0000

6
0000 0001  0000 0000  0000 0000  0000 0000

7
0000 0001  0000 0000  0000 0000  0000 0000

8
0000 0001  0000 0000  0000 0000  0000 0000

*/