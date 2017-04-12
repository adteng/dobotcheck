
// DobotCheckDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DobotCheck.h"
#include "DobotCheckDlg.h"
#include "afxdialogex.h"
//#include "common/utility.h"
#include <Setupapi.h>
#include <devguid.h>
#include "ACard.h"
#include "OperDobot2.h"

#pragma  comment(lib,   "setupapi")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//#pragma  comment(lib,   "lib/utility")
//#pragma  comment(lib,   "setupapi")
//#pragma  comment(lib,   "lib/common_d")
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CDobotCheckDlg �Ի���
CDobotCheckDlg::CDobotCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDobotCheckDlg::IDD, pParent)
	, m_f1(0)
	, m_f7(0)
	, m_f8(0)
	, m_f9(0)
	, m_f10(0)
	, m_f2(0)
	, m_f3(0)
	, m_f4(0)
	, m_f5(0)
	, m_f6(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDobotCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_TabCtrl);
}

BEGIN_MESSAGE_MAP(CDobotCheckDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CDobotCheckDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_Send, &CDobotCheckDlg::OnBnClickedSend)
	ON_BN_CLICKED(IDOK, &CDobotCheckDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON2, &CDobotCheckDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDobotCheckDlg::OnBnClickedButton3)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CDobotCheckDlg::OnTcnSelchangeTab1)
//	ON_WM_LBUTTONDOWN()
ON_BN_CLICKED(IDC_BUTTON_MARK, &CDobotCheckDlg::OnBnClickedButtonMark)
//ON_BN_CLICKED(IDC_BUTTON_BACK, &CDobotCheckDlg::OnBnClickedButtonBack)
END_MESSAGE_MAP()


// CDobotCheckDlg ��Ϣ�������
void ShowDobotStatus(float f1,float f2,float f3,float f4,float f5,float f6,float f7,float f8,float f9,float f10);
OperDobot *g_OperDobot;
CDobotCheckDlg *g_dlg;
CACard *g_pCardOP;

void ShowLog(const char *pInf)
{
	CString str(pInf);
	g_dlg->ShowInfo(str);
}
BOOL CDobotCheckDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// ��������...���˵�����ӵ�ϵͳ�˵��С�
	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	g_dlg = this;
	g_OperDobot = new OperDobot2();
	g_pCardOP = new CACard();
	g_OperDobot->SetLogInfoFun(ShowLog);
	g_OperDobot->ShowDobotStatus=ShowDobotStatus;
	g_pCardOP->SetLogInfoFun(ShowLog);
	char sComName[8] = {0};
	//SearchCOM(sComName,"Arduino Mega 2560");//"USB-SERIAL CH340"
	SearchCOM(sComName,"USB-SERIAL CH340");
	CString strCom;
	strCom.Format( "//./%s",sComName);
	g_OperDobot->SetCOM((LPCTSTR)strCom);

	//g_wmd->ShowWindow(SW_HIDE);

	m_TabCtrl.InsertItem(0,"ä������");
	m_TabCtrl.InsertItem(1,"��������");
	m_TabCtrl.InsertItem(2,"ѹ������");

	m_ScandCard.Create(IDD_DIALOG_M1,GetDlgItem(IDC_TAB1));
	m_FlashCard.Create(IDD_DIALOG_M2,GetDlgItem(IDC_TAB1));
	m_PressureCardNew.Create(IDD_DIALOG_M4,GetDlgItem(IDC_TAB1));
	
	RECT rect;
	GetClientRect(&rect);
	rect.top += 21;
	m_ScandCard.MoveWindow(&rect);
	m_FlashCard.MoveWindow(&rect);
	m_PressureCardNew.MoveWindow(&rect);

	((CButton *)GetDlgItem(IDC_RADIO3))->SetCheck(TRUE);

	if(!g_pCardOP->FindCard())
		MessageBox("δ���ֶ�����");
	
	if(!g_pCardOP->ConnectReader())
		MessageBox("���Ӷ�����ʧ��");

	m_ScandCard.ShowWindow(TRUE);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}
bool CDobotCheckDlg::SearchCOM(char *pName,const char *pFindName) 
{
	bool bArduino = false;
	//   �õ��豸��Ϣ��    
	HDEVINFO   hDevInfo   =   SetupDiGetClassDevs(    
		(LPGUID)&GUID_DEVCLASS_PORTS,NULL,0,DIGCF_PRESENT/*   |   DIGCF_ALLCLASSES*/);    
    if(hDevInfo==INVALID_HANDLE_VALUE)
    {
        printf("Error!   SetupDiGetClassDevs()   return   %d\n",   GetLastError());    
        SetupDiDestroyDeviceInfoList(hDevInfo);    
        return     0;        
    }    
    TCHAR   szBuf[MAX_PATH];    
    SP_DEVINFO_DATA   spDevInfoData   =   {sizeof(SP_DEVINFO_DATA)};    
    //   ��ʼ�о��豸    
    DWORD   i = 0; 
    for(i = 0;SetupDiEnumDeviceInfo(hDevInfo, i,&spDevInfoData);   i++)    
    {    
		//   �õ��豸����    
		if(SetupDiGetDeviceRegistryProperty(hDevInfo,&spDevInfoData,SPDRP_FRIENDLYNAME,NULL,(PBYTE)szBuf, MAX_PATH,NULL))
        {        
			if(memcmp(szBuf,pFindName,strlen(pFindName)) == 0)//�������⴮��
			{
				string strPort = szBuf;
				int nPos1,nPos2;
				nPos1 = strPort.find("(",0);
				nPos2 = strPort.find(")",nPos1);
				strPort = strPort.substr(nPos1+1,nPos2-nPos1-1);
				OperDobot::strTrim(strPort);
				strcpy(pName,strPort.c_str());
				bArduino = true;
				break;
			}
        }
	}
	return bArduino;

}

void CDobotCheckDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDobotCheckDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CDobotCheckDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDobotCheckDlg::ShowInfo(CString inf)
{
	CString strTmp;
	GetDlgItem(IDC_EDIT1)->GetWindowText(strTmp);
	strTmp+="\r\n";
	if(strTmp.GetLength() > 10240) strTmp = "";
	GetDlgItem(IDC_EDIT1)->SetWindowText(strTmp + inf);
	((CEdit *)GetDlgItem(IDC_EDIT1))->SetSel(0,strTmp.GetLength());
}

void ShowDobotStatus(float f1,float f2,float f3,float f4,float f5,float f6,float f7,float f8,float f9,float f10)
{
	char sData[512] = {0};
	sprintf(sData,"%0.4f",f1);
	g_dlg->GetDlgItem(IDC_EDIT2)->SetWindowText(sData);
	sprintf(sData,"%0.4f",f2);
	g_dlg->GetDlgItem(IDC_EDIT3)->SetWindowText(sData);
	sprintf(sData,"%0.4f",f3);
	g_dlg->GetDlgItem(IDC_EDIT4)->SetWindowText(sData);
	sprintf(sData,"%0.4f",f4);
	g_dlg->GetDlgItem(IDC_EDIT5)->SetWindowText(sData);
	sprintf(sData,"%0.4f",f5);
	g_dlg->GetDlgItem(IDC_EDIT6)->SetWindowText(sData);
	sprintf(sData,"%0.4f",f6);
	g_dlg->GetDlgItem(IDC_EDIT7)->SetWindowText(sData);
	sprintf(sData,"%0.4f",f7);
	g_dlg->GetDlgItem(IDC_EDIT8)->SetWindowText(sData);
	sprintf(sData,"%0.4f",f8);
	g_dlg->GetDlgItem(IDC_EDIT9)->SetWindowText(sData);
	sprintf(sData,"%0.4f",f9);
	g_dlg->GetDlgItem(IDC_EDIT10)->SetWindowText(sData);
	sprintf(sData,"%0.4f",f10);
	g_dlg->GetDlgItem(IDC_EDIT11)->SetWindowText(sData);


	sprintf(sData,"������\r\nX�����꣺\t%0.5\tY�����꣺%0.5f\tZ�����꣺\t%0.5f\tR�����λ�ã�\t%0.5f\r\n�����Ƕȣ�%0.5f\t��۽Ƕȣ�%0.5f\tС�۽Ƕȣ�%0.5f\t����Ƕȣ�\t%0.5f\r\nצ�ӽǶȣ�%0.5f",f1,f2,f3,f4,f5,f6,f7,f8,f10);
	g_dlg->GetDlgItem(IDC_EDIT_DOBOT_STATUS)->SetWindowText(sData);
}

void CDobotCheckDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//g_wmd->ShowWindow(FALSE);
	//CWnd *wmd = 
	GetDlgItem( IDC_BUTTON1 )->EnableWindow(FALSE);
	g_OperDobot->DobotStart();
}





unsigned int __stdcall FlashingReading(void * param)
{
	/*
	list<CAngle> pList;
	pList.clear();
	CAngle p[] ={ {0,    36.000,18.000},
				  {0,    73.314,43.057},
				  {0,    36.000,18.000},
				  {0,    73.314,43.057},
				  {-45.0,73.314,43.057},
				  {-45.0,36.000,18.000},
				  {0,    36.000,18.000}};

	for(int i=0;i<7;i++)
	{
		pList.push_back(p[i]);
	}


	for(list<CAngle>::iterator it = pList.begin();it!=pList.end();it++)
	{
		g_OperDobot->Move2AnglePoint(*it);
		Sleep(1000);
	}
	*/

	CDobotPoint p1 = g_OperDobot->GetCurrentAbsolutePosition();

	int a = 100;
	int b = 100;

	CDobotPoint p2,p3,p4;
	p2.x = p1.x + a;
	p2.y = p1.y;
	p2.z = p1.z;

	p3.x = p2.x;
	p3.y = p2.y + b;
	p3.z = p2.x;

	p4.x = p3.x - a;
	p4.y = p3.y; 
	p4.z = p3.z;


	//g_OperDobot->Move2AbsolutePosition(p1);
	//Sleep(1000);
	g_OperDobot->Move2AbsolutePosition(p2);
	Sleep(1000);

	g_OperDobot->Move2AbsolutePosition(p3);
	Sleep(1000);
	g_OperDobot->Move2AbsolutePosition(p4);

	Sleep(1000);

	return 0;
}




void CDobotCheckDlg::OnBnClickedSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);

//	g_wmd->ShowWindow(SW_SHOW);

	DobotOrder m_NewDobotOrder;
	memset(&m_NewDobotOrder,0,sizeof(DobotOrder));

	int iLoopT = 1;
	//m_CMDList.clear();
	//stringToHEX(IntTostrHex(iLoopT,8),&m_NewDobotOrder.cState[0]);
	//m_CMDList.push_back(m_NewDobotOrder);//��ѭ����������ȥ
	m_f1 = 3;
	m_f2 = 0;
	m_f3 = 0;//165.756 + 100 -20;//31.400;
	m_f4 = 45;//101.178-100-50-20 +68.823;//36.686;
	m_f5 = 45;//55.981-20-20;//29.595;
	m_f6 = 0.0;
	m_f7 = 0;
	m_f8 = 0;
	m_f9 = 0.0;
	m_f10 = 0.0;
	m_NewDobotOrder.cHead[0] = 0xA5;
	m_NewDobotOrder.cEnd[0] = 0x5A;
	OperDobot::FloatToByte(m_f1,m_NewDobotOrder.cState);
	OperDobot::FloatToByte(m_f2,m_NewDobotOrder.cAxis);
	OperDobot::FloatToByte(m_f3,m_NewDobotOrder.cX);
	OperDobot::FloatToByte(m_f4,m_NewDobotOrder.cY);
	OperDobot::FloatToByte(m_f5,m_NewDobotOrder.cZ);
	OperDobot::FloatToByte(m_f6,m_NewDobotOrder.cRHead);
	OperDobot::FloatToByte(m_f7,m_NewDobotOrder.cIsGrip);
	OperDobot::FloatToByte(m_f8,m_NewDobotOrder.cStartVe);
	OperDobot::FloatToByte(m_f9,m_NewDobotOrder.cEndVel);
	OperDobot::FloatToByte(m_f10,m_NewDobotOrder.cMaxVe);

	//if(g_OperDobot != NULL)
		//g_OperDobot->AddOrderList(m_NewDobotOrder);   


}


void CDobotCheckDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();
	//g_bRunning = false;
	//CDobotPoint p = g_OperDobot->GetStartAbsolutePosition();
	g_OperDobot->DobotStop();
	GetDlgItem( IDC_BUTTON1 )->EnableWindow(TRUE);
	CRect rect1(0,0,20,20),rect2(22,0,42,20);
	COLORREF crColor;
	static bool b = true;
	if(b)
		crColor = RGB(255, 0, 0);
	else
		crColor = RGB(0, 0, 255);
	//PaintDobotPoint(rect1,crColor);
	//PaintDobotPoint(rect2,crColor);
	b = !b;
}

void CDobotCheckDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	/*
	g_OperDobot->Go2StartPoint();
	CDobotPoint p = {0,0,0};
	CDobotPoint p1;
	SetupPosition(400.0,300.0,10,8,3,p);
	//for(int z=0;z<3;z++)
		for(int y=0;y<6;y++)
			for(int x=0;x<8;x++)
			{
				p1 = g_PositionGroup[x][y][2];
				PaintDobotPoint(p1,RGB(50,50,150));
			}
	*/

	//g_OperDobot->PointMoving(2,((CButton *)GetDlgItem(IDC_RADIO1))->GetCheck());

	//AfxBeginThread(CheckThread, (LPVOID)this);

	CString strCardNo;
	unsigned long fee0;
	BOOL b = g_pCardOP->checkcard(fee0,strCardNo);
	CString strMsg;
	strMsg.Format("��%ld",fee0);
	MessageBox(strMsg);
}


void CDobotCheckDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	/*
	unsigned int nThreadAddr;
	HANDLE	hSession = (HANDLE)_beginthreadex(0, 0, FlashingReading, this, 0, &nThreadAddr);
	if(hSession) CloseHandle( hSession );
	*/
	//g_OperDobot->PointMoving(0,((CButton *)GetDlgItem(IDC_RADIO1))->GetCheck());
	g_pCardOP->Consume();
}


void CDobotCheckDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int CurSel = m_TabCtrl.GetCurSel();
	switch(CurSel)
	{
		case 0:
			m_ScandCard.ShowWindow(TRUE);
			m_ScandCard.SetTimer(1,300,NULL);
			m_FlashCard.ShowWindow(FALSE);
			m_PressureCardNew.ShowWindow(FALSE);
			break;
		case 1:
			m_ScandCard.ShowWindow(FALSE);
			m_ScandCard.KillTimer(1);
			m_FlashCard.ShowWindow(TRUE);
			m_PressureCardNew.ShowWindow(FALSE);
			break;
		case 2:
			m_ScandCard.ShowWindow(FALSE);
			m_ScandCard.KillTimer(1);
			m_FlashCard.ShowWindow(FALSE);
			m_PressureCardNew.ShowWindow(TRUE);
		default:
			break;
	}
	*pResult = 0;
}

BOOL CDobotCheckDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->message == WM_LBUTTONDOWN)
	{
		if(pMsg->hwnd == GetDlgItem(IDC_BUTTON_LEFT)->m_hWnd)
		{
                     // ��ť����
			if(((CButton *)GetDlgItem(IDC_RADIO1))->GetCheck() || ((CButton *)GetDlgItem(IDC_RADIO2))->GetCheck())
				g_OperDobot->PointMoving(2,TRUE);
			else
				g_OperDobot->PointMoving(4,FALSE);
        }
		if(pMsg->hwnd == GetDlgItem(IDC_BUTTON_RIGHT)->m_hWnd)
		{
			if(((CButton *)GetDlgItem(IDC_RADIO1))->GetCheck() || ((CButton *)GetDlgItem(IDC_RADIO2))->GetCheck())
				g_OperDobot->PointMoving(1,TRUE);
			else
				g_OperDobot->PointMoving(3,FALSE);
		}
		if(pMsg->hwnd == GetDlgItem(IDC_BUTTON_UP)->m_hWnd)
		{
			if(((CButton *)GetDlgItem(IDC_RADIO1))->GetCheck())
				g_OperDobot->PointMoving(4,TRUE);
			else if(((CButton *)GetDlgItem(IDC_RADIO2))->GetCheck())
				g_OperDobot->PointMoving(6,TRUE);
			else
				g_OperDobot->PointMoving(5,FALSE);
		}
		if(pMsg->hwnd == GetDlgItem(IDC_BUTTON_DOWN)->m_hWnd)
		{
			if(((CButton *)GetDlgItem(IDC_RADIO1))->GetCheck())
				g_OperDobot->PointMoving(3,TRUE);
			else if(((CButton *)GetDlgItem(IDC_RADIO2))->GetCheck())
				g_OperDobot->PointMoving(5,TRUE);
			else
				g_OperDobot->PointMoving(6,FALSE);
		}
		if(pMsg->hwnd == GetDlgItem(IDC_BUTTON_BACK)->m_hWnd)
		{
			g_OperDobot->PointMoving(2,FALSE);
		}
		if(pMsg->hwnd == GetDlgItem(IDC_BUTTON_FORWARD)->m_hWnd)
		{
			g_OperDobot->PointMoving(1,FALSE);
		}
	}
	else if(pMsg->message == WM_LBUTTONUP)
	{
		if(pMsg->hwnd == GetDlgItem(IDC_BUTTON_LEFT)->m_hWnd)
		{
			g_OperDobot->PointMoving(0,((CButton *)GetDlgItem(IDC_RADIO1))->GetCheck());// ��ť����
			m_PressureCardNew.m_PressureCardDlg->OnBnClickedButtonRefresh();
		}
		if(pMsg->hwnd == GetDlgItem(IDC_BUTTON_RIGHT)->m_hWnd)
		{
			g_OperDobot->PointMoving(0,((CButton *)GetDlgItem(IDC_RADIO1))->GetCheck());
			m_PressureCardNew.m_PressureCardDlg->OnBnClickedButtonRefresh();
		}
		if(pMsg->hwnd == GetDlgItem(IDC_BUTTON_UP)->m_hWnd)
		{
			g_OperDobot->PointMoving(0,((CButton *)GetDlgItem(IDC_RADIO1))->GetCheck());
			m_PressureCardNew.m_PressureCardDlg->OnBnClickedButtonRefresh();
		}
		if(pMsg->hwnd == GetDlgItem(IDC_BUTTON_DOWN)->m_hWnd)
		{
			g_OperDobot->PointMoving(0,((CButton *)GetDlgItem(IDC_RADIO1))->GetCheck());
			m_PressureCardNew.m_PressureCardDlg->OnBnClickedButtonRefresh();
		}
		if(pMsg->hwnd == GetDlgItem(IDC_BUTTON_BACK)->m_hWnd)
		{
			g_OperDobot->PointMoving(0,FALSE);
			m_PressureCardNew.m_PressureCardDlg->OnBnClickedButtonRefresh();
		}
		if(pMsg->hwnd == GetDlgItem(IDC_BUTTON_FORWARD)->m_hWnd)
		{
			g_OperDobot->PointMoving(0,FALSE);
			m_PressureCardNew.m_PressureCardDlg->OnBnClickedButtonRefresh();
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CDobotCheckDlg::OnBnClickedButtonMark()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_OperDobot->theDobotStatusCritical.Lock();
	g_OperDobot->m_DobotStartStatus = g_OperDobot->m_DobotAndWinStatus.m_DS;
	g_OperDobot->theDobotStatusCritical.Unlock();
	GetDlgItem(IDC_BUTTON_MARK)->EnableWindow(FALSE);
}


