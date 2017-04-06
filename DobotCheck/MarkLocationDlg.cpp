// MarkLocationDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DobotCheck.h"
#include "MarkLocationDlg.h"
#include "afxdialogex.h"
#include "OperDobot.h"
// CMarkLocationDlg 对话框

IMPLEMENT_DYNAMIC(CMarkLocationDlg, CDialogEx)

CMarkLocationDlg::CMarkLocationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMarkLocationDlg::IDD, pParent)
{

}

CMarkLocationDlg::~CMarkLocationDlg()
{
}

void CMarkLocationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMarkLocationDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_MARK_FIRST_CARD, &CMarkLocationDlg::OnBnClickedButtonMarkFirstCard)
	ON_BN_CLICKED(IDC_BUTTON_MARK_SECOND_CARD, &CMarkLocationDlg::OnBnClickedButtonMarkSecondCard)
	ON_BN_CLICKED(IDC_BUTTON_MARK_THIRD_CARD, &CMarkLocationDlg::OnBnClickedButtonMarkThirdCard)
	ON_BN_CLICKED(IDC_BUTTON_MARK_FOUR_CARD, &CMarkLocationDlg::OnBnClickedButtonMarkFourCard)
	ON_BN_CLICKED(IDC_BUTTON_MARK_WORK_CENTER, &CMarkLocationDlg::OnBnClickedButtonMarkWorkCenter)
	ON_BN_CLICKED(IDC_BUTTON_MARK_READ_MACHE, &CMarkLocationDlg::OnBnClickedButtonMarkReadMache)
	ON_BN_CLICKED(IDC_BUTTON_POINT_SAVE, &CMarkLocationDlg::OnBnClickedButtonPointSave)
	ON_BN_CLICKED(IDC_BUTTON_POINT_LOAD, &CMarkLocationDlg::OnBnClickedButtonPointLoad)
END_MESSAGE_MAP()


// CMarkLocationDlg 消息处理程序
//extern CScandReading *g_dlg;
extern OperDobot *g_OperDobot;

void CMarkLocationDlg::OnBnClickedButtonMarkFirstCard()
{
	// TODO: 在此添加控件通知处理程序代码
	g_OperDobot->m_CardInfo[0].dp_point = g_OperDobot->GetCurrentAbsolutePosition();
	g_OperDobot->m_CardInfo[0].an_point = g_OperDobot->GetCurrentCAnglePosition();
	g_OperDobot->m_CardInfo[0].pawArmAngle = -1 * g_OperDobot->m_CardInfo[0].an_point.x;
	//GetDlgItem(IDC_BUTTON_MARK_FIRST_CARD)->EnableWindow(FALSE);


	float fSpaceCardY = 78.0;
	float fSpaceCardX = 115.0;

	g_OperDobot->m_CardInfo[1].dp_point = g_OperDobot->m_CardInfo[0].dp_point;
	g_OperDobot->m_CardInfo[1].dp_point.y -= fSpaceCardY;
	//GetDlgItem(IDC_BUTTON_MARK_SECOND_CARD)->EnableWindow(FALSE);
	g_OperDobot->m_CardInfo[2].dp_point = g_OperDobot->m_CardInfo[1].dp_point;
	g_OperDobot->m_CardInfo[2].dp_point.x -= fSpaceCardX;
	//GetDlgItem(IDC_BUTTON_MARK_THIRD_CARD)->EnableWindow(FALSE);
	g_OperDobot->m_CardInfo[3].dp_point = g_OperDobot->m_CardInfo[2].dp_point;
	g_OperDobot->m_CardInfo[3].dp_point.y += fSpaceCardY;
	//GetDlgItem(IDC_BUTTON_MARK_FOUR_CARD)->EnableWindow(FALSE);
	g_OperDobot->m_ReadMachePoint = g_OperDobot->m_CardInfo[0].dp_point;
	g_OperDobot->m_ReadMachePoint.x += 160;
	g_OperDobot->m_ReadMachePoint.z += 20;
	//GetDlgItem(IDC_BUTTON_MARK_READ_MACHE)->EnableWindow(FALSE);
	m_iFinishSum += 5;
	/*
	if(m_iFinishSum >=6)
	{
		GetDlgItem(IDC_BUTTON_POINT_SAVE)->EnableWindow(TRUE);
	}*/
}

void CMarkLocationDlg::OnBnClickedButtonMarkSecondCard()
{
	// TODO: 在此添加控件通知处理程序代码
	g_OperDobot->m_CardInfo[1].dp_point= g_OperDobot->GetCurrentAbsolutePosition();
	g_OperDobot->m_CardInfo[1].an_point = g_OperDobot->GetCurrentCAnglePosition();
	g_OperDobot->m_CardInfo[1].pawArmAngle = -1 * g_OperDobot->m_CardInfo[1].an_point.x;
	//GetDlgItem(IDC_BUTTON_MARK_SECOND_CARD)->EnableWindow(FALSE);
	//if(++m_iFinishSum >=6) GetDlgItem(IDC_BUTTON_POINT_SAVE)->EnableWindow(TRUE); 
}


void CMarkLocationDlg::OnBnClickedButtonMarkThirdCard()
{
	// TODO: 在此添加控件通知处理程序代码
	g_OperDobot->m_CardInfo[2].dp_point = g_OperDobot->GetCurrentAbsolutePosition();
	g_OperDobot->m_CardInfo[2].an_point = g_OperDobot->GetCurrentCAnglePosition();
	g_OperDobot->m_CardInfo[2].pawArmAngle = -1 * g_OperDobot->m_CardInfo[2].an_point.x;
	//GetDlgItem(IDC_BUTTON_MARK_THIRD_CARD)->EnableWindow(FALSE);
	//if(++m_iFinishSum >=6) GetDlgItem(IDC_BUTTON_POINT_SAVE)->EnableWindow(TRUE); 
}

void CMarkLocationDlg::OnBnClickedButtonMarkFourCard()
{
	// TODO: 在此添加控件通知处理程序代码
	g_OperDobot->m_CardInfo[3].dp_point = g_OperDobot->GetCurrentAbsolutePosition();
	g_OperDobot->m_CardInfo[3].an_point = g_OperDobot->GetCurrentCAnglePosition();
	g_OperDobot->m_CardInfo[3].pawArmAngle = -1 * g_OperDobot->m_CardInfo[3].an_point.x;
	//GetDlgItem(IDC_BUTTON_MARK_FOUR_CARD)->EnableWindow(FALSE);
	//if(++m_iFinishSum >=6) GetDlgItem(IDC_BUTTON_POINT_SAVE)->EnableWindow(TRUE); 
}

void CMarkLocationDlg::OnBnClickedButtonMarkWorkCenter()
{
	// TODO: 在此添加控件通知处理程序代码
	g_OperDobot->m_WorkCenterPoint = g_OperDobot->GetCurrentAbsolutePosition();

	//GetDlgItem(IDC_BUTTON_MARK_WORK_CENTER)->EnableWindow(FALSE);
	//if(++m_iFinishSum >=6) GetDlgItem(IDC_BUTTON_POINT_SAVE)->EnableWindow(TRUE); 
}

void CMarkLocationDlg::OnBnClickedButtonMarkReadMache()
{
	// TODO: 在此添加控件通知处理程序代码
	g_OperDobot->m_ReadMachePoint = g_OperDobot->GetCurrentAbsolutePosition();
	//GetDlgItem(IDC_BUTTON_MARK_READ_MACHE)->EnableWindow(FALSE);
	//if(++m_iFinishSum >=6) GetDlgItem(IDC_BUTTON_POINT_SAVE)->EnableWindow(TRUE); 
}

void CMarkLocationDlg::OnBnClickedButtonPointSave()
{
	// TODO: 在此添加控件通知处理程序代码
	ofstream m_ofs;
	m_ofs.open ("data/location.conf",ios::out);
	if(!m_ofs) 
	{
		MessageBox("打开文件<data/location.conf>失败");
		return ;
	}
	char sBuffer[128] = {0};
	sprintf(sBuffer,"%0.4f,%0.4f,%0.4f,",g_OperDobot->m_CardInfo[0].dp_point.x,g_OperDobot->m_CardInfo[0].dp_point.y,g_OperDobot->m_CardInfo[0].dp_point.z);
	m_ofs <<sBuffer<< endl;
	sprintf(sBuffer,"%0.4f,%0.4f,%0.4f,",g_OperDobot->m_CardInfo[1].dp_point.x,g_OperDobot->m_CardInfo[1].dp_point.y,g_OperDobot->m_CardInfo[1].dp_point.z);
	m_ofs <<sBuffer<< endl;
	sprintf(sBuffer,"%0.4f,%0.4f,%0.4f,",g_OperDobot->m_CardInfo[2].dp_point.x,g_OperDobot->m_CardInfo[2].dp_point.y,g_OperDobot->m_CardInfo[2].dp_point.z);
	m_ofs <<sBuffer<< endl;
	sprintf(sBuffer,"%0.4f,%0.4f,%0.4f,",g_OperDobot->m_CardInfo[3].dp_point.x,g_OperDobot->m_CardInfo[3].dp_point.y,g_OperDobot->m_CardInfo[3].dp_point.z);
	m_ofs <<sBuffer<< endl;
	sprintf(sBuffer,"%0.4f,%0.4f,%0.4f,%0.4f,",g_OperDobot->m_CardInfo[0].an_point.x,g_OperDobot->m_CardInfo[0].an_point.y,g_OperDobot->m_CardInfo[0].an_point.z,g_OperDobot->m_CardInfo[0].pawArmAngle);
	m_ofs <<sBuffer<< endl;
	sprintf(sBuffer,"%0.4f,%0.4f,%0.4f,%0.4f,",g_OperDobot->m_CardInfo[1].an_point.x,g_OperDobot->m_CardInfo[1].an_point.y,g_OperDobot->m_CardInfo[1].an_point.z,g_OperDobot->m_CardInfo[1].pawArmAngle);
	m_ofs <<sBuffer<< endl;
	sprintf(sBuffer,"%0.4f,%0.4f,%0.4f,%0.4f,",g_OperDobot->m_CardInfo[2].an_point.x,g_OperDobot->m_CardInfo[2].an_point.y,g_OperDobot->m_CardInfo[2].an_point.z,g_OperDobot->m_CardInfo[2].pawArmAngle);
	m_ofs <<sBuffer<< endl;
	sprintf(sBuffer,"%0.4f,%0.4f,%0.4f,%0.4f,",g_OperDobot->m_CardInfo[3].an_point.x,g_OperDobot->m_CardInfo[3].an_point.y,g_OperDobot->m_CardInfo[3].an_point.z,g_OperDobot->m_CardInfo[3].pawArmAngle);
	m_ofs <<sBuffer<< endl;
	sprintf(sBuffer,"%0.4f,%0.4f,%0.4f,",g_OperDobot->m_ReadMachePoint.x,g_OperDobot->m_ReadMachePoint.y,g_OperDobot->m_ReadMachePoint.z);
	m_ofs <<sBuffer<< endl;
	sprintf(sBuffer,"%0.4f,%0.4f,%0.4f,",g_OperDobot->m_WorkCenterPoint.x,g_OperDobot->m_WorkCenterPoint.y,g_OperDobot->m_WorkCenterPoint.z);
	m_ofs <<sBuffer<< endl;

	m_ofs.flush();
	m_ofs.close();
}

void CMarkLocationDlg::OnBnClickedButtonPointLoad()
{
	// TODO: 在此添加控件通知处理程序代码
	ifstream m_ifs;
	m_ifs.open ("data/location.conf",ios::in);
	if(!m_ifs) 
	{
		MessageBox("打开文件<data/location.conf>失败");
		return ;
	}
	char cLine[128] = {0};
	char sBuffer[16] = {0};
	string strLine;
	int nPos = 0;
	for(int i=0;i < 4;i++)
	{
		memset(cLine,0,128);
		m_ifs.getline(cLine,128);
		strLine = cLine;OperDobot::strTrim(strLine);

			nPos = strLine.find (",");memset(sBuffer,0,16);
			memcpy(sBuffer,strLine.c_str(),nPos);
			g_OperDobot->m_CardInfo[i].dp_point.x = atof(sBuffer);
			strLine.erase(0,nPos+1);

			 nPos = strLine.find (",");memset(sBuffer,0,16);
			memcpy(sBuffer,strLine.c_str (),nPos);
			g_OperDobot->m_CardInfo[i].dp_point.y = atof(sBuffer);
			strLine.erase(0,nPos+1);

			nPos = strLine.find (",");memset(sBuffer,0,16);
			memcpy(sBuffer,strLine.c_str (),nPos);
			g_OperDobot->m_CardInfo[i].dp_point.z = atof(sBuffer);
			strLine.erase(0,nPos+1);
	}

	nPos = 0;
	for(int i=0;i < 4;i++)
	{
		memset(cLine,0,128);
		m_ifs.getline(cLine,128);
		strLine = cLine;OperDobot::strTrim(strLine);

			nPos = strLine.find (",");memset(sBuffer,0,16);
			memcpy(sBuffer,strLine.c_str(),nPos);
			g_OperDobot->m_CardInfo[i].an_point.x = atof(sBuffer);
			strLine.erase(0,nPos+1);

			nPos = strLine.find (",");memset(sBuffer,0,16);
			memcpy(sBuffer,strLine.c_str (),nPos);
			g_OperDobot->m_CardInfo[i].an_point.y = atof(sBuffer);
			strLine.erase(0,nPos+1);

			nPos = strLine.find (",");memset(sBuffer,0,16);
			memcpy(sBuffer,strLine.c_str (),nPos);
			g_OperDobot->m_CardInfo[i].an_point.z = atof(sBuffer);
			strLine.erase(0,nPos+1);

			nPos = strLine.find (",");memset(sBuffer,0,16);
			memcpy(sBuffer,strLine.c_str (),nPos);
			g_OperDobot->m_CardInfo[i].pawArmAngle = atof(sBuffer);
			strLine.erase(0,nPos+1);
	}

		
		memset(cLine,0,128);
		m_ifs.getline(cLine,128);
		strLine = cLine;OperDobot::strTrim(strLine);

			 nPos = strLine.find (",");memset(sBuffer,0,16);
			memcpy(sBuffer,strLine.c_str(),nPos);
			g_OperDobot->m_ReadMachePoint.x = atof(sBuffer);
			strLine.erase(0,nPos+1);
			nPos = strLine.find (",");memset(sBuffer,0,16);
			memcpy(sBuffer,strLine.c_str (),nPos);
			g_OperDobot->m_ReadMachePoint.y = atof(sBuffer);
			strLine.erase(0,nPos+1);

			nPos = strLine.find (",");memset(sBuffer,0,16);
			memcpy(sBuffer,strLine.c_str (),nPos);
			g_OperDobot->m_ReadMachePoint.z = atof(sBuffer);
			strLine.erase(0,nPos+1);

		//GetDlgItem(IDC_BUTTON_MARK_READ_MACHE)->EnableWindow(FALSE);
		
		memset(cLine,0,128);
		m_ifs.getline(cLine,128);
		strLine = cLine;OperDobot::strTrim(strLine);

			 nPos = strLine.find (",");memset(sBuffer,0,16);
			memcpy(sBuffer,strLine.c_str(),nPos);
			g_OperDobot->m_WorkCenterPoint.x = atof(sBuffer);
			strLine.erase(0,nPos+1);

			 nPos = strLine.find (",");memset(sBuffer,0,16);
			memcpy(sBuffer,strLine.c_str (),nPos);
			g_OperDobot->m_WorkCenterPoint.y = atof(sBuffer);
			strLine.erase(0,nPos+1);

			 nPos = strLine.find (",");memset(sBuffer,0,16);
			memcpy(sBuffer,strLine.c_str (),nPos);
			g_OperDobot->m_WorkCenterPoint.z = atof(sBuffer);
			strLine.erase(0,nPos+1);

		//GetDlgItem(IDC_BUTTON_MARK_WORK_CENTER)->EnableWindow(FALSE);
	m_ifs.close();
	m_iFinishSum = 6;
}
