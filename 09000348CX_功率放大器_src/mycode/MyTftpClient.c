
//*****************************************************//
//		MyTftpClient.c
//*****************************************************//

//Includes ------------------------------------------------------------------*/
#include "lwip/pbuf.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/tcp.h"
#include "lwip/udp.h"
#include "netif/etharp.h"
#include "lwip/dhcp.h"


#include "ethernetif.h"


#include <stdio.h>

#include "MyAll.h"




/////////////////////////////////////////////////////////////////////

#define TFTP_PORT_SERVER		69
#define TFTP_PORT_CLIENT		22169


#define TFTP_OPCODE_RRQ		1	//Tftp opcode, 1 means Read Request.
#define TFTP_OPCODE_WRQ		2
#define TFTP_OPCODE_DATA	3
#define TFTP_OPCODE_ACK		4
#define TFTP_OPCODE_ERR		5


/////////////////////////////////////////////////////////////////////


struct udp_pcb *UDPpcb;
struct ip_addr ServerIp;

int ServerPort;

int ReadFlag;
int ReadStep;
int ReadTimer;

u8 DaBuf[516];

int ReadFileOffset;


/////////////////////////////////////////////////////////////////////

void TftpClientRecvCallback(void *arg, struct udp_pcb *upcb, struct pbuf *pkt_buf, struct ip_addr *addr, u16_t port);

void DataRecv(u8 *pbuf, int len);
void SendAck(void);
void SendNum(u16 num);


/////////////////////////////////////////////////////////////////////



/**
  * @brief  Creates and initializes a UDP PCB for TFTP receive operation  
  * @param  none  
  * @retval none
  */
void TftpClientInit(void)
{
	err_t err;

	
	IP4_ADDR(&ServerIp, 192, 168, 101, 187);

	/* create a new UDP PCB structure  */
	UDPpcb = udp_new();
	if (!UDPpcb) {
		return ;
	}

	/* Bind this PCB   */
	err = udp_bind(UDPpcb, IP_ADDR_ANY, TFTP_PORT_CLIENT);
	if (err == ERR_OK) {
		/* Initialize receive callback function  */
		udp_recv(UDPpcb, TftpClientRecvCallback, NULL);
	} 
	else {
		err = err;
	}
	
	ReadFlag = 0;
	ReadStep = 0;
	ReadTimer = 0;

	ReadFileOffset = 0;

	ServerPort = TFTP_PORT_SERVER;

}



/**
  * @brief  Processes traffic received on UDP
  * @param  args: pointer on tftp_connection arguments
  * @param  upcb: pointer on udp_pcb structure
  * @param  pbuf: pointer on packet buffer
  * @param  addr: pointer on the receive IP address
  * @param  port: receive port number
  * @retval none
  */
void TftpClientRecvCallback(void *arg, struct udp_pcb *upcb, struct pbuf *pkt_buf, struct ip_addr *addr, u16_t port)
{
	int opcode;
	int err_code;


	ServerPort = port;
	
	opcode = *(((u8 *)(pkt_buf->payload))+1);
	
	if(opcode == TFTP_OPCODE_DATA) {
		DataRecv((u8*)(pkt_buf->payload), (int)(pkt_buf->len));
	}
	else if(opcode == TFTP_OPCODE_ERR) {
		err_code = *(((u8 *)(pkt_buf->payload))+3);
		
		err_code = err_code;

	}

	pbuf_free(pkt_buf);
	  
}


/**
  * @brief  Tftp Client Send Buffer
  * @param  pbuf: pointer on buffer to send
  * @param  len: length of buffer to send
  * @retval none
  */
void TftpClientSendBuf(u8* pbuf, int len)
{
	struct pbuf *p;

	/* Connect to the remote client */
	udp_connect(UDPpcb, &ServerIp, ServerPort);
  
	/* allocate pbuf from pool*/
	p = pbuf_alloc(PBUF_TRANSPORT,len, PBUF_POOL); 
	if (p != NULL) {
		/* copy data to pbuf */
		pbuf_take(p, (u8*)pbuf, len);
          
		/* send udp data */
		udp_send(UDPpcb, p); 

		/* free the UDP connection, so we can accept new clients */
		udp_disconnect(UDPpcb);
			
		/* free pbuf */
		pbuf_free(p);
		
	}

}


void TftpClientProcess(void)
{


}


void TftpClient100ms(void)
{

	if(ReadFlag > 0) {

		ReadTimer ++;
		if(ReadTimer > 200) {
			ReadFlag = 0;
			ReadStep = 0;
			ReadTimer = 0;

			ReadFileOffset = 0;
		}
	}

}


void TftpClientIapStart(struct ip_addr server_ip, char *filename)
{
	u8 packet[64];
	u8 *p;
	int index;
	int len;
	char *mode_str = "octet";
	
	
	ServerIp = server_ip;
	
	p = packet;
	memset(p, 0, 64);

	*p = 0;
	*(p+1) = TFTP_OPCODE_RRQ;		

	index = 2;
	memcpy((p+index), filename, strlen(filename));

	index += strlen(filename);
	index += 1;
	memcpy((p+index), mode_str, strlen(mode_str));
	
	index += strlen(mode_str);
	len = index + 1;

	TftpClientSendBuf(p, len);

	ReadFlag = 1;
	ReadStep = 1;
	ReadTimer = 1;

	ReadFileOffset = 0;

	FlashInit();

}


void DataSave(int offset, u8 *pbuf, int len)
{
	int len32;

	len32 = len / 4;
	if((len % 4) > 0) {
		len32 += 1;
	}
	
	FlashWrite(offset, ((u32 *)(pbuf)), len32);

}

void DownloadFinish(int len)
{


	//flashcopy;
	FlashProgramCopy(ReadFileOffset);


}


void DataRecv(u8 *pbuf, int len)
{
	int block_num;
	

	if(ReadFlag == 0) {
		return;
	}

	block_num = ((int)(*(pbuf+2))<<8) + *(pbuf+3);

	if(block_num == ReadStep) {
		if(len == 516) {
			memcpy(DaBuf, (pbuf+4), (len-4));

			SendAck();
			
			DataSave(ReadFileOffset, DaBuf, (len-4));

			ReadFileOffset += (len - 4);

		}
		else if(len < 516) {
			memcpy(DaBuf, (pbuf+4), (len-4));

			SendAck();

			DataSave(ReadFileOffset, DaBuf, (len-4));

			ReadFileOffset += (len - 4);
			
			//flashcopy;
			DownloadFinish(ReadFileOffset +4);

			ReadFlag = 0;

		}
		else if(len > 516) {
			len = len;
		}

		ReadTimer = 1;

	}
	else
	{
		SendNum(block_num);
	}

}

void SendAck(void)
{
	u8 pkt[4];

	pkt[0] = 0;
	pkt[1] = TFTP_OPCODE_ACK;
	pkt[2] = (u8)(ReadStep<<8);
	pkt[3] = (u8)(ReadStep);

	TftpClientSendBuf(pkt, 4);

	if(ReadFlag > 0) {
		ReadStep++;
	}
	else {
		ReadStep = 0;
		ReadTimer = 0;
		ReadFileOffset = 0;
	}
	
}

void SendNum(u16 num)
{
	u8 pkt[4];

	pkt[0] = 0;
	pkt[1] = TFTP_OPCODE_ACK;
	pkt[2] = (u8)(num <<8);
	pkt[3] = (u8)(num);
	TftpClientSendBuf(pkt, 4);
} 



