#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include <fstream>
#include <iostream>
#include <string>

#include "OperDobot2.h"
using namespace std;

typedef struct tagDataLi
{
	Pose pt;
	float _time;
	bool _grab;
} DataLi;


// CPressureCard 对话框

class CPressureCard : public CDialogEx
{
	DECLARE_DYNAMIC(CPressureCard)

public:
	CPressureCard(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPressureCard();

// 对话框数据
	enum { IDD = IDD_DIALOG_M3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_ControlType;
	CComboBox m_MoveType;
	CEdit m_X;
	CEdit m_Y;
	CEdit m_Z;
	CEdit m_R;
	CComboBox m_IsGrip;
	CEdit m_PauseTime;
	CEdit m_Loop;
	void SetItemNO();
	void ExeSel(bool bExeAll = FALSE);
	void FindFileFromDir();
	vector<DataLi> m_data,m_copy_data;
	CListCtrl m_list_cmd;
	bool bExeStop,bDelAllItem;
	int nItemCur,nItemCnt;
	CEdit m_Gripper;
	CEdit m_Loop2;
	list<DobotOrder> m_CMDList;
	string sFileName;

	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedButtonSavefile();
	afx_msg void OnBnClickedButtonOpenefile();
	afx_msg void OnBnClickedButtonPause();
	afx_msg void OnBnClickedButtonExe();
	afx_msg void OnBnClickedButtonExesel();
	afx_msg void OnBnClickedButtonRefresh();
	afx_msg void OnNMDblclkListCmd(NMHDR *pNMHDR, LRESULT *pResult);
	CListBox m_list_sport;
	afx_msg void OnLbnDblclkListSport();
	afx_msg void OnLbnSelchangeListSport();
	void CmdWrite2File(CString strFileName);
	void SetRunningStatus(bool b);
	afx_msg void OnNMRClickListCmd(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnModify();
	afx_msg void OnPaste();
	afx_msg void OnCopy();
	afx_msg void OnDel();
	afx_msg void OnDelAll();
};
