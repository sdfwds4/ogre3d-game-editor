// MainFrm.h : interface of the CMainFrame class
//


#pragma once

class CMainFrame : public CXTPFrameWnd
{
	DECLARE_DYNCREATE(CMainFrame)

	// Attributes
public:
	CMainFrame();

	// Operations
public:

	// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CXTPStatusBar  m_wndStatusBar;
	CXTPDockingPaneManager m_paneManager;
	CXTPOfficeBorder<CStatic> m_wndOptions;
	CXTPOfficeBorder<CEdit> m_wndLogEdit;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnCustomize();
	afx_msg LRESULT OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
