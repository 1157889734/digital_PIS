
//*****************************************************//
//		MyLwip.c
//*****************************************************//



/* Includes ------------------------------------------------------------------*/
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




struct netif Netif;

struct ip_addr AllBroadIpAddr;


struct udp_pcb *pUdpTestPcb;

struct udp_pcb *pUdpCommanderPcb;


struct udp_pcb *pUdpControlRcvPcb;
struct udp_pcb *pUdpControlSendPcb;

struct udp_pcb *pUdpBroadcastRcvPcb;
struct udp_pcb *pUdpBroadcastSendPcb;

struct udp_pcb *pUdpTalkRcvPcb;
struct udp_pcb *pUdpTalkSendPcb;

struct udp_pcb *pUdpPlayclkRcvPcb;
struct udp_pcb *pUdpPlayclkSendPcb;





/* Private function prototypes -----------------------------------------------*/
void UdpTestReceiveCallback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port);
void UdpCommanderReceiveCallback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port);

void UdpControlReceiveCallback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port);
void UdpBroadcastReceiveCallback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port);
void UdpTalkReceiveCallback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port);
void UdpPlayclkReceiveCallback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port);





/**
  * @brief  Initializes the lwIP stack
  * @param  None
  * @retval None
  */
void LwipInit(void)
{
	struct ip_addr ipaddr;
	struct ip_addr netmask;
	struct ip_addr gw;


	/* Initializes the dynamic memory heap defined by MEM_SIZE.*/
	mem_init();

	/* Initializes the memory pools defined by MEMP_NUM_x.*/
	memp_init();

	IP4_ADDR(&ipaddr, NetLocalIp[0], NetLocalIp[1], NetLocalIp[2], NetLocalIp[3]);
	IP4_ADDR(&netmask, NetMaskIp[0], NetMaskIp[1], NetMaskIp[2], NetMaskIp[3]);
	IP4_ADDR(&gw, NetRouterIp[0], NetRouterIp[1], NetRouterIp[2], NetRouterIp[3]);


	IP4_ADDR(&AllBroadIpAddr, 255, 255, 255, 255);


	/* - netif_add(struct netif *netif, struct ip_addr *ipaddr,
            			struct ip_addr *netmask, struct ip_addr *gw,
            			void *state, err_t (* init)(struct netif *netif),
            			err_t (* input)(struct pbuf *p, struct netif *netif))
    
	Adds your network interface to the netif_list. Allocate a struct
	netif and pass a pointer to this structure as the first argument.
	Give pointers to cleared ip_addr structures when using DHCP,
	or fill them with sane numbers otherwise. The state pointer may be NULL.

	The init function pointer must point to a initialization function for
	your ethernet netif interface. The following code illustrates it's use.*/
	netif_add(&Netif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);

	/*  Registers the default network interface.*/
	netif_set_default(&Netif);

	/*  When the netif is fully configured this function must be called.*/
	netif_set_up(&Netif);



  	UdpTestInit();

  	UdpCommanderInit();


  	UdpControlRcvInit();
  	UdpControlSendInit();
  	
  	UdpBroadcastRcvInit();
  	UdpBroadcastSendInit();

  	UdpTalkRcvInit();
  	UdpTalkSendInit();

  	UdpPlayclkRcvInit();
  	UdpPlayclkSendInit();


  
}


void LwipProcess(void)
{

	/* check if any packet received */
	if (ETH_CheckFrameReceived())
	{ 
		/* process received ethernet packet */
		LwipPktHandle();
	}
	

}

void Lwip50ms(void)
{

	/* handle periodic timers for LwIP */
	LwipPeriodicHandle();

}



/**
  * @brief  Called when a frame is received
  * @param  None
  * @retval None
  */
void LwipPktHandle(void)
{
	/* Read a received packet from the Ethernet buffers and send it to the lwIP for handling */
	ethernetif_input(&Netif);
	
}


/**
  * @brief  LwIP periodic tasks
  * @param  localtime the current LocalTime value
  * @retval None
  */
void LwipPeriodicHandle(void)
{

//	static int tcptimer;
	static int arptimer;


//	tcptimer++;
	arptimer++;
	

//#if LWIP_TCP

	/* TCP periodic process every 250 ms */
//	if (tcptimer > 5)
//	{
//		tcptimer = 0;
//		tcp_tmr();
//	}
		
//#endif


  
	/* ARP periodic process every 5s */
	if (arptimer > 100)
	{
		arptimer = 0;
		etharp_tmr();
	}



}




//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

/**
  * @brief  Initialize the server application.
  * @param  None
  * @retval None
  */
void UdpTestInit(void)
{
//	err_t err;
   
	/* Create a new UDP control block  */
	pUdpTestPcb = udp_new();
  
	/* Bind the upcb to the UDP_PORT port */
	/* Using IP_ADDR_ANY allow the upcb to be used by any local interface */
	udp_bind(pUdpTestPcb, IP_ADDR_ANY, PORT_TEST_L);
      		
	/* Set a receive callback for the upcb */
	udp_recv(pUdpTestPcb, UdpTestReceiveCallback, NULL);


   
}




/**
  * @brief This function is called when an UDP datagrm has been received on the port UDP_PORT.
  * @param arg user supplied argument (udp_pcb.recv_arg)
  * @param pcb the udp_pcb which received data
  * @param p the packet buffer that was received
  * @param addr the remote IP address from which the packet was received
  * @param port the remote port from which the packet was received
  * @retval None
  */
void UdpTestReceiveCallback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port)
{

//	u8 inda[16];	


//	memcpy(inda, p->payload, 16);

	/* Connect to the remote client */
	udp_connect(upcb, addr, PORT_TEST_R);
    
	/* Tell the client that we have accepted it */
	udp_send(upcb, p);

	/* free the UDP connection, so we can accept new clients */
	udp_disconnect(upcb);
	
	/* Free the p buffer */
	pbuf_free(p);

	
   
}




void UdpTestSend(void)
{

	struct pbuf *p;


	u8 outda[8];


	outda[0] = 0x20;
	outda[1] = 0x21;
	outda[2] = 0x22;
	outda[3] = 0x23;
	outda[4] = 0x24;
	outda[5] = 0x25;
	outda[6] = 0x26;
	outda[7] = 0x27;


	/* Connect to the remote client */
	udp_connect(pUdpTestPcb, &AllBroadIpAddr, PORT_TEST_R);
    
  
	/* allocate pbuf from pool*/
	p = pbuf_alloc(PBUF_TRANSPORT,8, PBUF_POOL);
      
	if (p != NULL)
	{
		/* copy data to pbuf */
		pbuf_take(p, (u8*)outda, 8);
          
		/* send udp data */
		udp_send(pUdpTestPcb, p); 


		/* free the UDP connection, so we can accept new clients */
		udp_disconnect(pUdpTestPcb);
			
		/* free pbuf */
		pbuf_free(p);
		
	}

	
     

}




//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////




void UdpCommanderInit(void)
{
   
	/* Create a new UDP control block  */
	pUdpCommanderPcb = udp_new();
  
	/* Bind the upcb to the UDP_PORT port */
	/* Using IP_ADDR_ANY allow the upcb to be used by any local interface */
	udp_bind(pUdpCommanderPcb, IP_ADDR_ANY, PORT_COMMANDER_L);
      		
	/* Set a receive callback for the upcb */
	udp_recv(pUdpCommanderPcb, UdpCommanderReceiveCallback, NULL);


   
}


void UdpCommanderReceiveCallback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port)
{

	CommanderReceive((u8*)(p->payload), (int)(p->len));

	/* Free the p buffer */
	pbuf_free(p);
	
	
}



void UdpCommanderSend(u8* pbuf, int len)
{

	struct pbuf *p;

	/* Connect to the remote client */
	udp_connect(pUdpCommanderPcb, &AllBroadIpAddr, PORT_COMMANDER_R);
    
  
	/* allocate pbuf from pool*/
	p = pbuf_alloc(PBUF_TRANSPORT,len, PBUF_POOL);
      
	if (p != NULL)
	{
		/* copy data to pbuf */
		pbuf_take(p, pbuf, len);
          
		/* send udp data */
		udp_send(pUdpCommanderPcb, p); 


		/* free the UDP connection, so we can accept new clients */
		udp_disconnect(pUdpCommanderPcb);
			
		/* free pbuf */
		pbuf_free(p);
		
	}


	
}



//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////



void UdpControlRcvInit(void)
{   
	/* Create a new UDP control block  */
	pUdpControlRcvPcb = udp_new();
  
	/* Bind the upcb to the UDP_PORT port */
	/* Using IP_ADDR_ANY allow the upcb to be used by any local interface */
	udp_bind(pUdpControlRcvPcb, IP_ADDR_ANY, PORT_CONTROL_RCV_L);
      		
	/* Set a receive callback for the upcb */
	udp_recv(pUdpControlRcvPcb, UdpControlReceiveCallback, NULL);

}


void UdpControlReceiveCallback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port)
{

	NetControlReceiveProcess((u8*)(p->payload), (int)(p->len));

	/* Free the p buffer */
	pbuf_free(p);
	
   
}


void UdpControlSendInit(void)
{
   
	/* Create a new UDP control block  */
	pUdpControlSendPcb = udp_new();
  
	/* Bind the upcb to the UDP_PORT port */
	/* Using IP_ADDR_ANY allow the upcb to be used by any local interface */
	udp_bind(pUdpControlSendPcb, IP_ADDR_ANY, PORT_CONTROL_SND_L);
      		
   
}


void UdpControlSend(u8* pbuf, int len)
{

	struct pbuf *p;

	/* Connect to the remote client */
	udp_connect(pUdpControlSendPcb, &AllBroadIpAddr, PORT_CONTROL_SND_R);
    
  
	/* allocate pbuf from pool*/
	p = pbuf_alloc(PBUF_TRANSPORT,len, PBUF_POOL);
      
	if (p != NULL)
	{
		/* copy data to pbuf */
		pbuf_take(p, (u8*)pbuf, len);
          
		/* send udp data */
		udp_send(pUdpControlSendPcb, p); 

		/* free the UDP connection, so we can accept new clients */
		udp_disconnect(pUdpControlSendPcb);
			
		/* free pbuf */
		pbuf_free(p);
		
	}

	
}




//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////




void UdpBroadcastRcvInit(void)
{
   
	/* Create a new UDP control block  */
	pUdpBroadcastRcvPcb = udp_new();
  
	/* Bind the upcb to the UDP_PORT port */
	/* Using IP_ADDR_ANY allow the upcb to be used by any local interface */
	udp_bind(pUdpBroadcastRcvPcb, IP_ADDR_ANY, PORT_BROADCAST_RCV_L);
      		
	/* Set a receive callback for the upcb */
	udp_recv(pUdpBroadcastRcvPcb, UdpBroadcastReceiveCallback, NULL);


   
}


void UdpBroadcastReceiveCallback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port)
{

	NetBroadcastReceiveProcess((u8*)(p->payload), (int)(p->len));
	
	/* Free the p buffer */
	pbuf_free(p);

   
}


void UdpBroadcastSendInit(void)
{
   
	/* Create a new UDP control block  */
	pUdpBroadcastSendPcb = udp_new();
  
	/* Bind the upcb to the UDP_PORT port */
	/* Using IP_ADDR_ANY allow the upcb to be used by any local interface */
	udp_bind(pUdpBroadcastSendPcb, IP_ADDR_ANY, PORT_BROADCAST_SND_L);
      		
   
}


void UdpBroadcastSend(u8* pbuf, int len)
{

	struct pbuf *p;

	/* Connect to the remote client */
	udp_connect(pUdpBroadcastSendPcb, &AllBroadIpAddr, PORT_BROADCAST_SND_R);
    
  
	/* allocate pbuf from pool*/
	p = pbuf_alloc(PBUF_TRANSPORT,len, PBUF_POOL);
      
	if (p != NULL)
	{
		/* copy data to pbuf */
		pbuf_take(p, (u8*)pbuf, len);
          
		/* send udp data */
		udp_send(pUdpBroadcastSendPcb, p); 

		/* free the UDP connection, so we can accept new clients */
		udp_disconnect(pUdpBroadcastSendPcb);
			
		/* free pbuf */
		pbuf_free(p);
		
	}

	
}




//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////



void UdpTalkRcvInit(void)
{
   
	/* Create a new UDP control block  */
	pUdpTalkRcvPcb = udp_new();
  
	/* Bind the upcb to the UDP_PORT port */
	/* Using IP_ADDR_ANY allow the upcb to be used by any local interface */
	udp_bind(pUdpTalkRcvPcb, IP_ADDR_ANY, PORT_TALK_RCV_L);
      		
	/* Set a receive callback for the upcb */
	udp_recv(pUdpTalkRcvPcb, UdpTalkReceiveCallback, NULL);


   
}


void UdpTalkReceiveCallback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port)
{

	NetTalkReceiveProcess((u8*)(p->payload), (int)(p->len));
	
	/* Free the p buffer */
	pbuf_free(p);

   
}


void UdpTalkSendInit(void)
{
   
	/* Create a new UDP control block  */
	pUdpTalkSendPcb = udp_new();
  
	/* Bind the upcb to the UDP_PORT port */
	/* Using IP_ADDR_ANY allow the upcb to be used by any local interface */
	udp_bind(pUdpTalkSendPcb, IP_ADDR_ANY, PORT_TALK_SND_L);
      		
   
}


void UdpTalkSend(u8* pbuf, int len)
{

	struct pbuf *p;

	/* Connect to the remote client */
	udp_connect(pUdpTalkSendPcb, &AllBroadIpAddr, PORT_TALK_SND_R);
    
  
	/* allocate pbuf from pool*/
	p = pbuf_alloc(PBUF_TRANSPORT,len, PBUF_POOL);
      
	if (p != NULL)
	{
		/* copy data to pbuf */
		pbuf_take(p, (u8*)pbuf, len);
          
		/* send udp data */
		udp_send(pUdpTalkSendPcb, p); 

		/* free the UDP connection, so we can accept new clients */
		udp_disconnect(pUdpTalkSendPcb);
			
		/* free pbuf */
		pbuf_free(p);
		
	}

	
}



//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////



void UdpPlayclkRcvInit(void)
{
   
	/* Create a new UDP control block  */
	pUdpPlayclkRcvPcb = udp_new();
  
	/* Bind the upcb to the UDP_PORT port */
	/* Using IP_ADDR_ANY allow the upcb to be used by any local interface */
	udp_bind(pUdpPlayclkRcvPcb, IP_ADDR_ANY, PORT_PLAYCLK_RCV_L);
      		
	/* Set a receive callback for the upcb */
	udp_recv(pUdpPlayclkRcvPcb, UdpPlayclkReceiveCallback, NULL);

   
}


void UdpPlayclkReceiveCallback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port)
{

	NetPlayclkReceiveProcess((u8*)(p->payload), (int)(p->len));
	
	/* Free the p buffer */
	pbuf_free(p);

   
}


void UdpPlayclkSendInit(void)
{
   
	/* Create a new UDP control block  */
	pUdpPlayclkSendPcb = udp_new();
  
	/* Bind the upcb to the UDP_PORT port */
	/* Using IP_ADDR_ANY allow the upcb to be used by any local interface */
	udp_bind(pUdpPlayclkSendPcb, IP_ADDR_ANY, PORT_PLAYCLK_SND_L);
      		
   
}


void UdpPlayclkSend(u8* pbuf, int len)
{

	struct pbuf *p;

	/* Connect to the remote client */
	udp_connect(pUdpPlayclkSendPcb, &AllBroadIpAddr, PORT_PLAYCLK_SND_R);
    
  
	/* allocate pbuf from pool*/
	p = pbuf_alloc(PBUF_TRANSPORT,len, PBUF_POOL);
      
	if (p != NULL)
	{
		/* copy data to pbuf */
		pbuf_take(p, (u8*)pbuf, len);
          
		/* send udp data */
		udp_send(pUdpPlayclkSendPcb, p); 

		/* free the UDP connection, so we can accept new clients */
		udp_disconnect(pUdpPlayclkSendPcb);
			
		/* free pbuf */
		pbuf_free(p);
		
	}

	
}



//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////










//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////








