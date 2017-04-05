// PressureCardNew.cpp : 实现文件
//

#include "stdafx.h"
#include "DobotCheck.h"
#include "PressureCardNew.h"
#include "afxdialogex.h"

// CPressureCardNew 对话框

IMPLEMENT_DYNAMIC(CPressureCardNew, CDialogEx)

CPressureCardNew::CPressureCardNew(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPressureCardNew::IDD, pParent)
	, m_iLoopTotalSum(0)
	, m_iLoopCurrentSum(0)
{

}

CPressureCardNew::~CPressureCardNew()
{
}

void CPressureCardNew::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SPORT, m_combo_splist);
	DDX_Text(pDX, IDC_EDIT_LOOP_TOTAL_SUM, m_iLoopTotalSum);
	DDX_Text(pDX, IDC_EDIT_CURRENT_LOOP_SUM, m_iLoopCurrentSum);
}


BEGIN_MESSAGE_MAP(CPressureCardNew, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_START_PRESSURE, &CPressureCardNew::OnBnClickedButtonStartPressure)
	ON_BN_CLICKED(IDC_BUTTON_ADD_PRESSURE, &CPressureCardNew::OnBnClickedButtonAddPressure)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, &CPressureCardNew::OnBnClickedButtonRefresh)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CPressureCardNew::OnBnClickedButtonStop)
END_MESSAGE_MAP()


// CPressureCardNew 消息处理程序
extern OperDobot *g_OperDobot;
unsigned int __stdcall PressureReadingNew(void * param);

void CPressureCardNew::OnBnClickedButtonStartPressure()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int index = m_combo_splist.GetCurSel();
	if(index < 0) 
	{
		MessageBox("请选择一个方案");
		return ;
	}
	char sFileName[64] = {0};
	m_combo_splist.GetWindowTextA(sFileName,64);

	DobotOrder m_NewDobotOrder;
	memset(&m_NewDobotOrder.cHead[0],0,sizeof(DobotOrder));
	m_NewDobotOrder.cHead[0] = 0xA5;
	m_NewDobotOrder.cEnd[0] = 0x5A;

	CString strFileName;
	strFileName.Format("data/%s",sFileName);
	ifstream m_ifs;
	char sBuffer[9];
	char cLine[81];
	string strLine;
	int nPos=0,nItem;
	 memset(sBuffer,0,9);
	m_ifs.close();m_ifs.clear ();
	m_ifs.open (strFileName.GetBuffer(0),ios::in);
	if(!m_ifs) return ;
	m_CMDList.clear();
	memset(cLine,0,81);
	for(int i=0;m_ifs.getline(cLine,80);i++)
	{
		strLine = cLine;strTrim(strLine);
		//for(int j=1;j<=10;j++)
		{
			nPos = strLine.find (";");memset(sBuffer,0,9);
			memcpy(sBuffer,strLine.c_str (),nPos);
			FloatToByte(atof(sBuffer),m_NewDobotOrder.cState);//m_list_cmd.SetItemText(i,j,sBuffer);
			strLine.erase(0,nPos+1);
		}
		{
			nPos = strLine.find (";");memset(sBuffer,0,9);
			memcpy(sBuffer,strLine.c_str (),nPos);
			//m_list_cmd.SetItemText(i,j,sBuffer);
			strLine.erase(0,nPos+1);
		}
		{
			nPos = strLine.find (";");memset(sBuffer,0,9);
			memcpy(sBuffer,strLine.c_str (),nPos);
			if(sBuffer[0] == '0') 
				stringToHEX("00000000",m_NewDobotOrder.cStartVe,8);//运动模式 0：Jump；1：Movj；2:Movl
			else if(sBuffer[0] == '1')
				stringToHEX("0000803F",m_NewDobotOrder.cStartVe,8);//运动模式
			else
				stringToHEX("00000040",m_NewDobotOrder.cStartVe,8);//运动模式

			strLine.erase(0,nPos+1);
		}
		{
			nPos = strLine.find (";");memset(sBuffer,0,9);
			memcpy(sBuffer,strLine.c_str (),nPos);
			FloatToByte(atof(sBuffer),m_NewDobotOrder.cX);//m_list_cmd.SetItemText(i,j,sBuffer);
			strLine.erase(0,nPos+1);
		}
		{
			nPos = strLine.find (";");memset(sBuffer,0,9);
			memcpy(sBuffer,strLine.c_str (),nPos);
			FloatToByte(atof(sBuffer),m_NewDobotOrder.cY);//m_list_cmd.SetItemText(i,j,sBuffer);
			strLine.erase(0,nPos+1);
		}
		{
			nPos = strLine.find (";");memset(sBuffer,0,9);
			memcpy(sBuffer,strLine.c_str (),nPos);
			FloatToByte(atof(sBuffer),m_NewDobotOrder.cZ);//m_list_cmd.SetItemText(i,j,sBuffer);
			strLine.erase(0,nPos+1);
		}
		{
			nPos = strLine.find (";");memset(sBuffer,0,9);
			memcpy(sBuffer,strLine.c_str (),nPos);
			FloatToByte(atof(sBuffer),m_NewDobotOrder.cRHead);//m_list_cmd.SetItemText(i,j,sBuffer);
			strLine.erase(0,nPos+1);
		}
		{
			nPos = strLine.find (";");memset(sBuffer,0,9);
			memcpy(sBuffer,strLine.c_str (),nPos);
			if(sBuffer[0] == 'Y') 
			{
				stringToHEX("0000803F",m_NewDobotOrder.cIsGrip,8);//1
			}else
			{
				stringToHEX("00000000",m_NewDobotOrder.cIsGrip,8);//0
			}

			strLine.erase(0,nPos+1);
		}
		{
			nPos = strLine.find (";");memset(sBuffer,0,9);
			memcpy(sBuffer,strLine.c_str (),nPos);
			FloatToByte(atof(sBuffer),m_NewDobotOrder.cEndVel);//m_list_cmd.SetItemText(i,j,sBuffer);
			strLine.erase(0,nPos+1);
		}
		{
			nPos = strLine.find (";");memset(sBuffer,0,9);
			memcpy(sBuffer,strLine.c_str (),nPos);
			FloatToByte(atof(sBuffer),m_NewDobotOrder.cMaxVe);//m_list_cmd.SetItemText(i,j,sBuffer);
			strLine.erase(0,nPos+1);
		}
		//nItemCnt ++;
		m_CMDList.push_back(m_NewDobotOrder);
	}
	m_ifs.close();
	bExeStop = FALSE;
	unsigned int nThreadAddr;
	HANDLE	hSession = (HANDLE)_beginthreadex(0, 0, PressureReadingNew, this, 0, &nThreadAddr);
	if(hSession) CloseHandle( hSession );

}


void CPressureCardNew::OnBnClickedButtonAddPressure()
{
	// TODO: 在此添加控件通知处理程序代码
	
	//PressureCardDlg.Create(IDD_DIALOG_M3,GetDlgItem(IDC_TAB1));
	//PressureCardDlg.DoModal();
	m_PressureCardDlg->ShowWindow(SW_SHOWNORMAL);
}


BOOL CPressureCardNew::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_PressureCardDlg = new CPressureCard();
	m_PressureCardDlg->Create(IDD_DIALOG_M3);
	// TODO:  在此添加额外的初始化
	//this->m_combo_splist.AddString("aa");
	m_iLoopTotalSum = 1;
	m_iLoopCurrentSum = 0;
	bExeStop = TRUE;
	FindFileFromDir();
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPressureCardNew::FindFileFromDir()
{
	CString strPath;
	CFileFind finder;
	m_combo_splist.ResetContent();
	CString str =  "data\\*.play";
	BOOL bWorking = finder.FindFile((LPCTSTR)str);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		CString strFileName = finder.GetFileName();
		if(strFileName == "." || strFileName == "..") continue;

		m_combo_splist.AddString(strFileName);

		//strPath = finder.GetFilePath();
	}
	finder.Close();
}


void CPressureCardNew::OnBnClickedButtonRefresh()
{
	// TODO: 在此添加控件通知处理程序代码
	FindFileFromDir();
}


void CPressureCardNew::OnBnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码
	bExeStop = TRUE;
}

unsigned int __stdcall PressureReadingNew(void * param)
{
	CPressureCardNew *dlg = (CPressureCardNew *)param;
	dlg->GetDlgItem(IDC_BUTTON_START_PRESSURE)->EnableWindow(FALSE);
	for(int i = 0; i < dlg->m_iLoopTotalSum;i++)
	{
		list<DobotOrder>::iterator it;
		for(it = dlg->m_CMDList.begin();it != dlg->m_CMDList.end();it++)
		{
			g_OperDobot->AddOrderList(*it);
			if(dlg->bExeStop) break;
			Sleep(1000);
		}
		if(dlg->bExeStop) break;
		CString str;
		str.Format("%d",i+1);
		dlg->GetDlgItem(IDC_EDIT_CURRENT_LOOP_SUM)->SetWindowTextA(str.GetBuffer(0));
		Sleep(1000);
	}
	dlg->bExeStop = TRUE;
	dlg->m_CMDList.clear();
	dlg->GetDlgItem(IDC_BUTTON_START_PRESSURE)->EnableWindow(TRUE);
	return 0;
}