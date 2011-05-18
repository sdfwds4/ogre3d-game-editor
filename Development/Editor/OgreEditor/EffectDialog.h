#pragma once
#include "afxwin.h"


// CEffectDialog 对话框

class CEffectDialog : public CDialog
{
	DECLARE_DYNCREATE(CEffectDialog)

public:
	CEffectDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEffectDialog();

// 对话框数据
	enum { IDD = IDD_EFFECT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_EffectList;
	ParticleUniverse::ParticleSystemManager* m_ParticleSystemManager;
	afx_msg void OnLbnSelchangeEffectList();
};
