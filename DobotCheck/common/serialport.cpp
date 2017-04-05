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
	dcb.BaudRate = m_BaudRate;//CBR_115200; //������Ϊ115200
	//dcb.BaudRate = CBR_9600; //������Ϊ9600
//	dcb.BaudRate = CBR_256000; //������Ϊ256000
	dcb.ByteSize = 8; //����λ��Ϊ8λ
	dcb.fBinary = TRUE;	//������
	dcb.StopBits = ONESTOPBIT; //һ��ֹͣλ
/*	dcb.BaudRate = CBR_9600; //������Ϊ9600
//	dcb.BaudRate = CBR_256000; //������Ϊ256000
	dcb.fBinary = TRUE;	//������
	dcb.fParity = FALSE;	//Enable parity checking��У��
	dcb.fOutxCtsFlow = 0x00;
	dcb.fOutxDsrFlow = 0x00;
	dcb.fDtrControl = DTR_CONTROL_ENABLE; //DTR_CONTROL_ENABLE=0x01�������豸��ʱ��DTR�ź�����Ϊʹ�ܲ�ʼ�մ�
	dcb.fDsrSensitivity = 0x00;
	dcb.fTXContinueOnXoff = 0x00;
	dcb.fOutX = 0x00;
	dcb.fInX = 0x00;
	dcb.fErrorChar = 0x00;
	dcb.fNull = 0x00;
	dcb.fRtsControl = RTS_CONTROL_DISABLE; //�������豸��ʱ��RTS��ʼ��Ϊ��ֹ
	dcb.fAbortOnError = 0x00;
	dcb.fDummy2 = 0x00;
	dcb.wReserved = 0x00;
	dcb.XonLim = 0x0100;		//
	dcb.XoffLim = 0x0100;		//
	dcb.ByteSize = 8; //����λ��Ϊ8λ
	dcb.Parity = NOPARITY; //NOPARITY��У��
	dcb.StopBits = ONESTOPBIT; //һ��ֹͣλ
	dcb.XonChar = 0xc8;		//
	dcb.XoffChar = 0xb4;	//
	dcb.ErrorChar = 0xdd;	//ָ�������յ������ݷ�����żУ�����ʱ�滻���ַ���
	dcb.EofChar = 0x44;		//ָ��ĳ�ַ���Ϊ���ݽ�����ǡ�
	dcb.EvtChar = 0x00;		//ָ��ĳ�ַ���Ϊ�¼���ǡ�

*/	if (!SetCommState(hCom, &dcb))
	{
		strErrInfo = "SetCommState err, id=" + GetLastError();
		theLog<<strErrInfo<<ende;
		return -1;
	}
	//SetupComm(hCom, 1024, 1024);	//���ջ������ͷ��ͻ�������С��Ϊ42���ֽ�
	SetupComm(hCom, 42, 42);
	//PurgeComm������ֹ���еĺ�̨��д��������ս��ջ������ͷ��ͻ�����
//	PurgeComm(hCom, PURCE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	PurgeComm(hCom, PURGE_TXCLEAR|PURGE_RXCLEAR);
	//���ó�ʱ
	//COMMTIMEOUTS.ReadIntervalTimeout���������ַ���������ʱ��������λ������
	memset(&to, 0, sizeof(to));
	to.ReadIntervalTimeout = 1000;	//���������ַ�֮������ʱʱ�䣬���������
	to.ReadTotalTimeoutConstant = 1000;	//��ȡ�������ܳ�ʱ���ڵĳ��������������
	SetCommTimeouts(hCom, &to);
	if(!SetCommMask(hCom,EV_RXCHAR))
	{
		strErrInfo = "byteSendData SetCommMask err, id=" + GetLastError();
		theLog<<strErrInfo<<ende;
		return -2;
	}
	return 0;
};
//������ڻ�������0���������1:���ͣ�2�����գ�������ȫ��
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
/*	if(!SetCommMask(hCom,EV_TXEMPTY))//����������е������ѱ���ȫ�ͳ�
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
//		strErrInfo = "����������е������ѱ���ȫ�ͳ�, id=" + GetLastError();
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


