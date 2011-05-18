

#include "stdafx.h"
#include "OgreEditorDoc.h"
#include "BladeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// BladeView

IMPLEMENT_DYNCREATE(CBladeView, CView)

BEGIN_MESSAGE_MAP(CBladeView, CView)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CGdiView construction/destruction

CBladeView::CBladeView()
{
	// TODO: add construction code here

}

CBladeView::~CBladeView()
{

}

BOOL CBladeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void CBladeView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();

}

// CGdiView drawing

void CBladeView::OnDraw(CDC* pDC)
{
	CView::OnDraw(pDC);
	// TODO: add draw code for native data here
}

// CBladeView diagnostics

#ifdef _DEBUG
void CBladeView::AssertValid() const
{
	CView::AssertValid();
}

void CBladeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COgreEditorDoc* CBladeView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COgreEditorDoc)));
	return (COgreEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// COgreEditorView message handlers

int CBladeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	mStatusDialog.Create( CStatusDialog::IDD, this );
	mStatusDialog.ShowWindow( SW_SHOW );

	mRenderDialog.Create( CRenderDialog::IDD, this );
	mRenderDialog.ShowWindow( SW_SHOW );
	mRenderDialog.SetFocus();

	//CRuntimeClass* pNewViewClass;
	//pNewViewClass =	RUNTIME_CLASS(COgreEditorView);
	//// create the new view
	//CCreateContext context;
	//context.m_pNewViewClass = pNewViewClass;
	//mpMainView = CreateView<COgreEditorView>(pNewViewClass, CRect( 0, 0, 0, 0 ),  &context);

	//mpMainView->SetFocus();
	return 0;
}

void CBladeView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	CRect rc;
	GetClientRect(rc);
	rc.bottom -= 25;
	mRenderDialog.MoveWindow(rc, FALSE);
	mRenderDialog.SetFocus();

	GetClientRect(rc);
	rc.top = rc.bottom - 25;
	mStatusDialog.MoveWindow(rc, FALSE);
}

void CBladeView::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);
	mRenderDialog.SetFocus();
}

void CBladeView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CView::OnLButtonDown(nFlags, point);

	mRenderDialog.SetFocus();
}
