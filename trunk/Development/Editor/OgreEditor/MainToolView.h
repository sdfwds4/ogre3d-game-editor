

#pragma once

// CMainToolView 窗体视图
#include "Controls/RollupCtrl.h"
#include "Editor.h"


class CMainToolView : public CXTTabView
{
	DECLARE_DYNCREATE(CMainToolView)

protected:
	CMainToolView();           // 动态创建所使用的受保护的构造函数
	virtual ~CMainToolView();

public:
	enum { IDD = IDD_MAINTOOL_VIEW };
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
	UINT				m_nIDEvent;
	int					m_iHitTest;
	CImageList			m_imageList;
	CPoint				m_point;

	CRollupCtrl m_RollupCtrl[E_ROLLUP_CTRL_NUM];
	// Implementation
public:
	void UpdateTabBorders();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

protected:

	virtual void OnSelChanging();
	virtual void OnSelChange();
	void SetTabIcon(int iTab, int iImage);
	void SetTabIcon(int iTab, HICON hIcon);

	// Generated message map functions
protected:
	//{{AFX_MSG(CTabbedViewView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFileOpen();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCancelMode();
	afx_msg void OnTabActivate();
	afx_msg void OnTabClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	//}}AFX_MSG
	afx_msg void OnCloseTab();
	DECLARE_MESSAGE_MAP()
};


