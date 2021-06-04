#ifndef __iap_ram_func_h
#define __iap_ram_func_h


//#define __STM32F207ZG__ 1
#define __STM32F207ZC__ 1

#if defined (__STM32F207ZC__)
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbyte */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbyte */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbyte */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbyte */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbyte */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbyte */
// stm32f207zc flash is 256K
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 0 Kbyte */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08040000) /* Base @ of Sector 7, 0 Kbyte */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08040000) /* Base @ of Sector 8, 0 Kbyte */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x08040000) /* Base @ of Sector 9, 0 Kbyte */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x08040000) /* Base @ of Sector 10, 0 Kbyte */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x08040000) /* Base @ of Sector 11, 0 Kbyte */
/* End of the Flash address */
#define USER_FLASH_END_ADDRESS        0x0803FFFF

/*
            ***********************0x8000000
            *	IAP					*  sector 0 -- sector 4
            *		*****0x08004000
            *		app flag		*sector 1
            *		*****0x08008000
            *		save_param		*sector 2	//����ʵ�����ѡ��ʹ��
            *		*****0x0800C000
            *
            ***********************0x8020000
            *      APP					*   sector 5
            *						*
            ***********************0x8040000
*/
#define PROG_BAK_ADDRESS		ADDR_FLASH_SECTOR_5	//APP������ʼ��ַ
#define ApplicationAddress			ADDR_FLASH_SECTOR_5
#define Application_SECTOR_resSUM   (1)

#define PROG_IAP_ADDRESS		ADDR_FLASH_SECTOR_0		//IAP����ʼ��ַ
#define PROG_IAP_ADDRESS_end	(ADDR_FLASH_SECTOR_5 - 1)
#define PROG_FLAG_ADDRESS		ADDR_FLASH_SECTOR_1		//APP ��־��ʼ��ַ
#define PROG_FLAG_ADDRESS_end	(ADDR_FLASH_SECTOR_2 - 1)
#define SAVE_PARM_ADDRESS		ADDR_FLASH_SECTOR_2		//����������ʼ��ַ
#define SAVE_PARM_ADDRESS_end	(ADDR_FLASH_SECTOR_3 - 1)	//����������ʼ��ַ

#elif defined (__STM32F207ZG__)
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbyte */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbyte */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbyte */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbyte */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbyte */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbyte */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbyte */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbyte */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbyte */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbyte */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbyte */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbyte */
/* End of the Flash address */
#define USER_FLASH_END_ADDRESS        0x080FFFFF
/* Define the user application size */
//#define USER_FLASH_SIZE   (USER_FLASH_END_ADDRESS - SAVE_PARM_ADDRESS + 1)

/* Define the address from where user application will be loaded.
   Note: the 1st sector 0x08000000-0x08003FFF is reserved for the IAP code */

/*
            ***********************0x8000000
            *      IAP					*  sector 0 -- sector 4
            *						*
            ***********************0x8020000
            *      APP flag				*   sector 5
            *						*
            ***********************0x8040000
            *   	res					*  sector 6
            *						*
            ***********************0x8060000
            *      APP					*  sector 7 -- 11
            *						*
            ***********************
*/
#define PROG_BAK_ADDRESS	ADDR_FLASH_SECTOR_7	//����ʱ���򱸷���
#define ApplicationAddress		ADDR_FLASH_SECTOR_7
#define Application_SECTOR_resSUM   (11 - 7 + 1)

#define PROG_IAP_ADDRESS	ADDR_FLASH_SECTOR_0    //IAP����ʼ��ַ
#define PROG_FLAG_ADDRESS	ADDR_FLASH_SECTOR_5
#define PROG_FLAG_ADDRESS_end ADDR_FLASH_SECTOR_6
#define SAVE_PARM_ADDRESS	ADDR_FLASH_SECTOR_6   //����������ʼ��ַ
#define SAVE_PARM_ADDRESS_end	ADDR_FLASH_SECTOR_7    //����������ʼ��ַ

#endif

#define GPIO_PORT_WDT_IN	GPIOE
#define GPIO_Pin_WDT_IN	GPIO_Pin_2

extern unsigned int rcv_len;

void ram_flash_copy(unsigned int len);


#endif

