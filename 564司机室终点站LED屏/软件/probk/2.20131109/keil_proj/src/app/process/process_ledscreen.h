
#ifndef __process_ledscreen_h
#define __process_ledscreen_h

#define COLOR_NONE			0
#define COLOR_GREEN		1
#define COLOR_RED			2
#define COLOR_YELLOW		3


#if 0
//����
#define	DISP_COL_TAIL	46		//  
#define	DISP_COL_HEAD	1		//
#define	DIV				1		//ÿ���ƶ������� 1 - 7
#define	FILL_COLsum_tail		16		//ledbuf_tmpβ�����Ŀ�����
#define	FILL_COLsum_head	(32)	//
#else
extern uint16	DISP_COL_TAIL;		//  
extern uint16	DISP_COL_HEAD;		//
extern uint16	DIV				;		//ÿ���ƶ������� 1 - 7
extern uint16	FILL_COLsum_tail		;		//ledbuf_tmpβ�����Ŀ�����
extern uint16	FILL_COLsum_head	;//
#endif
//ע���޸�
#define LEDBUF_ROW_GRP	ROW_BYTE_SUM1
#define LEDBUF_COL_SUM		(1000 + 100)
#define DISP_ROW_GRP		LEDBUF_ROW_GRP
#define DISP_COL_SUM		COL_SUM

extern uint8	ledbuf_tmp1[LEDBUF_COL_SUM][LEDBUF_ROW_GRP];	//��ʾ���ݱ���,Э������ݵ�ֱ�ӿ���
extern uint8	ledbuf_tmp[LEDBUF_COL_SUM][LEDBUF_ROW_GRP];	//��ʾ���ݱ���
extern uint8 red_tmp[DISP_COL_SUM][DISP_ROW_GRP];
extern uint8 green_tmp[DISP_COL_SUM][DISP_ROW_GRP];


extern uint8	roll_speed;
extern uint16	brightness;

void screen_roll(uint16 roll_col_sum, uint8 color);
void clr_led_buf(void);
void LedScreen_process(void);

#endif




