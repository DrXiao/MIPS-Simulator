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
        if (Load_Use_Hazard)
            Instruction_Backtrack(-1);
        if (mipsIns.eof() == false) {
            prevIns.seekg(mipsIns.tellg());
            getline(mipsIns, instruction);
            Parse_Instruction(instruction, insToken);
        }
        else
            instruction = insToken[0] = "";
        cout << prevIns.tellg() << " " << mipsIns.tellg() << endl;
        if (cycle > 15) break;
        fprintf(outputFilePtr, "Cycle %d : \n", cycle);
        Move_Stages_Instruction(insToken[0]);
        Load_Use_Forwarding();
        CheckHazard();

        if (Load_Use_Hazard) printf("Hazard!\n");
        Write_Back();
        Memory_Read_Write();
        Execute();
        Instruction_Decode();
        Instruction_Fetch(insToken);
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
