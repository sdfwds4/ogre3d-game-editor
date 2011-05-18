

#pragma once

#include "TerrainTextureCombineDialog.h"
// CTerrainDialog �Ի���

class CTerrainDialog : public CDialog
{
	DECLARE_DYNCREATE(CTerrainDialog)

public:
	CTerrainDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTerrainDialog();

// �Ի�������
	enum { IDD = IDD_TERRAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	BOOL OnInitDialog();

	CTerrainTextureCombineDialog mTextureCombinDialog;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedModify();
	afx_msg void OnBnClickedPaint();
	afx_msg void OnBnClickedLayerTextureBtn();

	afx_msg void OnBnClickedLight();
};
