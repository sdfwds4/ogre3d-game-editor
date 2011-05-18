#pragma once

#include "resource.h"

// CEntityDialog 对话框

class CEntityDialog : public CDialog
{
	DECLARE_DYNCREATE(CEntityDialog)

public:
	CEntityDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEntityDialog();

// 对话框数据
	enum { IDD = IDD_ENTITY_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	BOOL OnInitDialog();


	DECLARE_MESSAGE_MAP()
};
