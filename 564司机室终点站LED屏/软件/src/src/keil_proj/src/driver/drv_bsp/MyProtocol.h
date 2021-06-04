//*******************************************************//
//						MyProtocol.h								//
//*******************************************************//

#ifndef _MYPROTOCOL_H
#define  _MYPROTOCOL_H

#include "MyAll.h"

#define	DATA_MAX_SIZE	1056

#define PRTCL_HEADS_LEN	21	


#define PRTCL_DEVTYPE_CC				0x01		//Center Controller
#define PRTCL_DEVTYPE_DCP	 		0x02		//Control Box
#define PRTCL_DEVTYPE_VIFU 			0x03		//Vehicle Interface Unit
#define PRTCL_DEVTYPE_RECON	 		0x04		//Reconnect Unit
#define PRTCL_DEVTYPE_RECORDER	 	0x05
#define PRTCL_DEVTYPE_RESERVE		0x06
#define PRTCL_DEVTYPE_DMP			0x07		//Dynamic Map
#define PRTCL_DEVTYPE_EHP			0x08		//Emergency Help Point
#define PRTCL_DEVTYPE_CLED			0x09		//Carriage LED
#define PRTCL_DEVTYPE_LLED			0x0A		//Locomotive LED
#define PRTCL_DEVTYPE_AMP			0x0B		//Amplifier
#define PRTCL_DEVTYPE_SWITCH		0x0C
#define PRTCL_DEVTYPE_PTU			0x0D

#define	PIS_PACKET_HEAD	0x7e

//控制
#define PRTCL_KEYWORD_INSPECTION		0x0001
#define PRTCL_KEYWORD_VOLUME			0x0002
#define PRTCL_KEYWORD_PLAYCLK			0x0003

#define PRTCL_KEYWORD_BROADCAST		0x0004
#define PRTCL_KEYWORD_TALK_DD			0x0005
#define PRTCL_KEYWORD_TALK_DC			0x0005

//控制
#define Ctrl_KEYWORD_ProcessData				0x0001


//返回
#define FeedBack_KEYWORD_ip_mac_id				0x0301
#define FeedBack_KEYWORD_softwareVersion		0x0333

//设置
#define Set_KEYWORD_mac					0x0130
#define Set_KEYWORD_IP						0x0101


//回读
#define Get_KEYWORD_ip_mac_id				0x0201
#define Get_KEYWORD_softwareVersion		0x0233




//__packed typedef struct 




__packed typedef struct 
{

	u16 TrainId;
	u8 DevType;
	u8 DevId;
	u32 Ip32;


}ProtocolDeviceInfoTypeDef;



__packed typedef struct 
{
	u8 HeadMark;

	ProtocolDeviceInfoTypeDef DestInfo;

	ProtocolDeviceInfoTypeDef SrcInfo;

	
}ProtocolHeadTypeDef;


__packed typedef struct 
{
	u8 Buf[DATA_MAX_SIZE];

	
}ProtocolDataTypeDef;



__packed typedef struct 
{

	ProtocolHeadTypeDef Head;

	u16 KeyWord;

	u16 DataLength;

	ProtocolDataTypeDef DataBuf;

	
}ProtocolPackTypeDef;




__packed typedef struct 
{

	u8 Priority;

	u8 DaBuf[1024];

	
}ProtocolAudioDataTypeDef;



__packed typedef struct 
{

	u8 Others[5];

	u8 Amp[16];

	u8 EhpPhone;
	u8 EhpSpeaker;

	u8 Dva;

	u8 Reserve[6];

	
}ProtocolControlVolumeTypeDef;





int ProtocolInit(void);


int ProtocolRcvHeadCheck(u8* pbuf);


u8 ProtocolRcvGetSrcDevType(u8* pbuf);

u8 ProtocolRcvGetSrcDevId(u8* pbuf);

u8 ProtocolRcvGetSrcStation(u8* pbuf);


u16 ProtocolRcvGetKeyword(u8* pbuf);

int ProtocolRcvGetDataLength(u8* pbuf);

u8* ProtocolRcvGetDataPointer(u8* pbuf);

u8 ProtocolRcvGetControlVolume(u8* pbuf, int len, u8 dev_type, u8 dev_id );




ProtocolHeadTypeDef ProtocolHeadMake(ProtocolDeviceInfoTypeDef remote_info);


int ProtocolSndGetInspectionPack(u8 * pbuf, int * plen, u8 priority, u8 state);

int ProtocolSndGetLocalIpPack(u8* pbuf, int* plen, u32 localip);

int ProtocolSndGetDevIdPack(u8* pbuf, int* plen, u8 inda);

int ProtocolSndGetLinePack(u8* pbuf, int* plen, u8 inda);

int ProtocolSndGetStationPack(u8* pbuf, int* plen, u8 inda);

int ProtocolSndGetAreaPack(u8* pbuf, int* plen, u8 inda);

int ProtocolSndGetPlayclkPack(u8* pbuf, int* plen, u32 inda);

int ProtocolSndGetVolumePack(u8* pbuf, int* plen, u8 inda);

int ProtocolSndGetTreblePack(u8* pbuf, int* plen, u8 inda);

int ProtocolSndGetBassPack(u8* pbuf, int* plen, u8 inda);

int ProtocolSndGetAudioListenPack(u8* pbuf, int* plen, u8* inbuf, int inlen);






extern ProtocolDeviceInfoTypeDef ProtocolLocalInfo;
extern ProtocolDeviceInfoTypeDef ProtocolBroadInfo;


#endif


