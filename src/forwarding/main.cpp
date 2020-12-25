#include <iostream>
#include <string>
#include <fstream>

#include "pipelineReg.h"
#include "forwarding/util.h"
#include "forwarding/forwarding.h"
using namespace std;

#define PAUSE                                                                  \
    printf("Press any key to continue...");                                    \
    cin.get();

int main(void) {

    Init_Reg_Mem();

    string instruction;
    string insToken[4];

    fstream mipsIns;
    mipsIns.open("memory.txt", ios::in);

    int cycle = 0;
    while (getline(mipsIns, instruction)) {
        cycle += 1;
        Parse_MPIS_Ins(instruction, insToken);
        cout << insToken[0] << " " << insToken[1] << " " << insToken[2] << " "
             << insToken[3] << endl;

        cout << endl;
    }

    mipsIns.close();
    PAUSE;
    return 0;
}