#ifndef __FORWARDING_H__
#define __FORWARDING_H__
#include <fstream>
using namespace std;

extern bool Load_Use_Hazard;
extern bool Branch_Hazard;
extern int Load_Use_count;

void Check_EX_And_MEM_Hazard(void);
void Check_Load_Use_Hazard(void);
void Load_Use_Forwarding(void);
void Load_Use_Hazard_Flush(void);

#endif