
//*****************************************************//
//		MyAdc.c
//*****************************************************//


#include "MyAll.h"



#define ADC3_DR_ADDRESS    ((uint32_t)0x4001224C)



__IO uint16_t AdcConvertedValue[256];




void AdcInit(void)
{

	ADC_InitTypeDef       ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	DMA_InitTypeDef       DMA_InitStructure;
	GPIO_InitTypeDef      GPIO_InitStructure;

	/* Enable ADC3, DMA2 and GPIO clocks ****************************************/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

	/* DMA2 Stream0 channel2 configuration **************************************/
	DMA_InitStructure.DMA_Channel = DMA_Channel_2;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC3_DR_ADDRESS;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&AdcConvertedValue;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = 256;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream0, ENABLE);


	/* Configure ADC3 Channel9 pin as analog input ******************************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	/* Configure ADC3 Channel14 pin as analog input ******************************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	
	

	/* ADC Common Init **********************************************************/
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div8;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	/* ADC3 Init ****************************************************************/
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 2;
	ADC_Init(ADC3, &ADC_InitStructure);

	/* ADC3 regular channel7 configuration *************************************/
	ADC_RegularChannelConfig(ADC3, ADC_Channel_9, 1, ADC_SampleTime_15Cycles);

	ADC_RegularChannelConfig(ADC3, ADC_Channel_14, 2, ADC_SampleTime_15Cycles);
	

	/* Enable DMA request after last transfer (Single-ADC mode) */
	ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);

	/* Enable ADC3 DMA */
	ADC_DMACmd(ADC3, ENABLE);

	/* Enable ADC3 */
	ADC_Cmd(ADC3, ENABLE);



	/* Start ADC3 Software Conversion */ 
	ADC_SoftwareStartConv(ADC3);


}



u16 AdcGetValue1(void)
{
	return(AdcConvertedValue[0]);
	
}


u16 AdcGetValue2(void)
{
	return(AdcConvertedValue[1]);

}



int AdcGetFilterValue1(void)
{
	int i;
	int div;
	int temp;
	int sum;
	int num;

	int result;
	

	sum = 0;
	num = 0;
	div = 8;
	for(i=0; i<256; i=i+div)
	{
		temp = AdcConvertedValue[i];

		if(temp >= 2048)
		{
			sum = sum + temp - 2048;
			num ++;

		}

	}

	result = sum/num;

	return result;
	
}


int AdcGetFilterValue2(void)
{
	int i;
	int div;
	int temp;
	int sum;
	int num;

	int result;
	

	sum = 0;
	num = 0;
	div = 8;
	for(i=0; i<256; i=i+div)
	{
		temp = AdcConvertedValue[i+1];

		if(temp >= 2048)
		{
			sum = sum + temp - 2048;
			num ++;

		}

	}

	result = sum/num;

	return result;
	
}



int AdcNewGetValue1(void)
{
	int i;
	int div;
	int temp;

	int result;

	int max;
	int min;


	max = 0;
	min = 4096;
	div = 2;

	for(i=0; i<256; i=i+div)
	{
		temp = AdcConvertedValue[i];

		if(temp > max)
		{
			max = temp;

		}
		else if(temp < min)
		{
			min = temp;

		}

	}

	result = max - min;

	return result;
	
}


int AdcNewGetValue2(void)
{
	int i;
	int div;
	int temp;

	int result;

	int max;
	int min;


	max = 0;
	min = 4096;
	div = 2;

	for(i=0; i<256; i=i+div)
	{
		temp = AdcConvertedValue[i+1];

		if(temp > max)
		{
			max = temp;

		}
		else if(temp < min)
		{
			min = temp;

		}

	}

	result = max - min;

	return result;
	
}















