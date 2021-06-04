/**
  ******************************************************************************
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/


#include "includes.h"
u32 Debuger;
u16 test;



#ifdef TEST_CODE

DEV_STATUS_T	 device_status_tmp;
void refresh_dev_status(void)
{
	static uint32	test_timer = 0;
	uint8  tmp;

	if (timer0_check_timer(&test_timer, 8000))
	{

		/**/
		if (device_status_tmp.up_down_flag)   //xia
		{

			if (device_status_tmp.status == ARRIVE_STATUS)
			{
				device_status_tmp.status = DEPARTURE_STATUS;
				device_status_tmp.door_side = DOOR_STATUS_INVALID;

				if ((device_status_tmp.next_station_num == device_status_tmp.end_station_num) || (device_status_tmp.cur_station_num == device_status_tmp.end_station_num))
				{

					if (device_status_tmp.up_down_flag)
					{
						device_status_tmp.up_down_flag = TRAIN_UP;

						tmp = device_status_tmp.start_station_num;
						device_status_tmp.start_station_num = device_status_tmp.end_station_num;
						device_status_tmp.end_station_num = tmp;

						device_status_tmp.cur_station_num = device_status_tmp.start_station_num;
						device_status_tmp.next_station_num = device_status_tmp.start_station_num + 1;
					}
					else
					{
						device_status_tmp.up_down_flag = TRAIN_DOWN;
						tmp = device_status_tmp.start_station_num;
						device_status_tmp.start_station_num = device_status_tmp.end_station_num;
						device_status_tmp.end_station_num = tmp;

						device_status_tmp.cur_station_num = device_status_tmp.start_station_num;
						device_status_tmp.next_station_num = device_status_tmp.start_station_num - 1;
					}
				}
				else
				{

					device_status_tmp.cur_station_num = device_status_tmp.next_station_num;
					device_status_tmp.next_station_num -= 1;
					//if((device_status_tmp.next_station_num == 5) || (device_status_tmp.next_station_num == 13)) {
					//	device_status_tmp.next_station_num -=2;

					//}
				}

			}
			else if (device_status_tmp.status == DEPARTURE_STATUS)
			{

				device_status_tmp.status = ARRIVE_STATUS;

				if (device_status_tmp.cur_station_num % 2)
				{
					device_status_tmp.door_side = THIS_DOOR;
				}
				else
				{
					device_status_tmp.door_side = OPPOSITE_DOOR;
				}

			}
			else if (dev_info.dev_status.status == SWITCH_UP_DOWN_STATUS)
			{
				device_status_tmp.status = DEPARTURE_STATUS;
			}
			else
			{
				;
			}
		}
		else  	//shang
		{

			if (device_status_tmp.status == ARRIVE_STATUS)
			{

				device_status_tmp.door_side = DOOR_STATUS_INVALID;
				device_status_tmp.status = DEPARTURE_STATUS;

				if ((device_status_tmp.next_station_num == device_status_tmp.end_station_num) || (device_status_tmp.cur_station_num == device_status_tmp.end_station_num))
				{


					if (device_status_tmp.up_down_flag)
					{
						device_status_tmp.up_down_flag = TRAIN_UP;

						tmp = device_status_tmp.start_station_num;
						device_status_tmp.start_station_num = device_status_tmp.end_station_num;
						device_status_tmp.end_station_num = tmp;

						device_status_tmp.cur_station_num = device_status_tmp.start_station_num;
						device_status_tmp.next_station_num = device_status_tmp.start_station_num + 1;
					}
					else
					{
						device_status_tmp.up_down_flag = TRAIN_DOWN;
						tmp = device_status_tmp.start_station_num;
						device_status_tmp.start_station_num = device_status_tmp.end_station_num;
						device_status_tmp.end_station_num = tmp;

						device_status_tmp.cur_station_num = device_status_tmp.start_station_num;
						device_status_tmp.next_station_num = device_status_tmp.start_station_num - 1;
					}
				}
				else
				{

					device_status_tmp.cur_station_num = device_status_tmp.next_station_num;
					device_status_tmp.next_station_num += 1;
					//if((device_status_tmp.next_station_num == 4) || (device_status_tmp.next_station_num == 13)) {
					//	device_status_tmp.next_station_num +=2;

					//}

				}

			}
			else if (device_status_tmp.status == DEPARTURE_STATUS)
			{
				device_status_tmp.status = ARRIVE_STATUS;

				if (device_status_tmp.cur_station_num % 2)
				{
					device_status_tmp.door_side = THIS_DOOR;
				}
				else
				{
					device_status_tmp.door_side = OPPOSITE_DOOR;
				}
			}
			else if (dev_info.dev_status.status == SWITCH_UP_DOWN_STATUS)
			{
				device_status_tmp.status = DEPARTURE_STATUS;
			}
			else
			{
				;
			}
		}

		/**/
		//serial_send_data(0x03, (uint8 *)&device_status_tmp, sizeof(DEVICE_STATUS_T));
	}
}

void get_device_status(void)
{
	static uint8 tmp = 1;
	uint8 down_flag_tmp;
	static uint32	timer_1 = 0;



	if (tmp)
	{
		tmp = 0;
		device_status_tmp.door_side = THIS_DOOR;
		device_status_tmp.brightness = BRIGHTNESS_VALUE;
		device_status_tmp.status = DEPARTURE_STATUS;
		device_status_tmp.up_down_flag = TRAIN_UP;
		device_status_tmp.start_station_num = 1;
		device_status_tmp.end_station_num = 5;
		device_status_tmp.cur_station_num = device_status_tmp.start_station_num;
		device_status_tmp.next_station_num = device_status_tmp.cur_station_num + 1;

		mem_cpy((uint8 *)&dev_info.dev_status, (uint8 *)&device_status_tmp, sizeof(DEV_STATUS_T));
	}
	else
	{
		//return;

		refresh_dev_status();




		down_flag_tmp = dev_info.dev_status.up_down_flag;
		mem_cpy((uint8 *)&dev_info.dev_status, (uint8 *)&device_status_tmp, sizeof(DEV_STATUS_T));

		if ((!device_status_tmp.up_down_flag) && down_flag_tmp)
		{
			dev_info.dev_status.up_down_flag = TRAIN_UP;
			dev_info.dev_status.status = SWITCH_UP_DOWN_STATUS;
			device_status_tmp.status = dev_info.dev_status.status;

		}
		else if ((device_status_tmp.up_down_flag) && (!down_flag_tmp))
		{
			dev_info.dev_status.up_down_flag = TRAIN_DOWN;
			dev_info.dev_status.status = SWITCH_UP_DOWN_STATUS;
			device_status_tmp.status = dev_info.dev_status.status;

		}
		else
		{
			;
		}
	}

	if (timer0_check_timer(&timer_1, 5000))
	{
		;//LED1Flash();
		device_status_tmp.brightness += 30;

		if (device_status_tmp.brightness > 100)
		{ device_status_tmp.brightness = 0; }

		mem_cpy((uint8 *)&dev_info.dev_status, (uint8 *)&device_status_tmp, sizeof(DEV_STATUS_T));
		adjust_brightness();

	}

}

#endif

#if 0
// test 应用程序与iap程序交替时是否有全局变量初值错乱的问题
//测试结果: 不会错乱
uint16 test_val_1 = 100;
uint16 test_val_2 = 200;
uint16 test_val_3 = 300;
uint16 test_val_4 = 400;

void test_val_pro(void)
{
	static uint8 flag1 = 1;
	static uint8 flag2 = 0;
	static uint8 flag3 = 1;
	char p_buf[100];
	static uint32 timer_1 = 0, timer_2 = 0;

	if (flag1)
	{
		flag1 = 0;
		timer_1 = get_timer0_clock();
		flag2 = 1;
	}

	if (flag2)
	{

		if (flag3)
		{
			if (timer0_check_timer(&timer_1, TIMING_5sec))
			{
				flag3 = 0;
				sprintf(p_buf, "trig: test_val_1[%d], test_val_2[%d], test_val_3[%d], test_val_4[%d]\n",
				        test_val_1, test_val_2, test_val_3, test_val_4
				       );
				UdpTestSend(p_buf, strlen(p_buf));
			}
		}
		else
		{
			test_val_1++;
			test_val_2++;
			test_val_3++;
			test_val_4++;

			if (timer0_check_timer(&timer_2, TIMING_2sec))
			{
				flag3 = 0;
				sprintf(p_buf, "runing: test_val_1[%d], test_val_2[%d], test_val_3[%d], test_val_4[%d]\n",
				        test_val_1, test_val_2, test_val_3, test_val_4
				       );
				UdpTestSend(p_buf, strlen(p_buf));
			}

		}


	}

}
#endif

__IO uint32_t TimingDelay = 0;
__IO uint32_t LsiFreq = 0;
__IO uint32_t CaptureNumber = 0, PeriodValue = 0;

uint32_t GetLSIFrequency(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	RCC_ClocksTypeDef  RCC_ClockFreq;

	/* Enable the LSI oscillator ************************************************/
	RCC_LSICmd(ENABLE);

	/* Wait till LSI is ready */

	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
	{}

	/* TIM5 configuration *******************************************************/
	/* Enable TIM5 clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

	/* Connect internally the TIM5_CH4 Input Capture to the LSI clock output */
	TIM_RemapConfig(TIM5, TIM5_LSI);

	/* Configure TIM5 presclaer */
	TIM_PrescalerConfig(TIM5, 0, TIM_PSCReloadMode_Immediate);

	/* TIM5 configuration: Input Capture mode ---------------------
	   The LSI oscillator is connected to TIM5 CH4
	   The Rising edge is used as active edge,
	   The TIM5 CCR4 is used to compute the frequency value
	------------------------------------------------------------ */
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV8;
	TIM_ICInitStructure.TIM_ICFilter = 0;
	TIM_ICInit(TIM5, &TIM_ICInitStructure);

	/* Enable TIM5 Interrupt channel */
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable TIM5 counter */
	TIM_Cmd(TIM5, ENABLE);

	/* Reset the flags */
	TIM5->SR = 0;

	/* Enable the CC4 Interrupt Request */
	TIM_ITConfig(TIM5, TIM_IT_CC4, ENABLE);


	/* Wait until the TIM5 get 2 LSI edges (refer to TIM5_IRQHandler() in
	  stm32f2xx_it.c file) ******************************************************/

	while (CaptureNumber != 2)
	{

	}

	/* Deinitialize the TIM5 peripheral registers to their default reset values */
	TIM_DeInit(TIM5);


	/* Compute the LSI frequency, depending on TIM5 input clock frequency (PCLK1)*/
	/* Get SYSCLK, HCLK and PCLKx frequency */
	RCC_GetClocksFreq(&RCC_ClockFreq);

	/* Get PCLK1 prescaler */

	if ((RCC->CFGR & RCC_CFGR_PPRE1) == 0)
	{
		/* PCLK1 prescaler equal to 1 => TIMCLK = PCLK1 */
		return ((RCC_ClockFreq.PCLK1_Frequency / PeriodValue) * 8);

	}
	else
	{
		/* PCLK1 prescaler different from 1 => TIMCLK = 2 * PCLK1 */
		return (((2 * RCC_ClockFreq.PCLK1_Frequency) / PeriodValue) * 8) ;

	}

}


void IWDG_Configuration(void)
{

	LsiFreq = GetLSIFrequency();

#if 1
	/*

	写入

	0x5555,

	用于允许狗狗寄存器写入功能

	*/

	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

	/*

	狗狗时钟分频

	,32K/32=1KHZ(1ms)
	*/

	IWDG_SetPrescaler(IWDG_Prescaler_32);



	/* Set counter reload value to obtain 2000ms IWDG TimeOut.
	   Counter Reload Value = 2000ms/IWDG counter clock period

							= LsiFreq/16
	 */
	IWDG_SetReload(LsiFreq / 16);


	IWDG_ReloadCounter();

	IWDG_Enable();
#if 0
	{
		uint8 b1 = 1, b2 = 1;

		while (b1 || b2)
		{
			b1 = IWDG_GetFlagStatus(IWDG_FLAG_PVU);
			b2 = IWDG_GetFlagStatus(IWDG_FLAG_RVU);
		}

	}
#endif


#endif

}


#define REBOOT_CNT	150
uint16 reboot_cnt = REBOOT_CNT;

void reset_reboot_cnt(void)
{
	reboot_cnt = REBOOT_CNT;
}

void reboot_process(void)
{
	static uint32 timer = 0;

	if (timer0_check_timer(&timer, TIMING_100ms))
	{
		if (reboot_cnt)
		{
			reboot_cnt--;

			if (!reboot_cnt)
			{
				turn_off_screen_clrbuf();
				while (1)
				{
					;
				}
			}
		}
	}

}


uint8 timer_flag = 0, timer3_flag;
volatile uint8 refresh_flag = 0;
volatile uint8 timer2_flag = 0;

#define BLANK_COUNTER		1

int main(void)
{
//	uint16 i,j;
	//static uint16 timer_tmp = 0;
	/*!< At this stage the microcontroller clock setting is already configured,
	     this is done through SystemInit() function which is called from startup
	     file (startup_stm32f2xx.s) before to branch to application main.
	     To reconfigure the default setting of SystemInit() function, refer to
	      system_stm32f2xx.c file
	   */

	static uint32	timer_1, timer_2, timer_3;
	timer_1 = timer_2 = timer_3 = 0;

	//ram_feeddog();
	//ram_flash_copy(100);
#if 0
#ifdef IAP_PROG
	Check_And_RunAPP();
#else
	App_SetVectorTable();
#endif
#endif

	Delay(1000);

	RCC_Configuration();

	Delay(1000);

	GPIO_Configuration();
	SysTick_Init();

	feed_dog();
	feed_dog();

	LED1Off();
	LED2Off();

	feed_dog();
	feed_dog();

	AppInit();

	StorageInit();

	NetInit();

#if 0
#ifdef IAP_PROG
	IAP_tftpd_init();
#endif
#endif

	Delay(1000);

	feed_dog();
	feed_dog();

	//AudioInit();

	//Delay(10000);
	//Delay(10000);
	//Delay(10000);
	StorageInfoRead();

	Delay(1000);

	feed_dog();
	feed_dog();

//--------------test-------------------

#if DMP_DRIVER
	device_init();

#ifdef TEST_MODE
	set_hc595_buf(0x00);
	Refresh_74HC595_status(hc595_info.green_led, hc595_info.red_led, hc595_info.hc595_config.buf_size);
#endif

#endif

//------------------------------------

	IWDG_Configuration();

	ledscreen_init();

	while (1)
	{

		AppProcess();

		LwipProcess();

		reboot_process();

		//NetProcess();

		//AudioProcess();


//-------------- app process


		if (proc_downloading_flag)
		{
			if (timer0_check_timer(&timer_2, TIMING_100ms))
			{
				feed_dog();
				LED1Flash();
			}

			turn_off_screen_clrbuf();
#if 0
			Hled_run_status = Run_STATUS_FAULT;

			if (timer0_check_timer(&timer_1, TIMING_500ms))
			{
				udp_CtrlMsg_send_process(Ctrl_KEYWORD_ProcessData);
			}

			if (timer0_check_timer(&timer_3, TIMING_5sec))
			{
				send_downloadReq();
			}

#endif

			tftp_cli_timeout_process();
		}
		else
		{
			if (timer0_check_timer(&timer_2, TIMING_100ms))
			{
				feed_dog();
			}

			if (timer0_check_timer(&timer_1, TIMING_500ms))
			{
				udp_CtrlMsg_send_process(Ctrl_KEYWORD_ProcessData);
				LED1Flash();
			}

			if (timer0_check_timer(&timer_3, TIMING_5sec))
			{
				send_AppInfo();
			}

			LedScreen_process();

			//check_iap_req_process();


		}

#if 0

		if (timer0_check_timer(&timer_1, 500))
		{
			udp_CtrlMsg_send_process(Ctrl_KEYWORD_ProcessData);
			LED2Flash();

			if (dmp_run_status)
			{
				dmp_run_status = 0;
			}

		}

#ifndef TEST_MODE
		LED_status_process();
#endif
#endif

//-------------------





		if (MyTmCnt10 >= TIMING_10ms)
		{
			MyTmCnt10 = 0;
			MyTimer10();
		}

		if (MyTmCnt20 >= TIMING_20ms)
		{
			MyTmCnt20 = 2;
			MyTimer20();
		}

		if (MyTmCnt50 >= TIMING_50ms)
		{
			MyTmCnt50 = 0;
			MyTimer50();
		}

		if (MyTmCnt100 >= TIMING_100ms)
		{
			MyTmCnt100 = 0;
			MyTimer100();
		}

		if (MyTmCnt500 >= TIMING_500ms)
		{
			MyTmCnt500 = 0;
			//MyTimer500();
		}

		if (MyTmCnt1000 >= TIMING_1sec)
		{
			MyTmCnt1000 = 0;
			MyTimer1000();
		}

		if (MyTmCnt2000 >= TIMING_2sec)
		{
			MyTmCnt2000 = 0;
			MyTimer2000();
		}


	}




}







#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	   ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1)
	{


	}
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

