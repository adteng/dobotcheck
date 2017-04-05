#pragma once


#include "afxwin.h"
#include "MarkLocationDlg.h"
#include "OperDobot.h"
// CScandReading 对话框
#include "_Application.h"
#include "_Documents.h"
#include "_Document.h"
#include "_Selection.h"
#include "_Tables.h"
#include "_Table.h"
#include "CnlineShapes.h"
#include "ScandOP.h"



class CScandReading : public CDialogEx
{
	DECLARE_DYNAMIC(CScandReading)

public:
	CScandReading(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CScandReading();

	void SetupPosition(float fXLen,float fYLen,int iX_sum,int iY_sum,int iZ_sum,const CDobotPoint &firstPoint);
	void DrawPointWithStatus();
	void DrawPointWithStatus_bak();

// 对话框数据
	enum { IDD = IDD_DIALOG_M1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBeginScand();
	afx_msg void OnBnClickedCancelScand();
	afx_msg void OnBnClickedButtonTestScandRegion();
	afx_msg void OnBnClickedButtonCreateReport();
	virtual BOOL OnInitDialog();
	float m_fXLen;
	float m_fYLen;
	int m_iX_Sum;
	int m_iY_Sum;
	float m_fLayerSpaceLen;
	int m_iLayer_Sum;
	int m_iDevL;
	int m_iDevW;
	int m_iDevH;
	int m_iFeeValue;
	BOOL m_bRunning;
	CScandOP m_scand;
	
	CComboBox m_comboLayer;
	CMarkLocationDlg *m_MarkLocationDlg;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonMarkBeginPoint();
	afx_msg void OnCbnSelendokComboLayer();
	int GetSucceedPointSum(int z);
	void SetDataEnable(BOOL bFlag);
	void DrawIcon(int iFlag);
	void DrawBackGround();
	void ShowBitmap(CDC *pDC, CString BmpName);
	void UpdateParameter();
	BOOL m_bFeeMode;
	CString m_strBeginTestTime;
	CString m_strEndTestTime;
	_Application	m_Application;
	_Selection		m_Selection;
	_Documents		m_Documents;
	_Document		m_ActiveDocument;
	_Tables			m_Tables;
	_Table			m_Table;
	CnlineShapes	m_LineShapes;
	afx_msg void OnBnClickedButtonSetParmeter();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
