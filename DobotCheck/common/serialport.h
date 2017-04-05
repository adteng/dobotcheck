/*------------------------------------------------------------------------
File name:             serialport.h
Author:                linhu
Version:               1.0
Date:                  2007 - 08 - 23
Description:           串口类
Others:       
Revision history:      
   1:
     Date:
     Author:
     Modifiacation:
   2：……
------------------------------------------------------------------------*/
#ifndef _SERIALPORT_H_
#define _SERIALPORT_H_

#include <windows.h>
#include <winbase.h>
#include "utility.h"

//#define	_DEBUG_


/*------------------------------------------------------------------------
Function:          class SerialPort
Description:       SerialPort类
----------------------------------------------------------------------*/
class SerialPort
{
private:
	HANDLE hCom;
	DCB dcb;
	COMMTIMEOUTS to;
	bool bIsOpen,bIsInit;
	string strErrInfo;

	int InitComm();		//Open函数自动调用
public:
	int m_BaudRate;
	SerialPort();
	virtual ~SerialPort();

	int Open(const string& strPort);	//打开串口
	void	Close();//关闭串口
	int byteSendData(const byte* byteBuff,int nSize);	//
	int	byteReadData(byte* byteBuff,int nSize);	//
	void ClearBuf(int nClearRT);//清除串口缓存区，0:发送；1：接收；2：全部
	string GetErrInfo(){return strErrInfo;}
};


#endif
