
//*****************************************************//

//*****************************************************//

#include "MyAll.h"


ErrorStatus HSEStartUpStatus;

/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_Configuration()
{

	/* Enable GPIOs clocks */
	RCC_AHB1PeriphClockCmd(	RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB |
                         				RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD |
          		 				RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF |
                         				RCC_AHB1Periph_GPIOG, ENABLE);

	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);  


	////Enable I2C2   ///////////////////////////////////////////////////
  	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);

	/* Reset I2C2 IP */
//	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, ENABLE);
	  
	/* Release reset signal of I2C2 IP */
//	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, DISABLE);


	




}


/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration()
{

	GPIO_InitTypeDef  GPIO_InitStructure;


////Configure LEDs   ///////////////////////////////////////////////////

  	/* Configure LED1 in output pushpull mode */
  	GPIO_InitStructure.GPIO_Pin = PIN_LED1;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
 	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	GPIO_Init(PORT_LED1, &GPIO_InitStructure);

  	/* Configure LED2 in output pushpull mode */
  	GPIO_InitStructure.GPIO_Pin = PIN_LED2;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
 	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	GPIO_Init(PORT_LED2, &GPIO_InitStructure);


////////////////////////////////////////////////////////////////////



////Configure I2C2   ///////////////////////////////////////////////////

	/*!< GPIO configuration */
	/* Connect PXx to I2C_SCL*/
	GPIO_PinAFConfig(PORT_I2C2, PINSOURCE_I2C2_SCL, GPIO_AF_I2C2);
	/* Connect PXx to I2C_SDA*/
	GPIO_PinAFConfig(PORT_I2C2, PINSOURCE_I2C2_SDA, GPIO_AF_I2C2); 


	 /*!< Configure I2C2 pins: SCL */   
	GPIO_InitStructure.GPIO_Pin = PIN_I2C2_SCL;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(PORT_I2C2, &GPIO_InitStructure);

	/*!< Configure sEE_I2C pins: SDA */
	GPIO_InitStructure.GPIO_Pin = PIN_I2C2_SDA;
	GPIO_Init(PORT_I2C2, &GPIO_InitStructure);


///////////////////////////////////////////////////////////////////



////Configure I2S2   ///////////////////////////////////////////////////

///*
  	GPIO_InitStructure.GPIO_Pin = PIN_I2S2_CKIN; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	GPIO_Init(PORT_I2S2_CKIN, &GPIO_InitStructure);     
	//Connect pins to I2S peripheral  //
  	GPIO_PinAFConfig(PORT_I2S2_CKIN, PINSOURCE_I2S2_CKIN, GPIO_AF_SPI2);  
  	
//*/


  	GPIO_InitStructure.GPIO_Pin = PIN_I2S2_MCK; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	GPIO_Init(PORT_I2S2_MCK, &GPIO_InitStructure);     
	//Connect pins to I2S peripheral  //
  	GPIO_PinAFConfig(PORT_I2S2_MCK, PINSOURCE_I2S2_MCK, GPIO_AF_SPI2);  


  	GPIO_InitStructure.GPIO_Pin = PIN_I2S2_WS; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	GPIO_Init(PORT_I2S2_WS, &GPIO_InitStructure);     
	//Connect pins to I2S peripheral  //
  	GPIO_PinAFConfig(PORT_I2S2_WS, PINSOURCE_I2S2_WS, GPIO_AF_SPI2);  


  	GPIO_InitStructure.GPIO_Pin = PIN_I2S2_SCK; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	GPIO_Init(PORT_I2S2_SCK, &GPIO_InitStructure);     
	//Connect pins to I2S peripheral  //
  	GPIO_PinAFConfig(PORT_I2S2_SCK, PINSOURCE_I2S2_SCK, GPIO_AF_SPI2);  


  	GPIO_InitStructure.GPIO_Pin = PIN_I2S2_SD; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	GPIO_Init(PORT_I2S2_SD, &GPIO_InitStructure);     
	//Connect pins to I2S peripheral  //
  	GPIO_PinAFConfig(PORT_I2S2_SD, PINSOURCE_I2S2_SD, GPIO_AF_SPI2);  



////////////////////////////////////////////////////////////////////


////Configure I2S3   ///////////////////////////////////////////////////

/*
  	GPIO_InitStructure.GPIO_Pin = PIN_I2S2_CKIN; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	GPIO_Init(PORT_I2S2_CKIN, &GPIO_InitStructure);     
	//Connect pins to I2S peripheral  //
  	GPIO_PinAFConfig(PORT_I2S2_CKIN, PINSOURCE_I2S2_CKIN, GPIO_AF_SPI2);  
  	
*/


  	GPIO_InitStructure.GPIO_Pin = PIN_I2S3_MCK; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	GPIO_Init(PORT_I2S3_MCK, &GPIO_InitStructure);     
	//Connect pins to I2S peripheral  //
  	GPIO_PinAFConfig(PORT_I2S3_MCK, PINSOURCE_I2S3_MCK, GPIO_AF_SPI3);  


  	GPIO_InitStructure.GPIO_Pin = PIN_I2S3_WS; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	GPIO_Init(PORT_I2S3_WS, &GPIO_InitStructure);     
	//Connect pins to I2S peripheral  //
  	GPIO_PinAFConfig(PORT_I2S3_WS, PINSOURCE_I2S3_WS, GPIO_AF_SPI3);  


  	GPIO_InitStructure.GPIO_Pin = PIN_I2S3_SCK; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	GPIO_Init(PORT_I2S3_SCK, &GPIO_InitStructure);     
	//Connect pins to I2S peripheral  //
  	GPIO_PinAFConfig(PORT_I2S3_SCK, PINSOURCE_I2S3_SCK, GPIO_AF_SPI3);  


  	GPIO_InitStructure.GPIO_Pin = PIN_I2S3_SD; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	GPIO_Init(PORT_I2S3_SD, &GPIO_InitStructure);     
	//Connect pins to I2S peripheral  //
  	GPIO_PinAFConfig(PORT_I2S3_SD, PINSOURCE_I2S3_SD, GPIO_AF_SPI3);  



/////////////////////////////////////////////////////////////////


////Configure L3   ///////////////////////////////////////////////////

  	/* Configure L3 in output pushpull mode */
  	GPIO_InitStructure.GPIO_Pin = PIN_L3DATA;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
 	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	GPIO_Init(PORT_L3DATA, &GPIO_InitStructure);

	/* Configure L3 in output pushpull mode */
  	GPIO_InitStructure.GPIO_Pin = PIN_L3CLK;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
 	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	GPIO_Init(PORT_L3CLK, &GPIO_InitStructure);

  	/* Configure L3 in output pushpull mode */
  	GPIO_InitStructure.GPIO_Pin = PIN_L3MODE;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
 	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	GPIO_Init(PORT_L3MODE, &GPIO_InitStructure);



/////////////////////////////////////////////////////////////////////


////Configure LEDdbs   ///////////////////////////////////////////////////


  	GPIO_InitStructure.GPIO_Pin = PIN_LED3PDB;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
 	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	GPIO_Init(PORT_LED3PDB, &GPIO_InitStructure);

  	GPIO_InitStructure.GPIO_Pin = PIN_LED0DB;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
 	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	GPIO_Init(PORT_LED0DB, &GPIO_InitStructure);

  	GPIO_InitStructure.GPIO_Pin = PIN_LED3DB;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
 	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	GPIO_Init(PORT_LED3DB, &GPIO_InitStructure);

  	GPIO_InitStructure.GPIO_Pin = PIN_LED6DB;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
 	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	GPIO_Init(PORT_LED6DB, &GPIO_InitStructure);

  	GPIO_InitStructure.GPIO_Pin = PIN_LED9DB;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
 	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	GPIO_Init(PORT_LED9DB, &GPIO_InitStructure);



/////////////////////////////////////////////////////////////////////


////Configure Addrs   ///////////////////////////////////////////////////


  	GPIO_InitStructure.GPIO_Pin = PIN_ADDR1;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_Init(PORT_ADDR1, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_ADDR2;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_Init(PORT_ADDR2, &GPIO_InitStructure);
  	
	GPIO_InitStructure.GPIO_Pin = PIN_ADDR3;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_Init(PORT_ADDR3, &GPIO_InitStructure);

  	GPIO_InitStructure.GPIO_Pin = PIN_ADDR4;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_Init(PORT_ADDR4, &GPIO_InitStructure);
  	


 ///////////////////////////////////////////////////////////////////// 


////Configure KEYs   ///////////////////////////////////////////////////


  	GPIO_InitStructure.GPIO_Pin = PIN_KEY_UP;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_Init(PORT_KEY_UP, &GPIO_InitStructure);


  	GPIO_InitStructure.GPIO_Pin = PIN_KEY_DOWN;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_Init(PORT_KEY_DOWN, &GPIO_InitStructure);




/////////////////////////////////////////////////////////////////////


////Configure AmpStates   ///////////////////////////////////////////////


  	GPIO_InitStructure.GPIO_Pin = PIN_SHORT;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_Init(PORT_SHORT, &GPIO_InitStructure);

  	GPIO_InitStructure.GPIO_Pin = PIN_OVP;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_Init(PORT_OVP, &GPIO_InitStructure);

  	GPIO_InitStructure.GPIO_Pin = PIN_UVP;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_Init(PORT_UVP, &GPIO_InitStructure);




////////////////////////////////////////////////////////////////////


////Configure DC24Vs   /////////////////////////////////////////////////


  	GPIO_InitStructure.GPIO_Pin = PIN_DC24V1;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_Init(PORT_DC24V1, &GPIO_InitStructure);


  	GPIO_InitStructure.GPIO_Pin = PIN_DC24V2;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_Init(PORT_DC24V2, &GPIO_InitStructure);




/////////////////////////////////////////////////////////////////////


////Configure EME   /////////////////////////////////////////////////////


  	GPIO_InitStructure.GPIO_Pin = PIN_EME;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_Init(PORT_EME, &GPIO_InitStructure);


/////////////////////////////////////////////////////////////////////


////Configure NoiseDetect   /////////////////////////////////////////////////


  	GPIO_InitStructure.GPIO_Pin = PIN_NOISE1_DETECT;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_Init(PORT_NOISE1_DETECT, &GPIO_InitStructure);


  	GPIO_InitStructure.GPIO_Pin = PIN_NOISE2_DETECT;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_Init(PORT_NOISE2_DETECT, &GPIO_InitStructure);




/////////////////////////////////////////////////////////////////////


////Configure LEDdbs   ///////////////////////////////////////////////////


  	GPIO_InitStructure.GPIO_Pin = PIN_FEEDDOG;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
 	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	GPIO_Init(PORT_FEEDDOG, &GPIO_InitStructure);


/////////////////////////////////////////////////////////////////////




	
	
  
}


/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures the nested vectored interrupt controller.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration()
{
	

}






void LED1Flash()
{
	GPIO_WriteBit(PORT_LED1, PIN_LED1, (BitAction)(1 - GPIO_ReadOutputDataBit(PORT_LED1, PIN_LED1)));	
}

void LED1On()
{
	GPIO_ResetBits(PORT_LED1, PIN_LED1);	
}

void LED1Off()
{
	GPIO_SetBits(PORT_LED1, PIN_LED1);	
}



void LED2Flash()
{
	GPIO_WriteBit(PORT_LED2, PIN_LED2, (BitAction)(1 - GPIO_ReadOutputDataBit(PORT_LED2, PIN_LED2)));	
}

void LED2On()
{
	GPIO_ResetBits(PORT_LED2, PIN_LED2);	
}

void LED2Off()
{
	GPIO_SetBits(PORT_LED2, PIN_LED2);	
}




void WriteL3Data (int bitvalue)
{

	if(bitvalue > 0)
 	{
		GPIO_SetBits(PORT_L3DATA, PIN_L3DATA);	  
 	}
	else
	{
		GPIO_ResetBits(PORT_L3DATA, PIN_L3DATA);	   
	}

}


void WriteL3Clock (int bitvalue)
{

	if(bitvalue > 0)
 	{
		GPIO_SetBits(PORT_L3CLK, PIN_L3CLK);	  
 	}
	else
	{
		GPIO_ResetBits(PORT_L3CLK, PIN_L3CLK);	   
	}
	

}


void WriteL3Mode (int bitvalue)
{

	if(bitvalue > 0)
 	{
		GPIO_SetBits(PORT_L3MODE, PIN_L3MODE);	  
 	}
	else
	{
		GPIO_ResetBits(PORT_L3MODE, PIN_L3MODE);	   
	}
	

}




/////////////////////////////////////////////////////////


void LED3pdbOn()
{
	GPIO_ResetBits(PORT_LED3PDB, PIN_LED3PDB);	
}
void LED3pdbOff()
{
	GPIO_SetBits(PORT_LED3PDB, PIN_LED3PDB);	
}


void LED0dbOn()
{
	GPIO_ResetBits(PORT_LED0DB, PIN_LED0DB);	
}
void LED0dbOff()
{
	GPIO_SetBits(PORT_LED0DB, PIN_LED0DB);	
}


void LED3dbOn()
{
	GPIO_ResetBits(PORT_LED3DB, PIN_LED3DB);	
}
void LED3dbOff()
{
	GPIO_SetBits(PORT_LED3DB, PIN_LED3DB);	
}


void LED6dbOn()
{
	GPIO_ResetBits(PORT_LED6DB, PIN_LED6DB);	
}
void LED6dbOff()
{
	GPIO_SetBits(PORT_LED6DB, PIN_LED6DB);	
}


void LED9dbOn()
{
	GPIO_ResetBits(PORT_LED9DB, PIN_LED9DB);	
}
void LED9dbOff()
{
	GPIO_SetBits(PORT_LED9DB, PIN_LED9DB);	
}





///////////////////////////////////////////////////////



u8 GetAddress(void)
{
	u8 addr;

	int rcv;


	addr = 0;
	
	rcv = GPIO_ReadInputDataBit(PORT_ADDR1, PIN_ADDR1);
	addr = addr + rcv;

	rcv = GPIO_ReadInputDataBit(PORT_ADDR2, PIN_ADDR2);
	addr = addr + rcv*2;

	rcv = GPIO_ReadInputDataBit(PORT_ADDR3, PIN_ADDR3);
	addr = addr + rcv*4;

	rcv = GPIO_ReadInputDataBit(PORT_ADDR4, PIN_ADDR4);
	addr = addr + rcv*8;

	addr=((~addr)&0x0F)+1; 	
	if(addr==9)
	{
		addr=2;
	}
	else if(addr==2)
	{
		addr=3;
	}
	else if(addr==10)
	{
		addr=4;
	}
	else if(addr==3)
	{
		addr=5;
	}
	else if(addr==11)
	{
		addr=6;
	}
	else if(addr==4)
	{
		addr=7;
	}
	else if(addr==12)
	{
		addr=8;
	}
	else if(addr==5)
	{
		addr=9;
	}
       	else if(addr==13)
	{
		addr=10;
	}
      	else if(addr==6)
	{
		addr=11;
	}
	else if(addr==14)
	{
		addr=12;
	}
	return addr;

}




///////////////////////////////////////////////////////


int GetKeyUp()
{
	int res;
	int rcv;

	rcv = GPIO_ReadInputDataBit(PORT_KEY_UP, PIN_KEY_UP);

	if(rcv == 0)
	{
		res = 0;
	}
	else
	{
		res = 1;
	}

	return res;

}

int GetKeyDown()
{
	int res;
	int rcv;

	rcv = GPIO_ReadInputDataBit(PORT_KEY_DOWN, PIN_KEY_DOWN);

	if(rcv == 0)
	{
		res = 0;
	}
	else
	{
		res = 1;
	}

	return res;

}





///////////////////////////////////////////////////////


int GetShort()
{
	int res;
	int rcv;

	rcv = GPIO_ReadInputDataBit(PORT_SHORT, PIN_SHORT);

	if(rcv == 0)
	{
		res = 0;
	}
	else
	{
		res = 1;
	}

	return res;

}


int GetOvp()
{
	int res;
	int rcv;

	rcv = GPIO_ReadInputDataBit(PORT_OVP, PIN_OVP);

	if(rcv == 0)
	{
		res = 0;
	}
	else
	{
		res = 1;
	}

	return res;

}


int GetUvp()
{
	int res;
	int rcv;

	rcv = GPIO_ReadInputDataBit(PORT_UVP, PIN_UVP);

	if(rcv == 0)
	{
		res = 0;
	}
	else
	{
		res = 1;
	}

	return res;

}



///////////////////////////////////////////////////////



int GetDC24V1()
{
	int res;
	int rcv;

	rcv = GPIO_ReadInputDataBit(PORT_DC24V1, PIN_DC24V1);

	if(rcv == 0)
	{
		res = 0;
	}
	else
	{
		res = 1;
	}

	return res;

}


int GetDC24V2()
{
	int res;
	int rcv;

	rcv = GPIO_ReadInputDataBit(PORT_DC24V2, PIN_DC24V2);

	if(rcv == 0)
	{
		res = 0;
	}
	else
	{
		res = 1;
	}

	return res;

}



///////////////////////////////////////////////////////



int GetEme()
{
	int res;
	int rcv;

	rcv = GPIO_ReadInputDataBit(PORT_EME, PIN_EME);

	if(rcv == 0)
	{
		res = 0;
	}
	else
	{
		res = 1;
	}

	return res;

}



///////////////////////////////////////////////////////


int GetNoise1Detect()
{
	int res;
	int rcv;

	rcv = GPIO_ReadInputDataBit(PORT_NOISE1_DETECT, PIN_NOISE1_DETECT);

	if(rcv == 0)
	{
		res = 0;
	}
	else
	{
		res = 1;
	}

	return res;

}


int GetNoise2Detect()
{
	int res;
	int rcv;

	rcv = GPIO_ReadInputDataBit(PORT_NOISE2_DETECT, PIN_NOISE2_DETECT);

	if(rcv == 0)
	{
		res = 0;
	}
	else
	{
		res = 1;
	}

	return res;

}



////////////////////////////////////////////////////////////////////



void FeedDog()
{
	GPIO_WriteBit(PORT_FEEDDOG, PIN_FEEDDOG, (BitAction)(1 - GPIO_ReadOutputDataBit(PORT_FEEDDOG, PIN_FEEDDOG)));	
}




////////////////////////////////////////////////////////////////////


void Wwdg_Init(void)
{

	/* Check if the system has resumed from WWDG reset */
	if (RCC_GetFlagStatus(RCC_FLAG_WWDGRST) != RESET)
	{ 
		/* Clear reset flags */
		RCC_ClearFlag();
	}

	WWDG_DeInit();

	/* WWDG configuration */
	/* Enable WWDG clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);

	/* WWDG clock counter = (PCLK1 (30MHz)/4096)/8 = 915 Hz (~1092 us)  */
	WWDG_SetPrescaler(WWDG_Prescaler_8);

	/* Set Window value to 80; WWDG counter should be refreshed only when the counter
	is below 80 (and greater than 64) otherwise a reset will be generated */
	WWDG_SetWindowValue(80);

//	WWDG_SetWindowValue(126);

	/* Enable WWDG and set counter value to 127, WWDG timeout = ~1092 us * 64 = 69.9 ms 
	In this case the refresh window is: ~1092 * (127-80) = 51.3 ms < refresh window < ~1092 * 64 = 69.9ms
	*/
	WWDG_Enable(127);

}

void Wwdg_Feed(void)
{
	/* Update WWDG counter */
//	WWDG_SetCounter(127);

}




/////////////////////////////////////////////////////////////////////////////










