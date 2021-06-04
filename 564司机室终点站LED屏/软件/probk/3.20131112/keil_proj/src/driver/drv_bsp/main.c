/**
  ******************************************************************************
  * 
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/


#include "MyAll.h"



u32 Debuger;



/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f2xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
        system_stm32f2xx.c file
     */

//	SystemInit()


	Delay(1000);
	Delay(1000);

	RCC_Configuration();

	Delay(1000);

	GPIO_Configuration();

	Delay(1000);

	LED1On();
	LED2On();

	Delay(500000);
	Delay(500000);
	Delay(500000);
	Delay(500000);

	SysTick_Init();
	Delay(100000);

	


	LED1Off();
	LED2Off();
	Delay(100000);	
	LED1On();


	Delay(10000);
	
	AppInit();

	StorageInit();

	NetInit();

	Delay(1000);

	AudioInit();

	Delay(10000);
	Delay(10000);
	Delay(10000);
	StorageInfoRead();

	Delay(10000);
	

	while(1)
	{

		AppProcess();

		LwipProcess();

		NetProcess();

		AudioProcess();
		

		if(MyTmCnt10 >= 1)
		{
			MyTmCnt10 = 0;
			MyTimer10();
		}

		if(MyTmCnt20 >= 2)
		{
			MyTmCnt20 = 2;
			MyTimer20();
		}
		
		if(MyTmCnt50 >= 5)
		{
			MyTmCnt50 = 0;
			MyTimer50();
		}

		if(MyTmCnt100 >= 10)
		{
			MyTmCnt100 = 0;
			MyTimer100();
		}

		if(MyTmCnt500 >= 50)
		{
			MyTmCnt500 = 0;
			MyTimer500();
		}

		if(MyTmCnt1000 >= 100)
		{
			MyTmCnt1000 = 0;
			MyTimer1000();
		}

		if(MyTmCnt2000 >= 200)
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
void assert_failed(uint8_t* file, uint32_t line)
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
