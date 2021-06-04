//*****************************************************//

//*****************************************************//



#include "lwip/pbuf.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/tcp.h"
#include "lwip/udp.h"
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "ctype.h"
#include "tftp_cli.h"
#include "tftpserver.h"



#include "MyAll.h"

#define HTTPTEST	"<html> \n<body> \n\
				<h1>����ͨ��  Beihai Communication</h1>\n\
				<p><a href=\"AAA\">Test AAA%s</a> </p>\n\
				<p><a href=\"BBB\">Test BBB</a></p>\n\
				<p>\
				<input type=\"text\" name=\"as_q\" class=\"box\"  id=\"searched_content\" title=\"12345\" />\
				<input type=\"submit\" value=\"Go\" class=\"button\" title=\"789\" />\
				</p>\n\
</body>\n</html>\n"

uint8 run_iap_req = 0;

char HttpTest[1024] ;


char HttpTestAAA[128] = "<html> \n<body> \n\
<h1>Test AAA</h1>\n\
<p><a href=\"index.html\">Back</a></p>\n\
</body>\n</html>\n" ;

char HttpTestBBB[128] = "<html> \n<body> \n\
<h1>download application</h1>\n\
<p><a href=\"index.html\">Back</a></p>\n\
</body>\n</html>\n" ;




//char HttpTest[64] = "hello" ;




/* Private function prototypes -----------------------------------------------*/
static err_t HttpAccept(void *arg, struct tcp_pcb *pcb, err_t err);
static err_t HttpRecv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err);






/** Allocate a struct http_state. */
static struct http_state *http_state_alloc(void)
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
static void http_state_free(struct http_state *hs)
{
	mem_free(hs);

}


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


#if 0
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


	Debuger = Debuger;



	/* process HTTP GET requests */
	if (strncmp(inda, "GET /", 5) == 0)
	{
		if ((strncmp(inda, "GET /resetmcu.cgi", 17) == 0))
		{

		}
		else if ((strncmp(inda, "GET /AAA", 8) == 0))
		{
			hs->buf = HttpTestAAA;
			hs->left = strlen(HttpTestAAA);

			pbuf_free(p);

			http_send_data(pcb, hs);

		}
		else if ((strncmp(inda, "GET /BBB", 8) == 0))
		{
			hs->buf = HttpTestBBB;
			hs->left = strlen(HttpTestBBB);

			pbuf_free(p);

			http_send_data(pcb, hs);

			run_iap_prog();

		}
		else
		{
			//send the  index page
			memset(HttpTest, 0, sizeof(HttpTest));
			sprintf(HttpTest, HTTPTEST, "abc");
			hs->buf = HttpTest;
			hs->left = strlen(HttpTest);

			pbuf_free(p);

			http_send_data(pcb, hs);

		}

	}


	http_close_conn(pcb, hs);


	return ERR_OK;


}

#endif

unsigned  int fill_tcp_data(unsigned char *buf, unsigned  int pos, const unsigned char *progmem_s)
{
	char c;

	while ((c = (char) * (progmem_s)) != '\0')
	{
		buf[pos] = c;
		pos ++;
		progmem_s ++;
	}

	return(pos);
}

//������name��ԭʼ��Ϣ���Լ���Ϣ���ڵ�λ���Լ�����
unsigned int find_name_from_recv_data(char *name, unsigned char *s, unsigned char *buffer, unsigned int *p_len, unsigned int buffer_size)
{
	unsigned  int pos = 0, len;
	int buf_len = 0;
	len = strlen(name);

	while (*s)
	{
		if ((*s == *name) && !memcmp(name, s, len))
		{
			s += len;
			pos += len;

			if (*s == '=')
			{
				s++;
				pos++;

				while ((*s != '&') && (*s))
				{
					*buffer = *s;
					buffer ++;
					s ++;
					pos ++;
					buf_len ++;

					if (buf_len + 1 >= buffer_size)
					{
						break;
					}
				}

				//if (*s)
				//{
				*buffer = '\0';
				*p_len = buf_len;
				return pos;
				//}
				//else
				//{
				//	return 0;
				//}

			}
		}

		pos++;
		s++;
	}

	return 0;
}

void send_http_data(struct tcp_pcb *pcb, struct http_state *hs, unsigned char *src_buf, uint16 len)
{

	hs->left = len;
	hs->buf = (char *)src_buf;
	http_send_data(pcb, hs);
}

/**
 * Data has been received on this pcb.
 * For HTTP 1.0, this should normally only happen once (if the request fits in one packet).
 */

#define MAX_BUF	1024


unsigned char str_tmp[1024] ;

const unsigned char head_buf[] = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n"; //Ŀǰ�ĸ�����44

const unsigned char frame_info[] = "<html>\
<frameset frameborder=\"no\">\
<frame name=\"m\" src=\"login.htm\" frameborder=\"no\" scrolling=\"no\" noresize=\"noresize\" marginhight=0>\
</frameset>\
</html>";

const unsigned char frame_set[] = "<html>\
<head><title>����pis����ʽά��ƽ̨</title></head>\
<frameset cols=\"20%%,80%%\" frameborder=\"yes\">\
<frame name=\"left\" src=\"left.htm\">\
<frame name=\"right\" src=\"run_info.htm\">\
</frameset>\
</html>";
#ifdef IAP_PROG
const unsigned char left[] = "<html>\
	<head base target=\"right\"></head>\
<body>\
	<b>\
<a href=\"run_info.htm\" target=\"right\">�豸������Ϣ</a><br />\
<!a href=\"mode_sel.htm\" target=\"right\"><!ģʽ�趨><!/a><!br />\
<a href=\"update_sw.htm\" target=\"right\">�������</a><br />\
	</b>\
</body>\
</html>";
#else
const unsigned char left[] = "<html>\
	<head base target=\"right\"></head>\
<body>\
	<b>\
<a href=\"run_info.htm\" target=\"right\">�豸������Ϣ</a><br />\
<a href=\"mode_sel.htm\" target=\"right\">ģʽ�趨</a><br />\
<a href=\"update_sw.htm\" target=\"right\">�������</a><br />\
	</b>\
</body>\
</html>";
#endif

const unsigned char log_err[] = "<html>\
<body>\
<center>\
<form action=\"log_err\" method=\"get\">\
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

#ifdef IAP_PROG
const unsigned char sw_update_input_err[] = "<html>\
<body>\
<center>\
	<h3><br />��Ϣ������������������!</h3>\
</center>\
</body>\
</html>";
#endif

const unsigned char login[] = "<html>\
<center>\
	<h2>�û���¼</h2>\
</center>\
<body>\
<form method=\"get\" action=\"login\">\
<table width=\"100%%\" border=\"0\">\
	<tr><td align=\"center\">Username<input type=\"text\" name=\"username\" /></td></tr>\
	<tr><td align=\"center\">Password<input type=\"text\" name=\"password\" /></td></tr>\
	<tr><td align=\"center\"><input type=\"submit\" value=\"login\" name=\"submit\" /></td></tr>\
</table>\
</form>\
</body>\
</html>";

const unsigned char mode_sel[] = "<html>\
<body>\
<center>\
	<h2>����ģʽ�趨</h2>\
</center>\
<form action=\"mode_sel\" method=\"get\">\
<table cellspacing=\"5\" width=\"100%%\" border=\"1\" cellpadding=\"5\">\
	<tr>\
		<th align=\"center\">����ģʽ</th>\
		<td align=\"center\">\
			<input type=\"radio\" name=\"mode_radio\" value=\"0\" />����\
			<input type=\"radio\" name=\"mode_radio\" value=\"1\" />������\
			<input type=\"radio\" name=\"mode_radio\" value=\"2\" />�汾��Ϣ\
			<input type=\"radio\" name=\"mode_radio\" value=\"3\" />����\
		</td>\
	</tr>\
	<tr>\
		<td align=\"center\" colspan=\"2\">\
			<input type=\"checkbox\" name=\"all_dev\" /><b>��������ͬ���豸</b>\
			<br />\
			<input type=\"submit\" name=\"modify\" value=\"apply\" />\
		</td>\
	</tr>\
</table>\
</form>\
</body>\
</html>";

#ifdef IAP_PROG
const unsigned char update_sw[] = "<html>\
<body>\
<center>\
	<h2>�������ά��</h2>\
</center>\
<form action=\"update_sw\" method=\"get\">\
<table cellspacing=\"5\" width=\"100%%\" border=\"1\" cellpadding=\"5\" >\
	<tr>\
		<td align=\"center\">\
			Ŀ���ļ�:<input type=\"text\" name=\"sw_file\" />\
			<br />\
			tftp������IP:<input type=\"text\" name=\"ip1\" maxlength=\"3\" size=\"6\"/>.<input type=\"text\" name=\"ip2\" maxlength=\"3\" size=\"6\"/>.<input type=\"text\" name=\"ip3\" maxlength=\"3\" size=\"6\"/>.<input type=\"text\" name=\"ip4\" maxlength=\"3\" size=\"6\"/>\
		</td>\
	</tr>\
	<tr>\
		<td align=\"center\" colspan=\"1\">\
			<input type=\"checkbox\" name=\"all_dev\" /><b>��������ͬ���豸</b>\
			<br />\
			<input type=\"submit\" name=\"modify\" value=\"update\" />\
		</td>\
	</tr>\
</table>\
</form>\
<p>\
	<em>˵��: ָ��Ŀ���ļ���,������IP����Ϣ,��\"update\"��ť,�����豸��Ӧ�ó���</em>\
	<br />\
	<em>ע��: 1. ���\"update\"��ťǰ,����ȷ����Ӧ��tftp�������Ѿ�����,��׼������Ӧ�ó����ļ�;\
	<br />\
	2. �����豸��Ҫ��дflash, ���\"update\"��ť��,���������ˢ�¿�������Щ;\
	</em>\
</p>\
</body>\
</html>";
#else

const unsigned char update_sw[] = "<html>\
<body>\
<center>\
	<h2>�������ά��</h2>\
</center>\
<form action=\"update_sw\" method=\"get\">\
<table cellspacing=\"5\" width=\"100%%\" border=\"1\" cellpadding=\"5\" >\
	<tr>\
		<td align=\"center\" colspan=\"1\">\
			<input type=\"checkbox\" name=\"all_dev\" /><b>��������ͬ���豸</b>\
			<br />\
			<input type=\"submit\" name=\"modify\" value=\"update\" />\
		</td>\
	</tr>\
</table>\
</form>\
<p>\
	<em>˵������\"update\"��ť,�����豸��bootloader,Ȼ�����µ�¼�豸.</em>\
</p>\
</body>\
</html>";
#endif

#ifdef IAP_PROG
const unsigned char updating[] = "<html>\
<body>\
<center>\
	<h3><br />���������...,�����ĵȴ�!<br />��ע�Ȿ�ص��Ե�tftp���ߵ���ʾ��Ϣ,������������Ϻ�,���µ�½�豸,����֤��������Ƿ�ɹ���</h3>\
</center>\
</body>\
</html>";
#else
const unsigned char updating[] = "<html>\
<body>\
<center>\
	<h3><br />bootloader����������...<br />Լ5��������µ�½�豸,����֤bootloader�����Ƿ���������.\
	<br />\
	bootloader�������������󣬰�����ʾ����tftp��������IP��Ӧ�ó����ļ�������Ϣ������Ӧ�ó�����¡�\
	</h3>\
</center>\
</body>\
</html>";
#endif


#define FMT_run_info	"<html>\
<body bgcolor=\"#ffffE7\" link=\"#3300FF\" alink=\"#ff00ff\" vlink=\"#9900FF\">\
	<marquee direction=\"left\" width=\"200\" hspace=\"10\">\
		<font face=\"����\" size=\"5\" color=\"#ff00ff\">����б���ͨ�ż������޹�˾</font>\
	</marquee>\
<hr>\
	<p>\
		<table width=\"100%%\" border=\"4\" bordercolor=\"#ff00ff\" align=\"center\" bgcolor=\"#DDFFDD\" height=\"100\" cellspacing=\"2\" cellpadding=\"8\">\
			<caption>�豸��Ϣ</caption>\
			<tr>\
				<th>�豸����</th>\
				<th>�豸ID</th>\
				<th>����汾</th>\
				<th>����ʱ��</th>\
				<th>�豸����</th>\
			</tr>\
			<tr>\
				<td>0x%02x</td><! 1 dev_type>\
				<td>%d</td><! 2 dev_id>\
				<td>%s</td><! 3 sw_version>\
				<td>%s</td><! 4 compile time>\
				<td>%s</td><! 5 dev description>\
			</tr>\
		</table>\
	</p>\
	<br />\
	<p>\
		<table width=\"100%%\" border=\"4\" bordercolor=\"#ff00ff\" align=\"center\" bgcolor=\"#DDFFDD\" height=\"100\" cellspacing=\"2\" cellpadding=\"8\">\
			<caption>�豸����ģʽ</caption>\
			<tr>\
				<th>״̬��</th>\
				<th>˵��</th>\
			</tr>\
			<tr>\
				<td>0x%02x</td><! 6 mode word>\
				<td>%s</td><! 7 mode description>\
			</tr>\
		</table>\
	</p>\
	<br />\
	<p align=\"right\">\
		<adress>����ͨ��&copy;</adress>\
	</p>\
</body>\
</html>"

const unsigned char *mode_description[] =
{
	"��ģʽ������ʾ�г����յ�վ��Ϣ",
	"��ģʽ���ڼ��LED��ʾ���Ƿ���ڻ���",
	"��ģʽ������ʾ�豸������汾���豸id����Ϣ",
	"��ģʽ������ʾ��̬�Ĳ�������",
	"��ǰ���е����豸��bootloader,ģʽ״̬����Ч,����ο�"
	"\0"
};

const unsigned char *dev_description[] =
{
	"���豸ΪLEDĿ�ĵ���ʾ����������ʾ�г����յ�վ��Ϣ���豸��������!",
	"���豸ΪLEDĿ�ĵ���ʾ�����豸��Ӧ�ó��򣬵�ǰ���е����豸��bootloader, ��������ȷ��Ӧ�ó���!!",
	"\0",
};
//const uint8 software_version[4] = {1, 10, 8, 13 };
//uint8 ptu_SetMode = 1;	//����ģʽ

extern const char build_date_str[];
extern const char build_time_str[];

#ifdef IAP_PROG
void fill_run_info(unsigned char *buf)
{
	char sw_version[40] = "";
	char build_date[40] = "";
	{
		char tmp[20];
		uint8 len_version;
		uint8 i;

		sprintf(sw_version, "%s", "V");
		len_version = sizeof(software_version);

		memset(tmp, 0, sizeof(tmp));

		for (i = 0; i < len_version; i++)
		{
			if (i == (len_version - 1))
			{
				sprintf(tmp, "%d", software_version[i]);
				strcat(sw_version, tmp);
			}
			else
			{
				sprintf(tmp, "%d.", software_version[i]);
				strcat(sw_version, tmp);
			}
		}

		sprintf(build_date, "%s, %s", build_date_str, build_time_str);
	}

	sprintf((char *)buf, FMT_run_info, DevType, DevId, sw_version, build_date, dev_description[1], 0xff, mode_description[4]);

}
#else
void fill_run_info(unsigned char *buf)
{

	unsigned char index;
	char sw_version[40] = "";
	char build_date[40] = "";
	{
		char tmp[20];
		uint8 len_version;
		uint8 i;

		sprintf(sw_version, "%s", "V");
		len_version = sizeof(software_version);

		memset(tmp, 0, sizeof(tmp));

		for (i = 0; i < len_version; i++)
		{
			if (i == (len_version - 1))
			{
				sprintf(tmp, "%d", software_version[i]);
				strcat(sw_version, tmp);
			}
			else
			{
				sprintf(tmp, "%d.", software_version[i]);
				strcat(sw_version, tmp);
			}
		}

		sprintf(build_date, "%s, %s", build_date_str, build_time_str);
	}

	switch (ptu_SetMode)
	{
		case 1:
			index = 1;
			break;

		case 2:
			index = 2;
			break;

		case 3:
			index = 3;
			break;

		default:
			index = 0;
	}

	sprintf((char *)buf, FMT_run_info, DevType, DevId, sw_version, build_date, dev_description[0], index, mode_description[index]);
}
#endif

void html_set_mode(unsigned char mode)
{
	if (mode == 0)  	//������ʾģʽ
	{
		clr_led_buf();
		clr_dispinfo();
		set_dispmode(SCREEN_MODE_stationID);
	}
	else if (mode == 1)  	//������ģʽ
	{
		clr_led_buf();
		clr_dispinfo();
		set_dispmode(SCREEN_MODE_test);
	}
	else if (mode == 2)  	//�汾��ʾģʽ
	{
		clr_led_buf();
		clr_dispinfo();
		set_dispmode(SCREEN_MODE_softwareVersion);
	}
	else if (mode == 3)  	//��̬��ʾģʽ
	{
		clr_led_buf();
		clr_dispinfo();
		set_dispmode(SCREEN_MODE_static_test);
	}
	else
	{
		return;
	}
}

static err_t HttpRecv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{

	struct http_state *hs;
	unsigned char recv_data[MAX_BUF], send_buffer[MAX_BUF];
	unsigned int len, bytes_received, plen, pos;

	// char P;
	//data=getenv("QUERY_STRING");
	//	p = getenv("QUERY_STRING");

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

		// 	if (hs == NULL)
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

	if (p ->len >= MAX_BUF)
	{
		bytes_received = MAX_BUF - 1;
	}
	else
	{
		bytes_received = p ->len;
	}

	memset(recv_data, 0, sizeof(recv_data));
	memcpy(recv_data, p->payload, bytes_received);
	recv_data[bytes_received] = '\0';
	pbuf_free(p);

	Debuger = Debuger;

	if (strncmp("GET / ", (char *) & (recv_data[0]), 6) == 0)
	{
		plen = fill_tcp_data(send_buffer, 0, head_buf);
		plen = fill_tcp_data(send_buffer, plen, frame_info);
		send_http_data(pcb, hs, send_buffer, plen);
	}
	else if (strncmp("GET /login.htm", (char *) & (recv_data[0]), strlen("GET /login.htm")) == 0)
	{
		plen = fill_tcp_data(send_buffer, 0, head_buf);
		plen = fill_tcp_data(send_buffer, plen, login);
		send_http_data(pcb, hs, send_buffer, plen);
	}
	else if (strncmp("GET /run_info.htm", (char *) & (recv_data[0]), strlen("GET /run_info.htm")) == 0)
	{
		plen = fill_tcp_data(send_buffer, 0, head_buf);
		fill_run_info(str_tmp);
		plen = fill_tcp_data(send_buffer, plen, str_tmp);
		send_http_data(pcb, hs, send_buffer, plen);
	}
	else if (strncmp("GET /left.htm", (char *) & (recv_data[0]), strlen("GET /left.htm")) == 0)
	{
		plen = fill_tcp_data(send_buffer, 0, head_buf);
		plen = fill_tcp_data(send_buffer, plen, left);
		send_http_data(pcb, hs, send_buffer, plen);
	}
	else if (strncmp("GET /mode_sel.htm", (char *) & (recv_data[0]), strlen("GET /mode_sel.htm")) == 0)
	{
		plen = fill_tcp_data(send_buffer, 0, head_buf);
		plen = fill_tcp_data(send_buffer, plen, mode_sel);
		send_http_data(pcb, hs, send_buffer, plen);
	}
	else if (strncmp("GET /update_sw.htm", (char *) & (recv_data[0]), strlen("GET /update_sw.htm")) == 0)
	{
		plen = fill_tcp_data(send_buffer, 0, head_buf);
		plen = fill_tcp_data(send_buffer, plen, update_sw);
		send_http_data(pcb, hs, send_buffer, plen);
	}
	else if (strncmp("GET /login?", (char *) & (recv_data[0]), strlen("GET /login?")) == 0) //���뼰�û�����ȷ
	{
		unsigned char user_name[100], passwd[100];
		unsigned int  name_len, passwd_len;
		unsigned int st = strlen("GET /login?");

		if ((len = find_name_from_recv_data("username", &recv_data[st], user_name, &name_len, sizeof(user_name))) != 0)
		{
			pos = st + len;
			len = find_name_from_recv_data("password", &recv_data[pos], passwd, &passwd_len, sizeof(passwd));

			if ((name_len == 5) && (!memcmp(user_name, "admin", name_len)) && (passwd_len == 0))
			{
				plen = fill_tcp_data(send_buffer, 0, head_buf);
				plen = fill_tcp_data(send_buffer, plen, frame_set);
				send_http_data(pcb, hs, send_buffer, plen);
			}
			else
			{
				plen = fill_tcp_data(send_buffer, 0, head_buf);
				plen = fill_tcp_data(send_buffer, plen, log_err);
				send_http_data(pcb, hs, send_buffer, plen);
			}
		}
		else
		{
			plen = fill_tcp_data(send_buffer, 0, head_buf);
			plen = fill_tcp_data(send_buffer, plen, log_err);
			send_http_data(pcb, hs, send_buffer, plen);
		}
	}
	else if (strncmp("GET /log_err?", (char *) & (recv_data[0]), strlen("GET /log_err?")) == 0)
	{
		plen = fill_tcp_data(send_buffer, 0, head_buf);
		plen = fill_tcp_data(send_buffer, plen, login);
		send_http_data(pcb, hs, send_buffer, plen);
	}

#ifndef IAP_PROG
	else if (strncmp("GET /mode_sel?", (char *) & (recv_data[0]), strlen("GET /mode_sel?")) == 0) //���뼰�û�����ȷ
	{
		unsigned char str[100];
		unsigned int  str_len;
		unsigned int st = strlen("GET /mode_sel?");
		unsigned char flag_all_device;
		uint8 mode;

		if ((len = find_name_from_recv_data("mode_radio", &recv_data[st], str, &str_len, sizeof(str))) != 0)
		{
			switch (str[0])
			{
				case '1':
					mode = 1;
					break;

				case '2':
					mode = 2;
					break;

				case '3':
					mode = 3;
					break;

				default:
					mode = 0;
					break;
			}

			pos = st + len;
			len = find_name_from_recv_data("all_dev", &recv_data[pos], str, &str_len, sizeof("all_dev"));

			flag_all_device = 0;

			if (len)
			{
				if ((str_len == 2) && (!memcmp(str, "on", str_len)))
				{
					flag_all_device = 1;
				}
			}

			if (mode != ptu_SetMode)
			{
				ptu_SetMode = mode;
				html_set_mode(mode);
			}

			{
				//flag_all_device = 1;

				if (flag_all_device)
				{
					udpTest_send_process(CMD_TEST_set_dispMode, (void *)&mode, 1);
				}
			}
		}

		plen = fill_tcp_data(send_buffer, 0, head_buf);
		plen = fill_tcp_data(send_buffer, plen, mode_sel);
		send_http_data(pcb, hs, send_buffer, plen);



	}

#endif
#ifdef IAP_PROG
	else if (strncmp("GET /update_sw?", (char *) & (recv_data[0]), strlen("GET /update_sw?")) == 0) //���뼰�û�����ȷ
	{
		unsigned int st = strlen("GET /update_sw?");
		unsigned int  section_len;
		unsigned char filename[128];
		unsigned char str[10];
		unsigned char ip[4];
		unsigned char err_f = 0;
		unsigned char flag_all_device;

		pos = st;

		if ((len = find_name_from_recv_data("sw_file", &recv_data[pos], filename, &section_len, sizeof(filename))) == 0)
		{
			// input err
			err_f = 1;
		}

		pos += len;

		if ((len = find_name_from_recv_data("ip1", &recv_data[pos], str, &section_len, sizeof(str))) == 0)
		{
			// input err
			err_f = 1;
		}

		pos += len;
		ip[0] = atoi((char *)str);

		if ((len = find_name_from_recv_data("ip2", &recv_data[pos], str, &section_len, sizeof(str))) == 0)
		{
			// input err
			err_f = 1;
		}

		pos += len;
		ip[1] = atoi((char *)str);

		if ((len = find_name_from_recv_data("ip3", &recv_data[pos], str, &section_len, sizeof(str))) == 0)
		{
			// input err
			err_f = 1;
		}

		pos += len;
		ip[2] = atoi((char *)str);

		if ((len = find_name_from_recv_data("ip4", &recv_data[pos], str, &section_len, sizeof(str))) == 0)
		{
			// input err
			err_f = 1;
		}

		ip[3] = atoi((char *)str);

		if ((ip[0] == 0) || (ip[1] == 0) || (ip[2] == 0) || (ip[3] == 0))
		{
			// input err
			err_f = 1;
		}

		pos += len;
		len = find_name_from_recv_data("all_dev", &recv_data[pos], str, &section_len, sizeof("all_dev"));

		flag_all_device = 0;

		if (len)
		{
			if ((section_len == 2) && (!memcmp(str, "on", section_len)))
			{
				flag_all_device = 1;
			}
		}

		if (err_f)
		{
			plen = fill_tcp_data(send_buffer, 0, head_buf);
			plen = fill_tcp_data(send_buffer, plen, sw_update_input_err);
			send_http_data(pcb, hs, send_buffer, plen);
		}
		else if (check_UDPpcb_tftp())
		{
			plen = fill_tcp_data(send_buffer, 0, head_buf);
			plen = fill_tcp_data(send_buffer, plen, updating);
			send_http_data(pcb, hs, send_buffer, plen);
		}
		else if (tftp_check_param(TFTP_RRQ, (char *)filename, MODE_OCTET, ip) != 0)
		{
			plen = fill_tcp_data(send_buffer, 0, head_buf);
			plen = fill_tcp_data(send_buffer, plen, sw_update_input_err);
			send_http_data(pcb, hs, send_buffer, plen);
		}
		else
		{
			tftp_cli_start(TFTP_RRQ, (char *)filename, MODE_OCTET, ip);

			plen = fill_tcp_data(send_buffer, 0, head_buf);
			plen = fill_tcp_data(send_buffer, plen, updating);
			send_http_data(pcb, hs, send_buffer, plen);


			{
				uint16 p_len = sizeof(TAG_download_app_req) + strlen((char *)filename);
				TAG_download_app_req *p_buf;

				p_buf = (TAG_download_app_req *)malloc(p_len);
				memcpy(p_buf->ip, ip, 4);
				p_buf->filename_len = (uint8)strlen((char *)filename);
				sprintf((char *)p_buf->filename, "%s", (char *)filename);

				//flag_all_device = 1;

				if (flag_all_device)
				{
					udpTest_send_process(CMD_TEST_download_app, p_buf, p_len);
				}

				free(p_buf);
			}
		}
	}

#else
	else if (strncmp("GET /update_sw?", (char *) & (recv_data[0]), strlen("GET /update_sw?")) == 0) //���뼰�û�����ȷ
	{
		unsigned char str[100];
		unsigned int  str_len;
		unsigned int st = strlen("GET /update_sw?");
		unsigned char flag_all_device;

		pos = st;
		len = find_name_from_recv_data("all_dev", &recv_data[pos], str, &str_len, sizeof("all_dev"));
		flag_all_device = 0;

		if (len)
		{
			if ((str_len == 2) && (!memcmp(str, "on", str_len)))
			{
				flag_all_device = 1;
			}
		}

		plen = fill_tcp_data(send_buffer, 0, head_buf);
		plen = fill_tcp_data(send_buffer, plen, updating);
		send_http_data(pcb, hs, send_buffer, plen);

		{
			//flag_all_device = 1;

			if (flag_all_device)
			{
				udpTest_send_process(CMD_TEST_run_iap, NULL, null);
			}
		}
		run_iap_req = 1;
		//run_iap_prog();
	}

#endif
	http_close_conn(pcb, hs);
	return ERR_OK;


}


void set_run_iap_req(uint8 val)
{
	run_iap_req = val;
}
void check_iap_req_process(void)
{
#ifndef IAP_PROG
	static uint32 timer_1 = 0;
	static uint8 iap_req, flag = 1;

	if (flag)
	{
		flag = 0;
		timer_1 = get_timer0_clock();
		iap_req = run_iap_req;
	}

	if (iap_req != run_iap_req)
	{
		iap_req = run_iap_req;
		timer_1 = get_timer0_clock();
	}

	if (iap_req)
	{
		if (timer0_check_timer(&timer_1, TIMING_5sec))
		{
			iap_req = 0;
			run_iap_req = 0;
			run_iap_prog();

		}
	}

#endif
}






