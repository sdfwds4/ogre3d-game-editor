// NpcDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "OgreEditor.h"
#include "editor.h"
#include "NpcDialog.h"
#include "EditTool/ObjectCreateTool.h"


// CNpcDialog 对话框

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


// CNpcDialog 消息处理程序


// CNpcDialog 消息处理程序
void CNpcDialog::OnLbnSelchangeNpcList()
{
	// TODO: 在此添加控件通知处理程序代码
	CString name;
	mNpcList.GetText( mNpcList.GetCurSel(), name );
	CObjectCreateTool* pTool = (CObjectCreateTool*)GetEditor()->GetCurrentEditTool();
	//pTool->SetCurEffectName(name);
}
