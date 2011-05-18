// LogoDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "OgreEditor.h"
#include "LogoDialog.h"

#include "Editor.h"

// CLogoDialog 对话框

IMPLEMENT_DYNAMIC(CLogoDialog, CDialog)

CLogoDialog::CLogoDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CLogoDialog::IDD, pParent)
{

}

CLogoDialog::~CLogoDialog()
{
	Ogre::ResourceGroupManager::getSingleton().removeResourceGroupListener(this);
}

void CLogoDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOGO_PIC, m_LogoPic);
	DDX_Control(pDX, IDC_LOG_TEXT, m_LogText);
}

BOOL CLogoDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	return TRUE;
}


BEGIN_MESSAGE_MAP(CLogoDialog, CDialog)
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()


// CLogoDialog 消息处理程序
void CLogoDialog::OnSetFocus(CWnd* pOldWnd)
{
	Invalidate();
	CDialog::OnSetFocus(pOldWnd);
}
