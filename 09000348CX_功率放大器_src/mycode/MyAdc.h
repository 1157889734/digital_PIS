//*******************************************************//
//						MyAdc.h								//
//*******************************************************//

#ifndef _MYADC_H
#define  _MYADC_H

#include "MyAll.h"





void AdcInit(void);

u16 AdcGetValue1(void);
u16 AdcGetValue2(void);


int AdcGetFilterValue1(void);
int AdcGetFilterValue2(void);


int AdcNewGetValue1(void);
int AdcNewGetValue2(void);





#endif


