//*******************************************************//
//						MyAudio.h								//
//*******************************************************//

#ifndef _MYAUDIO_H
#define  _MYAUDIO_H

#include "MyAll.h"





#define PLAY_PACK_SIZE	1		//1024
#define PLAY_PACK_NUM	1		//8


#define LISTEN_PACK_SIZE	1024
#define LISTEN_PACK_NUM	4





typedef struct 
{
	int EnableFlag;

	u8 SourceType;
	u8 SourceId;

	u16 Priority;

	
}AudioPlayInfoTypeDef;



extern AudioPlayInfoTypeDef AudioPlayInfo;



extern u8 PlayLeftFifo[PLAY_PACK_SIZE * PLAY_PACK_NUM];
extern u8* pPlayLeftFifo;
extern int PlayLeftProduceIndex;
extern int PlayLeftConsumeIndex;
extern int PlayLeftWaitFlag;		//used for clear the audio fifor automatically


extern u8 PlayRightFifo[PLAY_PACK_SIZE * PLAY_PACK_NUM];
extern u8* pPlayRightFifo;
extern int PlayRightProduceIndex;
extern int PlayRightConsumeIndex;
extern int PlayRightWaitFlag;		//used for clear the audio fifor automatically



extern u8 ListenLeftFifo[LISTEN_PACK_SIZE * LISTEN_PACK_NUM];
extern u8* pListenLeftFifo;
extern int ListenLeftProduceIndex;
extern int ListenLeftConsumeIndex;


extern u8 ListenRightFifo[LISTEN_PACK_SIZE * LISTEN_PACK_NUM];
extern u8* pListenRightFifo;
extern int ListenRightProduceIndex;
extern int ListenRightConsumeIndex;


extern int ListenFlag;
extern int ListenEnable;





extern int AudioVolume;
extern int AudioTreble;
extern int AudioBass;



extern int AudioFrontPlayFlag;
extern int AudioBackPlayFlag;




extern u32 PlayClock;
extern int PlayClockSendFlag;





void AudioInit(void);

void AudioProcess(void);

void Audio100ms(void);



int AudioWriteFrontData(u8* pbuf);

int AudioWriteBackData(u8* pbuf);



void AudioListenSendProcess(void);



int AudioSetVolume(int inda);





#endif


