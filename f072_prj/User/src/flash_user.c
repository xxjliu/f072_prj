
#include "stm32f0xx_hal.h"  
#include "stm32f0xx_hal_flash.h"  
#include "../ins/main.h"  
#include "../ins/flash_user.h"  


#define FLASH_ACR_DCEN                       ((uint32_t)0x00000400)

iapfun jump2app; 
uint32_t iapbuf[512]={0}; 	//2K字节缓存  
static uint32_t ARR_ADDR_FLASH_PAGE[64] ;
//uint32_t Data_1K_Tmp[256];
  
 
#if 1
sFlash_TmCntType sFlash_Tm; 
#define WR_BUF_LEN 	8
#define RD_BUF_LEN 	8
uint64_t wr_buf[WR_BUF_LEN] = {0};
uint64_t rd_buf[RD_BUF_LEN] = {0};
 
#define FLASH_ACR_DCEN                       ((uint32_t)0x00000400) 
void Flash_InitCbk(void)
{
	uint16_t i;
	uint8_t *p_data;  
	p_data = (uint8_t *)(&sFlash_Tm); 
	for(i = 0; i < sizeof(sFlash_Tm); i++)
	{
		*(p_data++) = 0;
	}    
	  
	for(i = 0; i < WR_BUF_LEN; i++)
	{
		wr_buf[i] = 0x1122123412341230 + i; 
	} 
	//Flash_Erase_OnePage(ADDR_FLASH_PAGE_USER_1_ADDR);	
	
	Flash_BMS_Data_Init();
}
void Flash_CountCbk(void)
{
	if(sFlash_Tm.cnt < 0xFFFF)
	{
			sFlash_Tm.cnt += 1 ;
	} 
} 
uint32_t Flash_ReadWord(uint32_t faddr)
{
	return *(vu32*)faddr; 
} 
uint16_t flash_local_index = 0;
void Flash_MainCbk(void)
{   
				//sFlash_Tm.cnt = 0;  
	  if((sFlash_Tm.cnt >= 20) && (sFlash_Tm.cnt <= 120)) 
		{
				sFlash_Tm.cnt = 130;  
			
        STMFLASH_Read(ADDR_FLASH_PAGE_USER_1_ADDR,(uint32_t*)(&rd_buf[0]),4);   
	/*
			//HAL_StatusTypeDef HAL_FLASH_Program(uint32_t TypeProgram, uint32_t Address, uint64_t Data)
			//HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,ADDR_FLASH_PAGE_USER_1_ADDR, uint64_t Data)
			wr_buf[flash_local_index] += 10*2;
			//HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,ADDR_FLASH_PAGE_USER_1_ADDR + 2*flash_local_index, wr_buf[flash_local_index]);
			//rd_buf[flash_local_index] = Flash_ReadWord(ADDR_FLASH_PAGE_USER_1_ADDR + 2*flash_local_index);
			flash_local_index += 1;
			if(flash_local_index >= WR_BUF_LEN)
			{
				flash_local_index = 0;
	      //Flash_Erase_OnePage(ADDR_FLASH_PAGE_USER_1_ADDR);	
			}
*/
		}
} 

uint8_t Flash_Erase_OnePage(uint32_t ErasePage_Addr)
{
	uint32_t PageError =0; 
	
	HAL_FLASH_Unlock();																		//解锁 
	FLASH_DataCacheCmd(DISABLE);													//FLASH擦除期间,必须禁止数据缓存
	
	FLASH_EraseInitTypeDef EraseInitStruct_Tmp;
	EraseInitStruct_Tmp.TypeErase = 0;//TYPEERASE_PAGES;
	EraseInitStruct_Tmp.PageAddress = ErasePage_Addr;
	EraseInitStruct_Tmp.NbPages = 1;  
	HAL_FLASHEx_Erase(&EraseInitStruct_Tmp, &PageError);
	
	FLASH_DataCacheCmd(ENABLE);														//FLASH擦除结束,开启数据缓存
	HAL_FLASH_Lock();																			//上锁  
	return (uint8_t)PageError;
	
}
void Flash_BMS_Data_Init(void)
{
	uint16_t j,i =0;
	uint32_t Data_1K_Tmp[256];
	for(i =0;i<256;i++)
	{
		Data_1K_Tmp[i] =(uint32_t)i;
	}
	
	Data_1K_Tmp[0] =0xAAAAAAAA;
	 					
	HAL_FLASH_Unlock();																		//解锁 
  FLASH_DataCacheCmd(DISABLE);													//FLASH擦除期间,必须禁止数据缓存
 		    
	
	if(STMFLASH_ReadWord(ADDR_FLASH_PAGE_USER_1_ADDR)!=	FLASH_INIT_VAL)//有非0XFFFFFFFF的地方,要擦除这个扇区
	{   
			Flash_Erase_OnePage(ADDR_FLASH_PAGE_USER_1_ADDR);
	}
	   
	j =0;
	while(j<256)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,ADDR_FLASH_PAGE_USER_1_ADDR+j*4,Data_1K_Tmp[j]);//以字为单位写入数据	
 		j+=1; 
	} 
	
  FLASH_DataCacheCmd(ENABLE);	//FLASH擦除结束,开启数据缓存
	HAL_FLASH_Lock();//上锁 
}
/**
  * @brief  Enables or disables the Data Cache feature.
  * @param  NewState: new state of the Data Cache.
  *          This parameter  can be: ENABLE or DISABLE.
  * @retval None
  */
void FLASH_DataCacheCmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if(NewState != DISABLE)
  {
    FLASH->ACR |= FLASH_ACR_DCEN;
  }
  else
  {
    FLASH->ACR &= (~FLASH_ACR_DCEN);
  }
}


/**
 @brief: 读取指定地址
 @param: faddr:读地址 
 @return:对应数据.
**/
uint32_t STMFLASH_ReadWord(uint32_t faddr)
{
	return *(vu32*)faddr; 
} 


/**
 @brief:  从指定地址开始写入指定长度的数据
@param:  WriteAddr:起始地址(此地址必须为4的倍数!!)
		 pBuffer:数据指针
		 NumToWrite:字(32位)数(就是要写入的32位数据的个数.)
@return: NONE
**/
void STMFLASH_Write(uint32_t WriteAddr,uint32_t *pBuffer,uint32_t NumToWrite)	
{  
	uint32_t addrx=0;
	uint32_t endaddr=0;	
	uint32_t  PageError = 0;
	FLASH_EraseInitTypeDef EraseInitStruct;
  if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4)					//非法地址
		return;																	
	HAL_FLASH_Unlock();																	//解锁 
  FLASH_DataCacheCmd(DISABLE);												//FLASH擦除期间,必须禁止数据缓存
 		
	addrx=WriteAddr;																		//写入的起始地址
	endaddr=WriteAddr+NumToWrite*4;											//写入的结束地址
	if(addrx<0x08020000)																//只有主存储区,才需要执行擦除操作!!  128K
	{
		while(addrx<endaddr)
		{
			if(STMFLASH_ReadWord(addrx)!=0xFFFFFFFF)//有非0XFFFFFFFF的地方,要擦除这个扇区
			{   
				//FLASH_PageErase(addrx);  //擦除
				;//copy this page
				EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
				EraseInitStruct.PageAddress = addrx;
				EraseInitStruct.NbPages = 1; //擦一页
				HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);
				//break;
			}
			else 
			{
				addrx+=4;
			}
		}
	}
	
	while(WriteAddr<endaddr)//写数据
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,WriteAddr,*pBuffer);//以字为单位写入数据	
		WriteAddr+=4;
		pBuffer++;
	} 
	
  FLASH_DataCacheCmd(ENABLE);	//FLASH擦除结束,开启数据缓存
	HAL_FLASH_Lock();//上锁
}


/**
 @brief:  从指定地址开始写入指定长度的数据
@param:  WriteAddr:起始地址(此地址必须为4的倍数!!)
				 pBuffer:数据指针
				 NumToWrite:字(32位)数(就是要写入的32位数据的个数.)
@return: NONE
**/
uint8_t User_Data_Flash_Write(uint32_t WriteAddr,uint32_t *pBuffer,uint32_t NumToWrite)	
{  
	uint32_t  i,j = 0;
	uint32_t 	endaddr=0;	
	uint32_t  PageError = 0;
	uint32_t 	Page_Begin_Addr=0;
	uint8_t   WriteAddr_Page_Index=0;
	uint32_t Data_1K_Tmp[256];
	Data_1K_Tmp[0] =0;
	//FLASH_EraseInitTypeDef EraseInitStruct_Tmp;
	
  if(WriteAddr<ADDR_FLASH_PAGE_USER_1_ADDR || WriteAddr >= ADDR_FLASH_PAGE_USER_2_ADDR ||WriteAddr%4)					//非法地址
		return 1;																	
	HAL_FLASH_Unlock();																		//解锁 
  FLASH_DataCacheCmd(DISABLE);													//FLASH擦除期间,必须禁止数据缓存
 		  
	;//	find out the block num with the addrx
	;//	copy this block data to the ram (Data_1K_Tmp[1024])
	;//	change the Data_1K_Tmp[1024] data
	;//	check the first word data of this block, if not equel to 0xAAAA AAAA erase the block data
	;// write the Data_1K_Tmp[1024] data to this block
	WriteAddr_Page_Index = (WriteAddr -FLASH_PAGE_BASE_ADDR)/FLASH_PAGE_SIZE;
	Page_Begin_Addr =FLASH_PAGE_BASE_ADDR +WriteAddr_Page_Index *FLASH_PAGE_SIZE;
	STMFLASH_Read(Page_Begin_Addr,Data_1K_Tmp,1024);
	for(i =WriteAddr;i<WriteAddr+NumToWrite*4;i++)
	{
		Data_1K_Tmp[i] =*(pBuffer++);
	}
	
	if(STMFLASH_ReadWord(Page_Begin_Addr)!=	FLASH_INIT_VAL)//有非0XFFFFFFFF的地方,要擦除这个扇区
	{   
			Flash_Erase_OnePage(Page_Begin_Addr);
	}
	   
	j =0;
	while(j<1024)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,Page_Begin_Addr+j,Data_1K_Tmp[j]);//以字为单位写入数据	
		j+=4; 
	} 
	
  FLASH_DataCacheCmd(ENABLE);	//FLASH擦除结束,开启数据缓存
	HAL_FLASH_Lock();//上锁
	return 0;
}


/***
 @brief:  从指定地址开始读出指定长度的数据
 @param:  ReadAddr:起始地址
		 pBuffer:数据指针
		 NumToRead:字(4位)数
 @return: NONE
***/
void STMFLASH_Read(uint32_t ReadAddr,uint32_t *pBuffer,uint32_t NumToRead)   	
{
	uint32_t i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadWord(ReadAddr);//读取4个字节.
		ReadAddr+=4;//偏移4个字节.	
	}
}

/***
 @brief:   把要更新的程序放到指定FLASH中
 @param:   appxaddr:应用程序的起始地址
		   appbuf:应用程序CODE.
		   appsize:应用程序大小(字节).
 @return:  NONE
***/
void IAP_Write_Appbin(uint32_t appxaddr,uint8_t *appbuf,uint32_t appsize)
{
	uint32_t t;
	uint16_t i=0;
	uint32_t temp;
	uint32_t fwaddr=appxaddr;//当前写入的地址
	uint8_t *dfu=appbuf;
	for(t=0;t<appsize;t+=4)
	{						   
		temp=(uint32_t)dfu[3]<<24;   
		temp|=(uint32_t)dfu[2]<<16;    
		temp|=(uint32_t)dfu[1]<<8;
		temp|=(uint32_t)dfu[0];	    //将8位数据转为32位数据
		dfu+=4;//偏移4个字节
		iapbuf[i++]=temp;	    
		if(i==512)
		{
			i=0; 
			STMFLASH_Write(fwaddr,iapbuf,512);
			fwaddr+=2048;//偏移2048  512*4=2048
		}
	} 
	if(i) STMFLASH_Write(fwaddr,iapbuf,i);//将最后的一些内容字节写进去.  
}
 

/***
 @brief:   接收数据错误处理
@param:   Start_ADDR:擦除区起始地址
					End_ADDR：结束地址
 @return:  NONE
***/
void  Data_Erase(uint32_t Start_ADDR, uint32_t End_ADDR)
{
	uint32_t  PageError = 0;
	FLASH_EraseInitTypeDef EraseInitStruct;
	
	HAL_FLASH_Unlock();
	FLASH_DataCacheCmd(DISABLE);//FLASH擦除期间,必须禁止数据缓存
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.PageAddress = Start_ADDR;
	EraseInitStruct.NbPages = (End_ADDR - Start_ADDR)/FLASH_PAGE_SIZE;//擦除10个page;
	HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);
	FLASH_DataCacheCmd(ENABLE);	//FLASH擦除结束,开启数据缓存
	HAL_FLASH_Lock();
}

		 



#endif