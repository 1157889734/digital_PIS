#include "includes.h"



//������buf ������ͬ
uint8	ledbuf_tmp1[LEDBUF_COL_SUM][LEDBUF_ROW_GRP];	//��ʾ���ݱ���,Э������ݵ�ֱ�ӿ���
uint8	ledbuf_tmp[LEDBUF_COL_SUM][LEDBUF_ROW_GRP];	//��ʾ���ݱ���

uint16	roll_col_sum_tmp = 0;			//��ʾ���������ֽ�������
uint8	color_tmp = COLOR_NONE;
uint16	roll_col_num = 0;


//��ԭred_EN, green_EN  �����һά��ά����
uint8 red_tmp[DISP_COL_SUM][DISP_ROW_GRP];
uint8 green_tmp[DISP_COL_SUM][DISP_ROW_GRP];

uint8	roll_speed = 12;
uint16	brightness=2;

#if 1
void screen_roll(uint16 roll_col_sum, uint8 color)
{
	
	uint16	col_sum;				//������
	uint16	disp_col_tail;			//��Ч��ʾ�����β��
	uint16	disp_col_head;		//��Ч��ʾ�������ʼ��
//	uint16	disp_col_sum;

	uint8	div;			//ÿ���ƶ������� 1 - 7
	uint16	fill_colsum_tail, fill_colsum_head;		//ledbuf_tmpͷ�����Ŀ�����

	uint16	i,j,k;

	static uint8 flag_tmp = 0;

	


//	uint8	dst_char_num, dst_bit_num, src_char_num, src_bit_num;
//	uint8	dst_st, src_st, len;


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
		// ���
		memset(ledbuf_tmp, 0xff, sizeof(ledbuf_tmp));
		memcpy(ledbuf_tmp[fill_colsum_head], ledbuf_tmp1[0], roll_col_sum_tmp*LEDBUF_ROW_GRP);
					
		roll_col_num = 0;
		flag_tmp = 1;
	}

	//�����������Ƿ��и���
	//��,�����´������濪ʼ����
#if 0
	if ((roll_col_sum_tmp == roll_col_sum) && (color_tmp == color)) {
		for (i = 0; i < roll_col_sum_tmp; i++) {
			for (j = 0; j < DISP_ROW_GRP; j++) {
				if (ledbuf_tmp[i+fill_colsum_head][j] != ledbuf_tmp1[i][j]) {
					memset(red_tmp[0], 0xff, sizeof(red_tmp));
					memset(green_tmp[0], 0xff, sizeof(green_tmp));
					// ���
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
		// ���
		memset(ledbuf_tmp, 0xff, sizeof(ledbuf_tmp));
		memcpy(ledbuf_tmp[fill_colsum_head], ledbuf_tmp1[0], roll_col_sum_tmp*LEDBUF_ROW_GRP);
					
		roll_col_num = 0;
	}
#endif

	//�����ܵ�����
	col_sum = roll_col_sum_tmp + fill_colsum_head + fill_colsum_tail;
	

	//��ɫ�ж�
	if ((color_tmp != COLOR_RED) && (color_tmp != COLOR_GREEN) && (color_tmp != COLOR_YELLOW)) {
		//��ɫ��Ч�����ʾ
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
	#if 0
	{
		// ��λ
		for (k = 0; k < div; k++) {

		for (i = disp_col_head; i < disp_col_tail; i++) {

			for (j = 0; j < DISP_ROW_GRP; j++) {

				switch (color_tmp) {
				case COLOR_GREEN:
					green_tmp[i][j] = green_tmp[i+1][j];
					break;
					
				case COLOR_RED:
					red_tmp[i][j] = red_tmp[i+1][j];
					break;
					
				case COLOR_YELLOW:
					green_tmp[i][j] = green_tmp[i+1][j];
					red_tmp[i][j] = red_tmp[i+1][j];
					break;
					
				default:
					break;
				}
			}
		}

		//��������������
		for (j = 0; j < DISP_ROW_GRP; j++) {

			switch (color_tmp) {
			case COLOR_GREEN:
				green_tmp[i][j] = ledbuf_tmp[roll_col_num][j];
				break;
					
			case COLOR_RED:
				red_tmp[i][j] = ledbuf_tmp[roll_col_num][j];
				break;
					
			case COLOR_YELLOW:
				green_tmp[i][j] = ledbuf_tmp[roll_col_num][j];
				red_tmp[i][j] = ledbuf_tmp[roll_col_num][j];
				break;
					
			default:
				break;
				
				}
			}
		}
	}
	#endif
	

	#if 0
	//��������
	disp_col_sum = (uint16)DISP_COL_SUM;

	for (i = 0; i < disp_col_sum; i++) {
		dst_char_num = i/8;
		dst_bit_num = i%8;

		src_char_num = i;
		for (j = 0; j < DISP_ROW_GRP; j++) {
			for (k = 0; k < 8; k++) {
				src_bit_num = 7-k;
				if (green_tmp[src_char_num][j] & (0x01 << src_bit_num)) {
					green_CN[8*j + k][dst_char_num] |= (0x01 << dst_bit_num);
				}
				else {
					green_CN[8*j + k][dst_char_num] &= ~(0x01 << dst_bit_num);
				}

				if (red_tmp[src_char_num][j] & (0x01 << src_bit_num)) {
					red_CN[8*j + k][dst_char_num] |= (0x01 << dst_bit_num);
				}
				else {
					red_CN[8*j + k][dst_char_num] &= ~(0x01 << dst_bit_num);
				}
			}
		}
	}
	#else

	memcpy(&green_CN[0][0], &green_tmp[0][0], sizeof(green_CN));
	memcpy(&red_CN[0][0], &red_tmp[0][0], sizeof(red_CN));

	//memcpy(&green_EN[0][0], &green_tmp[0][0], sizeof(green_EN));
	//memcpy(&red_EN[0][0], &red_tmp[0][0], sizeof(red_EN));

	#endif


	// ����Դ���ݵ��к�
	roll_col_num += div;
	if (roll_col_num >= col_sum) {
		roll_col_num = 0;
	}
	
	
}
#else
void screen_roll(uint16 roll_col_sum, uint8 color)
{
	
	uint16	col_sum;								//������
	uint16	fill_colsum_head;		//ledbuf_tmpͷ�����Ŀ�����
	
	fill_colsum_head = FILL_COLsum_head;


//�����������Ƿ��и���
//��,�����´������濪ʼ���

	//if(download_flag)
	{
		if(memcmp(ledbuf_tmp[fill_colsum_head], ledbuf_tmp1[0], 4) !=0|| ((color_tmp != color))||(roll_col_sum_tmp != roll_col_sum))	//memcmp ���Ϊ0
		{
			roll_col_sum_tmp = roll_col_sum;
			color_tmp = color;
			memset(red_CN, 0xff, sizeof(red_CN));
			memset(green_CN, 0xff, sizeof(green_CN));
			// ���
			memset(ledbuf_tmp, 0xff, sizeof(ledbuf_tmp));
			memcpy(ledbuf_tmp[fill_colsum_head], ledbuf_tmp1[0], 4);//roll_col_sum_tmp*LEDBUF_ROW_GRP);						
			roll_col_num = 0;
			
		}
	}

	//�����ܵ�����
	col_sum =  roll_col_sum_tmp + fill_colsum_head;


	//��ɫ�ж�
	if ((color_tmp != COLOR_RED) && (color_tmp != COLOR_GREEN) && (color_tmp != COLOR_YELLOW)) {
		//��ɫ��Ч�����ʾ
		memset(red_CN, 0xff, sizeof(red_CN));
		memset(green_CN, 0xff, sizeof(red_CN));
		
	}
	else {
		switch (color_tmp) {
			
		case COLOR_GREEN:
			memcpy(green_CN, ledbuf_tmp[roll_col_num], COL_SUM*2);  //����һ����320���ֽ�
			break;
					
		case COLOR_RED:
			
			memcpy(red_CN, ledbuf_tmp[roll_col_num], COL_SUM*2);  
			break;
					
		case COLOR_YELLOW:
			memcpy(green_CN, ledbuf_tmp[roll_col_num], COL_SUM*2);
			memcpy(red_CN, ledbuf_tmp[roll_col_num], COL_SUM*2);
			break;
					
		default:
			break;
		}
		
	}


	//memcpy(&green[0][0], &green_tmp[0][0], sizeof(green));
	//memcpy(&red[0][0], &red_tmp[0][0], sizeof(red));


	// ����Դ���ݵ��к�
	//roll_col_num += div;
	roll_col_num++;
	if (roll_col_num >= col_sum) {
		roll_col_num = 0;
	}
	
	
}
#endif




