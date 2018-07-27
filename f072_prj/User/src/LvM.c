
#include "stm32f0xx_hal.h" 
#include "../ins/LvM.h" 
 
#if 1
sLvM_TmCntType sLvM_Tm;

void LvM_InitCbk(void)
{
	uint16_t i;
	uint8_t *p_data; 
	p_data = (uint8_t *)(&sLvM_Tm); 
	for(i = 0; i < sizeof(sLvM_Tm); i++)
	{
		*(p_data++) = 0;
	}  
}
void LvM_CountCbk(void)
{
	if(sLvM_Tm.cnt < 0xFFFF)
	{
			sLvM_Tm.cnt += 1 ;
	}
	if(sLvM_Tm.led_cnt < 0xFFFF)
	{
			sLvM_Tm.led_cnt += 1 ;
	}
}
void LvM_MainCbk(void)
{  
	if(sLvM_Tm.cnt >= 10)
	{
		  sLvM_Tm.cnt = 0;
      HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
	}
	if(sLvM_Tm.led_cnt >= 10)
	{
			sLvM_Tm.led_cnt = 0 ; 
	}
}
#endif