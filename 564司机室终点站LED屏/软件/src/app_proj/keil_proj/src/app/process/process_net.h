#ifndef __process_net_h
#define __process_net_h

/**************************�����궨��********************************/
#define	Run_STATUS_OK		0
#define	Run_STATUS_FAULT	1


#define CMD_TEST_run_iap			0x0a01
#define CMD_TEST_set_dispMode	0x0a02
#define CMD_TEST_download_app	0x0a03
#define CMD_TEST_debug			0x0a5a

/************************** �˿ڶ��� ********************************/


/************************** ���Ͷ��� ********************************/
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
	uint8	run_status;			 //����״̬
	uint8	software_version[4];	 //����汾��
}TAG_HLED_Process_Data;	    //��ͷ���������ݽṹ��

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
//�㲥���ƺй������ݽṹ
__packed typedef struct {
	//�㲥����λ
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
		uint8	urgent_broadcast_flag:1;		//�����㲥����
		uint8	mic_broadcast_flag	:1;		//��Ͳ�㲥����

	// 3 byte
		uint8	res3_1						:6;
		uint8	stop_DriverDriverTalk_flag		:1;	//ֹͣ˾���Խ�
		uint8	start_DriverDriverTalk_flag	:1;

	// 4 byte
		uint8	res						:7;
		uint8	DriverPassengerTalk_flag	:1;	 //˾���˿ͶԽ�
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
	uint8	urgent_broadcast_sectionNo;	// 9 �����㲥����

	// 10 byte
	uint8	intercom_alarmNo;			// 10 �Խ��ı��������

	// 11 byte
	uint8	dcp_RunStatus;				//�㲥������״̬

	// 12 byte
	uint8	mic_vol;		//��Ͳ����

	// 13 byte
	uint8	broadcast_monitor_vol;	//�㲥��������
	
	// 14 byte
	uint8	talk_monitor_vol;			//�Խ���������
	
	// 15 byte
	uint8	dcp_softwareVersion;		//�㲥������汾

}TAG_dcp_process_data;
*/

//����������������ݽṹ
__packed typedef struct {
	//����λ
__packed	struct {
	// 1 byte
		uint8	atc_mode_flag	:1;	//atc ģʽ
		uint8	tms_mode_flag	:1;	//tms ģʽ
		uint8	dcp_mode_flag	:1;	//dcp ģʽ
		uint8	res1_1			:3;
		uint8	key_flag			:1;	//Կ�ױ�־
		uint8	master_flag		:1;	//����־
		
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
		uint8	arr_flag		:1;	//��վͣ��
		uint8	pre_flag		:1;	//��վ
		
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
	uint8	CentralController_softwareVersion;		//�������������汾

}TAG_CentralController_process_data;


typedef __packed struct
{
	uint8	ip[4];	
	uint8 filename_len;
	uint8	filename[1];
} TAG_download_app_req;	   //��ͷ���������ݽṹ��

/************************** �������� ********************************/
/* ȫ�ֱ�����������
 
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
/************************** �������� ********************************/
void udp_CtrlMsg_send_process(uint16 keyword);
void udp_CtrlMsg_receive_process(uint8 *buf);
void report_error(uint8 err_info);
void clr_dispinfo(void);
int UdpTestReceiveProcess(u8 *pbuf, int len);
void udpTest_send_process(uint16 keyword, void *p, uint16 len);

#endif


