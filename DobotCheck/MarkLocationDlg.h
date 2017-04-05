#pragma once


// CMarkLocationDlg 对话框

class CMarkLocationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMarkLocationDlg)

public:
	CMarkLocationDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMarkLocationDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_M5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_iFinishSum;
	afx_msg void OnBnClickedButtonMarkFirstCard();
	afx_msg void OnBnClickedButtonMarkSecondCard();
	afx_msg void OnBnClickedButtonMarkThirdCard();
	afx_msg void OnBnClickedButtonMarkFourCard();
	afx_msg void OnBnClickedButtonMarkWorkCenter();
	afx_msg void OnBnClickedButtonMarkReadMache();
	afx_msg void OnBnClickedButtonPointSave();
	afx_msg void OnBnClickedButtonPointLoad();
};
