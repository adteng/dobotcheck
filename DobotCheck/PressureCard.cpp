// PressureCard.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DobotCheck.h"
#include "PressureCard.h"
#include "afxdialogex.h"
#include "DobotCheckDlg.h"

// CPressureCard �Ի���

IMPLEMENT_DYNAMIC(CPressureCard, CDialogEx)

CPressureCard::CPressureCard(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPressureCard::IDD, pParent)
{

}

CPressureCard::~CPressureCard()
{
}

void CPressureCard::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CONTEOLTYPE, m_ControlType);
	DDX_Control(pDX, IDC_COMBO_MOVETYPE, m_MoveType);
	DDX_Control(pDX, IDC_EDIT_X, m_X);
	DDX_Control(pDX, IDC_EDIT_Y, m_Y);
	DDX_Control(pDX, IDC_EDIT_Z, m_Z);
	DDX_Control(pDX, IDC_EDIT_R, m_R);
	DDX_Control(pDX, IDC_COMBO_ISGRIP, m_IsGrip);
	DDX_Control(pDX, IDC_EDIT_PAUSETIME, m_PauseTime);
	DDX_Control(pDX, IDC_EDIT_LOOP, m_Loop);
	DDX_Control(pDX, IDC_LIST_CMD, m_list_cmd);
	DDX_Control(pDX, IDC_EDIT_GRIPPER, m_Gripper);
	DDX_Control(pDX, IDC_EDIT_LOOP2, m_Loop2);
	DDX_Control(pDX, IDC_LIST_SPORT, m_list_sport);
}


BEGIN_MESSAGE_MAP(CPressureCard, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CPressureCard::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CPressureCard::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_SAVEFILE, &CPressureCard::OnBnClickedButtonSavefile)
	ON_BN_CLICKED(IDC_BUTTON_OPENEFILE, &CPressureCard::OnBnClickedButtonOpenefile)
	ON_BN_CLICKED(IDC_BUTTON_PAUSE, &CPressureCard::OnBnClickedButtonPause)
	ON_BN_CLICKED(IDC_BUTTON_EXE, &CPressureCard::OnBnClickedButtonExe)
	ON_BN_CLICKED(IDC_BUTTON_EXESEL, &CPressureCard::OnBnClickedButtonExesel)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, &CPressureCard::OnBnClickedButtonRefresh)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CMD, &CPressureCard::OnNMDblclkListCmd)
	ON_LBN_DBLCLK(IDC_LIST_SPORT, &CPressureCard::OnLbnDblclkListSport)
	ON_LBN_SELCHANGE(IDC_LIST_SPORT, &CPressureCard::OnLbnSelchangeListSport)
END_MESSAGE_MAP()


// CPressureCard ��Ϣ�������

extern OperDobot *g_OperDobot;
extern CDobotCheckDlg *g_dlg;
BOOL CPressureCard::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	bExeStop = TRUE;
	m_list_cmd.InsertColumn(0,"����",LVCFMT_CENTER,50);
	m_list_cmd.InsertColumn(1,"���Ʒ�ʽ",LVCFMT_CENTER,80);
	m_list_cmd.InsertColumn(2,"Axis",LVCFMT_CENTER,50);
	m_list_cmd.InsertColumn(3,"�˶�ģʽ",LVCFMT_CENTER,80);
	m_list_cmd.InsertColumn(4,"����",LVCFMT_CENTER,60);
	m_list_cmd.InsertColumn(5,"���",LVCFMT_CENTER,60);
	m_list_cmd.InsertColumn(6,"С��",LVCFMT_CENTER,60);
	m_list_cmd.InsertColumn(7,"���",LVCFMT_CENTER,60);
	m_list_cmd.InsertColumn(8,"���̿���",LVCFMT_CENTER,80);
	m_list_cmd.InsertColumn(9,"צ�ӽǶ�",LVCFMT_CENTER,80);
	m_list_cmd.InsertColumn(10,"��ͣʱ��(s)",LVCFMT_CENTER,80);

	DWORD dwStyle = m_list_cmd.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
	dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��
//	dwStyle |= LVS_EX_CHECKBOXES;//itemǰ����checkbox�ؼ�
	m_list_cmd.SetExtendedStyle(dwStyle); //������չ���
	 int nIndex = 0;
    //ѡ��
    m_list_cmd.SetItemState(nIndex, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
    //ȡ��ѡ��
//    m_list_cmd.SetItemState(nIndex, 0, LVIS_SELECTED|LVIS_FOCUSED);
	//OnRadioAxis();
	m_Loop.SetWindowText("1");
	m_ControlType.SetCurSel(0);
	m_MoveType.SetCurSel(0);
	m_IsGrip.SetCurSel(1);
	m_X.SetWindowText("0");
	m_Y.SetWindowText("0");
	m_Z.SetWindowText("0");
	m_R.SetWindowText("0");
	m_Gripper.SetWindowText("0");
	m_PauseTime.SetWindowText("0");
	nItemCur = 0;
	nItemCnt = 0;
	bDelAllItem = false;
	//m_Slider.SetRange(1,100);
	//m_Slider.SetPos(50);
	//m_MoveSpeed.SetWindowText("50");
	//m_BT_Param.EnableWindow(false);
	sFileName = "Dobot.play";
	OnBnClickedButtonOpenefile();
	//theLog.Open("Dobot.log");
	//if(!theLogCritical.Init()) return -1;
	//if(!theDobotOrderCritical.Init()) return -1;
	//if(!theDobotStatusCritical.Init()) return -1;
//	if(!theWinStatusCritical.Init()) return -1;
	//memset(&m_DobotStatus,0,sizeof(DobotStatus));	
	m_IsGrip.AddString("YES");
	m_IsGrip.AddString("NO");
	m_IsGrip.SetCurSel(0);
	m_MoveType.AddString("0:Jump");
	m_MoveType.AddString("1:Movj");
	m_MoveType.AddString("2:Movl");
	m_MoveType.SetCurSel(0);
	m_ControlType.AddString("6ָ���˶�");
	m_ControlType.SetCurSel(0);


	//m_list_sport.AddString("��������");
	//m_list_sport.AddString("��������1");
	//m_list_sport.AddString("��������2");
	FindFileFromDir();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CPressureCard::SetItemNO()
{
	int i;
	char sBuffer[8];

	for(i=0;i<nItemCnt;i++)
	{
		memset(sBuffer,0,sizeof(sBuffer));
		sprintf(sBuffer,"%d",i+1);// = IToString(nItemCur);
		m_list_cmd.SetItemText(i,0,sBuffer);
	}
}


void CPressureCard::OnBnClickedButtonAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	int nItem;
	char sBuffer[16];
	/*
	POSITION pos = m_list_cmd.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		nItemCur = m_list_cmd.GetNextSelectedItem(pos) + 1;
	}else{
		nItemCur = nItemCnt;
	}
	*/
	nItemCur = m_list_cmd.GetItemCount();
	nItem = m_list_cmd.InsertItem (nItemCur,"");//.InsertItem(1,"0");
	memset(sBuffer,0,sizeof(sBuffer));
	sprintf(sBuffer,"%d",nItemCur+1);// = IToString(nItemCur);
	m_list_cmd.SetItemText(nItem,0,sBuffer);
	memset(sBuffer,0,sizeof(sBuffer));m_ControlType.GetWindowText(sBuffer,2);
	m_list_cmd.SetItemText(nItem,1,sBuffer);
	m_list_cmd.SetItemText(nItem,2,"");
	memset(sBuffer,0,sizeof(sBuffer));
	m_MoveType.GetWindowText(sBuffer,15);
	m_list_cmd.SetItemText(nItem,3,sBuffer);
	memset(sBuffer,0,sizeof(sBuffer));m_X.GetWindowText(sBuffer,15);
	m_list_cmd.SetItemText(nItem,4,sBuffer);
	memset(sBuffer,0,sizeof(sBuffer));m_Y.GetWindowText(sBuffer,15);
	m_list_cmd.SetItemText(nItem,5,sBuffer);
	memset(sBuffer,0,sizeof(sBuffer));m_Z.GetWindowText(sBuffer,15);
	m_list_cmd.SetItemText(nItem,6,sBuffer);
	memset(sBuffer,0,sizeof(sBuffer));m_R.GetWindowText(sBuffer,15);
	m_list_cmd.SetItemText(nItem,7,sBuffer);
	memset(sBuffer,0,sizeof(sBuffer));m_IsGrip.GetWindowText(sBuffer,15);
	m_list_cmd.SetItemText(nItem,8,sBuffer);
	memset(sBuffer,0,sizeof(sBuffer));m_Gripper.GetWindowText(sBuffer,15);
	m_list_cmd.SetItemText(nItem,9,sBuffer);
	memset(sBuffer,0,sizeof(sBuffer));m_PauseTime.GetWindowText(sBuffer,15);
	m_list_cmd.SetItemText(nItem,10,sBuffer);

	nItemCnt ++;
	SetItemNO();
}


void CPressureCard::OnBnClickedButtonDel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nItem;
	UINT flag;
	POSITION pos,pos1;

	flag = LVIS_SELECTED|LVIS_FOCUSED;
	if(bDelAllItem)
	{
		for(int j=0;j<nItemCnt;j++)
		{
			m_list_cmd.SetItemState(j, flag, flag);//ȫѡItem
		}
	}
	pos = m_list_cmd.GetFirstSelectedItemPosition();
	pos1 = pos;
	while (pos)
	{
		nItem = m_list_cmd.GetNextSelectedItem(pos);
		m_list_cmd.DeleteItem(nItem);
		nItemCnt --;
		pos = m_list_cmd.GetFirstSelectedItemPosition();
	}

	SetItemNO();
	nItem = m_list_cmd.GetNextSelectedItem(pos1);
	m_list_cmd.SetItemState(nItem, flag, flag);
	bDelAllItem = false;
}

void CPressureCard::OnBnClickedButtonSavefile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog *lpszOpenFile;
	lpszOpenFile = new CFileDialog(TRUE,".","",
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR ,
		"(*.*)||");
	lpszOpenFile->m_ofn.lpstrInitialDir = lpszOpenFile->GetPathName();
	if(lpszOpenFile->DoModal() == IDOK)
	{
//		CString szGetName;

		ofstream m_ofs;
		char sBuffer[9];

		memset(sBuffer,0,9);
		sFileName = lpszOpenFile->GetPathName();//�õ����ļ���·�����ļ�
		m_ofs.close();m_ofs.clear ();
		m_ofs.open (sFileName.c_str(),ios::out);
		for(int i = 0;i < nItemCnt;i++)
		{
			m_list_cmd.GetItemText(i,1,sBuffer,8);
			m_ofs<<sBuffer<<";";memset(sBuffer,0,9);
			m_list_cmd.GetItemText(i,2,sBuffer,8);
			m_ofs<<sBuffer<<";";memset(sBuffer,0,9);
			m_list_cmd.GetItemText(i,3,sBuffer,8);
			m_ofs<<sBuffer<<";";memset(sBuffer,0,9);
			m_list_cmd.GetItemText(i,4,sBuffer,8);
			m_ofs<<sBuffer<<";";memset(sBuffer,0,9);
			m_list_cmd.GetItemText(i,5,sBuffer,8);
			m_ofs<<sBuffer<<";";memset(sBuffer,0,9);
			m_list_cmd.GetItemText(i,6,sBuffer,8);
			m_ofs<<sBuffer<<";";memset(sBuffer,0,9);
			m_list_cmd.GetItemText(i,7,sBuffer,8);
			m_ofs<<sBuffer<<";";memset(sBuffer,0,9);
			m_list_cmd.GetItemText(i,8,sBuffer,8);
			m_ofs<<sBuffer<<";";memset(sBuffer,0,9);
			m_list_cmd.GetItemText(i,9,sBuffer,8);
			m_ofs<<sBuffer<<";";memset(sBuffer,0,9);
			m_list_cmd.GetItemText(i,10,sBuffer,8);
			m_ofs<<sBuffer<<";";memset(sBuffer,0,9);
			m_ofs<<endl;
		}
		m_ofs.flush();
		m_ofs.close();
		FindFileFromDir();
	}

	delete lpszOpenFile;//�ͷŷ���ĶԻ���
}


void CPressureCard::OnBnClickedButtonOpenefile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ifstream m_ifs;
	char sBuffer[9];
	char cLine[81];
	string strLine;
	int nPos=0,nItem;

	if(sFileName == "")
	{
		CFileDialog *lpszOpenFile;
		lpszOpenFile = new CFileDialog(TRUE,".","",
			OFN_HIDEREADONLY|OFN_NOCHANGEDIR ,
			"(*.*)||");
		lpszOpenFile->m_ofn.lpstrInitialDir = ".";
		if(lpszOpenFile->DoModal() == IDOK)
		{
			sFileName = lpszOpenFile->GetPathName();//�õ����ļ���·��
			delete lpszOpenFile;//�ͷŷ���ĶԻ���
		}else{
			return;
		}
	}
 	memset(sBuffer,0,9);
	m_ifs.close();m_ifs.clear ();
	m_ifs.open (sFileName.c_str(),ios::in);
	if(!m_ifs) return ;
	memset(cLine,0,81);
	sFileName = "";
	bDelAllItem = true;
	OnBnClickedButtonDel();//OnButtonDel();
	for(int i=0;m_ifs.getline(cLine,80);i++)
	{
		strLine = cLine;OperDobot::strTrim(strLine);
		nItem = m_list_cmd.InsertItem (i,"");//.InsertItem(1,"0");
		m_list_cmd.SetItemText(nItem,0,OperDobot::IToString(i+1).c_str());
		for(int j=1;j<=10;j++)
		{
			nPos = strLine.find (";");memset(sBuffer,0,9);
			memcpy(sBuffer,strLine.c_str (),nPos);
			m_list_cmd.SetItemText(i,j,sBuffer);
			strLine.erase(0,nPos+1);
		}
		nItemCnt ++;
	}
	m_ifs.close();

	UpdateData(false);
}


void CPressureCard::OnBnClickedButtonPause()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//theDobotStatusCritical.Lock();
	//m_DobotAndWinStatus.bExeStop = true;
	//theDobotStatusCritical.unLock();
	bExeStop = TRUE;
}

unsigned int __stdcall PressureReading(void * param)
{
	CPressureCard *dlg = (CPressureCard *)param;
	list<DobotOrder>::iterator it = dlg->m_CMDList.begin();
	while(it != dlg->m_CMDList.end())
	{
		g_OperDobot->AddOrderList(*it);
		it = dlg->m_CMDList.erase(it);
		if(dlg->bExeStop) break;
		Sleep(1000);
	}
	dlg->bExeStop = TRUE;
	dlg->m_CMDList.clear();
	return 0;
}

void CPressureCard::OnBnClickedButtonExe()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	UINT flag;
	int nRet;

//	m_Status.SetWindowText("��ESCִֹͣ��ָ�");
	m_Loop2.SetWindowText("0");
	//m_DobotAndWinStatus.bExeStop = false;
	UpdateData(false);
	flag = LVIS_SELECTED|LVIS_FOCUSED;
	for(int j=0;j<nItemCnt;j++)
	{
		m_list_cmd.SetItemState(j, flag, flag);//ȫѡItem
	}
	//OnButtonExeSel();//OnButtonExeSel(true);//
//	if((nRet = ExeSel(true)) < 0) return;// -1;//ִ��ѡ�е�Item
//	m_Status.SetWindowText("");
	


	ExeSel(TRUE);

}

void CPressureCard::ExeSel(bool bExeAll)
{
	int nItem;
	UINT flag;
	char sBuffer[16];
	float fTmp;
	unsigned char cBuf[5];
	DobotOrder m_NewDobotOrder;

	memset(&m_NewDobotOrder.cHead[0],0,sizeof(DobotOrder));
	m_NewDobotOrder.cHead[0] = 0xA5;
	m_NewDobotOrder.cEnd[0] = 0x5A;


	memset(sBuffer,0,16);
	memset(cBuf,0,5);

	
	if(!bExeAll)
	{
		m_Loop.SetWindowText("1");
		m_Loop2.SetWindowText("0");
	}

	bExeStop = FALSE;
	m_CMDList.clear();
	m_Loop.GetWindowText(sBuffer,7);

	flag = LVIS_SELECTED|LVIS_FOCUSED;
	POSITION pos = m_list_cmd.GetFirstSelectedItemPosition();
	while (pos)
	{
		nItem = m_list_cmd.GetNextSelectedItem(pos);
		m_list_cmd.GetItemText(nItem,1,sBuffer,15);
		fTmp = atof(sBuffer);OperDobot::FloatToByte(fTmp,cBuf);
		memcpy(m_NewDobotOrder.cState,cBuf,4);//�˶�ģʽ

		OperDobot::stringToHEX("00000000",m_NewDobotOrder.cAxis,8);//cAxis

		m_list_cmd.GetItemText(nItem,3,sBuffer,15);
		if(sBuffer[0] == '0') 
			OperDobot::stringToHEX("00000000",m_NewDobotOrder.cStartVe,8);//�˶�ģʽ 0��Jump��1��Movj��2:Movl
		else if(sBuffer[0] == '1')
			OperDobot::stringToHEX("0000803F",m_NewDobotOrder.cStartVe,8);//�˶�ģʽ
		else
			OperDobot::stringToHEX("00000040",m_NewDobotOrder.cStartVe,8);//�˶�ģʽ

		m_list_cmd.GetItemText(nItem,4,sBuffer,15);
		fTmp = atof(sBuffer);OperDobot::FloatToByte(fTmp,cBuf);
		memcpy(m_NewDobotOrder.cX,cBuf,4);

		m_list_cmd.GetItemText(nItem,5,sBuffer,15);
		fTmp = atof(sBuffer);OperDobot::FloatToByte(fTmp,cBuf);
		memcpy(m_NewDobotOrder.cY,cBuf,4);

		m_list_cmd.GetItemText(nItem,6,sBuffer,15);
		fTmp = atof(sBuffer);OperDobot::FloatToByte(fTmp,cBuf);
		memcpy(m_NewDobotOrder.cZ,cBuf,4);

		m_list_cmd.GetItemText(nItem,7,sBuffer,15);
		fTmp = atof(sBuffer);OperDobot::FloatToByte(fTmp,cBuf);
		memcpy(m_NewDobotOrder.cRHead,cBuf,4);

		m_list_cmd.GetItemText(nItem,8,sBuffer,15);
		if(sBuffer[0] == 'Y') 
		{
			OperDobot::stringToHEX("0000803F",m_NewDobotOrder.cIsGrip,8);//1
		}else{
			OperDobot::stringToHEX("00000000",m_NewDobotOrder.cIsGrip,8);//0
		}
		m_list_cmd.GetItemText(nItem,9,sBuffer,15);
		fTmp = atof(sBuffer);OperDobot::FloatToByte(fTmp,cBuf);
		memcpy(m_NewDobotOrder.cEndVel,cBuf,4);

		m_list_cmd.GetItemText(nItem,10,sBuffer,15);
		fTmp = atof(sBuffer);OperDobot::FloatToByte(fTmp,cBuf);
		memcpy(m_NewDobotOrder.cMaxVe,cBuf,4);

		m_CMDList.push_back(m_NewDobotOrder);
		m_list_cmd.SetItemState(nItem, 0,flag);//ȡ��ѡ��
		pos = m_list_cmd.GetFirstSelectedItemPosition();
	}

	unsigned int nThreadAddr;
	HANDLE	hSession = (HANDLE)_beginthreadex(0, 0, PressureReading, this, 0, &nThreadAddr);
	if(hSession) CloseHandle( hSession );
}

void CPressureCard::OnBnClickedButtonExesel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ExeSel(FALSE);
}


void CPressureCard::OnBnClickedButtonRefresh()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char sData[64] = {0};
	CString str;
	g_dlg->GetDlgItem(IDC_EDIT6)->GetWindowTextA(str);m_X.SetWindowTextA(str);
	g_dlg->GetDlgItem(IDC_EDIT7)->GetWindowTextA(str);m_Y.SetWindowTextA(str);
	g_dlg->GetDlgItem(IDC_EDIT8)->GetWindowTextA(str);m_Z.SetWindowTextA(str);
	g_dlg->GetDlgItem(IDC_EDIT9)->GetWindowTextA(str);m_R.SetWindowTextA(str);
	g_dlg->GetDlgItem(IDC_EDIT10)->GetWindowTextA(str);  atof(str) >= 0.9 ?   m_IsGrip.SetCurSel(0) : m_IsGrip.SetCurSel(1);
	g_dlg->GetDlgItem(IDC_EDIT11)->GetWindowTextA(str);m_Gripper.SetWindowTextA(str);
}


void CPressureCard::OnNMDblclkListCmd(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nItem;
	UINT flag;
	char sBuffer[16];

	memset(sBuffer,0,16);
	flag = LVIS_SELECTED|LVIS_FOCUSED;
	POSITION pos = m_list_cmd.GetFirstSelectedItemPosition();
	nItem = m_list_cmd.GetNextSelectedItem(pos);


	memset(sBuffer,0,16);	m_list_cmd.GetItemText(nItem,3,sBuffer,15);
	if(sBuffer[0] == '0')
		m_MoveType.SetCurSel(0);
	else if(sBuffer[0] == '1')
		m_MoveType.SetCurSel(1);
	else
		m_MoveType.SetCurSel(2);
	memset(sBuffer,0,16);	m_list_cmd.GetItemText(nItem,4,sBuffer,15);
	m_X.SetWindowText(sBuffer);
	memset(sBuffer,0,16);	m_list_cmd.GetItemText(nItem,5,sBuffer,15);
	m_Y.SetWindowText(sBuffer);
	memset(sBuffer,0,16);	m_list_cmd.GetItemText(nItem,6,sBuffer,15);
	m_Z.SetWindowText(sBuffer);
	memset(sBuffer,0,16);	m_list_cmd.GetItemText(nItem,7,sBuffer,15);
	m_R.SetWindowText(sBuffer);
	memset(sBuffer,0,16);	m_list_cmd.GetItemText(nItem,8,sBuffer,15);
	if(sBuffer[0] == 'Y')
		m_IsGrip.SetCurSel(0);
	else
		m_IsGrip.SetCurSel(1);
	memset(sBuffer,0,16);	m_list_cmd.GetItemText(nItem,9,sBuffer,15);
	m_Gripper.SetWindowText(sBuffer);
	memset(sBuffer,0,16);	m_list_cmd.GetItemText(nItem,10,sBuffer,15);
	m_PauseTime.SetWindowText(sBuffer);

	*pResult = 0;
}


void CPressureCard::OnLbnDblclkListSport()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CPressureCard::OnLbnSelchangeListSport()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strFileName,strTmp;
	m_list_sport.GetText(m_list_sport.GetCurSel(),strTmp);
	strFileName.Format("data/%s",strTmp);
	ifstream m_ifs;
	char sBuffer[9];
	char cLine[81];
	string strLine;
	int nPos=0,nItem;
	m_ifs.close();m_ifs.clear ();
	m_ifs.open (strFileName.GetBuffer(0),ios::in);
	if(!m_ifs) 
	{
		CString str;
		str = "���ļ�<" + strFileName + ">ʧ��";
		MessageBox(str);
		return ;
	}
	memset(cLine,0,81);
	sFileName = "";
	bDelAllItem = true;
	OnBnClickedButtonDel();//OnButtonDel();
	for(int i=0;m_ifs.getline(cLine,80);i++)
	{
		strLine = cLine;OperDobot::strTrim(strLine);
		nItem = m_list_cmd.InsertItem (i,"");//.InsertItem(1,"0");
		m_list_cmd.SetItemText(nItem,0,OperDobot::IToString(i+1).c_str());
		for(int j=1;j<=10;j++)
		{
			nPos = strLine.find (";");memset(sBuffer,0,9);
			memcpy(sBuffer,strLine.c_str (),nPos);
			m_list_cmd.SetItemText(i,j,sBuffer);
			strLine.erase(0,nPos+1);
		}
		nItemCnt ++;
	}
	m_ifs.close();
}

void CPressureCard::FindFileFromDir()
{
	CString strPath;
	CFileFind finder;
	m_list_sport.ResetContent();
	CString str =  "data\\*.play";
	BOOL bWorking = finder.FindFile((LPCTSTR)str);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		CString strFileName = finder.GetFileName();
		if(strFileName == "." || strFileName == "..") continue;

		m_list_sport.AddString(strFileName);

		//strPath = finder.GetFilePath();
	}
	finder.Close();
}
