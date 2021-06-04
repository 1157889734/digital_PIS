//*******************************************************//
//						MyApp.h								//
//*******************************************************//

#ifndef _MYAPP_H
#define  _MYAPP_H

#include "MyAll.h"




extern u8 DevType;
extern u8 DevId;

extern u16 TrainId;


extern int AppStartTimer;
extern int AppStartFlag;



void AppInit(void);

void AppStart(void);


void AppProcess(void);

void App100ms(void);

void App1000ms(void);



int SetDeviceId(int inda);

int SetTrainId(int inda);





#endif


