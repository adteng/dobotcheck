#include "StdAfx.h"
#include "ACard.h"
#include <math.h>
#include "Crc32.h"
#pragma  comment(lib,   "WINSCARD")

#define LCD_MODE		0  //1 use 0 close

unsigned char TAC[4];
unsigned char cSak=0,qbtype=0;
int Detype=0;

CACard::CACard(void)
{
	m_connActive = FALSE;
	m_strReader = "ACS ACR123U 0";
	m_iPayMoney = 1;
	DEVICE_SMARTCARD = 0x310000;
	IOCTL_SMARTCARD_ACR123U = 3500*4+DEVICE_SMARTCARD;
}

CACard::~CACard(void)
{
}

BOOL CACard::FindCard()
{
	BOOL bFind = FALSE;
	long retCode;
	retCode = SCardEstablishContext( SCARD_SCOPE_USER,NULL,NULL,&m_hContext );
	
	if( retCode != SCARD_S_SUCCESS )
	{
		m_logInfo("�Ҳ���USB������!");
		return FALSE;
	}

	//char tmpStr[256];
	DWORD Size = 256;
	
	//List all readers present and add it to the Combo box
	retCode = SCardListReaders( m_hContext,NULL,m_sReaderName,&Size);
	if( retCode != SCARD_S_SUCCESS )
	{
		m_logInfo("�Ҳ���USB�豸!");
		return FALSE;
	}
	char *pch = m_sReaderName;

	while (*pch != 0)
	{
		//m_cbReaderList.AddString (pch);
		if(strcmp(m_strReader.GetBuffer(0),pch) == 0)
		{
			bFind = TRUE;
			break;
		}
		pch += strlen (pch) + 1;
	}
	//Set the default reader
	//m_cbReaderList.SetCurSel(0); 
	m_IO_REQ.cbPciLength = sizeof(SCARD_IO_REQUEST);
	return bFind;
}


BOOL CACard::ConnectReader() 
{
	// TODO: Add your control notification handler code here
	
	long retCode;
	if(!m_connActive){
		//m_cbReaderList.GetWindowText(m_strReader);
		
		retCode = SCardConnect( m_hContext,m_strReader,SCARD_SHARE_DIRECT,0,&m_hCard, &m_PrefProtocol);
		
		if( retCode != SCARD_S_SUCCESS )
		{
			m_logInfo("����ʧ��!");
			return FALSE;
		} 

		m_connActive = TRUE;
		//m_connect.SetWindowText("�Ͽ�");
	}
	else{
		retCode = SCardDisconnect(m_hCard,SCARD_UNPOWER_CARD);
		if(retCode != SCARD_S_SUCCESS ){

		}
		//m_connect.SetWindowText("����");
		m_connActive = FALSE;
	}	
	m_linktype = 1;
	return m_connActive;
}

void CACard::ResetCard()
{
	long retCode;
	if(m_connActive==TRUE){
		retCode = SCardDisconnect(m_hCard, SCARD_UNPOWER_CARD);
		//m_connect.SetWindowText("����");
	}
	SCardReleaseContext	(m_hContext);
	//m_cbReaderList.ResetContent();
	m_connActive = FALSE;	
}


BOOL CACard::checkcard(unsigned long &bal,CString &strCardNo)//��ѯ���
{
	CString _str;
	CString strtemp;
	long retCode;
	//do
	//{

		retCode = PurchRequest();
		if(retCode !=0) return FALSE;
		if(m_cretcode!=0)
		{
			_str.Format("B4����%02X \n��ѯ���ʧ��!",m_cretcode);
			if(Detype==0)
			{
				m_logInfo(_str.GetBuffer(0));
				showerror(m_cretcode);
			}
			//else
			{
				//SetDlgItemText(IDC_INFO,_str);
				showerror(m_cretcode);
			}
			return FALSE;
		}	
		//PID
		_str="";
		if(m_linktype==0)
		{
			cSak = m_RecvBuff[33];
			qbtype = m_RecvBuff[0];
			_str.Format("%02X%02X%02X%02X%02X%02X%02X%02X",m_RecvBuff[1],m_RecvBuff[2],m_RecvBuff[3],m_RecvBuff[4],m_RecvBuff[5],m_RecvBuff[6],m_RecvBuff[7],m_RecvBuff[8]);	
		}
		else
		{
			cSak = m_RecvBuff[39];
			qbtype = m_RecvBuff[6];
			_str.Format("%02X%02X%02X%02X%02X%02X%02X%02X",m_RecvBuff[7],m_RecvBuff[8],m_RecvBuff[9],m_RecvBuff[10],m_RecvBuff[11],m_RecvBuff[12],m_RecvBuff[13],m_RecvBuff[14]);	
		}
		
		//SetDlgItemText(IDC_PID,_str);
		//LID
		_str="";
		if(m_linktype==0)
		{
			_str.Format("%02X%02X%02X%02X%02X%02X%02X%02X",m_RecvBuff[9],m_RecvBuff[10],m_RecvBuff[11],m_RecvBuff[12],m_RecvBuff[13],m_RecvBuff[14],m_RecvBuff[15],m_RecvBuff[16]);
		}
		else
		{
			_str.Format("%02X%02X%02X%02X%02X%02X%02X%02X",m_RecvBuff[15],m_RecvBuff[16],m_RecvBuff[17],m_RecvBuff[18],m_RecvBuff[19],m_RecvBuff[20],m_RecvBuff[21],m_RecvBuff[22]);
		}
		//_str.Format("%02X%02X%02X%02X%02X%02X%02X%02X",RecvBuff[15],RecvBuff[16],RecvBuff[17],RecvBuff[18],RecvBuff[19],RecvBuff[20],RecvBuff[21],RecvBuff[22]);
		//SetDlgItemText(IDC_LID,_str);
		strCardNo = _str;
		if(m_linktype==0)
		{
			bal =  (unsigned long)m_RecvBuff[17] << 24;
			bal |= (unsigned long)m_RecvBuff[18] << 16;
			bal |= (unsigned long)m_RecvBuff[19] << 8;
			bal |= (unsigned long)m_RecvBuff[20];
		}
		else
		{
			bal =  (unsigned long)m_RecvBuff[23] << 24;
			bal |= (unsigned long)m_RecvBuff[24] << 16;
			bal |= (unsigned long)m_RecvBuff[25] << 8;
			bal |= (unsigned long)m_RecvBuff[26];
		}

		_str="";
		switch(cSak)
		{
			case 0x08:
				_str.Format("M1 S50");
				break;
			case 0x18:
				_str.Format("M1 S70");
				break;
			case 0x20:
				_str.Format("CPU��");
				break;
			case 0x28:
				_str.Format("CPU��");
				break;
			default:
				_str.Format("��Ƭ����δ֪,%02X",qbtype);
				break;
		}
		//SetDlgItemText(IDC_CARD_INFO,_str);
		//Sleep(500);
	//}while(Detype);
		return TRUE;
}

long CACard::PurchRequest()
{
	long retCode;
	unsigned long unInitCRC = 0, unRtValue = 0;
	unsigned char ucChecked[4];
	int nRtn;
	unsigned char Cmd,RetCode,SendDataLen,RecvDataLen;

	memset(m_SendBuff,0,256);
	/*
	if(m_linktype==0)
	{
		//B4
		memset(m_SendBuff,0,256);
		memset(m_RecvBuff,0,256);
		Cmd = 0xb4;
		SendDataLen=0;
		RecvDataLen=0xFF;
		nRtn = ReaderCommand(Cmd,m_SendBuff,SendDataLen,&RetCode,m_RecvBuff,&RecvDataLen);
		if(nRtn != 0)
		{
			m_logInfo("��������Ѱ��ָ�����!");
			return -1;
		}
		m_cretcode = RetCode;
		return 0;
	}
	else
	*/
	{
		//FC010005B44221AEF4
		memcpy(m_SendBuff,"\xFC\x01\x00\x05\xb4",5);

		unRtValue = crc32(unInitCRC, m_SendBuff, 5);
		ucChecked[3] = (BYTE)(unRtValue & 0x000000FF);
		ucChecked[2] = (BYTE)((unRtValue >> 8) & 0x000000FF);
		ucChecked[1] = (BYTE)((unRtValue >> 16) & 0x000000FF);
		ucChecked[0] = (BYTE)((unRtValue >> 24) & 0x000000FF);
		memcpy(m_SendBuff+5,ucChecked,0x4);

		m_SendBuffLen = 0x09;
		m_RecvLength=0xff;
		retCode=SCardControl(m_hCard,IOCTL_SMARTCARD_ACR123U,m_SendBuff,m_SendBuffLen,m_RecvBuff,m_RecvLength,&m_RecvLength);
		if(retCode != SCARD_S_SUCCESS)
		{
			m_logInfo("��������Ѱ��ָ�����!");
			return -1;	
		}
		m_cretcode = m_RecvBuff[5];
		return 0;	
	}
}
/*
int CACard::ReaderCommand(unsigned char Cmd, unsigned char *SendData, unsigned char SendDataLen, unsigned char *RetCode, unsigned char *RecvData, unsigned char *RecvDataLen)
{
	CString str,_str;
	memset(ComPort,0,sizeof ComPort);
	int Return=0;
	int port = 1;//+m_com_list.GetCurSel();
	sprintf(ComPort,"COM%d",port);
	Return = SendReaderCommand(ComPort,Cmd,SendData,SendDataLen,RetCode,RecvData,RecvDataLen);

	_str.Empty();
	_str.Format("Cmd:%02X. RetCode:%02X. RecvDataLen:%d. REV DAT:",Cmd,*RetCode,*RecvDataLen);
	//theLog<<_str.GetBuffer(0)<<endi;
	_str.Empty();
	if(*RecvDataLen>0)
	{
		for(unsigned char i=0;i<*RecvDataLen;i++)
		{		
			str.Format("%02X",RecvData[i]);
			_str+=str;
		}
		//theLog<<_str.GetBuffer(0)<<endi;
	}
	return Return;
}
*/
void CACard::showerror(unsigned char cerror)
{
	CString _str;
	switch(cerror)
	{
		case 0x00:
			_str.Format("��������");
			break;
		case 0x60:
			_str.Format("û�а�װSAM��");
			break;
		case 0x61:	//
			_str.Format("SAM����ʼ�������δ��ʼ��");
			break;
		case 0x62:	//
			_str.Format("SAM������PIN����");
			break;
		case 0x63:	//
			_str.Format("SAM�������뽻�����Ͳ�ƥ��");
			break;
		case 0x64:	//
			_str.Format("SAM��ѡ���ļ�����");
			break;
		case 0x65:	//
			_str.Format("SAM��������");
			break;
		case 0x66:	//
			_str.Format("SAM��д����");
			break;
		case 0x67:	//
			_str.Format("SAM����֤����");
			break;
		case 0x68:	//
			_str.Format("SAM�����������");
			break;
		case 0x69:	//
			_str.Format("SAM��DES�������");
			break;
		case 0x6A:	//
			_str.Format("SAM������Ǯ����Կ����");
			break;
		case 0x71:	//
			_str.Format("PKI��RSA�������");
			break;
		case 0x72:	//
			_str.Format("PKI��DES�������");
			break;
		case 0x7E:	//
			_str.Format("SAM��ִ��APDU�������");
			break;
		case 0x7F:	//
			_str.Format("SAM��������ʱ");
			break;
		case 0x73:      //
			_str.Format("SAM���ϵ����");
			break;
		case 0xE0:	//
			_str.Format("MIFAREӲ����ʼ������");
			break;
		case 0xE1:	//
			_str.Format("SAMӲ����ʼ������");
			break;
		case 0xE2:	//
			_str.Format("�������");
			break;
		case 0xE3:	//
			_str.Format("��������");
			break;
		case 0xE4:	//
			_str.Format("����ʹ���");
			break;
		case 0xE5:	//
			_str.Format("��·ͨѶ��ʱ");
			break;
		case 0xE6:	//
			_str.Format("�ڲ�FLASHд����");
			break;
		case 0x80:	//
			_str.Format("û�п�");
			break;
		case 0x81:	//
			_str.Format("ѡ��Ƭ����");
			break;
		case 0x88:	//
			_str.Format("ѡ��Ƭ����");
			break;
		case 0x82:	//
			_str.Format("ͣ�ÿ�Ƭ����");
			break;
		case 0x83:	//
			_str.Format("��֤��Ƭ����");
			break;
		case 0x84:	//
			_str.Format("��Ƭ����������");
			break;
		case 0x85:	//
			_str.Format("��Ƭд��������");
			break;
		case 0x86:	//
			_str.Format("��Ƭд������;�ж�");
			break;
		case 0x87:	//
			_str.Format("��ֵ��Ƭ����Ӧ");
			break;
		case 0x0A:       //
			_str.Format("��cpu��");
			break;
		case 0x0B:       //
			_str.Format("��cpu��");
			break;
		case 0x0C:       //
			_str.Format("������Ȧ�泷������");
			break;
		case 0x0D:       //
			_str.Format("�����Ȧ������");
			break;
		case 0x0E:       //
			_str.Format("������Ȧ��Ԥ��������");
			break;
		case 0x1A:   //
			_str.Format("�����Ų�һ��");
			break;
		case 0x90:	//
			_str.Format("���Ǳ�ϵͳ��׼�Ŀ�Ƭ");
			break;
		case 0x91:	//
			_str.Format("��Ƭ������Ч��");
			break;
		case 0x92:	//
			_str.Format("���д����Ӧ�ô������");
			break;
		case 0x93:	//
			_str.Format("�Ƿ���");
			break;
		case 0x94:	//
			_str.Format("��������");
			break;
		case 0x95:	//
			_str.Format("Ǯ������");
			break;
		case 0x96:	//
			_str.Format("Ǯ����������");
			break;
		case 0x97:	//
			_str.Format("Ǯ��δ����");
			break;
		case 0x98:	//
			_str.Format("Ǯ����ͣ��");
			break;
		case 0x99:	//
			_str.Format("Ǯ���������ƻ�");
			break;
		case 0x9A:	//
			_str.Format("Ǯ����ͣ��");
			break;
		case 0x9F:	//
			_str.Format("������Ϣ�����ƻ�");
			break;
		case 0xAF:	//
			_str.Format("��Ƭ������ʱ");
			break;
		case 0xB0:	//
			_str.Format("���ײ�����;�ж�");
			break;
		case 0xB1:	//
			_str.Format("�����ж�");
			break;
		case 0xB2:	//
			_str.Format("ǰһ��ָ��δִ�л�ִ��ʧ��");
			break;
		case 0xC1:	//
			_str.Format("������ֵ���󱻾ܾ�");
			break;
		case 0xC2:	//
			_str.Format("������ֵ��֤ʧ��");
			break;
		case 0xC3:	//
			_str.Format("���׽���ύ����");
			break;
		case 0xCE:	//
			_str.Format("������ֵЭ�����");
			break;
		case 0xCF:	//
			_str.Format("���ײ�����ʱ");
			break;
		case 0xD0:	//
			_str.Format("Զ�̶�д��ִ�д�");
			break;
		case 0xD1:	//
			_str.Format("ָ���������Ż�����Ч");
			break;
		case 0x0F:       //
			_str.Format("��Ƭ������Ч�ڻ��߿�Ƭ��������ʱ��");
			break;
		case 0x3F:    //
			_str.Format("��֧�ֵ�����");
			break;
		default:
			_str.Format("δ֪����!�������[%02X].",cerror);
			break;
	}
	//if(Detype==0)
		m_logInfo(_str);
	//else
		//SetDlgItemText(IDC_INFO,_str);	
}


void CACard::Consume()
{
	CString _str;
	long retCode;

	unsigned long bal;
	unsigned long consumestart,consumetime;
	unsigned long errors=0;
	char cChar[512];
	int nMoney,iHigh,iLow;
	unsigned char cDat[10];
	int dsn;

	char pBuf_A[MAX_PATH];                                 //���·���ı��� 
	char pBuf_B[MAX_PATH];

	//_str.Empty();
	CTime	theTime=CTime::GetCurrentTime();
	memset(cChar,0,sizeof cChar);
	cChar[0] = Hex2BCD((unsigned char)(theTime.GetYear()/100));
	cChar[1] = Hex2BCD((unsigned char)(theTime.GetYear()%1000));
	cChar[2] = Hex2BCD(theTime.GetMonth());//month
	cChar[3] = Hex2BCD(theTime.GetDay());//day
	GetCurrentDirectory(MAX_PATH,pBuf_A);                   //��ȡ����ĵ�ǰĿ¼
	GetCurrentDirectory(MAX_PATH,pBuf_B);                   //��ȡ����ĵ�ǰĿ¼
	strcat(pBuf_A,"\\");
	strcat(pBuf_B,"\\");
	strcat(pBuf_A,"m1_jydata_");	
	strcat(pBuf_B,"cpu_jydata_");
	sprintf(cChar,"%02X%02X%02X%02X",cChar[0],cChar[1],cChar[2],cChar[3]);
	strcat(pBuf_A,cChar);
	strcat(pBuf_B,cChar);
	strcat(pBuf_A,".txt");
	strcat(pBuf_B,".txt");

	//SetDlgItemText(IDC_ERROR,"���������0");
	do{
		memset(cChar,0,sizeof cChar);
		//B4
		consumestart = GetTickCount();
		retCode = PurchRequest();
		if(retCode !=0) return;
		if(m_cretcode!=0)
		{
			_str.Format("B4����%02X \n��ѯ���ʧ��!",m_cretcode);
			//errors++;
			if(Detype==0)
			{
				m_logInfo(_str.GetBuffer(0));
				showerror(m_cretcode);
				return;
			}
			else
			{
				//SetDlgItemText(IDC_INFO,_str);
				showerror(m_cretcode);
				_str.Format("���������%d",errors);
				//SetDlgItemText(IDC_ERROR,_str);
				continue;	
			}			
		}	
		//PID
		_str="";
		if(m_linktype==0)
		{
			qbtype = m_RecvBuff[0];
			_str.Format("%02X%02X%02X%02X%02X%02X%02X%02X",m_RecvBuff[1],m_RecvBuff[2],m_RecvBuff[3],m_RecvBuff[4],m_RecvBuff[5],m_RecvBuff[6],m_RecvBuff[7],m_RecvBuff[8]);	
		}
		else
		{
			qbtype = m_RecvBuff[6];
			_str.Format("%02X%02X%02X%02X%02X%02X%02X%02X",m_RecvBuff[7],m_RecvBuff[8],m_RecvBuff[9],m_RecvBuff[10],m_RecvBuff[11],m_RecvBuff[12],m_RecvBuff[13],m_RecvBuff[14]);	
		}
		
		//SetDlgItemText(IDC_PID,_str);
		//LID
		_str="";
		if(m_linktype==0)
		{
			_str.Format("%02X%02X%02X%02X%02X%02X%02X%02X",m_RecvBuff[9],m_RecvBuff[10],m_RecvBuff[11],m_RecvBuff[12],m_RecvBuff[13],m_RecvBuff[14],m_RecvBuff[15],m_RecvBuff[16]);
		}
		else
		{
			_str.Format("%02X%02X%02X%02X%02X%02X%02X%02X",m_RecvBuff[15],m_RecvBuff[16],m_RecvBuff[17],m_RecvBuff[18],m_RecvBuff[19],m_RecvBuff[20],m_RecvBuff[21],m_RecvBuff[22]);
		}
		//_str.Format("%02X%02X%02X%02X%02X%02X%02X%02X",RecvBuff[15],RecvBuff[16],RecvBuff[17],RecvBuff[18],RecvBuff[19],RecvBuff[20],RecvBuff[21],RecvBuff[22]);
		//SetDlgItemText(IDC_LID,_str);
		if(m_linktype==0)
		{
			bal =  (unsigned long)m_RecvBuff[17] << 24;
			bal |= (unsigned long)m_RecvBuff[18] << 16;
			bal |= (unsigned long)m_RecvBuff[19] << 8;
			bal |= (unsigned long)m_RecvBuff[20];
		}
		else
		{
			bal =  (unsigned long)m_RecvBuff[23] << 24;
			bal |= (unsigned long)m_RecvBuff[24] << 16;
			bal |= (unsigned long)m_RecvBuff[25] << 8;
			bal |= (unsigned long)m_RecvBuff[26];
		}
		
		_str="";
		_str.Format("%ld.%02ld",bal/100,bal%100);	
		//SetDlgItemText(IDC_LAST_MONEY,_str);
	#if(LCD_MODE)
		_str.Empty();
		_str.Format("%ld.%02ld",bal/100,bal%100);
		len = 	_str.GetLength();	
		memset(tempstr,0,16);
		memcpy(tempstr,_str.GetBuffer(0),len);
		retCode = LcdMsg(tempstr,len);
		if(retCode!=0)
		{
		//	show("��ʾʧ��!!",FALSE);
		//	return;
		}
		if(cretcode!=0)
		{
		//	show("��ʾʧ��!!",FALSE);
		//	return;
		}
	#endif
		//B5
		retCode = YctSalePre();
		if(retCode !=0) return;
		if(m_cretcode!=0)
		{
			errors++;
			_str.Format("����Ԥ����ʧ��! \n B5����%02X",m_cretcode);
			if(Detype==0)
			{
				m_logInfo(_str.GetBuffer(0));
				showerror(m_cretcode);
				return;	
			}
			else
			{
				//SetDlgItemText(IDC_INFO,_str);	
				showerror(m_cretcode);
				_str.Format("���������%d",errors);
				//SetDlgItemText(IDC_ERROR,_str);
				continue;		
			}
		}
		if(qbtype==0x01)
		{
			memset(&m_PurchorgDat,0x00,sizeof(PURCH_ORG_DATA_A));
			//�ѻ�������ˮ��
			dsn = (int)m_TransBuff[0] << 24;//memcpy(TransBuff,RecvBuff+6,RecvDataLen);
			dsn |= (int)m_TransBuff[1] << 16;
			dsn |= (int)m_TransBuff[2] << 8;
			dsn |= (int)m_TransBuff[3];
			DecToBCD(dsn,m_PurchorgDat.DSN,sizeof(m_PurchorgDat.DSN));//4
			memcpy(m_PurchorgDat.ICN,&m_TransBuff[4],sizeof(m_PurchorgDat.ICN));//8
			memcpy(m_PurchorgDat.FCN,&m_TransBuff[12],sizeof(m_PurchorgDat.FCN));//4
			memcpy(m_PurchorgDat.LPID,&m_TransBuff[16],sizeof(m_PurchorgDat.LPID));//4
			memcpy(m_PurchorgDat.LTIM,&m_TransBuff[20],sizeof(m_PurchorgDat.LTIM));//7
			memcpy(m_PurchorgDat.PID,&m_TransBuff[27],sizeof(m_PurchorgDat.PID));//���ν����豸��  4
			memcpy(m_PurchorgDat.TIM,&m_TransBuff[31],sizeof(m_PurchorgDat.TIM));//���ν�������ʱ��7
			//���׽��
			int tf = (int)m_TransBuff[38] << 24;
			tf |= (int)m_TransBuff[39] << 16;
			tf |= (int)m_TransBuff[40] << 8;
			tf |= (int)m_TransBuff[41];
			DecToBCD(tf,m_PurchorgDat.TF,sizeof(m_PurchorgDat.TF));

			//�������
			tf = (int)m_TransBuff[42] << 24;
			tf |=(int)m_TransBuff[43] << 16;
			tf |=(int)m_TransBuff[44] << 8;
			tf |=(int)m_TransBuff[45];
			DecToBCD(tf,m_PurchorgDat.BAL,sizeof(m_PurchorgDat.BAL));

			m_PurchorgDat.TT = m_TransBuff[50];
			
			//memcpy(&mPurchorgDat.RN[1],RecvBuff+47,0x2);
			//Ʊ�����׼���
			int ts = m_TransBuff[51];
			ts <<=8;
			ts |= m_TransBuff[52];
			DecToBCD(ts,m_PurchorgDat.RN,sizeof(m_PurchorgDat.RN));
			memcpy(m_PurchorgDat.TPID,m_TransBuff+58,sizeof(m_PurchorgDat.TPID));
			memcpy(m_PurchorgDat.TTIM,m_TransBuff+62,sizeof(m_PurchorgDat.TTIM));

			m_PurchorgDat.AI[0]='0';
			m_PurchorgDat.AI[1]='0';

			m_PurchorgDat.VC='0';

			memcpy(m_PurchorgDat.TAC,m_TransBuff+69,sizeof m_PurchorgDat.TAC);
		}
		else if(qbtype==0x02)
		{
			memset(&m_cPurchorgDat,0x00,sizeof m_cPurchorgDat);
			memcpy(m_cPurchorgDat.PID,m_TransBuff,sizeof m_cPurchorgDat.PID);//6
			//�ն˽�����ˮ��
			memcpy(m_cPurchorgDat.DSN,m_TransBuff+6,sizeof m_cPurchorgDat.DSN);//4

			memcpy(m_cPurchorgDat.TIM,m_TransBuff+10,sizeof m_cPurchorgDat.TIM);//7
			memcpy(m_cPurchorgDat.ICN,m_TransBuff+17,sizeof m_cPurchorgDat.ICN);//8
			memcpy(m_cPurchorgDat.FCN,m_TransBuff+25,sizeof m_cPurchorgDat.FCN);//8
			memcpy(m_cPurchorgDat.TF, m_TransBuff+33,sizeof m_cPurchorgDat.TF); //4
			memcpy(m_cPurchorgDat.PTF,m_TransBuff+37,sizeof m_cPurchorgDat.PTF);//4
			memcpy(m_cPurchorgDat.BAL,m_TransBuff+41,sizeof m_cPurchorgDat.BAL);//4
			m_cPurchorgDat.TT = m_TransBuff[45];
			m_cPurchorgDat.ATT = m_TransBuff[46];
			memcpy(m_cPurchorgDat.RERN,m_TransBuff+47,sizeof m_cPurchorgDat.RERN);//2
			memcpy(m_cPurchorgDat.RN,m_TransBuff+49,sizeof m_cPurchorgDat.RN);//2
			memcpy(m_cPurchorgDat.CM,m_TransBuff+51,sizeof m_cPurchorgDat.CM);//2
			m_cPurchorgDat.BC = m_TransBuff[53];
			m_cPurchorgDat.RC = m_TransBuff[54];
			m_cPurchorgDat.LC = m_TransBuff[55];
			memcpy(m_cPurchorgDat.TPID,m_TransBuff+56,sizeof m_cPurchorgDat.TPID);//6
			memcpy(m_cPurchorgDat.TTIM,m_TransBuff+62,sizeof m_cPurchorgDat.TTIM);//7
			memcpy(m_cPurchorgDat.LPID,m_TransBuff+69,sizeof m_cPurchorgDat.LPID);//6
			memcpy(m_cPurchorgDat.LTIM,m_TransBuff+75,sizeof m_cPurchorgDat.LTIM);//4
			m_cPurchorgDat.ASN = m_TransBuff[79];
			memcpy(m_cPurchorgDat.ACT,m_TransBuff+80,sizeof m_cPurchorgDat.ACT);//2
			m_cPurchorgDat.ASN = m_TransBuff[82];
			//memcpy(cPurchorgDat.TAC,TransBuff+83,sizeof cPurchorgDat.TAC);//4
		}
		//B6
		retCode = PurchaseSale();
		if(retCode !=0) return;
		if(m_cretcode!=0)
		{
			errors++;
			_str.Format("ִ������ʧ��! \n B6����%02X",m_cretcode);
			if(Detype==0)
			{
				m_logInfo(_str);
				showerror(m_cretcode);
				return;
			}
			else
			{
				//SetDlgItemText(IDC_INFO,_str);	
				showerror(m_RecvBuff[5]);
				_str.Format("���������%d",errors);
				//SetDlgItemText(IDC_ERROR,_str);
				continue;	
			}
		}
		if(qbtype==0x2)
		{

			memcpy(m_cPurchorgDat.TAC,TAC,sizeof m_cPurchorgDat.TAC);
		}
		consumetime = GetTickCount()-consumestart;
		_str.Format("���ѳɹ�!  �����ܺ�ʱ%d����!",consumetime);
		//if(Detype==0)
		m_logInfo(_str.GetBuffer(0));
		//else
			//SetDlgItemText(IDC_INFO,_str);	
		if(qbtype==0x1)
		{
			//cChar[4] = '\t';
			//memcpy(cChar+5,cDataBuff+58,8);
			//�ѻ�������ˮ��
			sprintf(cChar,"%02X%02X%02X%02X",m_PurchorgDat.DSN[0],m_PurchorgDat.DSN[1],m_PurchorgDat.DSN[2],m_PurchorgDat.DSN[3]);
			strcat(cChar+8,"\t");
			//Ʊ����
			sprintf(cChar+9,"%02X%02X%02X%02X%02X%02X%02X%02X",m_PurchorgDat.ICN[0],m_PurchorgDat.ICN[1],m_PurchorgDat.ICN[2],m_PurchorgDat.ICN[3],m_PurchorgDat.ICN[4],m_PurchorgDat.ICN[5],m_PurchorgDat.ICN[6],m_PurchorgDat.ICN[7]);
			strcat(cChar+25,"\t");
			//������
			sprintf(cChar+26,"%02X%02X%02X%02X",m_PurchorgDat.FCN[0],m_PurchorgDat.FCN[1],m_PurchorgDat.FCN[2],m_PurchorgDat.FCN[3]);
			strcat(cChar+34,"\t");
			//�ϴ��豸���
			sprintf(cChar+35,"%02X%02X%02X%02X",m_PurchorgDat.LPID[0],m_PurchorgDat.LPID[1],m_PurchorgDat.LPID[2],m_PurchorgDat.LPID[3]);
			strcat(cChar+43,"\t");
			//�ϴν�������ʱ��
			sprintf(cChar+44,"%02X%02X%02X%02X%02X%02X%02X",m_PurchorgDat.LTIM[0],m_PurchorgDat.LTIM[1],m_PurchorgDat.LTIM[2],m_PurchorgDat.LTIM[3],m_PurchorgDat.LTIM[4],m_PurchorgDat.LTIM[5],m_PurchorgDat.LTIM[6]);
			strcat(cChar+58,"\t");
			//���ν����豸���
			sprintf(cChar+59,"%02X%02X%02X%02X",m_PurchorgDat.PID[0],m_PurchorgDat.PID[1],m_PurchorgDat.PID[2],m_PurchorgDat.PID[3]);
			strcat(cChar+67,"\t");
			//���ν�������ʱ��
			sprintf(cChar+68,"%02X%02X%02X%02X%02X%02X%02X",m_PurchorgDat.TIM[0],m_PurchorgDat.TIM[1],m_PurchorgDat.TIM[2],m_PurchorgDat.TIM[3],m_PurchorgDat.TIM[4],m_PurchorgDat.TIM[5],m_PurchorgDat.TIM[6]);
			strcat(cChar+82,"\t");
			//���׽��
			nMoney = 	BCDToDec(&m_PurchorgDat.TF[0],4);
			iHigh = nMoney/100;
			iLow = nMoney%100;
			//unsigned char cDat[10];
			memset(cDat,0,sizeof cDat);
			DecToBCD(iHigh,cDat,2);
			DecToBCD(iLow,cDat+2,1);
			sprintf(cChar+83,"0%02X%02X.%02X",cDat[0],cDat[1],cDat[2]);
			strcat(cChar+91,"\t");
			//�������
			nMoney = BCDToDec(&m_PurchorgDat.BAL[0],4);
			iHigh = nMoney/100;
			iLow = nMoney%100;
			DecToBCD(iHigh,cDat,2);
			DecToBCD(iLow,cDat+2,1);
			sprintf(cChar+92,"0%02X%02X.%02X",cDat[0],cDat[1],cDat[2]);
			strcat(cChar+100,"\t");
			//��������
			sprintf(cChar+101,"%02X",m_PurchorgDat.TT);
			strcat(cChar+103,"\t");
			//Ʊ�����׼���
			sprintf(cChar+104,"%X%02X%02X",m_PurchorgDat.RN[0],m_PurchorgDat.RN[1],m_PurchorgDat.RN[2]);
			strcat(cChar+109,"\t");
			//���ν�������豸���
			sprintf(cChar+110,"%02X%02X%02X%02X",m_PurchorgDat.PID[0],m_PurchorgDat.PID[1],m_PurchorgDat.PID[2],m_PurchorgDat.PID[3]);
			strcat(cChar+118,"\t");
			//���������������ʱ��
			sprintf(cChar+119,"%02X%02X%02X%02X%02X%02X%02X",m_PurchorgDat.TIM[0],m_PurchorgDat.TIM[1],m_PurchorgDat.TIM[2],m_PurchorgDat.TIM[3],m_PurchorgDat.TIM[4],m_PurchorgDat.TIM[5],m_PurchorgDat.TIM[6]);
			strcat(cChar+133,"\t");
			//������Ϣ
			sprintf(cChar+134,"%c%c",m_PurchorgDat.AI[0],m_PurchorgDat.AI[1]);
			strcat(cChar+136,"\t");
			
			sprintf(cChar+137,"%c",m_PurchorgDat.VC);
			strcat(cChar+138,"\t");
			sprintf(cChar+139,"%02X%02X%02X%02X",m_PurchorgDat.TAC[0],m_PurchorgDat.TAC[1],m_PurchorgDat.TAC[2],m_PurchorgDat.TAC[3]);
			strcat(cChar+147,"\r\n");
			//writeLog(pBuf_A,(char *)cChar);	
		}
		else
		{
			memset(cChar,0,sizeof cChar);
			//���ν����豸���6 chr(12)
			sprintf(cChar,"%02X%02X%02X%02X%02X%02X",m_cPurchorgDat.PID[0],m_cPurchorgDat.PID[1],m_cPurchorgDat.PID[2],m_cPurchorgDat.PID[3],m_cPurchorgDat.PID[4],m_cPurchorgDat.PID[5]);
			strcat(cChar+12,"\t");
			//�ն˽�����ˮ�� 4
			//int transinfo = 	BCDToDec(cDataBuff+60,4);
			unsigned int transinfo = (unsigned int)m_cPurchorgDat.DSN[0]<<24;
			transinfo |= (unsigned int)m_cPurchorgDat.DSN[1]<<16;
			transinfo |= (unsigned int)m_cPurchorgDat.DSN[2]<<8;
			transinfo |= (unsigned int)m_cPurchorgDat.DSN[3];
			memset(cDat,0,sizeof cDat);
			DecToBCD(transinfo,cDat,5);
			sprintf(cChar+13,"%02X%02X%02X%02X%02X",cDat[0],cDat[1],cDat[2],cDat[3],cDat[4]);
			strcat(cChar+23,"\t");
			//���ν�������ʱ��7
			sprintf(cChar+24,"%02X%02X%02X%02X%02X%02X%02X",m_cPurchorgDat.TIM[0],m_cPurchorgDat.TIM[1],m_cPurchorgDat.TIM[2],m_cPurchorgDat.TIM[3],m_cPurchorgDat.TIM[4],m_cPurchorgDat.TIM[5],m_cPurchorgDat.TIM[6]);
			strcat(cChar+38,"\t");
			//�߼����� 8
			sprintf(cChar+39,"%02X%02X%02X%02X%02X%02X%02X%02X",m_cPurchorgDat.ICN[0],m_cPurchorgDat.ICN[1],m_cPurchorgDat.ICN[2],m_cPurchorgDat.ICN[3],m_cPurchorgDat.ICN[4],m_cPurchorgDat.ICN[5],m_cPurchorgDat.ICN[6],m_cPurchorgDat.ICN[7]);
			strcat(cChar+55,"\t");
			//������8 
			sprintf(cChar+56,"%02X%02X%02X%02X%02X%02X%02X%02X",m_cPurchorgDat.FCN[0],m_cPurchorgDat.FCN[1],m_cPurchorgDat.FCN[2],m_cPurchorgDat.FCN[3],m_cPurchorgDat.FCN[4],m_cPurchorgDat.FCN[5],m_cPurchorgDat.FCN[6],m_cPurchorgDat.FCN[7]);
			strcat(cChar+72,"\t");
			//���׽��
			nMoney  = (int)m_cPurchorgDat.TF[0]<<24;
			nMoney |= (int)m_cPurchorgDat.TF[1]<<16;
			nMoney |= (int)m_cPurchorgDat.TF[2]<<8;
			nMoney |= (int)m_cPurchorgDat.TF[3];
			iHigh = nMoney/100;
			iLow  = nMoney%100;
			memset(cDat,0,sizeof cDat);
			DecToBCD(iHigh,cDat,2);
			DecToBCD(iLow,cDat+2,1);
			sprintf(cChar+73,"0%02X%02X.%02X",cDat[0],cDat[1],cDat[2]);
			strcat(cChar+81,"\t");
			//Ʊ��
			nMoney  = (int)m_cPurchorgDat.PTF[0]<<24;
			nMoney |= (int)m_cPurchorgDat.PTF[1]<<16;
			nMoney |= (int)m_cPurchorgDat.PTF[2]<<8;
			nMoney |= (int)m_cPurchorgDat.PTF[3];
			iHigh = nMoney/100;
			iLow = nMoney%100;
			memset(cDat,0, sizeof cDat);
			DecToBCD(iHigh,cDat,2);
			DecToBCD(iLow,cDat+2,1);
			sprintf(cChar+82,"0%02X%02X.%02X",cDat[0],cDat[1],cDat[2]);
			strcat(cChar+90,"\t");
			//�������
			nMoney  = (int)m_cPurchorgDat.BAL[0]<<24;
			nMoney |= (int)m_cPurchorgDat.BAL[1]<<16;
			nMoney |= (int)m_cPurchorgDat.BAL[2]<<8;
			nMoney |= (int)m_cPurchorgDat.BAL[3];
			iHigh = nMoney/100;
			iLow = nMoney%100;
			memset(cDat,0, sizeof cDat);
			DecToBCD(iHigh,cDat,2);
			DecToBCD(iLow,cDat+2,1);
			sprintf(cChar+91,"0%02X%02X.%02X",cDat[0],cDat[1],cDat[2]);
			strcat(cChar+99,"\t");	
			//��������
			sprintf(cChar+100,"%02X",m_cPurchorgDat.TT); 
			strcat(cChar+102,"\t");
			//���ӽ�������
			sprintf(cChar+103,"%02X",m_cPurchorgDat.ATT);
			strcat(cChar+105,"\t");
			//Ʊ����ֵ���׼���2
			nMoney = (int)m_cPurchorgDat.RERN[0]<<8;
			nMoney |=(int)m_cPurchorgDat.RERN[1];
			memset(cDat,0,sizeof cDat);
			DecToBCD(nMoney,cDat,3);
			sprintf(cChar+106,"%X%02X%02X",cDat[0],cDat[1],cDat[2]);
			strcat(cChar+111,"\t");
			//Ʊ�����ѽ��׼���2
			nMoney = (int)m_cPurchorgDat.RN[0]<<8;
			nMoney |=(int)m_cPurchorgDat.RN[1];
			memset(cDat,0,sizeof cDat);
			DecToBCD(nMoney,cDat,3);
			sprintf(cChar+112,"%X%02X%02X",cDat[0],cDat[1],cDat[2]);
			strcat(cChar+117,"\t");//117
			//�ۼ��ż��·�
			sprintf(cChar+118,"%02X%02X",m_cPurchorgDat.CM[0],m_cPurchorgDat.CM[1]);
			strcat(cChar+122,"\t");
			//�����ż�����
			memset(cDat,0,sizeof cDat);
			DecToBCD(m_cPurchorgDat.BC,cDat,2);
			sprintf(cChar+123,"%X%02X",cDat[0],cDat[1]);
			strcat(cChar+126,"\t");
			//�����ż�����
			memset(cDat,0,sizeof cDat);
			DecToBCD(m_cPurchorgDat.RC,cDat,2);
			sprintf(cChar+127,"%X%02X",cDat[0],cDat[1]);
			strcat(cChar+130,"\t");
			//�����ż�����
			memset(cDat,0,sizeof cDat);
			DecToBCD(m_cPurchorgDat.LC,cDat,2);
			sprintf(cChar+131,"%X%02X",cDat[0],cDat[1]);
			strcat(cChar+134,"\t");		
			//���ν�������豸���
			sprintf(cChar+135,"%02X%02X%02X%02X%02X%02X",m_cPurchorgDat.TPID[0],m_cPurchorgDat.TPID[1],m_cPurchorgDat.TPID[2],m_cPurchorgDat.TPID[3],m_cPurchorgDat.TPID[4],m_cPurchorgDat.TPID[5]);
			strcat(cChar+147,"\t");
			//���ν����������ʱ��
			sprintf(cChar+148,"%02X%02X%02X%02X%02X%02X%02X",m_cPurchorgDat.TTIM[0],m_cPurchorgDat.TTIM[1],m_cPurchorgDat.TTIM[2],m_cPurchorgDat.TTIM[3],m_cPurchorgDat.TTIM[4],m_cPurchorgDat.TTIM[5],m_cPurchorgDat.TTIM[6]);
			strcat(cChar+162,"\t");
			//�ϴν����豸���
			sprintf(cChar+163,"%02X%02X%02X%02X%02X%02X",m_cPurchorgDat.LPID[0],m_cPurchorgDat.LPID[1],m_cPurchorgDat.LPID[2],m_cPurchorgDat.LPID[3],m_cPurchorgDat.LPID[4],m_cPurchorgDat.LPID[5]);
			strcat(cChar+175,"\t");
			//�ϴν�������ʱ��
			sprintf(cChar+176,"%02X%02X%02X%02X",m_cPurchorgDat.LTIM[0],m_cPurchorgDat.LTIM[1],m_cPurchorgDat.LTIM[2],m_cPurchorgDat.LTIM[3]);
			strcat(cChar+184,"\t");
			//�������
			sprintf(cChar+185,"%02X",m_cPurchorgDat.ASN);
			strcat(cChar+187,"\t");
			//��������
			sprintf(cChar+188,"%02X%02X",m_cPurchorgDat.ACT[0],m_cPurchorgDat.ACT[1]);
			strcat(cChar+192,"\t");
			//��������
			sprintf(cChar+193,"%02X",m_cPurchorgDat.ASM);
			strcat(cChar+195,"\t");
			//������֤��
			sprintf(cChar+196,"%02X%02X%02X%02X",m_cPurchorgDat.TAC[0],m_cPurchorgDat.TAC[1],m_cPurchorgDat.TAC[2],m_cPurchorgDat.TAC[3]);
			strcat(cChar+204,"\r\n");
			//writeLog(pBuf_B,(char *)cChar);		
		}
	}while(Detype);
	//�����
	return;
	
}

unsigned char CACard::Hex2BCD(unsigned char cInData)
{
	unsigned char cOutData;

	//cOutData = ((cInData / 10) % 10) << 4 + (cInData % 10);
	cOutData = ((cInData / 10) % 10) * 16 + (cInData % 10);
	return cOutData;
}
int CACard::DecToBCD(int Dec, unsigned char *Bcd, int length)
{
    int tmp; 
    
    for(int i = length - 1; i >= 0; i--) 
    { 
        tmp = Dec % 100; 
        Bcd[i] = ((tmp / 10) << 4) + ((tmp % 10) & 0x0F); 
        Dec /= 100; 
    } 
    
    return 0; 
}
unsigned long CACard::BCDToDec(const unsigned char *bcd, int length)
{
    int tmp; 
    unsigned long dec = 0; 
    
    for(int i = 0; i < length; i++) 
    { 
        tmp = ((bcd[i] >> 4) & 0x0F) * 10 + (bcd[i] & 0x0F);    
        dec += tmp * (unsigned long)pow((float)100, length - 1 - i);           
    } 
    
    return dec; 
}

long CACard::YctSalePre()
{

	CString _str;
	int iPay;	
	long retCode;
	unsigned long unInitCRC = 0, unRtValue = 0;
	unsigned char ucChecked[4];
	int nRtn;
	unsigned char Cmd,RetCode,SendDataLen,RecvDataLen;

	memset(m_SendBuff,0,256);

	CTime	theTime=CTime::GetCurrentTime();
	//GetDlgItemText(IDC_IN_MONEY,_str);
	iPay = m_iPayMoney;//atoi(_str);

	memset(m_SendBuff,0,256);
	/*
	if(m_linktype==0)
	{
		memset(m_SendBuff,0,256);
		memset(m_RecvBuff,0,256);
		Cmd = 0xb5;

		m_SendBuff[0] = (unsigned char)(iPay >> 24);
		m_SendBuff[1] = (unsigned char)(iPay >> 16);
		m_SendBuff[2] = (unsigned char)(iPay >> 8);
		m_SendBuff[3] = (unsigned char)(iPay & 0xff);
		memcpy(m_SendBuff+4,m_SendBuff,4);

		m_SendBuff[8] = Hex2BCD((unsigned char)(theTime.GetYear()/100));//20
		m_SendBuff[9] = Hex2BCD((unsigned char)(theTime.GetYear()%1000));//11
		m_SendBuff[10] = Hex2BCD(theTime.GetMonth());//month
		m_SendBuff[11] = Hex2BCD(theTime.GetDay());//day
		m_SendBuff[12] = Hex2BCD(theTime.GetHour());//hour
		m_SendBuff[13] = Hex2BCD(theTime.GetMinute());//minute
		m_SendBuff[14] = Hex2BCD(theTime.GetSecond());//second
		m_SendBuff[15] = 0;//���ж�����ʱ��
		m_SendBuff[16] = 0;

		SendDataLen=17;
		RecvDataLen=0xFF;
		//BA 19  B5 00 00 00 01 00 00 00 01 20 12 10 30 15 50 09 00 10
		nRtn = ReaderCommand(Cmd,m_SendBuff,SendDataLen,&RetCode,m_RecvBuff,&RecvDataLen);
		if(nRtn != 0)
		{
			m_logInfo("����Ԥ����ָ�����!");
			return -1;
		}
		m_cretcode = RetCode;
		memcpy(m_TransBuff,m_RecvBuff,RecvDataLen);
		return 0;	
	}
	else
	*/
	{
		//FC010015B5000000010000000120121030155009001012BC1F20
		memcpy(m_SendBuff,"\xFC\x01\x00\x16\xb5",5);
		m_SendBuff[5] = (unsigned char)(iPay >> 24);
		m_SendBuff[6] = (unsigned char)(iPay >> 16);
		m_SendBuff[7] = (unsigned char)(iPay >> 8);
		m_SendBuff[8] = (unsigned char)(iPay & 0xff);
		memcpy(m_SendBuff+9,m_SendBuff+5,4);

		m_SendBuff[13] = Hex2BCD((unsigned char)(theTime.GetYear()/100));//20
		m_SendBuff[14] = Hex2BCD((unsigned char)(theTime.GetYear()%1000));//11
		m_SendBuff[15] = Hex2BCD(theTime.GetMonth());//month
		m_SendBuff[16] = Hex2BCD(theTime.GetDay());//day
		m_SendBuff[17] = Hex2BCD(theTime.GetHour());//hour
		m_SendBuff[18] = Hex2BCD(theTime.GetMinute());//minute
		m_SendBuff[19] = Hex2BCD(theTime.GetSecond());//second
		m_SendBuff[20] = 0;//���ж�����ʱ��
		m_SendBuff[21] = 0;

		unRtValue = crc32(unInitCRC, m_SendBuff, 22);
		ucChecked[3] = (BYTE)(unRtValue & 0x000000FF);
		ucChecked[2] = (BYTE)((unRtValue >> 8) & 0x000000FF);
		ucChecked[1] = (BYTE)((unRtValue >> 16) & 0x000000FF);
		ucChecked[0] = (BYTE)((unRtValue >> 24) & 0x000000FF);
		memcpy(m_SendBuff+22,ucChecked,0x4);

		m_SendBuffLen = 26;
		m_RecvLength=0xff;
		retCode=SCardControl(m_hCard,IOCTL_SMARTCARD_ACR123U,m_SendBuff,m_SendBuffLen,m_RecvBuff,m_RecvLength,&m_RecvLength);
		if(retCode != SCARD_S_SUCCESS)
		{
			m_logInfo("����Ԥ����ָ�����!");
			return -1;	
		}
		m_cretcode = m_RecvBuff[5];
		memcpy(m_TransBuff,m_RecvBuff+6,m_RecvLength-6);
		return 0;
	}
}

long CACard::PurchaseSale()
{	
	long retCode;
	unsigned long unInitCRC = 0, unRtValue = 0;
	unsigned char ucChecked[4];
	int nRtn;
	unsigned char Cmd,RetCode,SendDataLen,RecvDataLen;

	//FC010005B6AC2FCFD8
	memset(m_SendBuff,0,sizeof m_SendBuff);
	/*
	if(m_linktype==0)
	{
		memset(m_SendBuff,0,sizeof m_SendBuff);
		memset(m_RecvBuff,0,sizeof m_RecvBuff);
		Cmd = 0xb6;
		SendDataLen=0;
		RecvDataLen=0xFF;
		nRtn = ReaderCommand(Cmd,m_SendBuff,SendDataLen,&RetCode,m_RecvBuff,&RecvDataLen);
		if(nRtn != 0)
		{
			m_logInfo("����ִ������ָ�����!");
			return -1;
		}
		m_cretcode = RetCode;
		memcpy(TAC,m_RecvBuff,4);
		return 0;
	}
	else
	*/
	{
		memcpy(m_SendBuff,"\xFC\x01\x00\x05\xB6",0x5);

		unRtValue = crc32(unInitCRC, m_SendBuff, 0x5);
		ucChecked[3] = (BYTE)(unRtValue & 0x000000FF);
		ucChecked[2] = (BYTE)((unRtValue >> 8) & 0x000000FF);
		ucChecked[1] = (BYTE)((unRtValue >> 16) & 0x000000FF);
		ucChecked[0] = (BYTE)((unRtValue >> 24) & 0x000000FF);
		memcpy(m_SendBuff+0x5,ucChecked,0x4);

		m_SendBuffLen = 9;
		m_RecvLength=0xff;
		retCode=SCardControl(m_hCard,IOCTL_SMARTCARD_ACR123U,m_SendBuff,m_SendBuffLen,m_RecvBuff,m_RecvLength,&m_RecvLength);
		if(retCode != SCARD_S_SUCCESS)
		{
			m_logInfo("����ִ������ָ�����!");
			return -1;	
		}
		m_cretcode = m_RecvBuff[5];
		memcpy(TAC,m_RecvBuff+6,4);
		return 0;	
	}
}
