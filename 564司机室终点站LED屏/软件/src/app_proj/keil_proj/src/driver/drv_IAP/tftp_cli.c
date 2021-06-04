#include "tftp_cli.h"
#include "tftpserver.h"
#include "flash_if.h"
#include <string.h>
#include <stdio.h>
#include "iap_config.h"
#include "main.h"

static uint32_t Flash_Write_Address;
static struct udp_pcb *UDPpcb_tftp = NULL;
static __IO uint32_t total_count = 0;

const char *err_codes[8] =
{
	"Not defined",
	"File not found",
	"Access violation",
	"Disk full or allocation exceeded",
	"Illegal TFTP operation",
	"Unknown transfer ID",
	"File already exists",
	"No such user"
};

char *tftp_err_to_str(int err)
{
	if (err < 0 || err > 7)
	{ return NULL; }

	return (char *)err_codes[err];
}

static void tftp_recv_callback(void *arg, struct udp_pcb *upcb, struct pbuf *pkt_buf, struct ip_addr *addr, u16_t port);

int UdpSend(struct udp_pcb *upcb, struct ip_addr *to, uint16 to_port, char *pbuf, uint16 len)
{

	struct pbuf *p;
	err_t err;

	/* allocate pbuf from pool*/
	p = pbuf_alloc(PBUF_TRANSPORT, len, PBUF_POOL);

	if (p != NULL)
	{
		/* copy data to pbuf */
		err = pbuf_take(p, pbuf, len);

		if (err != ERR_OK)
		{
			/* free pbuf */
			pbuf_free(p);
			return -1;
		}

		/* send udp data */
		err = udp_sendto(upcb, p, to, to_port);

		/* free pbuf */
		pbuf_free(p);

		if (err == ERR_OK)
		{
			return 0;
		}
		else
		{
			return -1;
		}
	}

	return -1;


}

#define BLOCK_SIZE 512
#define MSGBUF_SIZE TFTP_DATA_HDR_LEN + BLOCK_SIZE
typedef  struct tftp_conn
{
	unsigned short op; /* Are we putting or getting? */
	unsigned short blocknr; /* The current block number */
	char *fname; /* The file name of the file we are putting or getting */
	char *mode; /* TFTP mode */
	struct udp_pcb *upcb;	/* Socket to communicate with server */
	/* destination ip:port */
	struct ip_addr to_ip;
	int to_port;

	/* total number of bytes transferred */
	int tot_bytes;
	char msgbuf[MSGBUF_SIZE]; /* Buffer for messages being sent or received */
} tftp_conn_t;

tftp_conn_t tftp_conn_cli;
/*
 Send a read request to the server.
 1. Format message.
 2. Send the request using the connection handle.
 3. Return the number of bytes sent, or negative on error.
 */
int tftp_send_rrq(tftp_conn_t *tc)
{
	int ret;
	/* struct tftp_rrq *rrq; */
	struct tftp_rrq *pTftp_rrq = (struct tftp_rrq *)malloc(TFTP_RRQ_LEN(tc->fname, tc->mode));
	memset(pTftp_rrq, 0, TFTP_RRQ_LEN(tc->fname, tc->mode));
	pTftp_rrq->opcode = htons(TFTP_RRQ);	//transform host ascii to net ascii
	memcpy(pTftp_rrq->req, tc->fname, strlen(tc->fname));
	memcpy(pTftp_rrq->req + strlen(tc->fname) + 1, tc->mode, strlen(tc->mode));
	/* send read request to the server */
	ret = UdpSend(tc->upcb, &tc->to_ip, tc->to_port, (char *) pTftp_rrq, (TFTP_RRQ_LEN(tc->fname, tc->mode)));
	free(pTftp_rrq);
	return ret;
}
#if 0
/*
 Send a write request to the server.
 1. Format message.
 2. Send the request using the connection handle.
 3. Return the number of bytes sent, or negative on error.
 */
int tftp_send_wrq(struct tftp_conn *tc)
{
	/* struct tftp_wrq *wrq; */
	struct tftp_wrq *pTftp_wrq = malloc(TFTP_WRQ_LEN(tc->fname, tc->mode));
	memset(pTftp_wrq, 0, TFTP_WRQ_LEN(tc->fname, tc->mode));
	pTftp_wrq->opcode = htons(OPCODE_WRQ);//transform host ascii to net ascii
	memcpy(pTftp_wrq->req, tc->fname, strlen(tc->fname));
	memcpy(pTftp_wrq->req + strlen(tc->fname) + 1, tc->mode, strlen(tc->mode));
	/* send write request to the server */
	int rqtResult = sendto(tc->sock, pTftp_wrq,
	                       TFTP_WRQ_LEN(tc->fname, tc->mode), 0,
	                       (struct sockaddr *)(&(tc->peer_addr)), (*tc).addrlen);
	free(pTftp_wrq);
	return rqtResult;
}
#endif

/*
 Acknowledge reception of a block.
 1. Format message.
 2. Send the acknowledgement using the connection handle.
 3. Return the number of bytes sent, or negative on error.
 */
int tftp_send_ack(struct tftp_conn *tc)
{
	int ret;
	/* struct tftp_ack *ack; */
	struct tftp_ack *pTftp_ack = (struct tftp_ack *) malloc(TFTP_ACK_HDR_LEN);
	pTftp_ack->opcode = htons(TFTP_ACK);//transform host ascii to net ascii
	/* Get receiving data block number from tc->msgbuf and ack to the server */
	//tc->blocknr = ntohs(((struct tftp_data *) tc->msgbuf)->blocknr);
	pTftp_ack->blocknr = htons(tc->blocknr);
	ret = UdpSend(tc->upcb, &tc->to_ip, tc->to_port, (char *) pTftp_ack, TFTP_ACK_HDR_LEN);
	free(pTftp_ack);
	return ret;
}

void erase_app(void)
{
	clr_prog_flag();
	/* init flash */
	FLASH_If_Init();

	while (FLASH_If_Erase_Application_sectors());

	FLASH_Lock();

	Flash_Write_Address = PROG_BAK_ADDRESS;
}

int FLASH_If_Write_tftp_cli(__IO uint32_t *FlashAddress, uint32_t *Data, uint16_t DataLength)
{
	uint32_t tmp_addr;

	tmp_addr = *FlashAddress;
	FLASH_If_Init();

	while (FLASH_If_Write(FlashAddress, Data , DataLength))
	{
		*FlashAddress = tmp_addr;
	}

	FLASH_Lock();

	return 0;
}

void tftp_cli_clearup_r(struct udp_pcb **UDPpcb_tftp)
{

	if (*UDPpcb_tftp)
	{
		udp_remove(*UDPpcb_tftp);
		*UDPpcb_tftp = NULL;
	}
}

int check_UDPpcb_tftp(void)
{
	if (UDPpcb_tftp)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

#define TFTP_cli_timeout 100		// 10 sec
static uint16 tftp_cli_timeout_cnt = 0;
void tftp_cli_timeout_set(uint16 val)
{
	tftp_cli_timeout_cnt = 0;
}

void tftp_cli_timeout_process(void)
{
	static uint32 timer_1 = 0;
	static struct udp_pcb *pcb_tmp = NULL;
	static uint8 flag = 1;

	if (flag)
	{
		flag = 0;
		pcb_tmp = UDPpcb_tftp;
		timer_1 = get_timer0_clock();
		tftp_cli_timeout_cnt = 0;
	}

	if (pcb_tmp != UDPpcb_tftp)
	{
		pcb_tmp = UDPpcb_tftp;
		timer_1 = get_timer0_clock();
		tftp_cli_timeout_cnt = 0;
	}

	if (pcb_tmp == NULL)
	{
		return;
	}

	if (timer0_check_timer(&timer_1, TIMING_100ms))
	{
		if (tftp_cli_timeout_cnt++ > TFTP_cli_timeout)
		{
			tftp_cli_clearup_r(&UDPpcb_tftp);
		}
	}

}

int tftp_connect(int opcode, char *filename, char *mode, unsigned char ip[4])
{
	err_t err;
	int remote_port = 69; /* 69 is the port used for TFTP protocol initial transaction */
	struct ip_addr remote_ip;

	tftp_conn_t *tc = &tftp_conn_cli;

	memset(tc, 0, sizeof(tftp_conn_t));

	IP4_ADDR(&remote_ip, ip[0], ip[1], ip[2], ip[3]);

	/* create a new UDP PCB structure  */
	UDPpcb_tftp = udp_new();

	if (!UDPpcb_tftp)
	{
		return -1;
	}

	/* Bind this PCB to port 69  */
	err = udp_bind(UDPpcb_tftp, IP_ADDR_ANY, 0);

	if (err != ERR_OK)
	{
		return -1;
	}

#if 0

	if (strlen(filename) >= sizeof(tc->fname))
	{
		return -1;
	}

	sprintf(tc->fname, "%s", filename);
#endif

	tc->fname = filename;
	tc->upcb = UDPpcb_tftp;
	tc->mode = mode;
	tc->to_ip = remote_ip;
	tc->to_port = remote_port;
	tc->op = opcode;

	/* Initialize receive callback function  */
	udp_recv(UDPpcb_tftp, tftp_recv_callback, tc);


	if (opcode == TFTP_RRQ)
	{
		if (tftp_send_rrq(tc) < 0)
		{
			return -1;
		}
		else
		{
			return 0;
		}
	}

	return -1;
}

int tftp_check_param(int opcode, char *filename, char *mode, unsigned char ip[4])
{
	// check param
	if ((opcode != TFTP_RRQ) && (opcode != TFTP_WRQ))
	{
		return -1;
	}

	if (strlen(filename) == 0)
	{
		return -1;
	}

	if ((strncmp(MODE_NETASCII, mode, strlen(MODE_NETASCII)) != 0)
	        && (strncmp(MODE_OCTET, mode, strlen(MODE_OCTET)) != 0))
	{
		return -1;
	}

	if ((ip[0] == 0) || (ip[1] == 0) || (ip[2] == 0) || (ip[3] == 0))
	{
		return -1;
	}

	return 0;
}

int tftp_cli_start(int opcode, char *filename, char *mode, unsigned char ip[4])
{

	if (UDPpcb_tftp)
	{
		// tcp客户端占用
		return -1;
	}

	// check param
	if ((opcode != TFTP_RRQ) && (opcode != TFTP_WRQ))
	{
		return -1;
	}

	if (strlen(filename) == 0)
	{
		return -1;
	}

	if ((strncmp(MODE_NETASCII, mode, strlen(MODE_NETASCII)) != 0)
	        && (strncmp(MODE_OCTET, mode, strlen(MODE_OCTET)) != 0))
	{
		return -1;
	}

	erase_app();

	tftp_connect(opcode, filename, mode, ip);

	return 0;

}

static void tftp_recv_callback(void *arg, struct udp_pcb *upcb, struct pbuf *pkt_buf,
                               struct ip_addr *addr, u16_t port)
{
	tftp_conn_t *tc = (tftp_conn_t *)arg;
	tftp_data_t *t_dat = NULL;
	uint8 *buf = (uint8 *)(pkt_buf->payload);
	uint8 end_f = 0;
	int buflen = pkt_buf->len;
	uint16 opcode;
	uint16 blocknr;
	uint16 blocknr_add1;

	end_f = 0;

	if (buflen > MSGBUF_SIZE)
	{
		buflen = MSGBUF_SIZE;
	}

	memcpy(tc->msgbuf, buf, buflen);
	tc->tot_bytes = buflen;

	t_dat = (tftp_data_t *)tc->msgbuf;

	opcode = ntohs(t_dat->opcode);

	if (tc->blocknr + 1 > 0xffff)
	{
		blocknr_add1 = 0;
	}
	else
	{
		blocknr_add1 = tc->blocknr + 1;
	}


	switch (opcode)
	{
		case TFTP_DATA:
		{
			blocknr = ntohs(t_dat->blocknr);

			if (tc->tot_bytes < TFTP_DATA_HDR_LEN)
			{
				// 如果 tftp传输的文件恰好为512的整数倍，则最后要发送一个空的数据包,所以数据包长度可以是 0
				// err
				break;
			}

			if (blocknr == 0)
			{
				blocknr = 0;
			}

			if ((blocknr == 1) && (blocknr_add1 == blocknr))
			{
				tc->to_ip = *addr;
				tc->to_port = port;
				//Flash_Write_Address = PROG_BAK_ADDRESS;
			}

			if (blocknr != 1)
			{
				if ((tc->to_ip.addr != (*addr).addr) || (tc->to_port != port))
				{
					// err
					break;
				}
			}

			if ((blocknr_add1 > blocknr) || ((blocknr_add1 == 0) && (blocknr == 0xffff)))
			{
				// 期望blocknr_add1与tftp服务器实际发送的blocknr不一致时的容错处理
				// 1. 期望blocknr_add1 之前的所有包 只回ack，不处理
				// 2. 当恰好达到最大blocknr时 只回ack，不处理
				struct tftp_ack Tftp_ack;
				struct tftp_ack *pTftp_ack = &Tftp_ack;
				pTftp_ack->opcode = htons(TFTP_ACK);
				pTftp_ack->blocknr = htons(blocknr);
				UdpSend(tc->upcb, &tc->to_ip, tc->to_port, (char *) pTftp_ack, TFTP_ACK_HDR_LEN);
				break;
			}
			else if (blocknr_add1 == blocknr)
			{
				// ok write flash
				uint32_t *p_wrdat = (uint32_t *)t_dat->data;
				uint16 count = ((tc->tot_bytes - TFTP_DATA_HDR_LEN) % 4) ? ((tc->tot_bytes - TFTP_DATA_HDR_LEN) / 4 + 1) : ((tc->tot_bytes - TFTP_DATA_HDR_LEN) / 4);
				FLASH_If_Write_tftp_cli(&Flash_Write_Address, p_wrdat , count);
#if 0
				{
					uint32_t data_buffer[128];
					uint32_t Flash_Write_Address_tmp;

					pbuf_copy_partial(pkt_buf, data_buffer, pkt_buf->len - TFTP_DATA_PKT_HDR_LEN,
					                  TFTP_DATA_PKT_HDR_LEN);

					count = (pkt_buf->len - TFTP_DATA_PKT_HDR_LEN) / 4;

					if (((pkt_buf->len - TFTP_DATA_PKT_HDR_LEN) % 4) != 0)
					{ count++; }

					/* Write received data in Flash */
					Flash_Write_Address_tmp = Flash_Write_Address;
					FLASH_If_Init();

					while (FLASH_If_Write(&Flash_Write_Address, data_buffer , count)) { Flash_Write_Address = Flash_Write_Address_tmp; }

					FLASH_Lock();
				}
#endif

				tc->blocknr = blocknr_add1;
				tftp_send_ack(tc);

				if (tc->tot_bytes < MSGBUF_SIZE)
				{
					end_f = 1;
				}
			}

			break;
		}

		case TFTP_ERROR:
		{
			break;
		}

		default:
		{
			break;
		}
	}

	pbuf_free(pkt_buf);

	if (end_f)
	{
		tftp_cli_clearup_r(&UDPpcb_tftp);
		run_app();
	}

	tftp_cli_timeout_set(0);

}


