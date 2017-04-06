#pragma once
#include "operdobot.h"
#include "stdint.h"
typedef struct tagPTPCmd {
uint8_t ptpMode;
float x;
float y;
float z;
float r;
} PTPCmd;
typedef struct tagPose {
float x; //��е������ϵx
float y; //��е������ϵy
float z; //��е������ϵz
float r; //��е������ϵr
float jointAngle[4]; //��е��4 ��Ƕ�
} Pose;

class OperDobot2 :
	public OperDobot
{
private:
	//bool m_bHavingRecvData;
	Pose m_CurrentPos;
public:
	OperDobot2(void);
	virtual ~OperDobot2(void);
	void AddOrderList(DobotOrder &order);
	void Move2AbsolutePosition(const CDobotPoint &pt,float angle = 0.0);
	void SetGrab(bool b);
	void SetSpeed(float f1,float f2,float f3,float f4,float f5,float f6,float f7,float f8=0.0);
	int ReadFromDobot();
	void SendGetPoseOrder();
	int GetVersion(){ return 2; }
	virtual void PointMoving(int nAxis,bool bAngle);
	virtual void DobotStart();
};
