
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



//u32 CentralController_IP32;
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




int NetInit()
{
	u32 temp;
	

	Delay(10000);
/*
	NetLocalMac[0] = 0x5e;
	NetLocalMac[1] = 0x45;
	NetLocalMac[2] = 0xa2;
	NetLocalMac[3] = 0xa8;	//168
	NetLocalMac[4] = 0x65;	//101
	NetLocalMac[5] = DevId;	//100
	*/

/*
	NetLocalMac[0] = 12;		//年
	NetLocalMac[1] = 8;		//月
	NetLocalMac[2] = 12;		//日
	NetLocalMac[3] = 13;		//时
	NetLocalMac[4] = 23;		//分
	NetLocalMac[5] = 55;		//秒
*/
	NetLocalIp[0] = 192;
	NetLocalIp[1] = 168;
	NetLocalIp[2] = 101;

	//if (!check_devid()) {
		NetLocalIp[3] = 2 + DevId + IP_offset - FirstHLED_ID;	// 1#中央控制器默认ip为2
	//}
	//else {
//		NetLocalIp[3] = 100;
	//}
	
	NetRouterIp[0] = NetLocalIp[0] ;
	NetRouterIp[1] = NetLocalIp[1] ;
	NetRouterIp[2] = NetLocalIp[2] ;
	NetRouterIp[3] = 1;

	NetLocalMac[0] = 0x5e;
	NetLocalMac[1] = 0x45;
	NetLocalMac[2] = 0xa2;
	NetLocalMac[3] = NetLocalIp[1] ;
	NetLocalMac[4] = NetLocalIp[2] ;
	NetLocalMac[5] = NetLocalIp[3] ;

	NetMaskIp[0] = 255;
	NetMaskIp[1] = 255;
	NetMaskIp[2] = 255;
	NetMaskIp[3] = 0;
	

	StorageInfoRead();


	//NetLocalMac[3] = NetLocalIp[1];
	//NetLocalMac[4] = NetLocalIp[2];
	//NetLocalMac[5] = NetLocalIp[3];


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
		
	NetPlayclkSendProcess();

}



void Net50ms(void)
{

	

}



void Net500ms(void)
{

	NetControlSendInspection();
	

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

	turn_off_screen_clrbuf();

	NetLocalIp[0] = ip1;
	NetLocalIp[1] = ip2;
	NetLocalIp[2] = ip3;
	NetLocalIp[3] = ip4;

	
	Delay(1000);
	StorageInfoSave();
	Delay(1000);
	StorageInfoSaveProcess();
	Delay(1000);
	Delay(1000);
	

	Delay(100000);
	Delay(100000);	
	NVIC_SystemReset();
	Delay(10000);
	Delay(10000);
	

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
	//u32 keyword;


	pNetReceiveBuf = pbuf;
	NetReceiveBufSize = len;


	if(ProtocolRcvHeadCheck(pNetReceiveBuf) != 0)
	{
		return -1;

	}

	udp_CtrlMsg_receive_process(pNetReceiveBuf);


/*
	keyword = ProtocolRcvGetKeyword(pNetReceiveBuf);
	
	switch(keyword)
	{
		case PRTCL_KEYWORD_VOLUME :
			{
				NetControlReceiveVolume();
				
				break;
			}
		case PRTCL_KEYWORD_PLAYCLK:
				{

			break;
		}
	

		////检测其他设备心跳
//		case PRTCL_KEYWORD_INSPECTION_SND:
			{
//				NetControlReceiveInspection();
				
//				break;
			}

		
			
		default :
			{


			}

			

	}
	*/
	
	
	return 0;
	
}


int NetControlReceiveVolume(void)
{
	u8 volume;

	volume = ProtocolRcvGetControlVolume(pNetReceiveBuf, NetReceiveBufSize, DevType, DevId);

	if(volume < 100)
	{
		AudioSetVolume(volume);

	}

	return 0;

}




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

	ProtocolSndGetInspectionPack(pNetSendBuf, &NetSendBufSize, priority, 0);

	UdpControlSend(pNetSendBuf, NetSendBufSize);


	return 0;

}





////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////



int NetBroadcastReceiveProcess(u8* pbuf, int len)
{
	u32 keyword;
	u8 source_type;
	u8 source_id;

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
	

	if(keyword == PRTCL_KEYWORD_BROADCAST)
	{

	
		pda = (ProtocolAudioDataTypeDef*)(ProtocolRcvGetDataPointer(pNetReceiveBuf));

		if(AudioPlayInfo.EnableFlag < 1)
		{
			AudioPlayInfo.EnableFlag = 10;
			AudioPlayInfo.SourceType = source_type;
			AudioPlayInfo.SourceId = source_id;
			AudioPlayInfo.Priority = pda->Priority;
			
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
			
				AudioWriteFrontData((u8*)(&(pda->DaBuf)));
			
			}
			else if(pda->Priority == AudioPlayInfo.Priority)
			{
				if((AudioPlayInfo.SourceType == source_type) && (AudioPlayInfo.SourceId == source_id))
				{
					AudioPlayInfo.EnableFlag = 10;
				
					AudioWriteFrontData((u8*)(&(pda->DaBuf)));
					
				}

			}

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
//			PlayClockRcvFlag ++;
//			if(PlayClockRcvFlag > 72)
			{
//				PlayClockRcvFlag = 72;

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














