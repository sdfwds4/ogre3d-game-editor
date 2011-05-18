

#include "stdafx.h"
//#include "ActivePaneView.h"

#include "ParticleViewDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CActivePaneViewDoc

IMPLEMENT_DYNCREATE(CParticleViewDoc, CDocument)

BEGIN_MESSAGE_MAP(CParticleViewDoc, CDocument)
	//{{AFX_MSG_MAP(CParticleViewDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CActivePaneViewDoc construction/destruction

CParticleViewDoc::CParticleViewDoc()
{
	// TODO: add one-time construction code here

}

CParticleViewDoc::~CParticleViewDoc()
{
}

BOOL CParticleViewDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CParticleViewDoc serialization

void CParticleViewDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CParticleViewDoc diagnostics

#ifdef _DEBUG
void CParticleViewDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CParticleViewDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CParticleViewDoc commands
