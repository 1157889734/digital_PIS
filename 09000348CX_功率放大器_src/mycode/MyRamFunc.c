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




/*  Assignment of Code Memory for Module to RAM (under Options for file 'RamFunc.c') */
/*   RO - Code/Const:       RAM1 (0x80010000-0x8001FFFF)                             */




#define SECTOR_MASK               ((uint32_t)0xFFFFFF07)


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


int count;


void ram_feeddog(void);


void ram_function (void) {

	count++;
	count++;
	count++;
	count++;
	count++;
	count++;
  
}



 
void ram_flash_copy(int len)
{
//	int i;
	int delay_i;
	int sector_num;
	int sector_code;
	
	int offset;
	u32 da;
	
	
	/* Unlock the Flash to enable the flash control register access *************/ 
//	FLASH_Unlock();
	 if((FLASH->CR & FLASH_CR_LOCK) != RESET) {
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
                  			FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR; 


	 //Erase Sector 0 to 5;
	 for(sector_num=0; sector_num<=4; sector_num++) {
	 	
	 	if(sector_num == 0) {
			sector_code = FLASH_Sector_0;
	 	}
		else if(sector_num == 1) {
			sector_code = FLASH_Sector_1;
	 	}
		else if(sector_num == 2) {
			sector_code = FLASH_Sector_2;
	 	}
		else if(sector_num == 3) {
			sector_code = FLASH_Sector_3;
	 	}
		else if(sector_num == 4) {
			sector_code = FLASH_Sector_4;
	 	}
		else if(sector_num == 5) {
			sector_code = FLASH_Sector_5;
	 	}		

		 /* Wait for last operation to be completed */
		while ((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY) {
			for (delay_i=0; delay_i<10; delay_i++) {
				delay_i = delay_i;
			}
            ram_feeddog();
		}

		//proceed to erase the sector 
		FLASH->CR &= CR_PSIZE_MASK;
		FLASH->CR |= VoltageRange_3;
		FLASH->CR &= SECTOR_MASK;
		FLASH->CR |= FLASH_CR_SER | sector_code;
		FLASH->CR |= FLASH_CR_STRT;

		 /* Wait for last operation to be completed */
		while ((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY) {
			for (delay_i=0; delay_i<10; delay_i++) {
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

		ram_feeddog();

	}


	

	 //Program Bytes.

	for(offset=0; offset<len; offset+=4) {

		//Wait for last operation to be completed
		while ((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY) {
			for (delay_i=0; delay_i<10; delay_i++) {
				delay_i = delay_i;
			}
            ram_feeddog();
		}

		//read byte
		da = *((__IO u32*)(ADDR_FLASH_SECTOR_5+offset));

		//Wait for last operation to be completed
		while ((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY) {
			for (delay_i=0; delay_i<10; delay_i++) {
				delay_i = delay_i;
			}
            ram_feeddog();
		}

		//proceed to program the new data
		FLASH->CR &= CR_PSIZE_MASK;
		FLASH->CR |= FLASH_PSIZE_WORD;
		FLASH->CR |= FLASH_CR_PG;
  
		*(__IO u32*)(ADDR_FLASH_SECTOR_0+offset)= da;
		
        
		//Wait for last operation to be completed
		while ((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY) {
			for (delay_i=0; delay_i<10; delay_i++) {
				delay_i = delay_i;
			}
		}

		/* if the program operation is completed, disable the PG Bit */
		FLASH->CR &= (~FLASH_CR_PG);

		//Wait for last operation to be completed
		while ((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY) {
			for (delay_i=0; delay_i<10; delay_i++) {
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

	//Wait for last operation to be completed
	while ((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY) {
		for (delay_i=0; delay_i<10; delay_i++) {
			delay_i = delay_i;
		}
        ram_feeddog();
	}

	ram_feeddog();

	for (delay_i=0; delay_i<100000; delay_i++) {
		delay_i = delay_i;
	}


//	NVIC_SystemReset();
//	SCB->AIRCR  = ((0x5FA << SCB_AIRCR_VECTKEY_Pos)      | 
//					(SCB->AIRCR & SCB_AIRCR_PRIGROUP_Msk) | 
//					SCB_AIRCR_SYSRESETREQ_Msk);                   /* Keep priority group unchanged */
    

	for (delay_i=0; delay_i<900000; delay_i++) {
		delay_i = delay_i;
	}

	for (delay_i=0; delay_i<900000; delay_i++) {
		delay_i = delay_i;
	}

	while(1) {
		delay_i = delay_i;
	}
	

}





void ram_feeddog(void)
{

//	#define PORT_FEEDDOG		GPIOE
//	#define PIN_FEEDDOG		GPIO_Pin_2

	static int bit_val = 0;
 
	if (bit_val == 0)
	{
		GPIOE->BSRRL = GPIO_Pin_2;
		bit_val = 1;
	}
	else
	{
		GPIOE->BSRRH = GPIO_Pin_2 ;
		bit_val = 0;
	}
	
}






