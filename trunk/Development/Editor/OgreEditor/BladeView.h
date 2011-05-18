

#pragma once

#include "RenderDialog.h"
#include "StatusDialog.h"

class CBladeView : public CView
{
protected: // create from serialization only
	CBladeView();
	DECLARE_DYNCREATE(CBladeView)

	// Attributes
public:
	COgreEditorDoc* GetDocument() const;

	// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();

protected:

	// Implementation
public:
	virtual ~CBladeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()

protected:
	CRenderDialog	mRenderDialog;
	//COgreEditorView* mpMainView;
	CStatusDialog mStatusDialog;

	template<class T>
	T* CreateView(CRuntimeClass * pViewClass, const RECT & lprect, CCreateContext * pContext)
	{
#ifdef _DEBUG
		ASSERT_VALID(this);
		ASSERT(pViewClass != NULL);
		ASSERT(pViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
		ASSERT(AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));
#endif

		BOOL bSendInitialUpdate = FALSE;

		CCreateContext contextT;
		if (pContext == NULL)
		{
			// if no context specified, generate one from the currently selected
			//  client if possible
			T* pOldView = NULL;
			if (pOldView != NULL && pOldView->IsKindOf(RUNTIME_CLASS(T)))
			{
				// set info about last pane
				ASSERT(contextT.m_pCurrentFrame == NULL);
				contextT.m_pLastView = pOldView;
				contextT.m_pCurrentDoc = pOldView->GetDocument();
				if (contextT.m_pCurrentDoc != NULL)
					contextT.m_pNewDocTemplate =
					contextT.m_pCurrentDoc->GetDocTemplate();
			}
			pContext = &contextT;
			bSendInitialUpdate = TRUE;
		}

		CWnd* pWnd;
		TRY
		{
			pWnd = (CWnd*)pViewClass->CreateObject();
			if (pWnd == NULL)
				AfxThrowMemoryException();
		}
		CATCH_ALL(e)
		{
			TRACE0("Out of memory creating a splitter pane.\n");
			// Note: DELETE_EXCEPTION(e) not required
			return (COgreEditorView*) NULL;
		}
		END_CATCH_ALL

			ASSERT_KINDOF(CWnd, pWnd);
		ASSERT(pWnd->m_hWnd == NULL);       // not yet created

		DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_BORDER ;//AFX_WS_DEFAULT_VIEW |;

		// Create with the right size (wrong position)
		CRect rect(lprect);
		if (!pWnd->Create(NULL, NULL, dwStyle,
			rect, this, 0, pContext))
		{
			TRACE0("Warning: couldn't create client pane for splitter.\n");
			// pWnd will be cleaned up by PostNcDestroy
			return (T*) NULL;
		}

		// send initial notification message
		//if (bSendInitialUpdate);
		//              pWnd->SendMessage(WM_INITIALUPDATE);
		return (T*) pWnd;
	}
};


#ifndef _DEBUG  // debug version in BladeView.cpp
inline COgreEditorDoc* CBladeView::GetDocument() const
{ return reinterpret_cast<COgreEditorDoc*>(m_pDocument); }
#endif
