
//*****************************************************//
//		MySwitch.c
//*****************************************************//


#include "MyAll.h"






void SwitchInit(void)
{
	u8 temp;


	I2cKsz8863Init();

	temp = I2cKsz8863ReadByte(0x00);
	Delay(1000);

	temp = temp;


	temp = I2cKsz8863ReadByte(0x1c);
	Delay(1000);

	temp = temp;

	I2cKsz8863WriteByte(0x1c, 0xff);
	Delay(1000);

	temp = I2cKsz8863ReadByte(0x1c);
	Delay(1000);

	temp = temp;



//	temp = I2cKsz8863ReadByte(0x02);
	Delay(1000);

	temp = temp;

//	I2cKsz8863WriteByte(0x02, 0x20);
	Delay(1000);

//	temp = I2cKsz8863ReadByte(0x02);
	Delay(1000);

	temp = temp;



	temp = I2cKsz8863ReadByte(0x01);
	Delay(1000);

	temp = temp;


	
	
	

}







