

#pragma once

// CNpcDialog 对话框

class CNpcDialog : public CDialog
{
	DECLARE_DYNCREATE(CNpcDialog)

public:
	CNpcDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNpcDialog();

// 对话框数据
	enum { IDD = IDD_NPC_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	BOOL OnInitDialog();

	afx_msg void OnLbnSelchangeNpcList();

	DECLARE_MESSAGE_MAP()
public:
	CListBox mNpcList;
};
