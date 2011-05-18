// EntityDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "editor.h"
#include "OgreEditor.h"
#include "EntityDialog.h"


// CEntityDialog 对话框

IMPLEMENT_DYNCREATE(CEntityDialog, CDialog)

CEntityDialog::CEntityDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CEntityDialog::IDD, pParent)
{

}

CEntityDialog::~CEntityDialog()
{
}

void CEntityDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BOOL CEntityDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	return TRUE;
}


BEGIN_MESSAGE_MAP(CEntityDialog, CDialog)
END_MESSAGE_MAP()


// CEntityDialog 消息处理程序
