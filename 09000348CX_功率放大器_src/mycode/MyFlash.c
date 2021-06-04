
//*****************************************************//
//		MyFlash.c
//*****************************************************//


#include "MyAll.h"



//////////////////////////////////////////////////////////

int ThisDebuger;


//////////////////////////////////////////////////////////


void FlashInit(void)
{

	/* Unlock the Flash to enable the flash control register access *************/ 
	FLASH_Unlock();

	/* Erase the user Flash area
	(area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

	/* Clear pending flags (if any) */  
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  			FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 

	if (FLASH_EraseSector(FLASH_Sector_5, VoltageRange_3) != FLASH_COMPLETE) {
		while (1) {
			ThisDebuger = ThisDebuger;
		}
	}


}

void FlashUnlock(void)
{
	/* Unlock the Flash to enable the flash control register access *************/ 
	FLASH_Unlock();

}

void FlashLock(void)
{
	/* Lock the Flash to disable the flash control register access (recommended
	to protect the FLASH memory against possible unwanted operation) *********/
	FLASH_Lock(); 	

}


void FlashWrite(int addr, u32 *pbuf, int len)
{
	int i;

	//Program Bytes.
	for(i=0; i<len; i++) {
		if (FLASH_ProgramWord((ADDR_FLASH_SECTOR_5+addr), (*(pbuf+i))) != FLASH_COMPLETE) {
			while (1) {
				ThisDebuger = ThisDebuger;
			}
		}

		addr += 4;
		
	}
	

}

void FlashWriteBytes(int addr, u8 *pbuf, int len)
{
	int i;

	/* Unlock the Flash to enable the flash control register access *************/ 
	FLASH_Unlock();
    
	/* Erase the user Flash area
	(area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

	/* Clear pending flags (if any) */  
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  			FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 

	if (FLASH_EraseSector(FLASH_Sector_5, VoltageRange_3) != FLASH_COMPLETE) {
		while (1) {
			ThisDebuger = ThisDebuger;
		}
	}

	
	//Program Bytes.
	for(i=0; i<len; i++) {
		if (FLASH_ProgramByte((ADDR_FLASH_SECTOR_5+addr+i), (*(pbuf+i))) != FLASH_COMPLETE) {
			while (1) {
				ThisDebuger = ThisDebuger;
			}
		}
	}
	

	/* Lock the Flash to disable the flash control register access (recommended
	to protect the FLASH memory against possible unwanted operation) *********/
	FLASH_Lock(); 	

}


void FlashRead(int addr, u8 *pbuf, int len)
{
	int i;

	for(i=0; i<len; i++) {
		*(pbuf + i) = *((__IO u8*)(ADDR_FLASH_SECTOR_5+addr+i));
	}

}



u32 FlashReadWord(u32 addr)
{
	u32 rdata;
	
	rdata = *(__IO uint32_t*)addr;	

	return rdata;

}



void FlashProgramCopy(int len)
{

	AppOff();

	Wwdg_Feed();
	
	FeedDog();
	Delay(1000);
	FeedDog();
	Delay(1000);

	ram_flash_copy(len);

}





