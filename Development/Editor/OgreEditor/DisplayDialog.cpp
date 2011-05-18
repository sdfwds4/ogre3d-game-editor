// DisplayDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "OgreEditor.h"
#include "DisplayDialog.h"


// CDisplayDialog 对话框

IMPLEMENT_DYNCREATE(CDisplayDialog, CDialog)

CDisplayDialog::CDisplayDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDisplayDialog::IDD, pParent)
{

}

CDisplayDialog::~CDisplayDialog()
{
}

void CDisplayDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BOOL CDisplayDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	return TRUE;
}

BEGIN_MESSAGE_MAP(CDisplayDialog, CDialog)
END_MESSAGE_MAP()


// CDisplayDialog 消息处理程序
