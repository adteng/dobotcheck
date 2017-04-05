#include "stdafx.h"
#include "serialport.h"

//////////////////////////////////////////////////////////////////////////////////
// class SerialPort
//////////////////////////////////////////////////////////////////////////////////
SerialPort::SerialPort()
{
	bIsOpen = false;
	bIsInit = false;
	m_BaudRate = CBR_9600;
};

SerialPort::~SerialPort()
{
	if(bIsOpen) Close();
};

int SerialPort::Open(const string& strPort)
{
	hCom = CreateFile(strPort.c_str(),GENERIC_READ|GENERIC_WRITE,0,NULL,
//		OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
		OPEN_EXISTING, 0, NULL);
	if (hCom == (HANDLE)0xFFFFFFFF)
	{
		DWORD errorno = GetLastError();
		string str = "open serial port err, id=" + GetLastError();
		theLog<< str <<ende;
		return -1;
	}
	bIsOpen = true;
	if(!bIsInit)
	{
		int nRet = InitComm();
		if(nRet < 0) return -1;
		bIsInit = true;
	}
	return 0;
};

int SerialPort::InitComm()
{
	if(!GetCommState(hCom,&dcb))
	{
		theLog<<"GetCommState err, id="<<GetLastError()<<ende;
		return -1;
	}
	dcb.BaudRate = m_BaudRate;//CBR_115200; //波特率为115200
	//dcb.BaudRate = CBR_9600; //波特率为9600
//	dcb.BaudRate = CBR_256000; //波特率为256000
	dcb.ByteSize = 8; //数据位数为8位
	dcb.fBinary = TRUE;	//二进制
	dcb.StopBits = ONESTOPBIT; //一个停止位
/*	dcb.BaudRate = CBR_9600; //波特率为9600
//	dcb.BaudRate = CBR_256000; //波特率为256000
	dcb.fBinary = TRUE;	//二进制
	dcb.fParity = FALSE;	//Enable parity checking无校验
	dcb.fOutxCtsFlow = 0x00;
	dcb.fOutxDsrFlow = 0x00;
	dcb.fDtrControl = DTR_CONTROL_ENABLE; //DTR_CONTROL_ENABLE=0x01当串口设备打开时，DTR信号线设为使能并始终打开
	dcb.fDsrSensitivity = 0x00;
	dcb.fTXContinueOnXoff = 0x00;
	dcb.fOutX = 0x00;
	dcb.fInX = 0x00;
	dcb.fErrorChar = 0x00;
	dcb.fNull = 0x00;
	dcb.fRtsControl = RTS_CONTROL_DISABLE; //当串口设备打开时，RTS线始终为禁止
	dcb.fAbortOnError = 0x00;
	dcb.fDummy2 = 0x00;
	dcb.wReserved = 0x00;
	dcb.XonLim = 0x0100;		//
	dcb.XoffLim = 0x0100;		//
	dcb.ByteSize = 8; //数据位数为8位
	dcb.Parity = NOPARITY; //NOPARITY无校验
	dcb.StopBits = ONESTOPBIT; //一个停止位
	dcb.XonChar = 0xc8;		//
	dcb.XoffChar = 0xb4;	//
	dcb.ErrorChar = 0xdd;	//指定当接收到的数据发生奇偶校验错误时替换的字符。
	dcb.EofChar = 0x44;		//指定某字符作为数据结束标记。
	dcb.EvtChar = 0x00;		//指定某字符作为事件标记。

*/	if (!SetCommState(hCom, &dcb))
	{
		strErrInfo = "SetCommState err, id=" + GetLastError();
		theLog<<strErrInfo<<ende;
		return -1;
	}
	//SetupComm(hCom, 1024, 1024);	//接收缓冲区和发送缓冲区大小均为42个字节
	SetupComm(hCom, 42, 42);
	//PurgeComm函数终止所有的后台读写操作并清空接收缓冲区和发送缓冲区
//	PurgeComm(hCom, PURCE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	PurgeComm(hCom, PURGE_TXCLEAR|PURGE_RXCLEAR);
	//设置超时
	//COMMTIMEOUTS.ReadIntervalTimeout定义两个字符到达的最大时间间隔，单位：毫秒
	memset(&to, 0, sizeof(to));
	to.ReadIntervalTimeout = 1000;	//接收两个字符之间的最长超时时间，按毫秒计算
	to.ReadTotalTimeoutConstant = 1000;	//读取操作的总超时周期的常数，按毫秒计算
	SetCommTimeouts(hCom, &to);
	if(!SetCommMask(hCom,EV_RXCHAR))
	{
		strErrInfo = "byteSendData SetCommMask err, id=" + GetLastError();
		theLog<<strErrInfo<<ende;
		return -2;
	}
	return 0;
};
//清除串口缓存区，0：不清除；1:发送；2：接收；其他：全部
void SerialPort::ClearBuf(int nClearRT)
{
	if(nClearRT == 0)
	{
		return;
	}else if(nClearRT == 1){
		PurgeComm(hCom, PURGE_TXCLEAR);
	}else if(nClearRT == 1){
		PurgeComm(hCom, PURGE_RXCLEAR);
	}else
		PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR);
};

void SerialPort::Close()
{
	PurgeComm(hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
//	PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR);
	CloseHandle(hCom);
	bIsInit = false;
	bIsOpen = false;
};

int SerialPort::byteSendData(const byte* byteBuff,int nSize)
{
	unsigned long nTmp = -1;// = nSize;
	if(!WriteFile(hCom,byteBuff,nSize,&nTmp,NULL))
	{
		strErrInfo = "byteSendData err, id=" + GetLastError();
		theLog<<strErrInfo<<ende;
		return -1;
	}
/*	if(!SetCommMask(hCom,EV_TXEMPTY))//输出缓冲区中的数据已被完全送出
	{
		strErrInfo = "byteSendData SetCommMask err, id=" + GetLastError();
		theLog<<strErrInfo<<ende;
		return -2;
	}
	if(!SetCommMask(hCom,EV_RXCHAR))
	{
		strErrInfo = "byteSendData SetCommMask err, id=" + GetLastError();
		theLog<<strErrInfo<<ende;
		return -2;
	}
*/	return nTmp;
};

int SerialPort::byteReadData(byte* byteBuff,int nSize)
{
//	unsigned long lEvtMask;
//	OVERLAPPED   overlapped; 

//	WaitCommEvent(hCom,&lEvtMask,NULL);
/*	if(!WaitCommEvent(hCom,&lEvtMask,&overlapped))
	{
		 if(GetLastError() == ERROR_IO_PENDING)
		 {
			 DWORD ret = WaitForSingleObject(overlapped.hEvent,10000);
			 if(ret == WAIT_TIMEOUT)
			 {
				theLog<<"byteReadData Timeout!"<<ende;
				return -1;
			 }
		 }else{
			strErrInfo = IToString(GetLastError());
			return -1;
		 }
	}*/
//	if((lEvtMask&EV_TXEMPTY) == EV_TXEMPTY)
//	{
//		strErrInfo = "输出缓冲区中的数据已被完全送出, id=" + GetLastError();
//		theLog<<strErrInfo<<endi;
//	}
	unsigned long nLen = -1;
//	if((lEvtMask&EV_RXCHAR) == EV_RXCHAR)
	{
		unsigned long nTmp = nSize;
		if(!ReadFile(hCom,byteBuff,nTmp,&nLen,NULL))
		{
			strErrInfo = "byteReadData err, id=" + GetLastError();
			theLog<<strErrInfo<<ende;
			return -2;
		}
	}/*else{
		strErrInfo = "byteReadData WaitCommEvent err, id=" + GetLastError();
		theLog<<strErrInfo<<ende;
		return -2;
	}*/
#ifdef _DEBUG_
	theLog<<"SerialPort ReadData=0x"<<HEXToString(byteBuff,nSize)<<endi;
#endif
	return nLen;
};


