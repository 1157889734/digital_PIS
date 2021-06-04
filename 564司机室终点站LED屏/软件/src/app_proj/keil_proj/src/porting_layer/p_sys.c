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

//记忆定时器周期，根据定时器中断的间隔设定
static uint16 timer0_ms = N_MS;

//系统时钟，在定时器中断中增加 timer0_ms
static uint32 timer0_clock = 0;

/*********************************************************************
函数作用:	定时器中断调用hook
				供下层驱动的中断服务程序调用
参数说明: 	--
全局变量：--
调用函数：--
注意事项:	本例按照宏定义方式读取
返回说明:	--
**********************************************************************/
void timer_isr_hook(void)
{
	static u8	tmp = 1;
	static u8  tmp2 = 1;
	if (tmp) {		//初次调用进行一次初始化
		timer0_clock = 0;
		timer0_ms = N_MS; //10ms	
		tmp = 0;
	}
	else {
		timer0_clock += timer0_ms;//系统时钟增加定时器的周期数
	}
	tmp2++;

	if(tmp2 >=roll_speed)
	{
		tmp2 = 0;
		timer2_flag++;
	}
	
}

//***********************************************************************************************************************
//函数作用:检查应用层是否时间到了
//参数说明:timer---应用层定义的计数器	interval---运行间隔
//注意事项:
//返回说明:无
//***********************************************************************************************************************
uint8 timer0_check_timer(uint32 *timer, uint32 interval)
{
	if (interval) {
		//判断溢出
		if (timer0_clock < (*timer)) {
			if (((timer0_clock) + (0xFFFFFFFF - *timer)) >= interval) {
				*timer = timer0_clock;
				return 1;
			}
		}
		else {
			//比较是否时间到
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

