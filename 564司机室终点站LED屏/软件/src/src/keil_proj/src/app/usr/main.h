#ifndef __MAIN_H
#define __MAIN_H

//#define	TEST_CODE


#include "MyAll.h"

/**************************�����궨��********************************/


/************************** �˿ڶ��� ********************************/


/************************** ���Ͷ��� ********************************/
typedef unsigned   char    u8_t;
typedef signed     char    s8_t;
typedef unsigned   short   u16_t;
typedef signed     short   s16_t;
typedef unsigned   long    u32_t;
typedef signed     long    s32_t;
typedef u32_t mem_ptr_t;
typedef int sys_prot_t;





extern u32 Debuger;


/************************** �������� ********************************/
/* ȫ�ֱ�����������
 
#ifdef model_GLOBALS
    #define model_EXT
#else
    #define model_EXT extern

*/


/************************** �������� ********************************/





/**************************�����궨��********************************/


/************************** �˿ڶ��� ********************************/


/************************** ���Ͷ��� ********************************/


/************************** �������� ********************************/
/* ȫ�ֱ�����������
 
#ifdef model_GLOBALS
    #define model_EXT
#else
    #define model_EXT extern

*/

#ifdef main_GLOBALS
    #define main_EXT
#else
    #define main_EXT extern
#endif



extern volatile uint8 refresh_flag;
extern volatile uint8 timer2_flag;
void reset_reboot_cnt(void);

/************************** �������� ********************************/

#endif

