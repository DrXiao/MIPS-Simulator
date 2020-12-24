#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "forwarding/pipelineReg.h"
using namespace std;

#define PAUSE                                                                  \
    printf("Press any key to continue...");                                    \
    fgetc(stdin);

extern IF_ID_Pipeline_Reg IF_ID_Reg;

static int mipsRegisters[32] = {0};
static int memory[32] = {0};

int main(void) {

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
            getline(ss, insToken[tokens], ',');
            cout << insToken[tokens] << " ";
        }
        cout << endl;
    }

    mipsIns.close();
    PAUSE;
    return 0;
}