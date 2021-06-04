
//*****************************************************//
//		MySelfTest.c
//*****************************************************//


#include "MyAll.h"




int SelfTestStartFlag;
int SelfTestStep;


int SelfTest1kVCount;
int SelfTest1kVVolt;

int SelfTest1kICount;
int SelfTest1kIA;



int SelfTestAmplification;

int SelfTestSnr;

SelfTestFrequencyResponseTypeDef SelfTestFrequencyResponse[16];


int SelfTestOutputPower;




void SelfTestInit(void)
{


	



}









int SelfTestStart(void)
{

	if(AudioPlayInfo.EnableFlag > 0)
	{
		return -1;

	}

	SelfTestStartFlag = 1;
	SelfTestStep = 1;

	SelfTestFrequencyResponseReset();


	return 0;

}


int SelfTestStop(void)
{

	AudioPlayTestSin(0, 0);

	SelfTestStartFlag = 0;
	SelfTestStep = 0;



	return 0;

}







void SelfTest100ms(void)
{

	if(SelfTestStartFlag > 0)
	{

//		CommanderSendAudioInputSampleV();
		CommanderSendAudioInputSampleVFigure();
		

	}


}



void SelfTest500ms(void)
{
	SelfTestProcess();


}



void SelfTestProcess(void)
{

	if(SelfTestStartFlag < 1)
	{
		SelfTestStep = 0;

	}
	else if(SelfTestStartFlag > 0)
	{
		if(SelfTestStep == 0)
		{

		}
		else if(SelfTestStep == 1)
		{
			//start
			AudioPlayTestSin(1, 1000);

			SelfTestStep++;

		}
		else if(SelfTestStep == 2)
		{
			SelfTestGetAmplification();
			SelfTestGetOutputPower();
			
			//and then
			AudioPlayTestSin(0, 0);

			SelfTestStep++;

		}
		else if(SelfTestStep == 3)
		{
			SelfTestGetSnr();
			
			//and then
			AudioPlayTestSin(1, 100);

			SelfTestStep++;

		}
		else if(SelfTestStep == 4)
		{

			SelfTestGetFrequencyResponse(100);
			
			//and then
			AudioPlayTestSin(1, 200);

			SelfTestStep++;

		}
		else if(SelfTestStep == 5)
		{
			SelfTestGetFrequencyResponse(200);

			AudioPlayTestSin(1, 500);

			SelfTestStep++;

		}
		else if(SelfTestStep == 6)
		{
			SelfTestGetFrequencyResponse(500);
			
			AudioPlayTestSin(1, 800);

			SelfTestStep++;

		}
		else if(SelfTestStep == 7)
		{
			SelfTestGetFrequencyResponse(800);
			
			AudioPlayTestSin(1, 1000);

			SelfTestStep++;

		}
		else if(SelfTestStep == 8)
		{
			SelfTestGetFrequencyResponse(1000);

			AudioPlayTestSin(1, 2000);

			SelfTestStep++;

		}
		else if(SelfTestStep == 9)
		{
			SelfTestGetFrequencyResponse(2000);

			AudioPlayTestSin(1, 5000);

			SelfTestStep++;

		}
		else if(SelfTestStep == 10)
		{
			SelfTestGetFrequencyResponse(5000);

			AudioPlayTestSin(1, 8000);

			SelfTestStep++;

		}
		else if(SelfTestStep == 11)
		{
			SelfTestGetFrequencyResponse(8000);

			AudioPlayTestSin(1, 10000);

			SelfTestStep++;

		}
		else if(SelfTestStep == 12)
		{
			SelfTestGetFrequencyResponse(10000);

			AudioPlayTestSin(1, 15000);

			SelfTestStep++;

		}
		else if(SelfTestStep == 13)
		{
			SelfTestGetFrequencyResponse(15000);

			AudioPlayTestSin(0, 0);

			SelfTestStop();

			SelfTestSendReport();
			
		}


	}


}




void SelfTestFrequencyResponseReset(void)
{
	int i;

	for(i=0; i<16; i++)
	{
		SelfTestFrequencyResponse[i].Frequency = 0;
		SelfTestFrequencyResponse[i].Value = 0;

	}

}





void SelfTestSendReport(void)
{
	CommanderSendSelfTestReport(); 

	NetControlSendSelfTestReport();

}




void SelfTestGetAmplification(void)
{

	SelfTest1kVCount = AudioInputSampleV;
	SelfTest1kICount = AudioInputSampleI;

	SelfTestAmplification = (AudioInputSampleV*128) / 65000;

	if(SelfTestAmplification > 200)
	{
		SelfTestAmplification = 200;

	}


}



void SelfTestGetOutputPower(void)
{

	SelfTestOutputPower = ((SelfTest1kVCount/10)*(SelfTest1kICount/10)) / 1728000;

	if(SelfTestOutputPower < 1)
	{
		SelfTestOutputPower = 1;

	}
	else if(SelfTestOutputPower > 200)
	{
		SelfTestOutputPower = 200;

	}
	


}



void SelfTestGetSnr(void)
{
	int noise;

	noise = AudioInputSampleV;

	if(noise > 100)
	{
		noise = noise - 100;

	}

	noise = noise / 2;

	SelfTestSnr = 20 * (log10(SelfTest1kVCount / noise));

	SelfTestSnr = 20 * (log10(SelfTest1kVCount / noise)) * 2;


	if(SelfTestSnr < 1)
	{
		SelfTestSnr = 1;

	}
	else if(SelfTestSnr > 200)
	{
		SelfTestSnr = 200;

	}


}




int SelfTestGetFrequencyResponse(int frequency)
{
	int i;

	int value;
	

	i = 0;

	while(SelfTestFrequencyResponse[i].Frequency > 0)
	{
		i++;

		if(i > 16)
		{
			return -1;

		}

	}


	value = 20 * (log10((double)AudioInputSampleV / (double)SelfTest1kVCount));

//	value = log10(0.001);
	

	SelfTestFrequencyResponse[i].Frequency = frequency;
	SelfTestFrequencyResponse[i].Value = value;
	

	
	return 0;

}















