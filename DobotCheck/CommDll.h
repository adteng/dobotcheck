#ifndef __COMMDLL__H
#define __COMMDLL__H 


int __stdcall SendReaderCommand(char *ComPort,unsigned char Cmd,unsigned char *SendData,unsigned  char SendDataLen,unsigned char *RetCode,unsigned char *RecvData,unsigned char* RecvDataLen);
int __stdcall SendNormalData(char *ComPort,unsigned long sBaud,unsigned char *SendData,unsigned  int SendDataLen,unsigned char *RecvData,unsigned int* RecvDataLen);
int __stdcall SendRecvData(unsigned char *SendData,unsigned  int SendDataLen,unsigned char *RecvData,unsigned int* RecvDataLen);
int __stdcall ComOpen(char* strPort,unsigned long sBaud);
void __stdcall ComClose();
#endif


