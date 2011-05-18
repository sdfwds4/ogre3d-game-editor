// MonsterDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "OgreEditor.h"
#include "MonsterDialog.h"


// CMonsterDialog 对话框

IMPLEMENT_DYNCREATE(CMonsterDialog, CDialog)

CMonsterDialog::CMonsterDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMonsterDialog::IDD, pParent)
{

}

CMonsterDialog::~CMonsterDialog()
{
}

void CMonsterDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MONSTER_LIST, mMonsterList);
}


BEGIN_MESSAGE_MAP(CMonsterDialog, CDialog)
END_MESSAGE_MAP()


// CMonsterDialog 消息处理程序
BOOL CMonsterDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();


	return TRUE;
}
