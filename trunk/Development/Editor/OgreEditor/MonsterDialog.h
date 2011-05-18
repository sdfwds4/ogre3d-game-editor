#pragma once
#include "afxwin.h"


// CMonsterDialog 对话框

class CMonsterDialog : public CDialog
{
	DECLARE_DYNCREATE(CMonsterDialog)

public:
	CMonsterDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMonsterDialog();

// 对话框数据
	enum { IDD = IDD_MONSTER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	BOOL OnInitDialog(); 

	DECLARE_MESSAGE_MAP()
public:
	CListBox mMonsterList;
};
