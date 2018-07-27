
#ifndef _SOC_H
#define _SOC_H

//==================================================
#include "stdint.h" 
 
#if 1

#define NULL (void*)0 
typedef void (* TaskPtrType)(void);  
 
typedef enum
{
	TIMER_TASK_CLASS,
	MAIN_TASK_CLASS,
}eTaskClassType;  
 
typedef struct
{
 eTaskClassType TaskClass;    
 TaskPtrType TaskPtr;
}sTaskType;

//============================== 
extern void Init_Task(void);
extern void Timer_Task(void);
extern void Main_Task(void);


#endif  
#endif
	




