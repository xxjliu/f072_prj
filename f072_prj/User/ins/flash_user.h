
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

#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址 
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define FLASH_INIT_VAL 								((uint32_t)0xFFFFFFFF)
#define FLASH_EXIST_VAL 							((uint32_t)0xAAAAAAAA)
  
typedef  void (*iapfun)(void);

extern void FLASH_PageErase(uint32_t PageAddress);
 
void FLASH_DataCacheCmd(FunctionalState NewState);


/**
 @brief: 读取指定地址
 @param: faddr:读地址 
 @return:对应数据.
**/
uint32_t STMFLASH_ReadWord(uint32_t faddr);

/**
 @brief:  从指定地址开始写入指定长度的数据
@param:  WriteAddr:起始地址(此地址必须为4的倍数!!)
		 pBuffer:数据指针
		 NumToWrite:字(32位)数(就是要写入的32位数据的个数.)
@return: NONE
**/
void STMFLASH_Write(uint32_t WriteAddr,uint32_t *pBuffer,uint32_t NumToWrite)	;

 

/***
 @brief:  从指定地址开始读出指定长度的数据
 @param:  ReadAddr:起始地址
		 pBuffer:数据指针
		 NumToRead:字(4位)数
 @return: NONE
***/
void STMFLASH_Read(uint32_t ReadAddr,uint32_t *pBuffer,uint32_t NumToRead);

/***
 @brief:   把要更新的程序放到指定FLASH中
 @param:   appxaddr:应用程序的起始地址
		   appbuf:应用程序CODE.
		   appsize:应用程序大小(字节).
 @return:  NONE
***/
void IAP_Write_Appbin(uint32_t appxaddr,uint8_t *appbuf,uint32_t appsize);
 

/***
 @brief:   接收数据错误处理
 @param:   Start_ADDR:擦除区起始地址
					 End_ADDR：结束地址
 @return:  NONE
***/
void  Data_Erase(uint32_t Start_ADDR, uint32_t End_ADDR);


uint8_t Flash_Erase_OnePage(uint32_t ErasePage_Addr);

void Flash_BMS_Data_Init(void);
uint8_t User_Data_Flash_Write(uint32_t WriteAddr,uint32_t *pBuffer,uint32_t NumToWrite);


#endif