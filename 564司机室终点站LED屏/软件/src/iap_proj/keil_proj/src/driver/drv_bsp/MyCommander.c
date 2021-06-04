
//*****************************************************//
//		MyCommander.c
//*****************************************************//

#include "MyAll.h"



char OutStr[256];


void CommanderInit(void)
{

	char logo_str[] = "//**********  Beihai Communication  ***********//\n\
Digital_PIS--Locomotive LED\n\
application version: V1.0\n";
	
	CommanderSendString(logo_str);
	CommanderSendLocalIp();
	CommanderSendDeviceType();
	CommanderSendDeviceId();
	CommanderSendTrainId();
	
}

void CommanderSend_route_mask_mac(void)
{
	memset(OutStr, 0, sizeof(OutStr));		
	sprintf(OutStr, "Route IP: %d.%d.%d.%d  \nNet Mask: %d.%d.%d.%d  \nMac: %02x:%02x:%02x:%02x:%02x:%02x\n",\
		NetRouterIp[0], NetRouterIp[1], NetRouterIp[2], NetRouterIp[3],\
		NetMaskIp[0], NetMaskIp[1], NetMaskIp[2], NetMaskIp[3],\
		NetLocalMac[0], NetLocalMac[1], NetLocalMac[2], NetLocalMac[3], NetLocalMac[4], NetLocalMac[5]
		);
	CommanderSendString(OutStr);

}

void send_AppInfo(void)
{
	char logo_str[200] = "//**********  Beihai Communication  ***********//\n\
Digital_PIS--Locomotive LED\n\
application version: V";

{
	char tmp[20];
	uint8 len_version;
	uint8 i;
	len_version = sizeof(software_version);

	memset(tmp, 0, sizeof(tmp));
	for (i = 0; i < len_version; i++) {
		if (i == (len_version-1)) {
			sprintf(tmp, "%d", software_version[i]);
			strcat(logo_str, tmp);
		}
		else {
			sprintf(tmp, "%d.", software_version[i]);
			strcat(logo_str, tmp);
		}
	}
}
	
	CommanderSendString(logo_str);
	CommanderSendLocalIp();
	CommanderSend_route_mask_mac();
	CommanderSendDeviceType();
	CommanderSendDeviceId();
	CommanderSendTrainId();
	
}
void send_downloadReq(void)
{
	char logo_str[200] = "//**********  Beihai Communication  ***********//\n\
Digital_PIS--Locomotive LED\n\
iap programe version: V";
	char req[] = "please download application programe now!!\n";

	//strcat(logo_str, HLED_SoftWareVersion_char);
{
	char tmp[20];
	uint8 len_version;
	uint8 i;
	len_version = sizeof(software_version);

	memset(tmp, 0, sizeof(tmp));
	for (i = 0; i < len_version; i++) {
		if (i == (len_version-1)) {
			sprintf(tmp, "%d", software_version[i]);
			strcat(logo_str, tmp);
		}
		else {
			sprintf(tmp, "%d.", software_version[i]);
			strcat(logo_str, tmp);
		}
	}
}
	CommanderSendString(logo_str);
	CommanderSendLocalIp();
	CommanderSend_route_mask_mac();
	CommanderSendDeviceType();
	CommanderSendDeviceId();
	CommanderSendTrainId();
	CommanderSendString(req);
	
}


void CommanderSendString(char* pstr)
{

	UdpCommanderSend((u8*)pstr, strlen(pstr));


}




void CommanderReceive(u8* pbuf, int len)
{

	char words[16][16];
	u8 rip[4];
	int rtemp;


	memset(words, 0, sizeof(words));
	
	LineToWords((char *)pbuf, len, &words[0][0], 16, 16);


	if((strcmp(&words[0][0], "set") == 0) && (strcmp(&words[1][0], "localip") == 0))
	{
		rip[0] = (u8)StrToInt(&words[2][0]);
		rip[1] = (u8)StrToInt(&words[3][0]);
		rip[2] = (u8)StrToInt(&words[4][0]);
		rip[3] = (u8)StrToInt(&words[5][0]);

		memset(OutStr, 0, sizeof(OutStr));		
		sprintf(OutStr, "Ok, please wait...\n");
		CommanderSendString(OutStr);

		NetSetLocalIp4(rip[0], rip[1], rip[2], rip[3]);
		

	}
	else if((strcmp(&words[0][0], "read") == 0) && (strcmp(&words[1][0], "localip") == 0))
	{
		CommanderSendLocalIp();
		
	}
	else if((strcmp(&words[0][0], "read") == 0) && (strcmp(&words[1][0], "devtype") == 0))
	{
		CommanderSendDeviceType();
		
	}
	else if((strcmp(&words[0][0], "set") == 0) && (strcmp(&words[1][0], "devid") == 0))
	{
		rtemp = StrToInt(&words[2][0]);
		SetDeviceId(rtemp);

	}
	else if((strcmp(&words[0][0], "read") == 0) && (strcmp(&words[1][0], "devid") == 0))
	{
		CommanderSendDeviceId();
		
	}
	else if((strcmp(&words[0][0], "set") == 0) && (strcmp(&words[1][0], "trainid") == 0))
	{
		rtemp = StrToInt(&words[2][0]);
		SetTrainId(rtemp);

	}
	else if((strcmp(&words[0][0], "read") == 0) && (strcmp(&words[1][0], "trainid") == 0))
	{
		CommanderSendTrainId();
		
	}
	else if((strcmp(&words[0][0], "set") == 0) && (strcmp(&words[1][0], "volume") == 0))
	{
		rtemp = StrToInt(&words[2][0]);
		AudioSetVolume(rtemp);

	}
	

	else
	{
		memset(OutStr, 0, sizeof(OutStr));		
		sprintf(OutStr, "Invalid Command !\n");
		CommanderSendString(OutStr);

	}



}



void CommanderSendLocalIp(void)
{

	memset(OutStr, 0, sizeof(OutStr));		
	sprintf(OutStr, "\nLocal IP: %d.%d.%d.%d  \n", NetLocalIp[0], NetLocalIp[1],NetLocalIp[2], NetLocalIp[3]);
	CommanderSendString(OutStr);

}



void CommanderSendDeviceType(void)
{

	memset(OutStr, 0, sizeof(OutStr));		
	sprintf(OutStr, "Device Type: %d  \n", DevType);
	CommanderSendString(OutStr);

}


void CommanderSendDeviceId(void)
{

	memset(OutStr, 0, sizeof(OutStr));		
	sprintf(OutStr, "Device Id: %d  \n", DevId);
	CommanderSendString(OutStr);

}


void CommanderSendTrainId(void)
{

	memset(OutStr, 0, sizeof(OutStr));		
	sprintf(OutStr, "Train Id: %d  \n", TrainId);
	CommanderSendString(OutStr);

}









