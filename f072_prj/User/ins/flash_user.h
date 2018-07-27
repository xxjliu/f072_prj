
#include "../ins/main.h"  
#include "stm32f0xx_hal.h"
 
#if 1
  //128K
	#define FLASH_USER_PAGE_SIZE	      				0x800U      /* Size of page : 1 Kbytes */
	#define FLASH_PAGE_BASE_ADDR	      	((uint32_t)0x08000000) 		 /* Base @ of Page 0,  1 Kbytes */
	#define ADDR_FLASH_PAGE_ADDR(x)       ((uint32_t)0x08000000 + (uint32_t)(x)*FLASH_USER_PAGE_SIZE) 	
	#define ADDR_FLASH_PAGE_END_ADDR      ADDR_FLASH_PAGE_ADDR(63) 	
		
	#define ADDR_FLASH_PAGE_USER_1_ADDR      ADDR_FLASH_PAGE_ADDR(60) 		
	#define ADDR_FLASH_PAGE_USER_2_ADDR      ADDR_FLASH_PAGE_ADDR(61) 	
  
	
typedef struct
{
    uint16_t cnt; 
} sFlash_TmCntType;

extern void Flash_InitCbk(void); 
extern void Flash_CountCbk(void); 
extern void Flash_MainCbk(void); 


typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;

#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH����ʼ��ַ 
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define FLASH_INIT_VAL 								((uint32_t)0xFFFFFFFF)
#define FLASH_EXIST_VAL 							((uint32_t)0xAAAAAAAA)
  
typedef  void (*iapfun)(void);

extern void FLASH_PageErase(uint32_t PageAddress);
 
void FLASH_DataCacheCmd(FunctionalState NewState);


/**
 @brief: ��ȡָ����ַ
 @param: faddr:����ַ 
 @return:��Ӧ����.
**/
uint32_t STMFLASH_ReadWord(uint32_t faddr);

/**
 @brief:  ��ָ����ַ��ʼд��ָ�����ȵ�����
@param:  WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ4�ı���!!)
		 pBuffer:����ָ��
		 NumToWrite:��(32λ)��(����Ҫд���32λ���ݵĸ���.)
@return: NONE
**/
void STMFLASH_Write(uint32_t WriteAddr,uint32_t *pBuffer,uint32_t NumToWrite)	;

 

/***
 @brief:  ��ָ����ַ��ʼ����ָ�����ȵ�����
 @param:  ReadAddr:��ʼ��ַ
		 pBuffer:����ָ��
		 NumToRead:��(4λ)��
 @return: NONE
***/
void STMFLASH_Read(uint32_t ReadAddr,uint32_t *pBuffer,uint32_t NumToRead);

/***
 @brief:   ��Ҫ���µĳ���ŵ�ָ��FLASH��
 @param:   appxaddr:Ӧ�ó������ʼ��ַ
		   appbuf:Ӧ�ó���CODE.
		   appsize:Ӧ�ó����С(�ֽ�).
 @return:  NONE
***/
void IAP_Write_Appbin(uint32_t appxaddr,uint8_t *appbuf,uint32_t appsize);
 

/***
 @brief:   �������ݴ�����
 @param:   Start_ADDR:��������ʼ��ַ
					 End_ADDR��������ַ
 @return:  NONE
***/
void  Data_Erase(uint32_t Start_ADDR, uint32_t End_ADDR);


uint8_t Flash_Erase_OnePage(uint32_t ErasePage_Addr);

void Flash_BMS_Data_Init(void);
uint8_t User_Data_Flash_Write(uint32_t WriteAddr,uint32_t *pBuffer,uint32_t NumToWrite);


#endif