#include "includes.h"

TAG_dmp_Process_Data		dmp_process_data = {Run_STATUS_OK, 0, DMP_SoftWareVersion};
uint8	dmp_run_status = Run_STATUS_OK;

uint8	pack_data[DATA_MAX_SIZE + 100];
uint16	pack_len;

void report_error(uint8 err_info)
{
	dmp_run_status = err_info;

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

void send_ip_mac_id(uint16 keyword)
{
	read_local_info();

	pis_pack_data(ProtocolBroadInfo, ProtocolLocalInfo, keyword, (uint8 *)null, null, pack_data, &pack_len);

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


void respond_Get_KEYWORD_softwareVersion(ProtocolPackTypeDef *msg)
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
	
	mem_cpy(local_mac, (uint8 *)msg->DataBuf.Buf, sizeof(local_mac));

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

const TAG_Evt_udp_CtrlMsg_recv	Evt_udp_CtrlMsg_recv_Tbl[] = {
	{1,		Ctrl_KEYWORD_ProcessData,				respond_Ctrl_KEYWORD_ProcessData		},
	{2,		Get_KEYWORD_ip_mac_id,					respond_Get_KEYWORD_ip_mac_id		},
	{3,		Get_KEYWORD_softwareVersion,				respond_Get_KEYWORD_softwareVersion	},
	{4,		Set_KEYWORD_mac,						respond_Set_KEYWORD_mac			},
	{5,		Set_KEYWORD_IP,							respond_Set_KEYWORD_IP				},
	{null,	null,										null						},
	{null,	null,										null						}
};

void udp_CtrlMsg_receive_process(uint8 *buf)
{
	uint16	index;
	ProtocolPackTypeDef *p;

	p = (ProtocolPackTypeDef *)buf;

	index = 0;

	while (Evt_udp_CtrlMsg_recv_Tbl[index].cmd_index != null) {
		if (Evt_udp_CtrlMsg_recv_Tbl[index].cmd == p->KeyWord) {
			Evt_udp_CtrlMsg_recv_Tbl[index].msghandler(p);
			break;
		}
		index++;
	}
}




