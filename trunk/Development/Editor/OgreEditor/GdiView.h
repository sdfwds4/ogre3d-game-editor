// GdiView.h : interface of the CGdiView class
//

#pragma once

#include "EditTool.h"


//默认的画图工具
class CGdiTool : public CEditTool
{
public:
	DECLARE_DYNCREATE(CGdiTool)

	CGdiTool();
	~CGdiTool();

	void SetParent( CWnd* pWnd )
	{
		mpParent = pWnd;
	}

	void BeginTool();
	void EndTool();
	void OnPaint();

	CWnd* mpParent;
};

class CGdiView : public CView
{
protected: // create from serialization only
	CGdiView();
	DECLARE_DYNCREATE(CGdiView)

	// Operations
public:

	// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();

	void SetEditTool( CEditTool* pEditTool )
	{
		if ( !pEditTool )
		{
			if ( mpEditTool && mpEditTool->IsKindOf( RUNTIME_CLASS(CGdiTool) ) )
				return;
			else
			{
				CGdiTool* pGdiTool = new CGdiTool;
				pGdiTool->SetParent(this);
				pEditTool = pGdiTool;
			}
		}

		if ( mpEditTool != pEditTool && mpEditTool != 0 )
		{
			mpEditTool->EndTool();
		}
		mpEditTool = pEditTool;
		if( mpEditTool )
			mpEditTool->BeginTool();
	}

	CEditTool* GetEditTool()
	{
		return mpEditTool;
	}

	void SetDatabaseType( int type )
	{
		mDatabaseType = type;
	}

protected:

// Implementation
public:
	virtual ~CGdiView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CEditTool* mpEditTool;
	int mDatabaseType;

protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pOldWnd);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg	BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()
};

