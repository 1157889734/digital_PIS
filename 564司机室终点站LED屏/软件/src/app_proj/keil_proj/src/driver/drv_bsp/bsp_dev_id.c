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
��������:	�����豸����״̬�������豸id
����˵��: 	uint16 pinLevel, ���ߵ�ƽ״̬
ȫ�ֱ�����--
���ú�����
ע������:	
	addr7 6 Ϊ 1 1 ʱ�����շ���һȷ���豸id
				��������� bcd�����,���Ϊ99
				��ΧΪ 00 -- 99
				�����豸id �� �������ʾһ��
	addr7 6 ��Ϊ 1 1 ʱ�����շ�����ȷ���豸id
				����addr5 ~ 0 ��״̬ +1 ȷ���豸id
				��ΧΪ 1 -- 64
����˵��:	0xff : ����. ʮλ �� ��λ����9 
				
**********************************************************************/
uint8 get_devid_call(uint16 pinLevel)
{
	uint16 dev_id_tmp1;
	uint8	ret;
	
	pinLevel &= 0x00ff;		// ֻȡ�ڰ�λ
	dev_id_tmp1 = pinLevel;

	switch (pinLevel & 0x00c0) {
	case 0x00c0:
		ret = (dev_id_tmp1&0x003f) + 1;	// ȡ�� 6λ�����豸id
		break;
		
	default:
		if (((dev_id_tmp1 & 0x000f) > 9) || ((dev_id_tmp1 >> 4) > 9)) {
			ret = 0xff;
			report_error(ERR46);
		}
		else {
			ret = (uint8)((dev_id_tmp1 >> 4)* 10 + (dev_id_tmp1 & 0x000f) );
			if ((ret < FirstHLED_ID) || ((ret + 1 - FirstHLED_ID) > IP_resSUM)) {
				report_error(ERR47);
				ret = 0xff;
			}
		}
		break;
	}

	return ret;
}

/*********************************************************************
��������:	�豸��ȡ�豸ID
����˵��: 	--
ȫ�ֱ�����--
���ú�����uint8 get_devid_call(uint16 pinLevel)
ע������:	
		
����˵��:
**********************************************************************/
uint8 get_devecie_id(void)
{
	uint32	time_1 = 0;
	uint16	dev_id_tmp1 = 0xffff;
	uint16	dev_id_tmp2 = 0x0000;
	uint8	compare_count = 0;
	uint8	ret;
	
	while (1) {
		while (!timer0_check_timer(&time_1, TIMING_100ms))
			feed_dog();
		dev_id_tmp1 = GPIO_ReadInputData(PORT_DEV_ID);
		if(dev_id_tmp2 == dev_id_tmp1) {
			compare_count++;
			if(compare_count >= 5) {
				break;
			}
		}
		else {
			compare_count = 0;
			dev_id_tmp2 = dev_id_tmp1;
		}
	}

	dev_id_tmp2 &= 0x00ff;

	ret = get_devid_call(dev_id_tmp2);
	
	return ret;
	
}

uint8 check_devid(void)
{
	if ((DevId < FirstHLED_ID) || ((DevId + 1 - FirstHLED_ID) > IP_resSUM)) {
		report_error(ERR47);
		return 1;
	}
	else
		return 0;
}

