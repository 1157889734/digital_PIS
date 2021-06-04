
//*****************************************************//
//		MyProtocol.c
//*****************************************************//

#include "MyAll.h"


ProtocolDeviceInfoTypeDef ProtocolLocalInfo;
ProtocolDeviceInfoTypeDef ProtocolBroadInfo;



int ProtocolInit(void)
{

	ProtocolLocalInfo.DevType = DevType;
	ProtocolLocalInfo.DevId =DevId;
	ProtocolLocalInfo.TrainId =TrainId;
	ProtocolLocalInfo.Ip32 = NetIp4toIp32(NetLocalIp);



	ProtocolBroadInfo.DevType = 0xff;
	ProtocolBroadInfo.DevId =0xff;
	ProtocolBroadInfo.TrainId =0xffff;
	ProtocolBroadInfo.Ip32 = NetIp4toIp32(NetLocalBroadcastIp);




	return 0;

}





int ProtocolRcvHeadCheck(u8* pbuf)
{

	ProtocolHeadTypeDef* phead;


	phead = (ProtocolHeadTypeDef*)(pbuf);


	if(phead->HeadMark != 0x7E)
	{
		return -1;
	}


	if((phead->DestInfo.DevType != DevType) && (phead->DestInfo.DevType != 0xFF))
	{
		return -1;
	}

	if((phead->DestInfo.DevId != DevId) && (phead->DestInfo.DevId  != 0xFF))
	{
		return -1;
	}

	if((phead->DestInfo.Ip32 != NetIp4toIp32(NetLocalIp)) && (phead->DestInfo.Ip32!= 0xFFFFFFFF) && (phead->DestInfo.Ip32!= (NetIp4toIp32(NetLocalIp) | 0x000000FF)))
	{
		return -1;
	}


	return 0;

}



u8 ProtocolRcvGetSrcDevType(u8* pbuf)
{
	u8 src_type;

	ProtocolHeadTypeDef* phead;


	phead = (ProtocolHeadTypeDef*)(pbuf);

	src_type = (phead->SrcInfo).DevType;

	return src_type;

}



u8 ProtocolRcvGetDstTrainId(u8* pbuf)
{
	u8 src_id;

	ProtocolHeadTypeDef* phead;


	phead = (ProtocolHeadTypeDef*)(pbuf);

	src_id = (phead->DestInfo).TrainId;

	return src_id;

}


u8 ProtocolRcvGetSrcDevId(u8* pbuf)
{
	u8 src_id;

	ProtocolHeadTypeDef* phead;


	phead = (ProtocolHeadTypeDef*)(pbuf);

	src_id = (phead->SrcInfo).DevId;

	return src_id;

}



u16 ProtocolRcvGetKeyword(u8* pbuf)
{
	u16 keyword;

	ProtocolPackTypeDef* ppack;


	ppack = (ProtocolPackTypeDef*)(pbuf);

	keyword = ppack->KeyWord;


	return(keyword);

}

int ProtocolRcvGetDataLength(u8* pbuf)
{
	int len;

	ProtocolPackTypeDef* ppack;


	ppack = (ProtocolPackTypeDef*)(pbuf);

	len = ppack->DataLength;


	return(len);

}

u8* ProtocolRcvGetDataPointer(u8* pbuf)
{
	u8* pdata;

	ProtocolPackTypeDef* ppack;


	ppack = (ProtocolPackTypeDef*)(pbuf);

	pdata = (u8*)(&(ppack->DataBuf));


	return(pdata);

}


u8 ProtocolRcvGetControlVolume(u8* pbuf, int len, u8 dev_type, u8 dev_id )
{
	u8* pdata;

	ProtocolPackTypeDef* ppack;

	ProtocolControlVolumeTypeDef* p_crl_volume;


	ppack = (ProtocolPackTypeDef*)(pbuf);

	pdata = (u8*)(&(ppack->DataBuf));

	p_crl_volume = (ProtocolControlVolumeTypeDef*)(pdata);

	if(dev_type == PRTCL_DEVTYPE_AMP)
	{
		if((CcState==CCVC_ST_CC_AC)||(CcState==CCVC_ST_CC))
		{

			if(p_crl_volume->Amp[dev_id-1]<=40)
			{
				if(PcDbSet==PCSETMUTE)
				{
					AudioSetVolume(CCVC_AUDIO_MUTE);
					VolumeToalDb=CCVC_AUDIO_MUTE;
				
				}
				else if(VolumeToalDb!=p_crl_volume->Amp[dev_id-1])
				{			
					VolumeToalDb=p_crl_volume->Amp[dev_id-1];
					VolumeChangeFlag=1;
					
				}

			}

		}
		if(p_crl_volume->OccVolume<=40)
		{
			if(VolumeOccDb!=p_crl_volume->OccVolume)
			{
				VolumeOccDb=p_crl_volume->OccVolume;
				VolumeChangeFlag=1;
			}

		}
		if(p_crl_volume->MediaVolume<=40)
		{
			if(VolumeMediaDb!=p_crl_volume->MediaVolume)
			{
				VolumeMediaDb=p_crl_volume->MediaVolume;
				VolumeChangeFlag=1;

			}

		}
		if(p_crl_volume->BroadcatVolume<=40)
		{

			if(VolumeBroadcastDb!=p_crl_volume->BroadcatVolume)
			{
				VolumeBroadcastDb=p_crl_volume->BroadcatVolume;
				VolumeChangeFlag=1;
			}

		}
		if(p_crl_volume->DoorVolume<=40)
		{
			if(VolumeDoorDb!=p_crl_volume->DoorVolume)
			{

				VolumeDoorDb=p_crl_volume->DoorVolume;
				VolumeChangeFlag=1;
			}
		}
		if(p_crl_volume->EmergencyVolume<=40)
		{
			if(VolumeEemergencyDb!=p_crl_volume->EmergencyVolume)
			{
				VolumeEemergencyDb=p_crl_volume->EmergencyVolume;
				VolumeChangeFlag=1;
			}


		}
		if(p_crl_volume->TestVolume<=40)
		{

			if(VolumeTestDb!=p_crl_volume->TestVolume)
			{
				VolumeTestDb=p_crl_volume->TestVolume;
				VolumeChangeFlag=1;
			}


		}
		if(p_crl_volume->MicVoulume<=40)
		{

			if(VolumeMicDb!=p_crl_volume->MicVoulume)
			{
				VolumeMicDb=p_crl_volume->MicVoulume;
				VolumeChangeFlag=1;
			}


		}


		return(VolumeToalDb);

	}
	else
	{

		return(255);
	}

}






ProtocolHeadTypeDef ProtocolHeadMake(ProtocolDeviceInfoTypeDef remote_info)
{
	ProtocolHeadTypeDef  head;


	ProtocolLocalInfo.DevType = DevType;
	ProtocolLocalInfo.DevId = DevId;
	ProtocolLocalInfo.TrainId = TrainId;
	ProtocolLocalInfo.Ip32 = NetIp4toIp32(NetLocalIp);


	head.HeadMark = 0x7E;

	head.DestInfo = remote_info;

	head.SrcInfo = ProtocolLocalInfo;


	return head;
}




int ProtocolSndGetInspectionPack(u8* pbuf, int* plen, u8 priority, u8 state, u8 volume, u8 version)
{

	ProtocolPackTypeDef* ppack;

	u8* pdata;


	ppack= (ProtocolPackTypeDef *)pbuf;


	ppack->Head = ProtocolHeadMake(ProtocolBroadInfo);

	ppack->KeyWord = PRTCL_KEYWORD_INSPECTION;

	ppack->DataLength = 8;


	pdata = (u8*)(&(ppack->DataBuf));

	*pdata = priority;
	*(pdata+1) = state;
	*(pdata+2) = volume;
	*(pdata+3) = NoiseDB;
	*(pdata+4) = version;
	*(pdata+5) = AppVersion2;
	*(pdata+6) = AppVersion3;
	*(pdata+7) = AppVersion4;


	*plen = PRTCL_HEADS_LEN + (ppack->DataLength);


	return 0;

}


int ProtocolSndGetLocalIpPack(u8* pbuf, int* plen, u32 localip)
{

	ProtocolPackTypeDef* ppack;

	u32* plocalip;


	ppack= (ProtocolPackTypeDef *)pbuf;


//	ppack->Head = ProtocolHeadMake(ProtocolStationBroadInfo);

//	ppack->KeyWord = PRTCL_KEYWORD_LOCALIP_SND;

	ppack->DataLength = 4;


	plocalip = (u32*)(&(ppack->DataBuf));

	*plocalip = localip;


	*plen = PRTCL_HEADS_LEN + (ppack->DataLength);


	return 0;

}


int ProtocolSndGetDevIdPack(u8* pbuf, int* plen, u8 inda)
{

	ProtocolPackTypeDef* ppack;

	u8* pda;


	ppack= (ProtocolPackTypeDef *)pbuf;


//	ppack->Head = ProtocolHeadMake(ProtocolStationBroadInfo);

//	ppack->KeyWord = PRTCL_KEYWORD_ID_SND;

	ppack->DataLength = 1;


	pda = (u8*)(&(ppack->DataBuf));

	*pda = inda;


	*plen = PRTCL_HEADS_LEN + (ppack->DataLength);


	return 0;

}




int ProtocolSndGetPlayclkPack(u8* pbuf, int* plen, u32 inda)
{

	ProtocolPackTypeDef* ppack;

	u32* pda;


	ppack= (ProtocolPackTypeDef *)pbuf;


	ppack->Head = ProtocolHeadMake(ProtocolBroadInfo);

	ppack->KeyWord = PRTCL_KEYWORD_PLAYCLK;

	ppack->DataLength = 4;


	pda = (u32*)(&(ppack->DataBuf));

	*pda = inda;


	*plen = PRTCL_HEADS_LEN + (ppack->DataLength);


	return 0;

}

int ProtocolSndGetVolumePack(u8* pbuf, int* plen, u8 inda)
{

	ProtocolPackTypeDef* ppack;

	u8* pda;


	ppack= (ProtocolPackTypeDef *)pbuf;


//	ppack->Head = ProtocolHeadMake(ProtocolStationBroadInfo);

//	ppack->KeyWord = PRTCL_KEYWORD_VOLUME_SND;

	ppack->DataLength = 2;


	pda = (u8*)(&(ppack->DataBuf));

//	*pda = Area;

	*(pda+1) = inda;


	*plen = PRTCL_HEADS_LEN + (ppack->DataLength);


	return 0;

}




int ProtocolSndGetTalkDcPack(u8* pbuf, int* plen, u8* inbuf, int inlen)
{

	ProtocolPackTypeDef* ppack;

	u8* pda;


	ppack= (ProtocolPackTypeDef *)pbuf;


	ppack->Head = ProtocolHeadMake(ProtocolBroadInfo);

	ppack->KeyWord = PRTCL_KEYWORD_TALK_DC;

	ppack->DataLength = 1024;


	pda = (u8*)(&(ppack->DataBuf));

	memcpy((pda), inbuf, inlen);



	*plen = PRTCL_HEADS_LEN + (ppack->DataLength);


	return 0;


}



int ProtocolSndGetTalkDdPack(u8* pbuf, int* plen, u8* inbuf, int inlen)
{

	ProtocolPackTypeDef* ppack;

	u8* pda;


	ppack= (ProtocolPackTypeDef *)pbuf;


	ppack->Head = ProtocolHeadMake(ProtocolBroadInfo);

	ppack->KeyWord = PRTCL_KEYWORD_TALK_DD;

	ppack->DataLength = 1024;


	pda = (u8*)(&(ppack->DataBuf));

	memcpy((pda), inbuf, inlen);



	*plen = PRTCL_HEADS_LEN + (ppack->DataLength);


	return 0;


}




int ProtocolSndGetTalkTestLeftPack(u8* pbuf, int* plen, u8* inbuf, int inlen)
{

	ProtocolPackTypeDef* ppack;

	u8* pda;


	ppack= (ProtocolPackTypeDef *)pbuf;


	ppack->Head = ProtocolHeadMake(ProtocolBroadInfo);

	ppack->KeyWord = PRTCL_KEYWORD_TALK_TEST_LEFT;

	ppack->DataLength = 1024;


	pda = (u8*)(&(ppack->DataBuf));

	memcpy((pda), inbuf, inlen);



	*plen = PRTCL_HEADS_LEN + (ppack->DataLength);


	return 0;


}


int ProtocolSndGetTalkTestRightPack(u8* pbuf, int* plen, u8* inbuf, int inlen)
{

	ProtocolPackTypeDef* ppack;

	u8* pda;


	ppack= (ProtocolPackTypeDef *)pbuf;


	ppack->Head = ProtocolHeadMake(ProtocolBroadInfo);

	ppack->KeyWord = PRTCL_KEYWORD_TALK_TEST_RIGHT;

	ppack->DataLength = 1024;


	pda = (u8*)(&(ppack->DataBuf));

	memcpy((pda), inbuf, inlen);



	*plen = PRTCL_HEADS_LEN + (ppack->DataLength);


	return 0;


}


int ProtocolSndGetSelfTestStatePack(u8* pbuf, int* plen, u8 state)
{

	ProtocolPackTypeDef* ppack;

	u8* pdata;


	ppack= (ProtocolPackTypeDef *)pbuf;


	ppack->Head = ProtocolHeadMake(ProtocolBroadInfo);

	ppack->KeyWord = PRTCL_KEYWORD_SELFTEST_STATE;

	ppack->DataLength = 2;


	pdata = (u8*)(&(ppack->DataBuf));

	*pdata = DevId;
	*(pdata+1) = state;


	*plen = PRTCL_HEADS_LEN + (ppack->DataLength);


	return 0;

}



int ProtocolSndGetSelfTestReportPack(u8* pbuf, int* plen)
{

	ProtocolPackTypeDef* ppack;

	u8* pdata;

	int i;



	ppack= (ProtocolPackTypeDef *)pbuf;


	ppack->Head = ProtocolHeadMake(ProtocolBroadInfo);

	ppack->KeyWord = PRTCL_KEYWORD_SELFTEST_REPORT;

	ppack->DataLength = 37;

	*plen = PRTCL_HEADS_LEN + (ppack->DataLength);


	pdata = (u8*)(&(ppack->DataBuf));

	*pdata = DevId;
	*(pdata+1) = 0x01;
	*(pdata+2) = (u8)SelfTestAmplification;
	*(pdata+3) = (u8)SelfTestSnr;
	*(pdata+4) = (u8)SelfTestOutputPower;

	for(i=0; i<16; i++)
	{
		*(pdata+5+2*i) = (u8)((SelfTestFrequencyResponse[i].Frequency)/100);
		*(pdata+6+2*i) = (u8)(SelfTestFrequencyResponse[i].Value);

	}


	return 0;

}

#if 0
int ProtocolSndGetPack(u8* pbuf, int* plen, u16 keyword, u8 *inda, int inda_len)
{
	ProtocolPackTypeDef* ppack;
	u8* pdata;

	ppack= (ProtocolPackTypeDef *)pbuf;

	ppack->Head = ProtocolHeadMake(ProtocolBroadInfo);
	ppack->KeyWord = keyword;
	ppack->DataLength = inda_len;
	*plen = PRTCL_HEADS_LEN + (ppack->DataLength);

	pdata = (u8*)(&(ppack->DataBuf));

	memcpy(pdata, inda, inda_len);


	return 0;

}
#endif


int ProtocolSndGetPack(u8* pbuf, int* plen, u16 kw, u8* dabuf, int dalen)
{
	ProtocolPackTypeDef* ppack;
	u8* pda;


	ppack= (ProtocolPackTypeDef *)pbuf;
	ppack->Head = ProtocolHeadMake(ProtocolBroadInfo);
	ppack->KeyWord = kw;
	ppack->DataLength = dalen;
	
	pda = (u8*)(&(ppack->DataBuf));
	memcpy((pda), dabuf, dalen);

	*plen = PRTCL_HEADS_LEN + (ppack->DataLength);

	return 0;

}






