
//*****************************************************//
//		MyI2c.c
//*****************************************************//


#include "MyAll.h"









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


	while(I2C_GetFlagStatus(I2C_AT24C32, I2C_FLAG_BUSY))
	{
		
	}
  
	/*!< Send START condition */
	I2C_GenerateSTART(I2C_AT24C32, ENABLE);
	
  
	/*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
	while(!I2C_CheckEvent(I2C_AT24C32, I2C_EVENT_MASTER_MODE_SELECT))
	{
	
	}
	
  
	/*!< Send EEPROM address for write */
	I2C_Send7bitAddress(I2C_AT24C32, I2C_AT24C32_ADDR, I2C_Direction_Transmitter);

  	/*!< Test on EV6 and clear it */
  	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  	{
		
  	} 


	/*!< Send the EEPROM's internal address to read from: Only one byte address */
	I2C_SendData(I2C_AT24C32, addr_h);  

	/*!< Test on EV8 and clear it */
	while(I2C_GetFlagStatus(I2C_AT24C32, I2C_FLAG_BTF) == RESET)
	{
  
	}

	/*!< Send the EEPROM's internal address to read from: Only one byte address */
	I2C_SendData(I2C_AT24C32, addr_l);  

	/*!< Test on EV8 and clear it */
	while(I2C_GetFlagStatus(I2C_AT24C32, I2C_FLAG_BTF) == RESET)
	{
  
	}




	I2C_SendData(I2C_AT24C32, value);  


	/*!< Test on EV8 and clear it */
	while(I2C_GetFlagStatus(I2C_AT24C32, I2C_FLAG_BTF) == RESET)
	{
  
	}

	while(!I2C_CheckEvent(I2C_AT24C32, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
	{

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
	

	while(I2C_GetFlagStatus(I2C_AT24C32, I2C_FLAG_BUSY))
	{
		
	}
  
	/*!< Send START condition */
	I2C_GenerateSTART(I2C_AT24C32, ENABLE);
	
  
	/*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
	while(!I2C_CheckEvent(I2C_AT24C32, I2C_EVENT_MASTER_MODE_SELECT))
	{
	
	}
	
  
	/*!< Send EEPROM address for write */
	I2C_Send7bitAddress(I2C_AT24C32, I2C_AT24C32_ADDR, I2C_Direction_Transmitter);

  	/*!< Test on EV6 and clear it */
  	while(!I2C_CheckEvent(I2C_AT24C32, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  	{
		
  	} 


	/*!< Send the EEPROM's internal address to read from: Only one byte address */
	I2C_SendData(I2C_AT24C32, addr_h);  

	/*!< Test on EV8 and clear it */
	while(I2C_GetFlagStatus(I2C_AT24C32, I2C_FLAG_BTF) == RESET)
	{
  
	}

	/*!< Send the EEPROM's internal address to read from: Only one byte address */
	I2C_SendData(I2C_AT24C32, addr_l);  

	/*!< Test on EV8 and clear it */
	while(I2C_GetFlagStatus(I2C_AT24C32, I2C_FLAG_BTF) == RESET)
	{
  
	}

	


	/*!< Send STRAT condition a second time */  
	I2C_GenerateSTART(I2C_AT24C32, ENABLE);
  
	/*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
	while(!I2C_CheckEvent(I2C_AT24C32, I2C_EVENT_MASTER_MODE_SELECT))
	{
   
	} 
  
	/*!< Send EEPROM address for read */
	I2C_Send7bitAddress(I2C_AT24C32, I2C_AT24C32_ADDR, I2C_Direction_Receiver);  


	/* Wait on ADDR flag to be set (ADDR is still not cleared at this level */
	while(I2C_GetFlagStatus(I2C_AT24C32, I2C_FLAG_ADDR) == RESET)
	{
	
	}     
    
	/*!< Disable Acknowledgment */
	I2C_AcknowledgeConfig(I2C_AT24C32, DISABLE);   
    
	/* Clear ADDR register by reading SR1 then SR2 register (SR1 has already been read) */
	(void)I2C2->SR2;
    
	/*!< Send STOP Condition */
	I2C_GenerateSTOP(I2C_AT24C32, ENABLE);
    
	/* Wait for the byte to be received */
	while(I2C_GetFlagStatus(I2C_AT24C32, I2C_FLAG_RXNE) == RESET)
	{
     
	}
    
	/*!< Read the byte received from the EEPROM */
	out_byte = I2C_ReceiveData(I2C_AT24C32);
 
    
	/* Wait to make sure that STOP control bit has been cleared */
	while(I2C_AT24C32->CR1 & I2C_CR1_STOP)
	{
 
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


	while(I2C_GetFlagStatus(I2C_AT24C32, I2C_FLAG_BUSY))
	{
		
	}
  
	/*!< Send START condition */
	I2C_GenerateSTART(I2C_AT24C32, ENABLE);
	
  
	/*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
	while(!I2C_CheckEvent(I2C_AT24C32, I2C_EVENT_MASTER_MODE_SELECT))
	{
	
	}
	
  
	/*!< Send EEPROM address for write */
	I2C_Send7bitAddress(I2C_AT24C32, I2C_AT24C32_ADDR, I2C_Direction_Transmitter);

  	/*!< Test on EV6 and clear it */
  	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  	{
		
  	} 


	/*!< Send the EEPROM's internal address to read from: Only one byte address */
	I2C_SendData(I2C_AT24C32, addr_h);  

	/*!< Test on EV8 and clear it */
	while(I2C_GetFlagStatus(I2C_AT24C32, I2C_FLAG_BTF) == RESET)
	{
  
	}

	/*!< Send the EEPROM's internal address to read from: Only one byte address */
	I2C_SendData(I2C_AT24C32, addr_l);  

	/*!< Test on EV8 and clear it */
	while(I2C_GetFlagStatus(I2C_AT24C32, I2C_FLAG_BTF) == RESET)
	{
  
	}

	
	for(i=0; i<len; i++)
	{

		I2C_SendData(I2C_AT24C32, *(pbuf + i));  


		/*!< Test on EV8 and clear it */
		while(I2C_GetFlagStatus(I2C_AT24C32, I2C_FLAG_BTF) == RESET)
		{
  
		}

	}
	

	while(!I2C_CheckEvent(I2C_AT24C32, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
	{

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
	

	while(I2C_GetFlagStatus(I2C_AT24C32, I2C_FLAG_BUSY))
	{
		
	}
  
	/*!< Send START condition */
	I2C_GenerateSTART(I2C_AT24C32, ENABLE);
	
  
	/*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
	while(!I2C_CheckEvent(I2C_AT24C32, I2C_EVENT_MASTER_MODE_SELECT))
	{
	
	}
	
  
	/*!< Send EEPROM address for write */
	I2C_Send7bitAddress(I2C_AT24C32, I2C_AT24C32_ADDR, I2C_Direction_Transmitter);

  	/*!< Test on EV6 and clear it */
  	while(!I2C_CheckEvent(I2C_AT24C32, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  	{
		
  	} 


	/*!< Send the EEPROM's internal address to read from: Only one byte address */
	I2C_SendData(I2C_AT24C32, addr_h);  

	/*!< Test on EV8 and clear it */
	while(I2C_GetFlagStatus(I2C_AT24C32, I2C_FLAG_BTF) == RESET)
	{
  
	}

	/*!< Send the EEPROM's internal address to read from: Only one byte address */
	I2C_SendData(I2C_AT24C32, addr_l);  

	/*!< Test on EV8 and clear it */
	while(I2C_GetFlagStatus(I2C_AT24C32, I2C_FLAG_BTF) == RESET)
	{
  
	}

	
	for(i=0; i<len; i++)
	{

	/*!< Send STRAT condition a second time */  
	I2C_GenerateSTART(I2C_AT24C32, ENABLE);
  
	/*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
	while(!I2C_CheckEvent(I2C_AT24C32, I2C_EVENT_MASTER_MODE_SELECT))
	{
   
	} 
  
	/*!< Send EEPROM address for read */
	I2C_Send7bitAddress(I2C_AT24C32, I2C_AT24C32_ADDR, I2C_Direction_Receiver);  


	/* Wait on ADDR flag to be set (ADDR is still not cleared at this level */
	while(I2C_GetFlagStatus(I2C_AT24C32, I2C_FLAG_ADDR) == RESET)
	{
	
	}     
    
	/*!< Disable Acknowledgment */
	I2C_AcknowledgeConfig(I2C_AT24C32, DISABLE);   
    
	/* Clear ADDR register by reading SR1 then SR2 register (SR1 has already been read) */
	(void)I2C2->SR2;
    
	/*!< Send STOP Condition */
	I2C_GenerateSTOP(I2C_AT24C32, ENABLE);
    
	/* Wait for the byte to be received */
	while(I2C_GetFlagStatus(I2C_AT24C32, I2C_FLAG_RXNE) == RESET)
	{
     
	}
    
	/*!< Read the byte received from the EEPROM */
	*(pbuf + i) = I2C_ReceiveData(I2C_AT24C32);
 
    
	/* Wait to make sure that STOP control bit has been cleared */
	while(I2C_AT24C32->CR1 & I2C_CR1_STOP)
	{
 
	}  
    
	/*!< Re-Enable Acknowledgment to be ready for another reception */
	I2C_AcknowledgeConfig(I2C_AT24C32, ENABLE);    

	}


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














