#pragma once


// CMarkLocationDlg �Ի���

class CMarkLocationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMarkLocationDlg)

public:
	CMarkLocationDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMarkLocationDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_M5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
