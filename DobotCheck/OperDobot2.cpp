#include "StdAfx.h"
#include "OperDobot2.h"

unsigned int __stdcall SendGetPoseData(void * param);
unsigned int __stdcall ReadData(void * param);
OperDobot2::OperDobot2(void)
{
	m_BaudRate = CBR_115200;
}
OperDobot2::~OperDobot2(void)
{
}

void OperDobot2::SendGetPoseOrder()
{
	uint8_t *cmd = new uint8_t[6];// = {0};
	cmd[0] = 0xAA;
	cmd[1] = 0xAA;
	cmd[2] = 2;
	cmd[3] = 10;
	cmd[4] = 0;

	uint8_t c = 0;
	for(int i = 0; i< 2; i++)
	{
		c += cmd[3+i];
	}
	cmd[5] = 256 - c;
	m_csList.Lock();
	int nRet = WriteComm(cmd,6);
	m_csList.Unlock();
	if(nRet !=6 )
	{
		logInfo("byteSendData  SendGetPoseOrder data failure");
	}
	/*
	for(int i = 0; i < 6; i++)
	{
		TRACE("%02X",cmd[i]);
	}
	*/
	delete [] cmd;
}
void OperDobot2::Move2AbsolutePosition(const CDobotPoint &pt,float angle)
{
	PTPCmd cmdInfo;
	memset(&cmdInfo,0,sizeof(PTPCmd));
	uint8_t *cmd = new uint8_t[23]; //{0};
	cmd[0] = 0xAA;
	cmd[1] = 0xAA;
	cmd[2] = 19;
	cmd[3] = 84;
	cmd[4] = 0x03;
	cmdInfo.ptpMode = 1;
	cmdInfo.x = pt.x;
	cmdInfo.y = pt.y;
	cmdInfo.z = pt.z;
	cmdInfo.r = 0;
	memcpy(cmd+5,(uint8_t *)&cmdInfo.ptpMode,1);
	memcpy(cmd+6,(uint8_t *)&cmdInfo.x,4);
	memcpy(cmd+10,(uint8_t *)&cmdInfo.y,4);
	memcpy(cmd+14,(uint8_t *)&cmdInfo.z,4);
	memcpy(cmd+18,(uint8_t *)&cmdInfo.r,4);
	int8_t c = 0;
	for(int i = 0; i< 19; i++)
	{
		c += cmd[3+i];
	}
	cmd[22] = 256 - c;
	m_csList.Lock();
	int nRet = WriteComm(cmd,23);
	m_csList.Unlock();
	if(nRet < 23)
	{
		logInfo("byteSendData data failure");
	}
	else if(nRet == 23)
	{
		logInfo("byteSendData data succeed");
	}
	for(int i = 0; i < 23; i++)
	{
		TRACE("%02X",cmd[i]);
	}
	TRACE("\n");
	delete [] cmd;
	
	int iLoop = 0;
	while(1)
	{
		if(OperDobot::IsSameDobotPoint(pt,GetCurrentAbsolutePosition())) break;
		if(iLoop++ > 1200) break;
		Sleep(10);
	}
}

void OperDobot2::SetGrab(bool b)
{
	uint8_t *cmd = new uint8_t[8];
	cmd[0] = 0xAA;
	cmd[1] = 0xAA;
	cmd[2] = 4;
	cmd[3] = 62;
	cmd[4] = 0x01;
	if(b)
	{
		cmd[5] = 1;
		cmd[6] = 1;
	}
	else
	{
		cmd[5] = 0;
		cmd[6] = 0;
	}
	int8_t c = 0;
	for(int i = 0; i< 4; i++)
	{
		c += cmd[3+i];
	}
	cmd[7] = 256 - c;
	m_csList.Lock();
	int nRet = WriteComm(cmd,8);
	m_csList.Unlock();
	delete [] cmd;
}
void OperDobot2::SetSpeed(float f1,float f2,float f3,float f4,float f5,float f6,float f7,float f8)
{
	uint8_t *cmd = new uint8_t[38];
	cmd[0] = 0xAA;
	cmd[1] = 0xAA;
	cmd[2] = 34;
	cmd[3] = 71;
	cmd[4] = 0x01;
	memcpy(cmd+5,(uint8_t *)&f1,4);
	memcpy(cmd+9,(uint8_t *)&f3,4);
	memcpy(cmd+13,(uint8_t *)&f5,4);
	memcpy(cmd+17,(uint8_t *)&f7,4);
	memcpy(cmd+21,(uint8_t *)&f2,4);
	memcpy(cmd+25,(uint8_t *)&f4,4);
	memcpy(cmd+29,(uint8_t *)&f6,4);
	memcpy(cmd+33,(uint8_t *)&f8,4);
	int8_t c = 0;
	for(int i = 0; i< 34; i++)
	{
		c += cmd[3+i];
	}
	cmd[37] = 256 - c;
	m_csList.Lock();
	int nRet = WriteComm(cmd,38);
	m_csList.Unlock();
	delete [] cmd;
}
int OperDobot2::ReadFromDobot()
{
	int iRet = 0;
	if(!bStatus)
	{
		byte head[3] = {0};
		memset(head,0,3);
		int iTotalLen = 3;
		int iRecvLen = 0;
		while(iRecvLen < 3)
		{
			int iLen = 0;
			if((iLen = ReadComm(head + iRecvLen,iTotalLen-iRecvLen)) < 0)
			{
				(*logInfo)("read data head err");
				return -9;
			}
			else if(iLen == 0)
				return 0;
			iRecvLen += iLen;
		}
		if(iRecvLen < 3)
		{
			return -1;
		}

		iTotalLen = head[2] + 1;
		iRecvLen =0;
		uint8_t *pData = new uint8_t[iTotalLen + 1];
		while(iRecvLen < iTotalLen)
		{
			int iLen = 0;
			if((iLen = ReadComm(pData + iRecvLen,iTotalLen-iRecvLen)) <= 0)
			{
				(*logInfo)("read data err");
				break;
			}
			iRecvLen += iLen;
		}
		if(iRecvLen < iTotalLen)
		{
			delete [] pData;
			return -9;
		}
		int id = pData[0];
		int8_t c = 0;
		for(int i = 0;i < iTotalLen-1;i++)
		{
			c += pData[i];
		}
		int8_t d =  pData[iTotalLen-1];
		if(c + d == 0)
		{
			//(*logInfo)("check succeed");
			static int iT = 0;
			static char sLog[128] = {0};
			//m_bHavingRecvData = true;
			switch(id)
			{
			case 10:
				//(*logInfo)("pos");
				theDobotStatusCritical.Lock();
				memcpy((char *)&m_CurrentPos,pData+2,sizeof(Pose));
				
				FloatToByte(m_CurrentPos.x,m_DobotAndWinStatus.m_DS.cX);
				FloatToByte(m_CurrentPos.y,m_DobotAndWinStatus.m_DS.cY);
				FloatToByte(m_CurrentPos.z,m_DobotAndWinStatus.m_DS.cZ);
				FloatToByte(m_CurrentPos.r,m_DobotAndWinStatus.m_DS.cR);
				FloatToByte(m_CurrentPos.jointAngle[0],m_DobotAndWinStatus.m_DS.cBaseAngel);
				FloatToByte(m_CurrentPos.jointAngle[1],m_DobotAndWinStatus.m_DS.cLongAngel);
				FloatToByte(m_CurrentPos.jointAngle[2],m_DobotAndWinStatus.m_DS.cShortAngel);
				FloatToByte(m_CurrentPos.jointAngle[3],m_DobotAndWinStatus.m_DS.cPawAngel);
				
		float f1,f2,f3,f4,f5,f6,f7,f8,f9,f10;
	
		f1 =  m_CurrentPos.x;// Hex_To_Decimal(m_DobotAndWinStatus.m_DS.cX,4);
		f2 =  m_CurrentPos.y;//Hex_To_Decimal(m_DobotAndWinStatus.m_DS.cY,4);
		f3 = m_CurrentPos.z;//Hex_To_Decimal(m_DobotAndWinStatus.m_DS.cZ,4);
		f4 =  m_CurrentPos.r;//Hex_To_Decimal(m_DobotAndWinStatus.m_DS.cR,4);
		f5 = m_CurrentPos.jointAngle[0];//	Hex_To_Decimal(m_DobotAndWinStatus.m_DS.cBaseAngel,4);
		f6 = m_CurrentPos.jointAngle[1];//Hex_To_Decimal(m_DobotAndWinStatus.m_DS.cLongAngel,4);
		f7 = m_CurrentPos.jointAngle[2];//Hex_To_Decimal(m_DobotAndWinStatus.m_DS.cShortAngel,4);
		f8 = m_CurrentPos.jointAngle[3];//Hex_To_Decimal(m_DobotAndWinStatus.m_DS.cPawAngel,4);
		f9 = 0;//Hex_To_Decimal(m_DobotAndWinStatus.m_DS.cIsGrip,4);
		f10 = 0;//Hex_To_Decimal(m_DobotAndWinStatus.m_DS.cGripperAngel,4);
		//if(iT++ < 5) m_DobotStartStatus = m_DobotAndWinStatus.m_DS;
		theDobotStatusCritical.Unlock();
		//sprintf(sLog,"%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f",f1,f2,f3,f4,f5,f6,f7,f8,f9,f10);
		//logInfo(sLog);
		ShowDobotStatus(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10);

				break;
				
			default:
				break;
			}
			delete [] pData;
			return 0;
		}
		else
			(*logInfo)("check failure");
		delete [] pData;
	}
	return -1;
}



void OperDobot2::DobotStart()
{
	if(OpenCom())
	{
		(*logInfo)("open com succeed");
	}
	else
	{
		(*logInfo)("open com err");
		m_bRunning = false;
		return;
	}
	m_bRunning = true;
	unsigned int nThreadAddr1,nThreadAddr2;
	HANDLE	hSession = (HANDLE)_beginthreadex(0, 0, SendGetPoseData, this, 0, &nThreadAddr1);
	if(hSession) CloseHandle( hSession );
	Sleep(5);
	hSession = (HANDLE)_beginthreadex(0, 0,  ReadData, this, 0, &nThreadAddr2);
	if(hSession) CloseHandle( hSession );
}

typedef struct tagJOGCmd {
uint8_t isJoint;
uint8_t cmd;
}JOGCmd;
void OperDobot2::PointMoving(int nAxis,bool bAngle)
{
	JOGCmd jogCmd;
	if(bAngle)
		jogCmd.isJoint = 1;
	else
		jogCmd.isJoint = 0;
	jogCmd.cmd = nAxis;
	uint8_t *cmd = new uint8_t[8]; 
	cmd[0] = 0xAA;
	cmd[1] = 0xAA;
	cmd[2] = 4;
	cmd[3] = 73;
	cmd[4] = 0x01;
	memcpy(cmd + 5,(uint8_t *)&jogCmd,2);
	uint8_t c = 0;
	for(int i = 0; i< 4; i++)
	{
		c += cmd[3+i];
	}
	cmd[7] = 256 - c;
	m_csList.Lock();
	int nRet = WriteComm(cmd,8);
	m_csList.Unlock();
	delete [] cmd;
}

unsigned int __stdcall SendGetPoseData(void * param)
{
	OperDobot2 *Opd = (OperDobot2 *)param;
	while(Opd->m_bRunning)
	{
		Opd->SendGetPoseOrder();
		Sleep(8);
	}
	return 0;
}

unsigned int __stdcall ReadData(void * param)
{
	OperDobot *Opd = (OperDobot *)param;
	while(Opd->m_bRunning)
	{
		if(Opd->ReadFromDobot() < 0)
		{
			Opd->m_bRunning = false;
		}
		Sleep(3);
	}
	Opd->CloseComm();
	return 0;
}

void OperDobot2::AddOrderList(DobotOrder &order)
{
	if(order.cIsGrip)
		SetGrab(true);
	else 
		SetGrab(false);
	CDobotPoint pt;
	pt.x = Hex_To_Decimal(order.cX, 4);
	pt.y = Hex_To_Decimal(order.cY, 4);
	pt.z = Hex_To_Decimal(order.cZ, 4);
	Move2AbsolutePosition(pt,0);
}



