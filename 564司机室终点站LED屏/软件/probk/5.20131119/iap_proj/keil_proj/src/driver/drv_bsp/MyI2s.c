
//*****************************************************//
//		MyI2s.c
//*****************************************************//


#include "MyAll.h"




/* Mask for the bit EN of the I2S CFGR register */
#define I2S_ENABLE_MASK                 0x0400



u8 DmaPlayMem0[DMAPLAY_PACK_SIZE];
u8 DmaPlayMem1[DMAPLAY_PACK_SIZE];


u8 DmaListenMem0[DMALISTEN_PACK_SIZE];
u8 DmaListenMem1[DMALISTEN_PACK_SIZE];


int DmaPlayFlag;




void I2s2_Init(void)
{

	I2S_InitTypeDef I2S_InitStructure;

	
	
//	RCC_I2SCLKConfig(RCC_I2S2CLKSource_Ext);


//	RCC_PLLI2SConfig();

	

  	/* Enable the CODEC_I2S peripheral clock */
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

  	/* CODEC_I2S peripheral configuration */
  	SPI_I2S_DeInit(SPI2);
  	I2S_InitStructure.I2S_AudioFreq = 44100;
  	I2S_InitStructure.I2S_Standard = I2S_Standard_Phillips;
  	I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_16b;
  	I2S_InitStructure.I2S_CPOL = I2S_CPOL_Low;
  	I2S_InitStructure.I2S_Mode = I2S_Mode_MasterTx;

  	I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Disable;


  	/* Initialize the I2S peripheral with the structure above */
  	I2S_Init(SPI2, &I2S_InitStructure);

  	/* The I2S peripheral will be enabled only in the EVAL_AUDIO_Play() function 
       or by user functions if DMA mode not enabled */  



	I2S_Cmd(SPI2, ENABLE);



  
  
}






void DmaPlay_Init(void)
{


	DMA_InitTypeDef DMA_InitStructure; 

	NVIC_InitTypeDef NVIC_InitStructure;

	
	/* Enable the DMA clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE); 
  
	/* Configure the DMA Stream */
	DMA_Cmd(DMA1_Stream4, DISABLE);
	DMA_DeInit(DMA1_Stream4);


	DMA_DoubleBufferModeConfig(DMA1_Stream4, (u32)DmaPlayMem1, DMA_Memory_0);
	DMA_DoubleBufferModeCmd(DMA1_Stream4, ENABLE);


	
  	/* Set the parameters to be configured */
  	DMA_InitStructure.DMA_Channel = DMA_Channel_0;  
  	DMA_InitStructure.DMA_PeripheralBaseAddr = 0x4000380C;
  	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)DmaPlayMem0;
  	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  	DMA_InitStructure.DMA_BufferSize = (u32)(DMAPLAY_PACK_SIZE/2);
  	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; 

  	
//  	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;

	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;


	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  
	DMA_Init(DMA1_Stream4, &DMA_InitStructure);  

	

	DMA_ITConfig(DMA1_Stream4, DMA_IT_TC, ENABLE);

	DMA_ITConfig(DMA1_Stream4, DMA_IT_HT, ENABLE);

	
//  DMA_ITConfig(AUDIO_MAL_DMA_STREAM, DMA_IT_HT, ENABLE);
//  DMA_ITConfig(AUDIO_MAL_DMA_STREAM, DMA_IT_TE | DMA_IT_FE | DMA_IT_DME, ENABLE);

 
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);


	/* Enable the I2S DMA Stream*/
	DMA_Cmd(DMA1_Stream4, ENABLE);

	/* Enable the I2S DMA request */
	SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE);
    

}





void DmaPlay_Isr(void)
{

	Debuger = Debuger;


	if(DMA_GetITStatus(DMA1_Stream4, DMA_IT_TCIF4) == SET)
	{

		DMA_ClearITPendingBit(DMA1_Stream4, DMA_IT_TCIF4);

		Debuger = Debuger;

		DmaPlayWriteLeft();
		DmaPlayWriteRight();

		DmaPlayFlag = 1;
		

		PlayClock++;
		PlayClockSendFlag = 1;
		

	}


	if(DMA_GetITStatus(DMA1_Stream4, DMA_IT_HTIF4) == SET)
	{

		DMA_ClearITPendingBit(DMA1_Stream4, DMA_IT_HTIF4);

		Debuger = Debuger;

	}


}




void DmaPlayWriteLeft(void)
{


	int i;
	s16 *pdest;
	s16 *psrc;


	if(DMA_GetCurrentMemoryTarget(DMA1_Stream4)== 0)
	{
		pdest= (s16*)(DmaPlayMem1);

	}
	else
	{
		pdest= (s16*)(DmaPlayMem0);

	}
	
	
	psrc= (s16*)(pPlayLeftFifo+(PlayLeftConsumeIndex*PLAY_PACK_SIZE));

	PlayLeftConsumeIndex = IndexAdd1(PlayLeftConsumeIndex, PLAY_PACK_NUM);

	

	for (i=0; i<DMAPLAY_PACK_SIZE/4; i++)
	{
		*(pdest+2*i) = *(psrc+ i);

	}



}



void DmaPlayWriteRight(void)
{


	int i;
	s16 *pdest;
	s16 *psrc;
	


	if(DMA_GetCurrentMemoryTarget(DMA1_Stream4)== 0)
	{
		pdest= (s16*)(DmaPlayMem1);

	}
	else
	{
		pdest= (s16*)(DmaPlayMem0);

	}
	
	
	psrc= (s16*)(pPlayRightFifo+(PlayRightConsumeIndex*PLAY_PACK_SIZE));

	PlayRightConsumeIndex = IndexAdd1(PlayRightConsumeIndex, PLAY_PACK_NUM);

	

	for (i=0; i<DMAPLAY_PACK_SIZE/4; i++)
	{
		*(pdest+2*i+1) = *(psrc+ i);

	}



}



/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////



void I2s3_Init(void)
{

	I2S_InitTypeDef I2S_InitStructure;
	

  	/* Enable the CODEC_I2S peripheral clock */
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);

  	/* CODEC_I2S peripheral configuration */
  	SPI_I2S_DeInit(SPI3);
  	I2S_InitStructure.I2S_AudioFreq = 44100;
  	I2S_InitStructure.I2S_Standard = I2S_Standard_Phillips;
  	I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_16b;
  	I2S_InitStructure.I2S_CPOL = I2S_CPOL_Low;
  	I2S_InitStructure.I2S_Mode = I2S_Mode_SlaveRx;

  	I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Disable;


  	/* Initialize the I2S peripheral with the structure above */
  	I2S_Init(SPI3, &I2S_InitStructure);

  	/* The I2S peripheral will be enabled only in the EVAL_AUDIO_Play() function 
       or by user functions if DMA mode not enabled */  

	I2S_Cmd(SPI3, ENABLE);
  
  
}



void DmaListen_Init(void)
{


	DMA_InitTypeDef DMA_InitStructure; 

	NVIC_InitTypeDef NVIC_InitStructure;


	
	/* Enable the DMA clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE); 
  
	/* Configure the DMA Stream */
	DMA_Cmd(DMA1_Stream2, DISABLE);
	DMA_DeInit(DMA1_Stream2);


	DMA_DoubleBufferModeConfig(DMA1_Stream2, (u32)DmaListenMem1, DMA_Memory_0);
	DMA_DoubleBufferModeCmd(DMA1_Stream2, ENABLE);


	
  	/* Set the parameters to be configured */
  	DMA_InitStructure.DMA_Channel = DMA_Channel_0;  
  	DMA_InitStructure.DMA_PeripheralBaseAddr = 0x40003C0C;
  	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)DmaListenMem0;
  	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  	DMA_InitStructure.DMA_BufferSize = (u32)(DMALISTEN_PACK_SIZE/2);
  	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; 

  	
//  	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;

	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;


	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  
	DMA_Init(DMA1_Stream2, &DMA_InitStructure);  

	

	DMA_ITConfig(DMA1_Stream2, DMA_IT_TC, ENABLE);

	DMA_ITConfig(DMA1_Stream2, DMA_IT_HT, ENABLE);

	
//  DMA_ITConfig(AUDIO_MAL_DMA_STREAM, DMA_IT_HT, ENABLE);
//  DMA_ITConfig(AUDIO_MAL_DMA_STREAM, DMA_IT_TE | DMA_IT_FE | DMA_IT_DME, ENABLE);

 
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);


	/* Enable the I2S DMA Stream*/
	DMA_Cmd(DMA1_Stream2, ENABLE);

	/* Enable the I2S DMA request */
	SPI_I2S_DMACmd(SPI3, SPI_I2S_DMAReq_Rx, ENABLE);
	
    

}





void DmaListen_Isr(void)
{

	Debuger = Debuger;


	if(DMA_GetITStatus(DMA1_Stream2, DMA_IT_TCIF2) == SET)
	{

		DMA_ClearITPendingBit(DMA1_Stream2, DMA_IT_TCIF2);

		DmaListenUpLoadLeft();

		ListenFlag = 1;
		

		Debuger = Debuger;
		

	}


	if(DMA_GetITStatus(DMA1_Stream2, DMA_IT_HTIF2) == SET)
	{

		DMA_ClearITPendingBit(DMA1_Stream2, DMA_IT_HTIF2);

		Debuger = Debuger;

	}


}


void DmaListenUpLoadLeft(void)
{

	int i;
	s16 *pdest;
	s16 *psrc;


	if(DMA_GetCurrentMemoryTarget(DMA1_Stream2)== 0)
	{
		psrc= (s16*)(DmaListenMem1);

	}
	else
	{
		psrc= (s16*)(DmaListenMem0);

	} 
	
	
	pdest= (s16*)(pListenLeftFifo+(ListenLeftConsumeIndex*LISTEN_PACK_SIZE));

	ListenLeftConsumeIndex = IndexAdd1(ListenLeftConsumeIndex, LISTEN_PACK_NUM);

	

	for (i=0; i<DMALISTEN_PACK_SIZE/4; i++)
	{
		*(pdest + i) = *(psrc+ 2*i);

	}



}










////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////



void L3Delay(void)
{
	int i;

	for(i = 0; i <5; i++);

}

void L3SendByte(u8 indata, int l3mode)
{
	u8 i;
	
	
	WriteL3Mode(l3mode);
	WriteL3Clock(1);
	WriteL3Data(0);

	L3Delay();

	for(i=0; i<8; i++)
	{	
		if( (u8)((indata>>i) & 0x01) == 0x00 )
		{
			WriteL3Data(0);
			WriteL3Data(0);
			
		}
		else
		{
			WriteL3Data(0);
			WriteL3Data(1);
			
		}
		
		WriteL3Clock(0);
		L3Delay();
		WriteL3Clock(1);
		L3Delay();

	}
	

	L3Delay();

	WriteL3Mode(1);

	L3Delay();
	

}




void L3WriteUda1341Status(void)
{
	L3SendByte((0x14 | 0x02), L3MODE_ADDR);		//selecte status

	
	L3SendByte(0x60, L3MODE_DATA);	//reset 256fs

//	L3SendByte(0x40, L3MODE_DATA);	//reset 512fs

//	L3SendByte(0x50, L3MODE_DATA);	//reset 384s


	L3SendByte(0x20, L3MODE_DATA);	//data format 256fs

//	L3SendByte(0x00, L3MODE_DATA);	//data format 512fs

//	L3SendByte(0x10, L3MODE_DATA);	//reset 384fs


	L3SendByte(0x83, L3MODE_DATA);		//input gain 0dB

//	L3SendByte(0xa3, L3MODE_DATA);	//input gain 6dB



}

void L3WriteUda1341Data0(void)
{
	L3SendByte((0x14 | 0x00), L3MODE_ADDR);		//selecte Data0
	
	L3SendByte(0x0f, L3MODE_DATA);	//volume

	L3SendByte(0x81, L3MODE_DATA);




	L3SendByte(0xc2, L3MODE_DATA);
	L3SendByte(0xe3, L3MODE_DATA);	//input mixer


//	L3SendByte(0xc2, L3MODE_DATA);
//	L3SendByte(0xe1, L3MODE_DATA);	//input channel 1


	L3SendByte(0xc1, L3MODE_DATA);
	L3SendByte(0xff, L3MODE_DATA);	//turn input channel 2 to off


	L3SendByte(0xc0, L3MODE_DATA);
	L3SendByte(0xf0, L3MODE_DATA);	//set input channel 1 gain

	



}

void L3WriteUda1341Data1(void)
{



}

void L3InitUda1341(void)
{
	L3WriteUda1341Status();
	L3WriteUda1341Data0();
//	L3WriteUda1341Data1();

}

int L3Uda1341SetVolume(u8 dbvalue)
{
	if(dbvalue == 0)
	{
		L3SendByte((0x14 | 0x00), L3MODE_ADDR);
		L3SendByte(0x00, L3MODE_DATA);

		return (0);
	}
	else if(dbvalue < 61)
	{
		L3SendByte((0x14 | 0x00), L3MODE_ADDR);
		L3SendByte(dbvalue+1, L3MODE_DATA);

		return (0);
	}
	else if(dbvalue <= 100)
	{
		L3SendByte((0x14 | 0x00), L3MODE_ADDR);
		L3SendByte(0x6f, L3MODE_DATA);

		return (0);
	}
	else
	{
	
		return (-1);
	}


}

int L3Uda1341SetBassTreble(u8 bass, u8 treble)
{
	u8 value;

	if(bass > 16)
	{
		bass = 16;
	}
	if(treble > 6)
	{
		treble = 6;
	}

	bass = bass/2;
	treble = treble/2;

	value = 0x40 | ((bass&0x0f)<<2) | (treble&0x03);

	
	L3SendByte((0x14 | 0x00), L3MODE_ADDR);
	L3SendByte(value, L3MODE_DATA);

	return (0);

}


int L3Uda1341SetInputGain(u8 dbvalue)
{

	L3SendByte((0x14 | 0x00), L3MODE_ADDR);		//selecte Data0

	
	if(dbvalue > 45)
	{
		dbvalue = 45;
	}

	dbvalue = (dbvalue*2)/3;
	
	L3SendByte(0xc0, L3MODE_DATA);
	L3SendByte((0xe0|dbvalue), L3MODE_DATA);	//set channel 1 gain

	return (0);



}
















