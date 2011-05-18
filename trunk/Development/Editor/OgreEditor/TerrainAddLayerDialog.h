#pragma once


// CTerrainAddLayerDialog 对话框

class CTerrainAddLayerDialog : public CDialog
{
	DECLARE_DYNAMIC(CTerrainAddLayerDialog)

public:
	CTerrainAddLayerDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTerrainAddLayerDialog();

// 对话框数据
	enum { IDD = IDD_TERRAIN_ADD_LAYER_DIALOG };
	int mWorldSize;
	CString mTexture1;
	CString mTexture2;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
