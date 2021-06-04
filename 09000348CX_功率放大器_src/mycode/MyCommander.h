//*******************************************************//
//						MyCommander.h		             //
//*******************************************************//

#ifndef _MYCOMMANDER_H
#define  _MYCOMMANDER_H

#include "MyAll.h"






void CommanderInit(void);


void CommanderSendString(char* pstr);

void Commander500ms(void);


void CommanderReceive(u8* pbuf, int len);




void CommanderSendLocalIp(void);
void CommanderSendDeviceType(void);
void CommanderSendDeviceId(void);
void CommanderSendTrainId(void);

void CommanderSendAdcValue(int value1, int value2);


void CommanderSendAddr(void);

void CommanderSendAmpState(void);

void CommanderSendSample(int sample_v, int sample_i, int sample_r);

void CommanderSendSpeakerState(void);

void CommanderSendDC24VsState(void);

void CommanderSendNoise(int db1, int db2, int db);


void CommanderSendEmeAudioTestFlag(void);

void CommanderSendMyNoise(int mydb);



void CommanderSendMyNoiseNew(int noise);

void CommanderSendSensorNoiseNew(int noise1, int noise2);



void CommanderSendI2cState(void);


void CommanderSendAudioInputSampleV(void);


void CommanderSendAudioInputSampleVFigure(void);


void CommanderSendSelfTestReport(void);


void CommanderUpdateProgramTest(void);
void CommanderSendHttpInfo(char *pbuf, int len);





#endif


