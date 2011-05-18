#pragma once


// CLayerDialog 对话框

class CLayerDialog : public CDialog
{
	DECLARE_DYNCREATE(CLayerDialog)

public:
	CLayerDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLayerDialog();

// 对话框数据
	enum { IDD = IDD_LAYER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
