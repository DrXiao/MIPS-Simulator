#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "pipelineReg.h"
using namespace std;


#define PAUSE                                                                  \
    printf("Press any key to continue...");                                    \
    fgetc(stdin);


static int mipsRegisters[32] = {0};
static int memory[32] = {0};

extern IF_ID_Pipeline_Reg IF_ID_Reg;
extern ID_EX_Pipeline_Reg ID_EX_Reg;
extern EX_MEM_Pipeline_Reg EX_MEM_Reg;
extern MEM_WB_Pipeline_Reg MEM_WB_Reg;


int main(void) {
    vector<inst> pipeline;
    /* 按照規定給 Register賦值  */
    /* 除了0號暫存器為0         */
    /* 其他為 1                 */

    for (int idxOfReg_Mem = 0; idxOfReg_Mem < 32; idxOfReg_Mem++) {
        mipsRegisters[idxOfReg_Mem] = memory[idxOfReg_Mem] = 1;
    }
    mipsRegisters[0] = 0;

    string instruction;
    string insToken[4];

    fstream mipsIns;
    mipsIns.open("memory.txt", ios::in);

    int cycle = 0;
    while (getline(mipsIns, instruction)) {
        cycle += 1;
        stringstream ss(instruction);
        for (int tokens = 0; tokens < 4; tokens++) {
            getline(ss, insToken[tokens], ' ');
            for(int i = 0;i<insToken[tokens].length();i++){
                if(insToken[tokens][i] == ',')
                    insToken[tokens][i]='\0';
                if (insToken[0] == "lw"){
                    
                }
                else if (insToken[0] == "sw"){}
                else if (insToken[0] == "add"){}
                else if (insToken[0] == "sub"){}
                else if (insToken[0] == "beq"){}
            }
        }




        cout << endl;
    }

    mipsIns.close();
    PAUSE;
    return 0;
}