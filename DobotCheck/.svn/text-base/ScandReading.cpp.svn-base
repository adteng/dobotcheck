// ScandReading.cpp : 实现文件
//
#include "stdafx.h"
#include "DobotCheck.h"
#include "ScandReading.h"
#include "afxdialogex.h"
#include "ACard.h"


#define		wdCollapseEnd			0
#define		wdLine					5
#define		wdWord9TableBehavior	1
#define		wdAutoFitFixed			0
#define		wdSortByName			0
#define		PI   3.1415926 

// CScandReading 对话框

IMPLEMENT_DYNAMIC(CScandReading, CDialogEx)

CScandReading::CScandReading(CWnd* pParent /*=NULL*/)
	: CDialogEx(CScandReading::IDD, pParent)
	, m_fXLen(0)
	, m_fYLen(0)
	, m_iX_Sum(0)
	, m_iY_Sum(0)
	, m_iLayer_Sum(0)
	, m_bFeeMode(FALSE)
	, m_iDevL(0)
	, m_iDevW(0)
	, m_iDevH(0)
	, m_iFeeValue(0)
	, m_fLayerSpaceLen(0)
{

}

CScandReading::~CScandReading()
{
}

void CScandReading::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_X_LENGTH, m_fXLen);
	DDX_Text(pDX, IDC_EDIT_Y_LENGTH, m_fYLen);
	DDX_Text(pDX, IDC_EDIT_X_POINT_SUM, m_iX_Sum);
	DDX_Text(pDX, IDC_EDIT_Y_POINT_SUM, m_iY_Sum);
	//  DDX_Text(pDX, IDC_EDIT_Z_LENGTH, m_ZLen);
	DDX_Text(pDX, IDC_EDIT_Z_POINT_SUM, m_iLayer_Sum);
	DDX_Control(pDX, IDC_COMBO_LAYER, m_comboLayer);
	DDV_MinMaxInt(pDX, m_iX_Sum, 1, 50);
	DDV_MinMaxInt(pDX, m_iY_Sum, 1, 50);
	DDX_Check(pDX, IDC_CHECK_FEE, m_bFeeMode);
	DDX_Text(pDX, IDC_EDIT_DEV_L, m_iDevL);
	DDX_Text(pDX, IDC_EDIT_DEV_W, m_iDevW);
	DDX_Text(pDX, IDC_EDIT_DEV_H, m_iDevH);
	DDX_Text(pDX, IDC_EDIT_FEE_VALUE, m_iFeeValue);
	DDX_Text(pDX, IDC_EDIT_LAYER_SPACE_LEN, m_fLayerSpaceLen);
	DDV_MinMaxFloat(pDX, m_fXLen, 1, 350);
	DDV_MinMaxFloat(pDX, m_fYLen, 1, 350);
	DDV_MinMaxFloat(pDX, m_fLayerSpaceLen, 1, 50);
	DDV_MinMaxInt(pDX, m_iLayer_Sum, 1, 8);
	DDV_MinMaxInt(pDX, m_iDevL, 1, 500);
	DDV_MinMaxInt(pDX, m_iDevW, 1, 500);
	DDV_MinMaxInt(pDX, m_iDevH, 1, 300);
	DDV_MinMaxInt(pDX, m_iFeeValue, 1, 500);
}

BEGIN_MESSAGE_MAP(CScandReading, CDialogEx)
	ON_BN_CLICKED(ID_BEGIN_SCAND, &CScandReading::OnBnClickedBeginScand)
	ON_BN_CLICKED(ID_CANCEL_SCAND, &CScandReading::OnBnClickedCancelScand)
	ON_BN_CLICKED(IDC_BUTTON_TEST_SCAND_REGION, &CScandReading::OnBnClickedButtonTestScandRegion)
	ON_BN_CLICKED(IDC_BUTTON_CREATE_REPORT, &CScandReading::OnBnClickedButtonCreateReport)
//	ON_WM_CREATE()
//ON_CBN_SELCHANGE(IDC_COMBO_LAYER, &CScandReading::OnCbnSelchangeComboLayer)
ON_WM_TIMER()
ON_BN_CLICKED(IDC_BUTTON_MARK_BEGIN_POINT, &CScandReading::OnBnClickedButtonMarkBeginPoint)
ON_CBN_SELENDOK(IDC_COMBO_LAYER, &CScandReading::OnCbnSelendokComboLayer)
ON_BN_CLICKED(IDC_BUTTON_SET_PARMETER, &CScandReading::OnBnClickedButtonSetParmeter)
ON_BN_CLICKED(IDC_BUTTON1, &CScandReading::OnBnClickedButton1)
ON_BN_CLICKED(IDC_BUTTON2, &CScandReading::OnBnClickedButton2)
END_MESSAGE_MAP()

// CScandReading 消息处理程序
//CWnd *g_wmd;
unsigned int __stdcall ScandReading(void * param);
unsigned int __stdcall ScandReading_noFee1(void * param);
unsigned int __stdcall TestRegionThread(void * param);


CScandReading *g_dlg;
CWnd *g_wmd;
extern OperDobot *g_OperDobot;
extern CACard *g_pCardOP;

void CScandReading::UpdateParameter()
{
	UpdateData(TRUE);
	m_scand.m_devInfo.m_iDevH = m_iDevH;
	m_scand.m_devInfo.m_iDevL = m_iDevL;
	m_scand.m_devInfo.m_iDevW = m_iDevW;
	m_scand.m_devInfo.m_iFeeValue = m_iFeeValue;
	m_scand.m_sysInfo.m_fLayerSpaceLen = m_fLayerSpaceLen;
	m_scand.m_sysInfo.m_fXLen = m_fXLen;
	m_scand.m_sysInfo.m_fYLen = m_fYLen;
	m_scand.m_sysInfo.m_iLayer_Sum = m_iLayer_Sum;
	m_scand.m_sysInfo.m_iX_Sum = m_iX_Sum;
	m_scand.m_sysInfo.m_iY_Sum = m_iY_Sum;
}


void CScandReading::OnBnClickedBeginScand()
{
	// TODO: 在此添加控件通知处理程序代码
	//UpdateData(TRUE);
	UpdateParameter();
	if(m_MarkLocationDlg->m_iFinishSum < 6)
	{
		MessageBox("请先标记位置点");
		return;
	}
	m_comboLayer.ResetContent();
	for(int i = 0;i < m_iLayer_Sum; i++)
	{
		CString str;
		str.Format("%d",i+1);
		m_comboLayer.AddString(str);
	}
	m_comboLayer.SetCurSel(0);
	UpdateData(FALSE);

	m_scand.m_FirstScandPoint = g_OperDobot->m_WorkCenterPoint;
	m_scand.m_FirstScandPoint.x -= m_fXLen/2.0;
	m_scand.m_FirstScandPoint.y -= m_fYLen/2.0;
	m_scand.m_FirstScandPoint.z += m_iDevH;
	m_bRunning = TRUE;
	unsigned int nThreadAddr;
	HANDLE	hSession;
	//if(m_bFeeMode)
		hSession = (HANDLE)_beginthreadex(0, 0, ScandReading, this, 0, &nThreadAddr);
	//else
		//hSession = (HANDLE)_beginthreadex(0, 0, ScandReading_noFee, this, 0, &nThreadAddr);
	if(hSession) CloseHandle( hSession );
}

void CScandReading::OnBnClickedCancelScand()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bRunning = FALSE;
	m_scand.m_bRunning = FALSE;
}

void CScandReading::OnBnClickedButtonTestScandRegion()
{
	// TODO: 在此添加控件通知处理程序代码
	//UpdateData(TRUE);
	UpdateParameter();
	if(m_MarkLocationDlg->m_iFinishSum < 6)
	{
		MessageBox("请先标记位置点");
		return;
	}
	unsigned int nThreadAddr;
	HANDLE	hSession = (HANDLE)_beginthreadex(0, 0, TestRegionThread, this, 0, &nThreadAddr);
	if(hSession) CloseHandle( hSession );
}

void CScandReading::OnBnClickedButtonCreateReport()
{
	// TODO: 在此添加控件通知处理程序代码
	//UpdateData(TRUE);
	UpdateParameter();
	/*
	//DrawPointWithStatus();
	if(CoInitialize(NULL)   !=   S_OK)
	{    
		AfxMessageBox( " 初始化COM支持库失败! " );    
        return  ;
	}
	if(!m_Application.CreateDispatch("Word.Application"))
	{
		MessageBox("创建Word服务失败，请确保已经安装了Word！");
		return;
	}

	m_Application.put_Visible(TRUE);
	CComVariant	varDefOption((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	m_Documents.AttachDispatch(m_Application.get_Documents());
	m_Documents.Add(&varDefOption, &varDefOption, &varDefOption, &varDefOption);

	m_Selection.AttachDispatch(m_Application.get_Selection());
	m_ActiveDocument.AttachDispatch(m_Application.get_ActiveDocument());
	m_Tables.AttachDispatch(m_ActiveDocument.get_Tables());

	//add text
	m_Selection.TypeText("读卡器盲区扫描测试报告");
	m_Selection.TypeParagraph();
	//m_Selection.TypeText("vs 简单控制 word 程序！");
	//m_Selection.TypeParagraph();

	//add table
	int nRows = 2;
	int nCols = 3;

	CComVariant	varDefTableBehavior(wdWord9TableBehavior);
	CComVariant varAutoFit(wdAutoFitFixed);

	CComVariant varWhat(-1);
	CComVariant varTemp(0);

	m_Table = m_Tables.Add(m_Selection.get_Range(), nRows, nCols, &varDefTableBehavior, &varAutoFit);

	CComVariant varStyle("网格型");
	if(m_Table.get_Style().bstrVal != _bstr_t("网格型"))
		m_Table.put_Style(&varStyle);

	m_Table.put_ApplyStyleFirstColumn(TRUE);
	m_Table.put_ApplyStyleHeadingRows(TRUE);
	m_Table.put_ApplyStyleLastColumn(TRUE);
	m_Table.put_ApplyStyleLastRow(TRUE);

	m_Selection.TypeText(_T("123"));
	m_Selection.MoveRight(COleVariant((short)1), COleVariant((short)1), COleVariant((short)0));
	m_Selection.TypeText(_T("456"));
	m_Selection.MoveRight(COleVariant((short)1), COleVariant((short)1), COleVariant((short)0));
	m_Selection.TypeText(_T("789"));

	//Save
	char szSavePath[MAX_PATH] = {'\0'};
	GetCurrentDirectoryA(MAX_PATH, szSavePath);
	strcat_s(szSavePath, MAX_PATH, "\\扫描报告.doc");

	CComVariant varSavePath(szSavePath);
	CComVariant varEmpty("");
	CComVariant varZero(0);
	CComVariant varTrue(TRUE);
	CComVariant varFalse(FALSE);
	m_ActiveDocument.SaveAs(&varSavePath, &varZero, &varFalse, &varEmpty, &varTrue, &varEmpty, &varFalse, &varFalse, 
		&varFalse, &varFalse, &varFalse, &varFalse, &varFalse, &varFalse, &varFalse, &varFalse);

	//clear
//	CComVariant	varDefOption((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	m_Selection.ReleaseDispatch();
	m_Documents.ReleaseDispatch();
	m_ActiveDocument.ReleaseDispatch();
	m_LineShapes.ReleaseDispatch();
	m_Tables.ReleaseDispatch();
	m_Table.ReleaseDispatch();

	m_Application.Quit(&varDefOption, &varDefOption, &varDefOption);
	m_Application.ReleaseDispatch();

	CoUninitialize();  
	*/
	ofstream m_ofs;
	m_ofs.open ("data/report.txt",ios::out);
	if(!m_ofs) 
	{
		MessageBox("打开文件<data/report.txt>失败");
		return ;
	}
	//char sBuffer[128] = {0};
		CTime time = CTime::GetCurrentTime();
		int nYear = time.GetYear();
		int nMonth = time.GetMonth();
		int nDay = time.GetDay();      ///日
		int nHour = time.GetHour();      ///小时
		int nMinute = time.GetMinute();   ///分钟
		int nSecond = time.GetSecond(); 
		char sTime[48] = {0};
		sprintf(sTime,"%d年%d月%d日 %02d:%02d:%02d",nYear,nMonth,nDay,nHour,nMinute,nSecond);
	m_ofs << "\t\t\t\t盲区测试报告" << endl;
	m_ofs << endl;
	//m_ofs <<"测试报告生成时间:" << sTime  << endl;
	m_ofs <<"测试开始时间:" << this->m_strBeginTestTime.GetBuffer(0)  << endl;
	m_ofs <<"测试结束时间:" << this->m_strEndTestTime.GetBuffer(0)  << endl;
	m_ofs << endl;
	m_ofs <<"卡片信息:" << endl;
	for(int i=0;i<4;i++)
		m_ofs <<"\t卡号：" << g_OperDobot->m_CardInfo[i].szCardNo << "\t位置：" << i+1 << "\t测试前余额（分）："<<  g_OperDobot->m_CardInfo[i].lInitFee  << endl;
	m_ofs << endl;
	m_ofs << "被测试终端信息：" << endl;
	m_ofs << "\t扣费金额（每次）：\t" << m_iFeeValue << "分" << endl;
	m_ofs << "\t天线长（毫米）：\t" << m_iDevL << endl;
	m_ofs << "\t天线宽（毫米）：\t" << m_iDevW << endl;
	m_ofs << "\t终端高度（毫米）：\t" << m_iDevH << endl;
	m_ofs << endl;
	m_ofs << "测试范围参数：" << endl;
	m_ofs << "\t测试范围，横向（毫米）：\t" << m_fYLen << endl;
	m_ofs << "\t测试范围，纵向（毫米）：\t" << m_fXLen << endl;
	m_ofs << "\t测试范围，高度（毫米）：\t" << m_fLayerSpaceLen*m_iLayer_Sum << endl;
	m_ofs << "\t每层测试点数，横向：\t" << m_iY_Sum << endl;
	m_ofs << "\t每层测试点数，纵向：\t" << m_iX_Sum  << endl;
	m_ofs << "\t每平面共点数：\t" << m_iY_Sum*m_iX_Sum << endl;
	m_ofs << "\t每点横向距离（毫米）：\t" << m_fYLen/m_iY_Sum << endl;
	m_ofs << "\t每点纵向距离（毫米）：\t" << m_fXLen/m_iX_Sum << endl;
	m_ofs << "\t每点高度距离（毫米）：\t" << m_fLayerSpaceLen << endl;
	m_ofs << "\t测试层数：\t" << m_iLayer_Sum  << endl;
	m_ofs << endl;
	m_ofs << "测试结果记录：" << endl;
	int iTotalSucceedSum = 0;
	for(int z=0;z<m_iLayer_Sum;z++)
	{
		int iSucceedSum = GetSucceedPointSum(z);iTotalSucceedSum+=iSucceedSum;
		m_ofs << "\t第" << z+1 << "层共" <<  m_iY_Sum*m_iX_Sum << "个点，其中" << iSucceedSum << "个点成功扣费，"<< m_iY_Sum*m_iX_Sum-iSucceedSum << "个点扣费失败。" <<endl;
	}
	m_ofs << "\t总测试点数：" << m_iY_Sum*m_iX_Sum*m_iLayer_Sum << "，其中成功点数：" << iTotalSucceedSum << "，失败点数：" << m_iY_Sum*m_iX_Sum*m_iLayer_Sum-iTotalSucceedSum << endl;
	m_ofs << endl;
	m_ofs << "测试前后的卡片信息" << endl;
	for(int i=0;i<4;i++)
		m_ofs <<"\t卡号：" << g_OperDobot->m_CardInfo[i].szCardNo << "\t位置：" << i+1 << "\t测试前余额（分）："<<  g_OperDobot->m_CardInfo[i].lInitFee  << "\t测试后余额（分）：" << g_OperDobot->m_CardInfo[i].lEndWFee << endl;
	m_ofs << endl;
	m_ofs << "每层每点直观对照表（O表示成功，X表示失败）" << endl;
	for(int z = 0; z < m_iLayer_Sum; z++)
	{
		m_ofs << "\t第" << z+1 << "层：" << endl;
		for(int x = 0;x < m_iX_Sum; x++)
		{
			for(int y = 0;y < m_iY_Sum; y++)
			{
				if(m_scand.m_iWorkingPoint[x][y][z].iPointStatus==2)
					m_ofs << "\tO\t";
				else
					m_ofs << "\tX\t";
			}
			m_ofs << endl;
		}
		m_ofs << endl;
	}
	m_ofs.flush();
	m_ofs.close();
	//ShellExecute(NULL,"open","data\\report.txt",NULL,NULL,SW_SHOW);
	WinExec("notepad data\\report.txt",SW_SHOWNORMAL);
	SetDataEnable(TRUE);
}

void LayerChange(int iLayer)
{
	g_dlg->m_comboLayer.SetCurSel(iLayer);
}
void ViewChange(char *pStr)
{
	g_dlg->GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(pStr);
}

BOOL CScandReading::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
	g_dlg = this;
	g_wmd = GetDlgItem(IDC_STATIC_RESULT_VIEW);

	m_fXLen = 80;
	m_fYLen = 80;
	m_iX_Sum = 6;
	m_iY_Sum = 6;
	m_iLayer_Sum = 3;
	m_fLayerSpaceLen = 5.0;//m_ZLen = 18;
	m_comboLayer.ResetContent();
	for(int i = 0;i < m_iLayer_Sum; i++)
	{
		CString str;
		str.Format("%d",i+1);
		m_comboLayer.AddString(str);
	}
	m_comboLayer.SetCurSel(0);
	SetTimer(1,300,NULL);

	m_MarkLocationDlg = new CMarkLocationDlg();
	m_MarkLocationDlg->Create(IDD_DIALOG_M5);
	m_MarkLocationDlg->m_iFinishSum = 0;
	for(int i=0;i<4;i++)
	{
	g_OperDobot->m_CardInfo[i].iFailureSum = 0;
	g_OperDobot->m_CardInfo[i].iSucceedSum = 0;
	g_OperDobot->m_CardInfo[i].lBeginWFee = 0;
	g_OperDobot->m_CardInfo[i].lEndWFee = 0;
	g_OperDobot->m_CardInfo[i].lInitFee = 0;
	memset(g_OperDobot->m_CardInfo[i].szCardNo,0,24);
	}
	
	m_iDevL = 70;
	m_iDevW = 70;
	m_iDevH = 40;
	m_iFeeValue = 50;
	
	m_bFeeMode = TRUE;
	GetDlgItem(IDC_EDIT_STATUS)->SetWindowText("未开始");

	m_MarkLocationDlg->OnBnClickedButtonPointLoad();
	m_scand.m_pCardOP = g_pCardOP;
	m_scand.m_pOperDobot = g_OperDobot;
	m_scand.pLayerChange = LayerChange;
	m_scand.pViewChange = ViewChange;
	UpdateData(FALSE);
	UpdateParameter();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CScandReading::DrawIcon(int iFlag)
{
	CBrush *brush1; 
	CWnd *wmd;
	if(iFlag == 1)
	{
		wmd = GetDlgItem( IDC_STATIC_SUCCEED_PIC);
		brush1 = new CBrush(RGB(0, 255, 0));
	}
	else
	{
		wmd = GetDlgItem( IDC_STATIC_FAILURE_PIC);
		brush1 = new CBrush(RGB(255, 0, 0));
	}
	CRect rectPar;
	wmd->GetClientRect(rectPar);
	CDC *pDC = wmd->GetDC();
	//CPen *pen = new CPen(PS_SOLID, 0, RGB(255, 0, 0));
	//CPen *pOldPen = pDC->SelectObject(pen);
	pDC->SelectObject(brush1);
	pDC-> Ellipse (rectPar);
	//pDC->SelectObject(pOldPen);
	//delete pen;
	delete brush1;
	ReleaseDC(pDC);
}
void CScandReading::DrawBackGround()
{
	CDC* pDC = g_wmd->GetDC();
	ShowBitmap(pDC,"data/bg.bmp") ;
	ReleaseDC(pDC);
}

void CScandReading::ShowBitmap(CDC *pDC, CString BmpName)  
{  
    HBITMAP hBitmap;  
    hBitmap = (HBITMAP) LoadImage(NULL,BmpName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_DEFAULTSIZE|LR_CREATEDIBSECTION);  
	CBitmap bitmap;
    if( bitmap.m_hObject )  
    {  
        bitmap.Detach();           //切断CWnd和窗口联系  
    }  
    bitmap.Attach(hBitmap);      //将句柄HBITMAP m_hBitmap与CBitmap m_bitmap关联  
	//bitmap.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
     
    CRect rect;  
    g_wmd->GetClientRect(&rect);  
   
    //定义并创建一个内存设备环境DC  
    CDC dcBmp;  
    if( !dcBmp.CreateCompatibleDC(pDC) )   //创建兼容性的DC  
        return;  

   
   BITMAP bm;                          //临时bmp图片变量  
   bitmap.GetBitmap(&bm);            //将图片载入位图中  
   CBitmap *pbmpOld = dcBmp.SelectObject(&bitmap);         //将位图选入临时内存设备环境  
     
    //图片显示调用函数stretchBlt  
   pDC->StretchBlt(0,0,rect.Width(),rect.Height(),&dcBmp,0,0,rect.Width(),rect.Height(),SRCCOPY);  
      
    /*******************************************************************************/  
    /* BOOL StretchBlt(int x,int y,int nWidth,int nHeight,CDC* pSrcDC,  
    /*                 int xSrc,int ySrc,int nSrcWidth,int nSrcHeight,DWORD dwRop ); 
    /* 1.参数x、y位图目标矩形左上角x、y的坐标值       
    /* 2.nWidth、nHeigth位图目标矩形的逻辑宽度和高度       
    /* 3.pSrcDC表示源设备CDC指针                           
    /* 4.xSrc、ySrc表示位图源矩形的左上角的x、y逻辑坐标值  
    /* 5.dwRop表示显示位图的光栅操作方式 SRCCOPY用于直接将位图复制到目标环境中             
    /*******************************************************************************/  
      
    dcBmp.SelectObject(pbmpOld);           //恢复临时DC的位图  
    DeleteObject(&bitmap);               //删除内存中的位图  
    dcBmp.DeleteDC();                      //删除CreateCompatibleDC得到的图片DC  
}  


void CScandReading::DrawPointWithStatus_bak()
{
	DrawIcon(0);
	DrawIcon(1);
	UpdateData(TRUE);
	if(m_iY_Sum < 1 || m_iX_Sum < 1 || m_fXLen < 1 || m_fYLen < 1 || m_fLayerSpaceLen < 1 || m_iLayer_Sum < 1 || m_iY_Sum > 50 || m_iX_Sum > 50 || m_iLayer_Sum > 8) return;

	CRect rectPar;
	g_wmd->GetClientRect(rectPar);
	int iHSPLen = rectPar.Height() / m_iX_Sum;
	int iWSPLen = rectPar.Width()  / m_iY_Sum;

	DrawBackGround();

	CDC* pDC = g_wmd->GetDC();
	/*
	pDC->GetBkColor();
	CBrush *bgbr = new CBrush(pDC->GetBkColor());
	pDC->FillRect(rectPar,bgbr);
	delete bgbr;
	*/
	CString strWStr,strHStr;
	strWStr.Format("%0.0f",m_fYLen);
	strHStr.Format("%0.0f",m_fXLen);

	CPen *penXY = new CPen(PS_SOLID, 0, RGB(255, 255, 255));
	CPen *pOldPen = pDC->SelectObject(penXY);
	/*
	 //生成X轴
	pDC->MoveTo(10,10);
	pDC->LineTo(rectPar.Width()/2,10);
	pDC->TextOutA(rectPar.Width()/2,2,strWStr);
	pDC->MoveTo(rectPar.Width()/2 + 25,10);
	pDC->LineTo(rectPar.Width()-10,10);
	pDC->LineTo(rectPar.Width()-10-8,10-8);//箭头
	pDC->MoveTo(rectPar.Width()-10,10);//箭头
	pDC->LineTo(rectPar.Width()-10-8,10+8);//箭头
	 //生成Y轴
	pDC->MoveTo(10,10);
	pDC->LineTo(10,rectPar.Height()/2);
	pDC->TextOutA(-2,rectPar.Height()/2,strHStr);
	pDC->MoveTo(10,rectPar.Height()/2+18);
	pDC->LineTo(10,rectPar.Height()-10);
	pDC->LineTo(10-8,rectPar.Height()-10-8);//箭头
	pDC->MoveTo(10,rectPar.Height()-10);//箭头
	pDC->LineTo(10+8,rectPar.Height()-10-8);//箭头
	*/
	pDC->MoveTo(10,rectPar.Height()/2.0);
	pDC->LineTo(rectPar.Width(),rectPar.Height()/2.0);
	pDC->MoveTo(rectPar.Width()/2.0,10);
	pDC->LineTo(rectPar.Width()/2.0,rectPar.Height());
	delete penXY;
	//开始画外围框
	CPen *penBorder = new CPen(PS_SOLID, 3, RGB(255, 0, 0));
	pOldPen = pDC->SelectObject(penBorder);
	pDC->MoveTo(0,0);
	pDC->LineTo(rectPar.Width()/2.0,0);
	pDC->TextOutA(rectPar.Width()/2.0,-10,"120");
	pDC->MoveTo(rectPar.Width()/2.0 + 25,0);
	pDC->LineTo(rectPar.Width(),0);
	pDC->LineTo(rectPar.Width(),rectPar.Height());
	pDC->LineTo(0,rectPar.Height());
	pDC->LineTo(0,rectPar.Height()/2.0+10);
	pDC->TextOutA(-10,rectPar.Height()/2.0,"120");
	pDC->MoveTo(0,rectPar.Height()/2.0-2);
	pDC->LineTo(0,0);
	pDC->SelectObject(pOldPen);
	delete penBorder;

	CPen *pen0 = new CPen(PS_SOLID, 5, RGB(255, 255, 255));
	pOldPen = pDC->SelectObject(pen0);
	//右
	pDC->MoveTo(rectPar.Width()/2.0 + 25*cos(PI/6.0) ,rectPar.Height()/2.0 +  25 *sin(PI/6.0));
	pDC->AngleArc(rectPar.Width()/2.0,rectPar.Height()/2.0,25,-30,60);
	pDC->MoveTo(rectPar.Width()/2.0 + 50*cos(PI/6.0) ,rectPar.Height()/2.0 +  50 *sin(PI/6.0));
	pDC->AngleArc(rectPar.Width()/2.0,rectPar.Height()/2.0,50,-30,60);
	pDC->MoveTo(rectPar.Width()/2.0 + 100*cos(PI/6.0) ,rectPar.Height()/2.0 +  100 *sin(PI/6.0));
	pDC->AngleArc(rectPar.Width()/2.0,rectPar.Height()/2.0,100,-30,60);
	pDC->MoveTo(rectPar.Width()/2.0 + 150*cos(PI/6.0) ,rectPar.Height()/2.0 +  150 *sin(PI/6.0));
	pDC->AngleArc(rectPar.Width()/2.0,rectPar.Height()/2.0,150,-30,60);
	//左
	pDC->MoveTo(rectPar.Width()/2.0 - 25*cos(PI/6.0) ,rectPar.Height()/2.0 -  25 *sin(PI/6.0));
	pDC->AngleArc(rectPar.Width()/2.0,rectPar.Height()/2.0,25,150,60);
	pDC->MoveTo(rectPar.Width()/2.0 - 50*cos(PI/6.0) ,rectPar.Height()/2.0 -  50 *sin(PI/6.0));
	pDC->AngleArc(rectPar.Width()/2.0,rectPar.Height()/2.0,50,150,60);
	pDC->MoveTo(rectPar.Width()/2.0 - 100*cos(PI/6.0) ,rectPar.Height()/2.0 -  100 *sin(PI/6.0));
	pDC->AngleArc(rectPar.Width()/2.0,rectPar.Height()/2.0,100,150,60);
	pDC->MoveTo(rectPar.Width()/2.0 - 150*cos(PI/6.0) ,rectPar.Height()/2.0 -  150 *sin(PI/6.0));
	pDC->AngleArc(rectPar.Width()/2.0,rectPar.Height()/2.0,150,150,60);
	pDC->Ellipse(rectPar.Width()/2.0-5,rectPar.Height()/2.0-5,rectPar.Width()/2.0+5,rectPar.Height()/2.0+5);
	pDC->SelectObject(pOldPen);
	delete pen0;

	int z = m_comboLayer.GetCurSel();
	for(int w=0;w<m_iY_Sum;w++)
		for(int h=0;h<m_iX_Sum;h++)
		{
			CPen *pen = new CPen(PS_SOLID, 0, RGB(255, 0, 0));
			pOldPen = pDC->SelectObject(pen);
			
			CBrush *brush1 = new CBrush(RGB(255, 0, 0));
			CBrush *brush2 = new CBrush(RGB(0, 255, 0));
			//CBrush *brush3 = new CBrush(RGB(0, 0, 255));
			
			if(m_scand.m_iWorkingPoint[h][w][z].iPointStatus == 0)//未处理的点
			{
				pDC->SelectStockObject(NULL_BRUSH);
			}
			else if(m_scand.m_iWorkingPoint[h][w][z].iPointStatus == 1)//正在处理的点
			{
				static bool bFlashLight = true;
				if(bFlashLight)
					pDC->SelectObject(brush1);
				else
					pDC->SelectObject(brush2);
				bFlashLight = !bFlashLight;
			}
			else if(m_scand.m_iWorkingPoint[h][w][z].iPointStatus == 2)//处理成功的点
			{
				pDC->SelectObject(brush2);
			}
			else if(m_scand.m_iWorkingPoint[h][w][z].iPointStatus == 3)//处理失败的点
			{
				pDC->SelectObject(brush1);
			}

			float fDiameter; 
			if(iWSPLen > iHSPLen)
			{
				fDiameter = iHSPLen/2.0;
			}
			else 
			{
				fDiameter = iWSPLen/2.0;
			}
			int iPointW = w * iWSPLen + iWSPLen/2.0;//中心点
			int iPointH = h * iHSPLen + iHSPLen/2.0;//中心点

			//CRect rect1( w * iWSPLen,h * iHSPLen ,(w+1) * iWSPLen,(h + 1) * iHSPLen);
			//pDC->Rectangle(rect1);

			CRect rect(iPointW - fDiameter/2.0,iPointH - fDiameter/2.0,iPointW + fDiameter/2.0,iPointH + fDiameter/2.0);
			pDC-> Ellipse (rect);

			pDC->SelectObject(pOldPen);
			delete brush1;
			delete brush2;
			//delete brush3;
			delete pen;
		}
	ReleaseDC(pDC);
}
void CScandReading::DrawPointWithStatus()
{
	DrawIcon(0);
	DrawIcon(1);
	//UpdateData(TRUE);
	UpdateParameter();
	if(m_iY_Sum < 1 || m_iX_Sum < 1 || m_fXLen < 1 || m_fYLen < 1 || m_fLayerSpaceLen < 1 || m_iLayer_Sum < 1 || m_iY_Sum > 50 || m_iX_Sum > 50 || m_iLayer_Sum > 8) return;

	CRect rectPar;
	g_wmd->GetClientRect(rectPar);
	int iMaxWidth = 120;
	int iMaxHeight = 120;
	//int iHSPLen = rectPar.Height() / m_iX_Sum;
	//int iWSPLen = rectPar.Width()  / m_iY_Sum;
	DrawBackGround();
	CDC* pDC = g_wmd->GetDC();
	CString strWStr,strHStr;
	strWStr.Format("%0.0f",m_fYLen);
	strHStr.Format("%0.0f",m_fXLen);

	CPen *penXY = new CPen(PS_SOLID, 0, RGB(255, 255, 255));
	CPen *pOldPen = pDC->SelectObject(penXY);
	pDC->MoveTo(10,rectPar.Height()/2.0);
	pDC->LineTo(rectPar.Width(),rectPar.Height()/2.0);
	pDC->MoveTo(rectPar.Width()/2.0,10);
	pDC->LineTo(rectPar.Width()/2.0,rectPar.Height());
	pDC->SelectObject(pOldPen);
	
	//开始画外围框
	CPen *penBorder = new CPen(PS_SOLID, 3, RGB(255, 0, 0));
	pOldPen = pDC->SelectObject(penBorder);
	pDC->MoveTo(0,0);
	pDC->LineTo(rectPar.Width()/2.0,0);
	pDC->TextOutA(rectPar.Width()/2.0,-10,"120");
	pDC->MoveTo(rectPar.Width()/2.0 + 25,0);
	pDC->LineTo(rectPar.Width(),0);
	pDC->LineTo(rectPar.Width(),rectPar.Height());
	pDC->LineTo(0,rectPar.Height());
	pDC->LineTo(0,rectPar.Height()/2.0+10);
	pDC->TextOutA(-10,rectPar.Height()/2.0,"120");
	pDC->MoveTo(0,rectPar.Height()/2.0-2);
	pDC->LineTo(0,0);
	pDC->SelectObject(pOldPen);
	
	//开始画工作范围框
	float fWLen = m_fYLen/iMaxWidth * rectPar.Width();//工作区域的长
	float fHLen = m_fXLen/iMaxHeight * rectPar.Height();//工作区域的宽
	float fWSpace = fWLen/(m_iY_Sum-1);
	float fHSpace = fHLen/(m_iX_Sum-1);
	POINT center;
	center.x = rectPar.Width()/2.0;center.y = rectPar.Height()/2.0;
	CPen *penWorkRegion = new CPen(PS_DASH, 0, RGB(0, 0, 255));
	pOldPen = pDC->SelectObject(penWorkRegion);
	pDC->SelectStockObject(NULL_BRUSH);
	pDC->Rectangle(center.x-fWLen/2.0,center.y-fHLen/2.0,center.x+fWLen/2.0,center.y+fHLen/2.0);
	pDC->SelectObject(pOldPen);
	/*
	CPen *pen0 = new CPen(PS_SOLID, 5, RGB(255, 255, 255));
	pOldPen = pDC->SelectObject(pen0);
	//右
	pDC->MoveTo(rectPar.Width()/2.0 + 25*cos(PI/6.0) ,rectPar.Height()/2.0 +  25 *sin(PI/6.0));
	pDC->AngleArc(rectPar.Width()/2.0,rectPar.Height()/2.0,25,-30,60);
	pDC->MoveTo(rectPar.Width()/2.0 + 50*cos(PI/6.0) ,rectPar.Height()/2.0 +  50 *sin(PI/6.0));
	pDC->AngleArc(rectPar.Width()/2.0,rectPar.Height()/2.0,50,-30,60);
	pDC->MoveTo(rectPar.Width()/2.0 + 100*cos(PI/6.0) ,rectPar.Height()/2.0 +  100 *sin(PI/6.0));
	pDC->AngleArc(rectPar.Width()/2.0,rectPar.Height()/2.0,100,-30,60);
	pDC->MoveTo(rectPar.Width()/2.0 + 150*cos(PI/6.0) ,rectPar.Height()/2.0 +  150 *sin(PI/6.0));
	pDC->AngleArc(rectPar.Width()/2.0,rectPar.Height()/2.0,150,-30,60);
	//左
	pDC->MoveTo(rectPar.Width()/2.0 - 25*cos(PI/6.0) ,rectPar.Height()/2.0 -  25 *sin(PI/6.0));
	pDC->AngleArc(rectPar.Width()/2.0,rectPar.Height()/2.0,25,150,60);
	pDC->MoveTo(rectPar.Width()/2.0 - 50*cos(PI/6.0) ,rectPar.Height()/2.0 -  50 *sin(PI/6.0));
	pDC->AngleArc(rectPar.Width()/2.0,rectPar.Height()/2.0,50,150,60);
	pDC->MoveTo(rectPar.Width()/2.0 - 100*cos(PI/6.0) ,rectPar.Height()/2.0 -  100 *sin(PI/6.0));
	pDC->AngleArc(rectPar.Width()/2.0,rectPar.Height()/2.0,100,150,60);
	pDC->MoveTo(rectPar.Width()/2.0 - 150*cos(PI/6.0) ,rectPar.Height()/2.0 -  150 *sin(PI/6.0));
	pDC->AngleArc(rectPar.Width()/2.0,rectPar.Height()/2.0,150,150,60);
	pDC->Ellipse(rectPar.Width()/2.0-5,rectPar.Height()/2.0-5,rectPar.Width()/2.0+5,rectPar.Height()/2.0+5);
	pDC->SelectObject(pOldPen);
	delete pen0;
	*/
	delete penBorder;
	delete penXY;
	delete penWorkRegion;

	int z = m_comboLayer.GetCurSel();
	for(int w=0;w<m_iY_Sum;w++)
		for(int h=0;h<m_iX_Sum;h++)
		{
			CPen *pen = new CPen(PS_SOLID, 0, RGB(255, 0, 0));
			pOldPen = pDC->SelectObject(pen);
			
			CBrush *brushRed = new CBrush(RGB(255, 0, 0));
			CBrush *brushGreen = new CBrush(RGB(0, 255, 0));
			
			if(m_scand.m_iWorkingPoint[h][w][z].iPointStatus == 0)//未处理的点
			{
				pDC->SelectStockObject(NULL_BRUSH);
			}
			else if(m_scand.m_iWorkingPoint[h][w][z].iPointStatus == 1)//正在处理的点
			{
				static bool bFlashLight = true;
				if(bFlashLight)
					pDC->SelectObject(brushRed);
				else
					pDC->SelectObject(brushGreen);
				bFlashLight = !bFlashLight;
			}
			else if(m_scand.m_iWorkingPoint[h][w][z].iPointStatus == 2)//处理成功的点
			{
				pDC->SelectObject(brushGreen);
			}
			else if(m_scand.m_iWorkingPoint[h][w][z].iPointStatus == 3)//处理失败的点
			{
				pDC->SelectObject(brushRed);
			}

			float fDiameter; 
			if(fWSpace > fHSpace) 
			{
				fDiameter = fHSpace*2/3.0;
			}
			else 
			{
				fDiameter = fWSpace*2/3.0;
			}
			if(fDiameter > 50) fDiameter = 50;
			int iPointW = center.x-fWLen/2.0 + w * fWSpace;  //w * iWSPLen + iWSPLen/2.0;//中心点
			int iPointH = center.y-fHLen/2.0 + h * fHSpace;  //h * iHSPLen + iHSPLen/2.0;//中心点

			//CRect rect1( w * iWSPLen,h * iHSPLen ,(w+1) * iWSPLen,(h + 1) * iHSPLen);
			//pDC->Rectangle(rect1);

			CRect rect(iPointW - fDiameter/2.0,iPointH - fDiameter/2.0,iPointW + fDiameter/2.0,iPointH + fDiameter/2.0);
			pDC-> Ellipse (rect);

			pDC->SelectObject(pOldPen);
			delete brushRed;
			delete brushGreen;
			//delete brush3;
			delete pen;
		}
	ReleaseDC(pDC);
}
int CScandReading::GetSucceedPointSum(int z)
{
	int iSucceedSum = 0;
	for(int x = 0; x < m_iX_Sum; x++)
		for(int y = 0; y < m_iY_Sum; y++)
			if(m_scand.m_iWorkingPoint[x][y][z].iPointStatus == 2) iSucceedSum++;
	return iSucceedSum;
}


void CScandReading::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDEvent == 1)
		DrawPointWithStatus();
	CDialogEx::OnTimer(nIDEvent);
}

void CScandReading::OnBnClickedButtonMarkBeginPoint()
{
	// TODO: 在此添加控件通知处理程序代码
	//UpdateData(TRUE);
	UpdateParameter();
	m_MarkLocationDlg->ShowWindow(SW_SHOWNORMAL);
}
void CScandReading::SetDataEnable(BOOL bFlag)
{
	GetDlgItem(ID_BEGIN_SCAND)->EnableWindow(bFlag);
	GetDlgItem(IDC_BUTTON1)->EnableWindow(bFlag);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(bFlag);
	GetDlgItem(IDC_BUTTON_SET_PARMETER)->EnableWindow(bFlag);
	GetDlgItem(IDC_BUTTON_MARK_BEGIN_POINT)->EnableWindow(bFlag);
	GetDlgItem(IDC_BUTTON_TEST_SCAND_REGION)->EnableWindow(bFlag);
	GetDlgItem(IDC_BUTTON_CREATE_REPORT)->EnableWindow(bFlag);
	GetDlgItem(IDC_EDIT_X_LENGTH)->EnableWindow(bFlag);
	GetDlgItem(IDC_EDIT_Y_LENGTH)->EnableWindow(bFlag);
	GetDlgItem(IDC_EDIT_X_POINT_SUM)->EnableWindow(bFlag);
	GetDlgItem(IDC_EDIT_Y_POINT_SUM)->EnableWindow(bFlag);
	GetDlgItem(IDC_EDIT_LAYER_SPACE_LEN)->EnableWindow(bFlag);
	GetDlgItem(IDC_EDIT_Z_POINT_SUM)->EnableWindow(bFlag);
	GetDlgItem(IDC_EDIT_DEV_L)->EnableWindow(bFlag);
	GetDlgItem(IDC_EDIT_DEV_W)->EnableWindow(bFlag);
	GetDlgItem(IDC_EDIT_DEV_H)->EnableWindow(bFlag);
	GetDlgItem(IDC_EDIT_FEE_VALUE)->EnableWindow(bFlag);
}

void CScandReading::OnCbnSelendokComboLayer()
{
	// TODO: 在此添加控件通知处理程序代码
	int iSel = m_comboLayer.GetCurSel();
}


unsigned int __stdcall ScandReading(void * param)
{
	CScandReading *dlg = (CScandReading *)param;
	dlg->SetDataEnable(FALSE);
	CTime time = CTime::GetCurrentTime();
	int nYear = time.GetYear();
	int nMonth = time.GetMonth();
	int nDay = time.GetDay();      ///日
	int nHour = time.GetHour();      ///小时
	int nMinute = time.GetMinute();   ///分钟
	int nSecond = time.GetSecond(); 
	dlg->m_strBeginTestTime.Format("%d年%d月%d日 %02d:%02d:%02d",nYear,nMonth,nDay,nHour,nMinute,nSecond);
	dlg->m_scand.ScandReading();
	dlg->m_scand.m_bRunning = FALSE;
	dlg->m_bRunning = FALSE;
	dlg->SetDataEnable(TRUE);
	time = CTime::GetCurrentTime();
	nYear = time.GetYear();
	nMonth = time.GetMonth();
	nDay = time.GetDay();      ///日
	nHour = time.GetHour();      ///小时
	nMinute = time.GetMinute();   ///分钟
	nSecond = time.GetSecond(); 
	dlg->m_strEndTestTime.Format("%d年%d月%d日 %02d:%02d:%02d",nYear,nMonth,nDay,nHour,nMinute,nSecond);
	return 0;
}

unsigned int __stdcall ScandReading_noFee1(void * param)
{
	CScandReading *dlg = (CScandReading *)param;
	g_dlg->SetDataEnable(FALSE);
	g_dlg->m_scand.ScandNoFee1();
	dlg->m_bRunning = FALSE;
	dlg->SetDataEnable(TRUE);
	dlg->GetDlgItem(IDC_EDIT_STATUS)->SetWindowText("未开始");
	return 0;
}
unsigned int __stdcall ScandReading_noFee2(void * param)
{
	CScandReading *dlg = (CScandReading *)param;
	g_dlg->SetDataEnable(FALSE);
	
	
		CTime time = CTime::GetCurrentTime();
		int nYear = time.GetYear();
		int nMonth = time.GetMonth();
		int nDay = time.GetDay();      ///日
		int nHour = time.GetHour();      ///小时
		int nMinute = time.GetMinute();   ///分钟
		int nSecond = time.GetSecond(); 
		dlg->m_strBeginTestTime.Format("%d年%d月%d日 %02d:%02d:%02d",nYear,nMonth,nDay,nHour,nMinute,nSecond);
	
	dlg->m_scand.ScandNoFee2();
	dlg->m_bRunning = FALSE;
	dlg->SetDataEnable(TRUE);

	time = CTime::GetCurrentTime();
	nYear = time.GetYear();
	nMonth = time.GetMonth();
	nDay = time.GetDay();      ///日
	nHour = time.GetHour();      ///小时
	nMinute = time.GetMinute();   ///分钟
	nSecond = time.GetSecond(); 
	dlg->m_strEndTestTime.Format("%d年%d月%d日 %02d:%02d:%02d",nYear,nMonth,nDay,nHour,nMinute,nSecond);

	return 0;
}
unsigned int __stdcall TestRegionThread(void * param)
{
	CScandReading *dlg = (CScandReading *)param;
	dlg->m_scand.TestRegion();
	return 0;
}

unsigned int __stdcall SetParmeterThread(void * param)
{
	CScandReading *dlg = (CScandReading *)param;
	dlg->m_scand.AppParameter();
	return 0;
}


void CScandReading::OnBnClickedButtonSetParmeter()
{
	// TODO: 在此添加控件通知处理程序代码
	unsigned int nThreadAddr;
	HANDLE	hSession;
	hSession = (HANDLE)_beginthreadex(0, 0, SetParmeterThread, this, 0, &nThreadAddr);
	if(hSession) CloseHandle( hSession );
}


void CScandReading::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
		//UpdateData(TRUE);
	UpdateParameter();
	if(m_MarkLocationDlg->m_iFinishSum < 6)
	{
		MessageBox("请先标记位置点");
		return;
	}
	m_comboLayer.ResetContent();
	for(int i = 0;i < m_iLayer_Sum; i++)
	{
		CString str;
		str.Format("%d",i+1);
		m_comboLayer.AddString(str);
	}
	m_comboLayer.SetCurSel(0);
	UpdateData(FALSE);

	m_scand.m_FirstScandPoint = g_OperDobot->m_WorkCenterPoint;
	m_scand.m_FirstScandPoint.x -= m_fXLen/2.0;
	m_scand.m_FirstScandPoint.y -= m_fYLen/2.0;
	m_scand.m_FirstScandPoint.z += m_iDevH;
	m_bRunning = TRUE;
	unsigned int nThreadAddr;
	HANDLE	hSession;

	hSession = (HANDLE)_beginthreadex(0, 0, ScandReading_noFee1, this, 0, &nThreadAddr);
	if(hSession) CloseHandle( hSession );
}


void CScandReading::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
		//UpdateData(TRUE);
	UpdateParameter();
	if(m_MarkLocationDlg->m_iFinishSum < 6)
	{
		MessageBox("请先标记位置点");
		return;
	}
	m_comboLayer.ResetContent();
	for(int i = 0;i < m_iLayer_Sum; i++)
	{
		CString str;
		str.Format("%d",i+1);
		m_comboLayer.AddString(str);
	}
	m_comboLayer.SetCurSel(0);
	UpdateData(FALSE);

	m_scand.m_FirstScandPoint = g_OperDobot->m_WorkCenterPoint;
	m_scand.m_FirstScandPoint.x -= m_fXLen/2.0;
	m_scand.m_FirstScandPoint.y -= m_fYLen/2.0;
	m_scand.m_FirstScandPoint.z += m_iDevH;
	m_bRunning = TRUE;
	unsigned int nThreadAddr;
	HANDLE	hSession;

	hSession = (HANDLE)_beginthreadex(0, 0, ScandReading_noFee2, this, 0, &nThreadAddr);
	if(hSession) CloseHandle( hSession );
}
