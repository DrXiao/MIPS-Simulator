#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>

#include "pipelineReg.h"
#include "forwarding/util.h"
#include "forwarding/forwarding.h"
using namespace std;

#define PAUSE                                                                  \
    printf("Press any key to continue...");                                    \
    cin.get();

#define OUTPUT_FILE_OPEN 0

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
    }
    mipsIns.close();

    FILE *outputFilePtr = stdout;
#if (OUTPUT_FILE_OPEN == 1)
    outputFilePtr = fopen("result.txt", "a");
#endif

    fprintf(outputFilePtr ,"MIPS code needs %d cycles\n", cycle);
    Print_Reg_Mem(outputFilePtr);

#if (OUTPUT_FILE_OPEN == 1)    
    fclose(outputFilePtr);
#endif
    PAUSE;
    return 0;
}