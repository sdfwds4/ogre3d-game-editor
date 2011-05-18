// RenderDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "OgreEditor.h"
#include "RenderDialog.h"
#include "Editor.h"


// CRenderDialog 对话框

IMPLEMENT_DYNAMIC(CRenderDialog, CDialog)

CRenderDialog::CRenderDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CRenderDialog::IDD, pParent), m_bInit(false)
{

}

CRenderDialog::~CRenderDialog()
{
}

void CRenderDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CRenderDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if ( !m_bInit )
	{
		m_bInit = true;
		// TODO: Add your specialized code here and/or call the base class
		CRect rect;
		GetClientRect( rect );
		GetEditor()->Create( GetSafeHwnd(), rect.Width(), rect.Height() );

		//tooltip = new CBitmapTooltip(CRect(300,300,0,0));
		//tooltip->ShowWindow(SW_SHOW);
	}

	return TRUE;
}

void CRenderDialog::OnOK()
{

}

void CRenderDialog::OnCancel()
{
	GetEditor()->SetEditTool(NULL);
}

BEGIN_MESSAGE_MAP(CRenderDialog, CDialog)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CRenderDialog 消息处理程序
BOOL CRenderDialog::OnEraseBkgnd(CDC* pDC) 
{
	// "Tile" bitmap (see demo for actual code)
	return TRUE; // tell Windows we handled it
}

void CRenderDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	CRect rect; 
	GetClientRect(&rect); 

	GetEditor()->Resize( rect.Width(), rect.Height() );
}

void CRenderDialog::OnDestroy()
{
	GetEditor()->Destroy();

	CDialog::OnDestroy();
}


void CRenderDialog::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);

	GetEditor()->OnKeyDown(nChar,nRepCnt,nFlags);
}

void CRenderDialog::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CDialog::OnKeyUp(nChar, nRepCnt, nFlags);

	GetEditor()->OnKeyUp(nChar,nRepCnt,nFlags);
}

void CRenderDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
	CDialog::OnLButtonDown(nFlags, point);

	SetFocus();
	GetEditor()->OnLButtonDown(nFlags,point);
}

void CRenderDialog::OnLButtonUp(UINT nFlags, CPoint point)
{
	CDialog::OnLButtonUp(nFlags, point);

	GetEditor()->OnLButtonUp(nFlags,point);
}

void CRenderDialog::OnMButtonDown(UINT nFlags, CPoint point)
{
	CDialog::OnMButtonDown(nFlags, point);

}

void CRenderDialog::OnMButtonUp(UINT nFlags, CPoint point)
{
	CDialog::OnMButtonUp(nFlags, point);
}

void CRenderDialog::OnMouseMove(UINT nFlags, CPoint point)
{
	CDialog::OnMouseMove(nFlags, point);

	GetEditor()->OnMouseMove(nFlags,point);
}

BOOL CRenderDialog::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	GetEditor()->OnMouseWheel( nFlags, zDelta, pt);

	return CDialog::OnMouseWheel( nFlags, zDelta, pt);
}

void CRenderDialog::OnRButtonDown(UINT nFlags, CPoint point)
{
	GetEditor()->OnRButtonDown(nFlags,point);
	SetFocus();

	CDialog::OnRButtonDown(nFlags, point);
}

void CRenderDialog::OnRButtonUp(UINT nFlags, CPoint point)
{
	GetEditor()->OnRButtonUp(nFlags,point);

	CDialog::OnRButtonUp(nFlags, point);
}

void CRenderDialog::OnSetFocus(CWnd* pOldWnd)
{
	CDialog::OnSetFocus(pOldWnd);
}

void CRenderDialog::OnKillFocus(CWnd* pOldWnd)
{
	CDialog::OnKillFocus(pOldWnd);
}