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

    while (!(mipsIns.tellg() == -1 && CheckEnding())) {
        cycle += 1;

        // cout << prevIns.tellg() << " " <<  mipsIns.tellg() << endl;
        if (!mipsIns.eof()) {
            insLine++;
            getline(mipsIns, instruction);
            Parse_Instruction(instruction, insToken);
        }
        else {
            // cout << "EOF!!!" << endl;
            instruction = insToken[0] = "";
            EOF_count++;
        }
        if(cycle>25) break;
        fprintf(outputFilePtr, "Cycle %d : \n", cycle);
        Move_Stages_Instruction(insToken[0]);

        Write_Back();
        Bubble(); //
        Memory_Read_Write();
        Bubble(); //
        Execute();
        // cout<< "[ID_Stage.ReadReg1]: "<< ID_Stage.ReadReg1 <<endl
        //         << "[ID_Stage.ReadReg2]: "<< ID_Stage.ReadReg2 <<endl;
        // 要stall 並且取出暫存器比較 並決定來沖刷

        Instruction_Decode();
        Instruction_Fetch(insToken);

        Check_hazard();
        if(!hazard && stage_ins[1] == BEQ)
        {
            if(mipsRegisters[ID_Stage.ReadReg1] == mipsRegisters[ID_Stage.ReadReg2])
            {
                stage_ins[0] = "";
                cout<<" jump to "<<ID_EX_Reg.Immediate<<endl;
                Instruction_Backtrack(ID_EX_Reg.Immediate - 1);
            }
        }
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
