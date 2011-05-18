#pragma once

#include "Editor.h"
// CObjectDialog �Ի���

class CObjectDialog : public CDialog
{
	DECLARE_DYNCREATE(CObjectDialog)

public:
	CObjectDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CObjectDialog();

// �Ի�������
	enum { IDD = IDD_OBJECTS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedNpc();
	afx_msg void OnBnClickedMonster();
	afx_msg void OnBnClickedEffect();

protected:
	CRollupCtrl* m_pObjectCtrl;
	int m_iCurrentPageId;
public:
	afx_msg void OnBnClickedMisc();
	afx_msg void OnBnClickedBrush();
};
