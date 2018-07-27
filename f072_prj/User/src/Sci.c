
#include "stm32f0xx_hal.h" 
#include "../ins/Sci.h" 
#include "../Inc/Usart.h" 
 
#if 1
sSci_TmCntType sSci_Tm;
sSci_SendDataType sSci_TxData;


void Sci_SendByteCbk(void);

void Sci_InitCbk(void)
{
	uint16_t i;
	uint8_t *p_data; 
	p_data = (uint8_t *)(&sSci_Tm); 
	for(i = 0; i < sizeof(sSci_Tm); i++)
	{
		*(p_data++) = 0;
	}   
	p_data = (uint8_t *)(&sSci_TxData);
	for(i = 0; i < sizeof(sSci_TxData); i++)
	{
		*(p_data++) = 0;
	}   
	for(i = 0; i < SCI_SEND_DATA_LEN; i++)
	{
		sSci_TxData.data[i] = i;
	}   
}
void Sci_CountCbk(void)
{
	if(sSci_Tm.cnt < 0xFFFF)
	{
			sSci_Tm.cnt += 1 ;
	} 
}
uint16_t sci_tx_fail_cnt = 0;
void Sci_MainCbk(void)
{  
		uint8_t i = 0;
	  if(sSci_Tm.cnt >= 20) 
		{
				sSci_Tm.cnt = 0;
			  sSci_TxData.data[0] += 1;
				for(i=0;i<10;i++)
				{
					if(HAL_ERROR != HAL_UART_Transmit_IT(&huart1, &sSci_TxData.data[0], 50))
					{
							sci_tx_fail_cnt = 0;
							break;
					}
				}
				if(i == 10)
				{
						sci_tx_fail_cnt += 1;
				}
		}
}
/*
void Sci_SendByteCbk(void)
{ 
    //(void)HAL_UART_Transmit_IT(&huart1, &sSci_TxData.data[0], 50);
		if(sSci_TxData.cnt < sSci_TxData.len)
		{ 
				__HAL_UART_ENABLE_IT(&huart1, UART_IT_TXE);
			  huart1.Instance->DR = sSci_TxData.data[sSci_TxData.cnt++]; 
		}
		else
		{
				sSci_TxData.cnt	= 0;
				sSci_TxData.len = 0;
		}
}
*/
#endif