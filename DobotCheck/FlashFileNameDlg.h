#pragma once


// CFlashFileNameDlg �Ի���

class CFlashFileNameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFlashFileNameDlg)

public:
	CFlashFileNameDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFlashFileNameDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_SAVE_FLASH_FILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strFlashFileName;
};
