
//*****************************************************//
//		MyAudio.c
//*****************************************************//


#include "MyAll.h"
#include "soundcard.h"




u8 PlayLeftFifo[PLAY_PACK_SIZE * PLAY_PACK_NUM];
u8* pPlayLeftFifo;
int PlayLeftProduceIndex;
int PlayLeftConsumeIndex;
int PlayLeftWaitFlag;

int PlayLeftProduceByteIndex;
int PlayLeftConsumeByteIndex;
int PlayLeftBytesToWrite;
int PlayLeftBytesToRead;



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

int ListenLeftFlag;
int ListenLeftEnable;

int ListenRightFlag;
int ListenRightEnable;




AudioPlayInfoTypeDef AudioPlayInfo;


int AudioVolume;
int AudioTreble;
int AudioBass;

int AudioMute;




int AudioFrontPlayFlag;
int AudioBackPlayFlag;


u32 PlayClock;
int PlayClockSendFlag;
int PlayClockEnable;
int PlayClockRcvFlag;


int SampleV;
int SampleVbuf[8];

int SampleI;
int SampleIbuf[8];

int SampleVIbufIndex;

int SampleR;

int SpeakerState;



int EmeVolume;
int EmeVolumeSetFlag;
int EmeAudioFlag;

int EmeAudioTestFlag;


int AudioUpDownFlag;


int AudioNdOffset;
int AudioNdFlag;
int AudioNdOffsetAim;
int AudioNdedVolume;


int AudioPlayTestSinFlag;
int AudioPlayTestSinFrequency;
u8 AudioPlayTestSinBuf[1024];
int AudioPlayTestSinSampleIndex;


int AudioInputSampleV;

int AudioInputSampleI;

u8 AudioPlayCurrentType;

int VolumeOccDb;
int VolumeMediaDb;
int VolumeToalDb;
int VolumeBroadcastDb;
int VolumeDoorDb;
int VolumeEemergencyDb;
int VolumeTestDb;
int VolumeMicDb;

u8 VolumeChangeFlag;






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


	PlayLeftProduceByteIndex = PLAY_PACK_SIZE * PLAY_PACK_NUM / 2;
	PlayLeftConsumeByteIndex = 0;
	PlayLeftBytesToWrite = PLAY_PACK_SIZE * PLAY_PACK_NUM / 2;
	PlayLeftBytesToRead = PLAY_PACK_SIZE * PLAY_PACK_NUM / 2;



	PlayRightConsumeIndex = 0;
	PlayRightProduceIndex = PLAY_PACK_NUM / 2;


//////////////////////////////////////////////////////


	pListenLeftFifo = ListenLeftFifo;

	ListenLeftConsumeIndex = 0;
	ListenLeftProduceIndex = LISTEN_PACK_NUM / 2;

	ListenLeftFlag = 0;
	ListenLeftEnable = 1;


	pListenRightFifo = ListenRightFifo;

	ListenRightConsumeIndex = 0;
	ListenRightProduceIndex = LISTEN_PACK_NUM / 2;

	ListenRightFlag = 0;
	ListenRightEnable = 1;


///////////////////////////////////////////////////////



	AudioPlayInfo.EnableFlag = 0;


	AudioVolume = 10;

	AudioNdedVolume = AudioVolume;
	AudioNdFlag = 0;
	AudioNdOffsetAim = 0;
	AudioNdedVolume = 0;
	AudioPlayCurrentType=0;
    soundcard_init();
    Delay(2000);
    I2s2_Init();
    I2s3_Init();
    DmaPlay_Init();
    DmaListen_Init();
    Delay(41000);
	AudioDown();
	StorageInfoRead();
	Delay(10000);
    AudioSetVolume(AudioVolume);

/*
	L3InitUda1341();
	Delay(1000);

	L3Uda1341SetInputGain(26);	//26 is a suitable value for sampling.
	Delay(1000);


	I2s2_Init();
	DmaPlay_Init();

	Delay(1000);

	I2s3_Init();
	DmaListen_Init();

	Delay(1000);

	AudioDown();


	Delay(10000);
	Delay(10000);
	StorageInfoRead();
	Delay(10000);
	Delay(10000);


	AudioSetVolume(AudioVolume);
*/

	SampleVIbufIndex = 0;


	EmeAudioFlag = 0;


	AudioPlayTestSinSampleIndex = 0;




}


void AudioDown(void)
{
//	I2s2Down();
//	I2s3Down();

	AudioUpDownFlag = 0;

//	L3Uda1341Down();

}

void AudioUp(void)
{
	I2s2Up();
	I2s3Up();

	AudioUpDownFlag = 1;

//	L3Uda1341Up();



}



void AudioProcess(void)
{


//write 0s to the play fifo if there is no audio data to receive.//////////////

	if(DmaPlayFlag > 0)
	{
		DmaPlayFlag = 0;

		PlayLeftWaitFlag ++;
		PlayRightWaitFlag ++;

		AudioPlayTestSinProcess();

	}

	if((PlayLeftWaitFlag > PLAY_PACK_NUM) && (PlayLeftWaitFlag < PLAY_PACK_NUM + 8))
	{
		memset(((u8*)pPlayLeftFifo), 0, (PLAY_PACK_SIZE * PLAY_PACK_NUM));

	}
	else if(PlayLeftWaitFlag > 128)
	{
		PlayLeftWaitFlag = 128;
	}

	if((PlayRightWaitFlag > PLAY_PACK_NUM) && (PlayRightWaitFlag < PLAY_PACK_NUM + 8))
	{
		memset(((u8*)pPlayRightFifo), 0, (PLAY_PACK_SIZE * PLAY_PACK_NUM));

	}
	else if(PlayRightWaitFlag > 128)
	{
		PlayRightWaitFlag = 128;
	}

////////////////////////////////////////////////////////////


	if(AppStartFlag == 1)
	{
//		AudioListenSendProcess();

//		AudioPlayTestSinProcess();

	}


////////////////////////////////////////////////////////////






}



void Audio10ms(void)
{

	if(AudioUpDownFlag == 0)
	{
//		AudioPlayClockSend();

	}


	AudioInputSampleProcess();

}


void Audio100ms(void)
{

//////////////////////////////////////////

	AudioPlayInfo.EnableFlag --;
	if(AudioPlayInfo.EnableFlag < 0)
	{
		AudioPlayInfo.EnableFlag = 0;

		AudioDown();

	}


//////////////////////////////////////////

	if(AppStartFlag > 0)
	{
//		EmeVolumeSetProcess();

		AudioSampleProcess();

	}


//////////////////////////////////////////////


	PlayClockRcvFlag = PlayClockRcvFlag - 2;
	if(PlayClockRcvFlag < 0)
	{
		PlayClockRcvFlag = 0;

	}


	if(PlayClockRcvFlag > (64 - DevId))
	{
		PlayClockEnable = 0;

	}
	else
	{
		PlayClockEnable = 1;

	}



//////////////////////////////////////////////





}



void Audio200ms(void)
{
//	AudioSetNdOffsetProcess();

}


void Audio500ms(void)
{
	AudioSetNdOffsetProcess();

}





int AudioWriteFrontData(u8* pbuf)
{

	if((PlayLeftProduceIndex==PlayLeftConsumeIndex)
	   || (PlayLeftProduceIndex== IndexAdd1(PlayLeftConsumeIndex, PLAY_PACK_NUM)))
	{
		PlayLeftProduceIndex = IndexAddn(PlayLeftConsumeIndex, PLAY_PACK_NUM, (PLAY_PACK_NUM/2+1));

	}

	memcpy((u8*)((u32)pPlayLeftFifo+(PlayLeftProduceIndex*PLAY_PACK_SIZE)), pbuf, PLAY_PACK_SIZE);

	PlayLeftProduceIndex = IndexAdd1(PlayLeftProduceIndex, PLAY_PACK_NUM);

	PlayLeftWaitFlag = 0;


	return 0;

}



int AudioWriteBackData(u8* pbuf)
{

	memcpy((pPlayRightFifo+(PlayRightProduceIndex*PLAY_PACK_SIZE)), pbuf, PLAY_PACK_SIZE);

	PlayRightProduceIndex = IndexAdd1(PlayRightProduceIndex, PLAY_PACK_NUM);

	PlayRightWaitFlag = 0;


	return 0;

}



int AudioPlayLeftFifoWrite(u8* pbuf, int len)
{
	int i;


	if(PlayLeftBytesToWrite >= (len + 1024))
	{
		for(i=0; i<len; i++)
		{
			PlayLeftFifo[PlayLeftProduceByteIndex] = *(pbuf + i);

			PlayLeftProduceByteIndex++;
			if(PlayLeftProduceByteIndex >= (PLAY_PACK_SIZE * PLAY_PACK_NUM))
			{
				PlayLeftProduceByteIndex = 0;

			}

		}

		PlayLeftBytesToWrite = PlayLeftBytesToWrite -len;

		PlayLeftBytesToRead = PlayLeftBytesToRead + len;
		if(PlayLeftBytesToRead >  (PLAY_PACK_SIZE * PLAY_PACK_NUM))
		{
			PlayLeftBytesToRead =  (PLAY_PACK_SIZE * PLAY_PACK_NUM);

		}


//		PlayLeftProduceIndex = PlayLeftProduceByteIndex / PLAY_PACK_NUM;

		PlayLeftWaitFlag = 0;


		return len;

	}
	else
	{
		return -1;

	}



}


int AudioPlayLeftFifoRead(u8* pbuf, int len)
{


	return -1;

}


int AudioPlayTestSinWrite(u8* p_sample_buf)
{
	int i;


	for(i=0; i<1024; i++)
	{
		AudioPlayTestSinBuf[i] = *(p_sample_buf + AudioPlayTestSinSampleIndex);

		AudioPlayTestSinSampleIndex++;
		if(AudioPlayTestSinSampleIndex >= 880)
		{
			AudioPlayTestSinSampleIndex = 0;

		}

	}

	AudioWriteFrontData(AudioPlayTestSinBuf);


	return 0;

}


int AudioPlayTestSin(int cmd, int frequency)
{

	if(AudioPlayInfo.EnableFlag > 0)
	{

		return -1;

	}

	if(cmd == 0)
	{
		AudioPlayTestSinFlag= 0;

		AudioPlayTestSinSampleIndex = 0;

		return 0;

	}
	else if(cmd == 1)
	{
		AudioPlayTestSinFlag = 1;


		if(frequency == 100)
		{
			AudioPlayTestSinFrequency = frequency;

		}
		else if(frequency == 200)
		{
			AudioPlayTestSinFrequency = frequency;

		}
		else if(frequency == 500)
		{
			AudioPlayTestSinFrequency = frequency;

		}
		else if(frequency == 800)
		{
			AudioPlayTestSinFrequency = frequency;

		}
		else if(frequency == 1000)
		{
			AudioPlayTestSinFrequency = frequency;

		}
		else if(frequency == 2000)
		{
			AudioPlayTestSinFrequency = frequency;

		}
		else if(frequency == 5000)
		{
			AudioPlayTestSinFrequency = frequency;

		}
		else if(frequency == 8000)
		{
			AudioPlayTestSinFrequency = frequency;

		}
		else if(frequency == 10000)
		{
			AudioPlayTestSinFrequency = frequency;

		}
		else if(frequency == 15000)
		{
			AudioPlayTestSinFrequency = frequency;

		}
		else if(frequency == 20000)
		{
			AudioPlayTestSinFrequency = frequency;

		}
		else
		{
			return -2;

		}


		return 0;

	}



	return -1;

}


void AudioPlayTestSinProcess(void)
{


	if(AudioPlayInfo.EnableFlag > 0)
	{
		return;

	}


	if(AudioPlayTestSinFlag == 0)
	{


	}
	else if(AudioPlayTestSinFlag == 1)
	{
		if(AudioPlayTestSinFrequency == 100)
		{
			//		AudioPlayLeftFifoWrite((u8*)Sin100Sample, 880);

			AudioPlayTestSinWrite((u8*)Sin100Sample);

		}
		else if(AudioPlayTestSinFrequency == 200)
		{
			//		AudioPlayLeftFifoWrite((u8*)Sin200Sample, 880);

			AudioPlayTestSinWrite((u8*)Sin200Sample);

		}
		else if(AudioPlayTestSinFrequency == 500)
		{
			//		AudioPlayLeftFifoWrite((u8*)Sin500Sample, 880);

			AudioPlayTestSinWrite((u8*)Sin500Sample);

		}
		else if(AudioPlayTestSinFrequency == 800)
		{
			//		AudioPlayLeftFifoWrite((u8*)Sin800Sample, 880);

			AudioPlayTestSinWrite((u8*)Sin800Sample);

		}
		else if(AudioPlayTestSinFrequency == 1000)
		{
			//		AudioPlayLeftFifoWrite((u8*)Sin1kSample, 880);

			AudioPlayTestSinWrite((u8*)Sin1kSample);

		}
		else if(AudioPlayTestSinFrequency == 2000)
		{
			//		AudioPlayLeftFifoWrite((u8*)Sin2kSample, 880);

			AudioPlayTestSinWrite((u8*)Sin2kSample);

		}
		else if(AudioPlayTestSinFrequency == 5000)
		{
			//		AudioPlayLeftFifoWrite((u8*)Sin5kSample, 880);

			AudioPlayTestSinWrite((u8*)Sin5kSample);

		}
		else if(AudioPlayTestSinFrequency == 8000)
		{
			//		AudioPlayLeftFifoWrite((u8*)Sin8kSample, 880);

			AudioPlayTestSinWrite((u8*)Sin8kSample);

		}
		else if(AudioPlayTestSinFrequency == 10000)
		{
			//		AudioPlayLeftFifoWrite((u8*)Sin10kSample, 880);

			AudioPlayTestSinWrite((u8*)Sin10kSample);

		}
		else if(AudioPlayTestSinFrequency == 15000)
		{
			//		AudioPlayLeftFifoWrite((u8*)Sin15kSample, 880);

			AudioPlayTestSinWrite((u8*)Sin15kSample);

		}
		else if(AudioPlayTestSinFrequency == 20000)
		{
			//		AudioPlayLeftFifoWrite((u8*)Sin20kSample, 880);

			AudioPlayTestSinWrite((u8*)Sin20kSample);

		}


	}





}




void AudioListenSendProcess(void)
{


	if(ListenLeftEnable == 1)
	{
		if(ListenLeftFlag == 1)
		{
			ListenLeftFlag = 0;

			ListenLeftConsumeIndex = IndexAdd1(ListenLeftProduceIndex, LISTEN_PACK_NUM);

			NetTalkTestLeftSend((u8*)(pListenLeftFifo+(ListenLeftConsumeIndex*LISTEN_PACK_SIZE)), LISTEN_PACK_SIZE);

//			ListenLeftConsumeIndex = IndexAdd1(ListenLeftConsumeIndex, LISTEN_PACK_NUM);

		}

	}



	if(ListenRightEnable == 1)
	{
		if(ListenRightFlag == 1)
		{
			ListenRightFlag = 0;

			ListenRightConsumeIndex = IndexAdd1(ListenRightProduceIndex, LISTEN_PACK_NUM);

			NetTalkTestRightSend((u8*)(pListenRightFifo+(ListenRightConsumeIndex*LISTEN_PACK_SIZE)), LISTEN_PACK_SIZE);

//			ListenRightConsumeIndex = IndexAdd1(ListenRightConsumeIndex, LISTEN_PACK_NUM);

		}

	}




}


void AudioSampleProcess(void)
{
	int i;
	int sum;


	if((ListenLeftFlag == 1) && (ListenRightFlag == 1))
	{
		ListenLeftFlag = 0;

		ListenLeftConsumeIndex = IndexAdd1(ListenLeftProduceIndex, LISTEN_PACK_NUM);

		AudioSamplingV((u8*)(pListenLeftFifo+(ListenLeftConsumeIndex*LISTEN_PACK_SIZE)), LISTEN_PACK_SIZE);




		ListenRightFlag = 0;

		ListenRightConsumeIndex = IndexAdd1(ListenRightProduceIndex, LISTEN_PACK_NUM);

		AudioSamplingI((u8*)(pListenRightFifo+(ListenRightConsumeIndex*LISTEN_PACK_SIZE)), LISTEN_PACK_SIZE);


		SampleVIbufIndex ++;

	}



	if(SampleVIbufIndex >= 8)
	{
		SampleVIbufIndex = 0;

		sum = 0;
		for(i=0; i<8; i++)
		{
			sum = sum + SampleVbuf[i];

		}

		SampleV = 10 * (sum/8);


		sum = 0;
		for(i=0; i<8; i++)
		{
			sum = sum + SampleIbuf[i];

		}

		SampleI = sum/8;



		if((SampleV<100) && (SampleI<10))
		{
			SampleR = 0;

			SpeakerState = SPEAKER_STATE_NORMAL;

		}
		else	if(SampleV < 20)
		{
			SpeakerState = SPEAKER_STATE_SHORT;

		}
		else	if(SampleI < 2)
		{
			SpeakerState = SPEAKER_STATE_OPEN;

		}
		else
		{
			SampleR = (SampleV / SampleI);

			if(SampleR > 200)
			{
				SpeakerState = SPEAKER_STATE_OPEN;

			}
			else if(SampleR < 10)
			{
				SpeakerState = SPEAKER_STATE_SHORT;

			}
			else
			{
				SpeakerState = SPEAKER_STATE_NORMAL;

			}


		}


	}




}


void AudioSamplingV(u8* pbuf, int len)
{
	int i;
	int div;

	s16* pda;
	int da_len;

	int temp;
	int sum;
	int num;


	pda = (s16*)(pbuf);
	da_len = len/2;

	sum = 0;
	num = 0;
	div = 8;
	for(i=0; i<da_len; i=i+div)
	{
		temp = *(pda + i);

//		SampleV = temp;

		if(temp >= -20)
		{
			sum = sum + temp + 20;
			num ++;

		}

//		sum = sum + temp;
//		num ++;

	}


	SampleVbuf[SampleVIbufIndex] = sum/num;



}

void AudioSamplingI(u8* pbuf, int len)
{
	int i;
	int div;

	s16* pda;
	int da_len;

	int temp;
	int sum;
	int num;


	pda = (s16*)(pbuf);
	da_len = len/2;

	sum = 0;
	num = 0;
	div = 8;
	for(i=0; i<da_len; i=i+div)
	{
		temp = *(pda + i);

//		SampleI = temp;

		if(temp >= -60)
		{
			sum = sum + temp + 60;
			num ++;

		}

//		sum = sum + temp;
//		num ++;

	}

	SampleIbuf[SampleVIbufIndex] = sum/num;


}






int AudioSetVolume(int inda)
{
    int AudioVolumeSet = 0;
    static int AudioVolume_bak = 100;
    AudioVolume = inda;

	if(AudioVolume  < 0)
	{
		AudioVolume = 0;

	}
	else if(AudioVolume  > 40)
	{
		AudioVolume = 40;

	}
	if((CcState==CCVC_ST_MC)||(CcState==CCVC_ST_MC_AC))
	{
		AudioNdedVolume = AudioVolume + (-(AudioNdOffset))+PcDbSet;
	}
	else
	{
		AudioNdedVolume = AudioVolume + (-(AudioNdOffset));
	}
	if(AudioNdedVolume  < 0)
	{
		AudioNdedVolume = 0;

	}
	else if(AudioNdedVolume  >=40)
	{
		AudioNdedVolume=40;

	}

    if (AudioNdedVolume <= 2)
    {
        AudioVolumeSet = 2;
    }
    else 
    {
        AudioVolumeSet = AudioNdedVolume;    
    }

    if (AudioVolumeSet != AudioVolume_bak) {
        AudioVolume_bak = AudioVolumeSet;
        soundcard_set_output_vol((u8)(AudioVolumeSet));
    }

//	StorageInfoSave();

	return 0;

}



void EmeVolumeSet(void)
{
	EmeVolumeSetFlag = 1;
	EmeVolume = AudioVolume;

}


void EmeVolumeSetProcess(void)
{

	if(EmeAudioTestFlag == 1)
	{
		if(EmeVolumeSetFlag > 0)
		{
			Delay(1000);
			PT2257Write(0, EmeVolume);
			Delay(1000);

			EmeVolumeSetFlag = 0;

		}

		return;

	}



	if(EmeAudioFlag == 0)		//eme audio will cut off
	{
		EmeVolume = 79;		// -79db for mute

		EmeAudioFlag = 10; 	//eme audio is cutting off

	}
	else if(EmeAudioFlag == 1)		//eme audio will put on
	{
		EmeVolume = AudioVolume;

		EmeAudioFlag = 11; 		//eme audio is putting on

	}
	else  if(EmeAudioFlag == 10)		//eme audio is cutting off
	{
		Delay(1000);
		PT2257Write(0, EmeVolume);
		Delay(1000);
		EmeVolumeSetFlag = 0;

		EmeAudioFlag = 20;	//eme audio was off

	}
	else  if(EmeAudioFlag == 11)		//eme audio is putting on
	{
		Delay(1000);
		PT2257Write(0, EmeVolume);
		Delay(1000);

		EmeAudioFlag = 21; 	//eme audio was on

	}
	else if(EmeAudioFlag == 20)	//eme audio was off
	{
		EmeVolumeSetFlag = 0;

		if(GetEme() == 0)
		{
			EmeAudioFlag = 1;
		}
		else
		{

		}

	}
	else if(EmeAudioFlag == 21)	//eme audio was on
	{
		if(EmeVolumeSetFlag > 0)
		{
			Delay(1000);
			PT2257Write(0, EmeVolume);
			Delay(1000);

			EmeVolumeSetFlag = 0;

		}

		if(GetEme() == 0)
		{

		}
		else
		{
			EmeAudioFlag = 0;
		}

	}



}



void EmeAudioTestFlagSet(int inflag)
{
	if(inflag > 0)
	{
		EmeAudioTestFlag = 1;

	}
	else
	{
		EmeAudioTestFlag = 0;

	}

}




int AudioPlayClockSend(void)
{

	if(AppStartFlag > 0);
	{
		PlayClock ++ ;

		if(PlayClockEnable == 1)
		{
			PlayClockSendFlag = 1;

		}

	}

	return 0;

}




int AudioSetNdOffset(u8 ndv)
{

	if((ndv > 100)  || (ndv < 60))
	{
		return -1;

	}

	if(ndv > 95)
	{
		ndv = 95;

	}
	else if(ndv < 73)
	{
		ndv = 73;

	}

//	if(AudioPlayInfo.EnableFlag < 1)
	{
		AudioNdOffsetAim = ((int)(ndv)) - 75;

	}

	AudioNdFlag = 4;


	return 0;

}


int AudioSetNdOffsetProcess(void)
{


	if(AudioNdFlag > 0)
	{
		AudioNdFlag--;

	}
	else
	{
		AudioNdOffsetAim = 0;

	}



	if(AudioNdOffset > AudioNdOffsetAim)
	{
		AudioNdOffset--;

		AudioSetVolume(AudioVolume);

	}
	else if(AudioNdOffset < AudioNdOffsetAim)
	{
		AudioNdOffset++;

		AudioSetVolume(AudioVolume);

	}



	return 0;

}





void AudioInputSampleProcess(void)		//new method of AudioSampleProcess()
{

	s16* pda;
	int pack_index;

	int i;
	int temp;
	int max;
	int min;

	static int vbuf[8];
	static int ibuf[8];
	static int buf_index = 0;

	int sum;


//	if((ListenLeftFlag == 1) && (ListenRightFlag == 1))
	{

		pack_index = IndexAdd1(ListenLeftProduceIndex, LISTEN_PACK_NUM);

		pda = (s16*)(pListenLeftFifo+(pack_index*LISTEN_PACK_SIZE));


		max = 0;
		min = 0;

		for(i=0; i<512; i++)
		{
			temp = *(pda+i);

			if(temp > max)
			{
				max = temp;

			}
			else if(temp < min)
			{
				min = temp;

			}

		}

		vbuf[buf_index] = max - min;



		pack_index = IndexAdd1(ListenRightProduceIndex, LISTEN_PACK_NUM);

		pda = (s16*)(pListenRightFifo+(pack_index*LISTEN_PACK_SIZE));


		max = 0;
		min = 0;

		for(i=0; i<512; i++)
		{
			temp = *(pda+i);

			if(temp > max)
			{
				max = temp;

			}
			else if(temp < min)
			{
				min = temp;

			}

		}

		ibuf[buf_index] = max - min;



		buf_index++;

		if(buf_index >= 8)
		{
			buf_index = 0;

			sum = 0;
			for(i=0; i<8; i++)
			{
				sum = sum + vbuf[i];

			}

			AudioInputSampleV = sum / 8;


			sum = 0;
			for(i=0; i<8; i++)
			{
				sum = sum + ibuf[i];

			}

			AudioInputSampleI= sum / 8;

		}



	}



}

void AduioTypeVolume(void)
{

	if((AudioPlayCurrentType==AudioPlay_PreStationType)||(AudioPlayCurrentType==AudioPlay_ArrStationType)||(AudioPlayCurrentType==AudioPlay_PreBroadcastType)||(AudioPlayCurrentType==AudioPlay_SkipType))
	{	
		AudioSetVolume(VolumeBroadcastDb+VolumeToalDb);
	}
	else if(AudioPlayCurrentType==AudioPlay_DoorType)
	{
		AudioSetVolume(VolumeDoorDb+VolumeToalDb);
	}
	else if(AudioPlayCurrentType==AudioPlay_OccType)
	{
		AudioSetVolume(VolumeOccDb+VolumeToalDb);
	}
	else if(AudioPlayCurrentType==AudioPlay_MediaType)
	{
		AudioSetVolume(VolumeMediaDb+VolumeToalDb);
	}
	else if(AudioPlayCurrentType==AudioPlay_TestType)
	{
		AudioSetVolume(VolumeTestDb+VolumeToalDb);
	}
	else if(AudioPlayCurrentType==AudioPlay_EmergencyType)
	{
		AudioSetVolume(VolumeEemergencyDb+VolumeToalDb);
	}
	else if(AudioPlayCurrentType==AudioPlay_PAType)
	{
		AudioSetVolume(VolumeToalDb);
	}
	else
	{
		AudioSetVolume(VolumeToalDb);

	}

}









