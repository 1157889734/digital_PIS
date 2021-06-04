
//*****************************************************//
//		MyAudio.c
//*****************************************************//


#include "MyAll.h"





u8 PlayLeftFifo[PLAY_PACK_SIZE * PLAY_PACK_NUM];
u8* pPlayLeftFifo;
int PlayLeftProduceIndex;
int PlayLeftConsumeIndex;
int PlayLeftWaitFlag;


u8 PlayRightFifo[PLAY_PACK_SIZE * PLAY_PACK_NUM];
u8* pPlayRightFifo;
int PlayRightProduceIndex;
int PlayRightConsumeIndex;
int PlayRightWaitFlag;



u8 ListenLeftFifo[LISTEN_PACK_SIZE * LISTEN_PACK_NUM];
u8* pListenLeftFifo;
int ListenLeftProduceIndex;
int ListenLeftConsumeIndex;

u8 ListenRightFifo[LISTEN_PACK_SIZE * LISTEN_PACK_NUM];
u8* pListenRightFifo;
int ListenRightProduceIndex;
int ListenRightConsumeIndex;



int ListenFlag;
int ListenEnable;


AudioPlayInfoTypeDef AudioPlayInfo;


int AudioVolume;
int AudioTreble;
int AudioBass;



int AudioFrontPlayFlag;
int AudioBackPlayFlag;


u32 PlayClock;
int PlayClockSendFlag;




void AudioInit(void)
{
	int i;

	for(i=0; i<PLAY_PACK_SIZE * PLAY_PACK_NUM; i++)
	{
		PlayLeftFifo[i] = 0;
		PlayRightFifo[i] = 0;

	}


	

	pPlayLeftFifo = PlayLeftFifo;
	pPlayRightFifo = PlayRightFifo;

	PlayLeftConsumeIndex = 0;
	PlayLeftProduceIndex = PLAY_PACK_NUM / 2;

	PlayRightConsumeIndex = 0;
	PlayRightProduceIndex = PLAY_PACK_NUM / 2;



	pListenLeftFifo = ListenLeftFifo;

	ListenLeftConsumeIndex = 0;
	ListenLeftProduceIndex = LISTEN_PACK_NUM / 2;


	pListenRightFifo = ListenRightFifo;

	ListenRightConsumeIndex = 0;
	ListenRightProduceIndex = LISTEN_PACK_NUM / 2;


	ListenFlag = 0;
	ListenEnable = 1;


	AudioPlayInfo.EnableFlag = 0;


	AudioVolume = 10;


	L3InitUda1341();

	Delay(1000);
	
  	I2s2_Init();
  	DmaPlay_Init();

  	Delay(1000);

  	I2s3_Init();
  	DmaListen_Init();

  	Delay(1000);


  	
	Delay(10000);
	Delay(10000);
	StorageInfoRead();
	Delay(10000);
	Delay(10000);


	AudioSetVolume(AudioVolume);



}



void AudioProcess(void)
{

	if(DmaPlayFlag > 0)
	{
		DmaPlayFlag = 0;
		
		PlayLeftWaitFlag ++;

	}



	if((PlayLeftWaitFlag > PLAY_PACK_NUM + 2) && (PlayLeftWaitFlag < PLAY_PACK_NUM + 8))
	{	
		memset(((u8*)pPlayLeftFifo), 0, (PLAY_PACK_SIZE * PLAY_PACK_NUM));

	}
	else if(PlayLeftWaitFlag > 128)
	{
		PlayLeftWaitFlag = 128;
	}
	



}


void Audio100ms(void)
{
	
	AudioPlayInfo.EnableFlag --;
	if(AudioPlayInfo.EnableFlag < 0)
	{
		AudioPlayInfo.EnableFlag = 0;

	}



}




int AudioWriteFrontData(u8* pbuf)
{

	memcpy((u8*)((u32)pPlayLeftFifo+(PlayLeftProduceIndex*PLAY_PACK_SIZE)), pbuf, PLAY_PACK_SIZE);


	PlayLeftProduceIndex = IndexAdd1(PlayLeftProduceIndex, PLAY_PACK_NUM);

	PlayLeftWaitFlag = 0;
	

	return 0;
	
}



int AudioWriteBackData(u8* pbuf)
{

	memcpy((pPlayRightFifo+(PlayRightProduceIndex*PLAY_PACK_SIZE)), pbuf, PLAY_PACK_SIZE);

	PlayRightProduceIndex = IndexAdd1(PlayRightProduceIndex, PLAY_PACK_NUM);
	

	return 0;
	
}



void AudioListenSendProcess(void)
{


	if(ListenEnable == 1)
	{
		if(ListenFlag == 1)
		{
			ListenFlag = 0;

			

			ListenLeftConsumeIndex = IndexAdd1(ListenLeftProduceIndex, LISTEN_PACK_SIZE);

//			NetListenSend((u8*)(pListenLeftFifo+(ListenLeftConsumeIndex*LISTEN_PACK_SIZE)), LISTEN_PACK_SIZE);		
		
		}

	}



}




int AudioSetVolume(int inda)
{
	
	AudioVolume = inda;

	if(AudioVolume  < 0)
	{
		AudioVolume = 0;

	}
	else if(AudioVolume  > 40)
	{
		AudioVolume = 40;

	}

	L3Uda1341SetVolume((u8)(AudioVolume));

	

	StorageInfoSave();

	

	return 0;

}











