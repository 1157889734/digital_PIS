#ifndef __tftp_cli_h
#define __tftp_cli_h

extern unsigned char proc_downloading_flag;

int tftp_cli_start(int opcode, char *filename, char *mode, unsigned char ip[4]);
void tftp_cli_timeout_process(void);
int check_UDPpcb_tftp(void);
int tftp_check_param(int opcode, char *filename, char *mode, unsigned char ip[4]);
void proc_download_process(void);

#endif
