
#define	process_led_GLOBALS
#include "includes.h"

#if DMP_DRIVER


/*
#define COLOR_NONE			0
#define COLOR_GREEN		1
#define COLOR_RED			2
#define COLOR_YELLOW		3
*/

typedef struct {
	uint8	color_index;
	uint8	color;
	uint8	green_status;
	uint8	red_status;
}COLOR_STATUS_T;

COLOR_STATUS_T led_color_status[] = {
	{	1,		COLOR_NONE,		1,			1	},
	{	2,		COLOR_GREEN,		0,			1	},
	{	3,		COLOR_RED,			1,			0	},
	{	4,		COLOR_YELLOW,		0,			0	},
	{	null,		null,					null,			null	}
};

/*********************************************************************
��������:	�趨led��ɫ
����˵��: 	λ��:	LOCATION_INDEX_T location_info, 
				��ɫ:	uint8 color
ȫ�ֱ�����--
���ú�����--
ע������:	--
����˵��:	--
**********************************************************************/
void Set_led_color(LOCATION_INDEX_T location_info, uint8 color)
{
	uint8	index;
	uint8	len;
	
	len = hc595_info.hc595_config.buf_size;
	if ((location_info.char_num == LED_NONE) || (location_info.char_num > (len - 1))) {
	
		report_error(ERR2);
		return;
	}

	if ((location_info.bit_num == LED_NONE) || (location_info.bit_num > 7)) {
		report_error(ERR3);
		return;
	}
	
	index = 0;
	while (led_color_status[index].color_index != null) {
		if (led_color_status[index].color == color) {
			if (led_color_status[index].green_status) {
				hc595_info.green_led[location_info.char_num] |= 1 << location_info.bit_num;
			}
			else {
				hc595_info.green_led[location_info.char_num] &= ~(1 << location_info.bit_num);
			}
			
			if (led_color_status[index].red_status) {
				hc595_info.red_led[location_info.char_num] |= 1 << location_info.bit_num;
			}
			else {
				hc595_info.red_led[location_info.char_num] &= ~(1 << location_info.bit_num);
			}
			break;
		}
		index++;
	}
	
}


/*********************************************************************
��������:	����վ�Ż�ȡλ��
����˵��: 	λ��:	LED_INDEX_T *led_location, 
				վ��:	uint8 station_num
ȫ�ֱ�����--
���ú�����--
ע������:	--
����˵��:	--
**********************************************************************/
void get_line_location(LED_INDEX_T *led_location, uint8 station_num)
{
	uint8	index;

	mem_set((uint8 *)led_location, LED_NONE, sizeof(LED_INDEX_T));

	led_location->station_index = station_num;
	
	index = 0;
	while (dev_info.line_index[index].station_index != null) {
		if (dev_info.line_index[index].station_index == station_num) {
			mem_cpy((uint8 *)led_location, (uint8 *)(&dev_info.line_index[index]), sizeof(LED_INDEX_T));
			return;
		}
		index++;
	}
	report_error(ERR4);
}


/*********************************************************************
��������:	//start1  ~ //end1 ֮��Ĵ����趨��̬��ͼ����ɫ��ʾ
����˵��: 	--
ȫ�ֱ�����--
���ú�����--
ע������:	--
����˵��:	--
**********************************************************************/
//start1
void SetLineColor_station(uint8 station_num, uint8 color)
{
	LED_INDEX_T	location;

	get_line_location(&location, station_num);

	Set_led_color(location.station_led, color);
}

void SetLineColor_R(uint8 station_num, uint8 color)
{
	LED_INDEX_T	location;

	get_line_location(&location, station_num);

	Set_led_color(location.r_direction_led, color);
}

void SetLineColor_L(uint8 station_num, uint8 color)
{
	LED_INDEX_T	location;

	get_line_location(&location, station_num);

	Set_led_color(location.l_direction_led, color);
}


#define	TYPE_station		1
#define	TYPE_R			2
#define	TYPE_L			3

#define	DOOR_this		4
#define	DOOR_opposite	5

/*
typedef void (*set_line_color_Callback)(uint8, uint8);
typedef struct
{
	uint8	type;
	set_line_color_Callback	method;
}SET_LINE_COLOR_PROCESS_T;

SET_LINE_COLOR_PROCESS_T code SetLineColor_process_table[] = {
	{	TYPE_station,			SetLineColor_station		},
	{	TYPE_R,				SetLineColor_R			},
	{	TYPE_L,				SetLineColor_L			},
	{	null,					null						}
};
*/

void set_line_color(uint8 station_num, uint8 type, uint8 color)
{

	switch (type) {
	case TYPE_station:
		SetLineColor_station(station_num, color);
		break;
		
	case TYPE_R:
		SetLineColor_R(station_num, color);
		break;
		
	case TYPE_L:
		SetLineColor_L(station_num, color);
		break;
		
	default:
		report_error(ERR5);
		break;

	}
	/*
	uint8	index;
	
	index = 0;
	while (SetLineColor_process_table[index].method != null) {
		if (SetLineColor_process_table[index].type == type) {
			SetLineColor_process_table[index].method();
			break;
		}
		index++;
	}
	report_error(ERR5);
	*/
}


void set_door_color(uint8 side, uint8 color)
{
	
	switch (side) {
	case DOOR_this:
		Set_led_color(dev_info.this_door, color);
		break;
		
	case DOOR_opposite:
		Set_led_color(dev_info.opposite_door, color);
		break;
		
	default:
		report_error(ERR6);
		break;

	}
}

//end1

/*********************************************************************
��������:	ˢ���ŵ�״̬
����˵��: 	--
ȫ�ֱ�����--
���ú�����--
ע������:	������Ӧ���Ų���
����˵��:	--
**********************************************************************/
void LED_door_process(void)
{
	switch(dev_info.dev_status.open_door_side) {
	case OPEN_THIS_DOOR:
		set_door_color(DOOR_this, COLOR_GREEN);
		set_door_color(DOOR_opposite, COLOR_NONE);
		break;
		
	case OPEN_OPPOSITE_DOOR:
		set_door_color(DOOR_this, COLOR_NONE);
		set_door_color(DOOR_opposite, COLOR_RED);
		break;
		
	case OPEN_TWOSIDE_DOOR:
		set_door_color(DOOR_this, COLOR_GREEN);
		set_door_color(DOOR_opposite, COLOR_RED);
		break;

	case OPEN_DOOR_STATUS_INVALID:
		//set_door_color(DOOR_this, COLOR_NONE);
		//set_door_color(DOOR_opposite, COLOR_NONE);
		break;

	default:
		break;
	}

	switch(dev_info.dev_status.close_door_side) {
	case CLOSE_THIS_DOOR:
		set_door_color(DOOR_this, COLOR_NONE);
		break;
		
	case CLOSE_OPPOSITE_DOOR:
		set_door_color(DOOR_opposite, COLOR_NONE);
		break;
		
	case CLOSE_TWOSIDE_DOOR:
		set_door_color(DOOR_this, COLOR_NONE);
		set_door_color(DOOR_opposite, COLOR_NONE);
		break;

	case CLOSE_DOOR_STATUS_INVALID:
		//set_door_color(DOOR_this, COLOR_NONE);
		//set_door_color(DOOR_opposite, COLOR_NONE);
		break;

	default:
		break;
	}
}


// ����tj23 ��ʽ����
#if 1
// ���������Է�ʽ����
void switch_up_down_process_1(void)
{
	static uint8	led_tmp = 0;
	uint8	i;
	uint8	start_sta, end_sta, next_sta, cur_sta;
	uint8	station_sum, direction;
	DIRECTION_STATUS up_direct;

	up_direct = dev_info.dev_config.up_direction;
	direction = dev_info.dev_status.up_down_flag;
	station_sum = dev_info.dev_config.station_quantity;
	cur_sta	= dev_info.dev_status.cur_station_num;
	start_sta	= dev_info.dev_status.start_station_num;
	end_sta	= dev_info.dev_status.end_station_num;
	next_sta	= dev_info.dev_status.next_station_num;

	led_tmp ^= 1;

	//if (cur_sta != start_sta) {
	//	report_error(ERR9);
	//	return;
	//}

	if (direction == TRAIN_DOWN) {
		//����
		if ((start_sta > station_sum) || (end_sta < 1) || (cur_sta < next_sta) || (cur_sta > start_sta) || (cur_sta < end_sta) || (end_sta >= start_sta) || (next_sta < end_sta))
		{
			report_error(ERR10);
			return;
		}
		
		set_hc595_buf(0xff);		//���buf
		
		//============1.��ǰվ��վ�����˸�̵�
		if(led_tmp) {
			set_line_color(cur_sta, TYPE_station, COLOR_GREEN);
		}
		else {
			set_line_color(cur_sta, TYPE_station, COLOR_NONE);
		}

		//============2.��һվ���յ�վ��վ��� �����

		if (cur_sta == next_sta) {	//��վʱ�л�������
			for (i = end_sta; i < next_sta; i++) {
				set_line_color(i, TYPE_station, COLOR_RED);
			}
		}
		else {
			for (i = end_sta; i <= next_sta; i++) {
				set_line_color(i, TYPE_station, COLOR_RED);
			}
		}
		
		
		//============3. ��ǰվ����һվ�����м�ͷ���̵�
		//for (i = next_sta-1; i < cur_sta; i++) {
		//	Set_led_color(station_index[i-1].down_directive_led, COLOR_GREEN);
		//}
		if (cur_sta > end_sta) {
			if (up_direct == RtoL) {
				if (cur_sta == next_sta) {
					set_line_color(cur_sta, TYPE_R, COLOR_GREEN);
				}
				else {
					for (i = next_sta + 1; i <= cur_sta; i++) {
						set_line_color(i, TYPE_R, COLOR_GREEN);
					}
				}
				
			}
			else if (up_direct == LtoR) {
				if (cur_sta == next_sta) {
					set_line_color(cur_sta, TYPE_L, COLOR_GREEN);
				}
				else {
					for (i = next_sta + 1; i <= cur_sta; i++) {
						set_line_color(i, TYPE_L, COLOR_GREEN);
					}
				}
			}
			else {
				report_error(ERR12);
			}
			
		}
	}
	else if (direction == TRAIN_UP){
		//����
		if ((end_sta > station_sum) || (start_sta < 1) || (cur_sta > next_sta) || (cur_sta > end_sta) || (cur_sta < start_sta) || (end_sta <= start_sta) || (next_sta > end_sta))
		{
			report_error(ERR13);
			return;
		}

		set_hc595_buf(0xff);		//���buf
		
		//============1.��ǰվ��վ�����˸�̵�
		if(led_tmp) {
			set_line_color(cur_sta, TYPE_station, COLOR_GREEN);
		}
		else {
			set_line_color(cur_sta, TYPE_station, COLOR_NONE);
		}

		//============2.��һվ���յ�վ��վ��� �����
		if (cur_sta == next_sta) {	//��վʱ�л�������
			for (i = next_sta+1; i <= end_sta ; i++) {
				set_line_color(i, TYPE_station, COLOR_RED);
			}
		}
		else {
			for (i = next_sta; i <= end_sta; i++) {
				set_line_color(i, TYPE_station, COLOR_RED);
			}
		}
		
		//============3. ��ǰվ����һվ�����м�ͷ���̵�
		//for (i = cur_sta; i < next_sta; i++) {
		//	Set_led_color(station_index[i-1].up_directive_led, COLOR_GREEN);
		//}
		if (cur_sta < end_sta) {
			if (up_direct == RtoL) {
				if (cur_sta == next_sta) {
					set_line_color(cur_sta, TYPE_L, COLOR_GREEN);
				}
				else {
					for (i = cur_sta; i < next_sta; i++) {
						set_line_color(i, TYPE_L, COLOR_GREEN);
					}
				}
				
			}
			else if (up_direct == LtoR) {
				if (cur_sta == next_sta) {
					set_line_color(cur_sta, TYPE_R, COLOR_GREEN);
				}
				else {
					for (i = cur_sta; i < next_sta; i++) {
						set_line_color(i, TYPE_R, COLOR_GREEN);
					}
				}
			}
			else {
				report_error(ERR14);
			}
			
		}

	}
	else {
		report_error(ERR11);
	}

}

void arrive_status_process_1(void)
{
	static uint8	led_tmp = 0;
	uint8	i;
	uint8	start_sta, end_sta, next_sta, cur_sta;
	uint8	station_sum, direction;
	DIRECTION_STATUS up_direct;

	up_direct = dev_info.dev_config.up_direction;
	direction = dev_info.dev_status.up_down_flag;
	station_sum = dev_info.dev_config.station_quantity;
	cur_sta	= dev_info.dev_status.cur_station_num;
	start_sta	= dev_info.dev_status.start_station_num;
	end_sta	= dev_info.dev_status.end_station_num;
	next_sta	= dev_info.dev_status.next_station_num;

	if (cur_sta != next_sta) {
		//��վʱ
		//cur_sta +/-n
		//next_sta ����
		report_error(ERR49);
		return;
	}

	led_tmp ^= 1;
	
	if (direction == TRAIN_DOWN) {
		//����
		if ((start_sta > station_sum) || (end_sta < 1) || (cur_sta < next_sta) || (cur_sta > start_sta) || (cur_sta < end_sta) || (end_sta >= start_sta) || (next_sta < end_sta))
		{
			report_error(ERR16);
			return;
		}
		
		set_hc595_buf(0xff);		//���buf
		
		//============1.��ǰվ��վ�����˸�̵�
		if(led_tmp) {
			set_line_color(cur_sta, TYPE_station, COLOR_GREEN);
		}
		else {
			set_line_color(cur_sta, TYPE_station, COLOR_NONE);
		}

		//============2.��һվ���յ�վ��վ��� �����
		for (i = end_sta; i < cur_sta; i++) {
			set_line_color(i, TYPE_station, COLOR_RED);
		}
		
		//============3. ��ǰվ����һվ�����м�ͷ���̵�
		if (cur_sta > end_sta) {
			if (up_direct == RtoL) {
				set_line_color(cur_sta, TYPE_R, COLOR_GREEN);
			}
			else if (up_direct == LtoR) {
				set_line_color(cur_sta, TYPE_L, COLOR_GREEN);
			}
			else {
				report_error(ERR17);
			}
			
		}
	}
	else if (direction == TRAIN_UP) {
		//����
		if ((end_sta > station_sum) || (start_sta < 1) || (cur_sta > next_sta) || (cur_sta > end_sta) || (cur_sta < start_sta) || (end_sta <= start_sta) || (next_sta > end_sta))
		{
			report_error(ERR18);
			return;
		}

		set_hc595_buf(0xff);		//���buf
		
		//============1.��ǰվ��վ�����˸�̵�
		if(led_tmp) {
			set_line_color(cur_sta, TYPE_station, COLOR_GREEN);
		}
		else {
			set_line_color(cur_sta, TYPE_station, COLOR_NONE);
		}

		//============2.��һվ���յ�վ��վ��� �����
		for (i = cur_sta + 1; i <= end_sta; i++) {
			set_line_color(i, TYPE_station, COLOR_RED);
		}
		
		//============3. ��ǰվ����һվ�����м�ͷ���̵�
		if (cur_sta < end_sta) {
			if (up_direct == RtoL) {
				set_line_color(cur_sta, TYPE_L, COLOR_GREEN);
			}
			else if (up_direct == LtoR) {
				set_line_color(cur_sta, TYPE_R, COLOR_GREEN);
			}
			else {
				report_error(ERR19);
			}
		}

	}
	else {
		report_error(ERR15);
	}

}


void departure_status_process_1(void)
{
	static uint8	led_tmp = 0;
	uint8	i;
	uint8	start_sta, end_sta, next_sta, cur_sta;
	uint8	station_sum, direction;
	DIRECTION_STATUS up_direct;

	up_direct = dev_info.dev_config.up_direction;
	direction = dev_info.dev_status.up_down_flag;
	station_sum = dev_info.dev_config.station_quantity;
	cur_sta	= dev_info.dev_status.cur_station_num;
	start_sta	= dev_info.dev_status.start_station_num;
	end_sta	= dev_info.dev_status.end_station_num;
	next_sta	= dev_info.dev_status.next_station_num;

	led_tmp ^= 1;

	if (cur_sta == next_sta) {
		//��վʱ
		//cur_sta ����
		//next_sta +/-n
		report_error(ERR50);
		return;
	}

	if (direction == TRAIN_DOWN) {
		//����
		if ((start_sta > station_sum) || (end_sta < 1) || (cur_sta < next_sta) || (cur_sta > start_sta) || (cur_sta < end_sta) || (end_sta >= start_sta) || (next_sta < end_sta))
		{
			report_error(ERR21);
			return;
		}
		
		set_hc595_buf(0xff);		//���buf
		
		//============1.��վվ�������ɫ
		//============2.����һվ���յ�վ��վ��� �����
		for (i = end_sta; i <= next_sta; i++) {
			if (i == next_sta) 
				set_line_color(i, TYPE_station, COLOR_GREEN);
			else
				set_line_color(i, TYPE_station, COLOR_RED);
		}
		
		//============3. ��ǰվ����һվ�����м�ͷ��˸�̵�
		for (i = next_sta + 1; i <= cur_sta; i++) {
			if (up_direct == RtoL) {
				if (led_tmp) {
					set_line_color(i, TYPE_R, COLOR_GREEN);
				}
				else {
					set_line_color(i, TYPE_R, COLOR_NONE);
				}
			}
			else if (up_direct == LtoR) {
				if (led_tmp) {
					set_line_color(i, TYPE_L, COLOR_GREEN);
				}
				else {
					set_line_color(i, TYPE_L, COLOR_NONE);
				}
			}
			else {
				report_error(ERR22);
			}
		}
	}
	else if (direction == TRAIN_UP) {
		//����
		if ((end_sta > station_sum) || (start_sta < 1) || (cur_sta > next_sta) || (cur_sta > end_sta) || (cur_sta < start_sta) || (end_sta <= start_sta) || (next_sta > end_sta))
		{
			report_error(ERR23);
			return;
		}

		set_hc595_buf(0xff);		//���buf
		
		//============1.��վվ�������ɫ
		//============2.����һվ���յ�վ��վ��� �����
		for (i = next_sta; i <= end_sta; i++) {
			if (i == next_sta) 
				set_line_color(i, TYPE_station, COLOR_GREEN);
			else
				set_line_color(i, TYPE_station, COLOR_RED);
		}
		
		//============3. ��ǰվ����һվ�����м�ͷ��˸�̵�
		for (i = cur_sta; i < next_sta; i++) {
			if (up_direct == RtoL) {
				if (led_tmp) {
					set_line_color(i, TYPE_L, COLOR_GREEN);
				}
				else {
					set_line_color(i, TYPE_L, COLOR_NONE);
				}
			}
			else if (up_direct == LtoR) {
				if (led_tmp) {
					set_line_color(i, TYPE_R, COLOR_GREEN);
				}
				else {
					set_line_color(i, TYPE_R, COLOR_NONE);
				}
			}
			else {
				report_error(ERR24);
			}
		}
	}
	else {
		report_error(ERR20);
	}

}

#else
void switch_up_down_process_1(void)
{
	static uint8	led_tmp = 0;
	uint8	i;
	uint8	start_sta, end_sta, next_sta, cur_sta;
	uint8	station_sum, direction;
	DIRECTION_STATUS up_direct;

	up_direct = dev_info.dev_config.up_direction;
	direction = dev_info.dev_status.up_down_flag;
	station_sum = dev_info.dev_config.station_quantity;
	cur_sta	= dev_info.dev_status.cur_station_num;
	start_sta	= dev_info.dev_status.start_station_num;
	end_sta	= dev_info.dev_status.end_station_num;
	next_sta	= dev_info.dev_status.next_station_num;

	led_tmp ^= 1;

	if (cur_sta != start_sta) {
		report_error(ERR9);
		return;
	}

	if (direction == TRAIN_DOWN) {
		//����
		if ((start_sta > station_sum) || (end_sta < 1) || (cur_sta < next_sta) || (cur_sta > start_sta) || (cur_sta < end_sta) || (end_sta >= start_sta) || (next_sta < end_sta))
		{
			report_error(ERR10);
			return;
		}
		
		set_hc595_buf(0xff);		//���buf
		
		//============1.��ǰվ��վ�����˸�̵�
		if(led_tmp) {
			set_line_color(cur_sta, TYPE_station, COLOR_GREEN);
		}
		else {
			set_line_color(cur_sta, TYPE_station, COLOR_NONE);
		}

		//============2.��һվ���յ�վ��վ��� �����
		for (i = end_sta; i <= next_sta; i++) {
			set_line_color(i, TYPE_station, COLOR_RED);
		}
		
		//============3. ��ǰվ����һվ�����м�ͷ���̵�
		//for (i = next_sta-1; i < cur_sta; i++) {
		//	Set_led_color(station_index[i-1].down_directive_led, COLOR_GREEN);
		//}
		if (cur_sta > end_sta) {
			if (up_direct == RtoL) {
				set_line_color(cur_sta, TYPE_R, COLOR_GREEN);
			}
			else if (up_direct == LtoR) {
				set_line_color(cur_sta, TYPE_L, COLOR_GREEN);
			}
			else {
				report_error(ERR12);
			}
			
		}
	}
	else if (direction == TRAIN_UP){
		//����
		if ((end_sta > station_sum) || (start_sta < 1) || (cur_sta > next_sta) || (cur_sta > end_sta) || (cur_sta < start_sta) || (end_sta <= start_sta) || (next_sta > end_sta))
		{
			report_error(ERR13);
			return;
		}

		set_hc595_buf(0xff);		//���buf
		
		//============1.��ǰվ��վ�����˸�̵�
		if(led_tmp) {
			set_line_color(cur_sta, TYPE_station, COLOR_GREEN);
		}
		else {
			set_line_color(cur_sta, TYPE_station, COLOR_NONE);
		}

		//============2.��һվ���յ�վ��վ��� �����
		for (i = next_sta; i <= end_sta; i++) {
			set_line_color(i, TYPE_station, COLOR_RED);
		}
		//============3. ��ǰվ����һվ�����м�ͷ���̵�
		//for (i = cur_sta; i < next_sta; i++) {
		//	Set_led_color(station_index[i-1].up_directive_led, COLOR_GREEN);
		//}
		if (cur_sta < end_sta) {
			if (up_direct == RtoL) {
				set_line_color(cur_sta, TYPE_L, COLOR_GREEN);
			}
			else if (up_direct == LtoR) {
				set_line_color(cur_sta, TYPE_R, COLOR_GREEN);
			}
			else {
				report_error(ERR14);
			}
		}

	}
	else {
		report_error(ERR11);
	}

}

void arrive_status_process_1(void)
{
	static uint8	led_tmp = 0;
	uint8	i;
	uint8	start_sta, end_sta, next_sta, cur_sta;
	uint8	station_sum, direction;
	DIRECTION_STATUS up_direct;

	up_direct = dev_info.dev_config.up_direction;
	direction = dev_info.dev_status.up_down_flag;
	station_sum = dev_info.dev_config.station_quantity;
	cur_sta	= dev_info.dev_status.cur_station_num;
	start_sta	= dev_info.dev_status.start_station_num;
	end_sta	= dev_info.dev_status.end_station_num;
	next_sta	= dev_info.dev_status.next_station_num;

	led_tmp ^= 1;
	
	if (direction == TRAIN_DOWN) {
		//����
		if ((start_sta > station_sum) || (end_sta < 1) || (cur_sta < next_sta) || (cur_sta > start_sta) || (cur_sta < end_sta) || (end_sta >= start_sta) || (next_sta < end_sta))
		{
			report_error(ERR16);
			return;
		}
		
		set_hc595_buf(0xff);		//���buf
		
		//============1.��ǰվ��վ�����˸�̵�
		//��ʱ�ĵ�ǰվ=�ϴ���վʱ��next_sta
		if(led_tmp) {
			set_line_color(next_sta, TYPE_station, COLOR_GREEN);
		}
		else {
			set_line_color(next_sta, TYPE_station, COLOR_NONE);
		}

		//============2.��һվ���յ�վ��վ��� �����
		//��ʱ����һվ=�ϴ���վʱ��next_sta-1
		for (i = end_sta; i < next_sta; i++) {
			set_line_color(i, TYPE_station, COLOR_RED);
		}
		
		//============3. ��ǰվ����һվ�����м�ͷ���̵�
		//��ʱ�ĵ�ǰվ=�ϴ���վʱ��next_sta
		if (next_sta > end_sta) {
			if (up_direct == RtoL) {
				set_line_color(next_sta, TYPE_R, COLOR_GREEN);
			}
			else if (up_direct == LtoR) {
				set_line_color(next_sta, TYPE_L, COLOR_GREEN);
			}
			else {
				report_error(ERR17);
			}
		}
	}
	else if (direction == TRAIN_UP) {
		//����
		if ((end_sta > station_sum) || (start_sta < 1) || (cur_sta > next_sta) || (cur_sta > end_sta) || (cur_sta < start_sta) || (end_sta <= start_sta) || (next_sta > end_sta))
		{
			report_error(ERR18);
			return;
		}

		set_hc595_buf(0xff);		//���buf
		
		//============1.��ǰվ��վ�����˸�̵�
		//��ʱ�ĵ�ǰվ=�ϴ���վʱ��next_sta
		if(led_tmp) {
			set_line_color(next_sta, TYPE_station, COLOR_GREEN);
		}
		else {
			set_line_color(next_sta, TYPE_station, COLOR_NONE);
		}

		//============2.��һվ���յ�վ��վ��� �����
		//��ʱ����һվ=�ϴ���վʱ��next_sta+1
		for (i = next_sta + 1; i <= end_sta; i++) {
			set_line_color(i, TYPE_station, COLOR_RED);
		}
		//============3. ��ǰվ����һվ�����м�ͷ���̵�
		//��ʱ�ĵ�ǰվ=�ϴ���վʱ��next_sta
		if (next_sta < end_sta) {
			if (up_direct == RtoL) {
				set_line_color(next_sta, TYPE_L, COLOR_GREEN);
			}
			else if (up_direct == LtoR) {
				set_line_color(next_sta, TYPE_R, COLOR_GREEN);
			}
			else {
				report_error(ERR19);
			}
		}

	}
	else {
		report_error(ERR15);
	}

}


void departure_status_process_1(void)
{
	static uint8	led_tmp = 0;
	uint8	i;
	uint8	start_sta, end_sta, next_sta, cur_sta;
	uint8	station_sum, direction;
	DIRECTION_STATUS up_direct;

	up_direct = dev_info.dev_config.up_direction;
	direction = dev_info.dev_status.up_down_flag;
	station_sum = dev_info.dev_config.station_quantity;
	cur_sta	= dev_info.dev_status.cur_station_num;
	start_sta	= dev_info.dev_status.start_station_num;
	end_sta	= dev_info.dev_status.end_station_num;
	next_sta	= dev_info.dev_status.next_station_num;

	led_tmp ^= 1;

	if (direction == TRAIN_DOWN) {
		//����
		if ((start_sta > station_sum) || (end_sta < 1) || (cur_sta < next_sta) || (cur_sta > start_sta) || (cur_sta < end_sta) || (end_sta >= start_sta) || (next_sta < end_sta))
		{
			report_error(ERR21);
			return;
		}
		
		set_hc595_buf(0xff);		//���buf
		
		//============1.��վվ�������ɫ
		//============2.����һվ���յ�վ��վ��� �����
		for (i = end_sta; i <= next_sta; i++) {
			if (i == next_sta) 
				set_line_color(i, TYPE_station, COLOR_GREEN);
			else
				set_line_color(i, TYPE_station, COLOR_RED);
		}
		//============3. ��ǰվ����һվ�����м�ͷ��˸�̵�
		for (i = next_sta + 1; i <= cur_sta; i++) {
			if (up_direct == RtoL) {
				if (led_tmp) {
					set_line_color(i, TYPE_R, COLOR_GREEN);
				}
				else {
					set_line_color(i, TYPE_R, COLOR_NONE);
				}
			}
			else if (up_direct == LtoR) {
				if (led_tmp) {
					set_line_color(i, TYPE_L, COLOR_GREEN);
				}
				else {
					set_line_color(i, TYPE_L, COLOR_NONE);
				}
			}
			else {
				report_error(ERR22);
			}
		}
	}
	else if (direction == TRAIN_UP) {
		//����
		if ((end_sta > station_sum) || (start_sta < 1) || (cur_sta > next_sta) || (cur_sta > end_sta) || (cur_sta < start_sta) || (end_sta <= start_sta) || (next_sta > end_sta))
		{
			report_error(ERR23);
			return;
		}

		set_hc595_buf(0xff);		//���buf
		
		//============1.��վվ�������ɫ
		//============2.����һվ���յ�վ��վ��� �����
		for (i = next_sta; i <= end_sta; i++) {
			if (i == next_sta) 
				set_line_color(i, TYPE_station, COLOR_GREEN);
			else
				set_line_color(i, TYPE_station, COLOR_RED);
		}
		//============3. ��ǰվ����һվ�����м�ͷ��˸�̵�
		for (i = cur_sta; i < next_sta; i++) {
			if (up_direct == RtoL) {
				if (led_tmp) {
					set_line_color(i, TYPE_L, COLOR_GREEN);
				}
				else {
					set_line_color(i, TYPE_L, COLOR_NONE);
				}
			}
			else if (up_direct == LtoR) {
				if (led_tmp) {
					set_line_color(i, TYPE_R, COLOR_GREEN);
				}
				else {
					set_line_color(i, TYPE_R, COLOR_NONE);
				}
			}
			else {
				report_error(ERR24);
			}
		}
	}
	else {
		report_error(ERR20);
	}

}
#endif


typedef void (*dev_Status_process_Callback)(void);

typedef struct
{
	uint8						method_index;
	dev_Status_process_Callback	dev_status_refresh;
}DEV_STATUS_PROCESS_T;

const DEV_STATUS_PROCESS_T  switch_up_down_process_table[] = {
	{	1,				switch_up_down_process_1		},
	{	null,				null								}
};

const DEV_STATUS_PROCESS_T  arrive_status_process_table[] = {
	{	1,				arrive_status_process_1			},
	{	null,				null								}
};

const DEV_STATUS_PROCESS_T  depart_status_process_table[] = {
	{	1,				departure_status_process_1		},
	{	null,				null								}
};


void LED_switch_up_down_process(void)
{
	uint8	index;

	index = 0;
	while (switch_up_down_process_table[index].dev_status_refresh) {
		if (switch_up_down_process_table[index].method_index == 1) {
			switch_up_down_process_table[index].dev_status_refresh();
			break;
		}
		index++;
	}
}

void LED_arrive_status_process(void)
{
	uint8	index;

	index = 0;
	while (arrive_status_process_table[index].dev_status_refresh) {
		if (arrive_status_process_table[index].method_index == 1) {
			arrive_status_process_table[index].dev_status_refresh();
			break;
		}
		index++;
	}
}

void LED_departure_status_process(void)
{
	uint8	index;

	index = 0;
	while (depart_status_process_table[index].dev_status_refresh) {
		if (depart_status_process_table[index].method_index == 1) {
			depart_status_process_table[index].dev_status_refresh();
			break;
		}
		index++;
	}
}

typedef void (*Led_Status_process_Callback)(void);

typedef struct {
	TRAIN_STATUS				status;
	Led_Status_process_Callback	led_status_refresh;
}LED_STATUS_PROCESS_T;

const LED_STATUS_PROCESS_T  led_status_process_table[] = {
	
	{	SWITCH_UP_DOWN_STATUS,	LED_switch_up_down_process			},
	{	ARRIVE_STATUS,				LED_arrive_status_process			},		
	{	DEPARTURE_STATUS,			LED_departure_status_process			},
	{	(TRAIN_STATUS)null,			null									}
};

void LED_status_process(void)
{
	uint8	index, len;
	static uint32 led_status_process_timer = 0;

	len = hc595_info.hc595_config.buf_size;
	
	if(timer0_check_timer(&led_status_process_timer, 500)) {
		index = 0;
		while(led_status_process_table[index].led_status_refresh) {
			if(led_status_process_table[index].status == dev_info.dev_status.status) {
				led_status_process_table[index].led_status_refresh();
				LED_door_process();
				Refresh_74HC595_status(hc595_info.green_led, hc595_info.red_led, len);
				break;
			}
			index++;
		}
	}
}


/*********************************************************************
��������:	��ȡhc595��������Ϣ
����˵��: 	TAG_74HC595_CONFIG *cfg_info
ȫ�ֱ�����--
���ú�����--
ע������:	�������պ궨�巽ʽ��ȡ
����˵��:	--
**********************************************************************/
void read_hc595_config_info(TAG_74HC595_CONFIG *cfg_info)
{
	uint8	tmp1, tmp2;

	//-------------------------------------
	cfg_info->led_map_quantity = led_map_SUM;
	cfg_info->hc595_quantity = HC595_NUM;

	//-------------------------------------
	tmp1 = cfg_info->hc595_quantity;
	tmp2 = cfg_info->led_map_quantity;
	cfg_info->buf_size = tmp1 * tmp2;
}

/*********************************************************************
��������:	����HC595�Ļ���������Ϊ��ֵͬ
				(��:ָʾ����ɫ)
����˵��: 	uint8 ch
ȫ�ֱ�����--
���ú�����--
ע������:	--
����˵��:	--
**********************************************************************/
void set_hc595_buf(uint8 ch)
{
	mem_set((uint8 *)hc595_info.green_led, ch, hc595_info.hc595_config.buf_size);
	mem_set((uint8 *)hc595_info.red_led, ch, hc595_info.hc595_config.buf_size);
}

/*********************************************************************
��������:	HC595��ʼ��
����˵��: 	--
ȫ�ֱ�����--
���ú�����--
ע������:	����ʼ����ʱ����һ��
����˵��:	--
**********************************************************************/
void init_hc595_info(void)
{

	mem_set((uint8 *)&hc595_info, 0, sizeof(TAG_74HC595_INFO));
	
	read_hc595_config_info(&hc595_info.hc595_config);
	
	hc595_info.green_led = (uint8 *)malloc(hc595_info.hc595_config.buf_size);
	hc595_info.red_led = (uint8 *)malloc(hc595_info.hc595_config.buf_size);

	set_hc595_buf(0xff);
	Refresh_74HC595_status(hc595_info.green_led, hc595_info.red_led, hc595_info.hc595_config.buf_size);
}

/*********************************************************************
��������:	��ȡ��̬��ͼ��������Ϣ
����˵��: 	DEV_CONFIG_T *cfg_info
ȫ�ֱ�����--
���ú�����--
ע������:	�������պ궨�巽ʽ��ȡ
����˵��:	--
**********************************************************************/
void read_dev_config_info(DEV_CONFIG_T *cfg_info)
{
	uint8	tmp1;

	//-------------------------------------
	cfg_info->station_quantity = STATION_SUM;
	cfg_info->first_led_location.char_num = FIRST_STATION_charNum;
	cfg_info->first_led_location.bit_num = FIRST_STATION_bitNum;
	cfg_info->this_door_charNum = ThisDoor_charNum;
	cfg_info->this_door_bitNum = ThisDoor_bitNum;
	cfg_info->opposite_door_charNum = OppositeDoor_charNum;
	cfg_info->opposite_door_bitNum = OppositeDoor_bitNum;
	

	tmp1 = UP_DIRECTION;
	if (tmp1 == R_TO_L) {
		cfg_info->up_direction = RtoL;
	}
	else if (tmp1 == L_TO_R) {
		cfg_info->up_direction = LtoR;
	}
	else {
		report_error(ERR1);
	}

	tmp1 = MAP_SIDE;
	if (tmp1 == RIGHT) {
		cfg_info->map_side = SIDE_RIGHT;
	}
	else if (tmp1 == LEFT) {
		cfg_info->map_side = SIDE_LEFT;
	}
	else {
		report_error(ERR25);
	}

	tmp1 = ARROW_STYLE;
	if (tmp1 == ARROW_converge) {
		cfg_info->arrow_sty = ARROW_STYLE_converge;
	}
	else if (tmp1 == ARROW_diverge) {
		cfg_info->arrow_sty = ARROW_STYLE_diverge;
	}
	else {
		report_error(ERR28);
	}
	
	
}

void set_line_location_diverge(void)
{
	uint16	station_sum;
	uint16	i, j, n;
	uint16	first_bit_num, need_bit_sum, actual_bit_sum, bit_sum;

	station_sum = dev_info.dev_config.station_quantity;

	//�м�ÿ����վ��Ҫ3��led������յ����2��led
	need_bit_sum = 3 * (station_sum - 1) + 1;
	
	//����ʼλ�ÿ�ʼ���ж���led�ɹ���·ʹ��
	//(48 ~ 55) + 56 *n��λ�ò�����
	bit_sum = hc595_info.hc595_config.buf_size;
	bit_sum *= 8;		//���е�hc595���ж���λ
	//��ʼλ��
	first_bit_num = dev_info.dev_config.first_led_location.char_num;
	first_bit_num = first_bit_num * 8 + dev_info.dev_config.first_led_location.bit_num;
	
	actual_bit_sum = 0;
	for (i = first_bit_num; i < bit_sum; i++) {

		if ((i >= (48 + 56 * (i/56)) ) && (i <= (55 + 56 * (i/56)) )) {
			continue;
		}
		actual_bit_sum++;
	}
	
	if (actual_bit_sum < need_bit_sum) {
		//���λ�ÿ�ʼʵ�ʵ�led ������Ҫ��led
		report_error(ERR35);
		return;
	}
	

	//PCB �ϵ�led ��Ӧ��hc595��ַ����Ϊ:  
	// L: �ߵ�ַ��λ; R: �͵�ַ��λ
	//n.7  ... n.0 | (n-1).7 ....(n-1).0 | .......| 0.7 ... 0.0
	if (dev_info.dev_config.up_direction == RtoL) {
		//վ����С�����ʼ����led��ַ������С�������

		dev_info.line_index[0].r_direction_led.char_num = LED_NONE;
		dev_info.line_index[0].r_direction_led.bit_num = LED_NONE;
		dev_info.line_index[station_sum-1].l_direction_led.char_num = LED_NONE;
		dev_info.line_index[station_sum-1].l_direction_led.bit_num = LED_NONE;
		
		j = 0;
		for (i = first_bit_num; i < bit_sum; i++) {
			if ((i >= (48 + 56 * (i/56)) ) && (i <= (55 + 56 * (i/56)) )) {
				continue;
			}

			if (!(j%3)) {
				//sta
				n = j/3;
				dev_info.line_index[n].station_index = n+1;
				dev_info.line_index[n].station_led.char_num = i/8;
				dev_info.line_index[n].station_led.bit_num = i%8;
			}
			else if ((j >= 2) && (!((j - 2)%3)) ) {
				//r_direct
				n = (j - 2)/3 + 1;
				dev_info.line_index[n].r_direction_led.char_num = i/8;
				dev_info.line_index[n].r_direction_led.bit_num = i%8;
			}
			else if ((j >= 1) && (!((j - 1)%3)) ) {
				//l_direct
				n = (j - 1)/3;
				dev_info.line_index[n].l_direction_led.char_num = i/8;
				dev_info.line_index[n].l_direction_led.bit_num = i%8;
			}
			else {
				report_error(ERR36);
			}
			j++;
			if (j == need_bit_sum) {
				break;
			}
		}
	}
	else if (dev_info.dev_config.up_direction == LtoR) {
		//վ����С�����ʼ����led��ַ�����ɴ�С����
		dev_info.line_index[station_sum-1].r_direction_led.char_num = LED_NONE;
		dev_info.line_index[station_sum-1].r_direction_led.bit_num = LED_NONE;
		dev_info.line_index[0].l_direction_led.char_num = LED_NONE;
		dev_info.line_index[0].l_direction_led.bit_num = LED_NONE;
		
		j = 0;
		for (i = first_bit_num; i < bit_sum; i++) {
			if ((i >= (48 + 56 * (i/56)) ) && (i <= (55 + 56 * (i/56)) )) {
				continue;
			}

			if (!(j%3)) {
				//sta
				n = station_sum - 1 - j/3;
				dev_info.line_index[n].station_index = n+1;
				dev_info.line_index[n].station_led.char_num = i/8;
				dev_info.line_index[n].station_led.bit_num = i%8;
			}
			else if ((j >= 2) && (!((j - 2)%3)) ) {
				//r_direct
				n = station_sum - 2 - (j - 2)/3;
				dev_info.line_index[n].r_direction_led.char_num = i/8;
				dev_info.line_index[n].r_direction_led.bit_num = i%8;
			}
			else if ((j >= 1) && (!((j - 1)%3)) ) {
				//l_direct
				n = station_sum - 1 - (j - 1)/3;
				dev_info.line_index[n].l_direction_led.char_num = i/8;
				dev_info.line_index[n].l_direction_led.bit_num = i%8;
			}
			else {
				report_error(ERR37);
			}
			j++;
			if (j == need_bit_sum) {
				break;
			}
		}

	}
	else {
		report_error(ERR38);
	}
}

void set_line_location_converge(void)
{
	uint16	station_sum;
	uint16	i, j, n;
	uint16	first_bit_num, need_bit_sum, actual_bit_sum, bit_sum;

	station_sum = dev_info.dev_config.station_quantity;

	//�м�ÿ����վ��Ҫ3��led������յ����2��led
	need_bit_sum = 3 * (station_sum - 1) + 1;
	
	//����ʼλ�ÿ�ʼ���ж���led�ɹ���·ʹ��
	//(48 ~ 55) + 56 *n��λ�ò�����
	bit_sum = hc595_info.hc595_config.buf_size;
	bit_sum *= 8;		//���е�hc595���ж���λ
	//��ʼλ��
	first_bit_num = dev_info.dev_config.first_led_location.char_num;
	first_bit_num = first_bit_num * 8 + dev_info.dev_config.first_led_location.bit_num;
	
	actual_bit_sum = 0;
	for (i = first_bit_num; i < bit_sum; i++) {

		if ((i >= (48 + 56 * (i/56)) ) && (i <= (55 + 56 * (i/56)) )) {
			continue;
		}
		actual_bit_sum++;
	}
	
	if (actual_bit_sum < need_bit_sum) {
		//���λ�ÿ�ʼʵ�ʵ�led ������Ҫ��led
		report_error(ERR30);
		return;
	}
	

	//PCB �ϵ�led ��Ӧ��hc595��ַ����Ϊ:  
	// L: �ߵ�ַ��λ; R: �͵�ַ��λ
	//n.7  ... n.0 | (n-1).7 ....(n-1).0 | .......| 0.7 ... 0.0
	if (dev_info.dev_config.up_direction == RtoL) {
		//վ����С�����ʼ����led��ַ������С�������

		dev_info.line_index[0].r_direction_led.char_num = LED_NONE;
		dev_info.line_index[0].r_direction_led.bit_num = LED_NONE;
		dev_info.line_index[station_sum-1].l_direction_led.char_num = LED_NONE;
		dev_info.line_index[station_sum-1].l_direction_led.bit_num = LED_NONE;
		
		j = 0;
		for (i = first_bit_num; i < bit_sum; i++) {
			if ((i >= (48 + 56 * (i/56)) ) && (i <= (55 + 56 * (i/56)) )) {
				continue;
			}

			if (!(j%3)) {
				//sta
				n = j/3;
				dev_info.line_index[n].station_index = n+1;
				dev_info.line_index[n].station_led.char_num = i/8;
				dev_info.line_index[n].station_led.bit_num = i%8;
			}
			else if ((j >= 2) && (!((j - 2)%3)) ) {
				//l_direct
				n = (j - 2)/3;
				dev_info.line_index[n].l_direction_led.char_num = i/8;
				dev_info.line_index[n].l_direction_led.bit_num = i%8;
			}
			else if ((j >= 1) && (!((j - 1)%3)) ) {
				//r_direct
				n = (j - 1)/3 + 1;
				dev_info.line_index[n].r_direction_led.char_num = i/8;
				dev_info.line_index[n].r_direction_led.bit_num = i%8;
			}
			else {
				report_error(ERR31);
			}
			j++;
			if (j == need_bit_sum) {
				break;
			}
		}
	}
	else if (dev_info.dev_config.up_direction == LtoR) {
		//վ����С�����ʼ����led��ַ�����ɴ�С����
		dev_info.line_index[station_sum-1].r_direction_led.char_num = LED_NONE;
		dev_info.line_index[station_sum-1].r_direction_led.bit_num = LED_NONE;
		dev_info.line_index[0].l_direction_led.char_num = LED_NONE;
		dev_info.line_index[0].l_direction_led.bit_num = LED_NONE;
		
		j = 0;
		for (i = first_bit_num; i < bit_sum; i++) {
			if ((i >= (48 + 56 * (i/56)) ) && (i <= (55 + 56 * (i/56)) )) {
				continue;
			}

			if (!(j%3)) {
				//sta
				n = station_sum - 1 - j/3;
				dev_info.line_index[n].station_index = n+1;
				dev_info.line_index[n].station_led.char_num = i/8;
				dev_info.line_index[n].station_led.bit_num = i%8;
			}
			else if ((j >= 2) && (!((j - 2)%3)) ) {
				//l_direct
				n = station_sum - 1 - (j - 2)/3;
				dev_info.line_index[n].l_direction_led.char_num = i/8;
				dev_info.line_index[n].l_direction_led.bit_num = i%8;
			}
			else if ((j >= 1) && (!((j - 1)%3)) ) {
				//r_direct
				n = station_sum - 2 - (j - 1)/3;
				dev_info.line_index[n].r_direction_led.char_num = i/8;
				dev_info.line_index[n].r_direction_led.bit_num = i%8;
			}
			else {
				report_error(ERR33);
			}
			j++;
			if (j == need_bit_sum) {
				break;
			}
		}

	}
	else {
		report_error(ERR32);
	}
	
}

/*********************************************************************
��������:	
	��̬��ͼվ��λ����Ϣ�����Ų�λ����Ϣ�ȳ�ʼ��
����˵��: 	--
ȫ�ֱ�����--
���ú�����--
ע������:	����ʼ����ʱ����һ��
����˵��:	--
**********************************************************************/
void init_dev_info(void)
{
	uint16	len;
	uint16	station_sum;
	uint16	i;
	ARROW_STYLE_T style;
	
	mem_set((uint8 *)&dev_info, 0, sizeof(DEV_INFO_T));
	
	read_dev_config_info(&dev_info.dev_config);
	
	dev_info.dev_status.status = TRAIN_STATUS_INVALID;
	dev_info.dev_status.brightness = BRIGHTNESS_VALUE;
	dev_info.dev_status.up_down_flag = TRAIN_UP;

	//���Ų�λ����Ϣ����
	dev_info.this_door.char_num = dev_info.dev_config.this_door_charNum;
	dev_info.this_door.bit_num = dev_info.dev_config.this_door_bitNum;
	dev_info.opposite_door.char_num =dev_info.dev_config.opposite_door_charNum;
	dev_info.opposite_door.bit_num = dev_info.dev_config.opposite_door_bitNum;

	//��·λ����Ϣ����

	station_sum = dev_info.dev_config.station_quantity;

	if (station_sum < 2) {
		report_error(ERR34);
		return;
	}

	//�ȳ�վ���������һ����Ԫ��ʼ��Ϊnull,����������
	len = (station_sum + 1) * sizeof(LED_INDEX_T);

	dev_info.line_index = (LED_INDEX_T *)malloc(len);
	mem_set((uint8 *)dev_info.line_index, 0, len);

	for (i = 0; i < station_sum+1; i++) {
		dev_info.line_index[i].station_index = null;
		dev_info.line_index[i].l_direction_led.char_num = LED_NONE;
		dev_info.line_index[i].l_direction_led.bit_num = LED_NONE;
		dev_info.line_index[i].station_led.char_num = LED_NONE;
		dev_info.line_index[i].station_led.bit_num = LED_NONE;
		dev_info.line_index[i].r_direction_led.char_num = LED_NONE;
		dev_info.line_index[i].r_direction_led.bit_num = LED_NONE;
	}

	//���ݼ�ͷ�ķ�������·λ��
	style = dev_info.dev_config.arrow_sty;
	switch (style) {
	case ARROW_STYLE_converge:
		set_line_location_converge();
		break;
	case ARROW_STYLE_diverge:
		set_line_location_diverge();
		break;
	default:
		report_error(ERR29);
		break;
	}
	
}

/*********************************************************************
��������:	
	����ָʾ����Ѳһ��,�������е�led
����˵��: 	--
ȫ�ֱ�����--
���ú�����--
ע������:	--
����˵��:	--
**********************************************************************/
//LtoR
void LedTest_process_1(void)
{
	uint32	led_test_timer = 0;
	uint8	i, j;
	uint8	len;

	len = hc595_info.hc595_config.buf_size;
	
	//off
	set_hc595_buf(0xff);
	Refresh_74HC595_status(hc595_info.green_led, hc595_info.red_led, len);

	//sequence on
	for (i = 0; i < len; i++) {
		
		for(j = 0; j < 8; j ++) {
			while(!timer0_check_timer(&led_test_timer, LIGHT_LED_INTERVAL1))
				feed_dog();
			hc595_info.green_led[len -1 - i] &= ~(0x80 >> j);
			Refresh_74HC595_status(hc595_info.green_led, hc595_info.red_led, len);
		}
	}

	while(!timer0_check_timer(&led_test_timer, LIGHT_LED_INTERVAL1))
		feed_dog();
	
	set_hc595_buf(0xff);
	Refresh_74HC595_status(hc595_info.green_led, hc595_info.red_led, len);

	for (i = 0; i < len; i++) {
		
		for(j = 0; j < 8; j ++) {
			while(!timer0_check_timer(&led_test_timer, LIGHT_LED_INTERVAL1))
				feed_dog();
			hc595_info.red_led[len -1 - i] &= ~(0x80 >> j);
			Refresh_74HC595_status(hc595_info.green_led, hc595_info.red_led, len);
		}
	}

	while(!timer0_check_timer(&led_test_timer, LIGHT_LED_INTERVAL1))
		feed_dog();
	
	set_hc595_buf(0xff);
	Refresh_74HC595_status(hc595_info.green_led, hc595_info.red_led, len);
}

void LedTest_process_2_diverge(void)
{
	uint32	led_test_timer = 0;
	uint16	i, j, n;
	uint16	len, station_sum, cnt;
	uint8	color;


	len = hc595_info.hc595_config.buf_size;
	station_sum = dev_info.dev_config.station_quantity;
	cnt = (station_sum - 1) * 3 +1;	//sta + r+l = 3; sta =1;
	
	//off
	set_hc595_buf(0xff);
	Refresh_74HC595_status(hc595_info.green_led, hc595_info.red_led, len);

	if (dev_info.dev_config.up_direction == RtoL) {

	for (j = 0; j < 2; j ++) {
		if (!(j%2)) {
			color = COLOR_GREEN;
		}
		else {
			color = COLOR_RED;
		}
		for (i = 0; i < cnt; i++) {
			while(!timer0_check_timer(&led_test_timer, LIGHT_LED_INTERVAL2))
				feed_dog();

			set_door_color(DOOR_opposite, color);
			set_door_color(DOOR_this, color);
			/*
			if (i == cnt - 1) {
				//opposite door
				set_door_color(DOOR_opposite, color);

			}
			else if (i == cnt - 2) {
				//this door
				set_door_color(DOOR_this, color);
			}
			else 
			*/
			if (!(i%3)) {
				//sta
				n = i/3 + 1;
				set_line_color((uint8)n, TYPE_station, color);
			}
			else if ((i >= 1) && (!((i - 1)%3))) {
				//l_direct
				n = (i - 1)/3 + 1;
				set_line_color((uint8)n, TYPE_L, color);
			}
			else if ((i >= 2) && (!((i - 2)%3))) {
				//r_direct
				n = (i - 2)/3 + 2;
				set_line_color((uint8)n, TYPE_R, color);
			}
			else {
				report_error(ERR41);
			}
			
			Refresh_74HC595_status(hc595_info.green_led, hc595_info.red_led, len);
		}

		while(!timer0_check_timer(&led_test_timer, LIGHT_LED_INTERVAL2))
			feed_dog();
		set_hc595_buf(0xff);
		Refresh_74HC595_status(hc595_info.green_led, hc595_info.red_led, len);
	}
	}
	else if (dev_info.dev_config.up_direction == LtoR) {
	for (j = 0; j < 2; j ++) {
		if (!(j%2)) {
			color = COLOR_GREEN;
		}
		else {
			color = COLOR_RED;
		}
		for (i = 0; i < cnt; i++) {
			while(!timer0_check_timer(&led_test_timer, LIGHT_LED_INTERVAL2))
				feed_dog();

			set_door_color(DOOR_opposite, color);
			set_door_color(DOOR_this, color);
			/*
			if (i == cnt - 1) {
				//opposite door
				set_door_color(DOOR_opposite, color);

			}
			else if (i == cnt - 2) {
				//this door
				set_door_color(DOOR_this, color);
			}
			else 
			*/

			if (!(i%3)) {
				//sta
				n = i/3 + 1;
				set_line_color((uint8)n, TYPE_station, color);
			}
			else if ((i >= 1) && (!((i - 1)%3))) {
				//r_direct
				n = (i - 1)/3 + 1;
				set_line_color((uint8)n, TYPE_R, color);
			}
			else if ((i >= 2) && (!((i - 2)%3))) {
				//l_direct
				n = (i - 2)/3 + 2;
				set_line_color((uint8)n, TYPE_L, color);
			}
			else {
				report_error(ERR42);
			}
			
			Refresh_74HC595_status(hc595_info.green_led, hc595_info.red_led, len);
		}

		while(!timer0_check_timer(&led_test_timer, LIGHT_LED_INTERVAL2))
			feed_dog();
		set_hc595_buf(0xff);
		Refresh_74HC595_status(hc595_info.green_led, hc595_info.red_led, len);
	}
	
	}
	else {
		report_error(ERR40);
	}
	
	set_hc595_buf(0xff);
	Refresh_74HC595_status(hc595_info.green_led, hc595_info.red_led, len);
}

void LedTest_process_2_converge(void)
{
	uint32	led_test_timer = 0;
	uint16	i, j, n;
	uint16	len, station_sum, cnt;
	uint8	color;


	len = hc595_info.hc595_config.buf_size;
	station_sum = dev_info.dev_config.station_quantity;
	cnt = (station_sum - 1) * 3 +1;	//sta + r+l = 3; sta =1;
	
	//off
	set_hc595_buf(0xff);
	Refresh_74HC595_status(hc595_info.green_led, hc595_info.red_led, len);

	if (dev_info.dev_config.up_direction == RtoL) {

	for (j = 0; j < 2; j ++) {
		if (!(j%2)) {
			color = COLOR_GREEN;
		}
		else {
			color = COLOR_RED;
		}
		for (i = 0; i < cnt; i++) {
			while(!timer0_check_timer(&led_test_timer, LIGHT_LED_INTERVAL2))
				feed_dog();

			set_door_color(DOOR_opposite, color);
			set_door_color(DOOR_this, color);
			/*
			if (i == cnt - 1) {
				//opposite door
				set_door_color(DOOR_opposite, color);

			}
			else if (i == cnt - 2) {
				//this door
				set_door_color(DOOR_this, color);
			}
			else 
			*/
			if (!(i%3)) {
				//sta
				n = i/3 + 1;
				set_line_color((uint8)n, TYPE_station, color);
			}
			else if ((i >= 1) && (!((i - 1)%3))) {
				//r_direct
				n = (i - 1)/3 + 2;
				set_line_color((uint8)n, TYPE_R, color);
			}
			else if ((i >= 2) && (!((i - 2)%3))) {
				//l_direct
				n = (i - 2)/3 + 1;
				set_line_color((uint8)n, TYPE_L, color);
			}
			else {
				report_error(ERR44);
			}
			
			Refresh_74HC595_status(hc595_info.green_led, hc595_info.red_led, len);
		}

		while(!timer0_check_timer(&led_test_timer, LIGHT_LED_INTERVAL2))
			feed_dog();
		set_hc595_buf(0xff);
		Refresh_74HC595_status(hc595_info.green_led, hc595_info.red_led, len);
	}
	}
	else if (dev_info.dev_config.up_direction == LtoR) {
	for (j = 0; j < 2; j ++) {
		if (!(j%2)) {
			color = COLOR_GREEN;
		}
		else {
			color = COLOR_RED;
		}
		for (i = 0; i < cnt; i++) {
			while(!timer0_check_timer(&led_test_timer, LIGHT_LED_INTERVAL2))
				feed_dog();

			set_door_color(DOOR_opposite, color);
			set_door_color(DOOR_this, color);
			/*
			if (i == cnt - 1) {
				//opposite door
				set_door_color(DOOR_opposite, color);

			}
			else if (i == cnt - 2) {
				//this door
				set_door_color(DOOR_this, color);
			}
			else 
			*/
			if (!(i%3)) {
				//sta
				n = i/3 + 1;
				set_line_color((uint8)n, TYPE_station, color);
			}
			else if ((i >= 1) && (!((i - 1)%3))) {
				//l_direct
				n = (i - 1)/3 + 2;
				set_line_color((uint8)n, TYPE_L, color);
			}
			else if ((i >= 2) && (!((i - 2)%3))) {
				//r_direct
				n = (i - 2)/3 + 1;
				set_line_color((uint8)n, TYPE_R, color);
			}
			else {
				report_error(ERR45);
			}
			
			Refresh_74HC595_status(hc595_info.green_led, hc595_info.red_led, len);
		}

		while(!timer0_check_timer(&led_test_timer, LIGHT_LED_INTERVAL2))
			feed_dog();
		set_hc595_buf(0xff);
		Refresh_74HC595_status(hc595_info.green_led, hc595_info.red_led, len);
	}
	
	}
	else {
		report_error(ERR43);
	}
	
	set_hc595_buf(0xff);
	Refresh_74HC595_status(hc595_info.green_led, hc595_info.red_led, len);
}

/*********************************************************************
��������:	
	�����õ���LED����վ�����з���,���࿪��,�Բ࿪��
	��˳����Ѳ
����˵��: 	--
ȫ�ֱ�����--
���ú�����--
ע������:	--
����˵��:	--
**********************************************************************/
void LedTest_process_2(void)
{
	ARROW_STYLE_T	style;


	//���ݼ�ͷ�ķ�������·λ��
	style = dev_info.dev_config.arrow_sty;
	switch (style) {
	case ARROW_STYLE_converge:
		LedTest_process_2_converge();
		break;
	case ARROW_STYLE_diverge:
		LedTest_process_2_diverge();
		break;
	default:
		report_error(ERR39);
		break;
	}
}

/*********************************************************************
��������:	
	һ��led��,����ָʾ����Ѳһ��,�������е�led
����˵��: 	--
ȫ�ֱ�����--
���ú�����--
ע������:	--
����˵��:	--
**********************************************************************/
//LtoR
void LedTest_process_3(void)
{
	uint32	led_test_timer = 0;
	uint8	i, j;
	uint8	len;

	len = hc595_info.hc595_config.hc595_quantity;
	
	//off
	set_hc595_buf(0xff);
	Refresh_74HC595_status(hc595_info.green_led, hc595_info.red_led, len);

	//sequence on
	for (i = 0; i < len; i++) {
		
		for(j = 0; j < 8; j ++) {
			while(!timer0_check_timer(&led_test_timer, LIGHT_LED_INTERVAL3))
				feed_dog();
			hc595_info.green_led[len -1 - i] &= ~(0x80 >> j);
			Refresh_74HC595_status(hc595_info.green_led, hc595_info.red_led, len);
		}
	}

	while(!timer0_check_timer(&led_test_timer, LIGHT_LED_INTERVAL3))
		feed_dog();
	
	set_hc595_buf(0xff);
	Refresh_74HC595_status(hc595_info.green_led, hc595_info.red_led, len);

	for (i = 0; i < len; i++) {
		
		for(j = 0; j < 8; j ++) {
			while(!timer0_check_timer(&led_test_timer, LIGHT_LED_INTERVAL3))
				feed_dog();
			hc595_info.red_led[len -1 - i] &= ~(0x80 >> j);
			Refresh_74HC595_status(hc595_info.green_led, hc595_info.red_led, len);
		}
	}

	while(!timer0_check_timer(&led_test_timer, LIGHT_LED_INTERVAL3))
		feed_dog();
	
	set_hc595_buf(0xff);
	Refresh_74HC595_status(hc595_info.green_led, hc595_info.red_led, len);
}

typedef void (*Led_Test_Callback)(void);

typedef struct {
	uint8				m_index;
	Led_Test_Callback		method;
}LED_TEST_PROCESS_T;

const LED_TEST_PROCESS_T  LedTest_process_table[] = {
	{	1,				LedTest_process_1		},
	{	2,				LedTest_process_2		},
	{	3,				LedTest_process_3		},
	{	null,				null						}
};

void LED_test(void)
{
	uint8	index;
	uint8	select;

	select = 2;
	#ifdef TEST_MODE
	select = 3;
	#endif

	index = 0;
	while (LedTest_process_table[index].method) {
		if (LedTest_process_table[index].m_index == select) {
			LedTest_process_table[index].method();
			break;
		}
		index++;
	}

}

/*********************************************************************
��������:	�豸��ʼ��
				����ָʾ����Ѳһ��
����˵��: 	--
ȫ�ֱ�����--
���ú�����void init_hc595_info(void)
				void init_dev_info(void)
				void LED_test(void)
ע������:	����ʼ����ʱ����һ��
����˵��:	--
**********************************************************************/
void device_init(void)
{
	init_hc595_info();
	init_dev_info();
	
	tim1_pwm_init();		//������init_dev_info();֮�����, ������init_dev_info();�г�ʼ��
	
	LED_test();
}



/*********************************************************************
��������:	�ж϶�̬��ͼ�Ĳ���(��������з���)
����˵��: 	LED_MAP_SIDE_T *p_map_side, 
				uint8 key_flag, 
				uint8 CentralController_devId, 
				uint8 dmp_devID
ȫ�ֱ�����--
���ú�����report_error(ERR48);
ע������:	--
����˵��:	--
**********************************************************************/
void judge_ledMap_side(LED_MAP_SIDE_T *p_map_side, uint8 key_flag, uint8 CentralController_devId, uint8 dmp_devID)
{

	if (key_flag) {	//��Կ��
		if ((dmp_devID >= FirstPart_devID_start) && (dmp_devID <= FirstPart_devID_end)) {
			//ǰ�벿�ֳ���
			if (dmp_devID%2) {
				if (CentralController_devId == FirstCentrolController_ID) {
					*p_map_side = SIDE_LEFT;
				}
				else {
					*p_map_side = SIDE_RIGHT;
				}
			}
			else {
				if (CentralController_devId == FirstCentrolController_ID) {
					*p_map_side = SIDE_RIGHT;
				}
				else {
					*p_map_side = SIDE_LEFT;
				}
			}
		}
		else if ((dmp_devID >= SecondPart_devID_start) && (dmp_devID <= SecondPart_devID_end)) {
			//��벿�ֳ���
			if (dmp_devID%2) {
				if (CentralController_devId == FirstCentrolController_ID) {
					*p_map_side = SIDE_RIGHT;
				}
				else {
					*p_map_side = SIDE_LEFT;
				}
			}
			else {
				if (CentralController_devId == FirstCentrolController_ID) {
					*p_map_side = SIDE_LEFT;
				}
				else {
					*p_map_side = SIDE_RIGHT;
				}
			}
			
		}
		else {
			*p_map_side = SIDE_INVALID;
		}

	}
	else {	//ûԿ��
		if ((dmp_devID >= FirstPart_devID_start) && (dmp_devID <= FirstPart_devID_end)) {
			//ǰ�벿�ֳ���
			if (dmp_devID%2) {
				if (CentralController_devId == FirstCentrolController_ID) {
					*p_map_side = SIDE_RIGHT;
				}
				else {
					*p_map_side = SIDE_LEFT;
				}
			}
			else {
				if (CentralController_devId == FirstCentrolController_ID) {
					*p_map_side = SIDE_LEFT;
				}
				else {
					*p_map_side = SIDE_RIGHT;
				}
			}

		}
		else if ((dmp_devID >= SecondPart_devID_start) && (dmp_devID <= SecondPart_devID_end)) {
			//��벿�ֳ���
			if (dmp_devID%2) {
				if (CentralController_devId == FirstCentrolController_ID) {
					*p_map_side = SIDE_LEFT;
				}
				else {
					*p_map_side = SIDE_RIGHT;
				}
			}
			else {
				if (CentralController_devId == FirstCentrolController_ID) {
					*p_map_side = SIDE_RIGHT;
				}
				else {
					*p_map_side = SIDE_LEFT;
				}
			}
		}
		else {
			*p_map_side = SIDE_INVALID;
		}
	}

	if (*p_map_side == SIDE_INVALID) {
		report_error(ERR48);
	}

}

/*********************************************************************
��������:	ˢ���ŵ�״̬
����˵��: 	uint8 door_tmp, LED_MAP_SIDE_T map_side
				//door_tmp.0: open_right
				//door_tmp.1: open_left
				//door_tmp.4: close_right
				//door_tmp.5: close_left
ȫ�ֱ�����--
���ú�����--
ע������:	--
����˵��:	--
**********************************************************************/
void refresh_doorStatus(uint8 door_tmp, LED_MAP_SIDE_T map_side)
{
	uint8	door_tmp_open, door_tmp_close;

	door_tmp_open = door_tmp & 0x03;
	door_tmp_close = (door_tmp >> 4) & 0x03;
	
	switch (door_tmp_open) {
		case 0x00:	//û�п����ź�
			dev_info.dev_status.open_door_side = OPEN_DOOR_STATUS_INVALID;
			break;

		case 0x01:	//right door
			if (map_side == SIDE_RIGHT) {
				dev_info.dev_status.open_door_side = OPEN_THIS_DOOR;
			}
			else if (map_side == SIDE_LEFT) {
				dev_info.dev_status.open_door_side = OPEN_OPPOSITE_DOOR;
			}
			else {
				dev_info.dev_status.open_door_side = OPEN_TWOSIDE_DOOR;
			}
			break;

		case 0x02:	//left door
			if (map_side == SIDE_LEFT) {
				dev_info.dev_status.open_door_side = OPEN_THIS_DOOR;
			}
			else if (map_side == SIDE_RIGHT) {
				dev_info.dev_status.open_door_side = OPEN_OPPOSITE_DOOR;
			}
			else {
				dev_info.dev_status.open_door_side = OPEN_TWOSIDE_DOOR;
			}
			break;

		case 0x03:	//two side door
			dev_info.dev_status.open_door_side = OPEN_TWOSIDE_DOOR;
			break;

		default:
			break;

		}


	switch (door_tmp_close) {
		case 0x00:	//û�й����ź�
			dev_info.dev_status.close_door_side = CLOSE_DOOR_STATUS_INVALID;
			break;

		case 0x01:	//right door
			if (map_side == SIDE_RIGHT) {
				dev_info.dev_status.close_door_side = CLOSE_THIS_DOOR;
			}
			else if (map_side == SIDE_LEFT) {
				dev_info.dev_status.close_door_side = CLOSE_OPPOSITE_DOOR;
			}
			else {
				dev_info.dev_status.close_door_side = CLOSE_TWOSIDE_DOOR;
			}
			break;

		case 0x02:	//left door
			if (map_side == SIDE_LEFT) {
				dev_info.dev_status.close_door_side = CLOSE_THIS_DOOR;
			}
			else if (map_side == SIDE_RIGHT) {
				dev_info.dev_status.close_door_side = CLOSE_OPPOSITE_DOOR;
			}
			else {
				dev_info.dev_status.close_door_side = CLOSE_TWOSIDE_DOOR;
			}
			break;

		case 0x03:	//two side door
			dev_info.dev_status.close_door_side = CLOSE_TWOSIDE_DOOR;
			break;

		default:
			break;

		}
}

/*********************************************************************
��������:	����Э������ˢ���豸״̬
����˵��: 	ProtocolPackTypeDef *msg
ȫ�ֱ�����--
���ú�����adjust_brightness();
				judge_ledMap_side(&map_side, key_flag, CentralController_devId, dmp_devID);
				refresh_doorStatus(door_tmp, map_side);
ע������:	--
����˵��:	--
**********************************************************************/
void refresh_device_status(ProtocolPackTypeDef *msg)
{
	static uint8	tmp = 1;

	uint8	refresh_flag;
	uint8	down_flag_tmp;
	//DEV_STATUS_T	dev_status_tmp;
	uint8 	CentralController_devId;
	uint8	dmp_devID;
	uint8	key_flag;
	uint8	door_tmp;

	uint8	*p_old_dat;
	uint8	*p_new_dat;
	
	uint16	i;
	LED_MAP_SIDE_T	map_side;

	TAG_CentralController_process_data *p_process_data;
	static TAG_CentralController_process_data process_data_tmp = {0};
	static uint8 CentralControler_devID_tmp = 0;

	dmp_devID = DevId;
	p_process_data = (TAG_CentralController_process_data *)msg->DataBuf.Buf;
	CentralController_devId = msg->Head.SrcInfo.DevId;

	refresh_flag = 0;
	p_old_dat = (uint8 *)&process_data_tmp;
	p_new_dat = (uint8 *)p_process_data;
	for (i =0; i < sizeof(TAG_CentralController_process_data); i++) {
		if ((*(p_old_dat+i) != *(p_new_dat+i)) || (CentralControler_devID_tmp != msg->Head.SrcInfo.DevId)) {
			refresh_flag = 1;
			CentralControler_devID_tmp = msg->Head.SrcInfo.DevId;
			mem_cpy(p_old_dat, p_new_dat, sizeof(TAG_CentralController_process_data));
			break;
		}
			
	}

	if (refresh_flag == 0) {
		return;
	}
	
	
	if (tmp) {		//�״��յ���������
		if (!(p_process_data->signal_1.down ^ p_process_data->signal_1.up)) {
			//�����б�־,��ͬ��Ϊ����Ч����
			return;
		}
		
		tmp = 0;

		//1. վ�����
		dev_info.dev_status.cur_station_num = p_process_data->current_station;
		dev_info.dev_status.next_station_num = p_process_data->next_station;
		dev_info.dev_status.start_station_num = p_process_data->start_station;
		dev_info.dev_status.end_station_num = p_process_data->end_station;

		//2. ���ȵ���
		if (dev_info.dev_status.brightness != p_process_data->dmp_brightness) {
			//��������
			dev_info.dev_status.brightness = p_process_data->dmp_brightness;
			adjust_brightness();
		}

		//3. �����б�־
		if (p_process_data->signal_1.down) {
			dev_info.dev_status.up_down_flag = TRAIN_DOWN;
		}
		if (p_process_data->signal_1.up) {
			dev_info.dev_status.up_down_flag = TRAIN_UP;
		}

		//4.״̬
		if (p_process_data->signal_1.arr_flag) {
			dev_info.dev_status.status = ARRIVE_STATUS;
		}
		else if (p_process_data->signal_1.pre_flag) {
			dev_info.dev_status.status = DEPARTURE_STATUS;
		}
		else {
			;
		}

		if (
			((!p_process_data->signal_1.arr_flag) && (!p_process_data->signal_1.pre_flag))
			||
			((p_process_data->current_station == p_process_data->start_station) && (p_process_data->next_station == p_process_data->start_station))
			) {

			dev_info.dev_status.status = SWITCH_UP_DOWN_STATUS;

		}

		
		//5.���Ų�
		//dev_info.dev_status.door_side = DOOR_STATUS_INVALID;

		//�ж϶�̬led�������Կ�׵İ�װλ��
		if (p_process_data->signal_1.key_flag) {
			key_flag = 1;
		}
		else {
			key_flag = 0;
		}
		judge_ledMap_side(&map_side, key_flag, CentralController_devId, dmp_devID);

		door_tmp = 0;
		if (p_process_data->signal_1.open_right_door_flag) {
			door_tmp |= 0x01;
		}
		if (p_process_data->signal_1.open_left_door_flag) {
			door_tmp |= 0x02;
		}
		if (p_process_data->signal_1.close_right_door_flag) {
			door_tmp |= 0x10;
		}
		if (p_process_data->signal_1.close_left_door_flag) {
			door_tmp |= 0x20;
		}
		//door_tmp.0: open_right
		//door_tmp.1: open_left
		//door_tmp.4: close_right
		//door_tmp.5: close_left

		refresh_doorStatus(door_tmp, map_side);
		
	}
	else {
		//return;
		//1. վ�����
		dev_info.dev_status.cur_station_num = p_process_data->current_station;
		dev_info.dev_status.next_station_num = p_process_data->next_station;
		dev_info.dev_status.start_station_num = p_process_data->start_station;
		dev_info.dev_status.end_station_num = p_process_data->end_station;

		//2. ���ȵ���
		if (dev_info.dev_status.brightness != p_process_data->dmp_brightness) {
			//��������
			dev_info.dev_status.brightness = p_process_data->dmp_brightness;
			adjust_brightness();
		}


		//4.״̬
		if (p_process_data->signal_1.arr_flag) {
			dev_info.dev_status.status = ARRIVE_STATUS;
		}
		else if (p_process_data->signal_1.pre_flag) {
			dev_info.dev_status.status = DEPARTURE_STATUS;
		}
		else {
			;
		}


		//3. �����б�־
		down_flag_tmp = dev_info.dev_status.up_down_flag;
		if (p_process_data->signal_1.down) {
			dev_info.dev_status.up_down_flag = TRAIN_DOWN;
		}
		if (p_process_data->signal_1.up) {
			dev_info.dev_status.up_down_flag = TRAIN_UP;
		}

		if (down_flag_tmp != dev_info.dev_status.up_down_flag) {
			dev_info.dev_status.status = SWITCH_UP_DOWN_STATUS;
		}
		
		
		//5.���Ų�
		//dev_info.dev_status.door_side = DOOR_STATUS_INVALID;

		//�ж϶�̬led�������Կ�׵İ�װλ��
		if (p_process_data->signal_1.key_flag) {
			key_flag = 1;
		}
		else {
			key_flag = 0;
		}
		judge_ledMap_side(&map_side, key_flag, CentralController_devId, dmp_devID);

		door_tmp = 0;
		if (p_process_data->signal_1.open_right_door_flag) {
			door_tmp |= 0x01;
		}
		if (p_process_data->signal_1.open_left_door_flag) {
			door_tmp |= 0x02;
		}
		if (p_process_data->signal_1.close_right_door_flag) {
			door_tmp |= 0x10;
		}
		if (p_process_data->signal_1.close_left_door_flag) {
			door_tmp |= 0x20;
		}
		//door_tmp.0: open_right
		//door_tmp.1: open_left
		//door_tmp.4: close_right
		//door_tmp.5: close_left
		refresh_doorStatus(door_tmp, map_side);
	}
	
}

#endif




