/*------------------------------------------------------------------------
File name:             serialport.h
Author:                linhu
Version:               1.0
Date:                  2007 - 08 - 23
Description:           ������
Others:       
Revision history:      
   1:
     Date:
     Author:
     Modifiacation:
   2������
------------------------------------------------------------------------*/
#ifndef _SERIALPORT_H_
#define _SERIALPORT_H_

#include <windows.h>
#include <winbase.h>
#include "utility.h"

//#define	_DEBUG_


/*------------------------------------------------------------------------
Function:          class SerialPort
Description:       SerialPort��
----------------------------------------------------------------------*/
class SerialPort
{
private:
	HANDLE hCom;
	DCB dcb;
	COMMTIMEOUTS to;
	bool bIsOpen,bIsInit;
	string strErrInfo;

	int InitComm();		//Open�����Զ�����
public:
	int m_BaudRate;
	SerialPort();
	virtual ~SerialPort();

	int Open(const string& strPort);	//�򿪴���
	void	Close();//�رմ���
	int byteSendData(const byte* byteBuff,int nSize);	//
	int	byteReadData(byte* byteBuff,int nSize);	//
	void ClearBuf(int nClearRT);//������ڻ�������0:���ͣ�1�����գ�2��ȫ��
	string GetErrInfo(){return strErrInfo;}
};


#endif
