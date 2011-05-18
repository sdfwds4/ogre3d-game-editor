// MainToolView.cpp : 实现文件
//

#include "stdafx.h"
#include "OgreEditor.h"
#include "MainToolView.h"
#include "ObjectDialog.h"
#include "TerrainDialog.h"
#include "DisplayDialog.h"
#include "LayerDialog.h"

// CMainToolView

IMPLEMENT_DYNCREATE(CMainToolView, CXTTabView)

CMainToolView::CMainToolView()
{
	// TODO: add construction code here
	m_iHitTest = -1;
	m_nIDEvent = 20;

	m_bXPBorder = true;
}

CMainToolView::~CMainToolView()
{
}

BOOL CMainToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CXTTabView::PreCreateWindow(cs);
}

void CMainToolView::DoDataExchange(CDataExchange* pDX)
{
	CXTTabView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMainToolView, CXTTabView)
	//{{AFX_MSG_MAP(CTabbedViewView)
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	////}}AFX_MSG_MAP
	//ON_COMMAND(XT_IDC_TAB_CLOSE, OnCloseTab)
END_MESSAGE_MAP()


// CMainToolView 诊断

#ifdef _DEBUG
void CMainToolView::AssertValid() const
{
	CXTTabView::AssertValid();
}

#ifndef _WIN32_WCE
void CMainToolView::Dump(CDumpContext& dc) const
{
	CXTTabView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMainToolView 消息处理程序


/////////////////////////////////////////////////////////////////////////////
// CMainToolView message handlers

int CMainToolView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CXTTabView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//// Register as a drop target
	//m_dropTarget.Register(this);
	//DragAcceptFiles();

	for ( int i = 0; i < E_ROLLUP_CTRL_NUM; i++ )
	{
		GetEditor()->SetRollupCtrl( (E_ROLLUP_CTRLS)i, &m_RollupCtrl[i]);
	}

	// Create the image list used by the tab control.
	if (!m_imageList.Create( IDB_IMAGELIST, 16, 1, RGB( 0x00,0xFF,0x00 )))
	{
		TRACE0("Failed to create image list.\n");
		return -1;
	}

	if (!m_RollupCtrl[E_OBJECT_CTRL].Create(WS_VISIBLE|WS_CHILD, CRect(0,0,0,0), this, 1))
	{
		TRACE0("Failed to create Object control.\n");
		return -1;
	}

	int id = m_RollupCtrl[E_OBJECT_CTRL].InsertPage("对象分类", IDD_OBJECTS, RUNTIME_CLASS(CObjectDialog), 1);
	m_RollupCtrl[E_OBJECT_CTRL].ExpandPage(id);

	if (!m_RollupCtrl[E_TERRAIN_CTRL].Create(WS_VISIBLE|WS_CHILD, CRect(0,0,0,0), this, 2))
	{
		TRACE0("Failed to create Terrain control.\n");
		return -1;
	}

	id = m_RollupCtrl[E_TERRAIN_CTRL].InsertPage("地形编辑", IDD_TERRAIN, RUNTIME_CLASS(CTerrainDialog), 1);
	m_RollupCtrl[E_TERRAIN_CTRL].ExpandPage(id);

	if (!m_RollupCtrl[E_DISPLAY_CTRL].Create(WS_VISIBLE|WS_CHILD, CRect(0,0,0,0), this, 3))
	{
		TRACE0("Failed to create Display control.\n");
		return -1;
	}

	id = m_RollupCtrl[E_DISPLAY_CTRL].InsertPage("显示对象", IDD_DISPLAY, RUNTIME_CLASS(CDisplayDialog), 1);
	m_RollupCtrl[E_DISPLAY_CTRL].ExpandPage(id);

	if (!m_RollupCtrl[E_LAYER_CTRL].Create(WS_VISIBLE|WS_CHILD, CRect(0,0,0,0), this, 4))
	{
		TRACE0("Failed to create Layer control.\n");
		return -1;
	}

	id = m_RollupCtrl[E_LAYER_CTRL].InsertPage("层级列表", IDD_LAYER,	RUNTIME_CLASS(CLayerDialog), 1);
	m_RollupCtrl[E_LAYER_CTRL].ExpandPage(id);


	AddControl(_T("对象"), &m_RollupCtrl[E_OBJECT_CTRL]);
	AddControl(_T("地形"), &m_RollupCtrl[E_TERRAIN_CTRL]);
	AddControl(_T("显示"), &m_RollupCtrl[E_DISPLAY_CTRL]);
	AddControl(_T("层级"), &m_RollupCtrl[E_LAYER_CTRL]);

	// Set the tab controls image list.
	GetTabCtrl().SetImageList(&m_imageList);

	// Set the active view to the second tab.
	SetActiveView(0);
	return 0;
}


void CMainToolView::SetTabIcon(int iTab, int iImage)
{
	TC_ITEM tci;
	tci.mask = TCIF_IMAGE;

	GetTabCtrl().GetItem(iTab, &tci);
	tci.iImage = iImage;
	GetTabCtrl().SetItem(iTab, &tci);
}

void CMainToolView::SetTabIcon(int iTab, HICON hIcon)
{
	CImageList* pImageList = GetTabCtrl().GetImageList();
	SetTabIcon(iTab, pImageList->Add(hIcon));
}

void CMainToolView::OnSelChanging()
{
	CXTTabView::OnSelChanging();

	// TODO: Add your code to handle tab selection.

}

void CMainToolView::OnSelChange()
{
	CXTTabView::OnSelChange();

	// TODO: Add your code to handle tab selection.

}

void CMainToolView::OnInitialUpdate() 
{
	CXTTabView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class

}

void CMainToolView::OnFileOpen() 
{
	// TODO: Add your command handler code here

}

void CMainToolView::OnRButtonDown(UINT /*nFlags*/, CPoint point) 
{
	// Get the tab index based upon the cursor position.
	m_iHitTest = GetTabFromPoint(point);
	if (m_iHitTest == -1)
		return;

	//CMenu popupMenu;
	//VERIFY(popupMenu.CreatePopupMenu());
	//popupMenu.AppendMenu(MF_STRING, ID_TAB_ACTIVATE, _T("Active View Tab"));
	//popupMenu.AppendMenu(MF_SEPARATOR);
	//popupMenu.AppendMenu(MF_STRING, ID_TAB_CLOSE, _T("&Close"));
	//popupMenu.AppendMenu(MF_STRING, ID_TAB_SAVE, _T("&Save\tCtrl+S"));
	//popupMenu.AppendMenu(MF_STRING, ID_TAB_SAVE_AS, _T("Save &As..."));
	//popupMenu.AppendMenu(MF_SEPARATOR);
	//popupMenu.AppendMenu(MF_STRING, ID_TAB_PRINT, _T("&Print\tCtrl+P"));
	//popupMenu.AppendMenu(MF_STRING, ID_TAB_PRINT_PREVIEW, _T("Print Pre&view"));

	//::SetMenuDefaultItem(popupMenu.m_hMenu, 0, TRUE);

	//CPoint pt = point;
	//ClientToScreen(&pt);
	//popupMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
	//	pt.x, pt.y, this);

	//popupMenu.DestroyMenu();
}

void CMainToolView::OnCancelMode() 
{
	CXTTabView::OnCancelMode();

	// TODO: Add your message handler code here

}

void CMainToolView::OnTabActivate()
{
	//	TRACE0("CTabbedViewView::OnTabActivate()\n");
	SetActiveView(m_iHitTest);
}

void CMainToolView::OnTabClose()
{
	//	TRACE0("CTabbedViewView::OnTabClose()\n");
	//if (GetTabCtrl().GetItemCount() == 1)
	//{
	//	AfxMessageBox( IDS_CANNOTDEL );
	//}
	//else
	//{
	//	DeleteView(m_iHitTest);
	//}
}

void CMainToolView::OnTimer(UINT_PTR nIDEvent) 
{
	if (m_nIDEvent == nIDEvent)
	{
		CPoint point;
		::GetCursorPos(&point);
		ScreenToClient(&point);

		if (m_point == point)
		{
			CTabCtrl& tabCtrl = GetTabCtrl();

			CRect rcItem;
			int iItem;
			for (iItem = 0; iItem < tabCtrl.GetItemCount(); ++iItem )
			{
				tabCtrl.GetItemRect(iItem, &rcItem);

				if (rcItem.PtInRect(m_point) && tabCtrl.GetCurSel() != iItem)
				{
					SetActiveView(iItem);
					break;
				}
			}
		}

		KillTimer(m_nIDEvent);
	}
	else
	{
		CXTTabView::OnTimer(nIDEvent);
	}
}

void CMainToolView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CXTTabView::OnMouseMove(nFlags, point);

	m_point = point;
	SetTimer(m_nIDEvent, 2500, NULL);
}

void CMainToolView::UpdateTabBorders()
{
	BOOL bIsOfficeTheme = XTThemeManager()->GetTheme() != xtThemeOffice2000;

	DWORD dwAdd    = bIsOfficeTheme ? 0 : WS_EX_CLIENTEDGE;
	DWORD dwRemove = bIsOfficeTheme ? WS_EX_CLIENTEDGE : 0;


	for ( int i = 0; i < E_ROLLUP_CTRL_NUM; i++ )
	{
		if (::IsWindow(m_RollupCtrl[i].m_hWnd)) 
		{
			m_RollupCtrl[i].ModifyStyleEx(dwRemove, dwAdd);
		}
	}

	CRect r;
	GetWindowRect(&r);

	SetWindowPos(NULL, 0,0,r.Width()+1,r.Height(),
		SWP_FRAMECHANGED|SWP_NOMOVE);

	SetWindowPos(NULL, 0,0,r.Width(),r.Height(),
		SWP_FRAMECHANGED|SWP_NOMOVE);
}

void CMainToolView::OnCloseTab()
{
	//if (GetTabCtrl().GetItemCount() == 1)
	//{
	//	AfxMessageBox( IDS_CANNOTDEL );
	//}
	//else
	//{
	//	DeleteView(GetTabCtrl().GetCurSel());
	//}
}

DROPEFFECT CMainToolView::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
	::GetCursorPos(&point);
	ScreenToClient(&point);

	CTabCtrl& tabCtrl = GetTabCtrl();

	CRect rcItem;
	for (int iItem = 0; iItem < tabCtrl.GetItemCount(); ++iItem )
	{
		tabCtrl.GetItemRect(iItem, &rcItem);
		if (rcItem.PtInRect(point) && tabCtrl.GetCurSel() != iItem)
		{
			SetActiveView(iItem);
			break;
		}
	}

	return CXTTabView::OnDragOver(pDataObject, dwKeyState, point);
}

void CMainToolView::OnDropFiles(HDROP hDropInfo) 
{
	CPoint point;
	::GetCursorPos(&point);
	ScreenToClient(&point);

	CTabCtrl& tabCtrl = GetTabCtrl();

	CRect rcItem;
	for (int iItem = 0; iItem < tabCtrl.GetItemCount(); ++iItem )
	{
		tabCtrl.GetItemRect(iItem, &rcItem);
		if (rcItem.PtInRect(point))
		{
			SetActiveView(iItem);
			CWnd* pView = GetView(iItem);
			if (pView && ::IsWindow(pView->m_hWnd))
				pView->PostMessage(WM_DROPFILES, (WPARAM)hDropInfo, 0L);
			break;
		}
	}

	CXTTabView::OnDropFiles(hDropInfo);
}


