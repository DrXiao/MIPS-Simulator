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
        // cout<< "[ID_Stage.ReadReg1]: "<< ID_Stage.ReadReg1 <<endl
        //         << "[ID_Stage.ReadReg2]: "<< ID_Stage.ReadReg2 <<endl;
        // 要stall 並且取出暫存器比較 並決定來沖刷
        if(branch_stall)
        {
            stage_ins[1] = "";
            
            if(mipsRegisters[ID_Stage.ReadReg1] == mipsRegisters[ID_Stage.ReadReg2]) taken = true;
            else taken = false;
            // cout<< "[IF_ID_Reg.RegRs]: "<< IF_ID_Reg.RegRs <<endl
            //     << "[IF_ID_Reg.RegRt]: "<< IF_ID_Reg.RegRs <<endl;
            // cout<<"ID_EX_Reg.Immediate"<<ID_EX_Reg.Immediate<<endl;
            if(taken){
                Instruction_Backtrack(ID_EX_Reg.Immediate - 1);
                stage_ins[0] ="";
                
            }
        }
        Instruction_Decode();
        Instruction_Fetch(insToken);

        
        Check_hazard();
        // 判斷是否發生branch branch_stall = 1
        if(!hazard && stage_ins[1] == BEQ){
            branch_stall = true ;
            Instruction_Backtrack(-1);
        }
        else branch_stall = false;
        //如果branch，則判斷是否taken 並且stall 
        
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
