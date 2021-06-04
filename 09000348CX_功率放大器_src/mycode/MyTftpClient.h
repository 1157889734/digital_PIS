//*******************************************************//
//						MyTftpClient.h								//
//*******************************************************//

#ifndef _MYTFTPCLIENT_H
#define  _MYTFTPCLIENT_H

#include "MyAll.h"





void TftpClientInit(void);

void TftpClientProcess(void);
void TftpClient100ms(void);

void TftpClientIapStart(struct ip_addr server_ip, char *filename);


#endif


