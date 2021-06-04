#include "includes.h"

#define	PORT_DEV_ID	GPIOG
#define	PORT_DEV_ADD_A	PORT_DEV_ID
#define	PIN_DEV_ADD_A		GPIO_Pin_0
#define	PORT_DEV_ADD_B	PORT_DEV_ID
#define	PIN_DEV_ADD_B		GPIO_Pin_1
#define	PORT_DEV_ADD_C	PORT_DEV_ID
#define	PIN_DEV_ADD_C		GPIO_Pin_2
#define	PORT_DEV_ADD_D	PORT_DEV_ID
#define	PIN_DEV_ADD_D		GPIO_Pin_3
#define	PORT_DEV_ADD_E	PORT_DEV_ID
#define	PIN_DEV_ADD_E		GPIO_Pin_4
#define	PORT_DEV_ADD_F	PORT_DEV_ID
#define	PIN_DEV_ADD_F		GPIO_Pin_5
#define	PORT_DEV_ADD_G	PORT_DEV_ID
#define	PIN_DEV_ADD_G		GPIO_Pin_6
#define	PORT_DEV_ADD_H	PORT_DEV_ID
#define	PIN_DEV_ADD_H		GPIO_Pin_7



void Device_id_GPIO_config(void)
{	
	GPIO_InitTypeDef  GPIO_InitStructure;


  	/* input mode */
  	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
 	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_InitStructure.GPIO_Pin = PIN_DEV_ADD_A | PIN_DEV_ADD_B | PIN_DEV_ADD_C | PIN_DEV_ADD_D | PIN_DEV_ADD_E | PIN_DEV_ADD_F | PIN_DEV_ADD_G | PIN_DEV_ADD_H;
  	GPIO_Init(PORT_DEV_ID, &GPIO_InitStructure);
}

/*********************************************************************
函数作用:	设备获取设备ID
参数说明: 	--
全局变量：--
调用函数：
注意事项:	拨码键按照 bcd码设计,最大为99
				范围为 00 -- 99
返回说明:	0xff : 错误. 十位 或 个位超过9 
				正常设备id 与 数码管显示一致
**********************************************************************/
uint8 get_devecie_id(void)
{
	uint32	time_1 = 0;
	uint16	dev_id_tmp1 = 0xffff;
	uint16	dev_id_tmp2 = 0x0000;
	uint8	compare_count = 0;
	uint8	ret;
	
	while (1) {
		while (!timer0_check_timer(&time_1, 100))
			feed_dog();
		dev_id_tmp1 = GPIO_ReadInputData(PORT_DEV_ID);
		if(dev_id_tmp2 == dev_id_tmp1) {
			compare_count++;
			if(compare_count >= 3) {
				break;
			}
		}
		else {
			compare_count = 0;
			dev_id_tmp2 = dev_id_tmp1;
		}
	}

	dev_id_tmp2 &= 0x00ff;

	dev_id_tmp1 = dev_id_tmp2;

	if (((dev_id_tmp1 & 0x000f) > 9) || ((dev_id_tmp1 >> 4) > 9)) {
		ret = 0xff;
		report_error(ERR46);
	}
	else {
		ret = (uint8)((dev_id_tmp1 >> 4) + (dev_id_tmp1 & 0x000f) * 10);
		if ((ret < Firstdmp_ID) || ((ret + 1 - Firstdmp_ID) > IP_resSUM)) {
			report_error(ERR47);
			ret = 0xff;
		}
		else {
			;
			//ret += 100;
		}
		
		
	}

	return ret;
	
}
