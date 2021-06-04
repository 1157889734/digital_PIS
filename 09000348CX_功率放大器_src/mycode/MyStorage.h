//*******************************************************//
//						MyStorage.h								//
//*******************************************************//

#ifndef _MYSTORAGE_H
#define  _MYSTORAGE_H

#include "MyAll.h"




__packed typedef struct 		//max for 32 bytes.
{
	u8 Head;				

	u8 LocalIp[4];			
	u8 RouterIp[4];
	u8 MaskIp[4];

	u8 DevId;
	u16 TrainId;

	u8 AudioVolume;


	u8 BaseIp[4];

	u8 LocalIpSetFlag;


	u8 Tail;

	
}StorageInfoTypeDef;




extern StorageInfoTypeDef StorageInfo;


extern int StorageInfoSaveFlag;



void StorageInit(void);


void StorageInfoSave(void);

void StorageInfoSaveProcess(void);

void StorageInfoRead(void);





#endif


