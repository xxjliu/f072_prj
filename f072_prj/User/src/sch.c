
#include "stm32f0xx_hal.h"
#include "../ins/sch.h"
#include "../ins/LvM.h"
#include "../ins/Sci.h"
#include "../ins/flash_user.h"
 
#if 1

sSch_TmCntType SysCnt;

sSch_SysFlagType SysFlag;

void var_init(void)
{ 
	uint16_t i;
	uint8_t *p_data; 
	p_data = (uint8_t *)(&SysCnt); 
	for(i = 0; i < sizeof(SysCnt); i++)
	{
		*(p_data++) = 0;
	}  
} 
//====Check if the system has resumed from off power reset  
void clear_reset_flag(void)
{  
	if(__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST) != RESET)
	{ 
			SysFlag.PowerOff_Reset = 1; 
	}
	else
	{
			SysFlag.PowerOff_Reset = 0; 
	}  
	__HAL_RCC_CLEAR_RESET_FLAGS(); 
	
}
void soft_reset(void)
{
	if(SysCnt.ResetFromCodeError < 200)
	{
		SysCnt.ResetFromCodeError +=1;
	}
	else
	{
		HAL_NVIC_SystemReset();//ÏµÍ³¸´Î»
	}
}

//=======================================
void Sch_InitCallback(void)
{  
		LvM_InitCbk();
		Sci_InitCbk(); 
	  //Flash_InitCbk();
}
//=========================
void Sch_TimerCallback(void)
{     
		soft_reset();   
		LvM_CountCbk();
		Sci_CountCbk();
		//Flash_CountCbk();
} 

void Sch_MainCallback(void)
{
		SysCnt.ResetFromCodeError = 0;
		LvM_MainCbk();
		Sci_MainCbk();
		//Flash_MainCbk();
}
   
 
 

#endif