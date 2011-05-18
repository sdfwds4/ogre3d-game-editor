#pragma once

#include "Editor.h"
// CObjectDialog 对话框

class CObjectDialog : public CDialog
{
	DECLARE_DYNCREATE(CObjectDialog)

public:
	CObjectDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CObjectDialog();

// 对话框数据
	enum { IDD = IDD_OBJECTS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
