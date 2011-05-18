// EffectDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "editor.h"
#include "OgreEditor.h"
#include "EffectDialog.h"
#include "EditTool/EffectTestTool.h"

// CEffectDialog 对话框

IMPLEMENT_DYNCREATE(CEffectDialog, CDialog)

CEffectDialog::CEffectDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CEffectDialog::IDD, pParent)
{

}

CEffectDialog::~CEffectDialog()
{
}

void CEffectDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EFFECT_LIST, m_EffectList);
}

BOOL CEffectDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_ParticleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();

	Ogre::vector<Ogre::String>::type nameList;
	m_ParticleSystemManager->particleSystemTemplateNames(nameList);

	for ( int i = 0; i < (int)nameList.size(); i++ )
	{
		m_EffectList.AddString( nameList[i].c_str() );
	}

	return TRUE;
}

BEGIN_MESSAGE_MAP(CEffectDialog, CDialog)
	ON_LBN_SELCHANGE(IDC_EFFECT_LIST, &CEffectDialog::OnLbnSelchangeEffectList)
END_MESSAGE_MAP()


// CEffectDialog 消息处理程序

void CEffectDialog::OnLbnSelchangeEffectList()
{
	// TODO: 在此添加控件通知处理程序代码
	CString name;
	m_EffectList.GetText( m_EffectList.GetCurSel(), name );
	CEffectTestTool* pEffectTool = (CEffectTestTool*)GetEditor()->GetCurrentEditTool();
	pEffectTool->SetCurEffectName(name);
}
