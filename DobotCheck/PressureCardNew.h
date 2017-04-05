#pragma once


// CPressureCardNew �Ի���
#include "PressureCard.h"
class CPressureCardNew : public CDialogEx
{
	DECLARE_DYNAMIC(CPressureCardNew)

public:
	CPressureCardNew(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPressureCardNew();

// �Ի�������
	enum { IDD = IDD_DIALOG_M4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonStartPressure();
	afx_msg void OnBnClickedButtonAddPressure();
	virtual BOOL OnInitDialog();
	CPressureCard *m_PressureCardDlg;
	CComboBox m_combo_splist;
	void FindFileFromDir();
	int m_iLoopTotalSum;
	int m_iLoopCurrentSum;
	list<DobotOrder> m_CMDList;
	bool bExeStop;
	afx_msg void OnBnClickedButtonRefresh();
	afx_msg void OnBnClickedButtonStop();
};
