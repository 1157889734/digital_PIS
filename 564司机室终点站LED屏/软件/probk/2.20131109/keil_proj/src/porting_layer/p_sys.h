
#ifndef __p_sys_h
#define __p_sys_h

//��ʱ����uint8 timer0_check_timer(uint32 *timer, uint32 interval) �����жϵ�����
#define	N_MS	1

#define	TIMING_10s		100000
#define	TIMING_1sec	10000
#define	TIMING_500ms	5000
#define	TIMING_250ms	2500
#define	TIMING_100ms	1000
#define	TIMING_50ms	500


uint8 timer0_check_timer(uint32 *timer, uint32 interval);
void timer_isr_hook(void);
void feed_dog(void);

#endif


