
#ifndef _SCH_H
#define _SCH_H

//==================================================
#include "stdint.h" 

#if 1
  
typedef struct
{
    uint16_t ResetFromCodeError;
    uint16_t ov_cnt;
    uint16_t uv_cnt;
} sSch_TmCntType;
   
   
typedef struct
{
    uint8_t PowerOff_Reset; 
} sSch_SysFlagType;
   
//==============================
 
extern void Sch_InitCallback(void);
extern void Sch_MainCallback(void);
extern void Sch_TimerCallback(void);
#endif
	
	
#endif
	




