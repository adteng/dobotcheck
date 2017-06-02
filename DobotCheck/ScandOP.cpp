#include "StdAfx.h"
#include "ScandOP.h"


CScandOP::CScandOP(void)
{
	m_bRunning = false;
}


CScandOP::~CScandOP(void)
{
}

void CScandOP::SetupPosition(float fR1,float fR2,float fH, float fBH,const CDobotPoint &firstPoint)
{
	m_space.clear();
	WorkingPointInf wi;
	wi.dp_point = firstPoint;
	wi.iPointStatus = 0;
	
	wi.dp_point.y -= fR1;
	m_space.push_back(wi);
	wi.dp_point.y += fR1;
	wi.dp_point.x -= fR1;
	m_space.push_back(wi);
	wi.dp_point.x += fR1;
	wi.dp_point.y += fR1;
	m_space.push_back(wi);
	wi.dp_point.y -= fR1;
	wi.dp_point.x += fR1;
	m_space.push_back(wi);
	wi.dp_point.x -= fR1;
	m_space.push_back(wi);

	for(int i = 0; i < 3; i++)
	{
	wi.dp_point.z += fH/4.0;
	wi.dp_point.y -= fR2;
	m_space.push_back(wi);
	wi.dp_point.y += fR2;
	wi.dp_point.x -= fR2;
	m_space.push_back(wi);
	wi.dp_point.y += fR2;
	wi.dp_point.x += fR2;
	m_space.push_back(wi);
	wi.dp_point.y -= fR2;
	wi.dp_point.x += fR2;
	m_space.push_back(wi);
	wi.dp_point.x -= (fR2-fR1);
	m_space.push_back(wi);
	wi.dp_point.x -= (fR2-fR1);
	wi.dp_point.y -= (fR2-fR1);
	m_space.push_back(wi);
	wi.dp_point.x -= (fR2-fR1);
	wi.dp_point.y += (fR2-fR1);
	m_space.push_back(wi);
	wi.dp_point.x += (fR2-fR1);
	wi.dp_point.y += (fR2-fR1);
	m_space.push_back(wi);
	wi.dp_point.y -= (fR2-fR1);
	m_space.push_back(wi);
	}

	wi.dp_point.z += fH/4.0;
	wi.dp_point.y -= fR1;
	m_space.push_back(wi);
	wi.dp_point.y += fR1;
	wi.dp_point.x -= fR1;
	m_space.push_back(wi);
	wi.dp_point.x += fR1;
	wi.dp_point.y += fR1;
	m_space.push_back(wi);
	wi.dp_point.y -= fR1;
	wi.dp_point.x += fR1;
	m_space.push_back(wi);
	wi.dp_point.x -= fR1;
	m_space.push_back(wi);
}

void CScandOP::TestRegion()
{
	CDobotPoint p1 = m_pOperDobot->m_WorkCenterPoint;
	p1.z += m_devInfo.m_iDevH;
	p1.x -= m_sysInfo.m_fR2 * 1.414;
	p1.y -= m_sysInfo.m_fR2 * 1.414;
	m_pOperDobot->Move2AbsolutePosition(p1,-15);//Sleep(1000);
	p1.x += m_sysInfo.m_fR2 * 1.414;
	m_pOperDobot->Move2AbsolutePosition(p1,-15);//Sleep(1000);
	p1.y += m_sysInfo.m_fR2 * 1.414;
	m_pOperDobot->Move2AbsolutePosition(p1,-15);//Sleep(1000);
	p1.x -= m_sysInfo.m_fR2 * 1.414;
	m_pOperDobot->Move2AbsolutePosition(p1,-15);//Sleep(1000);
	p1.y -= m_sysInfo.m_fR2 * 1.414;
	m_pOperDobot->Move2AbsolutePosition(p1,-15);//Sleep(1000);

	
	CDobotPoint ReadMachePoint_up  = m_pOperDobot->m_ReadMachePoint;ReadMachePoint_up.z += 50;
	m_pOperDobot->Move2AbsolutePosition(ReadMachePoint_up);//Sleep(1000);
	m_pOperDobot->Move2AbsolutePosition(m_pOperDobot->m_ReadMachePoint);//Sleep(1000);

	for(int i=0;i<4;i++)
	{
		p1 = m_pOperDobot->m_CardInfo[i].dp_point;p1.z += 50;
		m_pOperDobot->Move2AbsolutePosition(p1,-15);//Sleep(1000);
		m_pOperDobot->Move2AbsolutePosition(m_pOperDobot->m_CardInfo[i].dp_point,-15);//Sleep(1000);
		m_pOperDobot->Move2AbsolutePosition(p1,-15);//Sleep(1000);
		
		/*
		CAngle an = g_OperDobot->m_CardInfo[i].an_point;
		an.z -= 15; 
		an.y -= 10;
		g_OperDobot->Move2AnglePoint(an,g_OperDobot->m_CardInfo[i].pawArmAngle);Sleep(1000);
		g_OperDobot->Move2AnglePoint(g_OperDobot->m_CardInfo[i].an_point,g_OperDobot->m_CardInfo[i].pawArmAngle);Sleep(1000);
		g_OperDobot->Move2AnglePoint(an,g_OperDobot->m_CardInfo[i].pawArmAngle);Sleep(1000);
		*/
	}
	
	p1 = m_pOperDobot->m_WorkCenterPoint;p1.z += m_devInfo.m_iDevH + 60;
	m_pOperDobot->Move2AbsolutePosition(p1);
}
void CScandOP::AppParameter()
{
	CDobotPoint p1 = m_pOperDobot->m_WorkCenterPoint;p1.z += m_devInfo.m_iDevH + 60;
	m_pOperDobot->Move2AbsolutePosition(p1);
}
void CScandOP::ScandReading()
{
	SetupPosition(m_sysInfo.m_fR1,m_sysInfo.m_fR2,m_sysInfo.m_fH,m_sysInfo.m_fBH,m_FirstScandPoint);
	m_bRunning = true;
	int iPointStatus = 0;
	int iSucceedSum = 0;
	CString strCardNo;
	unsigned long fee0,fee1;
	int z_up = m_FirstScandPoint.z + 50;
	//dlg->GetDlgItem(IDC_EDIT_STATUS)->SetWindowText("开始测试...");
	int i = 0;
	for(i = 0; i < m_space.size(); i++)
{
	if(!m_bRunning) break;
	int z = 0;
	if(i < 5) z = 0;
	if(4 < i && i < 14)  z = 1;
	if(13 < i && i < 23) z = 2;
	if(22 < i && i < 32) z = 3;
	if(31 < i) z = 4;
	(pLayerChange)(z);
	/*
	if(i==0)(pLayerChange)(0);
	if(i==5)(pLayerChange)(1);
	if(i==14)(pLayerChange)(2);
	if(i==23)(pLayerChange)(3);
	if(i==32)(pLayerChange)(4);
	*/
	m_space[i].iPointStatus = 1;
	int iCardNo = i % 4;
	CDobotPoint CurrentCardPoint = m_pOperDobot->m_CardInfo[iCardNo].dp_point;
	CDobotPoint CurrentCardPoint_up = CurrentCardPoint;CurrentCardPoint_up.z = z_up;
	//CAngle CurrentAnglePoint = g_OperDobot->m_CardInfo[iCardNo].an_point;
	//CAngle CurrentAnglePoint_up = CurrentAnglePoint;CurrentAnglePoint_up.z -= 10;CurrentAnglePoint_up.y -= 15;
	m_pOperDobot->Move2AbsolutePosition(CurrentCardPoint_up,-15);//Sleep(1000);
	m_pOperDobot->Move2AbsolutePosition(CurrentCardPoint,-15);/*Sleep(1000);*/ m_pOperDobot->SetGrab(TRUE);Sleep(300);// 开始吸卡
	m_pOperDobot->Move2AbsolutePosition(CurrentCardPoint_up,-15);//Sleep(1000);

	CDobotPoint ReadMachePoint_up  = m_pOperDobot->m_ReadMachePoint;ReadMachePoint_up.z = z_up;
	m_pOperDobot->Move2AbsolutePosition(ReadMachePoint_up,-15);//Sleep(1000);
	CDobotPoint pReadMache = m_pOperDobot->m_ReadMachePoint; pReadMache.z += 10;
	m_pOperDobot->Move2AbsolutePosition(pReadMache,-15);//Sleep(1000);
	if(m_pCardOP->checkcard(fee0,strCardNo))  //开始查询余额,记录余额0
	{
		m_pOperDobot->m_CardInfo[iCardNo].lBeginWFee = fee0;
		//TRACE("fee0=%d\n",fee0);
		if(m_pOperDobot->m_CardInfo[iCardNo].lInitFee <= 0)
		{
			m_pOperDobot->m_CardInfo[iCardNo].lInitFee = fee0;
			strcpy(m_pOperDobot->m_CardInfo[iCardNo].szCardNo,(LPCTSTR)strCardNo);
		}
	}//结束查询余额,记录余额
	else
	{
		TRACE("failure query1\n");
	}
	
	m_pOperDobot->Move2AbsolutePosition(ReadMachePoint_up,-15);//Sleep(1000);

	CDobotPoint wp_u = m_pOperDobot->m_WorkCenterPoint;/*g_iWorkingPoint[x][y][z].dp_point;*/wp_u.z = z_up; //if(x == 0) TRACE("x,y,z=%f,%f,%f\n",wp_u.x,wp_u.y,wp_u.z);
	m_pOperDobot->Move2AbsolutePosition(wp_u,-15);//Sleep(1000);
	m_pOperDobot->Move2AbsolutePosition(m_space[i].dp_point,-15);//Sleep(1000);//g_pCardOP->Consume();//开始扣费f
	m_pOperDobot->Move2AbsolutePosition(wp_u,-15);//Sleep(1000);

	m_pOperDobot->Move2AbsolutePosition(ReadMachePoint_up,-15);//Sleep(1000);
	m_pOperDobot->Move2AbsolutePosition(pReadMache,-15);//Sleep(1000);
	if(m_pCardOP->checkcard(fee1,strCardNo))//开始查询余额,记录余额1 (余额0-余额1=f 表示成功)
	{
		m_pOperDobot->m_CardInfo[iCardNo].lEndWFee = fee1;
		if(fee0-fee1 == m_devInfo.m_iFeeValue) 
		{
			iPointStatus = 2;
			iSucceedSum++;
		}
		else iPointStatus = 3;
			m_pOperDobot->m_CardInfo[iCardNo].iSucceedSum++;
	}
	else
	{
		 iPointStatus = 3;
		 m_pOperDobot->m_CardInfo[iCardNo].iFailureSum++;
		 TRACE("failure query2\n");
	}//结束查询余额,记录余额1 (余额0-余额1=f 表示成功)
	
	m_pOperDobot->Move2AbsolutePosition(ReadMachePoint_up,-15);//Sleep(1000);

	m_pOperDobot->Move2AbsolutePosition(CurrentCardPoint_up,-15);//Sleep(1000);
	CDobotPoint p = CurrentCardPoint;p.z += 10;
	m_pOperDobot->Move2AbsolutePosition(p,-15);/*Sleep(1000);*/ m_pOperDobot->SetGrab(FALSE);Sleep(300);//放开卡
	m_pOperDobot->Move2AbsolutePosition(CurrentCardPoint_up,-15);//Sleep(1000);
	m_space[i].iPointStatus = iPointStatus;
	}
	char str[128] = {0};
	sprintf(str,"共%d个点，已测试了%d个，其中%d个成功，%d个失败",m_space.size(),i+1,iSucceedSum,i+1-iSucceedSum);
	(pViewChange)(str);

	CDobotPoint p = m_pOperDobot->m_WorkCenterPoint;
	p.z += m_devInfo.m_iDevH + 50;
	m_pOperDobot->Move2AbsolutePosition(p);
}

void CScandOP::ScandNoFee1()
{
	SetupPosition(m_sysInfo.m_fR1,m_sysInfo.m_fR2,m_sysInfo.m_fH,m_sysInfo.m_fBH,m_FirstScandPoint);
//	int iPointStatus = 0;
//	CString strCardNo;
//	unsigned long fee0,fee1;
	int z_up = m_FirstScandPoint.z + 50;
	CDobotPoint pCardPoint = m_pOperDobot->m_CardInfo[0].dp_point;
	CDobotPoint pCardPoint_up =  m_pOperDobot->m_CardInfo[0].dp_point;pCardPoint_up.z = z_up;

	m_pOperDobot->Move2AbsolutePosition(pCardPoint_up);//Sleep(1000);
	m_pOperDobot->Move2AbsolutePosition(pCardPoint);//Sleep(1000);
	m_pOperDobot->SetGrab(TRUE);Sleep(300);
	m_pOperDobot->Move2AbsolutePosition(pCardPoint_up);//Sleep(1000);
	m_bRunning = true;
	CDobotPoint p1 = m_pOperDobot->m_WorkCenterPoint;p1.z += m_devInfo.m_iDevH + 90;
	(pViewChange)("测试中...");
	int i;
	for(i = 0; i < m_space.size(); i++)
	{
		if(!m_bRunning) break;
		int z = 0;
		if(i < 5) z = 0;
		if(4 < i && i < 14)  z = 1;
		if(13 < i && i < 23) z = 2;
		if(22 < i && i < 32) z = 3;
		if(31 < i) z = 4;
		(pLayerChange)(z);
				m_space[i].iPointStatus = 1;
				//CDobotPoint wp_u = g_iWorkingPoint[x][y][z].dp_point;wp_u.z = z_up;
				//g_OperDobot->Move2AbsolutePosition(wp_u);Sleep(1000);
				m_pOperDobot->Move2AbsolutePosition(p1);//Sleep(1000);
				m_pOperDobot->Move2AbsolutePosition(m_space[i].dp_point);//Sleep(1000);
				//g_OperDobot->Move2AbsolutePosition(wp_u);Sleep(1000);
				m_space[i].iPointStatus = 0;
				//Sleep(1000);

	}
	m_pOperDobot->Move2AbsolutePosition(pCardPoint_up);//Sleep(1000);
	CDobotPoint p = pCardPoint;p.z+=10;
	m_pOperDobot->Move2AbsolutePosition(p);//Sleep(1000);
	m_pOperDobot->SetGrab(FALSE);Sleep(300);
	m_pOperDobot->Move2AbsolutePosition(pCardPoint_up);//Sleep(1000);
}

void CScandOP::ScandNoFee2()
{
	SetupPosition(m_sysInfo.m_fR1,m_sysInfo.m_fR2,m_sysInfo.m_fH,m_sysInfo.m_fBH,m_FirstScandPoint);
	m_bRunning = true;
	int iSucceedSum = 0;
	int iPointStatus = 0;
	CString strCardNo;
	unsigned long fee1;
	int z_up = m_FirstScandPoint.z + 50;
	CDobotPoint pCardPoint = m_pOperDobot->m_CardInfo[0].dp_point;
	CDobotPoint pCardPoint_up =  m_pOperDobot->m_CardInfo[0].dp_point;pCardPoint_up.z = z_up;

	m_pOperDobot->Move2AbsolutePosition(pCardPoint_up);//Sleep(1000);
	m_pOperDobot->Move2AbsolutePosition(pCardPoint);//Sleep(1000);
	m_pOperDobot->SetGrab(TRUE);Sleep(300);
	m_pOperDobot->Move2AbsolutePosition(pCardPoint_up);//Sleep(1000);
	CDobotPoint pMachePoint = m_pOperDobot->m_ReadMachePoint; pMachePoint.z+=10;
	m_pOperDobot->Move2AbsolutePosition(pMachePoint);//Sleep(1000);

	//unsigned long fee1;
	if(m_pCardOP->checkcard(fee1,strCardNo))//开始查询余额,记录余额1 (余额0-余额1=f 表示成功)
	{
		m_pOperDobot->m_CardInfo[0].lEndWFee = fee1;
		m_pOperDobot->m_CardInfo[0].lInitFee = fee1;
		strcpy(m_pOperDobot->m_CardInfo[0].szCardNo,strCardNo.GetBuffer(0));
	}

	CDobotPoint pBeginPoint = m_pOperDobot->m_WorkCenterPoint;pBeginPoint.z += m_devInfo.m_iDevH + 90;
	int i = 0;
	for(i = 0; i < m_space.size(); i++)
	{
				if(!m_bRunning) break;
	int z = 0;
	if(i < 5) z = 0;
	if(4 < i && i < 14)  z = 1;
	if(13 < i && i < 23) z = 2;
	if(22 < i && i < 32) z = 3;
	if(31 < i) z = 4;
	(pLayerChange)(z);

				m_space[i].iPointStatus = 1;
				CDobotPoint MachePoint_up = pMachePoint;MachePoint_up.z = z_up;
				m_pOperDobot->Move2AbsolutePosition(MachePoint_up);
				m_pOperDobot->Move2AbsolutePosition(pBeginPoint);//Sleep(1000);
				m_pOperDobot->Move2AbsolutePosition(m_space[i].dp_point);//Sleep(1000);
				m_pOperDobot->Move2AbsolutePosition(pBeginPoint);//Sleep(1000);
				m_pOperDobot->Move2AbsolutePosition(MachePoint_up);
				m_pOperDobot->Move2AbsolutePosition(pMachePoint);//Sleep(1000);

				if(m_pCardOP->checkcard(fee1,strCardNo))//开始查询余额,记录余额1 (余额0-余额1=f 表示成功)
				{
					if(m_pOperDobot->m_CardInfo[0].lEndWFee-fee1 == m_devInfo.m_iFeeValue) 
					{
						iPointStatus = 2;
						iSucceedSum++;
					}
					else
					{
						iPointStatus = 3;
					}
					m_pOperDobot->m_CardInfo[0].iSucceedSum++;
					m_pOperDobot->m_CardInfo[0].lEndWFee = fee1;
				}
				else
				{
					iPointStatus = 3;
					m_pOperDobot->m_CardInfo[0].iFailureSum++;
				}//结束查询余额,记录余额1 (余额0-余额1=f 表示成功)

				m_space[i].iPointStatus = iPointStatus;
				char str[256] = {0};
				sprintf(str,"共%d个点，已测试了%d个，其中%d个成功，%d个失败",m_space.size(),i+1,iSucceedSum,i+1-iSucceedSum);
				//dlg->GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(str);
				(pViewChange)(str);
	}
	m_pOperDobot->Move2AbsolutePosition(pCardPoint_up);//Sleep(1000);
	CDobotPoint p = pCardPoint;p.z+=10;
	m_pOperDobot->Move2AbsolutePosition(p);//Sleep(1000);
	m_pOperDobot->SetGrab(FALSE);Sleep(300);
	m_pOperDobot->Move2AbsolutePosition(pCardPoint_up);//Sleep(1000);
	m_bRunning = FALSE;
}

