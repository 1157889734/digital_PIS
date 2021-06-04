#include "includes.h"


#define GPIO_PORT_WDT_IN	GPIOE
#define GPIO_Pin_WDT_IN	GPIO_Pin_2

void watchdog_GPIO_config(void)
{	
	GPIO_InitTypeDef  GPIO_InitStructure;


	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
 	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_WDT_IN;
	GPIO_Init(GPIO_PORT_WDT_IN, &GPIO_InitStructure);

	GPIO_SetBits(GPIO_PORT_WDT_IN, GPIO_Pin_WDT_IN);

	GPIO_ResetBits(GPIO_PORT_WDT_IN, GPIO_Pin_WDT_IN);


}

void feed_dog(void)
{
	GPIO_WriteBit(GPIO_PORT_WDT_IN, GPIO_Pin_WDT_IN, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIO_PORT_WDT_IN, GPIO_Pin_WDT_IN)));
}
/*
void init_mcu(void)
{
	;
}

void delay_ms(unsigned int n_ms)
{
	;
}
*/

//���䶨ʱ�����ڣ����ݶ�ʱ���жϵļ���趨
static uint16 timer0_ms = N_MS;

//ϵͳʱ�ӣ��ڶ�ʱ���ж������� timer0_ms
static uint32 timer0_clock = 0;

/*********************************************************************
��������:	��ʱ���жϵ���hook
				���²��������жϷ���������
����˵��: 	--
ȫ�ֱ�����--
���ú�����--
ע������:	�������պ궨�巽ʽ��ȡ
����˵��:	--
**********************************************************************/
void timer_isr_hook(void)
{
	static u8	tmp = 1;
	static u8  tmp2 = 1;
	if (tmp) {		//���ε��ý���һ�γ�ʼ��
		timer0_clock = 0;
		timer0_ms = N_MS; //10ms	
		tmp = 0;
	}
	else {
		timer0_clock += timer0_ms;//ϵͳʱ�����Ӷ�ʱ����������
	}
	tmp2++;

	if(tmp2 >=roll_speed)
	{
		tmp2 = 0;
		timer2_flag++;
	}
	
}

//***********************************************************************************************************************
//��������:���Ӧ�ò��Ƿ�ʱ�䵽��
//����˵��:timer---Ӧ�ò㶨��ļ�����	interval---���м��
//ע������:
//����˵��:��
//***********************************************************************************************************************
uint8 timer0_check_timer(uint32 *timer, uint32 interval)
{
	if (interval) {
		//�ж����
		if (timer0_clock < (*timer)) {
			if (((timer0_clock) + (0xFFFFFFFF - *timer)) >= interval) {
				*timer = timer0_clock;
				return 1;
			}
		}
		else {
			//�Ƚ��Ƿ�ʱ�䵽
			if (timer0_clock-(*timer)>=interval) {
				*timer=timer0_clock;
				return 1;
			}
		}
	}
	return 0;
}

uint32 get_timer0_clock(void)
{
	uint32 timer0_clock_tmp;

	timer0_clock_tmp = timer0_clock;

	return timer0_clock_tmp;
}

