
//*****************************************************//
//		MyFlash.c
//*****************************************************//


#include "MyAll.h"



u32 FlashPage1Addr;

u8* pFlashPage1Buf;
int FlashPage1BufSize;


void FlashInit(void)
{

//	FlashWrite();

//	Delay(1000);

//	FlashReadWord(ADDR_FLASH_SECTOR_5);


	FlashPage1Addr = ADDR_FLASH_SECTOR_5;
	pFlashPage1Buf = (u8*)(&StorageInfo);
	FlashPage1BufSize = sizeof(StorageInfo);


}



void FlashWrite(void)
{
	int i;


	/* Unlock the Flash to enable the flash control register access *************/ 
	FLASH_Unlock();
    
	/* Erase the user Flash area
	(area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

	/* Clear pending flags (if any) */  
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  			FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 


	if (FLASH_EraseSector(FLASH_Sector_5, VoltageRange_3) != FLASH_COMPLETE)
	{
		while (1)
		{
		
		}
		
	}

	for(i=0; i<FlashPage1BufSize; i++)
	{

		if (FLASH_ProgramByte((FlashPage1Addr+i), (*(pFlashPage1Buf+i))) != FLASH_COMPLETE)
		{
			while (1)
			{
		
			}
		
		}

	}


	/* Lock the Flash to disable the flash control register access (recommended
	to protect the FLASH memory against possible unwanted operation) *********/
	FLASH_Lock(); 
	

}

void FlashReadPage1(void)
{
	int i;

//	memcpy(pFlashPage1Buf, ((__IO u32*)FlashPage1Addr), FlashPage1BufSize);

	for(i=0; i<FlashPage1BufSize; i++)
	{
		*(pFlashPage1Buf+i) = *((__IO u8*)(FlashPage1Addr+i));

	}

}



u32 FlashReadWord(u32 addr)
{
	u32 rdata;
	
	rdata = *(__IO uint32_t*)addr;	

	return rdata;

}





