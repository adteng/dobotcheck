#ifndef _ACARD_H_
#define _ACARD_H_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "winscard.h"
//#include "commdll.h"

typedef struct tagPurchOrgDataA //tsRec[88] m1
{
	unsigned char DSN[4]; 	//脱机交易流水号; 	8(Char)        4(BCD)-
	unsigned char ICN[8];	//票卡逻辑卡号    	16(Char)       8(HEX)- 
	unsigned char FCN[4];	//票卡物理卡号    	8(Char)        4(HEX)- 
	unsigned char LPID[4];	//上次交易设备号  	8(Char)        4(HEX)-
	unsigned char LTIM[7];	//上次交易日期时间	14(Char)       7(BCD)- 
	unsigned char PID[4];	//本次交易设备号  	8(Char)        4(HEX)-
	unsigned char TIM[7];	//本次交易日期时间	14(Char)       7(BCD)- 
	unsigned char TF[4];	//交易金额        	8(Char)        4(HEX)- 
	unsigned char BAL[4];	//本次余额        	8(Char)        4(HEX)- 
	unsigned char TT; 		//交易类型        	2(Char)        1(HEX)-                                        
	unsigned char RN[3];    //票卡交易计数    	3(Char)        3(BCD)-  
	unsigned char TPID[4];  //本次交易入口设备编号8(Char)	   4(BCD)-
	unsigned char TTIM[7];	//本次交易入口日期时间14(Char)   	BCD	7	以本次交易日期时间填充	
	unsigned char AI[2];  	//分帐信息        	2(Char)        2(HEX)- 	
	unsigned char VC;     	//校验码          	1(Char)        1(HEX)-                        
	unsigned char TAC[4];   //交易验证码        8(Char)        4(HEX)       
}  PURCH_ORG_DATA_A;
typedef struct tagPurchOrgDataB //tsRec[88] cpu
{
	unsigned char PID[6];	//本次交易设备号  	6  010099992625
	unsigned char DSN[4]; 	//脱机交易流水号; 	4
	unsigned char TIM[7];	//本次交易日期时间	7(BCD)
	unsigned char ICN[8];	//票卡逻辑卡号    	8(BCD) 
	unsigned char FCN[8];	//票卡物理卡号    	8(HEX) 
	unsigned char TF[4];	//交易金额        	4(HEX)
	unsigned char PTF[4];	//票价        		4(HEX)
	unsigned char BAL[4];	//本次余额        	4(HEX)
	unsigned char TT; 		//交易类型        	1(HEX) 06消费
	unsigned char ATT; 		//附加交易类型      1(HEX) 0x17为普通消费
	unsigned char RERN[2];	//票卡充值交易计数	2(HEX)
	unsigned char RN[2];    //票卡交易计数    	2(HEX)
	unsigned char CM[2];	//累计门槛月份		2(BCD)
	unsigned char BC;		//公交门槛计数		1(HEX)
	unsigned char RC;		//地铁门槛计数		1(HEX)
	unsigned char LC;		//联乘门槛计数		1(HEX)
	unsigned char TPID[6];  //本次交易入口设备编号6(BCD)
	unsigned char TTIM[7];	//本次交易入口日期时间  	BCD	7	以本次交易日期时间填充	
	unsigned char LPID[6];	//上次交易设备号  	6(HEX)
	unsigned char LTIM[4];	//上次交易日期时间	4(BCD)	
	unsigned char ASN;		//区域代码			1(BCD)
	unsigned char ACT[2];	//区域卡类型		2(BCD)
	unsigned char ASM;		//区域子码			1(BCD)
	unsigned char TAC[4];   //交易验证码        4(HEX) 
}  PURCH_ORG_DATA_B;


class CACard
{
public:
	CACard(void);
	~CACard(void);
	BOOL FindCard();
	BOOL ConnectReader();
	void ResetCard();
	BOOL checkcard(unsigned long &bal,CString &strCardNo);//查询余额
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

