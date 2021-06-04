//*******************************************************//
//						MySelfTest.h								//
//*******************************************************//

#ifndef _MYSELFTEST_H
#define  _MYSELFTEST_H

#include "MyAll.h"






typedef struct 
{
	int Frequency;

	int Value;

	
}SelfTestFrequencyResponseTypeDef;





extern int SelfTest1kVCount;
extern int SelfTest1kVVolt;

extern int SelfTest1kICount;
extern int SelfTest1kIA;


extern int SelfTestAmplification;

extern int SelfTestSnr;

extern SelfTestFrequencyResponseTypeDef SelfTestFrequencyResponse[16];

extern int SelfTestOutputPower;




void SelfTestInit(void);


int SelfTestStart(void);

int SelfTestStop(void);


void SelfTest100ms(void);

void SelfTest500ms(void);

void SelfTestProcess(void);



void SelfTestFrequencyResponseReset(void);


void SelfTestSendReport(void);


void SelfTestGetAmplification(void);

void SelfTestGetOutputPower(void);

void SelfTestGetSnr(void);

int SelfTestGetFrequencyResponse(int frequency);






#endif


