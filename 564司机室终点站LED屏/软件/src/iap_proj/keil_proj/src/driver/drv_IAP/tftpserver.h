
#ifndef __TFTPSERVER_H_
#define __TFTPSERVER_H_

#include "lwip/mem.h"
#include "lwip/udp.h"



#define TFTP_OPCODE_LEN         2
#define TFTP_BLKNUM_LEN         2
#define TFTP_DATA_LEN_MAX       512
#define TFTP_DATA_PKT_HDR_LEN   (TFTP_OPCODE_LEN + TFTP_BLKNUM_LEN)
#define TFTP_ERR_PKT_HDR_LEN    (TFTP_OPCODE_LEN + TFTP_ERRCODE_LEN)
#define TFTP_ACK_PKT_LEN        (TFTP_OPCODE_LEN + TFTP_BLKNUM_LEN)
#define TFTP_DATA_PKT_LEN_MAX   (TFTP_DATA_PKT_HDR_LEN + TFTP_DATA_LEN_MAX)
#define TFTP_MAX_RETRIES        3
#define TFTP_TIMEOUT_INTERVAL   5

typedef struct
{
  int op;    /*WRQ */
  /* last block read */
  char data[TFTP_DATA_PKT_LEN_MAX];
  int  data_len;
  /* destination ip:port */
  struct ip_addr to_ip;
  int to_port;
  /* next block number */
  int block;
  /* total number of bytes transferred */
  int tot_bytes;
  /* timer interrupt count when last packet was sent */
  /* this should be used to resend packets on timeout */
  unsigned long long last_time;
 
}tftp_connection_args;


/* TFTP opcodes as specified in RFC1350   */
typedef enum {
  TFTP_RRQ = 1,
  TFTP_WRQ = 2,
  TFTP_DATA = 3,
  TFTP_ACK = 4,
  TFTP_ERROR = 5
} tftp_opcode;


/* TFTP error codes as specified in RFC1350  */
typedef enum {
  TFTP_ERR_NOTDEFINED = 0,
  TFTP_ERR_FILE_NOT_FOUND,
  TFTP_ERR_ACCESS_VIOLATION,
  TFTP_ERR_DISKFULL,
  TFTP_ERR_ILLEGALOP,
  TFTP_ERR_UKNOWN_TRANSFER_ID,
  TFTP_ERR_FILE_ALREADY_EXISTS,
  TFTP_ERR_NO_SUCH_USER,
} tftp_errorcode;


#define MODE_NETASCII "netascii"
#define MODE_OCTET    "octet"
#define MODE_MAIL     "mail"


/* Timeout in seconds */
#define TFTP_TIMEOUT 2

/*
  A generic header for TFTP messages.
 */
__packed typedef  struct tftp_msg {
	unsigned short opcode;
	char msg[1];
}tftp_msg_t;

/*
  A TFTP read request.
 */
__packed typedef  struct tftp_rrq {
	unsigned short opcode;
	char req[1];
}tftp_rrq_t;

#define TFTP_RRQ_HDR_LEN (sizeof(struct tftp_rrq) - 1)
#define TFTP_RRQ_LEN(f,m) (TFTP_RRQ_HDR_LEN + strlen(f) + strlen(m) + 2)

/*
  A TFTP write request.
 */
__packed typedef  struct tftp_wrq {
	unsigned short opcode;
	char req[1];
}tftp_wrq_t;

#define TFTP_WRQ_HDR_LEN (sizeof(struct tftp_wrq) - 1)
#define TFTP_WRQ_LEN(f,m) (TFTP_WRQ_HDR_LEN + strlen(f) + strlen(m) + 2)

/*
  A TFTP data block message.
 */
__packed typedef  struct tftp_data {
	unsigned short opcode;
	unsigned short blocknr;
	char data[1];
}tftp_data_t;

#define TFTP_DATA_HDR_LEN (sizeof(struct tftp_data) - 1)

/*
  A TFTP ack message.
 */
__packed typedef  struct tftp_ack {
	unsigned short opcode;
	unsigned short blocknr;
}tftp_ack_t;

#define TFTP_ACK_HDR_LEN sizeof(struct tftp_ack)

/*
  A TFTP error message.
 */
__packed typedef  struct tftp_err {
	unsigned short opcode;
	unsigned short errcode;
	char errmsg[1];
}tftp_err_t;

#define TFTP_ERR_HDR_LEN (sizeof(struct tftp_err) - 1)




void IAP_tftpd_init(void);


#endif

