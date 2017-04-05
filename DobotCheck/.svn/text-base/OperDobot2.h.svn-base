#pragma once
#include "operdobot.h"

class OperDobot2 :
	public OperDobot
{
private:
	//bool m_bHavingRecvData;
public:
	OperDobot2(void);
	virtual ~OperDobot2(void);
	void Move2AbsolutePosition(const CDobotPoint &pt,float angle = 0.0);
	void SetGrab(bool b);
	void SetSpeed(float f1,float f2,float f3,float f4,float f5,float f6,float f7,float f8=0.0);
	int ReadFromDobot();
	void SendOrderFromList();
	void SendGetPoseOrder();
	int GetVersion(){ return 2; }
	virtual void PointMoving(int nAxis,bool bAngle);
};
