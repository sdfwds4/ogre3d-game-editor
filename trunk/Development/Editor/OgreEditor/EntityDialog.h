#pragma once

#include "resource.h"

// CEntityDialog �Ի���

class CEntityDialog : public CDialog
{
	DECLARE_DYNCREATE(CEntityDialog)

public:
	CEntityDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEntityDialog();

// �Ի�������
	enum { IDD = IDD_ENTITY_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	BOOL OnInitDialog();


	DECLARE_MESSAGE_MAP()
};
