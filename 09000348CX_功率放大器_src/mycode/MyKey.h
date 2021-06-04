//*******************************************************//
//						MyKey.h								//
//*******************************************************//

#ifndef _MYKEY_H
#define  _MYKEY_H

#include "MyAll.h"


#define KEY_VALUE_NULL	0
#define KEY_VALUE_UP		1
#define KEY_VALUE_DOWN	2



extern int KeyValue;




void KeyInit(void);

void Key10ms(void);


void Key100ms(void);






#endif


