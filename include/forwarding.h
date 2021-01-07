#ifndef __FORWARDING_H__
#define __FORWARDING_H__

extern bool Load_Use_Hazard;
extern int Load_Use_count;

extern int Branch_Stall;
extern bool Branch_Hazard;

void Check_EX_And_MEM_Hazard(void);
void Check_Load_Use_Hazard(void);
void Check_Branch_Stall(void);
void Branch_Data_Hazard_2nd_3nd(void);
void Check_Branch_Hazard(void);
void EX_MEM_Forwarding(void);
void Load_Use_Forwarding(void);
void Load_Use_Hazard_Flush(void);
void Stall_Flag_Re(void);

#endif