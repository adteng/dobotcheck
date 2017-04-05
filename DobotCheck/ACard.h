#ifndef _ACARD_H_
#define _ACARD_H_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "winscard.h"
//#include "commdll.h"

typedef struct tagPurchOrgDataA //tsRec[88] m1
{
	unsigned char DSN[4]; 	//�ѻ�������ˮ��; 	8(Char)        4(BCD)-
	unsigned char ICN[8];	//Ʊ���߼�����    	16(Char)       8(HEX)- 
	unsigned char FCN[4];	//Ʊ��������    	8(Char)        4(HEX)- 
	unsigned char LPID[4];	//�ϴν����豸��  	8(Char)        4(HEX)-
	unsigned char LTIM[7];	//�ϴν�������ʱ��	14(Char)       7(BCD)- 
	unsigned char PID[4];	//���ν����豸��  	8(Char)        4(HEX)-
	unsigned char TIM[7];	//���ν�������ʱ��	14(Char)       7(BCD)- 
	unsigned char TF[4];	//���׽��        	8(Char)        4(HEX)- 
	unsigned char BAL[4];	//�������        	8(Char)        4(HEX)- 
	unsigned char TT; 		//��������        	2(Char)        1(HEX)-                                        
	unsigned char RN[3];    //Ʊ�����׼���    	3(Char)        3(BCD)-  
	unsigned char TPID[4];  //���ν�������豸���8(Char)	   4(BCD)-
	unsigned char TTIM[7];	//���ν����������ʱ��14(Char)   	BCD	7	�Ա��ν�������ʱ�����	
	unsigned char AI[2];  	//������Ϣ        	2(Char)        2(HEX)- 	
	unsigned char VC;     	//У����          	1(Char)        1(HEX)-                        
	unsigned char TAC[4];   //������֤��        8(Char)        4(HEX)       
}  PURCH_ORG_DATA_A;
typedef struct tagPurchOrgDataB //tsRec[88] cpu
{
	unsigned char PID[6];	//���ν����豸��  	6  010099992625
	unsigned char DSN[4]; 	//�ѻ�������ˮ��; 	4
	unsigned char TIM[7];	//���ν�������ʱ��	7(BCD)
	unsigned char ICN[8];	//Ʊ���߼�����    	8(BCD) 
	unsigned char FCN[8];	//Ʊ��������    	8(HEX) 
	unsigned char TF[4];	//���׽��        	4(HEX)
	unsigned char PTF[4];	//Ʊ��        		4(HEX)
	unsigned char BAL[4];	//�������        	4(HEX)
	unsigned char TT; 		//��������        	1(HEX) 06����
	unsigned char ATT; 		//���ӽ�������      1(HEX) 0x17Ϊ��ͨ����
	unsigned char RERN[2];	//Ʊ����ֵ���׼���	2(HEX)
	unsigned char RN[2];    //Ʊ�����׼���    	2(HEX)
	unsigned char CM[2];	//�ۼ��ż��·�		2(BCD)
	unsigned char BC;		//�����ż�����		1(HEX)
	unsigned char RC;		//�����ż�����		1(HEX)
	unsigned char LC;		//�����ż�����		1(HEX)
	unsigned char TPID[6];  //���ν�������豸���6(BCD)
	unsigned char TTIM[7];	//���ν����������ʱ��  	BCD	7	�Ա��ν�������ʱ�����	
	unsigned char LPID[6];	//�ϴν����豸��  	6(HEX)
	unsigned char LTIM[4];	//�ϴν�������ʱ��	4(BCD)	
	unsigned char ASN;		//�������			1(BCD)
	unsigned char ACT[2];	//��������		2(BCD)
	unsigned char ASM;		//��������			1(BCD)
	unsigned char TAC[4];   //������֤��        4(HEX) 
}  PURCH_ORG_DATA_B;


class CACard
{
public:
	CACard(void);
	~CACard(void);
	BOOL FindCard();
	BOOL ConnectReader();
	void ResetCard();
	BOOL checkcard(unsigned long &bal,CString &strCardNo);//��ѯ���
	long PurchRequest();
	//int ReaderCommand(unsigned char Cmd, unsigned char *SendData, unsigned char SendDataLen, unsigned char *RetCode, unsigned char *RecvData, unsigned char *RecvDataLen);
	void showerror(unsigned char cerror);
	void Consume();
	unsigned char Hex2BCD(unsigned char cInData);
	int DecToBCD(int Dec, unsigned char *Bcd, int length);
	unsigned long BCDToDec(const unsigned char *bcd, int length);
	long YctSalePre();
	long PurchaseSale();
	void SetLogInfoFun(void (*f)(const char *)){ m_logInfo =  f;}
protected:
	SCARDCONTEXT  m_hContext;
	char m_sReaderName[256];
	char ComPort[10];
	BYTE m_SendBuff[256];
	BYTE m_RecvBuff[256];
	BYTE m_TransBuff[256];
	SCARD_IO_REQUEST m_IO_REQ;
	BOOL m_connActive;
	CString m_strReader;
	SCARDHANDLE m_hCard;
	DWORD m_PrefProtocol;
	unsigned char m_cretcode;
	unsigned char m_linktype;//0:COM, 1:usb 
	unsigned long m_SendBuffLen;
	unsigned long m_RecvLength;
	int m_iPayMoney;
	PURCH_ORG_DATA_A m_PurchorgDat;
	PURCH_ORG_DATA_B m_cPurchorgDat;
unsigned int DEVICE_SMARTCARD;
unsigned int IOCTL_SMARTCARD_ACR123U;
void (*m_logInfo)(const char *);
};
#endif

