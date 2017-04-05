#include "stdafx.h"
#include "Comm.h"

SerialPort::SerialPort(){
		bIsOpen = false;
		bIsInit = false;
}
SerialPort::~SerialPort(){}




// �򿪴���
// ����: pPort - �������ƻ��豸·��������"COM1"��"//./COM1"���ַ�ʽ�������ú���
//       nBaudRate - ������
//       nParity - ��żУ��
//       nByteSize - �����ֽڿ��
//       nStopBits - ֹͣλ
BOOL SerialPort::OpenComm(CString strPort, int nBaudRate, int nParity, int nByteSize, int nStopBits)
{
	DCB dcb;		// ���ڿ��ƿ�
	COMMTIMEOUTS timeouts = {	// ���ڳ�ʱ���Ʋ���
		100,				// ���ַ������ʱʱ��: 100 ms
		1,					// ������ʱÿ�ַ���ʱ��: 1 ms (n���ַ��ܹ�Ϊn ms)
		500,				// ������(�����)����ʱʱ��: 500 ms
		1,					// д����ʱÿ�ַ���ʱ��: 1 ms (n���ַ��ܹ�Ϊn ms)
		100};				// ������(�����)д��ʱʱ��: 100 ms

	hComm = CreateFile((LPCTSTR)strPort,	// �������ƻ��豸·��
			GENERIC_READ | GENERIC_WRITE,	// ��д��ʽ
			0,				// ����ʽ����ռ
			NULL,			// Ĭ�ϵİ�ȫ������
			OPEN_EXISTING,	// ������ʽ
			0,				// ���������ļ�����
			NULL);			// �������ģ���ļ�
	
	if(hComm == INVALID_HANDLE_VALUE) return FALSE;		// �򿪴���ʧ��

	/*
	if(!bIsInit)
	{
		int nRet = InitComm();
		if(nRet < 0) 
			{
				CloseComm();
				return -1;
			}
		bIsOpen = true;
		bIsInit = true;
	}
	return TRUE;
	*/

	GetCommState(hComm, &dcb);		// ȡDCB

	dcb.BaudRate = nBaudRate;
	dcb.ByteSize = nByteSize;
	dcb.Parity = nParity;
	dcb.StopBits = nStopBits;

	SetCommState(hComm, &dcb);		// ����DCB

	SetupComm(hComm, 4096, 1024);	// �������������������С


	SetCommTimeouts(hComm, &timeouts);	// ���ó�ʱ
	PurgeComm(hComm, PURGE_TXCLEAR|PURGE_RXCLEAR);
	if(!SetCommMask(hComm,EV_RXCHAR))
	{
		CloseComm();
		return FALSE;
	}
	bIsOpen = true;
	bIsInit = true;
	return TRUE;
}

// �رմ���
BOOL SerialPort::CloseComm()
{
	bIsOpen = false;
	bIsInit = false;
	PurgeComm(hComm, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	return CloseHandle(hComm);
}

// д����
// ����: pData - ��д�����ݻ�����ָ��
//       nLength - ��д�����ݳ���
// ����: ʵ��д������ݳ���
int SerialPort::WriteComm(void* pData, int nLength)
{
	DWORD dwNumWrite;	// ���ڷ��������ݳ���

	WriteFile(hComm, pData, (DWORD)nLength, &dwNumWrite, NULL);

	return (int)dwNumWrite;
}

// ������
// ����: pData - ���������ݻ�����ָ��
//       nLength - ������������ݳ���
// ����: ʵ�ʶ��������ݳ���
int SerialPort::ReadComm(void* pData, int nLength)
{
	DWORD dwNumRead;	// �����յ������ݳ���

	ReadFile(hComm, pData, (DWORD)nLength, &dwNumRead, NULL);
	
	return (int)dwNumRead;
}

/*
int SerialPort::InitComm()
{
	if(!GetCommState(hComm,&dcb))
	{
		return -1;
	}
	dcb.BaudRate = CBR_9600; //������Ϊ9600
//	dcb.BaudRate = CBR_256000; //������Ϊ256000
	dcb.ByteSize = 8; //����λ��Ϊ8λ
	dcb.fBinary = TRUE;	//������
	dcb.StopBits = ONESTOPBIT; //һ��ֹͣλ
	if (!SetCommState(hComm, &dcb))
	{
		return -1;
	}
	//SetupComm(hCom, 1024, 1024);	//���ջ������ͷ��ͻ�������С��Ϊ42���ֽ�
	SetupComm(hComm, 42, 42);
	//PurgeComm������ֹ���еĺ�̨��д��������ս��ջ������ͷ��ͻ�����
//	PurgeComm(hCom, PURCE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	PurgeComm(hComm, PURGE_TXCLEAR|PURGE_RXCLEAR);
	//���ó�ʱ
	//COMMTIMEOUTS.ReadIntervalTimeout���������ַ���������ʱ��������λ������
	memset(&to, 0, sizeof(to));
	to.ReadIntervalTimeout = 1000;	//���������ַ�֮������ʱʱ�䣬���������
	to.ReadTotalTimeoutConstant = 1000;	//��ȡ�������ܳ�ʱ���ڵĳ��������������
	SetCommTimeouts(hComm, &to);
	if(!SetCommMask(hComm,EV_RXCHAR))
	{
		return -2;
	}
	return 0;
}
*/

