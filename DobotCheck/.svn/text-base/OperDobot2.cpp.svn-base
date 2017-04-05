#include "StdAfx.h"
#include "OperDobot2.h"
#include "stdint.h"

typedef struct tagPTPCmd {
uint8_t ptpMode;
float x;
float y;
float z;
float r;
} PTPCmd;
typedef struct tagPose {
float x; //机械臂坐标系x
float y; //机械臂坐标系y
float z; //机械臂坐标系z
float r; //机械臂坐标系r
float jointAngle[4]; //机械臂4 轴角度
} Pose;
unsigned int __stdcall SendGetPoseData(void * param);
OperDobot2::OperDobot2(void)
{
	m_BaudRate = CBR_115200;
	//unsigned int nThreadAddr;
	//HANDLE	hSession = (HANDLE)_beginthreadex(0, 0, SendGetPoseData, this, 0, &nThreadAddr);
	//if(hSession) CloseHandle( hSession );
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
	int nRet = byteSendData(cmd,6);
	m_csList.Unlock();
	if(nRet < 0)
	{
		logInfo("byteSendData data failure");
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
	int nRet = byteSendData(cmd,23);
	m_csList.Unlock();
	if(nRet < 0)
	{
		logInfo("byteSendData data failure");
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
		Sleep(5);
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
	int nRet = byteSendData(cmd,8);
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
	int nRet = byteSendData(cmd,38);
	m_csList.Unlock();
	delete [] cmd;
}
void OperDobot2::SendOrderFromList()
{
	//if(!m_bHavingRecvData)
	SendGetPoseOrder();
}
int OperDobot2::ReadFromDobot()
{
	int iRet = 0;
	if(!bStatus)
	{

//while(1)
//{
		byte head[3] = {0};
		memset(head,0,3);
		int iTotalLen = 3;
		int iRecvLen = 0;
		while(iRecvLen < 3)
		{
			int iLen = 0;
			if((iLen = byteReadData(head + iRecvLen,iTotalLen-iRecvLen)) < 0)
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
			if((iLen = byteReadData(pData + iRecvLen,iTotalLen-iRecvLen)) <= 0)
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
			(*logInfo)("check succeed");
			//m_bHavingRecvData = true;
			switch(id)
			{
			case 10:
				Pose p;
				memcpy(&p,pData+2,sizeof(Pose));
				theDobotStatusCritical.Lock();
				FloatToByte(p.x,m_DobotAndWinStatus.m_DS.cX);
				FloatToByte(p.y,m_DobotAndWinStatus.m_DS.cY);
				FloatToByte(p.z,m_DobotAndWinStatus.m_DS.cZ);
				FloatToByte(p.r,m_DobotAndWinStatus.m_DS.cR);
				FloatToByte(p.jointAngle[0],m_DobotAndWinStatus.m_DS.cBaseAngel);
				FloatToByte(p.jointAngle[1],m_DobotAndWinStatus.m_DS.cLongAngel);
				FloatToByte(p.jointAngle[2],m_DobotAndWinStatus.m_DS.cShortAngel);
				FloatToByte(p.jointAngle[3],m_DobotAndWinStatus.m_DS.cPawAngel);
				theDobotStatusCritical.Unlock();
				break;
				
			default:
				break;
			}
		}
		else
			(*logInfo)("check failure");
		delete [] pData;
//}
	}
	return 0;
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
	int nRet = byteSendData(cmd,8);
	m_csList.Unlock();
	delete [] cmd;
}

unsigned int __stdcall SendGetPoseData(void * param)
{
	OperDobot2 *Opd = (OperDobot2 *)param;
	while(1)
	{
		if(Opd->m_bRunning)
		{
			Opd->SendGetPoseOrder();
			Sleep(50);
		}
		else
			Sleep(100);
	}
}

