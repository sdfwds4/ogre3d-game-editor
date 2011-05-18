// NpcDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OgreEditor.h"
#include "editor.h"
#include "NpcDialog.h"
#include "EditTool/ObjectCreateTool.h"


// CNpcDialog �Ի���

IMPLEMENT_DYNCREATE(CNpcDialog, CDialog)

CNpcDialog::CNpcDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CNpcDialog::IDD, pParent)
{

}

CNpcDialog::~CNpcDialog()
{
}

void CNpcDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NPC_LIST, mNpcList);
}

BOOL CNpcDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();


	return TRUE;
}

BEGIN_MESSAGE_MAP(CNpcDialog, CDialog)
	ON_LBN_SELCHANGE(IDC_NPC_LIST, &CNpcDialog::OnLbnSelchangeNpcList)
END_MESSAGE_MAP()


// CNpcDialog ��Ϣ�������


// CNpcDialog ��Ϣ�������
void CNpcDialog::OnLbnSelchangeNpcList()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString name;
	mNpcList.GetText( mNpcList.GetCurSel(), name );
	CObjectCreateTool* pTool = (CObjectCreateTool*)GetEditor()->GetCurrentEditTool();
	//pTool->SetCurEffectName(name);
}
