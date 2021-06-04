
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
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	

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
	




	//---------------------dmp-------------------------------
	Device_id_GPIO_config();
	HC595_GPIO_config();
	
  
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








