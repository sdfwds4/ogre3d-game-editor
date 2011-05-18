// OgreEditorView.cpp : implementation of the COgreEditorView class
//

#include "stdafx.h"
#include "OgreEditor.h"

#include "OgreEditorView.h"
#include "Editor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COgreEditorView

IMPLEMENT_DYNCREATE(COgreEditorView, CView)

BEGIN_MESSAGE_MAP(COgreEditorView, CView)
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

// COgreEditorView construction/destruction

COgreEditorView::COgreEditorView() : m_bInit(false)
{
	// TODO: add construction code here

}

COgreEditorView::~COgreEditorView()
{
}

BOOL COgreEditorView::OnEraseBkgnd(CDC* pDC) 
{
	// "Tile" bitmap (see demo for actual code)
	return TRUE; // tell Windows we handled it
}

BOOL COgreEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void COgreEditorView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();

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
}

// COgreEditorView drawing

void COgreEditorView::OnDraw(CDC* /*pDC*/)
{
	// TODO: add draw code for native data here
}

void COgreEditorView::OnDestroy()
{
	GetEditor()->Destroy();

	CView::OnDestroy();
}

// COgreEditorView diagnostics

#ifdef _DEBUG
void COgreEditorView::AssertValid() const
{
	CView::AssertValid();
}

void COgreEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

#endif //_DEBUG


// COgreEditorView message handlers

void COgreEditorView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	CRect rect; 
	GetClientRect(&rect); 

	GetEditor()->Resize( rect.Width(), rect.Height() );
}

void COgreEditorView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CView::OnKeyDown(nChar, nRepCnt, nFlags);

	GetEditor()->OnKeyDown(nChar,nRepCnt,nFlags);
}

void COgreEditorView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CView::OnKeyUp(nChar, nRepCnt, nFlags);

	GetEditor()->OnKeyUp(nChar,nRepCnt,nFlags);
}

void COgreEditorView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CView::OnLButtonDown(nFlags, point);

	GetEditor()->OnLButtonDown(nFlags,point);
}

void COgreEditorView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CView::OnLButtonUp(nFlags, point);

	GetEditor()->OnLButtonUp(nFlags,point);
}

void COgreEditorView::OnMButtonDown(UINT nFlags, CPoint point)
{
	CView::OnMButtonDown(nFlags, point);

}

void COgreEditorView::OnMButtonUp(UINT nFlags, CPoint point)
{
	CView::OnMButtonUp(nFlags, point);
}

void COgreEditorView::OnMouseMove(UINT nFlags, CPoint point)
{
	CView::OnMouseMove(nFlags, point);

	GetEditor()->OnMouseMove(nFlags,point);
}

BOOL COgreEditorView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	GetEditor()->OnMouseWheel( nFlags, zDelta, pt);

	return CView::OnMouseWheel( nFlags, zDelta, pt);
}

void COgreEditorView::OnRButtonDown(UINT nFlags, CPoint point)
{
	GetEditor()->OnRButtonDown(nFlags,point);

	CView::OnRButtonDown(nFlags, point);
}

void COgreEditorView::OnRButtonUp(UINT nFlags, CPoint point)
{
	GetEditor()->OnRButtonUp(nFlags,point);

	CView::OnRButtonUp(nFlags, point);
}

void COgreEditorView::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);
}

void COgreEditorView::OnKillFocus(CWnd* pOldWnd)
{
	CView::OnKillFocus(pOldWnd);
}