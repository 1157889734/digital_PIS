//*****************************************************//

//*****************************************************//

#ifndef _MYBOARD_H
#define  _MYBOARD_H

#include "MyAll.h"




#define PORT_LED1		GPIOF
#define PIN_LED1			GPIO_Pin_2

#define PORT_LED2		GPIOF
#define PIN_LED2			GPIO_Pin_3


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
#define PIN_L3MODE		GPIO_Pin_4





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






#endif
