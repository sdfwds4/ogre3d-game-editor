

#pragma once
#include "afxcmn.h"

// CTerrainModifyDialog �Ի���

class CTerrainModifyDialog : public CDialog
{
	DECLARE_DYNCREATE(CTerrainModifyDialog)

public:
	CTerrainModifyDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTerrainModifyDialog();

// �Ի�������
	enum { IDD = IDD_TERRAIN_MODIFY_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CComboBox mBrushTypeList;
	CSliderCtrl mBrushScaleSlider;

	CString mBrushType;
	int miScale;
	int miHardness;

	afx_msg void OnCbnSelchangeBrushTypeCombo();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	CSliderCtrl mHardnessSlider;
};
