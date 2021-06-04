/**
  ******************************************************************************
  * @file    STM32F2xx_IAP/src/flash_if.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    02-May-2011
  * @brief   This file provides all the memory related operation functions.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/** @addtogroup STM32F2xx_IAP
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "flash_if.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "iap_config.h"

#ifdef IAP_PROG
//const unsigned char save_param[SAVE_PARM_ADDRESS_end + 1 - SAVE_PARM_ADDRESS] __attribute__ ((at(SAVE_PARM_ADDRESS)));
const unsigned char prog_flag[PROG_FLAG_ADDRESS_end + 1 - PROG_FLAG_ADDRESS] __attribute__ ((at(PROG_FLAG_ADDRESS))) = {0x01, 0x02, 0x03, 0x04};
#endif

void turn_off_screen_clrbuf(void);
void Delay(u32 nCount);
void feed_dog(void);


static uint32_t GetSector(uint32_t Address);

/**
  * @brief  Gets the sector of a given address
  * @param  Address: Flash address
  * @retval The sector of a given address
  */
static uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0xffff;
  
  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_Sector_0;  
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_Sector_1;  
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_Sector_2;  
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_Sector_3;  
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_Sector_4;  
  }
  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_Sector_5;  
  }
  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_Sector_6;  
  }
  else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
  {
    sector = FLASH_Sector_7;  
  }
  else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
  {
    sector = FLASH_Sector_8;  
  }
  else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
  {
    sector = FLASH_Sector_9;  
  }
  else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
  {
    sector = FLASH_Sector_10;  
  }
  else if ((Address <= USER_FLASH_END_ADDRESS) && (Address >= ADDR_FLASH_SECTOR_11))
  {
    sector = FLASH_Sector_11;  
  }
  else {
	sector = 0xffff;
  }
  return sector;
}

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Unlocks Flash for write access
  * @param  None
  * @retval None
  */
void FLASH_If_Init(void)
{ 
	feed_dog();
  FLASH_Unlock(); 

  /* Clear pending flags (if any) */  
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
}

#if 0
saveparm_t saveparm;
saveparm_t defaultparm =
{
    	{192, 168, 101, 111},     //ip address
    	{255, 255, 255, 0},  //net mask
    	{192, 168, 101, 1},       //gateway
    	{0, 0, 0, 0, 0, 2},    //mac
};

/**
  * @brief  This function does an erase of all user flash area
  * @param  StartSector: start of user flash area
  * @retval 0: user flash area successfully erased
  *         1: error occurred
  */
uint32_t FLASH_If_Erase_Save_Parm(uint32_t StartSector)
{
  uint32_t UserStartSector = FLASH_Sector_1, i = 0;

  /* Get the sector where start the user flash area */
  UserStartSector = GetSector(StartSector);

  for(i = UserStartSector; i < (UserStartSector +8); i += 8) //use sector 7 for save parm
  {
    /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
       be done by word */ 
    if (FLASH_EraseSector(i, VoltageRange_3) != FLASH_COMPLETE)
    {
      /* Error occurred while page erase */
      return (1);
    }
  }
  
  return (0);
}

uint32_t FLASH_If_Erase_Prog_Bak(uint32_t StartSector)
{
  uint32_t UserStartSector = FLASH_Sector_1, i = 0;

  /* Get the sector where start the user flash area */
  UserStartSector = GetSector(StartSector);

  for(i = UserStartSector; i < (UserStartSector +8); i += 8) //sector 6 用做程序备份 
  {
    /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
       be done by word */ 
    if (FLASH_EraseSector(i, VoltageRange_3) != FLASH_COMPLETE)
    {
      /* Error occurred while page erase */
      return (1);
    }
  }
  
  return (0);
}


/**
  * @brief  Returns the write protection status of user flash area.
  * @param  None
  * @retval 0: No write protected sectors inside the user flash area
  *         1: Some sectors inside the user flash area are write protected
  */
uint16_t FLASH_If_GetWriteProtectionStatus(void)
{
  uint32_t UserStartSector = FLASH_Sector_1;

  /* Get the sector where start the user flash area */
  UserStartSector = GetSector(SAVE_PARM_ADDRESS);

  /* Check if there are write protected sectors inside the user flash area */
  if ((FLASH_OB_GetWRP() >> (UserStartSector/8)) == (0xFFF >> (UserStartSector/8)))
  { /* No write protected sectors inside the user flash area */
    return 1;
  }
  else
  { /* Some sectors inside the user flash area are write protected */
    return 0;
  }
}

/**
  * @brief  Disables the write protection of user flash area.
  * @param  None
  * @retval 1: Write Protection successfully disabled
  *         2: Error: Flash write unprotection failed
  */
uint32_t FLASH_If_DisableWriteProtection(void)
{
  __IO uint32_t UserStartSector = FLASH_Sector_1, UserWrpSectors = OB_WRP_Sector_1;

	feed_dog();
  /* Get the sector where start the user flash area */
  UserStartSector = GetSector(SAVE_PARM_ADDRESS);

  /* Mark all sectors inside the user flash area as non protected */  
  UserWrpSectors = 0xFFF-((1 << (UserStartSector/8))-1);
   
  /* Unlock the Option Bytes */
  FLASH_OB_Unlock();

	feed_dog();
	
  /* Disable the write protection for all sectors inside the user flash area */
  FLASH_OB_WRPConfig(UserWrpSectors, DISABLE);

  /* Start the Option Bytes programming process. */  
  if (FLASH_OB_Launch() != FLASH_COMPLETE)
  {
    /* Error: Flash write unprotection failed */
    return (2);
  }

  /* Write Protection successfully disabled */
  return (1);
}




void save_init(void)
{
    uint32_t i;
    uint8_t sum = 0;
    uint8_t *p = (uint8_t *)&saveparm;
    FLASH_If_Init();
    memcpy((uint8_t*)&saveparm, (uint8_t*)SAVE_PARM_ADDRESS, sizeof(saveparm_t));
    //核对校验和
    for(i=0; i<sizeof(saveparm_t); i++)
		sum+=p[i];
   if(0x55 != sum)
   {
      load_default_parm();
   }
}

void load_default_parm(void)
{
      uint32_t i;
	uint8_t *p=(uint8_t *)&saveparm;
	uint8_t const *p_default=(uint8_t const *)&defaultparm;
	
	for(i=0;i<sizeof(saveparm_t)-1;i++)
	{
	    p[i]=p_default[i];
	}
	save_parm();
}
void save_parm(void)
{
    uint32_t i;
    uint8_t sum = 0;
    uint8_t *p=(uint8_t *)&saveparm;
    uint32_t save_addr = SAVE_PARM_ADDRESS;
    for(i=0;i<sizeof(saveparm_t)-1;i++)
	{
	    sum += p[i];
	}
    p[i] = 0x55 - sum;
    FLASH_If_Erase_Save_Parm(SAVE_PARM_ADDRESS);
    FLASH_If_Write((uint32_t *)&save_addr, (uint32_t *)&saveparm, sizeof(saveparm_t));
}
#endif

/**
  * @brief  This function writes a data buffer in flash (data are 32-bit aligned).
  * @note   After writing data buffer, the flash content is checked.
  * @param  FlashAddress: start address for writing data buffer
  * @param  Data: pointer on data buffer
  * @param  DataLength: length of data buffer (unit is 32-bit word)   
  * @retval 0: Data successfully written to Flash memory
  *         1: Error occurred while writing data in Flash memory
  *         2: Written Data in flash memory is different from expected one
  */
uint32_t FLASH_If_Write(__IO uint32_t* FlashAddress, uint32_t* Data ,uint16_t DataLength)
{
  uint32_t i = 0;

  for (i = 0; (i < DataLength) && (*FlashAddress <= (USER_FLASH_END_ADDRESS-3)); i++)
  {
	  feed_dog();
    /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
       be done by word */ 
    if (FLASH_ProgramWord(*FlashAddress, *(uint32_t*)(Data+i)) == FLASH_COMPLETE)
    {
    	feed_dog();
		
     /* Check the written value */
      if (*(uint32_t*)*FlashAddress != *(uint32_t*)(Data+i))
      {
        /* Flash content doesn't match SRAM content */
        return(2);
      }
      /* Increment FLASH destination address */
      *FlashAddress += 4;
    }
    else
    {
    	feed_dog();
      /* Error occurred while writing data in Flash memory */
      return (1);
    }
  }

	feed_dog();
  return (0);
}


/**
	根据指定地址擦除对应的扇区
*/
FLASH_if_Status FLASH_If_AddrSectorErease(uint32_t addr)
{
	uint32_t UserStartSector;

	UserStartSector = GetSector(addr);

	feed_dog();
	
	if (!IS_FLASH_SECTOR((uint16_t)UserStartSector)) {
		return FLASH_if_err_sectorNum;
	}
	
#if 0
	if (FLASH_EraseSector(UserStartSector, VoltageRange_3) != FLASH_COMPLETE)
		return FLASH_if_err_NOCOMPLETE;
	else
		return FLASH_if_COMPLETE;
#else
	while (FLASH_EraseSector(UserStartSector, VoltageRange_3) != FLASH_COMPLETE);

	feed_dog();

	return FLASH_if_COMPLETE;
#endif
}
/**
	从指定地址开始按照字写入方式连续写入多字
	FlashAddress: 起始地址
	Data: 源数据地址
	DataLength: 源数据的字节长度
*/
FLASH_if_Status FLASH_If_write_flash(uint32_t FlashAddress, uint32_t *Data, uint32_t DataLength)
{
	uint32_t i = 0;
	uint32_t wordLength;
	uint32_t addr = FlashAddress;

	wordLength = DataLength/4;
	if (DataLength%4)
		wordLength++;
	if (wordLength == 0)
		return FLASH_if_COMPLETE;

	if (addr >= USER_FLASH_END_ADDRESS)
		return FLASH_if_err_address;
	if (addr+3 >  USER_FLASH_END_ADDRESS)
		return FLASH_if_err_address;
	
	if (wordLength > (USER_FLASH_END_ADDRESS-addr+1)/4)
		return FLASH_if_err_noSpace;

	feed_dog();
	
	for (i = 0; i < wordLength; i++) {
		/* Device voltage range supposed to be [2.7V to 3.6V], the operation will
		       be done by word */ 
#if 0
		 if (FLASH_ProgramWord(addr, *(uint32_t*)(Data+i)) == FLASH_COMPLETE) {
			 /* Check the written value */
			 if (*((uint32_t*)addr) != *((uint32_t*)(Data+i)))
			 	return FLASH_if_err_wrCHECK;
			 addr += 4;
		 }
		 else {
			return FLASH_if_err_NOCOMPLETE;
		 }
#else
		while (FLASH_ProgramWord(addr, *(uint32_t*)(Data+i)) != FLASH_COMPLETE);

		feed_dog();
		
		while (*((uint32_t*)addr) != *((uint32_t*)(Data+i))) {
			while (FLASH_ProgramWord(addr, *(uint32_t*)(Data+i)) != FLASH_COMPLETE)
				;
			feed_dog();
		}
		addr += 4;

		feed_dog();
#endif
	}
	return FLASH_if_COMPLETE;
}


FLASH_if_Status FLASH_If_set_flash(uint32_t FlashAddress, uint32_t Data, uint32_t DataLength)
{
	uint32_t i = 0;
	uint32_t wordLength;
	uint32_t addr = FlashAddress;

	wordLength = DataLength/4;
	if (DataLength%4)
		wordLength++;
	if (wordLength == 0)
		return FLASH_if_COMPLETE;

	if (addr >= USER_FLASH_END_ADDRESS)
		return FLASH_if_err_address;
	if (addr+3 >  USER_FLASH_END_ADDRESS)
		return FLASH_if_err_address;
	
	if (wordLength > (USER_FLASH_END_ADDRESS-addr+1)/4)
		return FLASH_if_err_noSpace;

	feed_dog();

	for (i = 0; i < wordLength; i++) {
		/* Device voltage range supposed to be [2.7V to 3.6V], the operation will
		       be done by word */ 
#if 0
		 if (FLASH_ProgramWord(addr, Data) == FLASH_COMPLETE) {
			 /* Check the written value */
			 if (*((uint32_t*)addr) != Data)
			 	return FLASH_if_err_wrCHECK;
			addr += 4;
		 }
		 else {
			return FLASH_if_err_NOCOMPLETE;
		 }
#else
		while (FLASH_ProgramWord(addr, Data) != FLASH_COMPLETE);

		feed_dog();
		
		while (*((uint32_t*)addr) != Data) {
			while (FLASH_ProgramWord(addr, Data) != FLASH_COMPLETE)
				;
			feed_dog();
		}
		addr += 4;

		feed_dog();
#endif
	}
	return FLASH_if_COMPLETE;
}

/**
  * 擦除用户程序区
  */
FLASH_if_Status FLASH_If_Erase_Application_sectors(void)
{
	uint32_t UserStartSector, i;

	/* Get the sector where start the user flash area */
	UserStartSector = GetSector(ApplicationAddress);

	feed_dog();
	if (!IS_FLASH_SECTOR((uint16_t)UserStartSector)) {
		return FLASH_if_err_sectorNum;
	}

	feed_dog();

	for(i = 0; i < Application_SECTOR_resSUM; i++) {
	/* Device voltage range supposed to be [2.7V to 3.6V], the operation will
		be done by word */ 
#if 0
		if (FLASH_EraseSector(UserStartSector + i*8, VoltageRange_3) != FLASH_COMPLETE)
			return FLASH_if_err_NOCOMPLETE;
#else
		while (FLASH_EraseSector(UserStartSector + i*8, VoltageRange_3) != FLASH_COMPLETE);
#endif
		feed_dog();
	}
	return FLASH_if_COMPLETE;
}

typedef struct {
	uint32_t flag;
}TAG_PROG_FLAG;

//TAG_PROG_FLAG prog_flag;
#define PROG_OK		0x12345678
#define PROG_fail		0xffffffff
uint8_t check_prog_flag(void)
{

#if 0
	uint32_t tmp_flag;

	feed_dog();
	FLASH_If_Init();
	memcpy((uint8_t*)&tmp_flag, (uint8_t*)PROG_FLAG_ADDRESS, sizeof(tmp_flag));
	FLASH_Lock();
	feed_dog();
	if (tmp_flag == PROG_OK) return 1;
	else return 0;
#endif

	uint32_t tmp_flag;

	feed_dog();

	memcpy((uint8_t*)&tmp_flag, (uint8_t*)PROG_FLAG_ADDRESS, sizeof(tmp_flag));
	
	if (tmp_flag == PROG_OK)
	{
		return 1;
	}
	else
	{
		return 0;
	}

}

void clr_prog_flag(void)
{
	uint32_t tmp_flag = PROG_fail;
	//uint32_t save_addr = PROG_FLAG_ADDRESS;
	FLASH_if_Status ret = FLASH_if_err_NOCOMPLETE;

	feed_dog();
	FLASH_If_Init();
	FLASH_If_AddrSectorErease(PROG_FLAG_ADDRESS);
	feed_dog();
	while (ret != FLASH_if_COMPLETE)
		//ret = FLASH_If_Write((uint32_t *)&save_addr, (uint32_t *)&tmp_flag, sizeof(tmp_flag));
		ret = FLASH_If_write_flash(PROG_FLAG_ADDRESS, (uint32_t *)&tmp_flag, sizeof(tmp_flag));
	FLASH_Lock();
	feed_dog();
}

void set_prog_flag(void)
{
	uint32_t tmp_flag = PROG_OK;
	//uint32_t save_addr = PROG_FLAG_ADDRESS;
	FLASH_if_Status ret = FLASH_if_err_NOCOMPLETE;

	feed_dog();
	
	FLASH_If_Init();
	FLASH_If_AddrSectorErease(PROG_FLAG_ADDRESS);
	feed_dog();
	while (ret != FLASH_if_COMPLETE)
		//ret = FLASH_If_Write((uint32_t *)&save_addr, (uint32_t *)&tmp_flag, sizeof(tmp_flag));
		ret = FLASH_If_write_flash(PROG_FLAG_ADDRESS, (uint32_t *)&tmp_flag, sizeof(tmp_flag));
	FLASH_Lock();
	feed_dog();
}



void app_to_iap_init(void)
{
	turn_off_screen_clrbuf();
}

void iap_to_app_init(void)
{
	turn_off_screen_clrbuf();
}

typedef  void (*pFunction)(void);

pFunction Jump_To_Application;
pFunction Jump_To_iapPROG;
uint32_t JumpAddress;

void run_app(void)
{ 
#ifdef IAP_PROG
	iap_to_app_init();
	
	Delay(9000);
	set_prog_flag();
	Delay(9000);

	feed_dog();

	SystemInit();
	Delay(9000);
	
      /* Jump to user application */
      JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
      Jump_To_Application = (pFunction) JumpAddress;
      /* Initialize user application's Stack Pointer */
      __set_MSP(*(__IO uint32_t*) ApplicationAddress);
      Jump_To_Application();
	while(1);
#endif
}

void run_iap_prog(void)
{ 
#ifndef IAP_PROG
	app_to_iap_init();

	Delay(9000);
	clr_prog_flag();
	Delay(9000);

	feed_dog();

	SystemInit();
	Delay(9000);
	
	//set_prog_flag();
      /* Jump to user application */
      JumpAddress = *(__IO uint32_t*) (PROG_IAP_ADDRESS + 4);
      Jump_To_iapPROG = (pFunction) JumpAddress;
      /* Initialize user application's Stack Pointer */
      __set_MSP(*(__IO uint32_t*) PROG_IAP_ADDRESS);
      Jump_To_iapPROG();
	while(1);
#endif
}


void flash_pro_test(void)
{
#if 0	
	FLASH_if_Status ret;
	FLASH_If_Init();
	ret = FLASH_If_AddrSectorErease(0x07ffffff);
	ret = FLASH_If_AddrSectorErease(0x08100000);
	ret = FLASH_If_AddrSectorErease(ADDR_FLASH_SECTOR_6);
	ret = FLASH_If_set_flash(ADDR_FLASH_SECTOR_6, 0x12345678, 3);
	ret = FLASH_If_AddrSectorErease(ADDR_FLASH_SECTOR_6);
	
	ret = FLASH_If_AddrSectorErease(ADDR_FLASH_SECTOR_7);
	ret = FLASH_If_set_flash(ADDR_FLASH_SECTOR_7, 0x12345678, 2*4);
	ret = FLASH_If_AddrSectorErease(ADDR_FLASH_SECTOR_7);

	ret = FLASH_If_AddrSectorErease(ADDR_FLASH_SECTOR_8);
	ret = FLASH_If_set_flash(ADDR_FLASH_SECTOR_8, 0x12345678, 2*4);
	ret = FLASH_If_AddrSectorErease(ADDR_FLASH_SECTOR_8);
	
	//ret = FLASH_If_AddrSectorErease(ADDR_FLASH_SECTOR_9);
	//ret = FLASH_If_set_flash(ADDR_FLASH_SECTOR_9+0x1ffff, 0x12345678, 2*4);
	
	ret = FLASH_If_AddrSectorErease(ADDR_FLASH_SECTOR_10);
	
	ret = FLASH_If_AddrSectorErease(ADDR_FLASH_SECTOR_11);
	ret = FLASH_If_set_flash(ADDR_FLASH_SECTOR_11, 0x12345678, USER_FLASH_END_ADDRESS - ADDR_FLASH_SECTOR_11+1);
	ret = FLASH_If_AddrSectorErease(ADDR_FLASH_SECTOR_11);

	ret = FLASH_If_set_flash(ADDR_FLASH_SECTOR_11, 0x12345678, 1);
	ret = FLASH_If_AddrSectorErease(ADDR_FLASH_SECTOR_11);

	ret = FLASH_If_set_flash(ADDR_FLASH_SECTOR_11+0x1ffff - 3, 0x12345678, 1);
	ret = FLASH_If_AddrSectorErease(ADDR_FLASH_SECTOR_11);

	ret = FLASH_If_set_flash(ADDR_FLASH_SECTOR_11+0x1ffff - 3, 0x12345678, 2);
	ret = FLASH_If_AddrSectorErease(ADDR_FLASH_SECTOR_11);

	ret = FLASH_If_set_flash(ADDR_FLASH_SECTOR_11+1, 0x12345678, 1);
	ret = FLASH_If_AddrSectorErease(ADDR_FLASH_SECTOR_11);
	
	ret = FLASH_If_AddrSectorErease(ADDR_FLASH_SECTOR_1);
	ret = FLASH_If_AddrSectorErease(ADDR_FLASH_SECTOR_1);
	ret = FLASH_If_AddrSectorErease(ADDR_FLASH_SECTOR_6 + 0x1ffff);
	ret = FLASH_If_AddrSectorErease(ADDR_FLASH_SECTOR_6 + 0x1ffff +1);
#endif
}


void Check_And_RunAPP(void)
{
	if (check_prog_flag()) {
		//if (((*(__IO uint32_t*)ApplicationAddress) & 0x2FFE0000 ) == 0x20000000)
		/* Jump to user application */
		JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
		Jump_To_Application = (pFunction) JumpAddress;
		/* Initialize user application's Stack Pointer */
		__set_MSP(*(__IO uint32_t*) ApplicationAddress);
		Jump_To_Application();

		while(1);
     }
}

void App_SetVectorTable(void)
{
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, (ApplicationAddress - ADDR_FLASH_SECTOR_0));
}


/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
