#ifndef __FORWARDING_H__
#define __FORWARDING_H__


extern bool Load_Use_Hazard;
extern bool Branch_Hazard;
extern int Load_Use_count;

void Check_EX_And_MEM_Hazard(void);
void Check_Load_Use_Hazard(void);
void Check_Branch_Hazard(void);
void EX_MEM_Forwarding(void);
void Load_Use_Forwarding(void);
void Load_Use_Hazard_Flush(void);
void Check_BEQ_TAKEN(void);
void BEQ_Flush(void);

#endif