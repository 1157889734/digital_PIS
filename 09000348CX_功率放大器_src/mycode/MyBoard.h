//*****************************************************//

//*****************************************************//

#ifndef _MYBOARD_H
#define  _MYBOARD_H

#include "MyAll.h"




#define PORT_LED1		GPIOF
#define PIN_LED1			GPIO_Pin_2

#define PORT_LED2		GPIOE
#define PIN_LED2			GPIO_Pin_10


#define PORT_I2C2			GPIOF
#define PIN_I2C2_SDA		GPIO_Pin_0
#define PIN_I2C2_SCL		GPIO_Pin_1
#define PINSOURCE_I2C2_SDA		GPIO_PinSource0
#define PINSOURCE_I2C2_SCL		GPIO_PinSource1




#define PORT_I2S2_CKIN			GPIOC
#define PIN_I2S2_CKIN				GPIO_Pin_9
#define PINSOURCE_I2S2_CKIN		GPIO_PinSource9

#define PORT_I2S2_WS				GPIOB
#define PORT_I2S2_SCK			GPIOB
#define PORT_I2S2_SD				GPIOB
#define PORT_I2S2_MCK			GPIOC
#define PIN_I2S2_WS				GPIO_Pin_12
#define PIN_I2S2_SCK				GPIO_Pin_13
#define PIN_I2S2_SD				GPIO_Pin_15
#define PIN_I2S2_MCK				GPIO_Pin_6
#define PINSOURCE_I2S2_WS		GPIO_PinSource12
#define PINSOURCE_I2S2_SCK		GPIO_PinSource13
#define PINSOURCE_I2S2_SD		GPIO_PinSource15
#define PINSOURCE_I2S2_MCK		GPIO_PinSource6


#define PORT_I2S3_WS				GPIOA
#define PORT_I2S3_SCK			GPIOC
#define PORT_I2S3_SD				GPIOC
#define PORT_I2S3_MCK			GPIOC
#define PIN_I2S3_WS				GPIO_Pin_4
#define PIN_I2S3_SCK				GPIO_Pin_10
#define PIN_I2S3_SD				GPIO_Pin_12
#define PIN_I2S3_MCK				GPIO_Pin_7
#define PINSOURCE_I2S3_WS		GPIO_PinSource4
#define PINSOURCE_I2S3_SCK		GPIO_PinSource10
#define PINSOURCE_I2S3_SD		GPIO_PinSource12
#define PINSOURCE_I2S3_MCK		GPIO_PinSource7



#define PORT_L3DATA		GPIOE
#define PIN_L3DATA		GPIO_Pin_13
#define PORT_L3CLK		GPIOE
#define PIN_L3CLK			GPIO_Pin_14
#define PORT_L3MODE		GPIOE
#define PIN_L3MODE		GPIO_Pin_15

//////////////////////////////////////////////////////////


#define PORT_LED3PDB		GPIOF
#define PIN_LED3PDB		GPIO_Pin_15

#define PORT_LED0DB		GPIOG
#define PIN_LED0DB		GPIO_Pin_0

#define PORT_LED3DB		GPIOG
#define PIN_LED3DB		GPIO_Pin_1

#define PORT_LED6DB		GPIOE
#define PIN_LED6DB		GPIO_Pin_7

#define PORT_LED9DB		GPIOE
#define PIN_LED9DB		GPIO_Pin_8


/////////////////////////////////////////////////////////


#define PORT_ADDR1		GPIOC
#define PIN_ADDR1		GPIO_Pin_11

#define PORT_ADDR2		GPIOD
#define PIN_ADDR2		GPIO_Pin_1

#define PORT_ADDR3		GPIOD
#define PIN_ADDR3		GPIO_Pin_0

#define PORT_ADDR4		GPIOD
#define PIN_ADDR4		GPIO_Pin_2


////////////////////////////////////////////////////////


#define PORT_KEY_UP			GPIOE
#define PIN_KEY_UP			GPIO_Pin_11

#define PORT_KEY_DOWN		GPIOE
#define PIN_KEY_DOWN		GPIO_Pin_9


////////////////////////////////////////////////////////


#define PORT_SHORT		GPIOF
#define PIN_SHORT		GPIO_Pin_11

#define PORT_OVP			GPIOF
#define PIN_OVP			GPIO_Pin_12

#define PORT_UVP			GPIOF
#define PIN_UVP			GPIO_Pin_13


////////////////////////////////////////////////////////


#define PORT_DC24V1		GPIOD
#define PIN_DC24V1		GPIO_Pin_4

#define PORT_DC24V2		GPIOD
#define PIN_DC24V2		GPIO_Pin_3


////////////////////////////////////////////////////////


#define PORT_EME		GPIOF
#define PIN_EME		GPIO_Pin_14


////////////////////////////////////////////////////////



#define PORT_NOISE1_DETECT		GPIOA
#define PIN_NOISE1_DETECT		GPIO_Pin_5

#define PORT_NOISE2_DETECT		GPIOA
#define PIN_NOISE2_DETECT		GPIO_Pin_3


////////////////////////////////////////////////////////


#define PORT_FEEDDOG		GPIOE
#define PIN_FEEDDOG		GPIO_Pin_2


////////////////////////////////////////////////////////






void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);


void LED1Flash(void);
void LED1On(void);
void LED1Off(void);

void LED2Flash(void);
void LED2On(void);
void LED2Off(void);



void WriteL3Data (int bitvalue);
void WriteL3Clock (int bitvalue);
void WriteL3Mode (int bitvalue);





void LED3pdbOn(void);
void LED3pdbOff(void);

void LED0dbOn(void);
void LED0dbOff(void);

void LED3dbOn(void);
void LED3dbOff(void);

void LED6dbOn(void);
void LED6dbOff(void);

void LED9dbOn(void);
void LED9dbOff(void);



u8 GetAddress(void);


int GetKeyUp(void);
int GetKeyDown(void);




int GetShort(void);
int GetOvp(void);
int GetUvp(void);



int GetDC24V1(void);
int GetDC24V2(void);


int GetEme(void);


int GetNoise1Detect(void);
int GetNoise2Detect(void);


void FeedDog(void);

void Wwdg_Init(void);
void Wwdg_Feed(void);




#endif
