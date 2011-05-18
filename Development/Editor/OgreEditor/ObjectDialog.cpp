// ObjectDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "OgreEditor.h"
#include "ObjectDialog.h"
#include "EffectDialog.h"
#include "EditTool/EffectTestTool.h"
#include "EditTool/NpcTestTool.h"
#include "EditTool/ObjectCreateTool.h"

// CObjectDialog 对话框

IMPLEMENT_DYNCREATE(CObjectDialog, CDialog)

CObjectDialog::CObjectDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CObjectDialog::IDD, pParent)
{

}

CObjectDialog::~CObjectDialog()
{
}

void CObjectDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CObjectDialog, CDialog)
	ON_BN_CLICKED(ID_NPC, &CObjectDialog::OnBnClickedNpc)
	ON_BN_CLICKED(ID_MONSTER, &CObjectDialog::OnBnClickedMonster)
	ON_BN_CLICKED(IDC_EFFECT, &CObjectDialog::OnBnClickedEffect)
	ON_BN_CLICKED(ID_MISC, &CObjectDialog::OnBnClickedMisc)
	ON_BN_CLICKED(ID_BRUSH, &CObjectDialog::OnBnClickedBrush)
END_MESSAGE_MAP()


// CObjectDialog 消息处理程序
void CObjectDialog::OnBnClickedNpc()
{
	// TODO: 在此添加控件通知处理程序代码
	if ( !GetEditor()->IsLevelLoaded() )
		return;

	GetEditor()->SetEditTool(new CNpcTestTool);	
}

void CObjectDialog::OnBnClickedMonster()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CObjectDialog::OnBnClickedEffect()
{
	// TODO: 在此添加控件通知处理程序代码
	if ( !GetEditor()->IsLevelLoaded() )
		return;

	GetEditor()->SetEditTool(new CEffectTestTool);	
}

void CObjectDialog::OnBnClickedMisc()
{
	// TODO: 在此添加控件通知处理程序代码

	GetEditor()->ShowParticleDialog(true);

}

void CObjectDialog::OnBnClickedBrush()
{
	// TODO: 在此添加控件通知处理程序代码
	if ( !GetEditor()->IsLevelLoaded() )
		return;

	CObjectCreateTool* pTool = new CObjectCreateTool;
	pTool->SetObjectType(E_BRUSH);
	GetEditor()->SetEditTool(pTool);	
}
