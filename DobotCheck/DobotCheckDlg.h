
// DobotCheckDlg.h : 头文件
//

#pragma once
#include "OperDobot.h"
#include "afxcmn.h"
#include "ScandReading.h"
#include "FlashCard.h"
#include "PressureCardNew.h"


// CDobotCheckDlg 对话框
class CDobotCheckDlg : public CDialogEx
{
// 构造
public:
	CDobotCheckDlg(CWnd* pParent = NULL);	// 标准构造函数
	void ShowInfo(CString inf);
	bool SearchCOM(char *pName,const char *pFindName);
	void PaintDobotPoint(const CDobotPoint &p,COLORREF crColor);
	
	CScandReading m_ScandCard;
	CFlashCard    m_FlashCard;
	CPressureCardNew m_PressureCardNew;

// 对话框数据
	enum { IDD = IDD_DOBOTCHECK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
