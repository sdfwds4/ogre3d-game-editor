

#pragma once

// CNpcDialog �Ի���

class CNpcDialog : public CDialog
{
	DECLARE_DYNCREATE(CNpcDialog)

public:
	CNpcDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNpcDialog();

// �Ի�������
	enum { IDD = IDD_NPC_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	BOOL OnInitDialog();

	afx_msg void OnLbnSelchangeNpcList();

	DECLARE_MESSAGE_MAP()
public:
	CListBox mNpcList;
};
