#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>

#include "pipelineReg.h"
#include "pipelineUnit.h"
#include "util.h"
#include "stall.h"
using namespace std;

#define PAUSE                                                                  \
    printf("Press any key to continue...");                                    \
    cin.get();

#define OUTPUT_FILE_OPEN 1

int main(void) {

    Init_Reg_Mem();

#if (OUTPUT_FILE_OPEN == 1)
    outputFilePtr = fopen("result.txt", "w");
#endif

    mipsIns.open("memory.txt", ios::in);
    headIns.open("memory.txt", ios::in);

    cycle = 0;
    string instruction = "";
    string *insToken = new string[4];

    while (!(mipsIns.tellg() == -1 && CheckEnding())) {
        cycle += 1;

        if (!mipsIns.eof()) {
            insLine++;
            getline(mipsIns, instruction);
            Parse_Instruction(instruction, insToken);
        }
        else {
            instruction = "";
            insToken[0] = "";
            EOF_count++;
        }
        fprintf(outputFilePtr, "Cycle %d : \n", cycle);
        Move_Stages_Instruction(insToken[0]);

        Check_hazard();

        Write_Back();
        Memory_Read_Write();
        Execute();

        Instruction_Decode();
        Instruction_Fetch(insToken);

        if (!hazard && stage_ins[1] == BEQ) {
            if (mipsRegisters[ID_Stage.ReadReg1] ==
                mipsRegisters[ID_Stage.ReadReg2]) {
                stage_ins[0] = "";
                Instruction_Backtrack(ID_EX_Reg.Immediate - 1);
            }
        }
    }
    mipsIns.close();
    headIns.close();

    fprintf(outputFilePtr, "MIPS code needs %d cycles\n", cycle);
    Print_Reg_Mem(outputFilePtr);

    delete[] insToken;

#if (OUTPUT_FILE_OPEN == 1)
    fclose(outputFilePtr);
#endif
    // PAUSE;
    return 0;
}
