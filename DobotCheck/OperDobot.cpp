#include "stdafx.h"
#include "OperDobot.h"

//OperDobot m_OperDobot;
unsigned int __stdcall ReadCom(void * param);
OperDobot::OperDobot()
{
	bIsOpen = false;
	bList = false;
	m_bRunning = false;
	m_bGrab = false;
	pDWS = new DobotAndWinStatus;
	memset(pDWS->cErrInfo,0,sizeof(pDWS->cErrInfo));
	m_CMDList.clear();
	m_BaudRate = CBR_9600;
};

OperDobot::~OperDobot()
{
	delete pDWS;
};

bool OperDobot::OpenCom()
{
	if(!bIsOpen)
	{
		//打开串口
		int nRet = Open(m_strCom);
		if(nRet < 0)
		{
			strErrInfo = "OPEN "+m_strCom+" error! " + GetErrInfo();
			(*logInfo)(strErrInfo.c_str());
			memset(pDWS->cErrInfo,0,sizeof(pDWS->cErrInfo));sprintf(pDWS->cErrInfo,("Code=001,"+strErrInfo).c_str());
			return bIsOpen;
		}
		bIsOpen = true;
		lRecv = 0;
		bStatus = false;
	}
	string str = "open " + m_strCom + " succeed";
	(*logInfo)(str.c_str());
	return bIsOpen;
};

void OperDobot::SetCOM(const string& strCom)
{
//	memset(&m_DobotOrder,0,sizeof(DobotOrder));
	m_strCom = strCom;
	return ;
};

int OperDobot::ReadFromDobot()
{
	int i,nRet;
	if(!bStatus)
	{
		byte byTmp[DOBOTDATABUFFERSIZE*2];

		memset(byteDobot2PCData,0,DOBOTDATABUFFERSIZE+1);
		for(lRecv=0;lRecv<4;lRecv++)
		{
			if((nRet = byteReadData(byteDobot2PCData,DOBOTDATABUFFERSIZE)) < 0)
			{
				strErrInfo = "Read SerialPort Timeout! " + GetErrInfo();
				theLog<<strErrInfo<<ende;(*logInfo)(strErrInfo.c_str());
				memset(pDWS->cErrInfo,0,sizeof(pDWS->cErrInfo));sprintf(pDWS->cErrInfo,("Code=002,"+strErrInfo).c_str());
				return -1;
			}
			//Sleep(100);
		}
		memcpy(byTmp,byteDobot2PCData,DOBOTDATABUFFERSIZE);
		if((nRet = byteReadData(byteDobot2PCData,DOBOTDATABUFFERSIZE)) < 0)
		{
			strErrInfo = "Read SerialPort Timeout! " + GetErrInfo();
			//theLog<<strErrInfo<<ende;
			(*logInfo)(strErrInfo.c_str());
			return -1;
		}
		memcpy(byTmp+DOBOTDATABUFFERSIZE,byteDobot2PCData,DOBOTDATABUFFERSIZE);
		if((byTmp[DOBOTDATABUFFERSIZE] == 0xA5) && (byTmp[DOBOTDATABUFFERSIZE*2-1] == 0x5A))//从1开始，确保取到最后一条
		{
			m_DobotAndWinStatus.m_DS = (DobotStatus &)byTmp[1];
			bStatus = true;
		}else{
			for(i=(DOBOTDATABUFFERSIZE-1);i>0;i--)
			{
				if((byTmp[i] == 0xA5) && (byTmp[DOBOTDATABUFFERSIZE+i-1] == 0x5A))
				{
					theDobotStatusCritical.Lock();
					m_DobotAndWinStatus.m_DS = (DobotStatus &)byTmp[i];
					theDobotStatusCritical.Unlock();
					//Sleep(60);
					if((nRet = byteReadData(byteDobot2PCData,i)) < 0)//丢弃
					{
						strErrInfo = "Code=200,Read SerialPort Timeout! " + GetErrInfo();
						theLog<<strErrInfo<<ende;(*logInfo)(strErrInfo.c_str());
						return -1;
					}
					bStatus = true;
					break;
				}
			}
		}
	}else{
		//Sleep(100);
		memset(byteDobot2PCData,0,DOBOTDATABUFFERSIZE+1);
		if((nRet = byteReadData(byteDobot2PCData,DOBOTDATABUFFERSIZE)) < 0)
		{
			strErrInfo = "Read SerialPort Timeout! " + GetErrInfo();
			theLog<<strErrInfo<<ende;(*logInfo)(strErrInfo.c_str());
			 (*logInfo)(strErrInfo.c_str());
			return -1;
		}
		if((byteDobot2PCData[0] == 0xA5) && (byteDobot2PCData[DOBOTDATABUFFERSIZE-1] == 0x5A))
		{
			theDobotStatusCritical.Lock();
			//m_DobotAndWinStatus.m_DS = (DobotStatus &)byteDobot2PCData[0];
			memcpy(&m_DobotAndWinStatus.m_DS,byteDobot2PCData,DOBOTDATABUFFERSIZE);
			theDobotStatusCritical.Unlock();
//			m_DobotAndWinStatus.nLooped = 0;
		}else{
			char sLog[128] = {0};
			sprintf(sLog,"%s,%d,%0x,%0x","COM Error! len,a,b=",nRet,byteDobot2PCData[0],byteDobot2PCData[DOBOTDATABUFFERSIZE-1]);
			//theLogCritical.Lock();
			//theLog<<strErrInfo<<ende;
			//theLog<<"COM Error! ReConnect Now......"<<ende;
			(*logInfo)(sLog);
			//theLogCritical.Unlock();
			return -9;
		}
	}
	return 0;
}
int OperDobot::SendOrder2Dobot(DobotOrder &order)
{
	if((order.cHead[0] == 0xA5) && (order.cEnd[0] = 0x5A))
	{
		int nRet = byteSendData(&order.cHead[0],DOBOTDATABUFFERSIZE);
		if(nRet < 0)
		{
			strErrInfo = "byteSendData error! Code="+IToString(nRet) + " " + GetErrInfo();
			//theLogCritical.Lock();
			//theLog<<"Code=100,byteSendData:"<<strErrInfo<<ende; 
			logInfo("byteSendData data failure");
			//theLogCritical.Unlock();
			//theDobotOrderCritical.Unlock();
			return -2;
		}
	}
	return 0;
}
CDobotPoint OperDobot::GetCurrentAbsolutePosition()
{
	CDobotPoint p;
	theDobotStatusCritical.Lock();
	p.x = Hex_To_Decimal(m_DobotAndWinStatus.m_DS.cX,4);
	p.y = Hex_To_Decimal(m_DobotAndWinStatus.m_DS.cY,4);
	p.z = Hex_To_Decimal(m_DobotAndWinStatus.m_DS.cZ,4);
	theDobotStatusCritical.Unlock();
	return p;
}
CAngle OperDobot::GetCurrentCAnglePosition()
{
	CAngle p;
	theDobotStatusCritical.Lock();
	p.x = Hex_To_Decimal(m_DobotAndWinStatus.m_DS.cBaseAngel,4);
	p.y = Hex_To_Decimal(m_DobotAndWinStatus.m_DS.cLongAngel,4);
	p.z = Hex_To_Decimal(m_DobotAndWinStatus.m_DS.cShortAngel,4);
	theDobotStatusCritical.Unlock();
	return p;
}
CDobotPoint OperDobot::GetStartAbsolutePosition()
{
	CDobotPoint p;
	p.x = Hex_To_Decimal(m_DobotStartStatus.cX,4);
	p.y = Hex_To_Decimal(m_DobotStartStatus.cY,4);
	p.z = Hex_To_Decimal(m_DobotStartStatus.cZ,4);
	return p;
}
void OperDobot::Move2AbsolutePosition(const CDobotPoint &pt,float angle)
{
	DobotOrder order;
	memset(&order,0,sizeof(order));
	order.cHead[0] = 0xA5;
	order.cEnd[0] = 0x5A;
	FloatToByte(3.0,order.cState);
	FloatToByte(0.0,order.cAxis);
	FloatToByte(pt.x,order.cX);
	FloatToByte(pt.y,order.cY);
	FloatToByte(pt.z,order.cZ);
	FloatToByte(angle,order.cRHead);
	if(m_bGrab)
		FloatToByte(1.0,order.cIsGrip);
	else
		FloatToByte(0.0,order.cIsGrip);
	FloatToByte(1,order.cStartVe);
	FloatToByte(0.0,order.cEndVel);
	FloatToByte(0.0,order.cMaxVe);//pause  second
	AddOrderList(order);  


	int iLoop = 0;
	while(1)
	{
		if(OperDobot::IsSameDobotPoint(pt,GetCurrentAbsolutePosition())) break;
		if(iLoop++ > 1200) break;
		Sleep(5);
	}
}
void OperDobot::Move2AnglePoint(const CAngle &pt,float angle,float startVe)
{
	if(angle > 90) angle = 90;
	DobotOrder order;
	memset(&order,0,sizeof(order));
	order.cHead[0] = 0xA5;
	order.cEnd[0] = 0x5A;
	FloatToByte(6.0,order.cState);
	FloatToByte(0.0,order.cAxis);
	FloatToByte(pt.x,order.cX);
	FloatToByte(pt.y,order.cY);
	FloatToByte(pt.z,order.cZ);
	FloatToByte(angle,order.cRHead);
	if(m_bGrab)
		FloatToByte(1.0,order.cIsGrip);
	else
		FloatToByte(0.0,order.cIsGrip);
	FloatToByte(startVe,order.cStartVe);
	FloatToByte(0.0,order.cEndVel);
	FloatToByte(0.0,order.cMaxVe);
	AddOrderList(order); 
}
void OperDobot::Go2StartPoint()
{
	CDobotPoint pt = {267.784,0.000,-67.886};
	Move2AbsolutePosition(pt);
}
CDobotPoint OperDobot::Move2RelativePosition(const CRelativeSpace &sp)
{
	//CDobotPoint p0 = GetCurrentAbsolutePosition();
	CDobotPoint p1;
	p1.x = m_CurrentMarkedDobotPoint.x + sp.x;
	p1.y = m_CurrentMarkedDobotPoint.y + sp.y;
	p1.z = m_CurrentMarkedDobotPoint.z + sp.z;
	Move2AbsolutePosition(p1);
	return p1;
}
CDobotPoint OperDobot::MarkADobotPiont()
{
	m_CurrentMarkedDobotPoint = GetCurrentAbsolutePosition();
	return m_CurrentMarkedDobotPoint;
}
void OperDobot::SendOrderFromList()
{
	m_csList.Lock();
	CMDLIST::iterator begin = m_CMDList.begin();
	if(begin!=m_CMDList.end())
	{
		DobotOrder order = *begin;
		m_CMDList.erase(begin);
		m_csList.Unlock();
		SendOrder2Dobot(order);
	}
	else
		m_csList.Unlock();
}
void OperDobot::AddOrderList(DobotOrder &order)
{
	m_csList.Lock();
	m_CMDList.push_back(order);
	m_csList.Unlock();
}

void OperDobot::PointMoving(int nAxis,bool bAngle)
{
	float fTmp = 70.0;
	char sBuffer[16];
	unsigned char cBuf[5];
	DobotOrder m_NewDobotOrder;
	memset(&m_NewDobotOrder,0,sizeof(DobotOrder));

	memset(sBuffer,0,16);
	memset(cBuf,0,5);

	FloatToByte(fTmp,cBuf);

	m_NewDobotOrder.cHead[0] = 0xA5;
	if(bAngle)
	{
		FloatToByte(2.0,m_NewDobotOrder.cState);//stringToHEX("00000040",m_NewDobotOrder.cState,8);//2
	}else{
		FloatToByte(7.0,m_NewDobotOrder.cState);//stringToHEX("0000E040",m_NewDobotOrder.cState,8);//7
	}
	switch(nAxis)
	{
	case 0:
		FloatToByte(0.0,m_NewDobotOrder.cAxis);//stringToHEX("00000000",m_NewDobotOrder.cAxis,8); 
		memset(cBuf,0,5);
		break;
	case 1:
		FloatToByte(1.0,m_NewDobotOrder.cAxis);//stringToHEX("0000803F",m_NewDobotOrder.cAxis,8); 
		break;
	case 2:
		FloatToByte(2.0,m_NewDobotOrder.cAxis);//stringToHEX("00000040",m_NewDobotOrder.cAxis,8); 
		break;
	case 3:
		FloatToByte(3.0,m_NewDobotOrder.cAxis);//stringToHEX("00004040",m_NewDobotOrder.cAxis,8); 
		break;
	case 4:
		FloatToByte(4.0,m_NewDobotOrder.cAxis);//stringToHEX("00008040",m_NewDobotOrder.cAxis,8); 
		break;
	case 5:
		FloatToByte(5.0,m_NewDobotOrder.cAxis);//stringToHEX("0000A040",m_NewDobotOrder.cAxis,8); 
		break;
	case 6:
		FloatToByte(6.0,m_NewDobotOrder.cAxis);//stringToHEX("0000C040",m_NewDobotOrder.cAxis,8); 
		break;
	case 7:
		FloatToByte(7.0,m_NewDobotOrder.cAxis);//stringToHEX("0000E040",m_NewDobotOrder.cAxis,8); 
		break;
	case 8:
		FloatToByte(8.0,m_NewDobotOrder.cAxis);//stringToHEX("00000041",m_NewDobotOrder.cAxis,8);
		break;
	case 9:
		FloatToByte(9.0,m_NewDobotOrder.cAxis);//stringToHEX("00001041",m_NewDobotOrder.cAxis,8);
		break;
	case 10:
		FloatToByte(10.0,m_NewDobotOrder.cAxis);//stringToHEX("00002041",m_NewDobotOrder.cAxis,8);
		break;
	case 11:
		FloatToByte(11.0,m_NewDobotOrder.cAxis);//stringToHEX("00003041",m_NewDobotOrder.cAxis,8);
		break;
	case 12:
		FloatToByte(12.0,m_NewDobotOrder.cAxis);//stringToHEX("00004041",m_NewDobotOrder.cAxis,8);
		break;
	case 13:
		FloatToByte(13.0,m_NewDobotOrder.cAxis);//stringToHEX("00005041",m_NewDobotOrder.cAxis,8);
		break;
	case 14:
		FloatToByte(14.0,m_NewDobotOrder.cAxis);//stringToHEX("00006041",m_NewDobotOrder.cAxis,8);
		break;
	default:
		break;
	}
	FloatToByte(0,m_NewDobotOrder.cX);//stringToHEX("00000000",m_NewDobotOrder.cX,8);//0
	FloatToByte(0,m_NewDobotOrder.cY);//stringToHEX("00000000",m_NewDobotOrder.cY,8);
	FloatToByte(0,m_NewDobotOrder.cZ);//stringToHEX("00000000",m_NewDobotOrder.cZ,8);
	FloatToByte(0,m_NewDobotOrder.cRHead);//stringToHEX("00000000",m_NewDobotOrder.cRHead,8);
	FloatToByte(0,m_NewDobotOrder.cIsGrip);//stringToHEX("00000000",m_NewDobotOrder.cIsGrip,8);
	memcpy(m_NewDobotOrder.cStartVe,cBuf,4);			//点动速度比例
	FloatToByte(0,m_NewDobotOrder.cEndVel);//stringToHEX("00000000",m_NewDobotOrder.cEndVel,8);
	FloatToByte(0,m_NewDobotOrder.cMaxVe);//stringToHEX("00000000",m_NewDobotOrder.cMaxVe,8);
	m_NewDobotOrder.cEnd[0] = 0x5A;
	AddOrderList(m_NewDobotOrder);
}
void OperDobot::SetGrab(bool b) 
{
	DobotOrder m_NewDobotOrder;
	m_NewDobotOrder.cHead[0] = 0xA5;
	stringToHEX("00000040",m_NewDobotOrder.cState,8);//2

	
	if(b)
		stringToHEX("00001041",m_NewDobotOrder.cAxis,8);
	else
		stringToHEX("00002041",m_NewDobotOrder.cAxis,8);
	
	stringToHEX("00000000",m_NewDobotOrder.cX,8);//0
	stringToHEX("00000000",m_NewDobotOrder.cY,8);
	stringToHEX("00000000",m_NewDobotOrder.cZ,8);
	stringToHEX("00000000",m_NewDobotOrder.cRHead,8);
	stringToHEX("00000000",m_NewDobotOrder.cIsGrip,8);
	//memcpy(m_NewDobotOrder.cStartVe,cBuf,4);			//点动速度比例
	FloatToByte(50,m_NewDobotOrder.cStartVe);
	stringToHEX("00000000",m_NewDobotOrder.cEndVel,8);
	stringToHEX("00000000",m_NewDobotOrder.cMaxVe,8);
	m_NewDobotOrder.cEnd[0] = 0x5A;
	AddOrderList(m_NewDobotOrder);
	m_bGrab = b;
}



void OperDobot::DobotStart()
{
	m_bRunning = true;
	unsigned int nThreadAddr;
	HANDLE	hSession = (HANDLE)_beginthreadex(0, 0, ReadCom, this, 0, &nThreadAddr);
	if(hSession) CloseHandle( hSession );
}
void OperDobot::DobotStop()
{
	m_bRunning = false;
}
/*
void OperDobot::SetSpeed(int iFlag)
{
	DobotOrder order;
	memset(&order,0,sizeof(order));
	order.cHead[0] = 0xA5;
	order.cEnd[0] = 0x5A;
	FloatToByte(9,order.cState);
	FloatToByte(1,order.cAxis);
	FloatToByte(400,order.cX);
	FloatToByte(400,order.cY);
	FloatToByte(400,order.cZ);
	FloatToByte(400,order.cRHead);
	FloatToByte(400,order.cIsGrip);
	FloatToByte(400,order.cStartVe);
	FloatToByte(0.0,order.cEndVel);
	FloatToByte(0.0,order.cMaxVe);
	AddOrderList(order); 
}
*/
void OperDobot::SetSpeed(float f1,float f2,float f3,float f4,float f5,float f6,float f7,float f8)
{
	DobotOrder order;
	memset(&order,0,sizeof(order));
	order.cHead[0] = 0xA5;
	order.cEnd[0] = 0x5A;
	FloatToByte(9,order.cState);
	FloatToByte(1,order.cAxis);
	FloatToByte(f1,order.cX);
	FloatToByte(f2,order.cY);
	FloatToByte(f3,order.cZ);
	FloatToByte(f4,order.cRHead);
	FloatToByte(f5,order.cIsGrip);
	FloatToByte(f6,order.cStartVe);
	FloatToByte(f7,order.cEndVel);
	FloatToByte(f8,order.cMaxVe);
	AddOrderList(order); 
}

unsigned int __stdcall ReadCom(void * param)
{
	int nRet=0,i=0;
	OperDobot *Opd = (OperDobot *)param;
	
	if(!Opd->OpenCom())
	{
		Opd->m_bRunning = false;
		return -1;
	}
	Opd->SendOrderFromList();
	while(Opd->m_bRunning)
	{
		if((nRet = Opd->ReadFromDobot()) < 0)
		{
			//Opd->m_bRunning = false;
			Opd->logInfo("read data failure");
			if(nRet==-9)
			{
				Opd->CloseCom();
				Sleep(2000);
				//Opd->m_bRunning = true;
				if(!Opd->OpenCom())
				{
					Opd->logInfo("reopen failure");
					Opd->m_bRunning = false;
					return -1;
				}
				Opd->logInfo("reopen succeed");
			}
			else//没数据
			{
				Opd->SendOrderFromList();
				Sleep(10);
				continue;
			}
		}
		static char sLog[128] = {0};
		static int iT = 0;
		float f1,f2,f3,f4,f5,f6,f7,f8,f9,f10;
		Opd->theDobotStatusCritical.Lock();
		f1 = Hex_To_Decimal(Opd->m_DobotAndWinStatus.m_DS.cX,4);
		f2 = Hex_To_Decimal(Opd->m_DobotAndWinStatus.m_DS.cY,4);
		f3 = Hex_To_Decimal(Opd->m_DobotAndWinStatus.m_DS.cZ,4);
		f4 = Hex_To_Decimal(Opd->m_DobotAndWinStatus.m_DS.cR,4);
		f5 = Hex_To_Decimal(Opd->m_DobotAndWinStatus.m_DS.cBaseAngel,4);
		f6 = Hex_To_Decimal(Opd->m_DobotAndWinStatus.m_DS.cLongAngel,4);
		f7 = Hex_To_Decimal(Opd->m_DobotAndWinStatus.m_DS.cShortAngel,4);
		f8 = Hex_To_Decimal(Opd->m_DobotAndWinStatus.m_DS.cPawAngel,4);
		f9 = Hex_To_Decimal(Opd->m_DobotAndWinStatus.m_DS.cIsGrip,4);
		f10 = Hex_To_Decimal(Opd->m_DobotAndWinStatus.m_DS.cGripperAngel,4);
		if(iT++ < 5) Opd->m_DobotStartStatus = Opd->m_DobotAndWinStatus.m_DS;
		Opd->theDobotStatusCritical.Unlock();
		sprintf(sLog,"%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f",f1,f2,f3,f4,f5,f6,f7,f8,f9,f10);
		Opd->logInfo(sLog);
		Opd->ShowDobotStatus(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10);
		Opd->SendOrderFromList();
		if(Opd->GetVersion() > 1) 
			Sleep(5);
		else
			Sleep(10);
	}
	Opd->CloseCom();

//	_endthreadex(0);

	return 0;
}

