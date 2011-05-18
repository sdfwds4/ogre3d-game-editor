

#include "stdafx.h"
#include "resource.h"
#include "ItemTreeView.h"
#include "databasedialog.h"
#include "GdiView.h"
#include "Editor.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CDatabaseDialog, CFrameWnd)

BEGIN_MESSAGE_MAP(CDatabaseDialog, CFrameWnd)
	//{{AFX_MSG_MAP(CDatabaseDialog)
	ON_WM_CREATE()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_CLOSE()

	//}}AFX_MSG_MAP

	ON_MESSAGE(XTPWM_TASKPANEL_NOTIFY, OnOutbarNotify)
	//ON_MESSAGE_VOID(CPWN_XT_PUSHPINBUTTON, OnPushPinButton)
	//ON_MESSAGE_VOID(CPWN_XT_PUSHPINCANCEL, OnPushPinCancel)

	//ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)
	ON_XTP_CREATECONTROL()

	//ON_XTP_EXECUTE(ID_GOTO_URL, OnGotoURL)

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

typedef enum
{
	IDX_MATERIAL,
	IDX_TEXTURE,
	IDX_MODEL,
	IDX_SCENENODE,
	IDX_COUNT
} INDEX;

typedef struct
{
	INDEX   m_nIndex;
	UINT    m_nTextID;
	UINT    m_nIconID;
}
FOLDER_ENTRY;

typedef struct
{
	UINT            m_nTextID;
	UINT            m_nEntries;
	FOLDER_ENTRY*   m_pEntries;
}
FOLDER_DEF;

static UINT arHiddenCmds[] =
{
	ID_FILE_SAVE_AS/*,
	ID_FILE_PRINT_PREVIEW,
	ID_EDIT_CUT,
	ID_EDIT_COPY,
	ID_EDIT_PASTE,
	ID_VIEW_STATUS_BAR,
	ID_NEW_FOLDER,
	ID_NEW_OUTLOOKBARSHORTCUT,
	ID_NEW_TASK,
	ID_NEW_JOURNALENTRY,
	ID_NEW_OFFICEDOCUMENT,
	ID_NEW_PERSONALFOLDERSFILEPST*/
};

static FOLDER_ENTRY OtherShortcuts[] =
{
	{ IDX_MATERIAL,        IDS_MATERIAL,      IDI_MATERIAL       },
	{ IDX_TEXTURE,         IDS_TEXTURE,       IDI_TEXTURE        },
	{ IDX_MODEL,		   IDS_MODEL,		  IDI_MODEL		     },
	{ IDX_SCENENODE,       IDS_SCENENODE,     IDI_SCENENODE      }
};

static FOLDER_DEF FolderDefs[] =
{
	//{ IDS_OUTLOOK_SHORTCUTS,    _countof(OutlookShortcuts), OutlookShortcuts    },
	//{ IDS_MY_SHORTCUTS,         _countof(MyShortcuts),      MyShortcuts         },
	{ IDS_RESOURCES,      _countof(OtherShortcuts),   OtherShortcuts      }
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CDatabaseDialog::CDatabaseDialog()
{
	// TODO: add member initialization code here
	m_pItemTreeView = NULL;
	m_bShowPreview = TRUE;
	m_pwndTreeCtrl = NULL;

	// Enable/Disable XP GUI Mode
	CXTRegistryManager regMgr;
	m_nTheme = (XTPPaintTheme)regMgr.GetProfileInt(_T("Settings"), _T("Theme"), xtpThemeOffice2003);

#ifdef _XTP_INCLUDE_SKINFRAMEWORK
	XTPSkinManager()->SetApplyOptions(XTPSkinManager()->GetApplyOptions() | xtpSkinApplyMetrics);
#endif
}

CDatabaseDialog::~CDatabaseDialog()
{
	CXTRegistryManager regMgr;
	regMgr.WriteProfileInt(_T("Settings"), _T("Theme"), m_nTheme);

	GetEditor()->DestroyDatabaseDialog();
}

int CDatabaseDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	// TODO: Remove this lihe if you don't want intelligent menus.


	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	//if (!InitCommandBars())
	//{
	//	return -1;      // fail to create
	//}

	//CXTPCommandBars* pCommandBars = GetCommandBars();

	//pCommandBars->SetMenu(_T("Menu Bar"), IDR_MAINFRAME);

	//CXTPToolBar* pStandardBar = (CXTPToolBar*)pCommandBars->Add(_T("Standard"), xtpBarTop);
	//if (!pStandardBar ||
	//	!pStandardBar->LoadToolBar(IDR_STANDARD))
	//{
	//	TRACE0("Failed to create toolbar\n");
	//	return -1;
	//}

	//CXTPToolBar* pSkinBar = (CXTPToolBar*)pCommandBars->Add(_T("Skin Framework"), xtpBarTop);
	//if (!pSkinBar ||
	//	!pSkinBar->LoadToolBar(IDR_SKINBAR))
	//{
	//	TRACE0("Failed to create toolbar\n");
	//	return -1;
	//}

	//DockRightOf(pSkinBar, pStandardBar);

	//CXTPCommandBarsOptions* pOptions = pCommandBars->GetCommandBarsOptions();
	//pOptions->bAlwaysShowFullMenus = FALSE;
	//pCommandBars->HideCommands(arHiddenCmds, _countof(arHiddenCmds));


	//LoadCommandBars(_T("CommandBars4"));


	//SetTheme(m_nTheme);

	//if (XTPImageManager()->IsAlphaIconsSupported())
	//{	
	//	XTPImageManager()->SetIcons(IDR_STANDARD, IDB_STANDARD_ALPHA);
	//	XTPImageManager()->SetIcons(IDR_NEW_POPUP, IDB_POPUP_ALPHA);
	//	XTPImageManager()->SetIcons(IDR_MAINFRAME, IDB_TOOLBAR_ALPHA);
	//}
	//else
	//{
	//	XTPImageManager()->SetIcons(IDR_NEW_POPUP);
	//	XTPImageManager()->SetIcons(IDR_MAINFRAME);
	//}

	return 0;
}

BOOL CDatabaseDialog::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.lpszClass = _T("XTPMainFrame");
	CXTPDrawHelpers::RegisterWndClass(AfxGetInstanceHandle(), cs.lpszClass, 
		CS_DBLCLKS, AfxGetApp()->LoadIcon( IDI_DATABASE ));


	cs.style &= ~FWS_ADDTOTITLE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CDatabaseDialog::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CDatabaseDialog::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDatabaseDialog message handlers

/*CR
	|- m_wndSplitter1 --|---------------------------------------------------------------|
	|                   |   |- m_wndSplitter2 --------------------------------------|   |
	|                   |   |                                                       |   |
	|                   |   |                   CaptionBar                          |   |
	|                   |   |                                                       |   |
	|                   |   |-------------------------------------------------------|   |
	|                   |   |   |- m_wndSplitter3 --|---------------------------|   |   |
	|                   |   |   |                   |   |- m_wndSplitter4 --|   |   |   |
	|                   |   |   |                   |   |                   |   |   |   |
	|                   |   |   |                   |   |                   |   |   |   |
	|                   |   |   |                   |   |    TeamUpView     |   |   |   |
	|     OutlookBar    |   |   |                   |   |                   |   |   |   |
	|                   |   |   |                   |   |                   |   |   |   |
	|                   |   |   |   TreeItemView    |   |-------------------|   |   |   |
	|                   |   |   |                   |   |                   |   |   |   |
	|                   |   |   |                   |   |                   |   |   |   |
	|                   |   |   |                   |   |    PreviewPane    |   |   |   |
	|                   |   |   |                   |   |                   |   |   |   |
	|                   |   |   |                   |   |                   |   |   |   |
	|                   |   |   |                   |   |-------------------|   |   |   |
	|                   |   |   |-------------------|---------------------------|   |   |
	|                   |   |-------------------------------------------------------|   |
	|-------------------|---------------------------------------------------------------|
***/

BOOL CDatabaseDialog::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
	// Create the splitter window with two columns
	if (!m_wndSplitter1.CreateStatic(this, 1, 2))
	{
		TRACE0("Failed to create splitter window\n");
		return FALSE;
	}

	// Create the Outlook bar, this will be inserted
	// into row 0, col 0.
	if (!InitializeOutlookBar())
		return FALSE;

	// Create the first nested splitter window to contain the
	// caption bar and a second nested splitter.
	if (!m_wndSplitter2.CreateStatic(&m_wndSplitter1,   // our parent window is the first splitter
		2, 1,                                           // the new splitter is 2 rows, 1 column
		WS_CHILD | WS_VISIBLE | WS_BORDER,              // style, WS_BORDER is needed
		m_wndSplitter1.IdFromRowCol(0, 1)))             // new splitter is in the first row, 3rd column of first splitter
	{
		TRACE0("Failed to create nested splitter\n");
		return FALSE;
	}

	// Create the caption bar, this will be inserted
	// into row 0, col 0 of nested splitter.
	if (!InitializeCaptionBar())
		return FALSE;

	// Second nested splitter contains tree view and a third
	// nested splitter.
	if (!m_wndSplitter3.CreateStatic(&m_wndSplitter2,   // our parent window is the first splitter
		1, 2,                                           // the new splitter is 2 rows, 1 column
		WS_CHILD | WS_VISIBLE | WS_BORDER,              // style, WS_BORDER is needed
		m_wndSplitter2.IdFromRowCol(1, 0)))             // new splitter is in the first row, 3rd column of first splitter
	{
		TRACE0("Failed to create nested splitter\n");
		return FALSE;
	}

	// add the tree view.
	if (!m_wndSplitter3.CreateView(0, 0, RUNTIME_CLASS(CItemTreeView),
		CSize(175, 0), pContext))
	{
		TRACE0("Failed to create CContainerView\n");
		return FALSE;
	}

	// Initialize m_pwndTreeCtrl to point to the tree control in CItemTreeView
	m_pItemTreeView = (CItemTreeView*)m_wndSplitter3.GetPane(0,0);
	ASSERT_VALID(m_pItemTreeView);

	m_pwndTreeCtrl = &m_pItemTreeView->m_wndTreeCtrl;
	ASSERT_VALID(m_pwndTreeCtrl);

	if (!m_wndSplitter3.CreateView(0, 1, RUNTIME_CLASS(CGdiView),
		CSize(175, 0), pContext))
	{
		TRACE0("Failed to create CContainerView\n");
		return FALSE;
	}

	m_pItemTreeView->SetGdiView( (CGdiView*)m_wndSplitter3.GetPane(0,1) );

	//// Third nested splitter contains list view and preview
	//// window.
	//if (!m_wndSplitter4.CreateStatic(&m_wndSplitter3,   // our parent window is the first splitter
	//	2, 1,                                           // the new splitter is 2 rows, 1 column
	//	WS_CHILD | WS_VISIBLE | WS_BORDER,              // style, WS_BORDER is needed
	//	m_wndSplitter3.IdFromRowCol(0, 1)))             // new splitter is in the first row, 3rd column of first splitter
	//{
	//	TRACE0("Failed to create nested splitter\n");
	//	return FALSE;
	//}

	//if (!m_wndSplitter4.CreateView(0, 0, RUNTIME_CLASS(CGUI_OutlookView),
	//	CSize(0, 200), pContext))
	//{
	//	TRACE0("Failed to create CContainerView\n");
	//	return FALSE;
	//}

	//if (!m_wndSplitter4.CreateView(1, 0, RUNTIME_CLASS(CPreviewPane),
	//	CSize(0, 0), pContext))
	//{
	//	TRACE0("Failed to create CContainerView\n");
	//	return FALSE;
	//}

	m_wndSplitter1.RecalcLayout();
	m_wndSplitter2.RecalcLayout();
	m_wndSplitter3.RecalcLayout();
	//m_wndSplitter4.RecalcLayout();

	m_wndSplitter2.SetSplitterStyle(XT_SPLIT_NOFULLDRAG | XT_SPLIT_NOSIZE | XT_SPLIT_NOBORDER);

	if (m_pwndTreeCtrl)
	{
		m_pwndTreeCtrl->SetFocus();
	}

	return TRUE;
}

BOOL CDatabaseDialog::InitializeOutlookBar()
{

	// Create the outbar control, set the id to the first row/col.
	if (!m_wndOutlookBar.Create(WS_CHILD|WS_VISIBLE, CRect(0,0,0,0),
		&m_wndSplitter1, m_wndSplitter1.IdFromRowCol(0, 0)))
	{
		TRACE0("Failed to create outlook bar.");
		return FALSE;
	}

	// Tell the control to send message to this window (the mainframe)
	// and not to its real parent (the splitter)
	m_wndOutlookBar.SetOwner(this);

	 //Add icons to the large and small image lists.
	int nFolder;

	m_wndOutlookBar.SetIconSize(CSize(32, 32));

	// Add folders to the outlook bar. Input values are folder text
	// and lParam value.
	for (nFolder = 0; nFolder < _countof(FolderDefs); nFolder++)
	{
		// Get pointer to folder definition
		FOLDER_DEF* pFolder = &FolderDefs[nFolder];

		// Load folder label and add folder
		CString str;
		str.LoadString (pFolder->m_nTextID);

		CXTPTaskPanelGroup* pGroup = m_wndOutlookBar.AddGroup(pFolder->m_nTextID);

		// Add folder entries
		UINT nEntry;
		for (nEntry = 0; nEntry < pFolder->m_nEntries; nEntry++)
		{
			// Get pointer to entry, load text and add entry
			FOLDER_ENTRY* pDef = &pFolder->m_pEntries[nEntry];

			str.LoadString (pDef->m_nTextID);

			pGroup->AddLinkItem(pDef->m_nTextID, pDef->m_nIndex);			
			m_wndOutlookBar.GetImageManager()->SetIcon(pDef->m_nIconID, pDef->m_nIndex);
		}
	}

	m_wndOutlookBar.SetBehaviour(xtpTaskPanelBehaviourList);
	m_wndOutlookBar.SetSelectItemOnFocus(TRUE);

	// Select the first folder.
	m_wndOutlookBar.GetAt(0)->SetExpanded(TRUE);
	//m_wndOutlookBar.AllowDrag(TRUE);

	// Sizing for splitter
	CRect r;
	GetClientRect(&r);
	m_wndSplitter1.SetColumnInfo( 0, r.Width()/7, 0 );
	m_wndSplitter1.SetColumnInfo( 1, r.Width()/5, 0 );


	return TRUE;
}

BOOL CDatabaseDialog::InitializeCaptionBar()
{
	if (!m_wndCaption.Create(&m_wndSplitter2, _T("×ÊÔ´"), NULL,
		WS_VISIBLE|SS_CENTER|SS_CENTERIMAGE, CRect(0,0,0,0), m_wndSplitter2.IdFromRowCol(0, 0)))
	{
		TRACE0("Failed to caption window.\n");
		return FALSE;
	}

	HICON hIcon = AfxGetApp()->LoadIcon(IDI_DATABASE);

	// set the caption colors.
	m_wndCaption.SetCaptionColors (GetXtremeColor(COLOR_3DFACE),
		GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_WINDOW) );

	LOGFONT lf;
	XTAuxData().fontBold.GetLogFont(&lf);
	lf.lfHeight = 18;
	m_fontCaption.CreateFontIndirect(&lf);

	// set the font and initial icon.
	m_wndCaption.ModifyCaptionStyle(0,
		&m_fontCaption, NULL, hIcon);

	// Sizing for splitter
	m_wndSplitter2.SetRowInfo (0, 22, 0);

	return TRUE;
}

LRESULT CDatabaseDialog::OnOutbarNotify(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case XTP_TPN_CLICK:
		{
			CXTPTaskPanelGroupItem* pItem = (CXTPTaskPanelGroupItem*)lParam;
			m_wndCaption.UpdateCaption( pItem->GetCaption(), NULL);
			m_pItemTreeView->UpdateTree( pItem->GetCaption() );
		}
		break;

	case XTP_TPN_RCLICK:
		OnOutbarRButtonDown((CXTPTaskPanelItem*)lParam);
		return TRUE;
	}

	return 0;
}

void CDatabaseDialog::OnRenameItem(CXTPTaskPanelItem* pItem)
{
	CXTPTaskPanelGroup* pGroup = pItem->GetItemGroup();

	if (pGroup->GetItemLayout() == xtpTaskItemLayoutImages)
		pGroup->SetItemLayout(xtpTaskItemLayoutDefault);

	m_wndOutlookBar.RenameItem(pItem);
}

void CDatabaseDialog::OnOutbarRButtonDown(CXTPTaskPanelItem* pItem)
{
	CPoint pt;
	GetCursorPos(&pt);

	CXTPTaskPanelGroup* pGroup = pItem->GetItemGroup();
	BOOL bGroup = pItem->GetType() == xtpTaskItemTypeGroup;


	ASSERT(pGroup);

	//CMenu menu;
	//menu.LoadMenu(IDR_POPUP_TOOLBOX);

	//CMenu* pMenuPopup = menu.GetSubMenu(bGroup? 1: 0);

	//if (pGroup->GetItemLayout() != xtpTaskItemLayoutImagesWithText)
	//	pMenuPopup->CheckMenuItem(ID_TOOLBOX_LISTVIEW, MF_CHECKED|MF_BYCOMMAND);

	//CXTPTaskPanelItems* pItems = pItem->GetParentItems();

	//if (pItem->GetIndex() == 0)
	//	pMenuPopup->EnableMenuItem(ID_TOOLBOX_MOVEUP, MF_DISABLED|MF_BYCOMMAND);

	//if (pItem->GetIndex() == pItems->GetCount() - 1)
	//	pMenuPopup->EnableMenuItem(ID_TOOLBOX_MOVEDOWN, MF_DISABLED|MF_BYCOMMAND);

	//if (!::IsClipboardFormatAvailable(CXTPTaskPanel::GetClipboardFormat()))
	//	pMenuPopup->EnableMenuItem(ID_EDIT_PASTE, MF_DISABLED|MF_BYCOMMAND);


	//int nReturn = CXTPCommandBars::TrackPopupMenu(pMenuPopup, TPM_LEFTALIGN|TPM_RETURNCMD|TPM_NONOTIFY, pt.x, pt.y, this);

	//switch (nReturn)
	//{
	//	case ID_TOOLBOX_RENAME:
	//		OnRenameItem(pItem);
	//		break;

	//	case ID_TOOLBOX_ADDTAB:
	//		{
	//			CXTPTaskPanelGroup* pGroup = m_wndOutlookBar.AddGroup(0);
	//			m_wndOutlookBar.RenameItem(pGroup);
	//		}
	//		break;

	//	case ID_TOOLBOX_LISTVIEW:
	//		if (pGroup->GetItemLayout() == xtpTaskItemLayoutImagesWithText)
	//		{
	//			pGroup->SetItemLayout(xtpTaskItemLayoutDefault);
	//			pGroup->SetIconSize(CSize(32, 32));
	//		}
	//		else
	//		{
	//			pGroup->SetItemLayout(xtpTaskItemLayoutImagesWithText);
	//			pGroup->SetIconSize(CSize(16, 16));
	//		}
	//		break;

	//	case ID_TOOLBOX_MOVEDOWN:
	//	case ID_TOOLBOX_MOVEUP:
	//		{
	//			CXTPTaskPanelItems* pItems = pItem->GetParentItems();
	//			pItems->Move(pItem, pItem->GetIndex() + (nReturn == ID_TOOLBOX_MOVEDOWN? +1: -1));
	//		}
	//		break;

	//	case ID_EDIT_COPY:
	//		pItem->CopyToClipboard();
	//		break;

	//	case ID_EDIT_PASTE:
	//		{
	//			CXTPTaskPanelItem* pItem = CXTPTaskPanelItem::PasteFromClipboard();
	//			if (pItem)
	//			{
	//				CXTPTaskPanelItems* pItems = pGroup->GetItems();
	//				pItems->InsertAt(pItems->GetCount(), pItem);
	//			}
	//		}
	//		break;

	//	case ID_EDIT_CUT:
	//		pItem->CopyToClipboard();

	//	case ID_TOOLBOX_DELETE:
	//		pItem->Remove();
	//		break;
	//}
}

void CDatabaseDialog::CloseTreeViewPane()
{
	ASSERT_VALID(this);
	m_wndCaption.SetChildWindow (m_pwndTreeCtrl, this);
	m_wndSplitter3.HideColumn(0);
}

void CDatabaseDialog::OnPushPinButton()
{
	m_wndSplitter3.ShowColumn();
}

void CDatabaseDialog::OnPushPinCancel()
{
	// TODO: Add your message handler code here and/or call default

}

void CDatabaseDialog::OnDoNothing()
{
	// TODO: Add your command handler code here

}

void CDatabaseDialog::OnFilePreviewpane()
{
	m_bShowPreview = !m_bShowPreview;

	//if (m_bShowPreview) {
	//	m_wndSplitter4.ShowRow();

	//}
	//else {
	//	m_wndSplitter4.HideRow(1);
	//}
}

void CDatabaseDialog::OnUpdateFilePreviewpane(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_bShowPreview);
}

void CDatabaseDialog::UpdateCaption(LPCTSTR lpszText, HICON hIcon)
{
	m_wndCaption.UpdateCaption(lpszText, hIcon);
}

void CDatabaseDialog::OnSysColorChange()
{
	CFrameWnd::OnSysColorChange();

	// update toolkit colors.
	XTPColorManager()->RefreshColors();
	XTThemeManager()->RefreshMetrics();

	SetTheme(m_nTheme);
}


BOOL CDatabaseDialog::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext)
{
	if ( !CFrameWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext) )
	{
		return FALSE;
	}


	return TRUE;
}

int CDatabaseDialog::OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl)
{
	//if (lpCreateControl->bToolBar && lpCreateControl->nID == ID_FILE_NEWMAIL)
	//{
	//	lpCreateControl->controlType = xtpControlSplitButtonPopup;
	//	lpCreateControl->buttonStyle = xtpButtonIconAndCaption;
	//	return TRUE;
	//}

	//if (lpCreateControl->bToolBar && lpCreateControl->nID == ID_SKIN_POPUP)
	//{
	//	lpCreateControl->controlType = xtpControlPopup;
	//	return TRUE;
	//}

	//if (lpCreateControl->bToolBar &&
	//	(
	//	lpCreateControl->nID == ID_ACTIONS_REPLY ||
	//	lpCreateControl->nID == ID_ACTIONS_REPLYTOALL ||
	//	lpCreateControl->nID == ID_FILE_SENDRECEIVE ||
	//	lpCreateControl->nID == ID_FILE_FIND ||
	//	lpCreateControl->nID == ID_FILE_ORGANIZE ||
	//	lpCreateControl->nID == ID_ACTIONS_FORWARD
	//	))
	//{
	//	lpCreateControl->buttonStyle = xtpButtonIconAndCaption;
	//	return TRUE;
	//}

	//if (lpCreateControl->nID == ID_GOTO_URL)
	//	{
	//		CXTPControlComboBox* pComboState = (CXTPControlComboBox*)CXTPControlComboBox::CreateObject();

	//		for (int i = 0; i < _countof(OutlookShortcuts); i++)
	//		{
	//			CString str;
	//			str.LoadString(OutlookShortcuts[i].m_nTextID);
	//			pComboState->AddString(str);
	//		}
	//		pComboState->SetFlags(xtpFlagManualUpdate);
	//		pComboState->SetDropDownWidth(150);
	//		lpCreateControl->pControl = pComboState;
	//		return TRUE;
	//	}

	return FALSE;
}


void CDatabaseDialog::OnCustomize()
{
	//CXTPCustomizeSheet cs(GetCommandBars());

	//CXTPCustomizeOptionsPage pageOptions(&cs);
	//cs.AddPage(&pageOptions);

	//CXTPCustomizeCommandsPage* pCommands = cs.GetCommandsPage();
	//pCommands->AddCategories(IDR_MAINFRAME);

	//pCommands->InsertAllCommandsCategory();
	//pCommands->InsertNewMenuCategory();


	//cs.DoModal();
}


void CDatabaseDialog::OnGotoURL(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;

	CXTPControlComboBox* pControl = (CXTPControlComboBox*)tagNMCONTROL->pControl;
	if (pControl->GetType() == xtpControlComboBox)
	{
		int nState = pControl->GetCurSel();
		if (nState != -1)
		{
			CString str;
			pControl->GetLBText(nState, str);

			HTREEITEM hItem = m_pwndTreeCtrl->FindItem (str);
			if (hItem) 
			{
				m_pwndTreeCtrl->SelectItem (hItem);
			}
		}
		*pResult = TRUE; // Handled
	}
}

void CDatabaseDialog::OnClose()
{
	SaveCommandBars(_T("CommandBars4"));
	CFrameWnd::OnClose();
}


void CDatabaseDialog::UpdateButtonStyle(CXTCaptionButton& btnCaption)
{
	//DWORD dwBtnStyle = (btnCaption.GetXButtonStyle() &
	//	~(BS_XT_FLAT|BS_XT_XPFLAT));

	//if (m_nTheme != xtpThemeOffice2000)
	//{
	//	btnCaption.SetXButtonStyle(
	//		dwBtnStyle|BS_XT_XPFLAT, FALSE);
	//}
	//else
	//{
	//	btnCaption.SetXButtonStyle(
	//		dwBtnStyle|BS_XT_FLAT, FALSE);
	//}
}

void CDatabaseDialog::SetTheme(XTPPaintTheme nTheme)
{
	//m_nTheme = nTheme;

	//XTPPaintManager()->SetTheme(nTheme);

	//UpdateButtonStyle(m_wndCaption.GetCaptionButton());
	//UpdateButtonStyle(m_pItemTreeView->m_wndCaption.GetCaptionButton());


	//XTThemeStyle nControlsTheme = m_nTheme == xtpThemeNativeWinXP? xtThemeOffice2000: m_nTheme == xtpThemeOffice2000? xtThemeOffice2000:
	//	m_nTheme == xtpThemeOfficeXP? xtThemeOfficeXP: xtThemeOffice2003;

	//XTThemeManager()->SetTheme(nControlsTheme);

	//if ( m_nTheme == xtpThemeNativeWinXP)
	//{
	//	m_wndOutlookBar.SetTheme(xtpTaskPanelThemeNativeWinXP);
	//	//CXTHeaderCtrl::SetDefaultTheme(new CXTHeaderCtrlThemeExplorer());
	//}
	//else if ( m_nTheme == xtpThemeOffice2000)
	//	m_wndOutlookBar.SetTheme(xtpTaskPanelThemeListView);
	//else if ( m_nTheme == xtpThemeOfficeXP)
	//	m_wndOutlookBar.SetTheme(xtpTaskPanelThemeListViewOfficeXP);
	//else m_wndOutlookBar.SetTheme(xtpTaskPanelThemeShortcutBarOffice2003);

	//if ( m_nTheme == xtpThemeNativeWinXP)
	//{
	//	m_wndCaption.SetTheme(xtThemeOffice2003);
	//	//m_pItemTreeView->m_wndCaption.SetTheme(new CXTCaptionThemeNativeWinXP);
	//}
	//else if ( m_nTheme != xtpThemeOffice2003)
	//{
	//	m_wndCaption.SetTheme(nControlsTheme);
	//	m_pItemTreeView->m_wndCaption.SetTheme(nControlsTheme);
	//}
	//else
	//{
	//	m_wndCaption.SetTheme(nControlsTheme);
	//	m_pItemTreeView->m_wndCaption.SetTheme(new CXTCaptionThemeOffice2003);
	//}

	//GetCommandBars()->RedrawCommandBars();
	//RedrawWindow(NULL, NULL,
	//	RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
	//RecalcLayout();
	//MoveWindow(CXTPWindowRect(this));
	//m_wndSplitter1.RecalcLayout();
	//m_wndSplitter2.RecalcLayout();
	//m_wndSplitter3.RecalcLayout();
	////m_wndSplitter4.RecalcLayout();
	//m_wndOutlookBar.Reposition(FALSE);
}


void CDatabaseDialog::OnViewThemeOfficexp()
{
	SetTheme(xtpThemeOfficeXP);

}

void CDatabaseDialog::OnUpdateViewThemeOfficexp(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nTheme == xtpThemeOfficeXP);

}

void CDatabaseDialog::OnViewThemeOffice2003()
{
	SetTheme(xtpThemeOffice2003);

}

void CDatabaseDialog::OnUpdateViewThemeOffice2003(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nTheme == xtpThemeOffice2003);

}

void CDatabaseDialog::OnViewThemeOffice2000()
{
	SetTheme(xtpThemeOffice2000);

}

void CDatabaseDialog::OnUpdateViewThemeOffice2000(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nTheme == xtpThemeOffice2000);

}

void CDatabaseDialog::OnViewThemeWinXP()
{
	SetTheme(xtpThemeNativeWinXP);

}

void CDatabaseDialog::OnUpdateViewThemeWinXP(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nTheme == xtpThemeNativeWinXP);

}

CString CDatabaseDialog::GetStylesPath()
{
	TCHAR szStylesPath[_MAX_PATH];

	VERIFY(::GetModuleFileName(
		AfxGetApp()->m_hInstance, szStylesPath, _MAX_PATH));		
	
	CString csStylesPath(szStylesPath);
	int nIndex  = csStylesPath.ReverseFind(_T('\\'));
	if (nIndex > 0) {
		csStylesPath = csStylesPath.Left(nIndex);
	}
	else {
		csStylesPath.Empty();
	}

	return csStylesPath + _T("\\Styles\\");
}

 

void CDatabaseDialog::OnSkinChanged(UINT nID)
{
//#ifdef _XTP_INCLUDE_SKINFRAMEWORK
//	switch (nID)
//	{
//	case ID_SKIN_LUNABLUE:
//		XTPSkinManager()->LoadSkin(GetStylesPath() + _T("WinXP.Luna.cjstyles"), _T("NormalBlue.INI"));
//		break;
//
//	case ID_SKIN_LUNAOLIVE:
//		XTPSkinManager()->LoadSkin(GetStylesPath() + _T("WinXP.Luna.cjstyles"), _T("NormalHomeStead.INI"));
//		break;
//
//	case ID_SKIN_LUNAMETALLIC:
//		XTPSkinManager()->LoadSkin(GetStylesPath() + _T("WinXP.Luna.cjstyles"), _T("NormalMetallic.INI"));
//		break;
//	
//	case ID_SKIN_ROYALE:
//		XTPSkinManager()->LoadSkin(GetStylesPath() + _T("WinXP.Royale.cjstyles"));
//		break;
//
//	default:
//		XTPSkinManager()->LoadSkin(_T(""), _T(""));
//	}
//#endif
}

void CDatabaseDialog::OnSkinPopup()
{

}