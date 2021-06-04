
//*****************************************************//
//		MyI2c.c
//*****************************************************//


#include "MyAll.h"



int I2cTimeout;

int I2cError;




void AT24C32_Init(void)
{

	I2C_InitTypeDef  I2C_InitStructure;

		
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = I2C_AT24C32_ADDR;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = I2C_AT24C32_SPEED;
  
	/* I2C2 Peripheral Enable */
	I2C_Cmd(I2C_AT24C32, ENABLE);
	/* Apply I2C2 configuration after enabling it */
	I2C_Init(I2C_AT24C32, &I2C_InitStructure);

  
}





void AT24C32_ByteWrite(u16 addr, u8 value)
{

	u8 addr_l;
	u8 addr_h;


	addr_h = (u8)((addr>>8) & 0x00ff);
	addr_l = (u8)((addr) & 0x00ff);


	I2cTimeout = 0;
	I2cError = 0;
	while(I2C_GetFlagStatus(I2C_AT24C32, I2C_FLAG_BUSY))
	{
		I2cTimeout++;
		if(I2cTimeout > I2C_TIMEOUT_MAX)
		{
			I2cError = 1;
			return;

		}
		
	}
  
	/*!< Send START condition */
	I2C_GenerateSTART(I2C_AT24C32, ENABLE);
	
  
	/*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
	I2cTimeout = 0;
	I2cError = 0;
	while(!I2C_CheckEvent(I2C_AT24C32, I2C_EVENT_MASTER_MODE_SELECT))
	{
		I2cTimeout++;
		if(I2cTimeout > I2C_TIMEOUT_MAX)
		{
			I2cError = 2;
			return;

		}
	}
	
  
	/*!< Send EEPROM address for write */
	I2C_Send7bitAddress(I2C_AT24C32, I2C_AT24C32_ADDR, I2C_Direction_Transmitter);

  	/*!< Test on EV6 and clear it */
  	I2cTimeout = 0;
	I2cError = 0;
  	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  	{
		I2cTimeout++;
		if(I2cTimeout > I2C_TIMEOUT_MAX)
		{
			I2cError = 3;
			return;

		}
  	} 


	/*!< Send the EEPROM's internal address to read from: Only one byte address */
	I2C_SendData(I2C_AT24C32, addr_h);  

	/*!< Test on EV8 and clear it */
	I2cTimeout = 0;
	I2cError = 0;
	while(I2C_GetFlagStatus(I2C_AT24C32, I2C_FLAG_BTF) == RESET)
	{
  		I2cTimeout++;
		if(I2cTimeout > I2C_TIMEOUT_MAX)
		{
			I2cError = 4;
			return;

		}
	}

	/*!< Send the EEPROM's internal address to read from: Only one byte address */
	I2C_SendData(I2C_AT24C32, addr_l);  

	/*!< Test on EV8 and clear it */
	I2cTimeout = 0;
	I2cError = 0;
	while(I2C_GetFlagStatus(I2C_AT24C32, I2C_FLAG_BTF) == RESET)
	{
  		I2cTimeout++;
		if(I2cTimeout > I2C_TIMEOUT_MAX)
		{
			I2cError = 5;
			return;

		}
	}




	I2C_SendData(I2C_AT24C32, value);  


	/*!< Test on EV8 and clear it */
	I2cTimeout = 0;
	I2cError = 0;
	while(I2C_GetFlagStatus(I2C_AT24C32, I2C_FLAG_BTF) == RESET)
	{
  		I2cTimeout++;
		if(I2cTimeout > I2C_TIMEOUT_MAX)
		{
			I2cError = 6;
			return;

		}
	}

	I2cTimeout = 0;
	I2cError = 0;
	while(!I2C_CheckEvent(I2C_AT24C32, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
	{
		I2cTimeout++;
		if(I2cTimeout > I2C_TIMEOUT_MAX)
		{
			I2cError = 7;
			return;

		}
	}  


	/*!< Send STOP Condition */
	I2C_GenerateSTOP(I2C_AT24C32, ENABLE);


	


}




u8 AT24C32_ByteRead(u16 addr)
{
	u8 out_byte;

	u8 addr_l;
	u8 addr_h;


	addr_h = (u8)((addr>>8) & 0x00ff);
	addr_l = (u8)((addr) & 0x00ff);
	
	I2cTimeout = 0;
	I2cError = 0;
	while(I2C_GetFlagStatus(I2C_AT24C32, I2C_FLAG_BUSY))
	{
		I2cTimeout++;
		if(I2cTimeout > I2C_TIMEOUT_MAX)
		{
			I2cError = 8;
			return (u8)(-1);

		}
	}
  
	/*!< Send START condition */
	I2C_GenerateSTART(I2C_AT24C32, ENABLE);
	
  
	/*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
	I2cTimeout = 0;
	I2cError = 0;
	while(!I2C_CheckEvent(I2C_AT24C32, I2C_EVENT_MASTER_MODE_SELECT))
	{
		I2cTimeout++;
		if(I2cTimeout > I2C_TIMEOUT_MAX)
		{
			I2cError = 9;
			return (u8)(-1);

		}
	}
	
  
	/*!< Send EEPROM address for write */
	I2C_Send7bitAddress(I2C_AT24C32, I2C_AT24C32_ADDR, I2C_Direction_Transmitter);

  	/*!< Test on EV6 and clear it */
  	I2cTimeout = 0;
	I2cError = 0;
  	while(!I2C_CheckEvent(I2C_AT24C32, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  	{
		I2cTimeout++;
		if(I2cTimeout > I2C_TIMEOUT_MAX)
		{
			I2cError = 10;
			return (u8)(-1);

		}
  	} 


	/*!< Send the EEPROM's internal address to read from: Only one byte address */
	I2C_SendData(I2C_AT24C32, addr_h);  

	/*!< Test on EV8 and clear it */
	I2cTimeout = 0;
	I2cError = 0;
	while(I2C_GetFlagStatus(I2C_AT24C32, I2C_FLAG_BTF) == RESET)
	{
  		I2cTimeout++;
		if(I2cTimeout > I2C_TIMEOUT_MAX)
		{
			I2cError = 11;
			return (u8)(-1);

		}
	}

	/*!< Send the EEPROM's internal address to read from: Only one byte address */
	I2C_SendData(I2C_AT24C32, addr_l);  

	/*!< Test on EV8 and clear it */
	I2cTimeout = 0;
	I2cError = 0;
	while(I2C_GetFlagStatus(I2C_AT24C32, I2C_FLAG_BTF) == RESET)
	{
  		I2cTimeout++;
		if(I2cTimeout > I2C_TIMEOUT_MAX)
		{
			I2cError = 11;
			return (u8)(-1);

		}
	}

	


	/*!< Send STRAT condition a second time */  
	I2C_GenerateSTART(I2C_AT24C32, ENABLE);
  
	/*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
	I2cTimeout = 0;
	I2cError = 0;
	while(!I2C_CheckEvent(I2C_AT24C32, I2C_EVENT_MASTER_MODE_SELECT))
	{
   		I2cTimeout++;
		if(I2cTimeout > I2C_TIMEOUT_MAX)
		{
			I2cError = 12;
			return (u8)(-1);

		}
	} 
  
	/*!< Send EEPROM address for read */
	I2C_Send7bitAddress(I2C_AT24C32, I2C_AT24C32_ADDR, I2C_Direction_Receiver);  


	/* Wait on ADDR flag to be set (ADDR is still not cleared at this level */
	I2cTimeout = 0;
	I2cError = 0;
	while(I2C_GetFlagStatus(I2C_AT24C32, I2C_FLAG_ADDR) == RESET)
	{
		I2cTimeout++;
		if(I2cTimeout > I2C_TIMEOUT_MAX)
		{
			I2cError = 13;
			return (u8)(-1);

		}
	}     
    
	/*!< Disable Acknowledgment */
	I2C_AcknowledgeConfig(I2C_AT24C32, DISABLE);   
    
	/* Clear ADDR register by reading SR1 then SR2 register (SR1 has already been read) */
	(void)I2C2->SR2;
    
	/*!< Send STOP Condition */
	I2C_GenerateSTOP(I2C_AT24C32, ENABLE);
    
	/* Wait for the byte to be received */
	I2cTimeout = 0;
	I2cError = 0;
	while(I2C_GetFlagStatus(I2C_AT24C32, I2C_FLAG_RXNE) == RESET)
	{
     		I2cTimeout++;
		if(I2cTimeout > I2C_TIMEOUT_MAX)
		{
			I2cError = 14;
			return (u8)(-1);

		}
	}
    
	/*!< Read the byte received from the EEPROM */
	out_byte = I2C_ReceiveData(I2C_AT24C32);
 
    
	/* Wait to make sure that STOP control bit has been cleared */
	I2cTimeout = 0;
	I2cError = 0;
	while(I2C_AT24C32->CR1 & I2C_CR1_STOP)
	{
 		I2cTimeout++;
		if(I2cTimeout > I2C_TIMEOUT_MAX)
		{
			I2cError = 15;
			return (u8)(-1);

		}
	}  
    
	/*!< Re-Enable Acknowledgment to be ready for another reception */
	I2C_AcknowledgeConfig(I2C_AT24C32, ENABLE);    



	return out_byte;



}



void AT24C32_PageWrite(int page, u8 * pbuf, int len)
{

	u16 addr;

	u8 addr_l;
	u8 addr_h;

	int i;

	
	addr = ((u16)(page)) << 5;

	addr_h = (u8)((addr>>8) & 0x00ff);
	addr_l = (u8)((addr) & 0x00ff);

	if(len > 32)
	{
		len = 32;

	}


	I2cTimeout = 0;
	I2cError = 0;
	while(I2C_GetFlagStatus(I2C_AT24C32, I2C_FLAG_BUSY))
	{
		I2cTimeout++;
		if(I2cTimeout > I2C_TIMEOUT_MAX)
		{
			I2cError = 16;
			return;

		}
	}
  
	/*!< Send START condition */
	I2C_GenerateSTART(I2C_AT24C32, ENABLE);
	
  
	/*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
	I2cTimeout = 0;
	I2cError = 0;
	while(!I2C_CheckEvent(I2C_AT24C32, I2C_EVENT_MASTER_MODE_SELECT))
	{
		I2cTimeout++;
		if(I2cTimeout > I2C_TIMEOUT_MAX)
		{
			I2cError = 17;
			return;

		}
	}
	
  
	/*!< Send EEPROM address for write */
	I2C_Send7bitAddress(I2C_AT24C32, I2C_AT24C32_ADDR, I2C_Direction_Transmitter);

  	/*!< Test on EV6 and clear it */
  	I2cTimeout = 0;
	I2cError = 0;
  	while(!I2C_CheckEvent(I2C_AT24C32, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  	{
		I2cTimeout++;
		if(I2cTimeout > I2C_TIMEOUT_MAX)
		{
			I2cError = 18;
			return;

		}
  	} 


	/*!< Send the EEPROM's internal address to read from: Only one byte address */
	I2C_SendData(I2C_AT24C32, addr_h);  

	/*!< Test on EV8 and clear it */
	I2cTimeout = 0;
	I2cError = 0;
	while(I2C_GetFlagStatus(I2C_AT24C32, I2C_FLAG_BTF) == RESET)
	{
  		I2cTimeout++;
		if(I2cTimeout > I2C_TIMEOUT_MAX)
		{
			I2cError = 19;
			return;

		}
	}

	/*!< Send the EEPROM's internal address to read from: Only one byte address */
	I2C_SendData(I2C_AT24C32, addr_l);  

	/*!< Test on EV8 and clear it */
	I2cTimeout = 0;
	I2cError = 0;
	while(I2C_GetFlagStatus(I2C_AT24C32, I2C_FLAG_BTF) == RESET)
	{
  		I2cTimeout++;
		if(I2cTimeout > I2C_TIMEOUT_MAX)
		{
			I2cError = 20;
			return;

		}
	}

	
	for(i=0; i<len; i++)
	{

		I2C_SendData(I2C_AT24C32, *(pbuf + i));  


		/*!< Test on EV8 and clear it */
		I2cTimeout = 0;
		I2cError = 0;
		while(I2C_GetFlagStatus(I2C_AT24C32, I2C_FLAG_BTF) == RESET)
		{
  			I2cTimeout++;
			if(I2cTimeout > I2C_TIMEOUT_MAX)
			{
				I2cError = 21;
				return;

			}
		}

	}
	

	I2cTimeout = 0;
	I2cError = 0;
	while(!I2C_CheckEvent(I2C_AT24C32, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
	{
		I2cTimeout++;
		if(I2cTimeout > I2C_TIMEOUT_MAX)
		{
			I2cError = 22;
			return;

		}
	}  


	/*!< Send STOP Condition */
	I2C_GenerateSTOP(I2C_AT24C32, ENABLE);




}


void AT24C32_PageRead(int page, u8 * pbuf, int len)
{

	u16 addr;

	u8 addr_l;
	u8 addr_h;

	int i;
	

	addr = ((u16)(page)) << 5;

	addr_h = (u8)((addr>>8) & 0x00ff);
	addr_l = (u8)((addr) & 0x00ff);

	if(len > 32)
	{
		len = 32;

	}
	
	I2cTimeout = 0;
	I2cError = 0;
	while(I2C_GetFlagStatus(I2C_AT24C32, I2C_FLAG_BUSY))
	{
		I2cTimeout++;
		if(I2cTimeout > I2C_TIMEOUT_MAX)
		{
			I2cError = 23;
			return;

		}
	}
  
	/*!< Send START condition */
	I2C_GenerateSTART(I2C_AT24C32, ENABLE);
	
  
	/*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
	I2cTimeout = 0;
	I2cError = 0;
	while(!I2C_CheckEvent(I2C_AT24C32, I2C_EVENT_MASTER_MODE_SELECT))
	{
		I2cTimeout++;
		if(I2cTimeout > I2C_TIMEOUT_MAX)
		{
			I2cError = 24;
			return;

		}
	}
	
  
	/*!< Send EEPROM address for write */
	I2C_Send7bitAddress(I2C_AT24C32, I2C_AT24C32_ADDR, I2C_Direction_Transmitter);

  	/*!< Test on EV6 and clear it */
  	I2cTimeout = 0;
	I2cError = 0;
  	while(!I2C_CheckEvent(I2C_AT24C32, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  	{
		I2cTimeout++;
		if(I2cTimeout > I2C_TIMEOUT_MAX)
		{
			I2cError = 25;
			return;

		}
  	} 


	/*!< Send the EEPROM's internal address to read from: Only one byte address */
	I2C_SendData(I2C_AT24C32, addr_h);  

	/*!< Test on EV8 and clear it */
	I2cTimeout = 0;
	I2cError = 0;
	while(I2C_GetFlagStatus(I2C_AT24C32, I2C_FLAG_BTF) == RESET)
	{
  		I2cTimeout++;
		if(I2cTimeout > I2C_TIMEOUT_MAX)
		{
			I2cError = 26;
			return;

		}
	}

	/*!< Send the EEPROM's internal address to read from: Only one byte address */
	I2C_SendData(I2C_AT24C32, addr_l);  

	/*!< Test on EV8 and clear it */
	I2cTimeout = 0;
	I2cError = 0;
	while(I2C_GetFlagStatus(I2C_AT24C32, I2C_FLAG_BTF) == RESET)
	{
  		I2cTimeout++;
		if(I2cTimeout > I2C_TIMEOUT_MAX)
		{
			I2cError = 27;
			return;

		}
	}

	
	for(i=0; i<len; i++)
	{

		/*!< Send STRAT condition a second time */  
		I2C_GenerateSTART(I2C_AT24C32, ENABLE);
  
		/*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
		I2cTimeout = 0;
		I2cError = 0;
		while(!I2C_CheckEvent(I2C_AT24C32, I2C_EVENT_MASTER_MODE_SELECT))
		{
   			I2cTimeout++;
			if(I2cTimeout > I2C_TIMEOUT_MAX)
			{
				I2cError = 28;
				return;

			}
		} 
  
		/*!< Send EEPROM address for read */
		I2C_Send7bitAddress(I2C_AT24C32, I2C_AT24C32_ADDR, I2C_Direction_Receiver);  


		/* Wait on ADDR flag to be set (ADDR is still not cleared at this level */
		I2cTimeout = 0;
		I2cError = 0;
		while(I2C_GetFlagStatus(I2C_AT24C32, I2C_FLAG_ADDR) == RESET)
		{
			I2cTimeout++;
			if(I2cTimeout > I2C_TIMEOUT_MAX)
			{
				I2cError = 29;
				return;

			}
		}     
    
		/*!< Disable Acknowledgment */
		I2C_AcknowledgeConfig(I2C_AT24C32, DISABLE);   
    
		/* Clear ADDR register by reading SR1 then SR2 register (SR1 has already been read) */
		(void)I2C2->SR2;
    
		/*!< Send STOP Condition */
		I2C_GenerateSTOP(I2C_AT24C32, ENABLE);
    
		/* Wait for the byte to be received */
		I2cTimeout = 0;
		I2cError = 0;
		while(I2C_GetFlagStatus(I2C_AT24C32, I2C_FLAG_RXNE) == RESET)
		{
     			I2cTimeout++;
			if(I2cTimeout > I2C_TIMEOUT_MAX)
			{
				I2cError = 30;
				return;

			}
		}
    
		/*!< Read the byte received from the EEPROM */
		*(pbuf + i) = I2C_ReceiveData(I2C_AT24C32);
 
    
		/* Wait to make sure that STOP control bit has been cleared */
		I2cTimeout = 0;
		I2cError = 0;
		while(I2C_AT24C32->CR1 & I2C_CR1_STOP)
		{
 			I2cTimeout++;
			if(I2cTimeout > I2C_TIMEOUT_MAX)
			{
				I2cError = 31;
				return;

			}
		}  
    
		/*!< Re-Enable Acknowledgment to be ready for another reception */
		I2C_AcknowledgeConfig(I2C_AT24C32, ENABLE);    

	}


}






void PT2257Init(void)
{

	I2C_InitTypeDef  I2C_InitStructure;

		
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = I2C_PT2257_ADDR;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = I2C_PT2257_SPEED;
  
	/* I2C2 Peripheral Enable */
	I2C_Cmd(I2C_PT2257, ENABLE);
	/* Apply I2C2 configuration after enabling it */
	I2C_Init(I2C_PT2257, &I2C_InitStructure);



}



void PT2257Write(int channel, int volume)
{
	
	u8 att1;
	u8 att2;

	u8 v1;
	u8 v2;


	if(volume > 79)
	{
		volume = 79;
	}
	else if(volume < 0)
	{
		volume = 0;

	}

	v1 = (u8)(volume / 10);
	v2 = (u8)(volume % 10);
	

	if(channel == 1)
	{
		att1 = 0xB0 |v1;
		att2 = 0xA0 |v2;

	}
	else if(channel == 2)
	{
		att1 = 0x30 |v1;
		att2 = 0x20 |v2;

	}
	else
	{
		att1 = 0xE0 |v1;
		att2 = 0xD0 |v2;

	}



	I2cTimeout = 0;
	I2cError = 0;
	while(I2C_GetFlagStatus(I2C_PT2257, I2C_FLAG_BUSY))
	{
		I2cTimeout++;
		if(I2cTimeout > I2C_TIMEOUT_MAX)
		{
			I2cError = 101;
			return;

		}
	}
  
	/*!< Send START condition */
	I2C_GenerateSTART(I2C_PT2257, ENABLE);
	
  
	/*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
	I2cTimeout = 0;
	I2cError = 0;
	while(!I2C_CheckEvent(I2C_PT2257, I2C_EVENT_MASTER_MODE_SELECT))
	{
		I2cTimeout++;
		if(I2cTimeout > I2C_TIMEOUT_MAX)
		{
			I2cError = 102;
			return;

		}
	}
	
  
	/*!< Send EEPROM address for write */
	I2C_Send7bitAddress(I2C_PT2257, I2C_PT2257_ADDR, I2C_Direction_Transmitter);

  	/*!< Test on EV6 and clear it */
  	I2cTimeout = 0;
	I2cError = 0;
  	while(!I2C_CheckEvent(I2C_PT2257, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  	{
		I2cTimeout++;
		if(I2cTimeout > I2C_TIMEOUT_MAX)
		{
			I2cError = 103;
			return;

		}
  	} 


	
	I2C_SendData(I2C_PT2257, att1);  

	/*!< Test on EV8 and clear it */
	I2cTimeout = 0;
	I2cError = 0;
	while(I2C_GetFlagStatus(I2C_PT2257, I2C_FLAG_BTF) == RESET)
	{
  		I2cTimeout++;
		if(I2cTimeout > I2C_TIMEOUT_MAX)
		{
			I2cError = 104;
			return;

		}
	}

	
	I2C_SendData(I2C_PT2257, att2);  

	/*!< Test on EV8 and clear it */
	I2cTimeout = 0;
	I2cError = 0;
	while(I2C_GetFlagStatus(I2C_PT2257, I2C_FLAG_BTF) == RESET)
	{
  		I2cTimeout++;
		if(I2cTimeout > I2C_TIMEOUT_MAX)
		{
			I2cError = 105;
			return;

		}
	}

	
	
	I2cTimeout = 0;
	I2cError = 0;
	while(!I2C_CheckEvent(I2C_PT2257, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
	{
		I2cTimeout++;
		if(I2cTimeout > I2C_TIMEOUT_MAX)
		{
			I2cError = 106;
			return;

		}
	}  


	/*!< Send STOP Condition */
	I2C_GenerateSTOP(I2C_PT2257, ENABLE);

	



	 	
}





























void I2cKsz8863Init(void)
{


}



u8 I2cKsz8863ReadByte(u8 reg_addr)
{

	 return 0;
}

void I2cKsz8863WriteByte(u8 reg_addr, u8 value)
{



}














