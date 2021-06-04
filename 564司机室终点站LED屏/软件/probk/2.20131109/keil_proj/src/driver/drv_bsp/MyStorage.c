
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

	if(StorageInfoSaveFlag == 1)
	{
		StorageInfoSaveFlag = 0;
	
		StorageInfo.Head = 0xa5;

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

		StorageInfo.LocalMac[0] = NetLocalMac[0];		//年
		StorageInfo.LocalMac[1] = NetLocalMac[1];		//月
		StorageInfo.LocalMac[2] = NetLocalMac[2];		//日
		StorageInfo.LocalMac[3] = NetLocalMac[3];		//时
		StorageInfo.LocalMac[4] = NetLocalMac[4];		//分
		StorageInfo.LocalMac[5] = NetLocalMac[5];		//秒

		StorageInfo.AudioVolume = AudioVolume;

		StorageInfo.TrainId= TrainId;

		//StorageInfo.ccIp32 = CentralController_IP32;

		StorageInfo.Tail = 0x5a;
		

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

/* */
	if((StorageInfo.Head == 0xa5) && (StorageInfo.Tail == 0x5a))
	{
		NetLocalIp[0] = StorageInfo.LocalIp[0];
		NetLocalIp[1] = StorageInfo.LocalIp[1];
		NetLocalIp[2] = StorageInfo.LocalIp[2];
		NetLocalIp[3] = StorageInfo.LocalIp[3];

		NetRouterIp[0] = StorageInfo.RouterIp[0];
		NetRouterIp[1] = StorageInfo.RouterIp[1];
		NetRouterIp[2] = StorageInfo.RouterIp[2];
		NetRouterIp[3] = StorageInfo.RouterIp[3];

		NetMaskIp[0] = StorageInfo.MaskIp[0];
		NetMaskIp[1] = StorageInfo.MaskIp[1];
		NetMaskIp[2] = StorageInfo.MaskIp[2];
		NetMaskIp[3] = StorageInfo.MaskIp[3];

		NetLocalMac[0] = StorageInfo.LocalMac[0];		//年
		NetLocalMac[1] = StorageInfo.LocalMac[1];		//月
		NetLocalMac[2] = StorageInfo.LocalMac[2];		//日
		NetLocalMac[3] = StorageInfo.LocalMac[3];		//时
		NetLocalMac[4] = StorageInfo.LocalMac[4];		//分
		NetLocalMac[5] = StorageInfo.LocalMac[5];		//秒

		AudioVolume = StorageInfo.AudioVolume;
		
		//DevId = StorageInfo.DevId;
		TrainId = StorageInfo.TrainId;

		//CentralController_IP32 = StorageInfo.ccIp32;

	}
	/* */


}





