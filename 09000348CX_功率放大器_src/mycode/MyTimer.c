//*****************************************************//

//*****************************************************//

#include "MyAll.h"  



int MyTmCnt10;
int MyTmCnt20;
int MyTmCnt50;
int MyTmCnt100;
int MyTmCnt200;
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
  	SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);  

  	
}


void SysTick_Isr(void)
{

	MyTmCnt10++;
	MyTmCnt20++;
	MyTmCnt50++;
	MyTmCnt100++;
	MyTmCnt200++;
	MyTmCnt500++;
	MyTmCnt1000++;
	MyTmCnt2000++;

	

}


void TimerOff(void)
{

	SysTick->CTRL  =   ~(SysTick_CTRL_CLKSOURCE_Msk | 
                   			SysTick_CTRL_TICKINT_Msk   | 
                   			SysTick_CTRL_ENABLE_Msk);     
	

}


void Timer1_Init(void)  
{

	
}


void Timer1_Isr(void)
{
	
	

}


void MyTimer10(void)
{
	
//	Key10ms();

	Noise10ms();

	Audio10ms();

	App10ms();



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

	App100ms();

	Audio100ms();


	if(AudioPlayInfo.EnableFlag > 0)
	{
		LED1Flash();
		LED2Flash();

	}

	StorageInfoSaveProcess();	


//	Key100ms();

	SelfTest100ms();

	Noise100ms();

	TftpClient100ms();


}


void MyTimer200(void)
{
	Audio200ms();


}


void MyTimer500(void)
{

	if(AudioPlayInfo.EnableFlag < 1)
	{
		LED1Flash();
		LED2Flash();

	}


	Net500ms();

	Audio500ms();

	Commander500ms();

	SelfTest500ms();

    if ((!NetOkFlag) && (NetHad)) {
        reboot_flag ++;
        if (reboot_flag > 30) {
            while(1) {
                Delay(1000);
            }
        }
    }
}


void MyTimer1000(void)
{

	App1000ms();
	

}


void MyTimer2000(void)
{

	

}






 
