// FlashCard.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DobotCheck.h"
#include "FlashCard.h"
#include "afxdialogex.h"
#include "FlashFileNameDlg.h"


// CFlashCard �Ի���
extern OperDobot *g_OperDobot;
IMPLEMENT_DYNAMIC(CFlashCard, CDialogEx)

CFlashCard::CFlashCard(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFlashCard::IDD, pParent)
	, m_iRadioSelect(0)
	, m_fPauseTime(0)
	, m_f1(0)
	, m_f2(0)
	, m_f3(0)
	, m_f5(0)
	, m_f6(0)
	, m_f8(0)
	, m_f4(0)
	, m_iLoop(0)
{

}

CFlashCard::~CFlashCard()
{
}

void CFlashCard::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_iRadioSelect);
	DDX_Text(pDX, IDC_EDIT_PAUSE_TIME, m_fPauseTime);
	DDX_Text(pDX, IDC_EDIT1, m_f1);
	DDX_Text(pDX, IDC_EDIT2, m_f2);
	DDX_Text(pDX, IDC_EDIT3, m_f3);
	//  DDX_Text(pDX, IDC_EDIT4, m_f4);
	DDX_Text(pDX, IDC_EDIT5, m_f5);
	DDX_Text(pDX, IDC_EDIT6, m_f6);
	DDX_Text(pDX, IDC_EDIT8, m_f8);
	DDX_Control(pDX, IDC_COMBO1, m_combo_flash_config);
	DDX_Text(pDX, IDC_EDIT4, m_f4);
	DDX_Control(pDX, IDC_CHECK_GRAB, m_Check_grib);
	DDX_Text(pDX, IDC_EDIT_FLASH_LOOP, m_iLoop);
}


BEGIN_MESSAGE_MAP(CFlashCard, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_START, &CFlashCard::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_GET_WORK_POINT, &CFlashCard::OnBnClickedButtonGetWorkPoint)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CFlashCard::OnBnClickedButtonSave)
//	ON_NOTIFY(NM_THEMECHANGED, IDC_COMBO1, &CFlashCard::OnNMThemeChangedCombo1)
//	ON_CBN_EDITCHANGE(IDC_COMBO1, &CFlashCard::OnCbnEditchangeCombo1)
	ON_CBN_SELENDOK(IDC_COMBO1, &CFlashCard::OnCbnSelendokCombo1)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE, &CFlashCard::OnBnClickedButtonChange)
	ON_BN_CLICKED(IDC_CHECK_GRAB, &CFlashCard::OnBnClickedCheckGrab)
END_MESSAGE_MAP()


// CFlashCard ��Ϣ�������


BOOL CFlashCard::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_iRadioSelect = 1;
	m_f1 = 300.000;
	m_f2 = 200.000;
	m_f3 = 300.000;
	m_f4 = 200.000;
	m_f5 = 300.000;
	m_f6 = 200.000;
	m_fPauseTime = 1;
	m_f8 = 20.000;
	m_iLoop = 1;
	m_WorkPoint = g_OperDobot->m_WorkCenterPoint;
	m_WorkPoint.z += 40;

	CString strPath;
	CFileFind finder;
	m_combo_flash_config.ResetContent();
	CString str =  "data\\*.flash";
	BOOL bWorking = finder.FindFile((LPCTSTR)str);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		CString strFileName = finder.GetFileName();
		if(strFileName == "." || strFileName == "..") continue;

		m_combo_flash_config.AddString(strFileName);

	}
	finder.Close();

	GetDlgItem(IDC_EDIT_FLASH_INFO)->SetWindowText("��ѭ��0��");

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
unsigned int __stdcall FlashCardThread(void * param);
unsigned int __stdcall FlashCardThread(void * param)
{

	CFlashCard *dlg = (CFlashCard *)param;
	switch(dlg->m_iRadioSelect)
	{
	case 0:
		dlg->Radio1();
		break;
	case 1:
		dlg->Radio2();
		break;
	case 2:
		dlg->Radio3();
		break;
	case 3:
		dlg->Radio4();
		break;
	case 4:
		dlg->Radio5();
		break;
	default:
		break;
	}

	return 0;
}

void CFlashCard::OnBnClickedButtonStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	int i = m_iRadioSelect;
	unsigned int nThreadAddr;
	HANDLE	hSession = (HANDLE)_beginthreadex(0, 0, FlashCardThread, this, 0, &nThreadAddr);
	if(hSession) CloseHandle( hSession );
}
void CFlashCard::Radio1()
{
	/*
	float fX = Hex_To_Decimal(g_OperDobot->m_DobotStartStatus.cX,4);
	float fY = Hex_To_Decimal(g_OperDobot->m_DobotStartStatus.cY,4);
	float fZ = Hex_To_Decimal(g_OperDobot->m_DobotStartStatus.cZ,4) + 200;

	DobotOrder m_NewDobotOrder;
	memset(&m_NewDobotOrder.cHead[0],0,sizeof(DobotOrder));
	m_NewDobotOrder.cHead[0] = 0xA5;
	m_NewDobotOrder.cEnd[0] = 0x5A;
	FloatToByte(6.0,m_NewDobotOrder.cState);
	FloatToByte(0.0,m_NewDobotOrder.cAxis);
	FloatToByte(fX,m_NewDobotOrder.cX);
	FloatToByte(fY,m_NewDobotOrder.cY);
	FloatToByte(fZ,m_NewDobotOrder.cZ);
	FloatToByte(0.0,m_NewDobotOrder.cRHead);
	FloatToByte(0.0,m_NewDobotOrder.cIsGrip);
	FloatToByte(0.0,m_NewDobotOrder.cStartVe);
	FloatToByte(0.0,m_NewDobotOrder.cEndVel);
	FloatToByte(0.0,m_NewDobotOrder.cMaxVe);
	g_OperDobot->AddOrderList(m_NewDobotOrder);
	Sleep(800);
	FloatToByte(fZ-50,m_NewDobotOrder.cZ);
	FloatToByte(m_fPauseTime,m_NewDobotOrder.cMaxVe);
	g_OperDobot->AddOrderList(m_NewDobotOrder);
	Sleep(800);
	FloatToByte(fZ+250,m_NewDobotOrder.cZ);
	g_OperDobot->AddOrderList(m_NewDobotOrder);
	*/

	CDobotPoint p1 = m_WorkPoint;
	p1.z += 100;
	g_OperDobot->Move2AbsolutePosition(p1);
	//Sleep(1000);
	g_OperDobot->Move2AbsolutePosition(m_WorkPoint);
	Sleep(m_fPauseTime+1);
	g_OperDobot->Move2AbsolutePosition(p1);
}
void CFlashCard::Radio2()
{
	CDobotPoint p1 = m_WorkPoint;
	CDobotPoint p2 = m_WorkPoint;
	p1.y -= 100;
	p2.y += 100;
	CDobotPoint p3 = p2;
	p3.z += 100;
	CDobotPoint p4 = p1;
	p4.z += 100;
	for(int i=0;i<m_iLoop;i++)
	{
	g_OperDobot->SetSpeed(300,200,m_f3,m_f4,300,200,0,m_f8);
	g_OperDobot->Move2AbsolutePosition(p1);
	g_OperDobot->SetSpeed(m_f1,m_f2,m_f3,m_f4,m_f5,m_f6,0,m_f8);
	g_OperDobot->Move2AbsolutePosition(p2);
	g_OperDobot->SetSpeed(300,200,m_f3,m_f4,300,200,0,m_f8);
	g_OperDobot->Move2AbsolutePosition(p3);
	g_OperDobot->Move2AbsolutePosition(p4);
	g_OperDobot->Move2AbsolutePosition(p1);
	CString str;
	str.Format("��ѭ��%d��",m_iLoop);
	GetDlgItem(IDC_EDIT_FLASH_INFO)->SetWindowText(str);
	}

	//Sleep(1000);
	//g_OperDobot->Move2AbsolutePosition(m_WorkPoint);
	//Sleep(m_fPauseTime+1);
	//g_OperDobot->Move2AbsolutePosition(p1);
	//Sleep(1000);
	//g_OperDobot->Move2AbsolutePosition(m_PausePoint);
	
}
void CFlashCard::Radio3()
{
	CDobotPoint p1 = m_WorkPoint;
	p1.y += 100;
	g_OperDobot->Move2AbsolutePosition(p1);
	//Sleep(1000);
	g_OperDobot->Move2AbsolutePosition(m_WorkPoint);
	Sleep(m_fPauseTime+1);
	g_OperDobot->Move2AbsolutePosition(p1);
	//Sleep(1000);
	g_OperDobot->Move2AbsolutePosition(m_PausePoint);
}
void CFlashCard::Radio4()
{
	CDobotPoint p1 = m_WorkPoint;
	p1.y -= 100;
	p1.z += 100;
	g_OperDobot->Move2AbsolutePosition(p1);
	//Sleep(1000);
	g_OperDobot->Move2AbsolutePosition(m_WorkPoint);
	Sleep(m_fPauseTime+1);
	g_OperDobot->Move2AbsolutePosition(p1);
	//Sleep(1000);
	g_OperDobot->Move2AbsolutePosition(m_PausePoint);
}
void CFlashCard::Radio5()
{
	CDobotPoint p1 = m_WorkPoint;
	p1.y += 100;
	p1.z += 100;
	g_OperDobot->Move2AbsolutePosition(p1);
	//Sleep(1000);
	g_OperDobot->Move2AbsolutePosition(m_WorkPoint);
	Sleep(m_fPauseTime+1);
	g_OperDobot->Move2AbsolutePosition(p1);
	//Sleep(1000);
	g_OperDobot->Move2AbsolutePosition(m_PausePoint);
}

void CFlashCard::OnBnClickedButtonGetWorkPoint()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_WorkPoint = g_OperDobot->GetCurrentAbsolutePosition();
	m_PausePoint = m_WorkPoint;
	m_PausePoint.z += 100;
}

void CFlashCard::WriteFlashFile(CString strDBPath)
{
				ofstream m_ofs;
				m_ofs.open (strDBPath.GetBuffer(0),ios::out);
				if(!m_ofs)
				{ 
					MessageBox("���ļ�<data/flash.conf>ʧ��");
					return ;
				}
				char sBuffer[128] = {0};
				sprintf(sBuffer,"%0.2f,%0.3f,%0.3f,%0.3f,%0.3f,%0.3f,%0.3f,%0.3f,",m_f1,m_f2,m_f3,m_f4,m_f5,m_f6,m_fPauseTime,m_f8);
				m_ofs <<sBuffer<< endl;
				m_ofs.flush();
				m_ofs.close();
				m_combo_flash_config.AddString(strDBPath.GetBuffer(0) + 5);
				UpdateData(FALSE);
}

void CFlashCard::OnBnClickedButtonSave()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
		CTime time = CTime::GetCurrentTime();
		int m_nDay = time.GetDay();      ///��
		int m_nHour = time.GetHour();      ///Сʱ
		int m_nMinute = time.GetMinute();   ///����
		int m_nSecond = time.GetSecond(); 
		//char sFileName[128] = {0};

		CFlashFileNameDlg dlg;
		if(dlg.DoModal() == IDOK)
		{
			CString strDBPath;
			strDBPath.Format("data/%s.flash",dlg.m_strFlashFileName);
			if (!PathFileExists(strDBPath))
			{
				//sprintf(sFileName,"data/%02d%02d%02d%02d.flash",m_nDay,m_nHour,m_nMinute,m_nSecond);
				WriteFlashFile(strDBPath);
			}
			else
			{
				if(IDOK==MessageBox("���ƴ��ڣ��Ƿ��滻","���ƴ���",MB_OKCANCEL))
				{
					WriteFlashFile(strDBPath);
				}
			}
		}
}


//void CFlashCard::OnNMThemeChangedCombo1(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	// �ù���Ҫ��ʹ�� Windows XP ����߰汾��
//	// ���� _WIN32_WINNT ���� >= 0x0501��
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	*pResult = 0;
//}


//void CFlashCard::OnCbnEditchangeCombo1()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//
//}


void CFlashCard::OnCbnSelendokCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char sBuffer[64] = {0};
	char cLine[81] = {0};
	m_combo_flash_config.GetWindowText(sBuffer,32);
	CString strFileName;
	strFileName.Format("data/%s",sBuffer);
	ifstream m_ifs;
	m_ifs.open (strFileName,ios::in);
	if(!m_ifs) 
	{
		CString str;
		str = "���ļ�<" + strFileName + ">ʧ��";
		MessageBox(str);
		return ;
	}
	memset(cLine,0,81);
	string strLine;
	for(int i=0;m_ifs.getline(cLine,80);i++)
	{
		strLine = cLine;strTrim(strLine);
			int nPos = strLine.find (",");memset(sBuffer,0,16);
			memcpy(sBuffer,strLine.c_str(),nPos);
			m_f1 = atof(sBuffer);
			strLine.erase(0,nPos+1);

			nPos = strLine.find (",");memset(sBuffer,0,16);
			memcpy(sBuffer,strLine.c_str (),nPos);
			m_f2 = atof(sBuffer);
			strLine.erase(0,nPos+1);

			nPos = strLine.find (",");memset(sBuffer,0,16);
			memcpy(sBuffer,strLine.c_str (),nPos);
			m_f3 = atof(sBuffer);
			strLine.erase(0,nPos+1);

			nPos = strLine.find (",");memset(sBuffer,0,16);
			memcpy(sBuffer,strLine.c_str (),nPos);
			m_f4 = atof(sBuffer);
			strLine.erase(0,nPos+1);

			nPos = strLine.find (",");memset(sBuffer,0,16);
			memcpy(sBuffer,strLine.c_str (),nPos);
			m_f5 = atof(sBuffer);
			strLine.erase(0,nPos+1);

			nPos = strLine.find (",");memset(sBuffer,0,16);
			memcpy(sBuffer,strLine.c_str (),nPos);
			m_f6 = atof(sBuffer);
			strLine.erase(0,nPos+1);

			nPos = strLine.find (",");memset(sBuffer,0,16);
			memcpy(sBuffer,strLine.c_str (),nPos);
			m_fPauseTime = atof(sBuffer);
			strLine.erase(0,nPos+1);

			nPos = strLine.find (",");memset(sBuffer,0,16);
			memcpy(sBuffer,strLine.c_str (),nPos);
			m_f8 = atof(sBuffer);
			strLine.erase(0,nPos+1);
			break;
	}

	UpdateData(FALSE);

//	MessageBox(sBuffer);
}


void CFlashCard::OnBnClickedButtonChange()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	g_OperDobot->SetSpeed(m_f1,m_f2,m_f3,m_f4,m_f5,m_f6,0,m_f8);
}

unsigned int __stdcall TestThread(void * param);
void CFlashCard::OnBnClickedCheckGrab()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	unsigned int nThreadAddr;
	HANDLE	hSession = (HANDLE)_beginthreadex(0, 0, TestThread, this, 0, &nThreadAddr);
	if(hSession) CloseHandle( hSession );
}

unsigned int __stdcall TestThread(void * param)
{
	CFlashCard *dlg = (CFlashCard *)param;
	CDobotPoint p = g_OperDobot->m_CardInfo[0].dp_point;
	p.z += 100;
	g_OperDobot->Move2AbsolutePosition(p);//Sleep(1000);
	g_OperDobot->Move2AbsolutePosition(g_OperDobot->m_CardInfo[0].dp_point);//Sleep(1000);
	/*
	int iLoop = 0;
	while(1)
	{
		if(OperDobot::IsSameDobotPoint(g_OperDobot->m_CardInfo[0].dp_point,g_OperDobot->GetCurrentAbsolutePosition())) break;
		if(iLoop++ > 2000) break;
		Sleep(10);
	}
	*/
	if(dlg->m_Check_grib.GetCheck())
		g_OperDobot->SetGrab(TRUE);
	else
		g_OperDobot->SetGrab(FALSE);

	g_OperDobot->Move2AbsolutePosition(p);
	return 0;
}


