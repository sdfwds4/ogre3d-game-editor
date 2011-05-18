

#if !defined(AFX_VIEWTWO_H__493646AA_CE77_4375_8D95_66AF7C81CB00__INCLUDED_)
#define AFX_VIEWTWO_H__493646AA_CE77_4375_8D95_66AF7C81CB00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// CViewTwo view

class CParticlePropertyView : public CView
{
protected:
	CParticlePropertyView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CParticlePropertyView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParticlePropertyView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CParticlePropertyView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CParticlePropertyView)
	afx_msg void OnViewTwo();
	afx_msg void OnUpdateViewTwo(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWTWO_H__493646AA_CE77_4375_8D95_66AF7C81CB00__INCLUDED_)
