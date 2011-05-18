

#include "stdafx.h"
#include "ParticlepropertyView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParticleProperty

IMPLEMENT_DYNCREATE(CParticlePropertyView, CView)

CParticlePropertyView::CParticlePropertyView()
{
}

CParticlePropertyView::~CParticlePropertyView()
{
}

BEGIN_MESSAGE_MAP(CParticlePropertyView, CView)
	//{{AFX_MSG_MAP(CParticlePropertyView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParticleProperty drawing

void CParticlePropertyView::OnDraw(CDC* pDC)
{
	CView::OnDraw(pDC);
}

/////////////////////////////////////////////////////////////////////////////
// CParticleProperty diagnostics

#ifdef _DEBUG
void CParticlePropertyView::AssertValid() const
{
	CView::AssertValid();
}

void CParticlePropertyView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CParticlePropertyView message handlers

void CParticlePropertyView::OnViewTwo()
{
	// alert user.
	AfxMessageBox(_T("Properties Pane - No. 2"));

	// set input focus
	SetFocus();
}

void CParticlePropertyView::OnUpdateViewTwo(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here

}
