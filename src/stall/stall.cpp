#include <cstring>
#include "stall.h"
#include "util.h"
#include "pipelineReg.h"
using namespace std;

bool hazard = 0;
void Bubble()
{    
    if(hazard)
    {
        // memset(&ID_EX_Reg,0,sizeof(ID_EX_Reg));    
        stage_ins[2] = "";
    }

}
