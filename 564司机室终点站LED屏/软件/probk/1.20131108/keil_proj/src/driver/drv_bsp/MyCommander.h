//*******************************************************//
//						MyCommander.h		             //
//*******************************************************//

#ifndef _MYCOMMANDER_H
#define  _MYCOMMANDER_H

#include "MyAll.h"






void CommanderInit(void);


void CommanderSendString(char* pstr);


void CommanderReceive(u8* pbuf, int len);




void CommanderSendLocalIp(void);
void CommanderSendDeviceType(void);
void CommanderSendDeviceId(void);
void CommanderSendTrainId(void);





#endif


