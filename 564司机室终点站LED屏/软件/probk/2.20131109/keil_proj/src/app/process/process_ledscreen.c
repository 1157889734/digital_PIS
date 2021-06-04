#include "includes.h"

//ÕâÁ½¸öbuf ³¤¶ÈÏàÍ¬
uint8	ledbuf_tmp1[LEDBUF_COL_SUM][LEDBUF_ROW_GRP];	//ÏÔÊ¾Êı¾İ±¸·İ,Ğ­Òé°üÊı¾İµÄÖ±½Ó¿½±´
uint8	ledbuf_tmp[LEDBUF_COL_SUM][LEDBUF_ROW_GRP];	//ÏÔÊ¾Êı¾İ±¸·İ

uint16	roll_col_sum_tmp = 0;			//ÏÔÊ¾Êı¾İ×İÏò×Ö½ÚÊı±¸·İ
uint8	color_tmp = COLOR_NONE;
uint16	roll_col_num = 0;


//ÓëÔ­red_EN, green_EN  Êı×éµÄÒ»Î¬¶şÎ¬µ¹ÖÃ
uint8 red_tmp[DISP_COL_SUM][DISP_ROW_GRP];
uint8 green_tmp[DISP_COL_SUM][DISP_ROW_GRP];

uint8	roll_speed = 12;
uint16	brightness=30;

uint16	DISP_COL_TAIL	=COL_SUM -1;		//  
uint16	DISP_COL_HEAD	=0;		//
uint16	DIV				=1;		//Ã¿´ÎÒÆ¶¯µÄÁĞÊı 1 - 7
uint16	FILL_COLsum_tail		=16;		//ledbuf_tmpÎ²²¿Ìî³äµÄ¿ÕÁĞÊı
uint16	FILL_COLsum_head	=COL_SUM;//

#if 1
void screen_roll(uint16 roll_col_sum, uint8 color)
{
	uint16	col_sum;				//×ÜÁĞÊı
	uint16	disp_col_tail;			//ÓĞĞ§ÏÔÊ¾ÇøÓòµÄÎ²ĞĞ
	uint16	disp_col_head;		//ÓĞĞ§ÏÔÊ¾ÇøÓòµÄÆğÊ¼ĞĞ
	uint8	div;			//Ã¿´ÎÒÆ¶¯µÄÁĞÊı 1 - 7
	uint16	fill_colsum_tail, fill_colsum_head;		//ledbuf_tmpÍ·²¿Ìî³äµÄ¿ÕÁĞÊı
	uint16	i,j,k;
	static uint8 flag_tmp = 0;

	disp_col_tail = DISP_COL_TAIL;
	disp_col_head = DISP_COL_HEAD;
	div = DIV;
	fill_colsum_head = FILL_COLsum_head;
	fill_colsum_tail = FILL_COLsum_tail;

	if(!flag_tmp)
	{
		roll_col_sum_tmp = roll_col_sum;
		color_tmp = color;
		memset(red_tmp[0], 0xff, sizeof(red_tmp));
		memset(green_tmp[0], 0xff, sizeof(green_tmp));
		// Ìî³ä
		memset(ledbuf_tmp, 0xff, sizeof(ledbuf_tmp));
		memcpy(ledbuf_tmp[fill_colsum_head], ledbuf_tmp1[0], roll_col_sum_tmp*LEDBUF_ROW_GRP);
					
		roll_col_num = 0;
		flag_tmp = 1;
	}

	//»º³åÇøÊı¾İÊÇ·ñÓĞ¸üĞÂ
	//ÓĞ,ÔòÖØĞÂ´Ó×îÓÒÃæ¿ªÊ¼¹öÆÁ
#if 1
	if ((roll_col_sum_tmp == roll_col_sum) && (color_tmp == color)) {
		for (i = 0; i < roll_col_sum_tmp; i++) {
			for (j = 0; j < DISP_ROW_GRP; j++) {
				if (ledbuf_tmp[i+fill_colsum_head][j] != ledbuf_tmp1[i][j]) {
					memset(red_tmp[0], 0xff, sizeof(red_tmp));
					memset(green_tmp[0], 0xff, sizeof(green_tmp));
					// Ìî³ä
					memset(ledbuf_tmp, 0xff, sizeof(ledbuf_tmp));
					memcpy(ledbuf_tmp[fill_colsum_head], ledbuf_tmp1[0], roll_col_sum_tmp*LEDBUF_ROW_GRP);
					roll_col_num = 0;
					break;
				}
			}
		}
	}
	else {
		roll_col_sum_tmp = roll_col_sum;
		color_tmp = color;
		memset(red_tmp[0], 0xff, sizeof(red_tmp));
		memset(green_tmp[0], 0xff, sizeof(green_tmp));
		// Ìî³ä
		memset(ledbuf_tmp, 0xff, sizeof(ledbuf_tmp));
		memcpy(ledbuf_tmp[fill_colsum_head], ledbuf_tmp1[0], roll_col_sum_tmp*LEDBUF_ROW_GRP);
					
		roll_col_num = 0;
	}
#endif

	//¼ÆËã×ÜµÄÁĞÊı
	col_sum = roll_col_sum_tmp + fill_colsum_head + fill_colsum_tail;
	

	//ÑÕÉ«ÅĞ¶Ï
	if ((color_tmp != COLOR_RED) && (color_tmp != COLOR_GREEN) && (color_tmp != COLOR_YELLOW)) {
		//ÑÕÉ«ÎŞĞ§Çå¿ÕÏÔÊ¾
		memset(red_tmp[0], 0xff, sizeof(red_tmp));
		memset(green_tmp[0], 0xff, sizeof(green_tmp));

	}
	else {
		switch (color_tmp) {
		case COLOR_GREEN:
			memcpy(green_tmp[disp_col_head], ledbuf_tmp[roll_col_num], (disp_col_tail + 1 - disp_col_head)*DISP_ROW_GRP);
			break;
					
		case COLOR_RED:
			memcpy(red_tmp[disp_col_head], ledbuf_tmp[roll_col_num], (disp_col_tail + 1 - disp_col_head)*DISP_ROW_GRP);
			break;
					
		case COLOR_YELLOW:
			memcpy(green_tmp[disp_col_head], ledbuf_tmp[roll_col_num], (disp_col_tail + 1 - disp_col_head)*DISP_ROW_GRP);
			memcpy(red_tmp[disp_col_head], ledbuf_tmp[roll_col_num], (disp_col_tail + 1 - disp_col_head)*DISP_ROW_GRP);
			break;
					
		default:
			break;
		}
		
	}
	memcpy(&green_CN[0][0], &green_tmp[0][0], sizeof(green_CN));
	memcpy(&red_CN[0][0], &red_tmp[0][0], sizeof(red_CN));

	//memcpy(&green_EN[0][0], &green_tmp[0][0], sizeof(green_EN));
	//memcpy(&red_EN[0][0], &red_tmp[0][0], sizeof(red_EN));

	// µ÷ÕûÔ´Êı¾İµÄÁĞºÅ
	roll_col_num += div;
	if (roll_col_num >= col_sum) {
		roll_col_num = 0;
	}
}
#else
void screen_roll(uint16 roll_col_sum, uint8 color)
{
	
	uint16	col_sum;								//×ÜÁĞÊı
	uint16	fill_colsum_head;		//ledbuf_tmpÍ·²¿Ìî³äµÄ¿ÕÁĞÊı
	
	fill_colsum_head = 0;


//»º³åÇøÊı¾İÊÇ·ñÓĞ¸üĞÂ
//ÓĞ,ÔòÖØĞÂ´Ó×îÓÒÃæ¿ªÊ¼¹öÆ

	//if(download_flag)
	{
		if(memcmp(ledbuf_tmp[fill_colsum_head], ledbuf_tmp1[0], roll_col_sum) !=0|| ((color_tmp != color))||(roll_col_sum_tmp != roll_col_sum))	//memcmp ÏàµÈÎª0
		{
			roll_col_sum_tmp = roll_col_sum;
			color_tmp = color;
			memset(red_CN, 0xff, sizeof(red_CN));
			memset(green_CN, 0xff, sizeof(green_CN));
			// Ìî³ä
			memset(ledbuf_tmp, 0xff, sizeof(ledbuf_tmp));
			memcpy(ledbuf_tmp[fill_colsum_head], ledbuf_tmp1[0], roll_col_sum);//roll_col_sum_tmp*LEDBUF_ROW_GRP);						
			roll_col_num = 0;
			
		}
	}

	//¼ÆËã×ÜµÄÁĞÊı
	col_sum =  roll_col_sum_tmp + fill_colsum_head;


	//ÑÕÉ«ÅĞ¶Ï
	if ((color_tmp != COLOR_RED) && (color_tmp != COLOR_GREEN) && (color_tmp != COLOR_YELLOW)) {
		//ÑÕÉ«ÎŞĞ§Çå¿ÕÏÔÊ¾
		memset(red_CN, 0xff, sizeof(red_CN));
		memset(green_CN, 0xff, sizeof(green_CN));
		
	}
	else {
		switch (color_tmp) {
			
		case COLOR_GREEN:
			memcpy(green_CN, ledbuf_tmp[roll_col_num], 48);  //¿½±´Ò»ÆÁ¹²320¸ö×Ö½Ú
			break;
					
		case COLOR_RED:
			
			memcpy(red_CN, ledbuf_tmp[roll_col_num], 48);  
			break;
					
		case COLOR_YELLOW:
			memcpy(green_CN, ledbuf_tmp[roll_col_num], 48);
			memcpy(red_CN, ledbuf_tmp[roll_col_num], 48);
			break;
					
		default:
			break;
		}
		
	}


	//memcpy(&green[0][0], &green_tmp[0][0], sizeof(green));
	//memcpy(&red[0][0], &red_tmp[0][0], sizeof(red));


	// µ÷ÕûÔ´Êı¾İµÄÁĞºÅ
	//roll_col_num += div;
	roll_col_num++;
	if (roll_col_num >= col_sum) {
		roll_col_num = 0;
	}
	
	
}
#endif



const uint8 Software_Version_V10[] = {
/*--  ÎÄ×Ö:  S  --*/
/*--  ËÎÌå12;  ´Ë×ÖÌåÏÂ¶ÔÓ¦µÄµãÕóÎª£º¿íx¸ß=8x16   --*/
0x00,0x0E,0x11,0x10,0x10,0x10,0x1C,0x00,0x00,0x1C,0x04,0x84,0x84,0x44,0x38,0x00,

/*--  ÎÄ×Ö:  o  --*/
/*--  ËÎÌå12;  ´Ë×ÖÌåÏÂ¶ÔÓ¦µÄµãÕóÎª£º¿íx¸ß=8x16   --*/
0x00,0x00,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0xF8,0x04,0x04,0x04,0x04,0xF8,0x00,

/*--  ÎÄ×Ö:  f  --*/
/*--  ËÎÌå12;  ´Ë×ÖÌåÏÂ¶ÔÓ¦µÄµãÕóÎª£º¿íx¸ß=8x16   --*/
0x00,0x01,0x01,0x0F,0x11,0x11,0x11,0x18,0x00,0x04,0x04,0xFC,0x04,0x04,0x00,0x00,

/*--  ÎÄ×Ö:  t  --*/
/*--  ËÎÌå12;  ´Ë×ÖÌåÏÂ¶ÔÓ¦µÄµãÕóÎª£º¿íx¸ß=8x16   --*/
0x00,0x01,0x01,0x07,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0xF8,0x04,0x04,0x00,0x00,

/*--  ÎÄ×Ö:  w  --*/
/*--  ËÎÌå12;  ´Ë×ÖÌåÏÂ¶ÔÓ¦µÄµãÕóÎª£º¿íx¸ß=8x16   --*/
0x01,0x01,0x00,0x01,0x00,0x01,0x01,0x01,0xF0,0x0C,0x30,0xC0,0x30,0x0C,0xF0,0x00,

/*--  ÎÄ×Ö:  a  --*/
/*--  ËÎÌå12;  ´Ë×ÖÌåÏÂ¶ÔÓ¦µÄµãÕóÎª£º¿íx¸ß=8x16   --*/
0x00,0x00,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x98,0x24,0x44,0x44,0x44,0xFC,0x04,

/*--  ÎÄ×Ö:  r  --*/
/*--  ËÎÌå12;  ´Ë×ÖÌåÏÂ¶ÔÓ¦µÄµãÕóÎª£º¿íx¸ß=8x16   --*/
0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x00,0x04,0x04,0xFC,0x84,0x04,0x00,0x80,0x00,

/*--  ÎÄ×Ö:  e  --*/
/*--  ËÎÌå12;  ´Ë×ÖÌåÏÂ¶ÔÓ¦µÄµãÕóÎª£º¿íx¸ß=8x16   --*/
0x00,0x00,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0xF8,0x44,0x44,0x44,0x44,0xC8,0x00,

/*--  ÎÄ×Ö:     --*/
/*--  ËÎÌå12;  ´Ë×ÖÌåÏÂ¶ÔÓ¦µÄµãÕóÎª£º¿íx¸ß=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  ÎÄ×Ö:  V  --*/
/*--  ËÎÌå12;  ´Ë×ÖÌåÏÂ¶ÔÓ¦µÄµãÕóÎª£º¿íx¸ß=8x16   --*/
0x10,0x1E,0x11,0x00,0x00,0x13,0x1C,0x10,0x00,0x00,0xE0,0x1C,0x70,0x80,0x00,0x00,

/*--  ÎÄ×Ö:  e  --*/
/*--  ËÎÌå12;  ´Ë×ÖÌåÏÂ¶ÔÓ¦µÄµãÕóÎª£º¿íx¸ß=8x16   --*/
0x00,0x00,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0xF8,0x44,0x44,0x44,0x44,0xC8,0x00,

/*--  ÎÄ×Ö:  r  --*/
/*--  ËÎÌå12;  ´Ë×ÖÌåÏÂ¶ÔÓ¦µÄµãÕóÎª£º¿íx¸ß=8x16   --*/
0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x00,0x04,0x04,0xFC,0x84,0x04,0x00,0x80,0x00,

/*--  ÎÄ×Ö:  s  --*/
/*--  ËÎÌå12;  ´Ë×ÖÌåÏÂ¶ÔÓ¦µÄµãÕóÎª£º¿íx¸ß=8x16   --*/
0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0xCC,0x24,0x24,0x24,0x24,0x98,0x00,

/*--  ÎÄ×Ö:  i  --*/
/*--  ËÎÌå12;  ´Ë×ÖÌåÏÂ¶ÔÓ¦µÄµãÕóÎª£º¿íx¸ß=8x16   --*/
0x00,0x01,0x19,0x19,0x00,0x00,0x00,0x00,0x00,0x04,0x04,0xFC,0x04,0x04,0x00,0x00,

/*--  ÎÄ×Ö:  o  --*/
/*--  ËÎÌå12;  ´Ë×ÖÌåÏÂ¶ÔÓ¦µÄµãÕóÎª£º¿íx¸ß=8x16   --*/
0x00,0x00,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0xF8,0x04,0x04,0x04,0x04,0xF8,0x00,

/*--  ÎÄ×Ö:  n  --*/
/*--  ËÎÌå12;  ´Ë×ÖÌåÏÂ¶ÔÓ¦µÄµãÕóÎª£º¿íx¸ß=8x16   --*/
0x01,0x01,0x00,0x01,0x01,0x01,0x00,0x00,0x04,0xFC,0x84,0x00,0x00,0x04,0xFC,0x04,

/*--  ÎÄ×Ö:     --*/
/*--  ËÎÌå12;  ´Ë×ÖÌåÏÂ¶ÔÓ¦µÄµãÕóÎª£º¿íx¸ß=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  ÎÄ×Ö:  V  --*/
/*--  ËÎÌå12;  ´Ë×ÖÌåÏÂ¶ÔÓ¦µÄµãÕóÎª£º¿íx¸ß=8x16   --*/
0x10,0x1E,0x11,0x00,0x00,0x13,0x1C,0x10,0x00,0x00,0xE0,0x1C,0x70,0x80,0x00,0x00,

/*--  ÎÄ×Ö:  1  --*/
/*--  ËÎÌå12;  ´Ë×ÖÌåÏÂ¶ÔÓ¦µÄµãÕóÎª£º¿íx¸ß=8x16   --*/
0x00,0x08,0x08,0x1F,0x00,0x00,0x00,0x00,0x00,0x04,0x04,0xFC,0x04,0x04,0x00,0x00,

/*--  ÎÄ×Ö:  .  --*/
/*--  ËÎÌå12;  ´Ë×ÖÌåÏÂ¶ÔÓ¦µÄµãÕóÎª£º¿íx¸ß=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x0C,0x00,0x00,0x00,0x00,0x00,

/*--  ÎÄ×Ö:  0  --*/
/*--  ËÎÌå12;  ´Ë×ÖÌåÏÂ¶ÔÓ¦µÄµãÕóÎª£º¿íx¸ß=8x16   --*/
0x00,0x07,0x08,0x10,0x10,0x08,0x07,0x00,0x00,0xF0,0x08,0x04,0x04,0x08,0xF0,0x00

};

const uint8 device_name[] = {
/*--  ÎÄ×Ö:  Äş  --*/
/*--  ËÎÌå12;  ´Ë×ÖÌåÏÂ¶ÔÓ¦µÄµãÕóÎª£º¿íx¸ß=16x16   --*/
0x04,0x18,0x11,0x11,0x11,0x11,0x91,0x71,0x11,0x11,0x11,0x11,0x11,0x14,0x18,0x00,
0x00,0x00,0x00,0x00,0x00,0x02,0x01,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  ÎÄ×Ö:  ²¨  --*/
/*--  ËÎÌå12;  ´Ë×ÖÌåÏÂ¶ÔÓ¦µÄµãÕóÎª£º¿íx¸ß=16x16   --*/
0x08,0x06,0x40,0x30,0x03,0x00,0x1F,0x11,0x11,0x11,0xFF,0x11,0x11,0x15,0x18,0x00,
0x20,0x20,0x3E,0xC0,0x01,0x06,0xF8,0x01,0xC2,0x34,0x08,0x14,0x62,0x81,0x01,0x00,

/*--  ÎÄ×Ö:  ¶ş  --*/
/*--  ËÎÌå12;  ´Ë×ÖÌåÏÂ¶ÔÓ¦µÄµãÕóÎª£º¿íx¸ß=16x16   --*/
0x00,0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00,0x00,
0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x00,

/*--  ÎÄ×Ö:  ºÅ  --*/
/*--  ËÎÌå12;  ´Ë×ÖÌåÏÂ¶ÔÓ¦µÄµãÕóÎª£º¿íx¸ß=16x16   --*/
0x01,0x01,0x01,0x7D,0x45,0x45,0x45,0x45,0x45,0x45,0x45,0x7D,0x01,0x01,0x01,0x00,
0x00,0x00,0x00,0x60,0xA0,0x20,0x20,0x20,0x22,0x21,0x22,0x3C,0x00,0x00,0x00,0x00,

/*--  ÎÄ×Ö:  Ïß  --*/
/*--  ËÎÌå12;  ´Ë×ÖÌåÏÂ¶ÔÓ¦µÄµãÕóÎª£º¿íx¸ß=16x16   --*/
0x04,0x0C,0x35,0xC6,0x04,0x18,0x01,0x09,0x09,0xFF,0x09,0x92,0x52,0x12,0x02,0x00,
0x44,0xE6,0x44,0x48,0x48,0x48,0x02,0x02,0x04,0xC8,0x30,0x28,0x44,0x82,0x1F,0x00,

/*--  ÎÄ×Ö:  ³µ  --*/
/*--  ËÎÌå12;  ´Ë×ÖÌåÏÂ¶ÔÓ¦µÄµãÕóÎª£º¿íx¸ß=16x16   --*/
0x00,0x10,0x11,0x12,0x14,0x18,0xF0,0x17,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00,
0x10,0x10,0x90,0x90,0x90,0x90,0x90,0xFF,0x90,0x90,0x90,0x90,0x90,0x10,0x10,0x00,

/*--  ÎÄ×Ö:  Í·  --*/
/*--  ËÎÌå12;  ´Ë×ÖÌåÏÂ¶ÔÓ¦µÄµãÕóÎª£º¿íx¸ß=16x16   --*/
0x00,0x00,0x08,0x06,0x20,0x18,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x81,0x81,0x82,0x82,0x84,0x88,0x90,0xE0,0x80,0xA0,0x90,0x88,0x84,0x83,0x80,0x00,

/*--  ÎÄ×Ö:  ÏÔ  --*/
/*--  ËÎÌå12;  ´Ë×ÖÌåÏÂ¶ÔÓ¦µÄµãÕóÎª£º¿íx¸ß=16x16   --*/
0x00,0x00,0x00,0x7F,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x7F,0x00,0x00,0x00,0x00,
0x02,0x42,0x22,0x1A,0x02,0xFE,0x02,0x02,0x02,0xFE,0x02,0x0A,0x12,0x62,0x02,0x00,

/*--  ÎÄ×Ö:  Ê¾  --*/
/*--  ËÎÌå12;  ´Ë×ÖÌåÏÂ¶ÔÓ¦µÄµãÕóÎª£º¿íx¸ß=16x16   --*/
0x02,0x02,0x42,0x42,0x42,0x42,0x42,0x43,0x42,0x42,0x42,0x42,0x42,0x02,0x02,0x00,
0x04,0x08,0x10,0x60,0x00,0x02,0x01,0xFE,0x00,0x00,0x00,0x40,0x20,0x10,0x0C,0x00,

/*--  ÎÄ×Ö:  ÆÁ  --*/
/*--  ËÎÌå12;  ´Ë×ÖÌåÏÂ¶ÔÓ¦µÄµãÕóÎª£º¿íx¸ß=16x16   --*/
0x00,0x00,0x7F,0x48,0x49,0x4D,0x4B,0x49,0x49,0x49,0x4B,0x4D,0x79,0x00,0x00,0x00,
0x02,0x0C,0xF0,0x20,0x21,0x26,0xF8,0x20,0x20,0x20,0xFF,0x20,0x20,0x20,0x00,0x00

};

const uint8 ceshizhong[] = {
/*--  ÎÄ×Ö:  ²â  --*/
/*--  ËÎÌå12;  ´Ë×ÖÌåÏÂ¶ÔÓ¦µÄµãÕóÎª£º¿íx¸ß=16x16   --*/
0x08,0x06,0x40,0x31,0x00,0x7F,0x40,0x4F,0x40,0x7F,0x00,0x1F,0x00,0xFF,0x00,0x00,
0x20,0x20,0x7E,0x80,0x01,0xE2,0x0C,0xF0,0x08,0xE4,0x00,0xE2,0x01,0xFE,0x00,0x00,

/*--  ÎÄ×Ö:  ÊÔ  --*/
/*--  ËÎÌå12;  ´Ë×ÖÌåÏÂ¶ÔÓ¦µÄµãÕóÎª£º¿íx¸ß=16x16   --*/
0x02,0x02,0x42,0x33,0x00,0x09,0x09,0x09,0x09,0x09,0xFF,0x08,0x88,0x68,0x08,0x00,
0x00,0x00,0x00,0xFC,0x08,0x14,0x06,0xFC,0x08,0x08,0x80,0x70,0x0C,0x02,0x0F,0x00,

/*--  ÎÄ×Ö:  ÖĞ  --*/
/*--  ËÎÌå12;  ´Ë×ÖÌåÏÂ¶ÔÓ¦µÄµãÕóÎª£º¿íx¸ß=16x16   --*/
0x00,0x00,0x0F,0x08,0x08,0x08,0x08,0xFF,0x08,0x08,0x08,0x08,0x0F,0x00,0x00,0x00,
0x00,0x00,0xF0,0x20,0x20,0x20,0x20,0xFF,0x20,0x20,0x20,0x20,0xF0,0x00,0x00,0x00

};

//1¸ö×Ö·û×İÏòÈ¡Ä£·½Ê½×ª»»Îª¿ÉÒÔÏòÏÔÊ¾»º³å¿½±´µÄ×Ö½ÚÁ÷ 
void Vertical_8_16_to_dispbuf(uint8 *dst, uint8 *src)
{
	uint8 i;
	uint8 tmp[8][2];

	for(i = 0; i < 16; i++) {
		tmp[i%8][i/8] = src[i]^0xff;
	}

	memcpy(dst, tmp, 16);
}

//1¸öºº×Ö×İÏòÈ¡Ä£·½Ê½×ª»»Îª¿ÉÒÔÏòÏÔÊ¾»º³å¿½±´µÄ×Ö½ÚÁ÷ 
void Vertical_16_16_to_dispbuf(uint8 *dst, uint8 *src)
{
	uint8 i;
	uint8 tmp[16][2];

	for(i = 0; i < 32; i++) {
		tmp[i%16][i/16] = src[i]^0xff;
	}

	memcpy(dst, tmp, 32);
}

void static_test_fill_ledbuf(void)
{
	uint16 col_sum_roll = 0;
	uint16 i;
	uint8 *dst = &red_CN[0][0];
	uint8 *src = (uint8 *)ceshizhong;
	uint8 *p_tmp = &red_CN[0][0];

	for (i = 0 ; i < sizeof(ceshizhong)/32; i++) {
		dst=p_tmp+i*32;
		if (dst+32 > p_tmp+sizeof(red_CN))
			break;
		Vertical_16_16_to_dispbuf(dst,src+i*32);
	}
	
}
uint16 test_fill_ledbuf(void)
{
	uint16 col_sum_roll = 0;
	uint16 i;
	uint8 *dst = &ledbuf_tmp1[0][0];
	uint8 *src = (uint8 *)device_name;
	uint8 *p_tmp = &ledbuf_tmp1[0][0];
	
	for (i = 0 ; i < sizeof(device_name)/32; i++) {
		dst=p_tmp+i*32;
		Vertical_16_16_to_dispbuf(dst,src+i*32);
	}

	//col_sum_roll = 16*(sizeof(device_name)/32);
	//return col_sum_roll;
	#if 1
	p_tmp+=i*32;
	src = (uint8 *)Software_Version_V10;
	for (i = 0; i < sizeof(Software_Version_V10)/16; i++) {
		dst=p_tmp+i*16;
		Vertical_8_16_to_dispbuf(dst, src+i*16);
	}
	col_sum_roll = 16*(sizeof(device_name)/32) + 8*(sizeof(Software_Version_V10)/16);

	
	return col_sum_roll;
	#endif
}






void clr_led_buf(void)
{

	memset(red_CN, 0xff, sizeof(red_CN));
	memset(green_CN, 0xff, sizeof(green_CN));
	memset(red_EN, 0xff, sizeof(red_EN));
	memset(green_EN, 0xff, sizeof(green_EN));
	memset(ledbuf_tmp, 0xff, sizeof(ledbuf_tmp));
	memset(ledbuf_tmp1, 0xff, sizeof(ledbuf_tmp1));
	memset(red_tmp, 0xff, sizeof(red_tmp));
	memset(green_tmp, 0xff, sizeof(green_tmp));
	
}

void turn_off_screen_clrbuf(void)
{
	clr_led_buf();
	set_138E();
}
void turn_off_screen(void)
{
	set_138E();
}


#define SCREEN_MODE_off 				1
#define SCREEN_MODE_test 				2
#define SCREEN_MODE_static_test 				3
#define SCREEN_MODE_softwareVersion 	4
#define SCREEN_MODE_stationID		 	5


typedef struct {
	uint8 mode;
	uint8 *param;
	uint16 len;
}TAG_SEL_MODE;
TAG_SEL_MODE sel_screen_MODE = {
	SCREEN_MODE_softwareVersion,
	NULL,
	0
};

typedef struct {
	uint16	parm_DISP_COL_TAIL;		//  
	uint16	parm_DISP_COL_HEAD;		//
	uint16	parm_DIV;		//Ã¿´ÎÒÆ¶¯µÄÁĞÊı 1 - 7
	uint16	parm_FILL_COLsum_tail;		//ledbuf_tmpÎ²²¿Ìî³äµÄ¿ÕÁĞÊı
	uint16	parm_FILL_COLsum_head;//
}TAG_disp_param_SCREEN_MODE_softwareVersion;

TAG_disp_param_SCREEN_MODE_softwareVersion disp_param_SCREEN_MODE_softwareVersion;

void set_dispmode_SCREEN_MODE_softwareVersion(void)
{
	disp_param_SCREEN_MODE_softwareVersion.parm_DISP_COL_HEAD = 0;
	disp_param_SCREEN_MODE_softwareVersion.parm_DISP_COL_TAIL = 47;
	disp_param_SCREEN_MODE_softwareVersion.parm_DIV = 1;
	disp_param_SCREEN_MODE_softwareVersion.parm_FILL_COLsum_head = 32;
	disp_param_SCREEN_MODE_softwareVersion.parm_FILL_COLsum_tail = 16;

	sel_screen_MODE.mode = SCREEN_MODE_softwareVersion;
	sel_screen_MODE.param = (uint8 *)&disp_param_SCREEN_MODE_softwareVersion;
	sel_screen_MODE.len = sizeof(disp_param_SCREEN_MODE_softwareVersion);
}

void dispcall_SCREEN_MODE_softwareVersion(uint8 mode_change, void *param, uint16 len)
{
	static uint16 col = 0;
	if (mode_change) {
		clr_led_buf();
		col = test_fill_ledbuf();
	}
	if (!col) {
		clr_led_buf();
		return;
	}

	if (!len) {
		DISP_COL_TAIL	=COL_SUM-1;		//  
		DISP_COL_HEAD	=0;		//
		DIV				=1;		//Ã¿´ÎÒÆ¶¯µÄÁĞÊı 1 - 7
		FILL_COLsum_tail		=16;		//ledbuf_tmpÎ²²¿Ìî³äµÄ¿ÕÁĞÊı
		FILL_COLsum_head	=COL_SUM;//
	}
	
	if(refresh_flag)
		{
			refresh_flag = 0;
			screen_roll(col, COLOR_RED);
		}
	
}


void set_dispmode_SCREEN_MODE_test(void)
{
	sel_screen_MODE.mode = SCREEN_MODE_test;
	sel_screen_MODE.param = NULL;
	sel_screen_MODE.len = NULL;
}

void dispcall_SCREEN_MODE_test(uint8 mode_change, void *param, uint16 len)
{
	uint16 col = 8;
	uint8 i;
	if (mode_change) {
		clr_led_buf();
		for (i = 0; i < 8; i++) {
			ledbuf_tmp1[i][0] = (1<<i)^0xff;
			ledbuf_tmp1[i][1] = (1<<i)^0xff;
		}
		
	}

	if (!len) {
		DISP_COL_TAIL	=COL_SUM-1;		//  
		DISP_COL_HEAD	=0;		//
		DIV				=1;		//Ã¿´ÎÒÆ¶¯µÄÁĞÊı 1 - 7
		FILL_COLsum_tail		=16;		//ledbuf_tmpÎ²²¿Ìî³äµÄ¿ÕÁĞÊı
		FILL_COLsum_head	=COL_SUM;//
	}
	
	if(refresh_flag)
		{
			refresh_flag = 0;
			screen_roll(8, COLOR_RED);
		}
	
}

void set_dispmode_SCREEN_MODE_static_test(void)
{
	sel_screen_MODE.mode = SCREEN_MODE_static_test;
	sel_screen_MODE.param = NULL;
	sel_screen_MODE.len = NULL;
}



void dispcall_SCREEN_MODE_static_test(uint8 mode_change, void *param, uint16 len)
{
	
	if (mode_change) {
		clr_led_buf();
		static_test_fill_ledbuf();
		
		
	}
	
}


typedef void(* Callback_screen_dispMode)(uint8 mode_change, void *param, uint16 len);
typedef struct {
	uint8 mode;
	Callback_screen_dispMode screen_dispMode_Callback;
}TAG_SCREEN_DISPMODE;

TAG_SCREEN_DISPMODE screen_mode_disp_tbl[] = {
	{SCREEN_MODE_softwareVersion, 	dispcall_SCREEN_MODE_softwareVersion},
	{SCREEN_MODE_test, 				dispcall_SCREEN_MODE_test},
	{SCREEN_MODE_static_test, dispcall_SCREEN_MODE_static_test},
	
	{NULL, NULL},
	{NULL, NULL}
};

typedef void(* Callback_set_dispmode)(void);
typedef struct {
	uint8 mode;
	Callback_set_dispmode set_dispmode_Callback;
}TAG_SET_DISPMODE;

TAG_SET_DISPMODE screen_mode_set_tbl[] = {
	{SCREEN_MODE_softwareVersion, 	set_dispmode_SCREEN_MODE_softwareVersion},
	{SCREEN_MODE_test, 				set_dispmode_SCREEN_MODE_test},
	{SCREEN_MODE_static_test, 		set_dispmode_SCREEN_MODE_static_test},

	{NULL, NULL},
	{NULL, NULL}
};

void set_dispmode(uint8 mode)
{
	uint8 index;

	while (screen_mode_set_tbl[index].set_dispmode_Callback != NULL) {
		if (screen_mode_set_tbl[index].mode == mode) {
			screen_mode_set_tbl[index].set_dispmode_Callback();
			break;
		}
		index++;
	}
}

uint8 mode_index_tbl[] = {
	SCREEN_MODE_softwareVersion,
	SCREEN_MODE_test, 
	SCREEN_MODE_static_test
};

void LedScreen_process(void)
{
	uint8 index;
	uint8 new_mode;
	uint8 flag;
	static uint16 col;
	static TAG_SEL_MODE mode_tmp = {0, 0, 0};
	static uint8 tmp = 1;
	static uint16 cnt = 0;
	static uint32 timer_1 = 0;
	static uint8 mode_index = 0;

	if (tmp) {
		
		tmp =0;
		memset(&sel_screen_MODE, 0, sizeof(sel_screen_MODE));
		set_dispmode(mode_index_tbl[0]);
	}


	if (timer0_check_timer(&timer_1, (uint32)TIMING_10s)) {
		mode_index++;
		if (mode_index >= sizeof(mode_index_tbl)) {
			mode_index = 0;
		}
		set_dispmode(mode_index_tbl[mode_index]);
		
	}

	index = 0;
	flag = 0;

	while (screen_mode_disp_tbl[index].screen_dispMode_Callback != NULL) {
		if (screen_mode_disp_tbl[index].mode == sel_screen_MODE.mode) {
			if (memcmp(&mode_tmp, &sel_screen_MODE,sizeof(TAG_SEL_MODE)) == 0) {
				new_mode = 0;
			}
			else {
				memcpy(&mode_tmp, &sel_screen_MODE,sizeof(TAG_SEL_MODE));
				new_mode = 1;
			}
			screen_mode_disp_tbl[index].screen_dispMode_Callback(new_mode, sel_screen_MODE.param, sel_screen_MODE.len);
			flag = 1;
			break;
		}
		index++;
	}

	if (!flag) {
		turn_off_screen_clrbuf();
	}
	else {
		/*
		if(refresh_flag)
		{
			refresh_flag = 0;
			screen_roll(col, COLOR_RED);
		}*/
		
		if(timer2_flag)
		{
			timer2_flag = 0;
			Refresh_74HC595_status();
		}
	}
	
}





