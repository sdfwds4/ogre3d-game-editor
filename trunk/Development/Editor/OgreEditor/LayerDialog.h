#pragma once


// CLayerDialog �Ի���

class CLayerDialog : public CDialog
{
	DECLARE_DYNCREATE(CLayerDialog)

public:
	CLayerDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLayerDialog();

// �Ի�������
	enum { IDD = IDD_LAYER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
