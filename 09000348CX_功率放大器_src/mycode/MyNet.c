
//*****************************************************//
//		MyNet.c
//*****************************************************//



/* Includes ------------------------------------------------------------------*/
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/tcp.h"
#include "lwip/udp.h"
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "ethernetif.h"
#include "main.h"
//#include "netconf.h"
#include <stdio.h>


#include "MyAll.h"




u8 NetLocalMac[6];

u8 NetLocalIp[4];
u8 NetRouterIp[4];
u8 NetMaskIp[4];

u8 NetShowIp[4];


u8 NetBuf[1280];

u8* pNetReceiveBuf;
int NetReceiveBufSize;

u8 *pNetSendBuf;
int NetSendBufSize;


int NetStartingFlag;

int NetListenSendFlag;

u8 NetBaseIp[4];

u8 NetLocalBroadcastIp[4];

int NetDownloadStarted;
int NetDownloadBlocks;
int NetDownloadBlockNum;
int reboot_flag = 0;
int NetHad = 0;
int NetOkFlag = 0;


int NetInit()
{
	u32 temp;


	Delay(10000);

	NetLocalMac[0] = 0x5e;
	NetLocalMac[1] = 0x45;
	NetLocalMac[2] = 0xc0;
	NetLocalMac[3] = 0xa8;	//168
	NetLocalMac[4] = 0x65;	//101
	NetLocalMac[5] = 0x64;	//100

	NetLocalIp[0] = 192;
	NetLocalIp[1] = 168;
	NetLocalIp[2] = 101;
	NetLocalIp[3] = 100;

	NetRouterIp[0] = 192;
	NetRouterIp[1] = 168;
	NetRouterIp[2] = 101;
	NetRouterIp[3] = 1;

	NetMaskIp[0] = 255;
	NetMaskIp[1] = 255;
	NetMaskIp[2] = 255;
	NetMaskIp[3] = 0;

	NetBaseIp[0] = 192;
	NetBaseIp[1] = 168;
	NetBaseIp[2] = 101;
	NetBaseIp[3] = 143;

	/*
		Delay(1000);
		StorageInfoSave();
		Delay(1000);
		StorageInfoSaveProcess();
		Delay(1000);
		Delay(1000);
	*/

//	NetSetBaseIp();

	Addr = GetAddress();

	DevId = Addr;

	NetLocalIp[0] = NetBaseIp[0];
	NetLocalIp[1] = NetBaseIp[1];
	NetLocalIp[2] = NetBaseIp[2];

	NetLocalIp[3] = NetBaseIp[3] + DevId;


	if(NetLocalIp[3] == 0)
	{
		NetLocalIp[3] = 255;

	}

	NetLocalMac[3] = NetLocalIp[1];
	NetLocalMac[4] = NetLocalIp[2];
	NetLocalMac[5] = NetLocalIp[3];


	NetLocalBroadcastIp[0] = NetLocalIp[0];
	NetLocalBroadcastIp[1] = NetLocalIp[1];
	NetLocalBroadcastIp[2] = NetLocalIp[2];
	NetLocalBroadcastIp[3] = 255;



	pNetSendBuf = NetBuf;

	NetStartingFlag = 1;



	Delay(10000);

	EthernetInit();

	Delay(10000);

//	temp = ETH_ReadPHYRegister(1,2);

	temp = temp;


//	SwitchInit();


	/* Initilaize the LwIP stack */
	LwipInit();

	Delay(10000);

	ProtocolInit();

	HttpdInit();



	return 0;
}


void NetProcess(void)
{

	//NetPlayclkSendProcess();

}



void Net50ms(void)
{



}



void Net500ms(void)
{

	NetControlSendInspection();

    if (NetOkFlag) {
    	NetOkFlag --;
        reboot_flag = 0;
    }
}






int NetIp32toIp4(u8* ip4, u32 ip32)
{

	*(ip4+0) = (u8)(ip32 >> 24);
	*(ip4+1) = (u8)(ip32 >> 16);
	*(ip4+2) = (u8)(ip32 >> 8);
	*(ip4+3) = (u8)(ip32 >> 0);


	return 0;

}


u32 NetIp4toIp32(u8* ip4)
{
	u32 ip32;

	ip32 = ((*(ip4))<<24) | ((*(ip4+1))<<16) | ((*(ip4+2))<<8) | (*(ip4+3));


	return ip32;

}




int NetSetLocalIp4(u8 ip1, u8 ip2, u8 ip3, u8 ip4)
{

	if(ip1 < 128)
	{
		return -1;
	}
	if(ip2 > 247)
	{
		return -1;
	}
	if(ip3 > 255)
	{
		return -1;
	}
	if(ip4 > 255)
	{
		return -1;
	}


	NetLocalIp[0] = ip1;
	NetLocalIp[1] = ip2;
	NetLocalIp[2] = ip3;
	NetLocalIp[3] = ip4;

	NetBaseIp[0] = ip1;
	NetBaseIp[1] = ip2;
	NetBaseIp[2] = ip3;
	NetBaseIp[3] = ip4;


	Delay(1000);
	StorageInfoSave();
	Delay(1000);
	StorageInfoSaveProcess();
	Delay(1000);
	Delay(1000);


	Delay(100000);
	Delay(100000);
//	NVIC_SystemReset();
//	Delay(10000);
//	Delay(10000);



	return 0;

}

int NetSetLocalIp32(u32 ip)
{
	u8 ip1;
	u8 ip2;
	u8 ip3;
	u8 ip4;


	ip1 = (u8)(ip>>24);
	ip2 = (u8)(ip>>16);
	ip3 = (u8)(ip>>8);
	ip4 = (u8)(ip);


	if(ip1 < 128)
	{
		return -1;
	}
	if(ip1 > 247)
	{
		return -1;
	}
	if(ip2 > 255)
	{
		return -1;
	}
	if(ip3 > 255)
	{
		return -1;
	}
	if(ip4 > 255)
	{
		return -1;
	}


	NetLocalIp[0] = ip1;
	NetLocalIp[1] = ip2;
	NetLocalIp[2] = ip3;
	NetLocalIp[3] = ip4;


	return 0;

}





int NetControlReceiveProcess(u8* pbuf, int len)
{
	u32 keyword;


	pNetReceiveBuf = pbuf;
	NetReceiveBufSize = len;

    NetOkFlag = 10;

    if (!NetHad) {
        NetHad = 1;
    }

	if(ProtocolRcvHeadCheck(pNetReceiveBuf) != 0)
	{
		return -1;

	}

	keyword = ProtocolRcvGetKeyword(pNetReceiveBuf);

	switch(keyword)
	{
		case PRTCL_KEYWORD_VOLUME :
		{

			NetControlReceiveVolume();
			break;
		}

		////????????????????
		case PRTCL_KEYWORD_INSPECTION:
		{
			NetControlReceiveInspection();
			break;
		}

		case PRTCL_KEYWORD_SELFTEST_CMD:
		{
			NetControlReceiveSelfTestCmd();
			break;
		}

		case PRTCL_KEYWORD_ALLDEV_UPDATE:
		{
			NetControlReceiveAllDevUpdate();
			break;
		}


		default :
		{

			break;
		}


	}


	return 0;

}


int NetControlReceiveVolume(void)
{
	u8 volume;

	volume = ProtocolRcvGetControlVolume(pNetReceiveBuf, NetReceiveBufSize, DevType, DevId);

	if(volume <= 100)
	{
		if(volume!=AudioVolume)
		{
			//	AudioSetVolume(volume);
		}


	}

	return 0;

}



int NetControlReceiveInspection(void)
{

//	u8* pda;

	ProtocolHeadTypeDef *phead;
	ProtocolDeviceInfoTypeDef *psrcdev;

	u32 src_ip;



	phead = (ProtocolHeadTypeDef*)(pNetReceiveBuf);
	psrcdev = (ProtocolDeviceInfoTypeDef*)(&(phead->SrcInfo));


//	pda = (u8*)(ProtocolRcvGetDataPointer(pNetReceiveBuf));



	if(psrcdev->DevType == PRTCL_DEVTYPE_CC)
	{
		if(psrcdev->DevId == 1)
		{
			src_ip = psrcdev->Ip32;
			NetGetBaseIp(src_ip);

		}

	}
	else if(psrcdev->DevType == PRTCL_DEVTYPE_DCP)
	{


	}
	else if(psrcdev->DevType == PRTCL_DEVTYPE_DCVC)
	{
		NetGetCcvcState();

	}
	else if(psrcdev->DevType == PRTCL_DEVTYPE_PCVC)
	{
		if((psrcdev->DevId == 1)&&((DevId==1)||(DevId==2)))
		{
			NetGetPcvcState();
		}
		else if((psrcdev->DevId == 2)&&((DevId==3)||(DevId==4)))
		{
			NetGetPcvcState();
		}
		else if((psrcdev->DevId == 3)&&((DevId==5)||(DevId==6)))
		{
			NetGetPcvcState();
		}
		else if((psrcdev->DevId == 4)&&((DevId==7)||(DevId==8)))
		{
			NetGetPcvcState();
		}
		else if((psrcdev->DevId == 5)&&((DevId==9)||(DevId==10)))
		{
			NetGetPcvcState();
		}
		else if((psrcdev->DevId == 6)&&((DevId==11)||(DevId==12)))
		{
			NetGetPcvcState();
		}
		

	}



	return 0;

}




int NetControlReceiveSelfTestCmd(void)
{
	u8* pda;
	int id;
	int cmd;
	int result;

	pda = ProtocolRcvGetDataPointer(pNetReceiveBuf);

	id = *(pda);
	cmd = *(pda+1);

	if((id == DevId) && (cmd == 0x01))
	{
		result = SelfTestStart();

		NetControlSendSelfTestState(result);

	}


	return 0;

}



int NetControlReceiveAllDevUpdate(void)
{
	u8* pda;
	int da_len;

	struct ip_addr server_ip;
	u8 filename[32];
	int fn_len;

	ProtocolHeadTypeDef *phead;
	ProtocolDeviceInfoTypeDef *psrcdev;


	phead = (ProtocolHeadTypeDef*)(pNetReceiveBuf);
	psrcdev = (ProtocolDeviceInfoTypeDef*)(&(phead->SrcInfo));

	if(psrcdev->DevType != DevType)
	{
		return -1;
	}


	pda = ProtocolRcvGetDataPointer(pNetReceiveBuf);
	da_len = ((ProtocolPackTypeDef*)(pNetReceiveBuf))->DataLength;

	IP4_ADDR(&server_ip, *(pda+3), *(pda+2), *(pda+1), *(pda+0));
	fn_len = da_len - 4;
	memcpy(filename, (pda+4), fn_len);

	TftpClientIapStart(server_ip, (char *)filename);


	return 0;

}






///////////////////////////////////////////////////////////////////



int NetControlSendInspection(void)
{
	u8 priority;

	if(AudioPlayInfo.EnableFlag > 0)
	{
		priority = AudioPlayInfo.Priority;

	}
	else
	{
		priority = 255;

	}

	ProtocolSndGetInspectionPack(pNetSendBuf, &NetSendBufSize, priority, CurrentState, (u8)AudioNdedVolume, AppVersion1);

	UdpControlSend(pNetSendBuf, NetSendBufSize);


	return 0;

}


int NetControlSendSelfTestState(int state)
{
	u8 send_state;

	if(state < 0)
	{
		send_state = 0x02;

	}
	else
	{
		send_state = 0x01;

	}


	ProtocolSndGetSelfTestStatePack(pNetSendBuf, &NetSendBufSize, send_state);

	UdpControlSend(pNetSendBuf, NetSendBufSize);


	return 0;

}


int NetControlSendSelfTestReport(void)
{


	ProtocolSndGetSelfTestReportPack(pNetSendBuf, &NetSendBufSize);

	UdpControlSend(pNetSendBuf, NetSendBufSize);


	return 0;

}


int NetControlSendAllDevUpdate(struct ip_addr server_ip, u8 *filename, int fn_len)
{
	u16 kw;
	u8 da[64];
	int len;

	u32 ip32;
	u32 *p_ip32;
	u8 ip4[4];


	kw = PRTCL_KEYWORD_ALLDEV_UPDATE;

	p_ip32 = (u32 *)&server_ip;

	ip32 = *p_ip32;

	NetIp32toIp4(ip4, ip32);


	da[0] = ip4[0];
	da[1] = ip4[1];
	da[2] = ip4[2];
	da[3] = ip4[3];

	memcpy(&(da[4]), filename, fn_len+1);

	len = 4 + fn_len+1;

	ProtocolSndGetPack(pNetSendBuf, &NetSendBufSize, kw, da, len);

	UdpControlSend(pNetSendBuf, NetSendBufSize);


	return 0;

}





////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////



int NetBroadcastReceiveProcess(u8* pbuf, int len)
{
	u16 keyword;
	u8 source_type;
	u8 source_id;
    u8 brd_type;
    u8 dst_train_id;


	ProtocolAudioDataTypeDef * pda;


	pNetReceiveBuf = pbuf;
	NetReceiveBufSize = len;


	if(len < 1024)
	{
		return -1;

	}

	if(ProtocolRcvHeadCheck(pNetReceiveBuf) != 0)
	{
		return -1;

	}

	keyword = ProtocolRcvGetKeyword(pNetReceiveBuf);
	source_type = ProtocolRcvGetSrcDevType(pNetReceiveBuf);
	source_id = ProtocolRcvGetSrcDevId(pNetReceiveBuf);
    pda = (ProtocolAudioDataTypeDef*)(ProtocolRcvGetDataPointer(pNetReceiveBuf));
    brd_type = pda->Type;
    dst_train_id = ProtocolRcvGetDstTrainId(pNetReceiveBuf);

	if(keyword == PRTCL_KEYWORD_BROADCAST)
	{

        if ((brd_type == 0x0c) && (keyword == PRTCL_KEYWORD_BROADCAST)) {
            if (TrainId != dst_train_id) {
                return;
            }
        }
        
        pda = (ProtocolAudioDataTypeDef*)(ProtocolRcvGetDataPointer(pNetReceiveBuf));

		if(AudioPlayInfo.EnableFlag < 1)
		{
//			AudioUp();

			AudioPlayInfo.EnableFlag = 10;
			AudioPlayInfo.SourceType = source_type;
			AudioPlayInfo.SourceId = source_id;
			AudioPlayInfo.Priority = pda->Priority;
			AudioPlayInfo.Type = pda->Type;
			if((AudioPlayCurrentType!=AudioPlayInfo.Type )||(VolumeChangeFlag==1))
			{
				AudioPlayCurrentType=AudioPlayInfo.Type;
				AduioTypeVolume();
				VolumeChangeFlag=0;
			}

			AudioWriteFrontData((u8*)(&(pda->DaBuf)));

		}
		else
		{
			if(pda->Priority < AudioPlayInfo.Priority)
			{
				AudioPlayInfo.EnableFlag = 10;
				AudioPlayInfo.SourceType = source_type;
				AudioPlayInfo.SourceId = source_id;
				AudioPlayInfo.Priority = pda->Priority;
				AudioPlayInfo.Type = pda->Type;
				if((AudioPlayCurrentType!=AudioPlayInfo.Type )||(VolumeChangeFlag==1))
				{
					AudioPlayCurrentType=AudioPlayInfo.Type;
					AduioTypeVolume();
					VolumeChangeFlag=0;
				}
				AudioWriteFrontData((u8*)(&(pda->DaBuf)));

			}
#if 1
			else if(pda->Priority == AudioPlayInfo.Priority)
			{
				if((AudioPlayInfo.SourceType == source_type) && (AudioPlayInfo.SourceId == source_id))
				{
					AudioPlayInfo.EnableFlag = 10;
					/*if(AudioPlayCurrentType!=AudioPlayInfo.Type = pda->Type)
					{
						AudioPlayCurrentType=AudioPlayInfo.Type = pda->Type;
						//AduioVolumeSet();
					}*/
                    AduioTypeVolume();
					AudioWriteFrontData((u8*)(&(pda->DaBuf)));

				}

			}
#endif

		}

	}



	return 0;

}




int NetBroadcastSendProcess(void)
{


	return 0;

}



int NetTalkReceiveProcess(u8* pbuf, int len)
{



	return 0;


}


int NetTalkSend(u8* pbuf, int len)
{



	return 0;


}

int NetTalkTestLeftSend(u8* pbuf, int len)
{

	ProtocolSndGetTalkTestLeftPack(pNetSendBuf, &NetSendBufSize, pbuf, len);

	UdpTalkSend(pNetSendBuf, NetSendBufSize);

	return 0;

}

int NetTalkTestRightSend(u8* pbuf, int len)
{

	ProtocolSndGetTalkTestRightPack(pNetSendBuf, &NetSendBufSize, pbuf, len);

	UdpTalkSend(pNetSendBuf, NetSendBufSize);

	return 0;

}






int NetPlayclkReceiveProcess(u8* pbuf, int len)
{

	u32 keyword;
	u8 src_id;

	pNetReceiveBuf = pbuf;
	NetReceiveBufSize = len;


	if(ProtocolRcvHeadCheck(pNetReceiveBuf) != 0)
	{
		return -1;

	}

	keyword = ProtocolRcvGetKeyword(pNetReceiveBuf);
	if(keyword == PRTCL_KEYWORD_PLAYCLK)
	{
		src_id = ProtocolRcvGetSrcDevId(pNetReceiveBuf);


		if(src_id < DevId)
		{
			PlayClockRcvFlag ++;
			if(PlayClockRcvFlag > 128)
			{
				PlayClockRcvFlag = 128;

			}

		}


	}


	return 0;

}



int NetPlayclkSendProcess(void)
{

	if(PlayClockSendFlag == 1)
	{

		ProtocolSndGetPlayclkPack(pNetSendBuf, &NetSendBufSize, PlayClock);


		UdpPlayclkSend(pNetSendBuf, NetSendBufSize);

		PlayClockSendFlag = 0;


	}



	return 0;

}


int NetGetBaseIp(u32 cc_ip)		//this is new for ningbo2
{
	u8 cc_ip1;
	u8 cc_ip2;
	u8 cc_ip3;
	u8 cc_ip4;

	u8 old_cc_ip1;
	u8 old_cc_ip2;
	u8 old_cc_ip3;
	u8 old_cc_ip4;


	cc_ip1 = (u8)(cc_ip>>24);
	cc_ip2 = (u8)(cc_ip>>16);
	cc_ip3 = (u8)(cc_ip>>8);
	cc_ip4 = (u8)(cc_ip);


	if(cc_ip1 < 128)
	{
		return -1;
	}
	if(cc_ip2 > 247)
	{
		return -1;
	}
	if(cc_ip3 > 255)
	{
		return -1;
	}
	if(cc_ip4 > 90)
	{
		return -1;
	}


	old_cc_ip1 = NetLocalIp[0];
	old_cc_ip2 = NetLocalIp[1];
	old_cc_ip3 = NetLocalIp[2];

	if(DevType == PRTCL_DEVTYPE_AMP)
	{
		old_cc_ip4 = NetLocalIp[3] - DevId- 142;

	}
	else if(DevType == PRTCL_DEVTYPE_DCP)
	{
		old_cc_ip4 = NetLocalIp[3] - DevId- 2;

	}
	else if(DevType == PRTCL_DEVTYPE_EHP)
	{
		old_cc_ip4 = NetLocalIp[3] - DevId- 92;

	}


	if((cc_ip1==old_cc_ip1) && (cc_ip2==old_cc_ip2) && (cc_ip3==old_cc_ip3) && (cc_ip4==old_cc_ip4))
	{
		return 1;

	}
	else
	{
		NetBaseIp[0] = cc_ip1;
		NetBaseIp[1] = cc_ip2;
		NetBaseIp[2] = cc_ip3;

		if(DevType == PRTCL_DEVTYPE_AMP)
		{
			NetBaseIp[3] = cc_ip4 + 142;

		}
		else if(DevType == PRTCL_DEVTYPE_DCP)
		{
			NetBaseIp[3] = cc_ip4 + 2;

		}
		else if(DevType == PRTCL_DEVTYPE_EHP)
		{
			NetBaseIp[3] = cc_ip4 + 92;

		}

		StorageInfoSave();

	}


	return 0;

}


int NetSetBaseIp(void)		//this is new for ningbo2
{

	StorageInfoRead();

	if(NetBaseIp[0] < 128)
	{
		NetBaseIp[0] = 192;
	}
	if(NetBaseIp[1] > 247)
	{
		NetBaseIp[0] = 168;
	}
	if(NetBaseIp[2] > 255)
	{
		NetBaseIp[2] = 101;
	}
	if(NetBaseIp[3] < 2)
	{
		NetBaseIp[3] = 100;
	}

	return 0;

}

int NetGetCcvcState(void)
{
	u8* pda;
	ProtocolCcvcStateTypeDef* ppack;
	pda = (u8*)(ProtocolRcvGetDataPointer(pNetReceiveBuf));
	ppack=(ProtocolCcvcStateTypeDef*)pda;

	if(ppack->Ccvc_active==1)
	{
		CcState=ppack->Ccvc_st;
		CcDbSet=ppack->Ccvc_volum;
		if(CcState==CCVC_ST_CC_AC)
		{
			if(PcDbSet==PCSETMUTE)
			{
				if(VolumeToalDb!=CCVC_AUDIO_MUTE)
				{
					AudioSetVolume(CCVC_AUDIO_MUTE);
					VolumeToalDb=CCVC_AUDIO_MUTE;
				}
			}
			NoiseFaultFun=0;

		}
		else if(CcState==CCVC_ST_CC)
		{
			if(PcDbSet==PCSETMUTE)
			{
				if(VolumeToalDb!=CCVC_AUDIO_MUTE)
				{
					AudioSetVolume(CCVC_AUDIO_MUTE);
					VolumeToalDb=CCVC_AUDIO_MUTE;
				}
			}
			NoiseFaultFun=1;
		}
		else  if(CcState==CCVC_ST_MC)
		{
			if(PcDbSet==PCSETMUTE)
			{
				if(VolumeToalDb!=CCVC_AUDIO_MUTE)
				{
					AudioSetVolume(	CCVC_AUDIO_MUTE);
					VolumeToalDb=CCVC_AUDIO_MUTE;

				}
			}
			else
			{
				CcDbSet=ppack->Ccvc_volum;
				if((CcDbSet!=VolumeToalDb)||(PcDbSet!=PcDbSet_Tmp))
				{
					VolumeToalDb=CcDbSet;
					VolumeChangeFlag=1;
					PcDbSet_Tmp=PcDbSet;
				}
			}
			NoiseFaultFun=1;

		}
		else  if(CcState==CCVC_ST_MC_AC)
		{

			if(PcDbSet==PCSETMUTE)
			{
				if(VolumeToalDb!=CCVC_AUDIO_MUTE)
				{
					AudioSetVolume(CCVC_AUDIO_MUTE);
					VolumeToalDb=CCVC_AUDIO_MUTE;

				}

			}
			else
			{
				CcDbSet=ppack->Ccvc_volum;
				if((CcDbSet!=VolumeToalDb)||(PcDbSet!=PcDbSet_Tmp))
				{
					VolumeToalDb=CcDbSet;
					VolumeChangeFlag=1;
					PcDbSet_Tmp=PcDbSet;

				}

			}
			NoiseFaultFun=0;

		}
		else  if(CcState==CCVC_ST_MUTE)
		{
			if(VolumeToalDb!=CCVC_AUDIO_MUTE)
			{
				AudioSetVolume(CCVC_AUDIO_MUTE);
				VolumeToalDb=CCVC_AUDIO_MUTE;

			}
			NoiseFaultFun=1;

		}

	}

	return 0;
}

int NetGetPcvcState(void)
{

	u8* pda;
	ProtocolPcvcStateTypeDef* ppack;
	pda = (u8*)(ProtocolRcvGetDataPointer(pNetReceiveBuf));
	ppack=(ProtocolPcvcStateTypeDef*)pda;
	PcDbSet=ppack->Pcvc_volum;
	if(PcDbSet==1)
	{
		PcDbSet=-3;		
	}
	else if(PcDbSet==2)
	{
		PcDbSet=-6;	
	}
	else if(PcDbSet==3)
	{
		PcDbSet=3;	
	}
	else if(PcDbSet==4)
	{
		PcDbSet=6;	
	}
	return 0;



}

int NetDownloadReceiveProcess(u8* pbuf, int len)
{
	u32 kw;
	u8* pda;
//	ProtocolHeadTypeDef *phead;
//	ProtocolDeviceInfoTypeDef *psrcdev;
//	int src_ip;
	int offset;

	pNetReceiveBuf = pbuf;
	NetReceiveBufSize = len;

	if(ProtocolRcvHeadCheck(pNetReceiveBuf) != 0)
	{
		return -1;
	}
	kw = ProtocolRcvGetKeyword(pNetReceiveBuf);	
//	phead = (ProtocolHeadTypeDef*)(pNetReceiveBuf);
//	psrcdev = (ProtocolDeviceInfoTypeDef*)(&(phead->SrcInfo));
	pda = (u8*)(ProtocolRcvGetDataPointer(pNetReceiveBuf));

	if(kw == PRTCL_KW_DOWNLOAD_APP)
	{
		NetDownloadStarted = 1;
		NetDownloadBlocks = ((*pda) * 256) + (*(pda+1)); 
		NetDownloadBlockNum = ((*(pda+2)) * 256) + (*(pda+3)); 

		if(NetDownloadBlockNum == 0)
		{
			FlashInit();
		}

		offset = NetDownloadBlockNum*1024;	
		NetDownloadReceiveAppDataSave(offset, pda+4, 1024);

		NetDownloadSendAppAck();

		if(NetDownloadBlockNum == NetDownloadBlocks)
		{
			Delay(10000);
			//flashcopy;
			FlashProgramCopy(offset + 4);
		}

	}

	return 0;

}




void NetDownloadReceiveAppDataSave(int offset, u8 *pbuf, int len)
{
	int len32;

	len32 = len / 4;
	if((len % 4) > 0) {
		len32 += 1;
	}
	FlashWrite(offset, ((u32 *)(pbuf)), len32);

}



int NetDownloadSendAppAck(void)
{
	u8 dabuf[4];
	int dalen = 4;
	u16 kw = PRTCL_KW_DOWNLOAD_APP_ACK;

	dabuf[0] = (u8)(NetDownloadBlocks >> 8);
	dabuf[1] = (u8)(NetDownloadBlocks);
	dabuf[2] = (u8)(NetDownloadBlockNum >> 8);
	dabuf[3] = (u8)(NetDownloadBlockNum);

	ProtocolSndGetPack(pNetSendBuf, &NetSendBufSize, kw, dabuf, dalen);
	
	UdpDownloadSend(pNetSendBuf, NetSendBufSize);


	return 0;

}



