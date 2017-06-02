// PressureCard.cpp : 实现文件
//

#include "stdafx.h"
#include "DobotCheck.h"
#include "PressureCard.h"
#include "afxdialogex.h"
#include "DobotCheckDlg.h"
#include "FlashFileNameDlg.h"
#include "DownloadSettingDlg.h"
#include "ModifyDlg.h"

// CPressureCard 对话框

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
	ON_NOTIFY(NM_RCLICK, IDC_LIST_CMD, &CPressureCard::OnNMRClickListCmd)
	ON_COMMAND(ID_MODIFY, &CPressureCard::OnModify)
	ON_COMMAND(ID_PASTE, &CPressureCard::OnPaste)
	ON_COMMAND(ID_COPY, &CPressureCard::OnCopy)
	ON_COMMAND(ID_DEL, &CPressureCard::OnDel)
	ON_COMMAND(ID_DEL_ALL, &CPressureCard::OnDelAll)
END_MESSAGE_MAP()


// CPressureCard 消息处理程序

extern OperDobot *g_OperDobot;
extern CDobotCheckDlg *g_dlg;
BOOL g_running = FALSE;
unsigned int __stdcall DownloadThread(void * param);
unsigned int __stdcall RunAllThread(void * param);
unsigned int __stdcall RunSelectedThread(void * param);

BOOL CPressureCard::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_list_cmd.InsertColumn(0,"序列",LVCFMT_CENTER,50);
	m_list_cmd.InsertColumn(1,"X坐标",LVCFMT_CENTER,180);
	m_list_cmd.InsertColumn(2,"Y坐标",LVCFMT_CENTER,180);
	m_list_cmd.InsertColumn(3,"Z坐标",LVCFMT_CENTER,180);
	m_list_cmd.InsertColumn(4,"底座",LVCFMT_CENTER,0);
	m_list_cmd.InsertColumn(5,"大臂",LVCFMT_CENTER,0);
	m_list_cmd.InsertColumn(6,"小臂",LVCFMT_CENTER,0);
	m_list_cmd.InsertColumn(7,"暂停时间(s)",LVCFMT_CENTER,162);

	DWORD dwStyle = m_list_cmd.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
//	dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	m_list_cmd.SetExtendedStyle(dwStyle); //设置扩展风格
    //选中
    //m_list_cmd.SetItemState(nIndex, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	m_data.clear();
	m_copy_data.clear();
	GetDlgItem(IDC_EDIT_PAUSETIME)->SetWindowTextA("0.0");
	GetDlgItem(IDC_EDIT_LOOP)->SetWindowTextA("1");
	FindFileFromDir();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}




void CPressureCard::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	Pose p= ((OperDobot2 *)g_OperDobot)->GetCurrentPoint2();
	char sBuffer[24] = {0};
	int nCountRow = m_list_cmd.GetItemCount();
	int nItem = m_list_cmd.InsertItem (nCountRow,"");
	sprintf(sBuffer,"%d",nCountRow+1);
	m_list_cmd.SetItemText(nItem,0,sBuffer);
	memset(sBuffer,0,24);sprintf(sBuffer,"%f",p.x);
	m_list_cmd.SetItemText(nItem,1,sBuffer);
	memset(sBuffer,0,24);sprintf(sBuffer,"%f",p.y);
	m_list_cmd.SetItemText(nItem,2,sBuffer);
	memset(sBuffer,0,24);sprintf(sBuffer,"%f",p.z);
	m_list_cmd.SetItemText(nItem,3,sBuffer);
	memset(sBuffer,0,24);sprintf(sBuffer,"%f",p.jointAngle[0]);
	m_list_cmd.SetItemText(nItem,4,sBuffer);
	memset(sBuffer,0,24);sprintf(sBuffer,"%f",p.jointAngle[1]);
	m_list_cmd.SetItemText(nItem,5,sBuffer);
	memset(sBuffer,0,24);sprintf(sBuffer,"%f",p.jointAngle[2]);
	m_list_cmd.SetItemText(nItem,6,sBuffer);
	CString str;GetDlgItem(IDC_EDIT_PAUSETIME)->GetWindowTextA(str);
	m_list_cmd.SetItemText(nItem,7,str);
	DataLi d;
	d.pt = p;
	d._time = atof((LPCTSTR)str);
	m_data.push_back(d);
}


void CPressureCard::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码
	int nItem;
	POSITION pos;
	//UINT flag = LVIS_SELECTED|LVIS_FOCUSED;
	pos = m_list_cmd.GetFirstSelectedItemPosition();
	while (pos)
	{
		nItem = m_list_cmd.GetNextSelectedItem(pos);
		vector<DataLi>::iterator it = m_data.begin() + nItem;
		m_data.erase(it);
		m_list_cmd.DeleteItem(nItem);
		pos = m_list_cmd.GetFirstSelectedItemPosition();
	}
}

void CPressureCard::OnBnClickedButtonSavefile()
{
	// TODO: 在此添加控件通知处理程序代码
	CFlashFileNameDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		CString strDBPath;
		strDBPath.Format("data/%s.play",dlg.m_strFlashFileName);
		if (!PathFileExists(strDBPath))
		{
			//sprintf(sFileName,"data/%02d%02d%02d%02d.flash",m_nDay,m_nHour,m_nMinute,m_nSecond);
			CmdWrite2File(strDBPath);
		}
		else
		{
			if(IDOK==MessageBox("名称存在，是否替换","名称存在",MB_OKCANCEL))
			{
				CmdWrite2File(strDBPath);
			}
		}
	}
}
void CPressureCard::CmdWrite2File(CString strFileName)
{
	if(m_data.empty()) return ;
	ofstream m_ofs;
	char sBuffer[24];
	m_ofs.close();m_ofs.clear ();
	m_ofs.open (strFileName.GetBuffer(0),ios::out);
	vector<DataLi>::iterator it;
	for(it = m_data.begin();it != m_data.end();++it)
	{
		DataLi d = *it;
		Pose ps = d.pt;
		memset(sBuffer,0,9);sprintf(sBuffer,"%f",ps.x);
		m_ofs<<sBuffer<<";";
		memset(sBuffer,0,9);sprintf(sBuffer,"%f",ps.y);
		m_ofs<<sBuffer<<";";
		memset(sBuffer,0,9);sprintf(sBuffer,"%f",ps.z);
		m_ofs<<sBuffer<<";";
		memset(sBuffer,0,9);sprintf(sBuffer,"%f",ps.jointAngle[0]);
		m_ofs<<sBuffer<<";";
		memset(sBuffer,0,9);sprintf(sBuffer,"%f",ps.jointAngle[1]);
		m_ofs<<sBuffer<<";";
		memset(sBuffer,0,9);sprintf(sBuffer,"%f",ps.jointAngle[2]);
		m_ofs<<sBuffer<<";" ;
		memset(sBuffer,0,9);sprintf(sBuffer,"%0.1f",d._time);
		m_ofs<<sBuffer<<";" << endl;
	}
	m_ofs.flush();
	m_ofs.close();
	FindFileFromDir();
}

typedef struct _PARAMETER 
{
	int arg[3];
	CPressureCard *dlg;
} PARAMETER;

void CPressureCard::OnBnClickedButtonOpenefile()
{
	// TODO: 在此添加控件通知处理程序代码
	if(g_running || m_data.empty())
	{
		return;
	}
	CDownloadSettingDlg dlg;
	dlg.m_bGohomeFlag = false;
	dlg.m_iLoopSum = 1;
	if(dlg.DoModal() == IDOK)
	{
		static PARAMETER par;
		par.arg[0] = dlg.m_iLoopSum;
		par.arg[1] = dlg.m_bGohomeFlag;
		par.dlg = this;
		g_running = TRUE;
		unsigned int nThreadAddr;
		HANDLE	hSession = (HANDLE)_beginthreadex(0, 0, DownloadThread, &par, 0, &nThreadAddr);
		if(hSession) CloseHandle( hSession );
	}
}


void CPressureCard::OnBnClickedButtonPause()
{
	// TODO: 在此添加控件通知处理程序代码
	g_running = FALSE;
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
	// TODO: 在此添加控件通知处理程序代码
	if(!g_running)
	{
		g_running = TRUE;
		unsigned int nThreadAddr;
		HANDLE	hSession = (HANDLE)_beginthreadex(0, 0, RunAllThread, this, 0, &nThreadAddr);
		if(hSession) CloseHandle( hSession );
	}
}

void CPressureCard::OnBnClickedButtonExesel()
{
	// TODO: 在此添加控件通知处理程序代码
	if(!g_running)
	{
		g_running = TRUE;
		unsigned int nThreadAddr;
		HANDLE	hSession = (HANDLE)_beginthreadex(0, 0, RunSelectedThread, this, 0, &nThreadAddr);
		if(hSession) CloseHandle( hSession );
	}
}


void CPressureCard::OnBnClickedButtonRefresh()
{
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    int index = pNMListView->iItem;
	OnModify();

	*pResult = 0;
}


void CPressureCard::OnLbnDblclkListSport()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CPressureCard::OnLbnSelchangeListSport()
{
	// TODO: 在此添加控件通知处理程序代码
	if(g_running)
	{
		return;
	}
	CString strFileName,strTmp;
	m_list_sport.GetText(m_list_sport.GetCurSel(),strTmp);
	strFileName.Format("data/%s.play",strTmp);
	ifstream m_ifs;
	char sBuffer[24];
	char cLine[81];
	string strLine;
	int nPos=0,nItem;
	m_ifs.close();m_ifs.clear ();
	m_ifs.open (strFileName.GetBuffer(0),ios::in);
	if(!m_ifs) 
	{
		CString str;
		str = "打开文件<" + strFileName + ">失败";
		MessageBox(str);
		return ;
	}
	m_list_cmd.DeleteAllItems();
	m_data.clear();
	memset(cLine,0,81);
	for(int i=0;m_ifs.getline(cLine,80);i++)
	{
		Pose p;
		DataLi d;
		strLine = cLine;OperDobot::strTrim(strLine);
		nItem = m_list_cmd.InsertItem (i,"");
		m_list_cmd.SetItemText(nItem,0,OperDobot::IToString(i+1).c_str());
		for(int j=1;j<=7;j++)
		{
			nPos = strLine.find (";");memset(sBuffer,0,24);
			memcpy(sBuffer,strLine.c_str (),nPos);
			m_list_cmd.SetItemText(i,j,sBuffer);
			if(j == 1) p.x = atof(sBuffer);
			if(j == 2) p.y = atof(sBuffer);
			if(j == 3) p.z = atof(sBuffer);
			if(j == 4) p.jointAngle[0] = atof(sBuffer);
			if(j == 5) p.jointAngle[1] = atof(sBuffer);
			if(j == 6) p.jointAngle[2] = atof(sBuffer);
			if(j == 7) d._time = atof(sBuffer);
			strLine.erase(0,nPos+1);
		}
		d.pt = p;
		m_data.push_back(d);
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

		m_list_sport.AddString(strFileName.Mid(0,strFileName.GetLength()-5));

		//strPath = finder.GetFilePath();
	}
	finder.Close();
}
void CPressureCard::SetRunningStatus(bool b)
{
	g_dlg->GetDlgItem(IDC_BUTTON_LEFT           )->EnableWindow(b);
	g_dlg->GetDlgItem(IDC_BUTTON_RIGHT          )->EnableWindow(b);
	g_dlg->GetDlgItem(IDC_BUTTON_BACK           )->EnableWindow(b);
	g_dlg->GetDlgItem(IDC_BUTTON_FORWARD        )->EnableWindow(b);
	g_dlg->GetDlgItem(IDC_BUTTON_MARK           )->EnableWindow(b);
	g_dlg->GetDlgItem(IDC_BUTTON_UP             )->EnableWindow(b);
	g_dlg->GetDlgItem(IDC_BUTTON_DOWN           )->EnableWindow(b);
	GetDlgItem(IDC_BUTTON_ADD            )->EnableWindow(b);
	GetDlgItem(IDC_BUTTON_DEL            )->EnableWindow(b);
	GetDlgItem(IDC_BUTTON_SAVEFILE       )->EnableWindow(b);
	GetDlgItem(IDC_BUTTON_OPENEFILE      )->EnableWindow(b);
}

unsigned int __stdcall RunAllThread(void * param)
{
	CPressureCard *dlg = (CPressureCard *)param;
	CString strSum;
	dlg->GetDlgItem(IDC_EDIT_LOOP)->GetWindowTextA(strSum);
	dlg->SetRunningStatus(false);
	for(int i = 0; i < atoi(strSum) && g_running;i++)
	{
		vector<DataLi>::iterator it = dlg->m_data.begin();
		while(g_running && it != dlg->m_data.end())
		{
			int nIndex = it-dlg->m_data.begin();
			//LVITEMA itema;
			//g_dlg->m_list_cmd.GetItem(&itema);
			//g_dlg->m_list_cmd.SetItemState(nIndex, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
			DataLi d = *it;
			CDobotPoint pt;
			pt.x = d.pt.x;
			pt.y = d.pt.y;
			pt.z = d.pt.z;
			g_OperDobot->Move2AbsolutePosition(pt);
			if(d._time * 1000 > 0) Sleep(d._time * 1000);
			++it;
			//g_dlg->m_list_cmd.SetItemState(nIndex, 0, LVIS_SELECTED|LVIS_FOCUSED);
		}
		dlg->GetDlgItem(IDC_EDIT_LOOP)->GetWindowTextA(strSum);
		char sBuff[8] = {0};
		sprintf(sBuff,"%d",i+1);
		dlg->GetDlgItem(IDC_EDIT_LOOP2)->SetWindowTextA(sBuff);
	}
	dlg->SetRunningStatus(true);
	g_running = FALSE;
	return 0;
}
unsigned int __stdcall RunSelectedThread(void * param)
{
	CPressureCard *dlg = (CPressureCard *)param;
	CString strSum;
	dlg->GetDlgItem(IDC_EDIT_LOOP)->GetWindowTextA(strSum);	
	dlg->SetRunningStatus(false);
	for(int i = 0; i < atoi(strSum) && g_running;i++)
	{
		POSITION pos;
		pos = dlg->m_list_cmd.GetFirstSelectedItemPosition();
		while (pos && g_running)
		{
			int nItem = dlg->m_list_cmd.GetNextSelectedItem(pos);
			vector<DataLi>::iterator it = dlg->m_data.begin() + nItem;
			DataLi d = *it;
			CDobotPoint pt;
			pt.x = d.pt.x;
			pt.y = d.pt.y;
			pt.z = d.pt.z;
			g_OperDobot->Move2AbsolutePosition(pt);
			if(d._time * 1000 > 0) Sleep(d._time * 1000);
		}
		dlg->GetDlgItem(IDC_EDIT_LOOP)->GetWindowTextA(strSum);
		char sBuff[8] = {0};
		sprintf(sBuff,"%d",i+1);
		dlg->GetDlgItem(IDC_EDIT_LOOP2)->SetWindowTextA(sBuff);
	}
	dlg->SetRunningStatus(true);
	g_running = FALSE;
	return 0;
}
unsigned int __stdcall DownloadThread(void * param)
{
		PARAMETER *p = (PARAMETER*)param;
		int iLoopSum = p->arg[0];
		int iGohomeFlag = p->arg[1];
		CPressureCard *dlg = (CPressureCard *) p->dlg;
		int iLinePerLoop = dlg->m_data.size()*2 + iGohomeFlag;

		dlg->GetDlgItem(IDC_BUTTON_OPENEFILE)->SetWindowTextA("正在下载...");
		dlg->SetRunningStatus(false);
		
		((OperDobot2 *)g_OperDobot)->ClearCmd();
		g_OperDobot->m_csList.Lock();
		((OperDobot2 *)g_OperDobot)->SetCmdDownLoadStar(iLoopSum,iLinePerLoop);
		Sleep(1000);
		if(iGohomeFlag)
		{
			BufferE be = ((OperDobot2 *)g_OperDobot)->CreateGohomeCmd();
			int iLen = ((OperDobot2 *)g_OperDobot)->WriteComm(be.pData,be.iLen);
			delete [] be.pData;
			Sleep(1000);
		}
		for( vector<DataLi>::iterator it = dlg->m_data.begin(); it != dlg->m_data.end() ; ++it)
		{
			DataLi li = *it;
			CDobotPoint pt;
			pt.x = li.pt.x;
			pt.y = li.pt.y;
			pt.z = li.pt.z;
			BufferE be = ((OperDobot2 *)g_OperDobot)->CreateMove2AbsoluteCmd(pt);
			int iLen = ((OperDobot2 *)g_OperDobot)->WriteComm(be.pData,be.iLen);
			delete [] be.pData;
			Sleep(500);

			be = ((OperDobot2 *)g_OperDobot)->CreateTimeWaitCmd(li._time * 1000);
			((OperDobot2 *)g_OperDobot)->WriteComm(be.pData,be.iLen);
			delete [] be.pData;
			Sleep(500);
		}
		((OperDobot2 *)g_OperDobot)->SetCmdDownLoadStop();
		g_OperDobot->m_csList.Unlock();
		dlg->SetRunningStatus(true);
		dlg->GetDlgItem(IDC_BUTTON_OPENEFILE      )->SetWindowTextA("下载到设备");
		g_running = FALSE;
		AfxMessageBox("下载完成!");
		return 0;
}

void CPressureCard::OnNMRClickListCmd(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CMenu   oMenu;  
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    if(pNMListView->iItem == -1)
    {
		//点击在空白
		if(oMenu.LoadMenu(IDR_MENU2))
		{
			CMenu*   pPopup   =   oMenu.GetSubMenu(0);
			ASSERT(pPopup   !=   NULL);
			CPoint   oPoint;
			GetCursorPos(   &oPoint   );
			SetForegroundWindow();
			pPopup->TrackPopupMenu(TPM_LEFTALIGN   |   TPM_RIGHTBUTTON,oPoint.x,oPoint.y,this);
		}
	}
	else
	{
		// 右击在一个item上
		if(oMenu.LoadMenu(IDR_MENU1))
		{
			CMenu*   pPopup   =   oMenu.GetSubMenu(0);
			ASSERT(pPopup   !=   NULL);
			CPoint   oPoint;
			GetCursorPos(   &oPoint   );
			SetForegroundWindow();
			pPopup->TrackPopupMenu(TPM_LEFTALIGN   |   TPM_RIGHTBUTTON,oPoint.x,oPoint.y,this);
		}
	}
	*pResult = 0;
}


void CPressureCard::OnModify()
{
	// TODO: 在此添加命令处理程序代码
	POSITION pos;
	pos = m_list_cmd.GetFirstSelectedItemPosition();
	if (pos && m_list_cmd.GetSelectedCount()==1)
	{
		int nItem = m_list_cmd.GetNextSelectedItem(pos);
		//m_list_cmd.SetItemState(nItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		vector<DataLi>::iterator it = m_data.begin() + nItem;
		CModifyDlg dlg;
		dlg.m_fX = it->pt.x;
		dlg.m_fY = it->pt.y;
		dlg.m_fZ = it->pt.z;
		dlg.m_fJ0 = it->pt.jointAngle[0];
		dlg.m_fJ1 = it->pt.jointAngle[1];
		dlg.m_fJ2 = it->pt.jointAngle[2];
		dlg.m_fTime = it->_time;
		if(dlg.DoModal() == IDOK)
		{
			char sBuffer[24] = {0};
			sprintf(sBuffer,"%f",dlg.m_fX);
			m_list_cmd.SetItemText(nItem,1,sBuffer);
			it->pt.x = dlg.m_fX;

			memset(sBuffer,0,24);
			sprintf(sBuffer,"%f",dlg.m_fY);
			m_list_cmd.SetItemText(nItem,2,sBuffer);
			it->pt.y = dlg.m_fY;

			memset(sBuffer,0,24);
			sprintf(sBuffer,"%f",dlg.m_fZ);
			m_list_cmd.SetItemText(nItem,3,sBuffer);
			it->pt.z = dlg.m_fZ;

			memset(sBuffer,0,24);
			sprintf(sBuffer,"%f",dlg.m_fJ0);
			m_list_cmd.SetItemText(nItem,4,sBuffer);
			it->pt.jointAngle[0] = dlg.m_fJ0;

			memset(sBuffer,0,24);
			sprintf(sBuffer,"%f",dlg.m_fJ1);
			m_list_cmd.SetItemText(nItem,5,sBuffer);
			it->pt.jointAngle[1] = dlg.m_fJ1;

			memset(sBuffer,0,24);
			sprintf(sBuffer,"%f",dlg.m_fJ2);
			m_list_cmd.SetItemText(nItem,6,sBuffer);
			it->pt.jointAngle[2] = dlg.m_fJ2;

			memset(sBuffer,0,24);
			sprintf(sBuffer,"%0.1f",dlg.m_fTime);
			m_list_cmd.SetItemText(nItem,7,sBuffer);
			it->_time = dlg.m_fTime;
			//MessageBox("modify");
		}
	}
}


void CPressureCard::OnPaste()
{
	// TODO: 在此添加命令处理程序代码
	char sBuffer[24] = {0};
	for(vector<DataLi>::iterator it = m_copy_data.begin(); it != m_copy_data.end(); ++it)
	{
		Pose p = it->pt;
		int nCountRow = m_list_cmd.GetItemCount();
		int nItem = m_list_cmd.InsertItem (nCountRow,"");
		memset(sBuffer,0,24);sprintf(sBuffer,"%d",nCountRow+1);
		m_list_cmd.SetItemText(nItem,0,sBuffer);
		memset(sBuffer,0,24);sprintf(sBuffer,"%f",p.x);
		m_list_cmd.SetItemText(nItem,1,sBuffer);
		memset(sBuffer,0,24);sprintf(sBuffer,"%f",p.y);
		m_list_cmd.SetItemText(nItem,2,sBuffer);
		memset(sBuffer,0,24);sprintf(sBuffer,"%f",p.z);
		m_list_cmd.SetItemText(nItem,3,sBuffer);
		memset(sBuffer,0,24);sprintf(sBuffer,"%f",p.jointAngle[0]);
		m_list_cmd.SetItemText(nItem,4,sBuffer);
		memset(sBuffer,0,24);sprintf(sBuffer,"%f",p.jointAngle[1]);
		m_list_cmd.SetItemText(nItem,5,sBuffer);
		memset(sBuffer,0,24);sprintf(sBuffer,"%f",p.jointAngle[2]);
		m_list_cmd.SetItemText(nItem,6,sBuffer);
		memset(sBuffer,0,24);sprintf(sBuffer,"%0.1f",it->_time);
		m_list_cmd.SetItemText(nItem,7,sBuffer);
		m_data.push_back(*it);
	}
}


void CPressureCard::OnCopy()
{
	// TODO: 在此添加命令处理程序代码
	m_copy_data.clear();
	int nItem;
	POSITION pos;
	pos = m_list_cmd.GetFirstSelectedItemPosition();
	while (pos)
	{
		nItem = m_list_cmd.GetNextSelectedItem(pos);
		vector<DataLi>::iterator it = m_data.begin() + nItem;
		m_copy_data.push_back(*it);	
	}
}


void CPressureCard::OnDel()
{
	// TODO: 在此添加命令处理程序代码
	OnBnClickedButtonDel();
}


void CPressureCard::OnDelAll()
{
	// TODO: 在此添加命令处理程序代码
	m_list_cmd.DeleteAllItems();
	m_data.clear();
}
