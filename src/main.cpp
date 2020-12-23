#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define PAUSE                                                                  \
    printf("Press any key to continue...");                                    \
    fgetc(stdin);

static uint32_t mipsRegisters[32] = {0};
static uint32_t memory[32] = {0};

int main(void) {

    for(int indexOfReg_Mem = 0 ; indexOfReg_Mem < 32  ; indexOfReg_Mem++) {
        mipsRegisters[indexOfReg_Mem] = 1;
        memory[indexOfReg_Mem] = 1;
    }
    mipsRegisters[0] = 0;
    FILE *mipsIns = fopen("memory.txt", "r");
    char inputBuf[30] = "";
    char instruction[4][10] = {"", "", "", ""};
    char *token = NULL;
    while (fgets(inputBuf, 30, mipsIns) != NULL) {
        int index = 0;
        char *assignPtr = instruction[index++];
        token = strtok(inputBuf, " ,\n");
        while (token != NULL) {
            strcpy(assignPtr, token);
            assignPtr = instruction[index++];
            token = strtok(NULL, " ,\n");
        }
        printf("%s %s %s %s\n", instruction[0], instruction[1], instruction[2],
               instruction[3]);

        memset(inputBuf, 0, sizeof(inputBuf));
    }
    fclose(mipsIns);
    // PAUSE;

    return 0;
}