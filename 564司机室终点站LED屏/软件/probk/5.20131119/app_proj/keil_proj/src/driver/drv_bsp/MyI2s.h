//*******************************************************//
//						MyI2s.h								//
//*******************************************************//

#ifndef _MYI2S_H
#define  _MYI2S_H

#include "MyAll.h"





#define L3MODE_ADDR	0
#define L3MODE_DATA	1


#define DMAPLAY_PACK_SIZE	2048

#define DMALISTEN_PACK_SIZE	2048


extern int DmaPlayFlag;



void I2s2_Init(void);

void DmaPlay_Init(void);

void DmaPlay_Isr(void);



void DmaPlayWriteLeft(void);

void DmaPlayWriteRight(void);





void I2s2_Play(u16 inda);

void DmaPlay_Play(uint32_t Addr, uint32_t Size);







void I2s3_Init(void);

void DmaListen_Init(void);

void DmaListen_Isr(void);


void DmaListenUpLoadLeft(void);






void L3SendByte(u8 indata, int l3mode);

void L3InitUda1380(void);
void L3InitUda1341(void);


int L3Uda1341SetVolume(u8 dbvalue);

int L3Uda1341SetBassTreble(u8 bass, u8 treble);

int L3Uda1341SetInputGain(u8 dbvalue);



#endif


