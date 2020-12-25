#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "pipelineReg.h"
#include "forwarding/init.h"
#include "forwarding/forwarding.h"
using namespace std;

#define PAUSE                                                                  \
    printf("Press any key to continue...");                                    \
    fgetc(stdin);


int main(void) {

    Init_Reg_Mem();

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