// ParticlePreviewDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OgreEditor.h"
#include "ParticlePreviewDialog.h"


// CParticlePreviewDialog �Ի���

IMPLEMENT_DYNAMIC(CParticlePreviewDialog, CDialog)

CParticlePreviewDialog::CParticlePreviewDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CParticlePreviewDialog::IDD, pParent)
{

}

CParticlePreviewDialog::~CParticlePreviewDialog()
{
}

void CParticlePreviewDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CParticlePreviewDialog, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CParticlePreviewDialog ��Ϣ�������
void CParticlePreviewDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
}