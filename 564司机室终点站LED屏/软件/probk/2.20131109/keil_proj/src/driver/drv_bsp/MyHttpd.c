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



char HttpTest[1024] ;


char HttpTestAAA[128] = "<html> \n<body> \n\
<h1>Test AAA</h1>\n\
<p><a href=\"index.html\">Back</a></p>\n\
</body>\n</html>\n" ;

char HttpTestBBB[128] = "<html> \n<body> \n\
<h1>Test BBB</h1>\n\
<p><a href=\"index.html\">Back</a></p>\n\
</body>\n</html>\n" ;




//char HttpTest[64] = "hello" ;




/* Private function prototypes -----------------------------------------------*/
static err_t HttpAccept(void *arg, struct tcp_pcb *pcb, err_t err);
static err_t HttpRecv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err);






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
		if ((strncmp(inda, "GET /resetmcu.cgi", 17) ==0))
		{
     
        	}     
        	else if ((strncmp(inda, "GET /AAA", 8) ==0))
		{
     			hs->buf = HttpTestAAA;
			hs->left = strlen(HttpTestAAA);
			
			pbuf_free(p);

			http_send_data(pcb, hs);
			
        	}     
        	else if ((strncmp(inda, "GET /BBB", 8) ==0))
		{
     			hs->buf = HttpTestBBB;
			hs->left = strlen(HttpTestBBB);
			
			pbuf_free(p);

			http_send_data(pcb, hs);
			
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











 
