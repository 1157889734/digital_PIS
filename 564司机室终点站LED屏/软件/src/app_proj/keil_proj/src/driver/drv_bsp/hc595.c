#include "includes.h"




#if DriverRoom_DRIVER
#define	NOselect138_No	17

uint8 red_CN[COL_SUM][ROW_BYTE_SUM1];
uint8 green_CN[COL_SUM][ROW_BYTE_SUM1];
uint8 red_EN[COL_SUM][ROW_BYTE_SUM2];
uint8 green_EN[COL_SUM][ROW_BYTE_SUM2];

typedef struct 
{
	uint8  row_index;
	
	struct 
	{
		uint8	d	:1;
		uint8	e	:1;
		uint8	c	:1;
		uint8	b	:1;
		uint8	a	:1;
		uint8	res	:3;
	}status;

}TAG_LED_ROW_SELECT;

const TAG_LED_ROW_SELECT led_row_select[] = {
	{1,		{0,	0,	0,	0,	0,	7}	},	//L1,17
	{2,		{0,	0,	0,	0,	1,	7}	},	//L2,18
	{3,		{0,	0,	0,	1,	0,	7}	},	//L3,19
	{4,		{0,	0,	0,	1,	1,	7}	},	//L4,20
	{5,		{0,	0,	1,	0,	0,	7}	},	//L5,21
	{6,		{0,	0,	1,	0,	1,	7}	},	//L6,22
	{7,		{0,	0,	1,	1,	0,	7}	},	//L7,23
	{8,		{0,	0,	1,	1,	1,	7}	},	//L8,24
	{9,		{1,	0,	0,	0,	0,	7}	},	//L9
	{10,		{1,	0,	0,	0,	1,	7}	},	//L10
	{11,		{1,	0,	0,	1,	0,	7}	},	//L11
	{12,		{1,	0,	0,	1,	1,	7}	},	//L12
	{13,		{1,	0,	1,	0,	0,	7}	},	//L13
	{14,		{1,	0,	1,	0,	1,	7}	},	//L14
	{15,		{1,	0,	1,	1,	0,	7}	},	//L15
	{16,		{1,	0,	1,	1,	1,	7}	},	//L16
	{17,		{1,	1,	0,	0,	0,	7}	}	//不选择
		
};




//n>16 no select
void select_138(uint8 n)
{
	//uint8 index;

	//index = 0;
	//while (led_row_select[index].row_index != NULL) {
		//if (led_row_select[index].row_index == n) {
	
		if ((n < 1) || (n > 17))
			return;
		
			if (led_row_select[(n -1)].status.a) {
				set_138A();
			}
			else {
				clr_138A();
			}

			if (led_row_select[(n -1)].status.b) {
				set_138B();
			}
			else {
				clr_138B();
			}

			if (led_row_select[(n -1)].status.c) {
				set_138C();
			}
			else {
				clr_138C();
			}

			if (led_row_select[(n -1)].status.d) {
				set_138D();
			}
			else {
				clr_138D();
			}
			//clr_138E();
			//set_138E();
#if 1
			if (led_row_select[(n -1)].status.e) {
				set_138E();
			}
			else {
				clr_138E();
			}
#endif
			//break;
		//}

	//}
}


void Device_74HC595_init(void)
{
	clr_CLK();
	clr_RCK();
}



void HC595_GPIO_config(void)
{	
	GPIO_InitTypeDef  GPIO_InitStructure;
  	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
 	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7
			|GPIO_Pin_8
			|GPIO_Pin_9
			|GPIO_Pin_10
			|GPIO_Pin_11
			|GPIO_Pin_12
			|GPIO_Pin_13
			|GPIO_Pin_14
			|GPIO_Pin_15;



	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 |GPIO_Pin_11;

	GPIO_Init(GPIOB, &GPIO_InitStructure);


	Device_74HC595_init();


	clr_led_buf();
	set_138E();


}

uint8 blank_buf[6] = {0xFF};


//**************显示扫描*********************
void Refresh_74HC595_status(void)
{
	static uint8 line_No = 1;
	uint8  i, j;
	uint8 mask = 0, offset = 0;
	//static uint8 line = 1;
	//static uint8 up_to_down = 1;
	//static uint8 roll_line = 1;
	//static uint8 cnt = 1;
#if 0
	uint8 *green1_status;
	uint8 *red1_status;
	uint8 *green2_status;
	uint8 *red2_status;

	if(line_No)
	{
		if(line_NO<9)
		{
			green1_status = green_CN[line_No - 1];
			green2_status = green_EN[line_No  - 1];
			red1_status = red_CN[line_No - 1];
			red2_status = red_EN[line_No  - 1];
		}
		else
		{
			green1_status = green_CN[line_No - 1];
			red1_status = red_CN[line_No - 1];
		}
	}

	for (i = 0; i < COL_BYTE_SUM; i++) {
		
		for (j = 0; j < 8; j++) {
			
			if ((green1_status[i] >> j) & 0x01) {
				set_GREEN1();
			}
			else {
				clr_GREEN1();
			}

			if ((red1_status[i] >> j) & 0x01) {
				set_RED1();
			}
			else {
				clr_RED1();
			}

			if ((green2_status[i] >> j) & 0x01) {
				set_GREEN2();
			}
			else {
				clr_GREEN2();
			}

			if ((red2_status[i] >> j) & 0x01) {
				set_RED2();
			}
			else {
				clr_RED2();
			}
			set_CLK();
			clr_CLK();
		}
	
	}
#else
	mask = 1 << (7-(line_No - 1) % 8);
	offset = (line_No - 1) /8;
	for (j = 0; j < COL_SUM; j++)
	{
		i = 8*(j/8) + 7 - j%8;
		
		if(green_CN[i][offset] & mask)
		{
			set_GREEN2();
		}
		else
		{
			clr_GREEN2();
		}
		if(red_CN[i][offset] & mask)
		{
			set_RED2();
		}
		else
		{
			clr_RED2();
		}
	#if 0
		if(offset==0)
		{
			if(green_EN[i][offset] & mask)
			{
				set_GREEN2();
			}
			else
			{
				clr_GREEN2();
			}
			if(red_EN[i][offset] & mask)
			{
				set_RED2();
			}
			else
			{
				clr_RED2();
			}
        	}
	#endif
		set_CLK();
		clr_CLK();
	}
#endif
	
	//select_138(17);
	set_138E();			//取消选择
	Delay(brightness);

	set_RCK();
	clr_RCK();
	
	select_138(line_No);
	//Delay(20);
#if 0
	if (up_to_down) {
		line_No++;
		if(line_No >= 16) {
			up_to_down = 0;
			
		}

		if ((line_No % 16) == 1)
		{
			refresh_flag = 1;
			
		}
	}
	else {
		if (line_No == 0)
			line_No = 1;
		line_No--;
		if(line_No <= 1) {
			up_to_down = 1;
		}

		if ((line_No % 16) == 0)
		{
			//refresh_flag = 1;
			
		}

		
	}

	if ((line_No % 16) == 0)
		{
			refresh_flag = 1;
			
		}
#endif
#if 1
	line_No++;
		if(line_No > 16) {
			line_No = 1;
			//up_to_down = 0;
			
		}

		if ((line_No % 16) == 1)
		{
			refresh_flag = 1;
			
		}
#endif
#if 0
	cnt++;
	if (cnt > 16) {
		cnt = 1;
		roll_line++;
		if (roll_line > 16)
			roll_line = 1;
	}

	roll_line = 1;
	line_No = 17-cnt;
		if (line_No == roll_line)
		{
			refresh_flag = 1;
			
		}
#endif

	
	
}


#if 0
void Refresh_74HC595_black(void)
{
	static int8	line_No = 1;
	uint8  i, j;
	uint8 mask, offset;
	static int8 add_value = 1;
	


	offset = (line_No - 1)/8;
	mask = 0x01 << (7 - (line_No-1)%8);


	

	set_GREEN1();
	set_RED1();
	set_GREEN2();
	set_RED2();
	for (i = 0; i < COL_SUM; i++) 
	{		
		set_CLK();
		clr_CLK();
	}


	set_138E();
	Delay(2);

	set_RCK();
	clr_RCK();
	
	select_138(line_No);

	line_No++;
	if(line_No > 32) {
		line_No = 1;
	}
	if((line_No % 16) == 1)
	{
		refresh_flag = 1;
	}
	else
	{
		refresh_flag = 0;
	}

}
#endif

#endif




