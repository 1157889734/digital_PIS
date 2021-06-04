
//*****************************************************//
//		MyCommander.c
//*****************************************************//

#include "MyAll.h"



char OutStr[512];

int CommanderSendAdcValueFlag;
int CommanderSendSampleFlag;
int CommanderSendNoiseFlag;


void CommanderInit(void)
{

	char logo_str[] = "//**********  Beihai Communication  ***********//\n\n\
Digital_PIS         \n\
Power_Amp_FW version: 1.0  \n   \n" ;
	
	CommanderSendString(logo_str);


	CommanderSendLocalIp();
	CommanderSendDeviceType();
	CommanderSendDeviceId();
	CommanderSendTrainId();
	CommanderSendI2cState();
	

}


void CommanderSendString(char* pstr)
{

	UdpCommanderSend((u8*)pstr, strlen(pstr));


}


void Commander500ms(void)
{

	if(AppStartFlag > 0)
	{

		//////////////////////////////////////////////////////////
		
		if(CommanderSendAdcValueFlag > 0)
		{
			CommanderSendAdcValue(AdcGetValue1(), AdcGetValue2());

		}

		//////////////////////////////////////////////////////////
		
		if(CommanderSendSampleFlag > 0)
		{
			CommanderSendSample(SampleV, SampleI, SampleR);

		}

		//////////////////////////////////////////////////////////

		if(CommanderSendNoiseFlag > 0)
		{
			
			//CommanderSendNoise(Noise1DB, Noise2DB, Noise1Value);
			CommanderSendNoise(Noise1DB, Noise2DB, NoiseDB);

		}

		//////////////////////////////////////////////////////////


		

	}
	

}




void CommanderReceive(u8* pbuf, int len)
{

	char words[16][16];
	u8 rip[4];
	int rtemp;


	memset(words, 0, sizeof(words));
	
	LineToWords((char *)pbuf, len, &words[0][0], 16, 16);


	if((strcmp(&words[0][0], "set") == 0) && (strcmp(&words[1][0], "localip") == 0))
	{
		rip[0] = (u8)StrToInt(&words[2][0]);
		rip[1] = (u8)StrToInt(&words[3][0]);
		rip[2] = (u8)StrToInt(&words[4][0]);
		rip[3] = (u8)StrToInt(&words[5][0]);

		memset(OutStr, 0, sizeof(OutStr));		
		sprintf(OutStr, "Ok, please wait...\n");
		CommanderSendString(OutStr);

		NetSetLocalIp4(rip[0], rip[1], rip[2], rip[3]);
		

	}
	else if((strcmp(&words[0][0], "read") == 0) && (strcmp(&words[1][0], "localip") == 0))
	{
		CommanderSendLocalIp();
		
	}
	else if((strcmp(&words[0][0], "read") == 0) && (strcmp(&words[1][0], "devtype") == 0))
	{
		CommanderSendDeviceType();
		
	}
	else if((strcmp(&words[0][0], "set") == 0) && (strcmp(&words[1][0], "devid") == 0))
	{
		rtemp = StrToInt(&words[2][0]);
		SetDeviceId(rtemp);

	}
	else if((strcmp(&words[0][0], "read") == 0) && (strcmp(&words[1][0], "devid") == 0))
	{
		CommanderSendDeviceId();
		
	}
	else if((strcmp(&words[0][0], "set") == 0) && (strcmp(&words[1][0], "trainid") == 0))
	{
		rtemp = StrToInt(&words[2][0]);
		SetTrainId(rtemp);

	}
	else if((strcmp(&words[0][0], "read") == 0) && (strcmp(&words[1][0], "trainid") == 0))
	{
		CommanderSendTrainId();
		
	}
	else if((strcmp(&words[0][0], "set") == 0) && (strcmp(&words[1][0], "volume") == 0))
	{
		rtemp = StrToInt(&words[2][0]);
		AudioSetVolume(rtemp);

	}


	else if((strcmp(&words[0][0], "read") == 0) && (strcmp(&words[1][0], "adc") == 0))
	{
		if (strcmp(&words[2][0], "start") == 0)
		{
			CommanderSendAdcValueFlag = 1;

		}
		else if (strcmp(&words[2][0], "stop") == 0)
		{
			CommanderSendAdcValueFlag = 0;

		}	

	}
	else if((strcmp(&words[0][0], "read") == 0) && (strcmp(&words[1][0], "addr") == 0))
	{
		CommanderSendAddr();
		
	}
	else if((strcmp(&words[0][0], "read") == 0) && (strcmp(&words[1][0], "ampstate") == 0))
	{
		CommanderSendAmpState();
		
	}
	else if((strcmp(&words[0][0], "read") == 0) && (strcmp(&words[1][0], "sample") == 0))
	{
		if (strcmp(&words[2][0], "start") == 0)
		{
			CommanderSendSampleFlag = 1;

		}
		else if (strcmp(&words[2][0], "stop") == 0)
		{
			CommanderSendSampleFlag = 0;

		}	

	}
	else if((strcmp(&words[0][0], "read") == 0) && (strcmp(&words[1][0], "speakerstate") == 0))
	{
		CommanderSendSpeakerState();
		
	}
	else if((strcmp(&words[0][0], "read") == 0) && (strcmp(&words[1][0], "dc24v") == 0))
	{
		CommanderSendDC24VsState();
		
	}
	else if((strcmp(&words[0][0], "read") == 0) && (strcmp(&words[1][0], "noise") == 0))
	{
		if (strcmp(&words[2][0], "start") == 0)
		{
			CommanderSendNoiseFlag = 1;

		}
		else if (strcmp(&words[2][0], "stop") == 0)
		{
			CommanderSendNoiseFlag = 0;

		}	

	}

	else if((strcmp(&words[0][0], "set") == 0) && (strcmp(&words[1][0], "emetest") == 0))
	{
		rtemp = StrToInt(&words[2][0]);
		EmeAudioTestFlagSet(rtemp);

	}

	else if((strcmp(&words[0][0], "read") == 0) && (strcmp(&words[1][0], "emetest") == 0))
	{
		CommanderSendEmeAudioTestFlag();

	}


	else if((strcmp(&words[0][0], "start") == 0) && (strcmp(&words[1][0], "sintest") == 0))
	{
		AudioPlayTestSin(1, StrToInt(&words[2][0]));

	}
	else if((strcmp(&words[0][0], "stop") == 0) && (strcmp(&words[1][0], "sintest") == 0))
	{
		AudioPlayTestSin(0, 0);

	}


	else if((strcmp(&words[0][0], "start") == 0) && (strcmp(&words[1][0], "selftest") == 0))
	{
		SelfTestStart();
		
	}
	else if((strcmp(&words[0][0], "stop") == 0) && (strcmp(&words[1][0], "selftest") == 0))
	{

		
	}


	else if((strcmp(&words[0][0], "update") == 0) && (strcmp(&words[1][0], "program") == 0))
	{
		CommanderUpdateProgramTest();
		
	}
	
	

	else
	{
		memset(OutStr, 0, sizeof(OutStr));		
		sprintf(OutStr, "Invalid Command !\n");
		CommanderSendString(OutStr);

	}



}



void CommanderSendLocalIp(void)
{

	memset(OutStr, 0, sizeof(OutStr));		
	sprintf(OutStr, "Local IP: %d.%d.%d.%d  \n", NetLocalIp[0], NetLocalIp[1],NetLocalIp[2], NetLocalIp[3]);
	CommanderSendString(OutStr);

}



void CommanderSendDeviceType(void)
{

	memset(OutStr, 0, sizeof(OutStr));		
	sprintf(OutStr, "Device Type: %d  \n", DevType);
	CommanderSendString(OutStr);

}


void CommanderSendDeviceId(void)
{

	memset(OutStr, 0, sizeof(OutStr));		
	sprintf(OutStr, "Device Id: %d  \n", DevId);
	CommanderSendString(OutStr);

}


void CommanderSendTrainId(void)
{

	memset(OutStr, 0, sizeof(OutStr));		
	sprintf(OutStr, "Train Id: %d  \n", TrainId);
	CommanderSendString(OutStr);

}


void CommanderSendAdcValue(int value1, int value2)
{

	memset(OutStr, 0, sizeof(OutStr));		
	sprintf(OutStr, "Adc Value1: %d  \nAdc Value2: %d  \n\n", value1, value2);
	CommanderSendString(OutStr);

}


void CommanderSendAddr(void)
{

	memset(OutStr, 0, sizeof(OutStr));		
	sprintf(OutStr, "Address: %d  \n", Addr);
	CommanderSendString(OutStr);

}


void CommanderSendAmpState(void)
{

	memset(OutStr, 0, sizeof(OutStr));	
	
	sprintf(OutStr, "AmpState: %d  \n", AmpState);
	CommanderSendString(OutStr);

}


void CommanderSendSample(int sample_v, int sample_i, int sample_r)
{

	memset(OutStr, 0, sizeof(OutStr));	
	
	sprintf(OutStr, "SampleV: %d  \nSampleI: %d   \nSampleR: %d \n\n", sample_v, sample_i, sample_r);
	CommanderSendString(OutStr);

}


void CommanderSendSpeakerState(void)
{

	memset(OutStr, 0, sizeof(OutStr));	
	
	sprintf(OutStr, "SpeakerState: %d  \n", SpeakerState);
	CommanderSendString(OutStr);

}


void CommanderSendDC24VsState(void)
{

	memset(OutStr, 0, sizeof(OutStr));	
	
	sprintf(OutStr, "DC24V1State: %d  \nDC24V2State: %d  \n\n", DC24V1State, DC24V2State);
	CommanderSendString(OutStr);

}


void CommanderSendNoise(int db1, int db2, int db)
{

	memset(OutStr, 0, sizeof(OutStr));	
	
	sprintf(OutStr, "Noise1 dB: %d  \nNoise2 dB: %d  \nNoise dB: %d   \n\n", db1, db2, db);
	CommanderSendString(OutStr);

}


void CommanderSendEmeAudioTestFlag(void)
{
	memset(OutStr, 0, sizeof(OutStr));		
	sprintf(OutStr, "EmeTestFlag: %d  \n", EmeAudioTestFlag);
	CommanderSendString(OutStr);

}


void CommanderSendMyNoise(int mydb)
{

	memset(OutStr, 0, sizeof(OutStr));	
	
	sprintf(OutStr, "My Noise dB: %d  \n", mydb);
	CommanderSendString(OutStr);

}


void CommanderSendMyNoiseNew(int noise)
{

	memset(OutStr, 0, sizeof(OutStr));	
	
	sprintf(OutStr, "My Noise (new): %d  \n", noise);
	CommanderSendString(OutStr);

}

void CommanderSendSensorNoiseNew(int noise1, int noise2)
{

	memset(OutStr, 0, sizeof(OutStr));	
	
	sprintf(OutStr, "Sensor1 Noise (new): %d  \n", noise1);
	CommanderSendString(OutStr);

	sprintf(OutStr, "Sensor2 Noise (new): %d  \n", noise2);
	CommanderSendString(OutStr);

}




void CommanderSendI2cState(void)
{

	memset(OutStr, 0, sizeof(OutStr));	

	if(I2cError == 0)
	{
		sprintf(OutStr, "I2C OK!  \n");
	}
	else
	{
		sprintf(OutStr, "I2C Error: %d  \n\n\n", I2cError);

	}
	
	
	CommanderSendString(OutStr);

}



void CommanderSendAudioInputSampleV(void)
{

	memset(OutStr, 0, sizeof(OutStr));	
	
	sprintf(OutStr, "AudioInputSampleV: %d  \n", AudioInputSampleV);
	CommanderSendString(OutStr);

}


void CommanderSendAudioInputSampleVFigure(void)
{
	int i;

	memset(OutStr, 0, sizeof(OutStr));	

	for(i=0; i<(AudioInputSampleV/400); i++)
	{
		OutStr[i] = '#';

	}

	
	
	CommanderSendString(OutStr);

	CommanderSendString("\n");

}




void CommanderSendSelfTestReport(void)
{
	int i;
	

	memset(OutStr, 0, sizeof(OutStr));	


	sprintf(OutStr, "SelfTest Report:  \n");
	CommanderSendString(OutStr);
	

	sprintf(OutStr, "1K VCount: %d  \n", SelfTest1kVCount);
	CommanderSendString(OutStr);

	sprintf(OutStr, "1K ICount: %d  \n", SelfTest1kICount);
	CommanderSendString(OutStr);
	

	sprintf(OutStr, "Amplification: %d  \n", SelfTestAmplification);
	CommanderSendString(OutStr);

	sprintf(OutStr, "SNR: %d  \n", SelfTestSnr);
	CommanderSendString(OutStr);
	
	for(i=0; i<16; i++)
	{
		sprintf(OutStr, "Frequency Response: %d    %d  \n", SelfTestFrequencyResponse[i].Frequency, SelfTestFrequencyResponse[i].Value);
		CommanderSendString(OutStr);

	}

	sprintf(OutStr, "Output Power: %d  \n", SelfTestOutputPower);
	CommanderSendString(OutStr);
	

	sprintf(OutStr, "  \n");
	CommanderSendString(OutStr);

	sprintf(OutStr, "  \n");
	CommanderSendString(OutStr);
	

}


void CommanderUpdateProgramTest(void)
{
	u32 ip32;

	u32 ip1;
	u32 ip2;
	u32 ip3;
	u32 ip4;

	ip1 = 192;
	ip2 = 168;
	ip3 = 101;
	ip4 = 187;

	ip32 = (ip1<<24) | (ip2<<16) | (ip3<<8) | ip4;

	ip32 = ip32;


//	TftpClientIapStart(ip32, "PisAmp.bin");

}


void CommanderSendHttpInfo(char *pbuf, int len)
{
//	int ll;

//	ll = sizeof(OutStr);
	
	memset(OutStr, 0, sizeof(OutStr));	
	
	sprintf(OutStr, "len = %d; >> %s  \n\n", len, pbuf);

	CommanderSendString(OutStr);

}







