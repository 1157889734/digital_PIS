//*****************************************************//

//*****************************************************//



#include "lwip/pbuf.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/tcp.h"
#include "lwip/udp.h"
#include "netif/etharp.h"
#include "lwip/dhcp.h"



#include "MyAll.h"  

#define HTTPTEST	"<html> \n<body> \n\
				<h1>北海通信  Beihai Communication</h1>\n\
				<p><a href=\"AAA\">Test AAA%s</a> </p>\n\
				<p><a href=\"BBB\">Test BBB</a></p>\n\
				<p>\
				<input type=\"text\" name=\"as_q\" class=\"box\"  id=\"searched_content\" title=\"12345\" />\
				<input type=\"submit\" value=\"Go\" class=\"button\" title=\"789\" />\
				</p>\n\
				</body>\n</html>\n"




#define HTTPD_TCP_PRIO        	TCP_PRIO_NORMAL
#define HTTPD_SERVER_PORT   	80


struct http_state {

	char *buf;       /* Pointer to first unsent byte in buf. */

	u32_t left;       /* Number of unsent bytes in buf. */
	u8_t retries;

};


char HttpTest[1024] ;


char HttpTestAAA[128] = "<html> \n<body> \n\
<h1>Test AAA</h1>\n\
<p><a href=\"index.html\">Back</a></p>\n\
</body>\n</html>\n" ;

char HttpTestBBB[128] = "<html> \n<body> \n\
<h1>Test BBB</h1>\n\
<p><a href=\"index.html\">Back</a></p>\n\
</body>\n</html>\n" ;



const char PageLogin[] = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n\
<html>\
	<center>\
		<h2>用户登录</h2>\
	</center>\
	<body>\
		<form method=\"get\" action=\"login_submit\">\
			<table width=\"100%%\" border=\"0\">\
				<tr><td align=\"center\">Username<input type=\"text\" name=\"username\" /></td></tr>\
				<tr><td align=\"center\">Password<input type=\"password\" name=\"password\" /></td></tr>\
				<tr><td align=\"center\"><input type=\"submit\" value=\"登陆\" name=\"submit\" /></td></tr>\
			</table>\
		</form>\
	</body>\
</html>";


const char Page1[] = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n\
<html>\
	<body bgcolor=\"#99bbbb\">\
		<p align=\"center\">  设备ID:%d      </p>\
		<p align=\"center\">  版本号:%d.%d     </p>\
	</body>\
</html>";


const char PageLogErr[] = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n\
<html>\
	<body>\
		<center>\
			<form action=\"log_err_submit\" method=\"get\">\
				<table align=\"center\" cellpadding=\"10\">\
					<tr bgcolor=\"#00FF33\">\
						<td align=\"center\">Error<br>Invalid User name or Password</td>\
					</tr>\
					<tr>\
						<td align=\"center\"><input type=\"submit\" name=\"back\" value=\"ok\"></td>\
					</tr>\
				</table>\
			</form>\
		</center>\
	</body>\
</html>";

const char PageFrameMain[] = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n\
<html>\
	<frameset cols=\"15%,85%\">\
		<frame src=\"frame_index.htm\">\
		<frame src=\"frame1.htm\" name=\"show\">\
	</frameset>\
</html>";

const char PageFrameIndex[] = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n\
<html>\
	<body bgcolor=\"#66bbbb\">\
		<p align=\"right\">  <a href=\"frame1.htm\" target=\"show\">基本信息</a>     </p>\
		<p align=\"right\">  <a href=\"frame2.htm\" target=\"show\">软件升级</a>     </p>\
	</body>\
</html>";

const char PageFrame1[] = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n\
<html>\
	<center>\
		<h2>设备信息</h2>\
	</center>\
	<body bgcolor=\"#99bbbb\">\
		<p align=\"center\">  设备类型: 数字功放      </p>\
		<p align=\"center\">  设备ID:%d      </p>\
		<p align=\"center\">  版本号:%d.%d.%d.%d     </p>\
	</body>\
</html>";

const char PageFrame2[] = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n\
<html>\
	<body bgcolor=\"#99bbbb\">\
		<p align=\"center\">  暂时不支持    </p>\
	</body>\
</html>";

const char PageUpdate[] =  "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n\
<html>\
	<body bgcolor=\"#99bbbb\">\
		<center>\
			<h2>软件更新维护</h2>\
		</center>\
		<form action=\"update_fw\" method=\"get\">\
			<table cellspacing=\"5\" width=\"100%%\" border=\"1\" cellpadding=\"5\" >\
				<tr>\
					<td align=\"center\">\
						目标文件:<input type=\"text\" name=\"filename\" />\
						<br />\
					</td>\
				</tr>\
				<tr>\
					<td align=\"center\" colspan=\"1\">\
						<input type=\"checkbox\" name=\"all_dev\" /><b>操作所有同类设备</b>\
						<br />\
						<input type=\"submit\" name=\"modify\" value=\"Update\" />\
						<br />\
					</td>\
				</tr>\
			</table>\
		</form>\
		<p>\
			<em>说明: 首先打开本机TFTP服务器，并准备好了相应的目标程序；</em>\
			<br />\
			<em>输入目标文件名,按\"Update\"按钮更新程序。</em>\
			<br />\
		</p>\
	</body>\
</html>";


const char PageUpdateInputErr[] = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n\
<html>\
	<body bgcolor=\"#99bbbb\">\
		<center>\
			<h3><br />信息输入有误，请重新输入!</h3>\
		</center>\
	</body>\
</html>";

const char PageUpdating[] = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n\
<html>\
	<body bgcolor=\"#99bbbb\">\
		<center>\
			<h3><br />程序更新中...,请耐心等待!<br />并注意本地电脑的TFTP工具的提示信息,当程序下载完毕后,重新登陆设备,以验证程序更新是否成功。</h3>\
		</center>\
	</body>\
</html>";




char PageBuf[1024];



/* Private function prototypes -----------------------------------------------*/
static err_t HttpAccept(void *arg, struct tcp_pcb *pcb, err_t err);
static err_t HttpRecv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err);


void SendPageLogin(struct tcp_pcb *pcb, struct http_state *hs, char *inda);
void SendPage1(struct tcp_pcb *pcb, struct http_state *hs, char *inda);
void SendPageLogErr(struct tcp_pcb *pcb, struct http_state *hs, char *inda);
void SendPageFrameMain(struct tcp_pcb *pcb, struct http_state *hs, char *inda);
void SendPageFrameIndex(struct tcp_pcb *pcb, struct http_state *hs, char *inda);
void SendPageFrame1(struct tcp_pcb *pcb, struct http_state *hs, char *inda);
void SendPageFrame2(struct tcp_pcb *pcb, struct http_state *hs, char *inda);
void SendPageUpdate(struct tcp_pcb *pcb, struct http_state *hs, char *inda);
void SendPageUpdateInputErr(struct tcp_pcb *pcb, struct http_state *hs, char *inda);
void SendPageUpdating(struct tcp_pcb *pcb, struct http_state *hs, char *inda);

void RcvLogin(struct tcp_pcb *pcb, struct http_state *hs, char *inda);
void RcvUpdate(struct tcp_pcb *pcb, struct http_state *hs, char *inda);

int GetInputValue(char *name, char *inda, u8 *value, int *p_value_len, int value_size);




/** Allocate a struct http_state. */
static struct http_state* http_state_alloc(void)
{
	struct http_state *ret;

	ret = (struct http_state *)mem_malloc(sizeof(struct http_state));

	if (ret != NULL) 
	{
		/* Initialize the structure. */
		memset(ret, 0, sizeof(struct http_state));
	}
	
	return ret;
  
}


/** Free a struct http_state.
 * Also frees the file data if dynamic.
 */
//static void http_state_free(struct http_state *hs)
//{
//	mem_free(hs);
	
//}


/**
 * The pcb had an error and is already deallocated.
 * The argument might still be valid (if != NULL).
 */
static void http_err(void *arg, err_t err)
{
	struct http_state *hs = (struct http_state *)arg;


	if (hs != NULL) 
	{
		mem_free(hs);
	}
 
}



/**
  * @brief  closes tcp connection
  * @param  pcb: pointer to a tcp_pcb struct
  * @param  hs: pointer to a http_state struct
  * @retval
  */
static void http_close_conn(struct tcp_pcb *pcb, struct http_state *hs)
{
	tcp_arg(pcb, NULL);
	tcp_sent(pcb, NULL);
	tcp_recv(pcb, NULL);
	tcp_err(pcb, NULL);
	
	mem_free(hs);
	tcp_close(pcb);
	
}

/**
  * @brief sends data found in  member "file" of a http_state struct
  * @param pcb: pointer to a tcp_pcb struct
  * @param hs: pointer to a http_state struct
  * @retval none
  */
static void http_send_data(struct tcp_pcb *pcb, struct http_state *hs)
{
	err_t err;
	u16_t len;

	/* We cannot send more data than space available in the send buffer */
	if (tcp_sndbuf(pcb) < hs->left)
	{
		len = tcp_sndbuf(pcb);
	}
	else
	{
		len = hs->left;
	}
	
	err = tcp_write(pcb, hs->buf, len, 0);
	
	if (err == ERR_OK)
	{
		hs->buf += len;
		hs->left -= len;
	}
	
}




/**
 * Data has been sent and acknowledged by the remote host.
 * This means that more data can be sent.
 */
static err_t http_sent(void *arg, struct tcp_pcb *pcb, u16_t len)
{
	struct http_state *hs;


	hs = (struct http_state *)arg;

	if (hs == NULL)
	{
		return ERR_OK;
	}

	hs->retries = 0;

	http_send_data(pcb, hs);

	return ERR_OK;
	
}




/**
 * The poll function is called every 2nd second.
 * If there has been no data sent (which resets the retries) in 8 seconds, close.
 * If the last portion of a file has not been sent in 2 seconds, close.
 *
 * This could be increased, but we don't want to waste resources for bad connections.
 */
static err_t http_poll(void *arg, struct tcp_pcb *pcb)
{
	if (arg == NULL)
	{
		tcp_close(pcb);
	}
	else
	{
		http_send_data(pcb, (struct http_state *)arg);
	}
	
	return ERR_OK;

}





void HttpdInit(void)
{

	struct tcp_pcb *pcb;
	err_t err;

	pcb = tcp_new();
	
	/* set SOF_REUSEADDR here to explicitly bind httpd to multiple interfaces */
	err = tcp_bind(pcb, IP_ADDR_ANY, HTTPD_SERVER_PORT);
	err = err;

	pcb = tcp_listen(pcb);
	
	
	/* initialize callback arg and accept callback */
	tcp_arg(pcb, pcb);


	tcp_accept(pcb, HttpAccept);



}




/**
 * A new incoming connection has been accepted.
 */
static err_t HttpAccept(void *arg, struct tcp_pcb *pcb, err_t err)
{
	struct http_state *hs;
//	struct tcp_pcb_listen *lpcb = (struct tcp_pcb_listen*)arg;
  

	/* Decrease the listen backlog counter */
//	tcp_accepted(lpcb);
	/* Set priority */
//	tcp_setprio(pcb, HTTPD_TCP_PRIO);

	/* Allocate memory for the structure that holds the state of the
     	connection - initialized by that function. */
	hs = http_state_alloc();
	if (hs == NULL) 
	{
		return ERR_MEM;
  	}

  	 /* Initialize the structure. */
	hs->buf = NULL;
	hs->left = 0;
  	

	/* Tell TCP that this is the structure we wish to be passed for our
	callbacks. */
	tcp_arg(pcb, hs);

	/* Set up the various callback functions */
	tcp_recv(pcb, HttpRecv);
	tcp_err(pcb, http_err);
	tcp_poll(pcb, http_poll, 10);
	tcp_sent(pcb, http_sent);

	return ERR_OK;
	
}



/**
 * Data has been received on this pcb.
 * For HTTP 1.0, this should normally only happen once (if the request fits in one packet).
 */
static err_t HttpRecv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{	
	struct http_state *hs;

	char inda[128];
	int len;
	
	hs = (struct http_state *)arg;
	
	if ((err != ERR_OK) || (p == NULL)) 
	{
		/* error or closed by other side? */
		if (p != NULL) 
		{
			/* Inform TCP that we have taken the data. */
			tcp_recved(pcb, p->tot_len);
			pbuf_free(p);
    		}
   // 		if (hs == NULL) 
    		{
			/* this should not happen, only to be robust */
		}
	
		http_close_conn(pcb, hs);
		
		return ERR_OK;
	
  	}

  	/* Inform TCP that we have taken the data */
	tcp_recved(pcb, p->tot_len);

	len = p->tot_len;
	len = len;
	memcpy(inda, p->payload, 128);

	pbuf_free(p);

	if(len <= 0) {
		return -1;
	}

	CommanderSendHttpInfo(inda, len);

	Debuger = Debuger;

	//process HTTP requests
	if ((strncmp(inda, "GET / ", strlen( "GET / ")) ==0))	//index page
	{
     		SendPageLogin(pcb, hs, inda);
	}     
	else if ((strncmp(inda, "GET /login_submit?", strlen( "GET /login_submit?")) ==0))
	{
		RcvLogin(pcb, hs, inda);
	} 
	else if ((strncmp(inda, "GET /log_err_submit?", strlen( "GET /log_err_submit?")) ==0))
	{
		SendPageLogin(pcb, hs, inda);
	}   
	else if ((strncmp(inda, "GET /frame_index.htm", strlen( "GET /frame_index.htm")) ==0))
	{
		SendPageFrameIndex(pcb, hs, inda);
	}   
	else if ((strncmp(inda, "GET /frame1.htm", strlen( "GET /frame1.htm")) ==0))
	{
		SendPageFrame1(pcb, hs, inda);
	}   
	else if ((strncmp(inda, "GET /frame2.htm", strlen( "GET /frame2.htm")) ==0))
	{
//		SendPageFrame2(pcb, hs, inda);
		SendPageUpdate(pcb, hs, inda);
	}   
	else if (strncmp(inda, "GET /update_fw?", strlen("GET /update_fw?")) == 0)		//firmware update
	{
		RcvUpdate(pcb, hs, inda);

	}
	else
	{
		
	}
   
	http_close_conn(pcb, hs);

	return ERR_OK;
  
}



void SendPageLogin(struct tcp_pcb *pcb, struct http_state *hs, char *inda)
{

	sprintf(PageBuf, PageLogin);

	hs->buf = PageBuf;
	hs->left = strlen(PageBuf);

	http_send_data(pcb, hs);
	
}

void SendPage1(struct tcp_pcb *pcb, struct http_state *hs, char *inda)
{

	sprintf(PageBuf, Page1, DevId, AppVersion1, AppVersion2);
	
	hs->buf = PageBuf;
	hs->left = strlen(PageBuf);

	http_send_data(pcb, hs);

}


void SendPageLogErr(struct tcp_pcb *pcb, struct http_state *hs, char *inda)
{

	sprintf(PageBuf, PageLogErr);

	hs->buf = PageBuf;
	hs->left = strlen(PageBuf);

	http_send_data(pcb, hs);
	
}

void SendPageFrameMain(struct tcp_pcb *pcb, struct http_state *hs, char *inda)
{

	sprintf(PageBuf, PageFrameMain);

	hs->buf = PageBuf;
	hs->left = strlen(PageBuf);

	http_send_data(pcb, hs);
	
}

void SendPageFrameIndex(struct tcp_pcb *pcb, struct http_state *hs, char *inda)
{

	sprintf(PageBuf, PageFrameIndex);

	hs->buf = PageBuf;
	hs->left = strlen(PageBuf);

	http_send_data(pcb, hs);
	
}

void SendPageFrame1(struct tcp_pcb *pcb, struct http_state *hs, char *inda)
{

	sprintf(PageBuf, PageFrame1, DevId, AppVersion1, AppVersion2, AppVersion3, AppVersion4);
//	sprintf(page, PageFrame1, 10, 1, 12);

	hs->buf = PageBuf;
	hs->left = strlen(PageBuf);

	http_send_data(pcb, hs);
	
}

void SendPageFrame2(struct tcp_pcb *pcb, struct http_state *hs, char *inda)
{
	sprintf(PageBuf, PageFrame2);
	hs->buf = PageBuf;
	hs->left = strlen(PageBuf);
	http_send_data(pcb, hs);	
}

void SendPageUpdate(struct tcp_pcb *pcb, struct http_state *hs, char *inda)
{
	sprintf(PageBuf, PageUpdate);
	hs->buf = PageBuf;
	hs->left = strlen(PageBuf);
	http_send_data(pcb, hs);
}

void SendPageUpdateInputErr(struct tcp_pcb *pcb, struct http_state *hs, char *inda)
{
	sprintf(PageBuf, PageUpdateInputErr);
	hs->buf = PageBuf;
	hs->left = strlen(PageBuf);
	http_send_data(pcb, hs);	
}

void SendPageUpdating(struct tcp_pcb *pcb, struct http_state *hs, char *inda)
{
	sprintf(PageBuf, PageUpdating);
	hs->buf = PageBuf;
	hs->left = strlen(PageBuf);
	http_send_data(pcb, hs);	
}


void RcvLogin(struct tcp_pcb *pcb, struct http_state *hs, char *inda)
{
	u8 username[32];
	u8 passwd[32];
	int username_len;
	int passwd_len;

	if (GetInputValue("username", inda, username, &username_len, sizeof(username)) > 0)
	{
		GetInputValue("password", inda, passwd, &passwd_len, sizeof(passwd));
		
//		printf("username=%s, name_len=%d, passwd=%s, passwd_len=%d\n", username, username_len, passwd, passwd_len);

		if ((memcmp(username, "root", strlen("root")) == 0) && (memcmp(passwd, "beihai", strlen("beihai"))==0))
//		if (memcmp(username, "admin", strlen("admin")) == 0)
		{
//			SendPageFrameMain(socket_fd);
			SendPageFrameMain(pcb, hs, inda);
		}
		else
		{
			SendPageLogErr(pcb, hs, inda);
		}

	}
	else
	{
		SendPageLogErr(pcb, hs, inda);
	}


}


void RcvUpdate(struct tcp_pcb *pcb, struct http_state *hs, char *inda)
{
	u8 filename[128];
	int filename_len;

	struct ip_addr server_ip;

	u8 all_dev_str[32];
	int all_dev_str_len = 0;


	if (GetInputValue("filename", inda, filename, &filename_len, sizeof(filename)) <= 0)
	{
		SendPageUpdateInputErr(pcb, hs, inda);
		return;
	}

	server_ip = pcb->remote_ip;

	GetInputValue("all_dev", inda, all_dev_str, &all_dev_str_len, sizeof(all_dev_str));
	if(memcmp(all_dev_str, "on", sizeof("on")) == 0)
	{
		NetControlSendAllDevUpdate(server_ip, filename, filename_len);
	}
	
	SendPageUpdating(pcb, hs, inda);

	TftpClientIapStart(server_ip, (char *)filename);

	
}



// Get the user input value form http request data 
int GetInputValue(char *name, char *inda, u8 *value, int *p_value_len, int value_size)
{
	int pos;
	int name_len;
	int value_len;

	pos = 0;
	name_len = strlen(name);
	value_len = 0;
	
	while ((*inda != 0)  && (*inda != 0x0a) && (*inda != 0x0d))
	{
		if ((*inda == *name) && (memcmp(name, inda, name_len) == 0))
		{
			inda += name_len;	
			pos += name_len;
			if (*inda == '=')
			{
				inda++;
				pos++;
				while ((*inda != '&') && (*inda != 0) && (*inda != 0x0a) && (*inda != 0x0d))
				{
					*value= *inda;
					value++;
					inda ++;
					pos ++;
					value_len++;

					if (value_len + 1 >= value_size)
					{
						break;
					}
				}
				*value= '\0';
				*p_value_len = value_len;
				return pos;
			}
		}
		pos++;
		inda++;
	}

	return -1;

}





 
