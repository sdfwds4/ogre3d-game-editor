// ParticlePreviewDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "OgreEditor.h"
#include "ParticlePreviewDialog.h"


// CParticlePreviewDialog 对话框

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


// CParticlePreviewDialog 消息处理程序
void CParticlePreviewDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
}