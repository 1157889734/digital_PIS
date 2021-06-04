#ifndef __MAIN_H
#define __MAIN_H

//#define	TEST_CODE


#include "MyAll.h"

/**************************常量宏定义********************************/


/************************** 端口定义 ********************************/


/************************** 类型定义 ********************************/
typedef unsigned   char    u8_t;
typedef signed     char    s8_t;
typedef unsigned   short   u16_t;
typedef signed     short   s16_t;
typedef unsigned   long    u32_t;
typedef signed     long    s32_t;
typedef u32_t mem_ptr_t;
typedef int sys_prot_t;





extern u32 Debuger;


/************************** 变量定义 ********************************/
/* 全局变量定义样例
 
#ifdef model_GLOBALS
    #define model_EXT
#else
    #define model_EXT extern

*/


/************************** 函数声明 ********************************/





/**************************常量宏定义********************************/


/************************** 端口定义 ********************************/


/************************** 类型定义 ********************************/


/************************** 变量定义 ********************************/
/* 全局变量定义样例
 
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

/************************** 函数声明 ********************************/

#endif

