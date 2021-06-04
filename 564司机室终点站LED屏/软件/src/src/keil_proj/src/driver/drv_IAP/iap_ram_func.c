/******************************************************************************/
/* RamFunc.c: External RAM_Function: RO,RW & ZI sections                      */
/******************************************************************************/
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2005-2006 Keil Software. All rights reserved.                */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/


#include "stm32f2xx.h"
#include "iap_ram_func.h"




/*  Assignment of Code Memory for Module to RAM (under Options for file 'RamFunc.c') */
/*   RO - Code/Const:       RAM1 (0x80010000-0x8001FFFF)                             */




#define SECTOR_MASK               ((uint32_t)0xFFFFFF07)

#if 0
/* Base address of the Flash sectors */
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbytes */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbytes */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbytes */

#define DATA_32                 ((uint32_t)0x12345678)
#endif

unsigned int rcv_len = 0;

#define KR_KEY_RELOAD    ((uint16_t)0xAAAA)
#define KR_KEY_ENABLE    ((uint16_t)0xCCCC)


void ram_feeddog(void)
{
	static int bit_val = 0;

	if (!bit_val)
	{
		GPIO_PORT_WDT_IN->BSRRL = GPIO_Pin_WDT_IN;
		bit_val = 1;
	}
	else
	{
		GPIO_PORT_WDT_IN->BSRRH = GPIO_Pin_WDT_IN ;
		bit_val = 0;
	}

	IWDG->KR = KR_KEY_RELOAD;

}

FLASH_Status ram_FLASH_GetStatus(void)
{
	FLASH_Status flashstatus = FLASH_COMPLETE;

	if ((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY)
	{
		flashstatus = FLASH_BUSY;
	}
	else
	{
		if ((FLASH->SR & FLASH_FLAG_WRPERR) != (uint32_t)0x00)
		{
			flashstatus = FLASH_ERROR_WRP;
		}
		else
		{
			if ((FLASH->SR & (uint32_t)0xEF) != (uint32_t)0x00)
			{
				flashstatus = FLASH_ERROR_PROGRAM;
			}
			else
			{
				if ((FLASH->SR & FLASH_FLAG_OPERR) != (uint32_t)0x00)
				{
					flashstatus = FLASH_ERROR_OPERATION;
				}
				else
				{
					flashstatus = FLASH_COMPLETE;
				}
			}
		}
	}

	/* Return the FLASH Status */
	return flashstatus;
}

#define VECT_TAB_OFFSET  0x00 /*!< Vector Table base offset field. 
                                   This value must be a multiple of 0x200. */


/* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N */
#define PLL_M      25
#define PLL_N      240

/* SYSCLK = PLL_VCO / PLL_P */
#define PLL_P      2

/* USB OTG FS, SDIO and RNG Clock =  PLL_VCO / PLLQ */
#define PLL_Q      5

void ram_system_init()
{
	/* Reset the RCC clock configuration to the default reset state ------------*/
	/* Set HSION bit */
	RCC->CR |= (uint32_t)0x00000001;

	/* Reset CFGR register */
	RCC->CFGR = 0x00000000;

	/* Reset HSEON, CSSON and PLLON bits */
	RCC->CR &= (uint32_t)0xFEF6FFFF;

	/* Reset PLLCFGR register */
	RCC->PLLCFGR = 0x24003010;

	/* Reset HSEBYP bit */
	RCC->CR &= (uint32_t)0xFFFBFFFF;

	/* Disable all interrupts */
	RCC->CIR = 0x00000000;

	/* Configure the System clock source, PLL Multiplier and Divider factors,
	   AHB/APBx prescalers and Flash settings ----------------------------------*/
	{
		/******************************************************************************/
		/*            PLL (clocked by HSE) used as System clock source                */
		/******************************************************************************/
		__IO uint32_t StartUpCounter = 0, HSEStatus = 0;

		/* Enable HSE */
		RCC->CR |= ((uint32_t)RCC_CR_HSEON);

		/* Wait till HSE is ready and if Time out is reached exit */
		do
		{
			HSEStatus = RCC->CR & RCC_CR_HSERDY;
			StartUpCounter++;
		}
		while ((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

		if ((RCC->CR & RCC_CR_HSERDY) != RESET)
		{
			HSEStatus = (uint32_t)0x01;
		}
		else
		{
			HSEStatus = (uint32_t)0x00;
		}

		if (HSEStatus == (uint32_t)0x01)
		{
			/* HCLK = SYSCLK / 1*/
			RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

			/* PCLK2 = HCLK / 2*/
			RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;

			/* PCLK1 = HCLK / 4*/
			RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;

			/* Configure the main PLL */
			RCC->PLLCFGR = PLL_M | (PLL_N << 6) | (((PLL_P >> 1) - 1) << 16) |
			               (RCC_PLLCFGR_PLLSRC_HSE) | (PLL_Q << 24);

			/* Enable the main PLL */
			RCC->CR |= RCC_CR_PLLON;

			/* Wait till the main PLL is ready */
			while ((RCC->CR & RCC_CR_PLLRDY) == 0)
			{
			}

			/* Configure Flash prefetch, Instruction cache, Data cache and wait state */
			FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_LATENCY_3WS;

			/* Select the main PLL as system clock source */
			RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
			RCC->CFGR |= RCC_CFGR_SW_PLL;

			/* Wait till the main PLL is used as system clock source */
			while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

			{
			}
		}
		else
		{
			/* If HSE fails to start-up, the application will have wrong clock
			     configuration. User can add here some code to deal with this error */
		}

	}

	/* Configure the Vector Table location add offset address ------------------*/
#ifdef VECT_TAB_SRAM
	SCB->VTOR = SRAM_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM */
#else
	SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH */
#endif

	__disable_irq();
}
void ram_flash_copy(unsigned int len)
#if 1
{
//	int i;
	unsigned int delay_i;
	unsigned int sector_num;
	unsigned int sector_code;
	unsigned int offset;
	u32 da;

	ram_system_init();


	/* Unlock the Flash to enable the flash control register access *************/
//	FLASH_Unlock();
	if ((FLASH->CR & FLASH_CR_LOCK) != RESET)
	{
		/* Authorize the FLASH Registers access */
		FLASH->KEYR = FLASH_KEY1;
		FLASH->KEYR = FLASH_KEY2;
	}

	ram_feeddog();


	/* Erase the user Flash area
	(area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

	/* Clear pending flags (if any) */
//	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
//                  			FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);

	FLASH->SR = FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
	            FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR;

	//Erase Sector 0 to 5;
	for (sector_num = 0; sector_num <= 4; sector_num++)
	{
		ram_feeddog();

		if (sector_num == 0)
		{
			sector_code = FLASH_Sector_0;
		}
		else if (sector_num == 1)
		{
			sector_code = FLASH_Sector_1;
		}
		else if (sector_num == 2)
		{
			sector_code = FLASH_Sector_2;
		}
		else if (sector_num == 3)
		{
			sector_code = FLASH_Sector_3;
		}
		else if (sector_num == 4)
		{
			sector_code = FLASH_Sector_4;
		}
		else if (sector_num == 5)
		{
			sector_code = FLASH_Sector_5;
		}

		/* Wait for last operation to be completed */
		//while ((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY)
		while (ram_FLASH_GetStatus() != FLASH_COMPLETE)
		{
			for (delay_i = 0; delay_i < 10; delay_i++)
			{
				delay_i = delay_i;
			}

			ram_feeddog();
		}

#if 0
		//proceed to erase the sector
		FLASH->CR &= CR_PSIZE_MASK;
		FLASH->CR |= VoltageRange_3;
		FLASH->CR &= SECTOR_MASK;
		FLASH->CR |= FLASH_CR_SER | sector_code;
		FLASH->CR |= FLASH_CR_STRT;
#else
		//proceed to erase the sector
		FLASH->CR &= CR_PSIZE_MASK;
		FLASH->CR |= FLASH_PSIZE_WORD;	//apply for VoltageRange_3
		FLASH->CR &= SECTOR_MASK;
		FLASH->CR |= FLASH_CR_SER | sector_code;
		FLASH->CR |= FLASH_CR_STRT;
#endif

		/* Wait for last operation to be completed */
		//while ((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY)
		while (ram_FLASH_GetStatus() != FLASH_COMPLETE)
		{
			for (delay_i = 0; delay_i < 10; delay_i++)
			{
				delay_i = delay_i;
			}

			ram_feeddog();
		}

		/* if the erase operation is completed, disable the SER Bit */
		FLASH->CR &= (~FLASH_CR_SER);
		FLASH->CR &= SECTOR_MASK;

		//if (FLASH_EraseSector(FLASH_Sector_5, VoltageRange_3) != FLASH_COMPLETE) {
		//	while (1) {
		//		ThisDebuger = ThisDebuger;
		//	}
		//}
	}


	//Program Bytes.

	for (offset = 0; offset < len; offset += 4)
	{
		ram_feeddog();

#if 0

		//Wait for last operation to be completed
		//while ((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY)
		while (ram_FLASH_GetStatus() != FLASH_COMPLETE)
		{
			for (delay_i = 0; delay_i < 10; delay_i++)
			{
				delay_i = delay_i;
			}

			ram_feeddog();
		}

#endif

		//read byte
		da = *((__IO u32 *)(PROG_BAK_ADDRESS + offset));

		//Wait for last operation to be completed
		//while ((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY)
		while (ram_FLASH_GetStatus() != FLASH_COMPLETE)
		{
			for (delay_i = 0; delay_i < 10; delay_i++)
			{
				delay_i = delay_i;
			}

			ram_feeddog();
		}

		//proceed to program the new data
		FLASH->CR &= CR_PSIZE_MASK;
		FLASH->CR |= FLASH_PSIZE_WORD;
		FLASH->CR |= FLASH_CR_PG;

		*(__IO u32 *)(ADDR_FLASH_SECTOR_0 + offset) = da;


		//Wait for last operation to be completed
		//while ((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY)
		while (ram_FLASH_GetStatus() != FLASH_COMPLETE)
		{
			for (delay_i = 0; delay_i < 10; delay_i++)
			{
				delay_i = delay_i;
			}

			ram_feeddog();
		}

		/* if the program operation is completed, disable the PG Bit */
		FLASH->CR &= (~FLASH_CR_PG);

		//Wait for last operation to be completed
		//while ((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY)
		while (ram_FLASH_GetStatus() != FLASH_COMPLETE)
		{
			for (delay_i = 0; delay_i < 10; delay_i++)
			{
				delay_i = delay_i;
			}

			ram_feeddog();
		}

	}

	ram_feeddog();


	/* Lock the Flash to disable the flash control register access (recommended
	to protect the FLASH memory against possible unwanted operation) *********/
//	FLASH_Lock();

	/* Set the LOCK Bit to lock the FLASH Registers access */
	FLASH->CR |= FLASH_CR_LOCK;
#if 0

	//Wait for last operation to be completed
	//while ((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY)
	while (ram_FLASH_GetStatus() != FLASH_COMPLETE)
	{
		for (delay_i = 0; delay_i < 10; delay_i++)
		{
			delay_i = delay_i;
		}

		ram_feeddog();
	}

#endif

	ram_feeddog();

	for (delay_i = 0; delay_i < 100000; delay_i++)
	{
		delay_i = delay_i;
	}


//	NVIC_SystemReset();
//	SCB->AIRCR  = ((0x5FA << SCB_AIRCR_VECTKEY_Pos)      |
//					(SCB->AIRCR & SCB_AIRCR_PRIGROUP_Msk) |
//					SCB_AIRCR_SYSRESETREQ_Msk);                   /* Keep priority group unchanged */


	for (delay_i = 0; delay_i < 900000; delay_i++)
	{
		delay_i = delay_i;
	}

	for (delay_i = 0; delay_i < 900000; delay_i++)
	{
		delay_i = delay_i;
	}

	while (1)
	{
		delay_i = delay_i;
	}

}
#else
{
//	int i;
	int delay_i;
	int sector_num;
	int sector_code;

	int offset;
	u32 da;

	ram_system_init();


	/* Unlock the Flash to enable the flash control register access *************/
//	FLASH_Unlock();
	if ((FLASH->CR & FLASH_CR_LOCK) != RESET)
	{
		/* Authorize the FLASH Registers access */
		FLASH->KEYR = FLASH_KEY1;
		FLASH->KEYR = FLASH_KEY2;
	}

	ram_feeddog();


	/* Erase the user Flash area
	(area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

	/* Clear pending flags (if any) */
//	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
//                  			FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);

	FLASH->SR = FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
	            FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR;


	//Erase Sector 0 to 5;
	for (sector_num = 0; sector_num <= 4; sector_num++)
	{

		if (sector_num == 0)
		{
			sector_code = FLASH_Sector_0;
		}
		else if (sector_num == 1)
		{
			sector_code = FLASH_Sector_1;
		}
		else if (sector_num == 2)
		{
			sector_code = FLASH_Sector_2;
		}
		else if (sector_num == 3)
		{
			sector_code = FLASH_Sector_3;
		}
		else if (sector_num == 4)
		{
			sector_code = FLASH_Sector_4;
		}
		else if (sector_num == 5)
		{
			sector_code = FLASH_Sector_5;
		}

		/* Wait for last operation to be completed */
		while ((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY)
		{
			for (delay_i = 0; delay_i < 10; delay_i++)
			{
				delay_i = delay_i;
			}
		}

		//proceed to erase the sector
		FLASH->CR &= CR_PSIZE_MASK;
		FLASH->CR |= VoltageRange_3;
		FLASH->CR &= SECTOR_MASK;
		FLASH->CR |= FLASH_CR_SER | sector_code;
		FLASH->CR |= FLASH_CR_STRT;

		/* Wait for last operation to be completed */
		while ((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY)
		{
			for (delay_i = 0; delay_i < 10; delay_i++)
			{
				delay_i = delay_i;
			}
		}

		/* if the erase operation is completed, disable the SER Bit */
		FLASH->CR &= (~FLASH_CR_SER);
		FLASH->CR &= SECTOR_MASK;

		//if (FLASH_EraseSector(FLASH_Sector_5, VoltageRange_3) != FLASH_COMPLETE) {
		//	while (1) {
		//		ThisDebuger = ThisDebuger;
		//	}
		//}

		ram_feeddog();

	}




	//Program Bytes.

	for (offset = 0; offset < len; offset += 4)
	{

		//Wait for last operation to be completed
		while ((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY)
		{
			for (delay_i = 0; delay_i < 10; delay_i++)
			{
				delay_i = delay_i;
			}
		}

		//read byte
		da = *((__IO u32 *)(ADDR_FLASH_SECTOR_5 + offset));

		//Wait for last operation to be completed
		while ((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY)
		{
			for (delay_i = 0; delay_i < 10; delay_i++)
			{
				delay_i = delay_i;
			}
		}

		//proceed to program the new data
		FLASH->CR &= CR_PSIZE_MASK;
		FLASH->CR |= FLASH_PSIZE_WORD;
		FLASH->CR |= FLASH_CR_PG;

		*(__IO u32 *)(ADDR_FLASH_SECTOR_0 + offset) = da;


		//Wait for last operation to be completed
		while ((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY)
		{
			for (delay_i = 0; delay_i < 10; delay_i++)
			{
				delay_i = delay_i;
			}
		}

		/* if the program operation is completed, disable the PG Bit */
		FLASH->CR &= (~FLASH_CR_PG);

		//Wait for last operation to be completed
		while ((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY)
		{
			for (delay_i = 0; delay_i < 10; delay_i++)
			{
				delay_i = delay_i;
			}
		}

	}

	ram_feeddog();


	/* Lock the Flash to disable the flash control register access (recommended
	to protect the FLASH memory against possible unwanted operation) *********/
//	FLASH_Lock();

	/* Set the LOCK Bit to lock the FLASH Registers access */
	FLASH->CR |= FLASH_CR_LOCK;

	//Wait for last operation to be completed
	while ((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY)
	{
		for (delay_i = 0; delay_i < 10; delay_i++)
		{
			delay_i = delay_i;
		}
	}

	ram_feeddog();

	for (delay_i = 0; delay_i < 100000; delay_i++)
	{
		delay_i = delay_i;
	}


//	NVIC_SystemReset();
//	SCB->AIRCR  = ((0x5FA << SCB_AIRCR_VECTKEY_Pos)      |
//					(SCB->AIRCR & SCB_AIRCR_PRIGROUP_Msk) |
//					SCB_AIRCR_SYSRESETREQ_Msk);                   /* Keep priority group unchanged */


	for (delay_i = 0; delay_i < 900000; delay_i++)
	{
		delay_i = delay_i;
	}

	for (delay_i = 0; delay_i < 900000; delay_i++)
	{
		delay_i = delay_i;
	}

	while (1)
	{
		delay_i = delay_i;
	}


}
#endif






