#pragma once


// CDisplayDialog �Ի���

class CDisplayDialog : public CDialog
{
	DECLARE_DYNCREATE(CDisplayDialog)

public:
	CDisplayDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDisplayDialog();

// �Ի�������
	enum { IDD = IDD_DISPLAY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};
