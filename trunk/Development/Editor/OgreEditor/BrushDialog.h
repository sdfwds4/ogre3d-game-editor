#pragma once



// CBrushBialog �Ի���

class CBrushDialog : public CDialog
{
	DECLARE_DYNCREATE(CBrushDialog)

public:
	CBrushDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBrushDialog();

// �Ի�������
	enum { IDD = IDD_BRUSH_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	BOOL OnInitDialog();

	afx_msg void OnLbnSelchangeBrushList();
	DECLARE_MESSAGE_MAP()
public:
	CListBox mBrushList;
};
