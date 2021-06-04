#ifndef __process_led_h
#define __process_led_h



//#define		TEST_MODE


#define	TRAIN_UP			0
#define	TRAIN_DOWN		1
#define	LED_NONE			0xFF

#define	BRIGHTNESS_VALUE	80	//��̬��ͼ�ĳ�ʼ����


//---���ᶯ̬��ͼ�ֲ�����

#define	DMP_SoftWareVersion	1		//����������
#define	IP_offset				(12)		//���1���пص�ipƫ��
#define	IP_resSUM				(80)		//PIS ϵͳ��dmp ��Ԥ��ip����

#define	FirstCentrolController_ID	(0x01)	//��һ���пص�id
#define	Firstdmp_ID				(0x01)	//��һ�鶯̬��ͼ��id

#define	FirstPart_carSum			4
#define	SecondPart_carSum		4
#define	FirstPart_devID_start		Firstdmp_ID

#define	FirstPart_devID_end		(FirstPart_devID_start + FirstPart_carSum * 8 - 1)
#define	SecondPart_devID_start	(FirstPart_devID_end + 1)
#define	SecondPart_devID_end	(SecondPart_devID_start + SecondPart_carSum * 8 - 1)



#if DMP_DRIVER

/**************************�����궨��********************************/
//
#define	R_TO_L		0
#define	L_TO_R		1

#define	RIGHT		2
#define	LEFT		3

//�����ͷ���
//���O <- -> O <- -> O <- -> O
#define	ARROW_converge	4
//��ɢO -> <- O -> <- O -> <- O
#define	ARROW_diverge		5

//������з���̬��ͼ��λ��
#define	MAP_SIDE		LEFT

//���з���
#define	UP_DIRECTION	L_TO_R	//���з���

//�����ͷ���
#define	ARROW_STYLE	ARROW_diverge



#define	LIGHT_LED_INTERVAL1	(30)	//�Լ췽ʽһ���
#define	LIGHT_LED_INTERVAL2	(30)	//�Լ췽ʽ�� ���
#define	LIGHT_LED_INTERVAL3	(30)	//�Լ췽ʽ�����

#ifdef TEST_MODE
#define	led_map_SUM	1			//��������ģʽֻ��һ����ʾ��
#else
#define	led_map_SUM	3			//��̬��ͼ��ʾ�������
#endif

#define	HC595_NUM		7			//ÿ�鶯̬��ʾ����ƺ�/��led��74hc595 ���Ե�����

#define	STATION_SUM	48			//��վ����

#define	FIRST_STATION_charNum		0	//led�����λ��
#define	FIRST_STATION_bitNum		1

#define	ThisDoor_charNum		13	//���࿪��ָʾ��λ��
#define	ThisDoor_bitNum			1

#define	OppositeDoor_charNum		6	//�Բ࿪��ָʾ��λ��
#define	OppositeDoor_bitNum			1

/************************** �˿ڶ��� ********************************/


/************************** ���Ͷ��� ********************************/

typedef enum {
	ARROW_STYLE_INVALID = 1,
	ARROW_STYLE_converge,
	ARROW_STYLE_diverge
}ARROW_STYLE_T;

typedef enum {
	TRAIN_STATUS_INVALID = 0,
	ARRIVE_STATUS,
	//GENERAL_STATUS,
	//SKIP_STATUS,
	//ARRIVE_SKIP_STATUS,
	//TEST_STATUS,
	DEPARTURE_STATUS,
	SWITCH_UP_DOWN_STATUS
}TRAIN_STATUS;

typedef enum {
	CLOSE_DOOR_STATUS_INVALID = 1,
	CLOSE_THIS_DOOR,
	CLOSE_OPPOSITE_DOOR,
	CLOSE_TWOSIDE_DOOR
	
}CLOSE_DOOR_STATUS;

typedef enum {
	OPEN_DOOR_STATUS_INVALID = 1,
	OPEN_THIS_DOOR,
	OPEN_OPPOSITE_DOOR,
	OPEN_TWOSIDE_DOOR
	
}OPEN_DOOR_STATUS;

//������з���,��̬��ͼ��װ�Ĳ���
typedef enum {
	SIDE_INVALID = 1,
	SIDE_LEFT,
	SIDE_RIGHT
}LED_MAP_SIDE_T;

typedef enum {
	DIRECTION_INVALID = 1,
	LtoR,
	RtoL
}DIRECTION_STATUS;

typedef struct {
	uint8	char_num;
	uint8	bit_num;
}LOCATION_INDEX_T;

typedef struct {
	LED_MAP_SIDE_T			map_side;
	ARROW_STYLE_T			arrow_sty;
	DIRECTION_STATUS		up_direction;
	LOCATION_INDEX_T	first_led_location;		//���ledλ��
	
	uint8				station_quantity;		//��վ����

	uint8				this_door_charNum;
	uint8				this_door_bitNum;
	uint8				opposite_door_charNum;
	uint8				opposite_door_bitNum;
}DEV_CONFIG_T;

typedef struct {
	uint8 				station_index;			//LED����
	LOCATION_INDEX_T	l_direction_led;			//�����ͷ
	LOCATION_INDEX_T	station_led;				//վ��
	LOCATION_INDEX_T	r_direction_led;			//�ҷ����ͷ
}LED_INDEX_T;

typedef struct {
	TRAIN_STATUS	status;
	CLOSE_DOOR_STATUS	close_door_side;
	OPEN_DOOR_STATUS	open_door_side;
	uint8			brightness;
	uint8			up_down_flag;
	uint8			start_station_num;
	uint8			end_station_num;
	uint8			cur_station_num;
	uint8			next_station_num;
}DEV_STATUS_T;

typedef struct {
	DEV_CONFIG_T		dev_config;	//������Ϣ
	
	DEV_STATUS_T		dev_status;		//״̬��Ϣ���Թ�������

	//���Ų�λ����Ϣ
	LOCATION_INDEX_T	this_door;
	LOCATION_INDEX_T	opposite_door;
	
	//(վ�㡢�����ͷλ����Ϣ)
	//�������з������,վ����С����:sta1,sta2,sta3.....staN ,NULL
	LED_INDEX_T		*line_index;		//����dev_config���з���ͳ�ʼ��
	
}DEV_INFO_T;

typedef struct {
	uint8		led_map_quantity;	//��̬��ͼ��ʾ�������
	uint8		hc595_quantity;		//ÿ�鶯̬��ʾ����ƺ�/��led��74hc595 ���Ե�����
	uint16		buf_size;			//��/�̸��ԵĻ����ֽ���
}TAG_74HC595_CONFIG;

typedef struct {
	TAG_74HC595_CONFIG	hc595_config;
	
	uint8			*green_led;		//����led_map_quantity �����ڴ�
	uint8			*red_led;		//����led_map_quantity �����ڴ�
}TAG_74HC595_INFO;


/************************** �������� ********************************/
/* ȫ�ֱ�����������
 
#ifdef model_GLOBALS
    #define model_EXT
#else
    #define model_EXT extern

*/
#ifdef process_led_GLOBALS
    #define process_led_EXT
#else
    #define process_led_EXT extern
#endif

process_led_EXT	TAG_74HC595_INFO	hc595_info;
process_led_EXT	DEV_INFO_T			dev_info;


/************************** �������� ********************************/
void device_init(void);
void set_hc595_buf(uint8 ch);
void LED_status_process(void);

void refresh_device_status(ProtocolPackTypeDef *msg);

#endif



#endif


