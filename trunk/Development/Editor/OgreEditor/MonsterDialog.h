#pragma once
#include "afxwin.h"


// CMonsterDialog �Ի���

class CMonsterDialog : public CDialog
{
	DECLARE_DYNCREATE(CMonsterDialog)

public:
	CMonsterDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMonsterDialog();

// �Ի�������
	enum { IDD = IDD_MONSTER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	BOOL OnInitDialog(); 

	DECLARE_MESSAGE_MAP()
public:
	CListBox mMonsterList;
};
