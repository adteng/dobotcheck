
// DobotCheckDlg.h : ͷ�ļ�
//

#pragma once
#include "OperDobot.h"
#include "afxcmn.h"
#include "ScandReading.h"
#include "FlashCard.h"
#include "PressureCardNew.h"


// CDobotCheckDlg �Ի���
class CDobotCheckDlg : public CDialogEx
{
// ����
public:
	CDobotCheckDlg(CWnd* pParent = NULL);	// ��׼���캯��
	void ShowInfo(CString inf);
	bool SearchCOM(char *pName,const char *pFindName);
	void PaintDobotPoint(const CDobotPoint &p,COLORREF crColor);
	
	CScandReading m_ScandCard;
	CFlashCard    m_FlashCard;
	CPressureCardNew m_PressureCardNew;

// �Ի�������
	enum { IDD = IDD_DOBOTCHECK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedSend();
	float m_f1;
	float m_f7;
	float m_f8;
	float m_f9;
	float m_f10;
	float m_f2;
	float m_f3;
	float m_f4;
	float m_f5;
	float m_f6;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	CTabCtrl m_TabCtrl;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonMark();
};
