#pragma once



// CBrushBialog 对话框

class CBrushDialog : public CDialog
{
	DECLARE_DYNCREATE(CBrushDialog)

public:
	CBrushDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBrushDialog();

// 对话框数据
	enum { IDD = IDD_BRUSH_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	BOOL OnInitDialog();

	afx_msg void OnLbnSelchangeBrushList();
	DECLARE_MESSAGE_MAP()
public:
	CListBox mBrushList;
};
