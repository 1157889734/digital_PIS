#include "includes.h"


void feed_dog(void)
{
	;
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


