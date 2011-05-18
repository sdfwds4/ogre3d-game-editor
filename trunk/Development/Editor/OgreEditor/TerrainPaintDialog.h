

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CTerrainPaintDialog �Ի���

class CTerrainPaintDialog : public CDialog
{
	DECLARE_DYNCREATE(CTerrainPaintDialog)

	struct LayerInfo
	{
		HTREEITEM mItem;
		int mId;
	};
public:
	CTerrainPaintDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTerrainPaintDialog();

// �Ի�������
	enum { IDD = IDD_TERRAIN_PAINT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	BOOL OnInitDialog();

	void CreateLayerTree();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCbnSelchangeBrushTypeCombo();

	CSliderCtrl mScaleSlider;
	CSliderCtrl mHardnessSlider;
	CComboBox mBrushTypeList;
	CTreeCtrl mLayerTree;
	CEdit mScaleEdit;
	CEdit mHardnessEdit;

	int miScale;
	int miHardness;
	CString mBrushType;
	int mMaxLayers;
	int mLayers;
	LayerInfo* mLayerInfos;
	HTREEITEM mhRootItem;

	Terrain* mpTerrain;

	CStatic mLayerInfoText;
	afx_msg void OnBnClickedAddLayerBtn();
	afx_msg void OnBnClickedDelLayerBtn();
};
