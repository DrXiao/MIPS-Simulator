#ifndef __STALL_H__
#define __STALL_H__

extern int EOF_count;
extern bool hazard;
extern bool taken;
extern bool branch_stall;
void Bubble(void);
void Check_hazard();
void Check_Branch(void);
void Branch_Flush(void);
void Check_BEQ_Taken(void);

#endif