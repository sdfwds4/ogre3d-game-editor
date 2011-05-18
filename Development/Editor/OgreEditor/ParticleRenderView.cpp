

#include "stdafx.h"
#include "ParticleRenderView.h"
#include "ParticleDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParticleRenderView

IMPLEMENT_DYNCREATE(CParticleRenderView, CView)

BEGIN_MESSAGE_MAP(CParticleRenderView, CView)
	//{{AFX_MSG_MAP(CParticleRenderView)
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	//ON_COMMAND(ID_VIEW_THREE, OnViewThree)
	//ON_UPDATE_COMMAND_UI(ID_VIEW_THREE, OnUpdateViewThree)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
	// Standard printing commands
	//ON_COMMAND(ID_FILE_PRINT, CViewDraw::OnFilePrint)
	//ON_COMMAND(ID_FILE_PRINT_DIRECT, CViewDraw::OnFilePrint)
	//ON_COMMAND(ID_FILE_PRINT_PREVIEW, CViewDraw::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParticleRenderView construction/destruction

CParticleRenderView::CParticleRenderView()
: m_bEnable(FALSE)
{
	// TODO: add construction code here
}

CParticleRenderView::~CParticleRenderView()
{
}

BOOL CParticleRenderView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CParticleRenderView drawing

void CParticleRenderView::OnDraw(CDC* pDC)
{
	CView::OnDraw(pDC);
}

/////////////////////////////////////////////////////////////////////////////
// CParticleRenderView diagnostics

#ifdef _DEBUG
void CParticleRenderView::AssertValid() const
{
	CView::AssertValid();
}

void CParticleRenderView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CParticleRenderView message handlers

void CParticleRenderView::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp)
{
	// bypass base class.
	CView::OnNcCalcSize(bCalcValidRects, lpncsp);
}

void CParticleRenderView::OnNcPaint()
{
	// bypass base class.
	CView::OnNcPaint();
}

void CParticleRenderView::OnViewThree()
{
	// TODO: Add your command handler code here
	AfxMessageBox(_T("SDI View - No. 3"));
}

void CParticleRenderView::OnUpdateViewThree(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bEnable);
}

void CParticleRenderView::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);

	// TODO: Add your message handler code here
	m_bEnable = TRUE;
}

void CParticleRenderView::OnKillFocus(CWnd* pNewWnd)
{
	CView::OnKillFocus(pNewWnd);

	// TODO: Add your message handler code here
	m_bEnable = FALSE;
}
