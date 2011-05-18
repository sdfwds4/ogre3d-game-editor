// BrushBidlog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "editor.h"
#include "OgreEditor.h"
#include "BrushDialog.h"
#include "EditTool/ObjectCreateTool.h"


// CBrushBidlog �Ի���

IMPLEMENT_DYNCREATE(CBrushDialog, CDialog)

CBrushDialog::CBrushDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CBrushDialog::IDD, pParent)
{

}

CBrushDialog::~CBrushDialog()
{
}

void CBrushDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BRUSH_LIST, mBrushList);
}

BOOL CBrushDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	for ( int i = 1; i < CEntityManager::getSingletonPtr()->GetBrushTable()->GetFileLineNum(); i++ )
	{
		CBrushLine* pBrush = CEntityManager::getSingletonPtr()->GetBrushTable()->GetExcelLine(i);
		int id = mBrushList.AddString( pBrush->mstrName.GetBuffer() );
		mBrushList.SetItemData( id, (DWORD_PTR)pBrush );
	}

	return TRUE;
}

BEGIN_MESSAGE_MAP(CBrushDialog, CDialog)
	ON_LBN_SELCHANGE(IDC_BRUSH_LIST, &CBrushDialog::OnLbnSelchangeBrushList)
END_MESSAGE_MAP()


// CBrushBidlog ��Ϣ�������
void CBrushDialog::OnLbnSelchangeBrushList()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CBrushLine* pBrush = (CBrushLine*)mBrushList.GetItemData( mBrushList.GetCurSel() );
	CObjectCreateTool* pTool = (CObjectCreateTool*)GetEditor()->GetCurrentEditTool();
	pTool->SetBrush( pBrush );
}
