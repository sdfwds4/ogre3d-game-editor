#pragma once
#include "afxwin.h"


// CEffectDialog �Ի���

class CEffectDialog : public CDialog
{
	DECLARE_DYNCREATE(CEffectDialog)

public:
	CEffectDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEffectDialog();

// �Ի�������
	enum { IDD = IDD_EFFECT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_EffectList;
	ParticleUniverse::ParticleSystemManager* m_ParticleSystemManager;
	afx_msg void OnLbnSelchangeEffectList();
};
