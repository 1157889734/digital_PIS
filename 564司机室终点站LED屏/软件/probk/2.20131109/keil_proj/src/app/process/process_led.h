#ifndef __process_led_h
#define __process_led_h



//#define		TEST_MODE


#define	TRAIN_UP			0
#define	TRAIN_DOWN		1
#define	LED_NONE			0xFF

#define	BRIGHTNESS_VALUE	80	//动态地图的初始亮度


//---车厢动态地图分布定义

#define	DMP_SoftWareVersion	1		//软件发布版次
#define	IP_offset				(12)		//相对1号中控的ip偏移
#define	IP_resSUM				(80)		//PIS 系统中dmp 的预留ip数量

#define	FirstCentrolController_ID	(0x01)	//第一块中控的id
#define	Firstdmp_ID				(0x01)	//第一块动态地图的id

#define	FirstPart_carSum			4
#define	SecondPart_carSum		4
#define	FirstPart_devID_start		Firstdmp_ID

#define	FirstPart_devID_end		(FirstPart_devID_start + FirstPart_carSum * 8 - 1)
#define	SecondPart_devID_start	(FirstPart_devID_end + 1)
#define	SecondPart_devID_end	(SecondPart_devID_start + SecondPart_carSum * 8 - 1)



#if DMP_DRIVER

/**************************常量宏定义********************************/
//
#define	R_TO_L		0
#define	L_TO_R		1

#define	RIGHT		2
#define	LEFT		3

//方向箭头风格
//汇聚O <- -> O <- -> O <- -> O
#define	ARROW_converge	4
//发散O -> <- O -> <- O -> <- O
#define	ARROW_diverge		5

//相对上行方向动态地图的位置
#define	MAP_SIDE		LEFT

//上行方向
#define	UP_DIRECTION	L_TO_R	//上行方向

//方向箭头风格
#define	ARROW_STYLE	ARROW_diverge



#define	LIGHT_LED_INTERVAL1	(30)	//自检方式一间隔
#define	LIGHT_LED_INTERVAL2	(30)	//自检方式二 间隔
#define	LIGHT_LED_INTERVAL3	(30)	//自检方式三间隔

#ifdef TEST_MODE
#define	led_map_SUM	1			//工厂测试模式只用一块显示板
#else
#define	led_map_SUM	3			//动态地图显示板的数量
#endif

#define	HC595_NUM		7			//每块动态显示板控制红/绿led的74hc595 各自的数量

#define	STATION_SUM	48			//车站数量

#define	FIRST_STATION_charNum		0	//led的起点位置
#define	FIRST_STATION_bitNum		1

#define	ThisDoor_charNum		13	//本侧开门指示灯位置
#define	ThisDoor_bitNum			1

#define	OppositeDoor_charNum		6	//对侧开门指示灯位置
#define	OppositeDoor_bitNum			1

/************************** 端口定义 ********************************/


/************************** 类型定义 ********************************/

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

//相对上行方向,动态地图安装的侧面
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
	LOCATION_INDEX_T	first_led_location;		//起点led位置
	
	uint8				station_quantity;		//车站数量

	uint8				this_door_charNum;
	uint8				this_door_bitNum;
	uint8				opposite_door_charNum;
	uint8				opposite_door_bitNum;
}DEV_CONFIG_T;

typedef struct {
	uint8 				station_index;			//LED索引
	LOCATION_INDEX_T	l_direction_led;			//左方向箭头
	LOCATION_INDEX_T	station_led;				//站点
	LOCATION_INDEX_T	r_direction_led;			//右方向箭头
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
	DEV_CONFIG_T		dev_config;	//配置信息
	
	DEV_STATUS_T		dev_status;		//状态信息来自过程数据

	//开门侧位置信息
	LOCATION_INDEX_T	this_door;
	LOCATION_INDEX_T	opposite_door;
	
	//(站点、方向箭头位置信息)
	//按照上行方向分配,站号由小到大:sta1,sta2,sta3.....staN ,NULL
	LED_INDEX_T		*line_index;		//根据dev_config进行分配和初始化
	
}DEV_INFO_T;

typedef struct {
	uint8		led_map_quantity;	//动态地图显示板的数量
	uint8		hc595_quantity;		//每块动态显示板控制红/绿led的74hc595 各自的数量
	uint16		buf_size;			//红/绿各自的缓存字节数
}TAG_74HC595_CONFIG;

typedef struct {
	TAG_74HC595_CONFIG	hc595_config;
	
	uint8			*green_led;		//根据led_map_quantity 分配内存
	uint8			*red_led;		//根据led_map_quantity 分配内存
}TAG_74HC595_INFO;


/************************** 变量定义 ********************************/
/* 全局变量定义样例
 
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


/************************** 函数声明 ********************************/
void device_init(void);
void set_hc595_buf(uint8 ch);
void LED_status_process(void);

void refresh_device_status(ProtocolPackTypeDef *msg);

#endif



#endif


