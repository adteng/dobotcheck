#include "StdAfx.h"
#include "ScandOP.h"


CScandOP::CScandOP(void)
{
	m_bRunning = false;
}


CScandOP::~CScandOP(void)
{
}

void CScandOP::SetupPosition(float fXLen,float fYLen,int iX_sum,int iY_sum,int iZ_sum,const CDobotPoint &firstPoint)
{
	float fXSpace,fYSpace,fZSpace;
	fXSpace = fXLen/(iX_sum - 1);
	fYSpace = fYLen/(iY_sum - 1);
	fZSpace = m_sysInfo.m_fLayerSpaceLen; // m_ZLen/(m_iLayer_Sum -1); //iZ_sum * g_fYSpace;
	for(int z=0;z<iZ_sum;z++)
		for(int y=0;y<iY_sum;y++)
			for(int x=0;x<iX_sum;x++)
			{
				m_iWorkingPoint[x][y][z].dp_point.x = firstPoint.x + x * fXSpace;
				m_iWorkingPoint[x][y][z].dp_point.y = firstPoint.y + y * fYSpace;
				m_iWorkingPoint[x][y][z].dp_point.z = firstPoint.z + z * fZSpace;
				m_iWorkingPoint[x][y][z].iPointStatus = 0;
			}
}

void CScandOP::TestRegion()
{
	CDobotPoint p1 = m_pOperDobot->m_WorkCenterPoint;
	p1.z += m_devInfo.m_iDevH;
	p1.x -= m_sysInfo.m_fXLen/2.0;
	p1.y -= m_sysInfo.m_fYLen/2.0;
	m_pOperDobot->Move2AbsolutePosition(p1,-15);//Sleep(1000);
	p1.x += m_sysInfo.m_fXLen;
	m_pOperDobot->Move2AbsolutePosition(p1,-15);//Sleep(1000);
	p1.y+= m_sysInfo.m_fYLen;
	m_pOperDobot->Move2AbsolutePosition(p1,-15);//Sleep(1000);
	p1.x -= m_sysInfo.m_fXLen;
	m_pOperDobot->Move2AbsolutePosition(p1,-15);//Sleep(1000);
	p1.y -= m_sysInfo.m_fYLen;
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
	SetupPosition(m_sysInfo.m_fXLen,m_sysInfo.m_fYLen,m_sysInfo.m_iX_Sum,m_sysInfo.m_iY_Sum,m_sysInfo.m_iLayer_Sum, m_FirstScandPoint);
	m_bRunning = true;
	int iPointStatus = 0;
	int iSucceedSum = 0;
	CString strCardNo;
	unsigned long fee0,fee1;
	int z_up = m_FirstScandPoint.z + 50;
	//dlg->GetDlgItem(IDC_EDIT_STATUS)->SetWindowText("开始测试...");
	for(int z=0;z<m_sysInfo.m_iLayer_Sum;z++)
	{
		//dlg->m_comboLayer.SetCurSel(z);
		(pLayerChange)(z);
		for(int y=0;y<m_sysInfo.m_iY_Sum;y++)
			for(int x=0;x<m_sysInfo.m_iX_Sum;x++)
{
	if(!m_bRunning) break;

	

	m_iWorkingPoint[x][y][z].iPointStatus = 1;
	int iSeqNo = z * m_sysInfo.m_iY_Sum * m_sysInfo.m_iX_Sum + y * m_sysInfo.m_iX_Sum + x;
	int iCardNo = iSeqNo % 4;
	CDobotPoint CurrentCardPoint = m_pOperDobot->m_CardInfo[iCardNo].dp_point;
	CDobotPoint CurrentCardPoint_up = CurrentCardPoint;CurrentCardPoint_up.z = z_up;
	//CAngle CurrentAnglePoint = g_OperDobot->m_CardInfo[iCardNo].an_point;
	//CAngle CurrentAnglePoint_up = CurrentAnglePoint;CurrentAnglePoint_up.z -= 10;CurrentAnglePoint_up.y -= 15;
	m_pOperDobot->Move2AbsolutePosition(CurrentCardPoint_up,-15);//Sleep(1000);
	m_pOperDobot->Move2AbsolutePosition(CurrentCardPoint,-15);/*Sleep(1000);*/ m_pOperDobot->SetGrab(TRUE);//Sleep(1000);// 开始吸卡
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

	CDobotPoint wp_u = m_pOperDobot->m_WorkCenterPoint;/*g_iWorkingPoint[x][y][z].dp_point;*/wp_u.z = z_up; if(x == 0) TRACE("x,y,z=%f,%f,%f\n",wp_u.x,wp_u.y,wp_u.z);
	m_pOperDobot->Move2AbsolutePosition(wp_u,-15);//Sleep(1000);
	m_pOperDobot->Move2AbsolutePosition(m_iWorkingPoint[x][y][z].dp_point,-15);//Sleep(1000);//g_pCardOP->Consume();//开始扣费f
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
	m_pOperDobot->Move2AbsolutePosition(p,-15);/*Sleep(1000);*/ m_pOperDobot->SetGrab(FALSE);//Sleep(1000);//放开卡
	m_pOperDobot->Move2AbsolutePosition(CurrentCardPoint_up,-15);//Sleep(1000);
	m_iWorkingPoint[x][y][z].iPointStatus = iPointStatus;

	char str[256] = {0};
	sprintf(str,"共%d个点，已测试了%d个，其中%d个成功，%d个失败",m_sysInfo.m_iX_Sum*m_sysInfo.m_iY_Sum*m_sysInfo.m_iLayer_Sum,iSeqNo+1,iSucceedSum,iSeqNo+1-iSucceedSum);
	(pViewChange)(str);
}
	}


	CDobotPoint p = m_pOperDobot->m_WorkCenterPoint;
	p.z += m_devInfo.m_iDevH + 50;
	m_pOperDobot->Move2AbsolutePosition(p);

}

void CScandOP::ScandNoFee1()
{
	SetupPosition(m_sysInfo.m_fXLen,m_sysInfo.m_fYLen,m_sysInfo.m_iX_Sum,m_sysInfo.m_iY_Sum,m_sysInfo.m_iLayer_Sum,m_FirstScandPoint);
//	int iPointStatus = 0;
//	CString strCardNo;
//	unsigned long fee0,fee1;
	int z_up = m_FirstScandPoint.z + 50;
	CDobotPoint pCardPoint = m_pOperDobot->m_CardInfo[0].dp_point;
	CDobotPoint pCardPoint_up =  m_pOperDobot->m_CardInfo[0].dp_point;pCardPoint_up.z = z_up;

	m_pOperDobot->Move2AbsolutePosition(pCardPoint_up);//Sleep(1000);
	m_pOperDobot->Move2AbsolutePosition(pCardPoint);//Sleep(1000);
	m_pOperDobot->SetGrab(TRUE);
	m_pOperDobot->Move2AbsolutePosition(pCardPoint_up);//Sleep(1000);
	m_bRunning = true;
	CDobotPoint p1 = m_pOperDobot->m_WorkCenterPoint;p1.z += m_devInfo.m_iDevH + 90;
	//dlg->GetDlgItem(IDC_EDIT_STATUS)->SetWindowText("测试中...");
	(pViewChange)("测试中...");
	for(int z=0;z<m_sysInfo.m_iLayer_Sum;z++)
	{
		if(m_bRunning) break;
		//dlg->m_comboLayer.SetCurSel(z);
		(pLayerChange)(z);
		for(int y=0;y<m_sysInfo.m_iY_Sum;y++)
		{
			if(!m_bRunning) break;
			for(int x=0;x<m_sysInfo.m_iX_Sum;x++)
			{
				if(!m_bRunning) break;
				m_iWorkingPoint[x][y][z].iPointStatus = 1;
				//CDobotPoint wp_u = g_iWorkingPoint[x][y][z].dp_point;wp_u.z = z_up;
				//g_OperDobot->Move2AbsolutePosition(wp_u);Sleep(1000);
				m_pOperDobot->Move2AbsolutePosition(p1);//Sleep(1000);
				m_pOperDobot->Move2AbsolutePosition(m_iWorkingPoint[x][y][z].dp_point);//Sleep(1000);
				//g_OperDobot->Move2AbsolutePosition(wp_u);Sleep(1000);
				m_iWorkingPoint[x][y][z].iPointStatus = 0;
				//Sleep(1000);
			}
		}
	}
	m_pOperDobot->Move2AbsolutePosition(pCardPoint_up);//Sleep(1000);
	CDobotPoint p = pCardPoint;p.z+=10;
	m_pOperDobot->Move2AbsolutePosition(p);//Sleep(1000);
	m_pOperDobot->SetGrab(FALSE);
	m_pOperDobot->Move2AbsolutePosition(pCardPoint_up);//Sleep(1000);
}

void CScandOP::ScandNoFee2()
{
	SetupPosition(m_sysInfo.m_fXLen,m_sysInfo.m_fYLen,m_sysInfo.m_iX_Sum,m_sysInfo.m_iY_Sum,m_sysInfo.m_iLayer_Sum,m_FirstScandPoint);
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
	m_pOperDobot->SetGrab(TRUE);
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
	for(int z=0;z<m_sysInfo.m_iLayer_Sum;z++)
	{
		//dlg->m_comboLayer.SetCurSel(z);
		(pLayerChange)(z);
		for(int y=0;y<m_sysInfo.m_iY_Sum;y++)
		{
			if(!m_bRunning) break;
			for(int x=0;x<m_sysInfo.m_iX_Sum;x++)
			{
				int iSeqNo = z * m_sysInfo.m_iY_Sum * m_sysInfo.m_iX_Sum + y * m_sysInfo.m_iX_Sum + x;
				if(!m_bRunning) break;
				m_iWorkingPoint[x][y][z].iPointStatus = 1;
				CDobotPoint MachePoint_up = pMachePoint;MachePoint_up.z = z_up;
				m_pOperDobot->Move2AbsolutePosition(MachePoint_up);
				m_pOperDobot->Move2AbsolutePosition(pBeginPoint);//Sleep(1000);
				m_pOperDobot->Move2AbsolutePosition(m_iWorkingPoint[x][y][z].dp_point);//Sleep(1000);
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

				m_iWorkingPoint[x][y][z].iPointStatus = iPointStatus;
				char str[256] = {0};
				sprintf(str,"共%d个点，已测试了%d个，其中%d个成功，%d个失败",m_sysInfo.m_iX_Sum*m_sysInfo.m_iY_Sum*m_sysInfo.m_iLayer_Sum,iSeqNo+1,iSucceedSum,iSeqNo+1-iSucceedSum);
				//dlg->GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(str);
				(pViewChange)(str);
			}
		}
	}
	m_pOperDobot->Move2AbsolutePosition(pCardPoint_up);//Sleep(1000);
	CDobotPoint p = pCardPoint;p.z+=10;
	m_pOperDobot->Move2AbsolutePosition(p);//Sleep(1000);
	m_pOperDobot->SetGrab(FALSE);
	m_pOperDobot->Move2AbsolutePosition(pCardPoint_up);//Sleep(1000);
	m_bRunning = FALSE;
}

