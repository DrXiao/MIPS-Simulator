#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

#define PAUSE                                                                  \
    printf("Press any key to continue...");                                    \
    fgetc(stdin);

static int mipsRegisters[32] = {0};
static int memory[32] = {0};

int main(void) {

    string instruction;
    string insToken[4];
    
    fstream mipsIns;
    mipsIns.open("memory.txt", ios::in);

    while(getline(mipsIns, instruction)) {
        stringstream ss(instruction);
        for(int tokens = 0 ; tokens < 4 ; tokens++) {
            getline(ss, insToken[tokens], ',');
            cout << insToken[tokens] << " ";
        }
        cout << endl;
    } 

    mipsIns.close();
    PAUSE;
    return 0;
}