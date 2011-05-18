

#if !defined(AFX_VIEWONE_H__3BE9D49F_0459_4C89_AFFE_035EE2A7E564__INCLUDED_)
#define AFX_VIEWONE_H__3BE9D49F_0459_4C89_AFFE_035EE2A7E564__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// CViewOne view

class CParticleResView : public CView
{
protected:
	CParticleResView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CParticleResView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewOne)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CParticleResView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CViewOne)
	afx_msg void OnViewOne();
	afx_msg void OnUpdateViewOne(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWONE_H__3BE9D49F_0459_4C89_AFFE_035EE2A7E564__INCLUDED_)
