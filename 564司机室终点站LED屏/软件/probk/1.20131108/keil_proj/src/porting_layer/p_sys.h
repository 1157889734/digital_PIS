
#ifndef __p_sys_h
#define __p_sys_h

//��ʱ����uint8 timer0_check_timer(uint32 *timer, uint32 interval) �����жϵ�����
#define	N_MS	1


uint8 timer0_check_timer(uint32 *timer, uint32 interval);
void timer_isr_hook(void);
void feed_dog(void);

#endif


