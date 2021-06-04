/**
  ******************************************************************************
  * @file    STM32F2xx_IAP/inc/flash_if.h 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    02-May-2011
  * @brief   This file provides all the headers of the flash_if functions.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FLASH_IF_H
#define __FLASH_IF_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Base address of the Flash sectors */

//#define __STM32F207ZG__ 1
#define __STM32F207ZC__ 1

#if defined (__STM32F207ZC__)
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbyte */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbyte */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbyte */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbyte */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbyte */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbyte */
// stm32f207zc flash is 256K
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 0 Kbyte */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08040000) /* Base @ of Sector 7, 0 Kbyte */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08040000) /* Base @ of Sector 8, 0 Kbyte */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x08040000) /* Base @ of Sector 9, 0 Kbyte */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x08040000) /* Base @ of Sector 10, 0 Kbyte */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x08040000) /* Base @ of Sector 11, 0 Kbyte */
/* End of the Flash address */
#define USER_FLASH_END_ADDRESS        0x0803FFFF

/*
            ***********************0x8000000
            *	IAP					*  sector 0 -- sector 4
            *		*****0x08004000
            *		app flag		*sector 1
            *		*****0x08008000
            *		save_param		*sector 2	//根据实际情况选择使用
            *		*****0x0800C000
            *
            ***********************0x8020000
            *      APP					*   sector 5
            *						*
            ***********************0x8040000
*/
#define PROG_BAK_ADDRESS		ADDR_FLASH_SECTOR_5	//APP程序起始地址
#define ApplicationAddress			ADDR_FLASH_SECTOR_5
#define Application_SECTOR_resSUM   (1)

#define PROG_IAP_ADDRESS		ADDR_FLASH_SECTOR_0		//IAP程序开始地址
#define PROG_IAP_ADDRESS_end	(ADDR_FLASH_SECTOR_5 - 1)
#define PROG_FLAG_ADDRESS		ADDR_FLASH_SECTOR_1		//APP 标志起始地址
#define PROG_FLAG_ADDRESS_end	(ADDR_FLASH_SECTOR_2 - 1)
#define SAVE_PARM_ADDRESS		ADDR_FLASH_SECTOR_2		//参数保存起始地址
#define SAVE_PARM_ADDRESS_end	(ADDR_FLASH_SECTOR_3 - 1)	//参数保存起始地址

#elif defined (__STM32F207ZG__)
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbyte */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbyte */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbyte */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbyte */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbyte */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbyte */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbyte */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbyte */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbyte */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbyte */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbyte */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbyte */
/* End of the Flash address */
#define USER_FLASH_END_ADDRESS        0x080FFFFF
/* Define the user application size */
//#define USER_FLASH_SIZE   (USER_FLASH_END_ADDRESS - SAVE_PARM_ADDRESS + 1)

/* Define the address from where user application will be loaded.
   Note: the 1st sector 0x08000000-0x08003FFF is reserved for the IAP code */

/*
            ***********************0x8000000
            *      IAP					*  sector 0 -- sector 4
            *						*
            ***********************0x8020000
            *      APP flag				*   sector 5
            *						*
            ***********************0x8040000
            *   	res					*  sector 6
            *						*
            ***********************0x8060000
            *      APP					*  sector 7 -- 11
            *						*
            ***********************
*/
#define PROG_BAK_ADDRESS	ADDR_FLASH_SECTOR_7	//升级时程序备份区
#define ApplicationAddress		ADDR_FLASH_SECTOR_7
#define Application_SECTOR_resSUM   (11 - 7 + 1)

#define PROG_IAP_ADDRESS	ADDR_FLASH_SECTOR_0    //IAP程序开始地址
#define PROG_FLAG_ADDRESS	ADDR_FLASH_SECTOR_5
#define PROG_FLAG_ADDRESS_end ADDR_FLASH_SECTOR_6
#define SAVE_PARM_ADDRESS	ADDR_FLASH_SECTOR_6   //参数保存起始地址
#define SAVE_PARM_ADDRESS_end	ADDR_FLASH_SECTOR_7    //参数保存起始地址

#endif

typedef enum
{ 
	FLASH_if_COMPLETE = 0,
	FLASH_if_err_NOCOMPLETE,
	FLASH_if_err_sectorNum,
	FLASH_if_err_address,
	FLASH_if_err_noSpace,
	FLASH_if_err_wrCHECK
}FLASH_if_Status;

typedef struct
{
    uint8_t ipaddr[4];
    uint8_t net_mask[4];
    uint8_t gw_addr[4];
    uint8_t mac[6];
    uint8_t reverse[64];
    uint8_t crc;
}saveparm_t;
extern saveparm_t saveparm;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void FLASH_If_Init(void);
//uint32_t FLASH_If_Erase_Save_Parm(uint32_t StartSector);
//uint32_t FLASH_If_Erase_Prog_Bak(uint32_t StartSector);
uint32_t FLASH_If_Write(__IO uint32_t* FlashAddress, uint32_t* Data, uint16_t DataLength);
//uint16_t FLASH_If_GetWriteProtectionStatus(void);
//uint32_t FLASH_If_DisableWriteProtection(void);
//void save_parm(void);
//void save_init(void);
//void load_default_parm(void);
uint8_t check_prog_flag(void);
void clr_prog_flag(void);
void set_prog_flag(void);
FLASH_if_Status FLASH_If_Erase_Application_sectors(void);
FLASH_if_Status FLASH_If_AddrSectorErease(uint32_t addr);
FLASH_if_Status FLASH_If_write_flash(uint32_t FlashAddress, uint32_t *Data, uint32_t DataLength);
FLASH_if_Status FLASH_If_set_flash(uint32_t FlashAddress, uint32_t Data, uint32_t DataLength);
	
#endif  /* __FLASH_IF_H */

/*******************(C)COPYRIGHT 2011 STMicroelectronics *****END OF FILE******/
