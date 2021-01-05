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
        
        if (mipsIns.eof() == false ) {
            cout << prevIns.tellg() << " " <<  mipsIns.tellg() << endl;
            prevIns.seekg(mipsIns.tellg());
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
        BEQ_Flush();
        Load_Use_Hazard_Flush();
        Instruction_Decode();
        Instruction_Fetch(insToken);

        Check_EX_And_MEM_Hazard();
        Check_Load_Use_Hazard();
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