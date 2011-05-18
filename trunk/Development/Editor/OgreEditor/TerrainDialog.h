

#pragma once

#include "TerrainTextureCombineDialog.h"
// CTerrainDialog 对话框

class CTerrainDialog : public CDialog
{
	DECLARE_DYNCREATE(CTerrainDialog)

public:
	CTerrainDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTerrainDialog();

// 对话框数据
	enum { IDD = IDD_TERRAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	BOOL OnInitDialog();

	CTerrainTextureCombineDialog mTextureCombinDialog;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedModify();
	afx_msg void OnBnClickedPaint();
	afx_msg void OnBnClickedLayerTextureBtn();

	afx_msg void OnBnClickedLight();
};
