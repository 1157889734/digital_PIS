#ifndef __process_net_h
#define __process_net_h

/**************************常量宏定义********************************/
#define	Run_STATUS_OK		0
#define	Run_STATUS_FAULT	1


#define CMD_TEST_run_iap			0x0a01
#define CMD_TEST_set_dispMode	0x0a02
#define CMD_TEST_download_app	0x0a03
#define CMD_TEST_debug			0x0a5a

/************************** 端口定义 ********************************/


/************************** 类型定义 ********************************/
typedef void (*MsgHandler_udp_CtrlMsg_send)(uint16 keyword);

typedef struct {
	uint16	cmd_index;
	uint16	cmd;
	MsgHandler_udp_CtrlMsg_send	msghandler;
	
}TAG_Evt_udp_CtrlMsg_send;


typedef void (*MsgHandler_udp_CtrlMsg_recv)(ProtocolPackTypeDef *msg);

typedef struct {
	uint16	cmd_index;
	uint16	cmd;
	MsgHandler_udp_CtrlMsg_recv	msghandler;
	
}TAG_Evt_udp_CtrlMsg_recv;


typedef __packed struct {
	uint8	run_status;
	uint8	brightness;
	uint8	software_version;
}TAG_dmp_Process_Data;

typedef __packed struct {
	uint8	run_status;			 //运行状态
	uint8	software_version[4];	 //程序版本号
}TAG_HLED_Process_Data;	    //车头屏过程数据结构体

typedef __packed struct 
{
   u16 dispDataLen;
   u8 totalPackNum;
   u8 currentPackNumber;
   u8 dispColor;
   u8 dispKind;
   
   u8 dispData[1024];
}TAG_Disp_Info;

typedef __packed struct 
{
   u8 totalPackNum;
   u8 currentPackNumber;
   u8 dispColor;
   u8 dispKind;
   u8 dispData[1024];
}TAG_protocol_Disp_Info;


/*
//广播控制盒过程数据结构
__packed typedef struct {
	//广播控制位
__packed	struct {
	
	// 1 byte 
		uint8	res1_1		:4;
		uint8	down		:1;
		uint8	up			:1;
		uint8	manual_flag	:1;
		uint8	master_flag	:1;

	// 2 byte 
		uint8	test					:1;
		uint8	res2_1				:1;
		uint8	stop_arr_broadcast	:1;
		uint8	start_arr_broadcast	:1;
		uint8	stop_pre_broadcast	:1;
		uint8	start_pre_broadcast	:1;
		uint8	urgent_broadcast_flag:1;		//紧急广播触发
		uint8	mic_broadcast_flag	:1;		//话筒广播触发

	// 3 byte
		uint8	res3_1						:6;
		uint8	stop_DriverDriverTalk_flag		:1;	//停止司机对讲
		uint8	start_DriverDriverTalk_flag	:1;

	// 4 byte
		uint8	res						:7;
		uint8	DriverPassengerTalk_flag	:1;	 //司机乘客对讲
	}signal_1;

	// 5 byte
	uint8	start_station;

	// 6 byte
	uint8	end_station;

	// 7 byte
	uint8	current_station;

	// 8 byte
	uint8	next_station;

	// 9 byte
	uint8	urgent_broadcast_sectionNo;	// 9 紧急广播代码

	// 10 byte
	uint8	intercom_alarmNo;			// 10 对讲的报警器编号

	// 11 byte
	uint8	dcp_RunStatus;				//广播盒运行状态

	// 12 byte
	uint8	mic_vol;		//话筒音量

	// 13 byte
	uint8	broadcast_monitor_vol;	//广播监听音量
	
	// 14 byte
	uint8	talk_monitor_vol;			//对讲监听音量
	
	// 15 byte
	uint8	dcp_softwareVersion;		//广播盒软件版本

}TAG_dcp_process_data;
*/

//中央控制器过程数据结构
__packed typedef struct {
	//控制位
__packed	struct {
	// 1 byte
		uint8	atc_mode_flag	:1;	//atc 模式
		uint8	tms_mode_flag	:1;	//tms 模式
		uint8	dcp_mode_flag	:1;	//dcp 模式
		uint8	res1_1			:3;
		uint8	key_flag			:1;	//钥匙标志
		uint8	master_flag		:1;	//主标志
		
	// 2 byte
		uint8	res2_1					:2;
		uint8	close_right_door_flag		:1;
		uint8	close_left_door_flag		:1;
		uint8	open_right_door_flag		:1;
		uint8	open_left_door_flag		:1;
		uint8	down					:1;
		uint8	up						:1;
		
// 3 byte
		uint8	res3_1		:6;
		uint8	arr_flag		:1;	//到站停稳
		uint8	pre_flag		:1;	//离站
		
// 4 byte
		uint8	res4_1					:1;
		uint8	dcp_arr_broadcasting		:1;
		uint8	dcp_pre_broadcasting		:1;
		uint8	dcp_urgent_broadcasting	:1;
		uint8	res4_2					:4;
	}signal_1;

	// 5 byte
	uint8	start_station;

	// 6 byte
	uint8	end_station;

	// 7 byte
	uint8	current_station;

	// 8 byte
	uint8	next_station;

	//9 byte
	uint8	CentralController_status;
	
	// 10 byte
	uint8	dmp_brightness;

	// 11 byte
	uint8	car_ledScreen_rollSpeed;

	// 12 byte
	uint8	CentralController_softwareVersion;		//中央控制器软件版本

}TAG_CentralController_process_data;


typedef __packed struct
{
	uint8	ip[4];	
	uint8 filename_len;
	uint8	filename[1];
} TAG_download_app_req;	   //车头屏过程数据结构体

/************************** 变量定义 ********************************/
/* 全局变量定义样例
 
#ifdef model_GLOBALS
    #define model_EXT
#else
    #define model_EXT extern

*/
#ifdef process_net_GLOBALS
    #define process_net_EXT
#else
    #define process_net_EXT extern
#endif


extern uint8	dmp_run_status;
extern TAG_dmp_Process_Data		dmp_process_data;
extern uint8	Hled_run_status;

extern TAG_Disp_Info netCNDispInfo;
extern TAG_Disp_Info netENDispInfo;
extern TAG_protocol_Disp_Info tmpProtocolDispInfo;
/************************** 函数声明 ********************************/
void udp_CtrlMsg_send_process(uint16 keyword);
void udp_CtrlMsg_receive_process(uint8 *buf);
void report_error(uint8 err_info);
void clr_dispinfo(void);
int UdpTestReceiveProcess(u8 *pbuf, int len);
void udpTest_send_process(uint16 keyword, void *p, uint16 len);

#endif


