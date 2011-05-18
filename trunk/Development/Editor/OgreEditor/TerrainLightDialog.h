

#pragma once
#include "controls/FloatEdit.h"
using namespace Ogre;

// CTerrainLightDialog �Ի���

class CTerrainLightDialog : public CDialog
{
	DECLARE_DYNCREATE(CTerrainLightDialog)

public:
	CTerrainLightDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTerrainLightDialog();

// �Ի�������
	enum { IDD = IDD_TERRAIN_LIGHT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
