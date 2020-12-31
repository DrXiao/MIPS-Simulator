#ifndef __FORWARDING_H__
#define __FORWARDING_H__
#include <fstream>
using namespace std;

extern bool Load_Use_Hazard;
extern bool Branch_Hazard;

void Load_Use_Forwarding(void);
void CheckHazard(void);

#endif