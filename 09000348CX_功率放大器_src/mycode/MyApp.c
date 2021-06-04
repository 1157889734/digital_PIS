
//*****************************************************//
//		MyApp.c
//*****************************************************//


#include "MyAll.h"



u8 DevType;
u8 DevId;

u16 TrainId;



int AppStartTimer;
int AppStartFlag;


u8 Addr;

int AmpState;

int DC24V1State;
int DC24V2State;


int NoiseDetectState;

u8 CurrentState;

u8 AppVersion1;
u8 AppVersion2;
u8 AppVersion3;
u8 AppVersion4;

u8 CcState;
int CcDbSet;
u8 CcTestSet;
int PcDbSet;
int PcDbSet_Tmp;

u8 voltotalset;


int LedBrightness[5];



void AppInit(void)
{

	AppVersion1 = 3;
	AppVersion2 = 2;
	AppVersion3 = 5;
	AppVersion4 = 95;


	AppStartTimer = 1;

	DevType = PRTCL_DEVTYPE_AMP;

	DevId = 1;

	//TrainId = 1;

	Addr = GetAddress();

    TrainId = (Addr - 1)/2 + 1;

	DevId = Addr;

    reboot_flag = 0;
    NetHad = 0;


	AdcInit();


	KeyInit();


	SelfTestInit();
//	SelfTestStart();


	ram_function();


}

void AppOff(void)
{

	Delay(10000);
	I2sOff();

	Wwdg_Feed();
	Delay(10000);
	LwipOff();

	Wwdg_Feed();

	Delay(10000);
	TimerOff();

	Delay(10000);

}



void AppStart(void)
{
	CommanderInit();

}



void AppProcess(void)
{
#if 0
	if(KeyValue == KEY_VALUE_UP)
	{
		AudioVolume --;
		AudioSetVolume(AudioVolume);

		KeyValue = KEY_VALUE_NULL;

	}
	else if(KeyValue == KEY_VALUE_DOWN)
	{
		AudioVolume ++;
		AudioSetVolume(AudioVolume);

		KeyValue = KEY_VALUE_NULL;

	}
	LedLightProcess();
#endif
	VolumeDevProcess();



}


void App10ms(void)
{

	Wwdg_Feed();


}



void App100ms(void)
{

	FeedDog();

	AppStartTimer++;

	if(AppStartTimer == 20)
	{
		AppStartFlag = 1;
		AppStart();

	}
	else if(AppStartTimer > 1000)
	{
		AppStartTimer = 1000;

	}


	DisplayDb();

	AmpState = GetAmpState();

	GetDC24Vs();

	GetNoiseDetectState();

	GetCurrentState();



}


void App1000ms(void)
{



}



int SetDeviceId(int inda)
{

	if(inda > 64)
	{
		return -1;
	}

	DevId = (u8)inda;

	StorageInfoSave();

	CommanderSendDeviceId();

	return 0;

}


int SetTrainId(int inda)
{

	if(inda > 65536)
	{
		return -1;
	}

	TrainId = (u16)inda;

	StorageInfoSave();

	CommanderSendTrainId();

	return 0;

}



void DisplayDb(void)
{


	if(AudioVolume == 0)
	{
		LedLight(0, 10);
		LedLight(1, 0);
		LedLight(2, 0);
		LedLight(3, 0);
		LedLight(4, 0);

	}
	else if((AudioVolume > 0) && (AudioVolume <= 3))
	{
		LedLight(0, 0);
		LedLight(1, 10);
		LedLight(2, 0);
		LedLight(3, 0);
		LedLight(4, 0);

	}
	else if((AudioVolume > 3) && (AudioVolume <= 6))
	{
		LedLight(0, 0);
		LedLight(1, 0);
		LedLight(2, 10);
		LedLight(3, 0);
		LedLight(4, 0);

	}
	else if((AudioVolume > 6) && (AudioVolume <= 9))
	{
		LedLight(0, 0);
		LedLight(1, 0);
		LedLight(2, 0);
		LedLight(3, 10);
		LedLight(4, 0);

	}
	else if((AudioVolume > 9) && (AudioVolume <= 12))
	{
		LedLight(0, 0);
		LedLight(1, 0);
		LedLight(2, 0);
		LedLight(3, 0);
		LedLight(4, 10);

	}
	else
	{
		LedLight(0, 0);
		LedLight(1, 0);
		LedLight(2, 0);
		LedLight(3, 0);
		LedLight(4, 0);

	}


///*
	if(AudioNdedVolume == 0)
	{
		if(LedGetBrightness(0) == 0)
		{
			LedLight(0, 1);
		}
		if(LedGetBrightness(1) == 0)
		{
			LedLight(1, 0);
		}
		if(LedGetBrightness(2) == 0)
		{
			LedLight(2, 0);
		}
		if(LedGetBrightness(3) == 0)
		{
			LedLight(3, 0);
		}
		if(LedGetBrightness(4) == 0)
		{
			LedLight(4, 0);
		}

	}
	else if((AudioNdedVolume > 0) && (AudioNdedVolume <= 3))
	{
		if(LedGetBrightness(0) == 0)
		{
			LedLight(0, 0);
		}
		if(LedGetBrightness(1) == 0)
		{
			LedLight(1, 1);
		}
		if(LedGetBrightness(2) == 0)
		{
			LedLight(2, 0);
		}
		if(LedGetBrightness(3) == 0)
		{
			LedLight(3, 0);
		}
		if(LedGetBrightness(4) == 0)
		{
			LedLight(4, 0);
		}

	}
	else if((AudioNdedVolume > 3) && (AudioNdedVolume <= 6))
	{
		if(LedGetBrightness(0) == 0)
		{
			LedLight(0, 0);
		}
		if(LedGetBrightness(1) == 0)
		{
			LedLight(1, 0);
		}
		if(LedGetBrightness(2) == 0)
		{
			LedLight(2, 1);
		}
		if(LedGetBrightness(3) == 0)
		{
			LedLight(3, 0);
		}
		if(LedGetBrightness(4) == 0)
		{
			LedLight(4, 0);
		}

	}
	else if((AudioNdedVolume > 6) && (AudioNdedVolume <= 9))
	{
		if(LedGetBrightness(0) == 0)
		{
			LedLight(0, 0);
		}
		if(LedGetBrightness(1) == 0)
		{
			LedLight(1, 0);
		}
		if(LedGetBrightness(2) == 0)
		{
			LedLight(2, 0);
		}
		if(LedGetBrightness(3) == 0)
		{
			LedLight(3, 1);
		}
		if(LedGetBrightness(4) == 0)
		{
			LedLight(4, 0);
		}

	}
	else if((AudioNdedVolume > 9) && (AudioNdedVolume <= 12))
	{
		if(LedGetBrightness(0) == 0)
		{
			LedLight(0, 0);
		}
		if(LedGetBrightness(1) == 0)
		{
			LedLight(1, 0);
		}
		if(LedGetBrightness(2) == 0)
		{
			LedLight(2, 0);
		}
		if(LedGetBrightness(3) == 0)
		{
			LedLight(3, 0);
		}
		if(LedGetBrightness(4) == 0)
		{
			LedLight(4, 1);
		}

	}
//*/


}




void LedLight(int num, int brightness)
{
	LedBrightness[num] = brightness;

}

int LedGetBrightness(int num)
{
	return(LedBrightness[num]);

}



void LedLightProcess(void)
{
	static int step = 0;

	if(LedBrightness[0] > step)
	{
		LED3pdbOn();

	}
	else
	{
		LED3pdbOff();

	}

	if(LedBrightness[1] > step)
	{
		LED0dbOn();

	}
	else
	{
		LED0dbOff();

	}

	if(LedBrightness[2] > step)
	{
		LED3dbOn();

	}
	else
	{
		LED3dbOff();

	}

	if(LedBrightness[3] > step)
	{
		LED6dbOn();

	}
	else
	{
		LED6dbOff();

	}

	if(LedBrightness[4] > step)
	{
		LED9dbOn();

	}
	else
	{
		LED9dbOff();

	}


	step ++;
	if(step >= 10)
	{
		step = 0;

	}


}

void VolumeDevProcess(void)
{
	if(CcState==CCVC_ST_CC)
	{

	}
	else  if(CcState==CCVC_ST_MC)
	{


	}
	else  if(CcState==CCVC_ST_CC_AC)
	{


	}
	else  if(CcState==CCVC_ST_MC_AC)
	{


	}



}




int GetAmpState(void)
{
	int result;

	int r_short;
	int r_ovp;
	int r_uvp;


	r_short = GetShort();
	r_ovp = GetOvp();
	r_uvp = GetUvp();


	if(r_short == 0)
	{
		if(r_ovp == 0)
		{
			result = AMPSTATE_OVP;

		}
		else if(r_uvp == 0)
		{
			result = AMPSTATE_UVP;

		}
		else
		{
			result = AMPSTATE_SHORT;

		}

	}
	else
	{
		if(r_ovp == 0)
		{
			result = AMPSTATE_ERR;

		}
		else if(r_uvp == 0)
		{
			result = AMPSTATE_ERR;

		}
		else
		{
			result = AMPSTATE_OK;

		}

	}


	return result;

}


void GetDC24Vs(void)
{

	if(GetDC24V1() > 0)
	{
		DC24V1State = 0;

	}
	else
	{
		DC24V1State = 1;

	}

	if(GetDC24V2() > 0)
	{
		DC24V2State = 0;

	}
	else
	{
		DC24V2State = 1;

	}


}


void GetNoiseDetectState(void)
{

	if((GetNoise1Detect() > 0) && (GetNoise2Detect() > 0))
	{
		NoiseDetectState = NDSTATE_NULL;

	}
	else if((GetNoise1Detect() == 0) && (GetNoise2Detect() > 0))
	{
		NoiseDetectState = NDSTATE_ONLY1;

	}
	else if((GetNoise1Detect() > 0) && (GetNoise2Detect() == 0))
	{
		NoiseDetectState = NDSTATE_ONLY2;

	}
	else if((GetNoise1Detect() == 0) && (GetNoise2Detect() == 0))
	{
		NoiseDetectState = NDSTATE_BOTH;

	}



}


void GetCurrentState(void)
{

	if(AmpState == AMPSTATE_OK)
	{
		CurrentState = CurrentState & (~0x03);

	}
	else if(AmpState == AMPSTATE_OVP)
	{
		CurrentState = CurrentState & (~0x03);
		CurrentState = CurrentState | (0x01);

	}
	else if(AmpState == AMPSTATE_UVP)
	{
		CurrentState = CurrentState & (~0x03);
		CurrentState = CurrentState | (0x02);

	}
	else if(AmpState == AMPSTATE_SHORT)
	{
		CurrentState = CurrentState & (~0x03);
//		CurrentState = CurrentState | (0x03);

	}



	if(SpeakerState == SPEAKER_STATE_NORMAL)
	{
		CurrentState = CurrentState & (~(0x03<<2));

	}
	else if(SpeakerState == SPEAKER_STATE_OPEN)
	{
		CurrentState = CurrentState & (~(0x03<<2));
		CurrentState = CurrentState | (0x01<<2);

	}
	else if(SpeakerState == SPEAKER_STATE_SHORT)
	{
		CurrentState = CurrentState & (~(0x03<<2));
		CurrentState = CurrentState | (0x02<<2);

	}



	if(DC24V1State == 0)
	{
		CurrentState = CurrentState & (~(0x01<<4));

	}
	else
	{
		CurrentState = CurrentState | (0x01<<4);

	}


	if(DC24V2State == 0)
	{
		CurrentState = CurrentState & (~(0x02<<4));

	}
	else
	{
		CurrentState = CurrentState | (0x02<<4);

	}




	if(NoiseDetectState == NDSTATE_NULL)
	{
		CurrentState = CurrentState & (~(0x03<<6));

	}
	else if(NoiseDetectState == NDSTATE_ONLY1)
	{
		CurrentState = CurrentState & (~(0x03<<6));
		CurrentState = CurrentState | (0x01<<6);

	}
	else if(NoiseDetectState == NDSTATE_ONLY2)
	{
		CurrentState = CurrentState & (~(0x03<<6));
		CurrentState = CurrentState | (0x02<<6);

	}
	else if(NoiseDetectState == NDSTATE_BOTH)
	{
		CurrentState = CurrentState & (~(0x03<<6));
		CurrentState = CurrentState | (0x03<<6);

	}




}

u8 Volumeset(int volpercent,int totalpercent,int totaldb)
{

	if(volpercent==100 )
	{
		return(totaldb);
	}
	else if(volpercent==0)
	{
		return(CCVC_AUDIO_MUTE);
	}
	else
	{
		return(40-(((40-totaldb)*volpercent)/100) );
	}



}










