//*****************************************************//

//*****************************************************//

#include "MyAll.h"  



int MyTmCnt10;
int MyTmCnt20;
int MyTmCnt50;
int MyTmCnt100;
int MyTmCnt500;
int MyTmCnt1000;
int MyTmCnt2000;



void SysTick_Init(void)
{
	RCC_ClocksTypeDef RCC_Clocks;

	 /* Configure Systick clock source as HCLK */
  	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);

 	 /* SystTick configuration: an interrupt every 10ms */
  	RCC_GetClocksFreq(&RCC_Clocks);
  	SysTick_Config(RCC_Clocks.HCLK_Frequency / 10000);

  	
}


void SysTick_Isr(void)
{

	MyTmCnt10++;
	MyTmCnt20++;
	MyTmCnt50++;
	MyTmCnt100++;
	MyTmCnt500++;
	MyTmCnt1000++;
	MyTmCnt2000++;

	timer_isr_hook();

}


void Timer1_Init(void)  
{

	
}


void Timer1_Isr(void)
{
	
	

}


void MyTimer10(void)
{
	



}

void MyTimer20(void)
{
	
	

}

void MyTimer50(void)
{

	Net50ms();

	Lwip50ms();


}

void MyTimer100(void)
{

	//App100ms();

	//Audio100ms();


	//if(AudioPlayInfo.EnableFlag > 0)
	//{
	//	LED1Flash();

	//}

	StorageInfoSaveProcess();	


}

void MyTimer500(void)
{

	if(AudioPlayInfo.EnableFlag < 1)
	{
		LED1Flash();

	}


	Net500ms();


}


void MyTimer1000(void)
{

	App1000ms();
	

}


void MyTimer2000(void)
{

	

}






 
