//*******************************************************//
//						MyApp.h								//
//*******************************************************//

#ifndef _MYAPP_H
#define  _MYAPP_H

#include "MyAll.h"




#define AMPSTATE_OK			0
#define AMPSTATE_OVP			1
#define AMPSTATE_UVP			2
#define AMPSTATE_SHORT		3
#define AMPSTATE_ERR			4


#define NDSTATE_NULL			0
#define NDSTATE_ONLY1		1
#define NDSTATE_ONLY2		2
#define NDSTATE_BOTH			3

#define CCVC_ST_CC         1
#define CCVC_ST_MC         2
#define CCVC_ST_CC_AC         3
#define CCVC_ST_MC_AC         4
#define CCVC_ST_MUTE     5

#define CCVC_ST_TEST       1
#define CCVC_AUDIO_MUTE    100


#define CCSET0DB    	       0
#define CCSET1DB        	1
#define CCSET2DB         	2
#define CCSET3DB              3
#define CCSET6DB              6
#define CCSET9DB              9
#define CCSET12DB            12
#define CCSET15DB            15

#define PCSET0DB    	       0
#define PCSET1DB        	1
#define PCSET2DB         	2
#define PCSET3DB              3
#define PCSETMUTE           5

extern u8 DevType;
extern u8 DevId;

extern u16 TrainId;


extern int AppStartTimer;
extern int AppStartFlag;


extern u8 Addr;

extern int AmpState;

extern int DC24V1State;
extern int DC24V2State;


extern int NoiseDetectState;

extern u8 CurrentState;

extern u8 AppVersion1;
extern u8 AppVersion2;
extern u8 AppVersion3;
extern u8 AppVersion4;

extern u8 CcState;
extern int CcDbSet;
extern int PcDbSet;
extern int PcDbSet_Tmp;
extern u8 CcTestSet;





void AppInit(void);

void AppOff(void);

void AppStart(void);


void AppProcess(void);


void App10ms(void);

void App100ms(void);

void App1000ms(void);



int SetDeviceId(int inda);

int SetTrainId(int inda);


void DisplayDb(void);

void LedLight(int num, int brightness);

int LedGetBrightness(int num);


void LedLightProcess(void);



int GetAmpState(void);


void GetDC24Vs(void);

void GetNoiseDetectState(void);


void GetCurrentState(void);

void VolumeDevProcess(void);

extern u8 Volumeset(int volpercent,int totalpercent,int totaldb);








#endif


