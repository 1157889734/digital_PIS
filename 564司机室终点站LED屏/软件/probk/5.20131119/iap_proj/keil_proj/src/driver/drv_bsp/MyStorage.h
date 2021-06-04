//*******************************************************//
//						MyStorage.h								//
//*******************************************************//

#ifndef _MYSTORAGE_H
#define  _MYSTORAGE_H

#include "MyAll.h"




__packed typedef struct 
{
	u8 Head;

	u8 LocalIp[4];
	u8 RouterIp[4];
	u8 MaskIp[4];
	u8 LocalMac[6];

	//u8 DevId;
	//u8 brightness;
	

	u8 AudioVolume;

	u16 TrainId;
	
	u8 Tail;

	//u32 ccIp32;	// 1ºÅÖÐ¿Ø µÄip
	u8 checksum;
	
}StorageInfoTypeDef;




extern StorageInfoTypeDef StorageInfo;



void StorageInit(void);


void StorageInfoSave(void);

void StorageInfoSaveProcess(void);

void StorageInfoRead(void);





#endif


