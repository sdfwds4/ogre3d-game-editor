

#if !defined(AFX_PARTICLEDIALOG_H__A0A246FE_A271_44D7_BCAA_013E2C4ADE86__INCLUDED_)
#define AFX_PARTICLEDIALOG_H__A0A246FE_A271_44D7_BCAA_013E2C4ADE86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParticlePreviewDialog.h"

class CParticleResView;
class CParticlePropertyView;

class CParticleDialog : public CXTPFrameWnd
{
public: // create from serialization only
	CParticleDialog();
	DECLARE_DYNCREATE(CParticleDialog)

// Attributes
public:

// Operations
public:

	CXTPDockingPane* GetActivePane();
	CWnd* GetActivePaneView();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParticleDialog)
	public:
	virtual void OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CParticleDialog();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CXTPStatusBar           m_wndStatusBar;
	CXTPDockingPaneManager  m_paneManager;
	CParticleResView*       mpParticleResView;
	CParticlePropertyView*  mpParticlePropertyView;
	CParticlePreviewDialog  mParticlePreviewDialog;
	CXTPDockingPane*        m_pActivePane;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	afx_msg void OnCustomize();
	afx_msg LRESULT OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARTICLEDIALOG_H__A0A246FE_A271_44D7_BCAA_013E2C4ADE86__INCLUDED_)
