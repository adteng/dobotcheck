// FlashFileNameDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DobotCheck.h"
#include "FlashFileNameDlg.h"
#include "afxdialogex.h"


// CFlashFileNameDlg 对话框

IMPLEMENT_DYNAMIC(CFlashFileNameDlg, CDialogEx)

CFlashFileNameDlg::CFlashFileNameDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFlashFileNameDlg::IDD, pParent)
	, m_strFlashFileName(_T(""))
{

}

CFlashFileNameDlg::~CFlashFileNameDlg()
{
}

void CFlashFileNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FLASH_FILE_NAME, m_strFlashFileName);
}


BEGIN_MESSAGE_MAP(CFlashFileNameDlg, CDialogEx)
END_MESSAGE_MAP()


// CFlashFileNameDlg 消息处理程序
