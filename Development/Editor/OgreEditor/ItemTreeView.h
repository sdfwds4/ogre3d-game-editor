

#if !defined(AFX_ITEMTREEVIEW_H__070E3271_48AD_11D4_82DA_DA5C3BCA3D37__INCLUDED_)
#define AFX_ITEMTREEVIEW_H__070E3271_48AD_11D4_82DA_DA5C3BCA3D37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GdiView.h"

/////////////////////////////////////////////////////////////////////////////
// CItemTreeView view
class CItemTreeView : public CXTPOfficeBorder<CView>
{
protected:
	CItemTreeView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CItemTreeView)

// Attributes
protected:

	CXTCaption  m_wndCaption;
	CImageList  m_ilTreeIcons;
	CXTTreeCtrl m_wndTreeCtrl;
	CGdiView*		 mpGdiView;

public:

	BOOL InitializeImageLists();
	BOOL InitilalizeTreeControl();
	void UpdateTree( LPCTSTR lpszText );
	HTREEITEM CreateSceneTree();
	void UpdateSceneNodeTree( Ogre::Node* node, HTREEITEM treeitem );

	CXTTreeCtrl& GetTreeCtrl() 
	{
		return m_wndTreeCtrl;
	}

	void SetGdiView(  CGdiView*  gdiView )
	{
		mpGdiView = gdiView;
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CItemTreeView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CItemTreeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV Ö§³Ö
	//{{AFX_MSG(CItemTreeView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnCaptButton();
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSysColorChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	friend class CDatabaseDialog;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ITEMTREEVIEW_H__070E3271_48AD_11D4_82DA_DA5C3BCA3D37__INCLUDED_)
