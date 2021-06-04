/**
  ******************************************************************************
  * 
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/


#include "includes.h"
u32 Debuger;
u16 test;



#ifdef TEST_CODE

DEV_STATUS_T	 device_status_tmp;
void refresh_dev_status(void)
{
	static uint32	test_timer = 0;
	uint8  tmp;
	if(timer0_check_timer(&test_timer, 8000)) {

		/**/
		if (device_status_tmp.up_down_flag) { //xia
		
			if (device_status_tmp.status == ARRIVE_STATUS) {
				device_status_tmp.status = DEPARTURE_STATUS;
				device_status_tmp.door_side = DOOR_STATUS_INVALID;
				if ((device_status_tmp.next_station_num == device_status_tmp.end_station_num) || (device_status_tmp.cur_station_num == device_status_tmp.end_station_num)) {
				
					if (device_status_tmp.up_down_flag) {
						device_status_tmp.up_down_flag = TRAIN_UP;
						
						tmp = device_status_tmp.start_station_num;
						device_status_tmp.start_station_num = device_status_tmp.end_station_num;
						device_status_tmp.end_station_num = tmp;
						
						device_status_tmp.cur_station_num = device_status_tmp.start_station_num;
						device_status_tmp.next_station_num = device_status_tmp.start_station_num+1;
					}
					else {
						device_status_tmp.up_down_flag = TRAIN_DOWN;
						tmp = device_status_tmp.start_station_num;
						device_status_tmp.start_station_num = device_status_tmp.end_station_num;
						device_status_tmp.end_station_num = tmp;
						
						device_status_tmp.cur_station_num = device_status_tmp.start_station_num;
						device_status_tmp.next_station_num = device_status_tmp.start_station_num-1;
					}
				}
				else {
					
					device_status_tmp.cur_station_num = device_status_tmp.next_station_num;
					device_status_tmp.next_station_num -=1;
					//if((device_status_tmp.next_station_num == 5) || (device_status_tmp.next_station_num == 13)) {
					//	device_status_tmp.next_station_num -=2;

					//}
				}
				
			}
			else if (device_status_tmp.status == DEPARTURE_STATUS) {
				
					device_status_tmp.status = ARRIVE_STATUS;
					
					if (device_status_tmp.cur_station_num % 2) {
						device_status_tmp.door_side = THIS_DOOR;
					}
					else {
						device_status_tmp.door_side = OPPOSITE_DOOR;
					}
					
			}
			else if (dev_info.dev_status.status == SWITCH_UP_DOWN_STATUS){
				device_status_tmp.status = DEPARTURE_STATUS;
			}
			else {
				;
			}
		}
		else {	//shang
		
			if (device_status_tmp.status == ARRIVE_STATUS) {

				device_status_tmp.door_side = DOOR_STATUS_INVALID;
				device_status_tmp.status = DEPARTURE_STATUS;
				if ((device_status_tmp.next_station_num == device_status_tmp.end_station_num) || (device_status_tmp.cur_station_num == device_status_tmp.end_station_num)) {
								

					if (device_status_tmp.up_down_flag) {
						device_status_tmp.up_down_flag = TRAIN_UP;
						
						tmp = device_status_tmp.start_station_num;
						device_status_tmp.start_station_num = device_status_tmp.end_station_num;
						device_status_tmp.end_station_num = tmp;
						
						device_status_tmp.cur_station_num = device_status_tmp.start_station_num;
						device_status_tmp.next_station_num = device_status_tmp.start_station_num+1;
					}
					else {
						device_status_tmp.up_down_flag = TRAIN_DOWN;
						tmp = device_status_tmp.start_station_num;
						device_status_tmp.start_station_num =device_status_tmp.end_station_num;
						device_status_tmp.end_station_num = tmp;
						
						device_status_tmp.cur_station_num = device_status_tmp.start_station_num;
						device_status_tmp.next_station_num = device_status_tmp.start_station_num-1;
					}
				}
				else {
					
					device_status_tmp.cur_station_num = device_status_tmp.next_station_num;
					device_status_tmp.next_station_num +=1;
					//if((device_status_tmp.next_station_num == 4) || (device_status_tmp.next_station_num == 13)) {
					//	device_status_tmp.next_station_num +=2;

					//}
					
				}
				
			}
			else if (device_status_tmp.status == DEPARTURE_STATUS) {
					device_status_tmp.status = ARRIVE_STATUS;

					if (device_status_tmp.cur_station_num % 2) {
						device_status_tmp.door_side = THIS_DOOR;
					}
					else {
						device_status_tmp.door_side = OPPOSITE_DOOR;
					}
			}
			else if (dev_info.dev_status.status == SWITCH_UP_DOWN_STATUS){
				device_status_tmp.status = DEPARTURE_STATUS;
			}
			else {
				;
			}
		}

		/**/
		//serial_send_data(0x03, (uint8 *)&device_status_tmp, sizeof(DEVICE_STATUS_T));
	}
}

void get_device_status(void)
{
	static uint8 tmp = 1;
	uint8 down_flag_tmp;
	static uint32	timer_1 = 0;

	
	
	if (tmp) {
		tmp = 0;
		device_status_tmp.door_side = THIS_DOOR;
		device_status_tmp.brightness = BRIGHTNESS_VALUE;
		device_status_tmp.status = DEPARTURE_STATUS;
		device_status_tmp.up_down_flag = TRAIN_UP;
		device_status_tmp.start_station_num = 1;
		device_status_tmp.end_station_num =5;
		device_status_tmp.cur_station_num = device_status_tmp.start_station_num;
		device_status_tmp.next_station_num =device_status_tmp.cur_station_num+1;
		
		mem_cpy((uint8 *)&dev_info.dev_status, (uint8 *)&device_status_tmp, sizeof(DEV_STATUS_T));
	}
	else {
		//return;
		
		refresh_dev_status();



		
		down_flag_tmp = dev_info.dev_status.up_down_flag;
		mem_cpy((uint8 *)&dev_info.dev_status, (uint8 *)&device_status_tmp, sizeof(DEV_STATUS_T));

		if ((!device_status_tmp.up_down_flag) && down_flag_tmp) {
			dev_info.dev_status.up_down_flag = TRAIN_UP;
			dev_info.dev_status.status = SWITCH_UP_DOWN_STATUS;
			device_status_tmp.status = dev_info.dev_status.status;
			
		}
		else if ((device_status_tmp.up_down_flag) && (!down_flag_tmp)) {
			dev_info.dev_status.up_down_flag = TRAIN_DOWN;
			dev_info.dev_status.status = SWITCH_UP_DOWN_STATUS;
			device_status_tmp.status = dev_info.dev_status.status;
			
		}
		else {
			;
		}
	}

	if (timer0_check_timer(&timer_1, 5000)) {
			;//LED1Flash();
			device_status_tmp.brightness += 30;
			if (device_status_tmp.brightness > 100)
				device_status_tmp.brightness = 0;

			mem_cpy((uint8 *)&dev_info.dev_status, (uint8 *)&device_status_tmp, sizeof(DEV_STATUS_T));
			adjust_brightness();
			
		}
	
}

#endif


/**
  * @brief  Main program
  * @param  None
  * @retval None
  */

const uint8 aaa[] = {
	//5
  0x00,0x00,0x74,0x54,0x54,0x5C,0x00,0x00,
  //t
  0x00,0x40,0x40,0x7E,0x40,0x40,0x00,0x00,
	//o
	0x00,0x00,0x7E,0x42,0x42,0x7E,0x00,0x00,
	//p
	0x00,0x00,0xFE,0x90,0x90,0xF0,0x00,0x00
	
	}; 

const uint8 zi_di[]={/*--  文字:  地  --*/
/*--  楷体_GB231218;  此字体下对应的点阵为：宽x高=24x24   --*/
0xFF,0xFF,0xFF,0xFF,0xFB,0xF8,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xDF,0xC0,0xE7,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF7,0xF7,0xF7,0x00,0xEF,0xED,
0xFD,0xBB,0x80,0xF7,0xE7,0x00,0xDF,0xDD,0xBC,0x00,0x8F,0xFF,0xFF,0xFE,0xFF,0xFF,
0xFF,0xCF,0xCF,0xCF,0x9F,0x3F,0x7F,0x7F,0xFF,0xFF,0x0F,0xE7,0xF7,0x77,0xF7,0xF7,
0x77,0xF7,0xF7,0xE7,0xE7,0x07,0x8F,0xFF};


const uint8 Software_Version_V10[] = {
/*--  文字:  S  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x0E,0x11,0x10,0x10,0x10,0x1C,0x00,0x00,0x1C,0x04,0x84,0x84,0x44,0x38,0x00,

/*--  文字:  o  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0xF8,0x04,0x04,0x04,0x04,0xF8,0x00,

/*--  文字:  f  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x01,0x01,0x0F,0x11,0x11,0x11,0x18,0x00,0x04,0x04,0xFC,0x04,0x04,0x00,0x00,

/*--  文字:  t  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x01,0x01,0x07,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0xF8,0x04,0x04,0x00,0x00,

/*--  文字:  w  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x01,0x01,0x00,0x01,0x00,0x01,0x01,0x01,0xF0,0x0C,0x30,0xC0,0x30,0x0C,0xF0,0x00,

/*--  文字:  a  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x98,0x24,0x44,0x44,0x44,0xFC,0x04,

/*--  文字:  r  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x00,0x04,0x04,0xFC,0x84,0x04,0x00,0x80,0x00,

/*--  文字:  e  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0xF8,0x44,0x44,0x44,0x44,0xC8,0x00,

/*--  文字:     --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  文字:  V  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x10,0x1E,0x11,0x00,0x00,0x13,0x1C,0x10,0x00,0x00,0xE0,0x1C,0x70,0x80,0x00,0x00,

/*--  文字:  e  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0xF8,0x44,0x44,0x44,0x44,0xC8,0x00,

/*--  文字:  r  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x00,0x04,0x04,0xFC,0x84,0x04,0x00,0x80,0x00,

/*--  文字:  s  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0xCC,0x24,0x24,0x24,0x24,0x98,0x00,

/*--  文字:  i  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x01,0x19,0x19,0x00,0x00,0x00,0x00,0x00,0x04,0x04,0xFC,0x04,0x04,0x00,0x00,

/*--  文字:  o  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0xF8,0x04,0x04,0x04,0x04,0xF8,0x00,

/*--  文字:  n  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x01,0x01,0x00,0x01,0x01,0x01,0x00,0x00,0x04,0xFC,0x84,0x00,0x00,0x04,0xFC,0x04,

/*--  文字:     --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  文字:  V  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x10,0x1E,0x11,0x00,0x00,0x13,0x1C,0x10,0x00,0x00,0xE0,0x1C,0x70,0x80,0x00,0x00,

/*--  文字:  1  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x08,0x08,0x1F,0x00,0x00,0x00,0x00,0x00,0x04,0x04,0xFC,0x04,0x04,0x00,0x00,

/*--  文字:  .  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x0C,0x00,0x00,0x00,0x00,0x00,

/*--  文字:  0  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x07,0x08,0x10,0x10,0x08,0x07,0x00,0x00,0xF0,0x08,0x04,0x04,0x08,0xF0,0x00,

};

const uint8 device_name[] = {
/*--  文字:  宁  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x04,0x18,0x11,0x11,0x11,0x11,0x91,0x71,0x11,0x11,0x11,0x11,0x11,0x14,0x18,0x00,
0x00,0x00,0x00,0x00,0x00,0x02,0x01,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  文字:  波  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x08,0x06,0x40,0x30,0x03,0x00,0x1F,0x11,0x11,0x11,0xFF,0x11,0x11,0x15,0x18,0x00,
0x20,0x20,0x3E,0xC0,0x01,0x06,0xF8,0x01,0xC2,0x34,0x08,0x14,0x62,0x81,0x01,0x00,

/*--  文字:  二  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x00,0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00,0x00,
0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x00,

/*--  文字:  号  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x01,0x01,0x01,0x7D,0x45,0x45,0x45,0x45,0x45,0x45,0x45,0x7D,0x01,0x01,0x01,0x00,
0x00,0x00,0x00,0x60,0xA0,0x20,0x20,0x20,0x22,0x21,0x22,0x3C,0x00,0x00,0x00,0x00,

/*--  文字:  线  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x04,0x0C,0x35,0xC6,0x04,0x18,0x01,0x09,0x09,0xFF,0x09,0x92,0x52,0x12,0x02,0x00,
0x44,0xE6,0x44,0x48,0x48,0x48,0x02,0x02,0x04,0xC8,0x30,0x28,0x44,0x82,0x1F,0x00,

/*--  文字:  车  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x00,0x10,0x11,0x12,0x14,0x18,0xF0,0x17,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00,
0x10,0x10,0x90,0x90,0x90,0x90,0x90,0xFF,0x90,0x90,0x90,0x90,0x90,0x10,0x10,0x00,

/*--  文字:  头  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x00,0x00,0x08,0x06,0x20,0x18,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x81,0x81,0x82,0x82,0x84,0x88,0x90,0xE0,0x80,0xA0,0x90,0x88,0x84,0x83,0x80,0x00,

/*--  文字:  显  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x00,0x00,0x00,0x7F,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x7F,0x00,0x00,0x00,0x00,
0x02,0x42,0x22,0x1A,0x02,0xFE,0x02,0x02,0x02,0xFE,0x02,0x0A,0x12,0x62,0x02,0x00,

/*--  文字:  示  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x02,0x02,0x42,0x42,0x42,0x42,0x42,0x43,0x42,0x42,0x42,0x42,0x42,0x02,0x02,0x00,
0x04,0x08,0x10,0x60,0x00,0x02,0x01,0xFE,0x00,0x00,0x00,0x40,0x20,0x10,0x0C,0x00,

/*--  文字:  屏  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x00,0x00,0x7F,0x48,0x49,0x4D,0x4B,0x49,0x49,0x49,0x4B,0x4D,0x79,0x00,0x00,0x00,
0x02,0x0C,0xF0,0x20,0x21,0x26,0xF8,0x20,0x20,0x20,0xFF,0x20,0x20,0x20,0x00,0x00,

};

//1个字符纵向取模方式转换为可以向显示缓冲拷贝的字节流 
void Vertical_8_16_to_dispbuf(uint8 *dst, uint8 *src)
{
	uint8 i;
	uint8 tmp[8][2];

	for(i = 0; i < 16; i++) {
		tmp[i%8][i/8] = src[i];
	}

	memcpy(dst, tmp, 16);
}

//1个汉字纵向取模方式转换为可以向显示缓冲拷贝的字节流 
void Vertical_16_16_to_dispbuf(uint8 *dst, uint8 *src)
{
	uint8 i;
	uint8 tmp[16][2];

	for(i = 0; i < 32; i++) {
		tmp[i%16][i/16] = src[i];
	}

	memcpy(dst, tmp, 32);
}




uint8 timer_flag = 0,timer3_flag;
volatile uint8 refresh_flag = 0;
volatile uint8 timer2_flag = 0;

#define BLANK_COUNTER		1

int main(void)
{
	uint16 i,j;
	//static uint16 timer_tmp = 0;
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f2xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
        system_stm32f2xx.c file
     */
	
	static uint32	timer_1, timer_2;	//,, timer_3;

  	//timer_3 = timer_2 = 
	timer_1 = 0;

	
	//Delay(1000);
	Delay(1000);

	RCC_Configuration();

	Delay(1000);

	GPIO_Configuration();
	SysTick_Init();

	//Delay(1000);

	
	

	//LED1On();
	//LED2On();

	//Delay(500000);
	//Delay(500000);
	//Delay(500000);
	//Delay(500000);
	//Delay(1000);

	
	//Delay(100000);

	LED1Off();
	LED2Off();


	
	//Delay(1000);
	//Delay(100000);	
	//LED1On();


	//Delay(10000);


	
	AppInit();

	StorageInit();

	NetInit();

	Delay(1000);

	//AudioInit();

	//Delay(10000);
	//Delay(10000);
	//Delay(10000);
	StorageInfoRead();

	Delay(1000);
	

//--------------test-------------------

#if DMP_DRIVER
	device_init();

#ifdef TEST_MODE
	set_hc595_buf(0x00);
	Refresh_74HC595_status(hc595_info.green_led, hc595_info.red_led, hc595_info.hc595_config.buf_size);
#endif

#endif


//------------------------------------


	mem_set(green_CN[0], 0xff, sizeof(green_CN));
	mem_set(green_EN[0], 0xff, sizeof(green_EN));
	mem_set(red_CN[0], 0xff, sizeof(red_CN));
	mem_set(red_EN[0], 0xff, sizeof(red_EN));


	for (i = 0; i<sizeof(aaa); i++) {

		aaa[i] ^= 0xff;
	}
	//memcpy(ledbuf_tmp1, aaa, sizeof(aaa));

	//red_CN[0][0] = 0x00;
	//red_CN[0][1] = 0x00;
	ledbuf_tmp1[0][0] = 0x00;
	ledbuf_tmp1[0][1] = 0x00;

	ledbuf_tmp1[1][0] = 0x00;
	ledbuf_tmp1[1][1] = 0x00;

	memcpy(ledbuf_tmp1, zi_di, sizeof(zi_di));
	for(i = 0; i<sizeof(zi_di); i++) {
		ledbuf_tmp1[i%16][(i/16)%2] = zi_di[i];
	}
	
	
	/* *
	for (i=0; i < LEDBUF_COL_SUM; i++) {
		for (j = 0; j < LEDBUF_ROW_GRP; j++) {
			if ((i>15) && (i<24) )
				ledbuf_tmp1[i][j] = 0xff;
			else 
				ledbuf_tmp1[i][j] = 0x01;
		}

	}
	**/

/**
	for (i = 0; i < COL_SUM; i++) {

		for (j = 0; j < ROW_BYTE_SUM1; j++) {
			//if (!(i % 2))
			//if (i == 0  || i == 9)
			red_CN[i][j] = 0x7f;
		}
	}
	
	**/
	

	while (1) {

		if (timer0_check_timer(&timer_2, 500)) {
			LED1Flash();
		}

		//if (timer0_check_timer(&timer_1, 80)) 
		//if(timer3_flag == 3)
		{
			//screen_roll(sizeof(aaa), COLOR_RED);
			//timer_flag = 1;
			//tmp1 ^= 0xFF;
			//mem_set(red_CN[0], tmp1, sizeof(red_CN));
			
			//screen_roll(sizeof(aaa), COLOR_RED);

		}

			//if (timer0_check_timer(&timer_1, 5000)) 
				{

		if(refresh_flag)
		{
			refresh_flag = 0;
			screen_roll(16, COLOR_RED);
		}
		
		if(timer2_flag)
		{
			timer2_flag = 0;
			Refresh_74HC595_status();
		}

			}
	}






	while(1)
	{



		AppProcess();

		LwipProcess();

		NetProcess();

		//AudioProcess();


//-------------- app process


/**
		if (timer0_check_timer(&timer_3, 5000)) {
			;//LED1Flash();
			dev_info.dev_status.brightness += 30;
			if (dev_info.dev_status.brightness > 100)
				dev_info.dev_status.brightness = 0;
			adjust_brightness();
			
		}
		**/

#if 0
		if (timer0_check_timer(&timer_1, 500)) {
			udp_CtrlMsg_send_process(Ctrl_KEYWORD_ProcessData);
			LED2Flash();
			
			if (dmp_run_status) {
				dmp_run_status = 0;
			}
			
		}
#ifndef TEST_MODE		
		LED_status_process();
#endif

#endif

//-------------------



		

		if(MyTmCnt10 >= 1)
		{
			MyTmCnt10 = 0;
			MyTimer10();
		}

		if(MyTmCnt20 >= 2)
		{
			MyTmCnt20 = 2;
			MyTimer20();
		}
		
		if(MyTmCnt50 >= 5)
		{
			MyTmCnt50 = 0;
			MyTimer50();
		}

		if(MyTmCnt100 >= 10)
		{
			MyTmCnt100 = 0;
			MyTimer100();
		}

		if(MyTmCnt500 >= 50)
		{
			MyTmCnt500 = 0;
			//MyTimer500();
		}

		if(MyTmCnt1000 >= 100)
		{
			MyTmCnt1000 = 0;
			MyTimer1000();
		}

		if(MyTmCnt2000 >= 200)
		{
			MyTmCnt2000 = 0;
			MyTimer2000();
		}


	}

	   

  
}







#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {

  
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

