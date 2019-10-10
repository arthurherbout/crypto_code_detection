#pragma once
#include "afxcmn.h"
#include "afxshelltreectrl.h"

#include "MyList.h"
#include "TrueColorToolBar.h"

// CFileDlg �Ի���

class CFileDlg : public CDialog
{
	DECLARE_DYNAMIC(CFileDlg)

public:
	CFileDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFileDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_FILE };

	CTrueColorToolBar m_wndToolBar; //������
	CTrueColorToolBar m_wndToolBar_goto; //������

	CWnd* m_wParent;

	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CListCtrl m_fileList;
	CComboBoxEx m_filePath;
	CTreeCtrl m_dirTree;
	// ��ʼ���ļ������б�

	afx_msg void OnRclickTreeFiledirectory(NMHDR *pNMHDR, LRESULT *pResult);
};



void HsLoadFileTreeList(CFileDlg *cFleDlg);