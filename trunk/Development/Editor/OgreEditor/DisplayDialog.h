#pragma once


// CDisplayDialog 对话框

class CDisplayDialog : public CDialog
{
	DECLARE_DYNCREATE(CDisplayDialog)

public:
	CDisplayDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDisplayDialog();

// 对话框数据
	enum { IDD = IDD_DISPLAY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};
