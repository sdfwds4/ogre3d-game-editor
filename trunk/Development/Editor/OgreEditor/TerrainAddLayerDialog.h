#pragma once


// CTerrainAddLayerDialog �Ի���

class CTerrainAddLayerDialog : public CDialog
{
	DECLARE_DYNAMIC(CTerrainAddLayerDialog)

public:
	CTerrainAddLayerDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTerrainAddLayerDialog();

// �Ի�������
	enum { IDD = IDD_TERRAIN_ADD_LAYER_DIALOG };
	int mWorldSize;
	CString mTexture1;
	CString mTexture2;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
