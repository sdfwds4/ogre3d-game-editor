#pragma once

#include "resource.h"
// CParticlePreviewDialog 对话框

class CParticlePreviewDialog : public CDialog
{
	DECLARE_DYNAMIC(CParticlePreviewDialog)

public:
	CParticlePreviewDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CParticlePreviewDialog();

// 对话框数据
	enum { IDD = IDD_PARTICLE_PREVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);

};
