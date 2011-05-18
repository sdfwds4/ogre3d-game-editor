#pragma once


// CTerrainTextureCombineDialog 对话框

class CTerrainTextureCombineDialog : public CDialog
{
	DECLARE_DYNAMIC(CTerrainTextureCombineDialog)

public:
	CTerrainTextureCombineDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTerrainTextureCombineDialog();

// 对话框数据
	enum { IDD = IDD_TEXTURE_COMBINE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	CString mDiffuse;
	CString mSpecular;
	CString mNormal;
	CString mDisp;
	CString mDiffuseSpecular;
	CString mNormalDisp;
};
