// BrushBidlog.cpp : 实现文件
//

#include "stdafx.h"
#include "editor.h"
#include "OgreEditor.h"
#include "BrushDialog.h"
#include "EditTool/ObjectCreateTool.h"


// CBrushBidlog 对话框

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


// CBrushBidlog 消息处理程序
void CBrushDialog::OnLbnSelchangeBrushList()
{
	// TODO: 在此添加控件通知处理程序代码

	CBrushLine* pBrush = (CBrushLine*)mBrushList.GetItemData( mBrushList.GetCurSel() );
	CObjectCreateTool* pTool = (CObjectCreateTool*)GetEditor()->GetCurrentEditTool();
	pTool->SetBrush( pBrush );
}
