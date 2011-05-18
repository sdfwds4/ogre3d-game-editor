

#pragma once
#include "controls/FloatEdit.h"
using namespace Ogre;

// CTerrainLightDialog 对话框

class CTerrainLightDialog : public CDialog
{
	DECLARE_DYNCREATE(CTerrainLightDialog)

public:
	CTerrainLightDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTerrainLightDialog();

// 对话框数据
	enum { IDD = IDD_TERRAIN_LIGHT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CFloatEdit mXEdit;
	CFloatEdit mYEdit;
	CFloatEdit mZEdit;

	Vector3 mDir;
	Light* mTerrainLight;
	afx_msg void OnBnClickedApplyBtn();
};
