#ifndef __STALL_H__
#define __STALL_H__

extern int EOF_count;
extern bool hazard;
extern int branch;
extern bool branch_taken;
void Bubble(void);
void Check_hazard();
void Check_Branch(void);
void Branch_Flush(void);
void Check_BEQ_Taken(void);
#endif