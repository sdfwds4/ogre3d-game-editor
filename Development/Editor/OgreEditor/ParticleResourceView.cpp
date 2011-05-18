

#include "stdafx.h"
#include "ParticleResourceView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParticleResView

IMPLEMENT_DYNCREATE(CParticleResView, CView)

CParticleResView::CParticleResView()
{
}

CParticleResView::~CParticleResView()
{
}

BEGIN_MESSAGE_MAP(CParticleResView, CView)
	//{{AFX_MSG_MAP(CViewOne)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewOne drawing

void CParticleResView::OnDraw(CDC* pDC)
{
	CView::OnDraw(pDC);
}

/////////////////////////////////////////////////////////////////////////////
// CParticleResView diagnostics

#ifdef _DEBUG
void CParticleResView::AssertValid() const
{
	CView::AssertValid();
}

void CParticleResView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CParticleResView message handlers

void CParticleResView::OnViewOne()
{
	// alert user.
	AfxMessageBox(_T("Options Pane - No. 1"));

	// set input focus
	SetFocus();
}

void CParticleResView::OnUpdateViewOne(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here

}
