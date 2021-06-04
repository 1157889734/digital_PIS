//*******************************************************//
//						MyI2c.h								//
//*******************************************************//

#ifndef _MYI2C_H
#define  _MYI2C_H

#include "MyAll.h"


#define I2C_TIMEOUT_MAX		1024000



#define I2C_AT24C32			I2C2
#define I2C_AT24C32_SPEED   	100000
#define I2C_AT24C32_ADDR		0xA0


#define I2C_PT2257			I2C2
#define I2C_PT2257_SPEED   	100000
#define I2C_PT2257_ADDR		0x88



extern int I2cError;



void AT24C32_Init(void);

void AT24C32_ByteWrite(u16 addr, u8 byte);

u8 AT24C32_ByteRead(u16 addr);

void AT24C32_PageWrite(int page, u8 * pbuf, int len);

void AT24C32_PageRead(int page, u8 * pbuf, int len);




void PT2257Init(void);

void PT2257Write(int channel, int volume);






void I2cKsz8863Init(void);

u8 I2cKsz8863ReadByte(u8 reg_addr);

void I2cKsz8863WriteByte(u8 reg_addr, u8 value);



#endif


