// DisplayDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OgreEditor.h"
#include "DisplayDialog.h"


// CDisplayDialog �Ի���

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


// CDisplayDialog ��Ϣ�������
