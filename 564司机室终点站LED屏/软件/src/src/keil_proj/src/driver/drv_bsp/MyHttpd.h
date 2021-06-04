//*******************************************************//
//						   			      					  //
//*******************************************************//

#ifndef _MYHTTPD_H
#define  _MYHTTPD_H

#include "MyAll.h"




#define HTTPD_TCP_PRIO                      TCP_PRIO_NORMAL

#define HTTPD_SERVER_PORT                   80





struct http_state {

	char *buf;       /* Pointer to first unsent byte in buf. */


	u32_t left;       /* Number of unsent bytes in buf. */
	u8_t retries;

};







void HttpdInit(void);

void check_iap_req_process(void);
void set_run_iap_req(uint8 val);



 

#endif
