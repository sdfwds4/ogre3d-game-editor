// MonsterDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OgreEditor.h"
#include "MonsterDialog.h"


// CMonsterDialog �Ի���

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


// CMonsterDialog ��Ϣ�������
BOOL CMonsterDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();


	return TRUE;
}
