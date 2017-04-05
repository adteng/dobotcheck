#pragma once


// CFlashFileNameDlg 对话框

class CFlashFileNameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFlashFileNameDlg)

public:
	CFlashFileNameDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFlashFileNameDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SAVE_FLASH_FILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strFlashFileName;
};
