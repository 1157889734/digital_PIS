#include "includes.h"

TAG_dmp_Process_Data		dmp_process_data = {Run_STATUS_OK, 0, DMP_SoftWareVersion};
uint8	dmp_run_status = Run_STATUS_OK;

uint8	pack_data[DATA_MAX_SIZE + 100];
uint16	pack_len;

void report_error(uint8 err_info)
{
	//dmp_run_status = err_info;
	Hled_run_status =err_info;

}
void read_local_info(void)
{
	ProtocolLocalInfo.DevType = DevType;
	ProtocolLocalInfo.DevId = DevId;
	ProtocolLocalInfo.TrainId = TrainId;
	ProtocolLocalInfo.Ip32 = NetIp4toIp32(NetLocalIp);
	
}

void pis_pack_data(ProtocolDeviceInfoTypeDef dst_info, 
						ProtocolDeviceInfoTypeDef src_info,
						uint16 cmd,
						uint8 *src_data, uint16 src_len,
						uint8 *pack_data, uint16 *pack_len)
{
	ProtocolPackTypeDef *p_pack;

	p_pack = (ProtocolPackTypeDef *)pack_data;

	//head
	p_pack->Head.DestInfo = dst_info;
	p_pack->Head.SrcInfo = src_info;
	p_pack->Head.HeadMark = PIS_PACKET_HEAD;

	//cmd
	p_pack->KeyWord = cmd;

	//data
	mem_cpy((uint8 *)p_pack->DataBuf.Buf, (uint8 *) src_data, src_len);
	p_pack->DataLength = src_len;

	//length
	*pack_len = sizeof(ProtocolPackTypeDef) - DATA_MAX_SIZE + src_len;
}

TAG_HLED_Process_Data		hled_process_data = {Run_STATUS_OK, HLED_SoftWareVersion};
uint8	Hled_run_status = Run_STATUS_OK;

void send_ProcessData(uint16 keyword)
{
	#if DMP_DRIVER
	dmp_process_data.brightness = dev_info.dev_status.brightness;
	dmp_process_data.run_status = dmp_run_status;
	dmp_process_data.software_version = DMP_SoftWareVersion;

	read_local_info();


	pis_pack_data(ProtocolBroadInfo, ProtocolLocalInfo, keyword, (uint8 *)&dmp_process_data, sizeof(TAG_dmp_Process_Data), pack_data, &pack_len);
	UdpControlSend(pack_data, pack_len);
	#endif


}

void send_ProcessData_ledScreen(uint16 keyword)
{
	hled_process_data.run_status = Hled_run_status;
	hled_process_data.software_version = HLED_SoftWareVersion;
	read_local_info();
	pis_pack_data(ProtocolBroadInfo, ProtocolLocalInfo, keyword, (uint8 *)&hled_process_data, sizeof(TAG_HLED_Process_Data), pack_data, &pack_len);
	UdpControlSend(pack_data, pack_len);

}

void send_ip_mac_id(uint16 keyword)
{
	read_local_info();
	
	pis_pack_data(ProtocolBroadInfo, ProtocolLocalInfo, keyword, (uint8 *)&ProtocolLocalInfo.Ip32, 4, pack_data, &pack_len);

	UdpControlSend(pack_data, pack_len);
}


void send_ip_mac_id_ledScreen(uint16 keyword)
{
	read_local_info();
	
	pis_pack_data(ProtocolBroadInfo, ProtocolLocalInfo, keyword, (uint8 *)&ProtocolLocalInfo.Ip32, 4, pack_data, &pack_len);

	UdpControlSend(pack_data, pack_len);
}
void send_softwareVersion(uint16 keyword)
{
	
	uint8	softwareVersion;
	

	softwareVersion = DMP_SoftWareVersion;
	
	read_local_info();

	pis_pack_data(ProtocolBroadInfo, ProtocolLocalInfo, keyword, (uint8 *)&softwareVersion, 1, pack_data, &pack_len);

	UdpControlSend(pack_data, pack_len);
}

void send_softwareVersion_ledScreen(uint16 keyword)
{
	
	uint8	softwareVersion;
	

	softwareVersion = HLED_SoftWareVersion;
	
	read_local_info();

	pis_pack_data(ProtocolBroadInfo, ProtocolLocalInfo, keyword, (uint8 *)&softwareVersion, 1, pack_data, &pack_len);

	UdpControlSend(pack_data, pack_len);
}

#if 0
const TAG_Evt_udp_CtrlMsg_send Evt_udp_CtrlMsg_send_Tbl[] = {
	{1,		Ctrl_KEYWORD_ProcessData,				send_ProcessData		},
	{2,		FeedBack_KEYWORD_ip_mac_id,				send_ip_mac_id			},
	{3,		FeedBack_KEYWORD_softwareVersion,		send_softwareVersion		},
	{null,	null,										null						},
	{null,	null,										null						}
};

void udp_CtrlMsg_send_process(uint16 keyword)
{
	uint16	index;

	index = 0;

	while (Evt_udp_CtrlMsg_send_Tbl[index].cmd_index != null) {
		if (Evt_udp_CtrlMsg_send_Tbl[index].cmd == keyword) {
			Evt_udp_CtrlMsg_send_Tbl[index].msghandler(keyword);
			break;
		}
		index++;
	}
}
#else
const TAG_Evt_udp_CtrlMsg_send Evt_udp_CtrlMsg_send_Tbl_ledScreen[] = {
	{1,		Ctrl_KEYWORD_ProcessData,				send_ProcessData_ledScreen		},
	{2,		FeedBack_KEYWORD_ip_mac_id,				send_ip_mac_id_ledScreen			},
	{3,		FeedBack_KEYWORD_softwareVersion,		send_softwareVersion_ledScreen		},
	{null,	null,										null						},
	{null,	null,										null						}
};

void udp_CtrlMsg_send_process(uint16 keyword)
{
	uint16	index;

	index = 0;

	while (Evt_udp_CtrlMsg_send_Tbl_ledScreen[index].cmd_index != null) {
		if (Evt_udp_CtrlMsg_send_Tbl_ledScreen[index].cmd == keyword) {
			Evt_udp_CtrlMsg_send_Tbl_ledScreen[index].msghandler(keyword);
			break;
		}
		index++;
	}
}
#endif

/*********************************************************************
��������:	����Э����е�Ŀ����Ϣ,�Ƿ��뱾����Ϣ��ͬ
����˵��: 	ProtocolPackTypeDef *msg
ȫ�ֱ�����--
���ú�����read_local_info();
ע������:	--
����˵��:	1: ��ͬ, 0:��ͬ
**********************************************************************/
uint16 check_dst_is_self(ProtocolPackTypeDef *msg)
{
	//uint16	i;
	//uint8	*p_msg_dev_info;
	//uint8	*p_local_dev_info;
	
	read_local_info();		//��ȡ������Ϣ��ProtocolLocalInfo

	//p_msg_dev_info = (uint8 *)&msg->Head.DestInfo;
	//p_local_dev_info = (uint8 *)&ProtocolLocalInfo;

	//for (i = 0; i < sizeof(ProtocolDeviceInfoTypeDef); i++) {
	//	if (*(p_msg_dev_info + i) != *(p_local_dev_info + i)) {
	//		return 0;
	//	}
	//}

	if ((msg->Head.DestInfo.DevId != ProtocolLocalInfo.DevId) 
		&& (msg->Head.DestInfo.DevType != ProtocolLocalInfo.DevType)) {
		return 0;
	}

	return 1;
}

/*********************************************************************
��������:	�����������
����˵��: 	ProtocolPackTypeDef *msg
ȫ�ֱ�����--
���ú�����--
ע������:	��̬��ͼֻ��Ӧ�пصĹ�������
����˵��:	--
**********************************************************************/
void respond_Ctrl_KEYWORD_ProcessData(ProtocolPackTypeDef *msg)
{
	uint8	src_dev_type;
	uint8	src_dev_id;
	uint8	src_ip1, src_ip2, src_ip3, src_ip4;
	uint16	src_ip4_tmp;
	uint16	src_trainid;

	uint32	src_ip32;

	TAG_CentralController_process_data *p_process_data;

	p_process_data = (TAG_CentralController_process_data *)msg->DataBuf.Buf;
	
	src_dev_type = msg->Head.SrcInfo.DevType;
	src_dev_id = msg->Head.SrcInfo.DevId;
	

	if (src_dev_type != PRTCL_DEVTYPE_CC) {
		//�����пصĹ�������ֱ�ӷ���
		return;
	}
	if (!p_process_data->signal_1.master_flag) {
		//����,����Ϊ������Ч
		return;
	}

	//�ж��Ƿ��Զ�����ip
	if (src_dev_id == FirstCentrolController_ID) {
		//�����1���пأ����ж��Ƿ�����ip


		if ((DevId + 1- Firstdmp_ID) > IP_resSUM) {
			//����Ԥ������
			return;
		}

		src_trainid = msg->Head.SrcInfo.TrainId;
		
		//src_ip32 = CentralController_IP32;
		src_ip32 = msg->Head.SrcInfo.Ip32;
		src_ip1 = (uint8)(src_ip32>>24);
		src_ip2 = (uint8)(src_ip32>>16);
		src_ip3 = (uint8)(src_ip32>>8);
		src_ip4 = (uint8)(src_ip32);

		src_ip4_tmp = src_ip4;
		src_ip4_tmp = src_ip4_tmp + IP_offset + DevId;

		if (src_ip4_tmp > 255) {
			//Ԥ����ip���Ϸ�
			return;
		}
		
		src_ip4 = src_ip4_tmp;

		if ((src_ip1 < 128) || (src_ip1 > 247) || (src_ip2 > 255) || (src_ip3 > 255) || (src_ip4 > 255)) {
			//Ԥ����ip���Ϸ�
			return;
		}
		
		//if (src_ip32 == msg->Head.SrcInfo.Ip32) {
		if ((src_ip1 == NetLocalIp[0])
			&& (src_ip2 == NetLocalIp[1])
			&&(src_ip3 == NetLocalIp[2])
			&&(src_ip4 == NetLocalIp[3])
			) {
			//Ԥ��ip�뱾��ip��ͬ
			if (TrainId != src_trainid) {
				TrainId = src_trainid;
				Delay(1000);
				StorageInfoSave();
				Delay(1000);
				StorageInfoSaveProcess();
				Delay(1000);
				Delay(1000);
				//return;
			}
		}
		else {
			TrainId = src_trainid;
			//CentralController_IP32 = src_ip32;
			NetSetLocalIp4(src_ip1, src_ip2, src_ip3, src_ip4);
			return;
		}
	}


	#if DMP_DRIVER
	//�������ݴ���
	refresh_device_status(msg);
	#endif
	
}

void respond_Ctrl_KEYWORD_ProcessData_ledscreen(ProtocolPackTypeDef *msg)
{
	uint8	src_dev_type;
	uint8	src_dev_id;
	uint8	src_ip1, src_ip2, src_ip3, src_ip4;
	uint16	src_ip4_tmp;
	uint16	src_trainid;

	uint32	src_ip32;

	TAG_CentralController_process_data *p_process_data;

	p_process_data = (TAG_CentralController_process_data *)msg->DataBuf.Buf;
	
	src_dev_type = msg->Head.SrcInfo.DevType;
	src_dev_id = msg->Head.SrcInfo.DevId;
	

	if (src_dev_type != PRTCL_DEVTYPE_CC) {
		//�����пصĹ�������ֱ�ӷ���
		return;
	}
	if (!p_process_data->signal_1.master_flag) {
		//����,����Ϊ������Ч
		return;
	}

	//�ж��Ƿ��Զ�����ip
	if (src_dev_id == FirstCentrolController_ID) {
		//�����1���пأ����ж��Ƿ�����ip

	/*
		if (DevId < FirstHLED_ID)
			return;

		if ((DevId + 1- FirstHLED_ID) > IP_resSUM) {
			//����Ԥ������
			return;
		}
	*/

		if (check_devid())
			return;

		src_trainid = msg->Head.SrcInfo.TrainId;
		
		//src_ip32 = CentralController_IP32;
		src_ip32 = msg->Head.SrcInfo.Ip32;
		src_ip1 = (uint8)(src_ip32>>24);
		src_ip2 = (uint8)(src_ip32>>16);
		src_ip3 = (uint8)(src_ip32>>8);
		src_ip4 = (uint8)(src_ip32);

		src_ip4_tmp = src_ip4;
		src_ip4_tmp = src_ip4_tmp + IP_offset + DevId - FirstHLED_ID;

		if (src_ip4_tmp > 255) {
			//Ԥ����ip���Ϸ�
			return;
		}
		
		src_ip4 = src_ip4_tmp;

		if ((src_ip1 < 128) || (src_ip1 > 247) || (src_ip2 > 255) || (src_ip3 > 255) || (src_ip4 > 255)) {
			//Ԥ����ip���Ϸ�
			return;
		}
		
		//if (src_ip32 == msg->Head.SrcInfo.Ip32) {
		if ((src_ip1 == NetLocalIp[0])
			&& (src_ip2 == NetLocalIp[1])
			&&(src_ip3 == NetLocalIp[2])
			&&(src_ip4 == NetLocalIp[3])
			) {
			//Ԥ��ip�뱾��ip��ͬ
			if (TrainId != src_trainid) {
				turn_off_screen();
				TrainId = src_trainid;
				Delay(1000);
				StorageInfoSave();
				Delay(1000);
				StorageInfoSaveProcess();
				Delay(1000);
				Delay(1000);
				//return;
			}
		}
		else {
			turn_off_screen();
			TrainId = src_trainid;
			//CentralController_IP32 = src_ip32;
			NetSetLocalIp4(src_ip1, src_ip2, src_ip3, src_ip4);
			return;
		}
	}


	#if DMP_DRIVER
	//�������ݴ���
	refresh_device_status(msg);
	#endif
	
}


void respond_Get_KEYWORD_softwareVersion(ProtocolPackTypeDef *msg)
{
	if (!check_dst_is_self(msg)) {
		//�豸��Ϣ��ƥ��
		return;
	}

	udp_CtrlMsg_send_process(FeedBack_KEYWORD_softwareVersion);
}

void respond_Get_KEYWORD_softwareVersion_ledscreen(ProtocolPackTypeDef *msg)
{
	if (!check_dst_is_self(msg)) {
		//�豸��Ϣ��ƥ��
		return;
	}

	udp_CtrlMsg_send_process(FeedBack_KEYWORD_softwareVersion);
}
/*********************************************************************
��������:	��ӦЭ��������������ر�����Ϣ
����˵��: 	ProtocolPackTypeDef *msg
ȫ�ֱ�����--
���ú�����--
ע������:	�ڽ�������ǰ��
				����Э����е�Ŀ����Ϣ,�Ƿ��뱾����Ϣ��ͬ
				��ͬ��ֱ�ӷ��أ�����Ӧ
����˵��:	--
		?????? 
		Э����Ҫ��ظ�:
					�豸��IP��MAC��ַ���г����
				���ؽ����ݿ���������ָ������ݲ���
		������������Ҫ��ظ������ݰ��в�����MAC
**********************************************************************/
void respond_Get_KEYWORD_ip_mac_id(ProtocolPackTypeDef *msg)
{
	if (!check_dst_is_self(msg)) {
		//�豸��Ϣ��ƥ��
		return;
	}

	udp_CtrlMsg_send_process(FeedBack_KEYWORD_ip_mac_id);
	
}
void respond_Get_KEYWORD_ip_mac_id_ledscreen(ProtocolPackTypeDef *msg)
{
	if (!check_dst_is_self(msg)) {
		//�豸��Ϣ��ƥ��
		return;
	}

	udp_CtrlMsg_send_process(FeedBack_KEYWORD_ip_mac_id);
	
}
/*********************************************************************
��������:	��ӦЭ��������������ñ���mac
����˵��: 	ProtocolPackTypeDef *msg
ȫ�ֱ�����--
���ú�����--
ע������:	�ڽ�������ǰ��
				����Э����е�Ŀ����Ϣ,�Ƿ��뱾����Ϣ��ͬ
				��ͬ��ֱ�ӷ��أ�������
����˵��:	--
**********************************************************************/
void respond_Set_KEYWORD_mac(ProtocolPackTypeDef *msg)
{
	uint8	local_mac[6];
	uint8	i;

	if (!check_dst_is_self(msg)) {
		//�豸��Ϣ��ƥ��
		return;
	}
	
	memcpy(local_mac, (uint8 *)msg->DataBuf.Buf, sizeof(local_mac));

	for (i = 0; i < 6; i++) {
		NetLocalMac[i] = local_mac[5 - i];
	}
	

	Delay(1000);
	StorageInfoSave();
	Delay(1000);
	StorageInfoSaveProcess();
	Delay(1000);
	Delay(1000);
	
	Delay(100000);
	Delay(100000);	
	NVIC_SystemReset();
	Delay(10000);
	Delay(10000);

}

void respond_Set_KEYWORD_mac_ledscreen(ProtocolPackTypeDef *msg)
{
	uint8	local_mac[6];
	uint8	i;
	uint8	difference_flag;

	if (!check_dst_is_self(msg)) {
		//�豸��Ϣ��ƥ��
		return;
	}
	
	memcpy(local_mac, (uint8 *)msg->DataBuf.Buf, sizeof(local_mac));

	difference_flag = 0;
	for (i = 0; i < 6; i++) {
		if (NetLocalMac[i] != local_mac[5 - i]) {
			NetLocalMac[i] = local_mac[5 - i];
			difference_flag = 1;
		}
		
	}
	if (!difference_flag)
		return;
	
	turn_off_screen_clrbuf();

	Delay(1000);
	StorageInfoSave();
	Delay(1000);
	StorageInfoSaveProcess();
	Delay(1000);
	Delay(1000);
	
	Delay(100000);
	Delay(100000);	
	NVIC_SystemReset();
	Delay(10000);
	Delay(10000);

}

/*********************************************************************
��������:	��ӦЭ��������������ñ���ip
����˵��: 	ProtocolPackTypeDef *msg
ȫ�ֱ�����--
���ú�����--
ע������:	�ڽ�������ǰ��
				����Э����е�Ŀ����Ϣ,�Ƿ��뱾����Ϣ��ͬ
				��ͬ��ֱ�ӷ��أ�������
����˵��:	--
**********************************************************************/
void respond_Set_KEYWORD_IP(ProtocolPackTypeDef *msg)
{
	uint8	ip_addr[4];

	if (!check_dst_is_self(msg)) {
		//�豸��Ϣ��ƥ��
		return;
	}

	mem_cpy(ip_addr, (uint8 *)msg->DataBuf.Buf, sizeof(ip_addr));

	NetSetLocalIp4(ip_addr[3], ip_addr[2], ip_addr[1], ip_addr[0]);
}
void respond_Set_KEYWORD_IP_ledscreen(ProtocolPackTypeDef *msg)
{
	uint8	ip_addr[4];

	if (!check_dst_is_self(msg)) {
		//�豸��Ϣ��ƥ��
		return;
	}

	memcpy(ip_addr, (uint8 *)msg->DataBuf.Buf, sizeof(ip_addr));


	if ((ip_addr[3] == NetLocalIp[0])
			&& (ip_addr[2] == NetLocalIp[1])
			&&(ip_addr[1] == NetLocalIp[2])
			&&(ip_addr[0] == NetLocalIp[3])
			) {
			return;
		}
		else {
			turn_off_screen();

			NetSetLocalIp4(ip_addr[3], ip_addr[2], ip_addr[1], ip_addr[0]);
			return;
		}
}


TAG_Disp_Info netCNDispInfo,netENDispInfo;
TAG_protocol_Disp_Info tmpProtocolDispInfo;



void reset_TAG_Disp_Info(TAG_Disp_Info *p)
{
	memset(p, 0xff, sizeof(TAG_Disp_Info));
	p->dispDataLen = 0;
	p->totalPackNum = 0;
	p->currentPackNumber = 0;
	p->dispColor = 0;
	p->dispKind = 0;
}

void reset_TAG_protocol_Disp_Info(TAG_protocol_Disp_Info *p)
{
	memset(p, 0xff, sizeof(TAG_protocol_Disp_Info));
	p->totalPackNum = 0;
	p->currentPackNumber = 0;
	p->dispColor = 0;
	p->dispKind = 0;
}


void clr_dispinfo(void)
{
	
	reset_TAG_Disp_Info(&netCNDispInfo);
	reset_TAG_Disp_Info(&netCNDispInfo);
	reset_TAG_protocol_Disp_Info(&tmpProtocolDispInfo);
}

//������Ļ�����ٶ�
void prc_set_roll_speed(ProtocolPackTypeDef *msg)
{	
	uint8 speed;
	
	if (!check_dst_is_self(msg)) {
		//�豸��Ϣ��ƥ��
		return;
	}

	if (msg->DataLength > 1)
		return;
	speed = msg->DataBuf.Buf[0];
	speed = PREset_roll_speed;	//		speed/netENDispInfo.dispDataLen*1000;
	roll_speed = speed;

}

//������Ļ����
void prc_set_brightness(ProtocolPackTypeDef *msg)
{
	uint16 bright;
	
	if (!check_dst_is_self(msg)) {
		//�豸��Ϣ��ƥ��
		return;
	}

	if (msg->DataLength > 2)
		return;

	bright = msg->DataBuf.Buf[0] + msg->DataBuf.Buf[1]*256;
	bright = PREset_roll_speed;
	brightness = bright;
}
void prc_down_content(ProtocolPackTypeDef *msg)
{
	uint8 *p;
	uint16 i;
	uint16 disp_len, buf_len;
	uint8 NetDispType;
	TAG_protocol_Disp_Info *p_protocol_disp;
	

	if (!check_dst_is_self(msg)) {
		//�豸��Ϣ��ƥ��
		return;
	}

	buf_len = msg->DataLength;
	p_protocol_disp = (TAG_protocol_Disp_Info *)&msg->DataBuf.Buf[0];
	
#if  1
	if ((buf_len < 4) || ((p_protocol_disp->dispKind != 1) && (p_protocol_disp->dispKind != 2))) {
		//set_dispmode(SCREEN_MODE_off);
		hled_process_data.run_status=Run_STATUS_FAULT; 
		Hled_run_status = Run_STATUS_FAULT;
		udp_CtrlMsg_send_process(Ctrl_KEYWORD_ProcessData);
		return;
	}

	hled_process_data.run_status = Run_STATUS_OK; 
	Hled_run_status = Run_STATUS_OK;
	if (buf_len == 4) {
		if (p_protocol_disp->dispKind == 0x01)
			reset_TAG_Disp_Info(&netCNDispInfo);
		else if (p_protocol_disp->dispKind == 0x02)
			reset_TAG_Disp_Info(&netENDispInfo);
		set_dispmode_SCREEN_MODE_stationID();
		return;
	}
	disp_len = buf_len - 4;
	if (disp_len > 1024)
		disp_len = 1024;
	
	//�յ�����ʾ�������ݸ�ʽΪ:�ܰ���+��ǰ����+��ʾ��ɫ+����+��ʾ����
    //Ŀǰ�ܰ����͵�ǰ���ž������ã����Բ��迼��
    
	NetDispType= p_protocol_disp->dispKind;   //����LED���յ�������������ʾ���������Ļ���Ӣ������
	if(NetDispType==1)        //����
	{
		set_dispmode_SCREEN_MODE_stationID();
		memcpy(&tmpProtocolDispInfo, &netCNDispInfo.totalPackNum, sizeof(tmpProtocolDispInfo));
		p = tmpProtocolDispInfo.dispData;
		for (i = 0; i < disp_len; i++) {
			p[i] ^= 0xff;
		}
		if ((memcmp(&tmpProtocolDispInfo, p_protocol_disp, disp_len+4) == 0) && (disp_len == netCNDispInfo.dispDataLen)) {
			//��ʾ������ͬ
			return;
		}
		else {
			//��ʾ���ݲ���ͬ
			memcpy(&tmpProtocolDispInfo, p_protocol_disp, disp_len+4);
			p = tmpProtocolDispInfo.dispData;
			for (i = 0; i < disp_len; i++) {
				p[i] ^= 0xff;
			}
			reset_TAG_Disp_Info(&netCNDispInfo);
			netCNDispInfo.dispDataLen = disp_len;
			memcpy(&netCNDispInfo.totalPackNum,&tmpProtocolDispInfo, disp_len+4);
			return;
		}
	}
	else if(NetDispType==2)		 //Ӣ�ģ��ֽڳ��Ȳ�ȷ��
	{
		//set_dispmode_SCREEN_MODE_stationID();
		memcpy(&tmpProtocolDispInfo, &netENDispInfo.totalPackNum, sizeof(tmpProtocolDispInfo));
		p = tmpProtocolDispInfo.dispData;
		for (i = 0; i < disp_len; i++) {
			p[i] ^= 0xff;
		}
		if ((memcmp(&tmpProtocolDispInfo, p_protocol_disp, disp_len+4) == 0) && (disp_len == netENDispInfo.dispDataLen)) {
			//��ʾ������ͬ
			return;
		}
		else {
			//��ʾ���ݲ���ͬ
			memcpy(&tmpProtocolDispInfo, p_protocol_disp, disp_len+4);
			p = tmpProtocolDispInfo.dispData;
			for (i = 0; i < disp_len; i++) {
				p[i] ^= 0xff;
			}
			reset_TAG_Disp_Info(&netENDispInfo);
			netENDispInfo.dispDataLen = disp_len;
			memcpy(&netENDispInfo.totalPackNum,&tmpProtocolDispInfo, disp_len+4);
			return;
		}
	}
	else                        //�����д�
	{
		//set_dispmode(SCREEN_MODE_off);
		hled_process_data.run_status=Run_STATUS_FAULT; 
		Hled_run_status = Run_STATUS_FAULT;
		udp_CtrlMsg_send_process(Ctrl_KEYWORD_ProcessData);
		return;
	}
#endif
}
const TAG_Evt_udp_CtrlMsg_recv	Evt_udp_CtrlMsg_recv_Tbl[] = {
	{1,		Ctrl_KEYWORD_ProcessData,				respond_Ctrl_KEYWORD_ProcessData		},
	{2,		Get_KEYWORD_ip_mac_id,					respond_Get_KEYWORD_ip_mac_id		},
	{3,		Get_KEYWORD_softwareVersion,				respond_Get_KEYWORD_softwareVersion	},
	{4,		Set_KEYWORD_mac,						respond_Set_KEYWORD_mac			},
	{5,		Set_KEYWORD_IP,							respond_Set_KEYWORD_IP				},
	{null,	null,										null						},
	{null,	null,										null						}
};


#define CMD_Ctrl_KEYWORD_ProcessData			0x0001
#define CMD_Get_KEYWORD_ip_mac_id				0x0201
#define CMD_Get_KEYWORD_softwareVersion		0x0233
#define CMD_Set_KEYWORD_mac					0x0130
#define CMD_Set_KEYWORD_IP					0x0101
#define CMD_SET_ROLL_SPEED 				0x010e       //������������������ù����ٶȵ�������
#define CMD_SET_DISP_BRIGHTNESS 			0x010f       //���������������������Ļ��ʾ���ȵ�������
#define CMD_DOWN_CONT 					0x0008         //������������͵ĳ�ͷLED����ʾ����

const TAG_Evt_udp_CtrlMsg_recv	Evt_udp_CtrlMsg_recv_Tbl_ledScreen[] = {
	{1,	CMD_Ctrl_KEYWORD_ProcessData,				respond_Ctrl_KEYWORD_ProcessData_ledscreen		},
	{2,	CMD_Get_KEYWORD_ip_mac_id,					respond_Get_KEYWORD_ip_mac_id_ledscreen		},
	{3,	CMD_Get_KEYWORD_softwareVersion,				respond_Get_KEYWORD_softwareVersion_ledscreen	},
	{4,	CMD_Set_KEYWORD_mac,						respond_Set_KEYWORD_mac_ledscreen			},
	{5,	CMD_Set_KEYWORD_IP,						respond_Set_KEYWORD_IP_ledscreen				},
	//{6,	CMD_SET_ROLL_SPEED,							prc_set_roll_speed						},
	//{7,	CMD_SET_DISP_BRIGHTNESS,					prc_set_brightness						},
	{8,	CMD_DOWN_CONT,							prc_down_content						},
	{null,	null,										null						}

};

void udp_CtrlMsg_receive_process(uint8 *buf)
{
	uint16	index;
	ProtocolPackTypeDef *p;

	p = (ProtocolPackTypeDef *)buf;

	index = 0;

	while (Evt_udp_CtrlMsg_recv_Tbl_ledScreen[index].cmd_index != null) {
		if (Evt_udp_CtrlMsg_recv_Tbl_ledScreen[index].cmd == p->KeyWord) {
			Evt_udp_CtrlMsg_recv_Tbl_ledScreen[index].msghandler(p);
			break;
		}
		index++;
	}
}




