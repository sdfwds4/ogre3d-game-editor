#pragma once

#include "resource.h"
// CParticlePreviewDialog �Ի���

class CParticlePreviewDialog : public CDialog
{
	DECLARE_DYNAMIC(CParticlePreviewDialog)

public:
	CParticlePreviewDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CParticlePreviewDialog();

// �Ի�������
	enum { IDD = IDD_PARTICLE_PREVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);

};
