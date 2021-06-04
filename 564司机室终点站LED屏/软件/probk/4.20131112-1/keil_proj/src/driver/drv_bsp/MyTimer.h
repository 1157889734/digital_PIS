//*******************************************************//
//						MyTimer.h								//
//*******************************************************//

#ifndef _MYTIMER_H
#define  _MYTIMER_H

#include "MyAll.h"


extern int MyTmCnt10;
extern int MyTmCnt20;
extern int MyTmCnt50;
extern int MyTmCnt100;
extern int MyTmCnt500;
extern int MyTmCnt1000;
extern int MyTmCnt2000;


extern void SysTick_Init(void);
extern void SysTick_Isr(void);

extern void Timer1_Init(void);
extern void Timer1_Isr(void);


extern void MyTimer10(void);
extern void MyTimer20(void);
extern void MyTimer50(void);
extern void MyTimer100(void);
extern void MyTimer500(void);
extern void MyTimer1000(void);
extern void MyTimer2000(void);

 

#endif
