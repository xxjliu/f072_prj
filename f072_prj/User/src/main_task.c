
#include "../ins/main_task.h"
#include "../ins/sch.h" 

#if 1

#define MAX_NUM_MAIN_TASK 	5
#define MAX_NUM_TM_TASK 		5

const sTaskType MainTaskStack[MAX_NUM_MAIN_TASK] = 
  {
    #ifndef __DEBUG_DISABLE_PRODUCT_TASK  
      {
        MAIN_TASK_CLASS,    
        Sch_MainCallback,
      },
    #endif      
    #ifdef __DEBUG_TASK
     {  
       MAIN_TASK_CLASS,
       Debug_MainTask,
     }, 
    #endif  
  };
   
const sTaskType TimerTaskStack[MAX_NUM_TM_TASK] = 
  {
    #ifndef __DEBUG_DISABLE_PRODUCT_TASK       
     /* {
        TIMER_TASK_CLASS,  //==0
        Sch_CounterCallback,
      }, */
      {
        TIMER_TASK_CLASS,
        Sch_TimerCallback,
      },
    #endif
    #ifdef __DEBUG_TASK
      {
        TIMER_TASK_CLASS,
        Debug_TimerTask,
      },
    #endif  
  };
 
//============================================================
void Init_Task(void)
{
		Sch_InitCallback();
}
//============================================================
void Timer_Task(void)
{
		unsigned int i;
		for(i = 0; i < MAX_NUM_TM_TASK; i++)
		{
			if( (TIMER_TASK_CLASS == TimerTaskStack[i].TaskClass) && (NULL != TimerTaskStack[i].TaskPtr))
			{
				(*TimerTaskStack[i].TaskPtr)();
			}
			else
			{
				break;
			}
		}
}

//============================================================
void Main_Task(void)
{
		unsigned int i;
		for(i = 0; i < MAX_NUM_MAIN_TASK; i++)
		{
				if( (MAIN_TASK_CLASS == MainTaskStack[i].TaskClass) && (NULL != MainTaskStack[i].TaskPtr))
				{
						(*MainTaskStack[i].TaskPtr)();
				}
				else
				{
						break;
				}
		}
}
#endif