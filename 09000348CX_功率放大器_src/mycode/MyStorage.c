
//*****************************************************//
//		MyStorage.c
//*****************************************************//

#include "MyAll.h"



StorageInfoTypeDef StorageInfo;

int StorageInfoSaveFlag;



void StorageInit(void)
{

//	FlashInit();

	AT24C32_Init();

}

void StorageInfoSave(void)
{

	StorageInfoSaveFlag = 1;

}


void StorageInfoSaveProcess(void)
{

	if((StorageInfoSaveFlag == 1) && (EmeVolumeSetFlag == 0))
	{
		StorageInfoSaveFlag = 0;
	
		StorageInfo.Head = 0x23;

		StorageInfo.LocalIp[0] = NetLocalIp[0];
		StorageInfo.LocalIp[1] = NetLocalIp[1];
		StorageInfo.LocalIp[2] = NetLocalIp[2];
		StorageInfo.LocalIp[3] = NetLocalIp[3];

		StorageInfo.RouterIp[0] = NetRouterIp[0];
		StorageInfo.RouterIp[1] = NetRouterIp[1];
		StorageInfo.RouterIp[2] = NetRouterIp[2];
		StorageInfo.RouterIp[3] = NetRouterIp[3];

		StorageInfo.MaskIp[0] = NetMaskIp[0];
		StorageInfo.MaskIp[1] = NetMaskIp[1];
		StorageInfo.MaskIp[2] = NetMaskIp[2];
		StorageInfo.MaskIp[3] = NetMaskIp[3];
	
//		StorageInfo.DevId= DevId;		
		StorageInfo.TrainId= TrainId;

		StorageInfo.AudioVolume = AudioVolume;
		

		StorageInfo.BaseIp[0] = NetBaseIp[0];
		StorageInfo.BaseIp[1] = NetBaseIp[1];
		StorageInfo.BaseIp[2] = NetBaseIp[2];
		StorageInfo.BaseIp[3] = NetBaseIp[3];
		

//		FlashWrite();

		AT24C32_PageWrite(8, ((u8*)(&StorageInfo)), sizeof(StorageInfo));

		
		Delay(1000);
	

	}


}


void StorageInfoRead(void)
{

	AT24C32_PageRead(8, ((u8*)(&StorageInfo)), sizeof(StorageInfo));

//	FlashReadPage1();


	Delay(1000);

	if(StorageInfo.Head == 0x23)
	{



		//DevId = StorageInfo.DevId;
		//TrainId = StorageInfo.TrainId;

		AudioVolume = StorageInfo.AudioVolume;

	}


}





