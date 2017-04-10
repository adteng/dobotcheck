#ifndef SCAND_OP_H
#define SCAND_OP_H
#pragma once
#include "OperDobot.h"
#include "ACard.h"
#include <vector>

using namespace std;

typedef struct 
{
	CDobotPoint dp_point;
	int iPointStatus;//0--Î´¿ªÊ¼¼ì²â 1--ÕýÔÚ¼ì²â 2--¼ì²â³É¹¦ 3--¼ì²âÊ§°Ü
} WorkingPointInf;

typedef struct
{
	int m_iDevL;
	int m_iDevW;
	int m_iDevH;
	int m_iFeeValue;
} DevInfo;

typedef struct
{
	float m_fXLen;
	float m_fYLen;
	int m_iX_Sum;
	int m_iY_Sum;
	float m_fLayerSpaceLen;
	int m_iLayer_Sum;
} SystemInfo;

typedef vector<WorkingPointInf> SPACE;

class CScandOP
{
private:
	
public:
	CScandOP(void);
	virtual ~CScandOP(void);
	OperDobot *m_pOperDobot;
	CACard *m_pCardOP;
	WorkingPointInf m_iWorkingPoint[9][9][5];
	SPACE m_space;
	CDobotPoint m_FirstScandPoint;
	DevInfo m_devInfo;
	SystemInfo m_sysInfo;
	BOOL m_bRunning;
	void (*pLayerChange)(int);
	void (*pViewChange)(char *);
	void SetupPosition(float fXLen,float fYLen,int iX_sum,int iY_sum,int iZ_sum,const CDobotPoint &firstPoint);
	void TestRegion();
	void AppParameter();
	void ScandReading();
	void ScandNoFee1();
	void ScandNoFee2();
};
#endif

