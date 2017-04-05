#pragma once


// CFlashCard 对话框
#include "OperDobot.h"
#include "afxwin.h"
class CFlashCard : public CDialogEx
{
	DECLARE_DYNAMIC(CFlashCard)

public:
	CFlashCard(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFlashCard();

// 对话框数据
	enum { IDD = IDD_DIALOG_M2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonStart();
	void Radio1();
	void Radio2();
	void Radio3();
	void Radio4();
	void Radio5();
	void WriteFlashFile(CString strDBPath);
	int m_iRadioSelect;
	float m_fPauseTime;
	CDobotPoint m_WorkPoint,m_PausePoint;
	afx_msg void OnBnClickedButtonGetWorkPoint();
	float m_f1;
	float m_f2;
	float m_f3;
//	CString m_f4;
	float m_f5;
	float m_f6;
	float m_f8;
	afx_msg void OnBnClickedButtonSave();
	CComboBox m_combo_flash_config;
	float m_f4;
//	afx_msg void OnNMThemeChangedCombo1(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnCbnEditchangeCombo1();
	afx_msg void OnCbnSelendokCombo1();
	afx_msg void OnBnClickedButtonChange();
	afx_msg void OnBnClickedCheckGrab();
	CButton m_Check_grib;
	int m_iLoop;
};
