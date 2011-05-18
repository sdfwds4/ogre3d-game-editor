// GdiView.cpp : implementation of the CGdiView class
//

#include "stdafx.h"
#include "GdiView.h"
#include "Editor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CGdiTool, CEditTool)

CGdiTool::CGdiTool() 
{

}

CGdiTool::~CGdiTool()
{

}

void CGdiTool::BeginTool()
{
	// TODO: 在此添加控件通知处理程序代码

}

void CGdiTool::EndTool()
{

}

void CGdiTool::OnPaint()
{
	CPaintDC paintDC(mpParent);
	Gdiplus::Graphics graphics(paintDC.GetSafeHdc());
	CRect rc = paintDC.m_ps.rcPaint;
	graphics.Clear(Gdiplus::Color::Gray);
}

// COgreEditorView

IMPLEMENT_DYNCREATE(CGdiView, CView)

BEGIN_MESSAGE_MAP(CGdiView, CView)
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
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CGdiView construction/destruction

CGdiView::CGdiView() : mDatabaseType(255), mpEditTool(NULL)
{
	// TODO: add construction code here

}

CGdiView::~CGdiView()
{

}

BOOL CGdiView::OnEraseBkgnd(CDC* pDC) 
{
	// "Tile" bitmap (see demo for actual code)
	return TRUE; // tell Windows we handled it
}


BOOL CGdiView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void CGdiView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();

	SetEditTool(NULL);
}

// CGdiView drawing

void CGdiView::OnDraw(CDC* pDC)
{
	CView::OnDraw(pDC);
	// TODO: add draw code for native data here
}

void CGdiView::OnDestroy()
{
	CView::OnDestroy();

	if ( mpEditTool )
	{
		mpEditTool->EndTool();
	}
}

// CGdiView diagnostics

#ifdef _DEBUG
void CGdiView::AssertValid() const
{
	CView::AssertValid();
}

void CGdiView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

#endif //_DEBUG


// COgreEditorView message handlers

void CGdiView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
}

void CGdiView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CGdiView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CView::OnKeyUp(nChar, nRepCnt, nFlags);

}

void CGdiView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CView::OnLButtonDown(nFlags, point);

	mpEditTool->OnLButtonDown(nFlags,point);
}

void CGdiView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CView::OnLButtonUp(nFlags, point);

	mpEditTool->OnLButtonUp(nFlags,point);
}

void CGdiView::OnMButtonDown(UINT nFlags, CPoint point)
{
	CView::OnMButtonDown(nFlags, point);

}

void CGdiView::OnMButtonUp(UINT nFlags, CPoint point)
{
	CView::OnMButtonUp(nFlags, point);
}


void CGdiView::OnMouseMove(UINT nFlags, CPoint point)
{
	CView::OnMouseMove(nFlags, point);

	mpEditTool->OnMouseMove(nFlags,point);
}

BOOL CGdiView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	mpEditTool->OnMouseWheel( nFlags, zDelta, pt);

	return CView::OnMouseWheel( nFlags, zDelta, pt);
}

void CGdiView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CView::OnRButtonDown(nFlags, point);

	SetFocus();

	if ( GetCapture() != this )
	{
		SetCapture();
	}

	mpEditTool->OnRButtonDown(nFlags,point);

}

void CGdiView::OnRButtonUp(UINT nFlags, CPoint point)
{
	CView::OnRButtonUp(nFlags, point);
	ReleaseCapture();

	mpEditTool->OnRButtonUp(nFlags,point);
}

void CGdiView::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);
}

void CGdiView::OnKillFocus(CWnd* pOldWnd)
{
	CView::OnKillFocus(pOldWnd);
}

void CGdiView::OnPaint()
{
	mpEditTool->OnPaint();

	CView::OnPaint();
}

void CGdiView::OnTimer(UINT_PTR nIDEvent) 
{
	mpEditTool->OnTimer(nIDEvent);

	CView::OnTimer(nIDEvent);
}

