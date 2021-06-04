//*******************************************************//
//						MyLwip.h								//
//*******************************************************//

#ifndef _MYLWIP_H
#define  _MYLWIP_H

#include "MyAll.h"



#define PORT_TEST_RCV_L				15202
#define PORT_TEST_SND_L				15203

#define PORT_TEST_RCV_R				15202
#define PORT_TEST_SND_R				15203

#define PORT_COMMANDER_L		15200
#define PORT_COMMANDER_R		15201




#define PORT_CONTROL_SND_L		50151
#define PORT_CONTROL_SND_R		50152

#define PORT_CONTROL_RCV_L		50152
#define PORT_CONTROL_RCV_R		50151


#define PORT_BROADCAST_SND_L	51151
#define PORT_BROADCAST_SND_R	51152

#define PORT_BROADCAST_RCV_L	51152
#define PORT_BROADCAST_RCV_R	51151


#define PORT_TALK_SND_L			52151
#define PORT_TALK_SND_R			52152

#define PORT_TALK_RCV_L			52152
#define PORT_TALK_RCV_R			52151


#define PORT_PLAYCLK_SND_L		53151
#define PORT_PLAYCLK_SND_R		53152

#define PORT_PLAYCLK_RCV_L		53152
#define PORT_PLAYCLK_RCV_R		53151






extern u32 LocalTime;



void LwipInit(void);

void LwipProcess(void);

void Lwip50ms(void);

void LwipPktHandle(void);

void LwipPeriodicHandle(void);





void UdpTestRcvInit(void);
void UdpTestSendInit(void);
void UdpTestSend(u8* pbuf, int len);


void UdpCommanderInit(void);
void UdpCommanderSend(u8* pbuf, int len);



void UdpControlRcvInit(void);

void UdpControlSendInit(void);
void UdpControlSend(u8* pbuf, int len);


void UdpBroadcastRcvInit(void);

void UdpBroadcastSendInit(void);
void UdpBroadcastSend(u8* pbuf, int len);



void UdpTalkRcvInit(void);

void UdpTalkSendInit(void);
void UdpTalkSend(u8* pbuf, int len);



void UdpPlayclkRcvInit(void);

void UdpPlayclkSendInit(void);
void UdpPlayclkSend(u8* pbuf, int len);








#endif


