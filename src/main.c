#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define PAUSE                                                                  \
    printf("Press any key to continue...");                                    \
    fgetc(stdin);

static uint32_t mipsRegisters[32] = {[0] = 0, [1 ... 31] = 1};
static uint32_t memory[32] = {[0 ... 31] = 1};


int main(void) {
    
    FILE *mipsIns = fopen("memory.txt", "r");
    char inputBuf[30] = "";
    char instruction[4][10] = {"", "", "", ""};
    char *token = NULL;
    while (fgets(inputBuf, 30, mipsIns) != NULL) {
        char *assignPtr = instruction;
        token = strtok(inputBuf, " ,\n");
        while (token != NULL) {
            strcpy(assignPtr, token);
            assignPtr += 10;
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