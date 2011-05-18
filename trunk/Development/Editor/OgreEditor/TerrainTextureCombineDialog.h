#pragma once


// CTerrainTextureCombineDialog �Ի���

class CTerrainTextureCombineDialog : public CDialog
{
	DECLARE_DYNAMIC(CTerrainTextureCombineDialog)

public:
	CTerrainTextureCombineDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTerrainTextureCombineDialog();

// �Ի�������
	enum { IDD = IDD_TEXTURE_COMBINE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
