

#include "stdafx.h"
#include "resource.h"
#include "Editor.h"
#include "ParticlepropertyView.h"
#include "ParticleResourceView.h"
#include "ParticleRenderView.h"
#include "ParticleDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParticleDialog

IMPLEMENT_DYNCREATE(CParticleDialog, CFrameWnd)

BEGIN_MESSAGE_MAP(CParticleDialog, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	ON_WM_CREATE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)
	ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, OnDockingPaneNotify)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

static UINT uHideCmds[] =
{
	ID_FILE_PRINT,
	ID_FILE_PRINT_PREVIEW,
};
/////////////////////////////////////////////////////////////////////////////
// CParticleDialog construction/destruction

CParticleDialog::CParticleDialog()
: mpParticleResView(NULL)
, mpParticlePropertyView(NULL)
, m_pActivePane(NULL)
{
	// TODO: add member initialization code here

}

CParticleDialog::~CParticleDialog()
{
	GetEditor()->DestroyParticleDialog();
}

int CParticleDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;


	// Create Status bar.
	// Important: All control bars including the Status Bar
	// must be created before CommandBars....
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// Initialize the command bars
	if (!InitCommandBars())
		return -1;

	// Get a pointer to the command bars object.
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if(pCommandBars == NULL)
	{
		TRACE0("Failed to create command bars object.\n");
		return -1;      // fail to create
	}

	// Add the menu bar
	CXTPCommandBar* pMenuBar = pCommandBars->SetMenu(
		_T("Menu Bar"), IDR_PARTICLE_EDIT_MENU);
	if(pMenuBar == NULL)
	{
		TRACE0("Failed to create menu bar.\n");
		return -1;      // fail to create
	}

	// Create ToolBar
	CXTPToolBar* pToolBar = (CXTPToolBar*)
		pCommandBars->Add(_T("Standard"), xtpBarTop);
	if (!pToolBar || !pToolBar->LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	// Set Office 2003 Theme
	CXTPPaintManager::SetTheme(xtpThemeOfficeXP);

	// Hide array of commands
	pCommandBars->HideCommands(uHideCmds, _countof(uHideCmds));

	// Set "Always Show Full Menus" option to the FALSE
	pCommandBars->GetCommandBarsOptions()->bAlwaysShowFullMenus = FALSE;

	// Load the previous state for toolbars and menus.
	LoadCommandBars(_T("CommandBars"));


	// Initialize the docking pane manager and set the
	// initial them for the docking panes.  Do this only after all
	// control bars objects have been created and docked.
	m_paneManager.InstallDockingPanes(this);
	// Set Office 2003 Theme
	m_paneManager.SetTheme(xtpPaneThemeVisualStudio2003);

	// Create docking panes.
	CXTPDockingPane* pwndPane1 = m_paneManager.CreatePane(
		IDR_PANE_OPTIONS, CRect(0, 0,200, 120), xtpPaneDockLeft);
	CXTPDockingPane* pwndPane2 = m_paneManager.CreatePane(
		IDR_PANE_PROPERTIES, CRect(0, 0,200, 120), xtpPaneDockBottom, pwndPane1);
	CXTPDockingPane* pwndPane3 = m_paneManager.CreatePane(
		IDR_PANE_PREVIEW, CRect(0, 0,800, 600), xtpPaneDockTop);

	m_paneManager.EnableKeyboardNavigate();

	// Set the icons for the docking pane tabs.
	int nIDIcons[] = {IDR_PANE_OPTIONS, IDR_PANE_PROPERTIES,IDR_PANE_PREVIEW};
		m_paneManager.SetIcons(IDB_BITMAP_ICONS, nIDIcons,
		_countof(nIDIcons), RGB(0, 255, 0));

	//// Load the previous state for docking panes.
	//CXTPDockingPaneLayout layoutNormal(&m_paneManager);
	//if (layoutNormal.Load(_T("NormalLayout")))
	//{
	//	m_paneManager.SetLayout(&layoutNormal);
	//}

	return 0;
}

void CParticleDialog::OnClose()
{

	// Save the current state for toolbars and menus.
	SaveCommandBars(_T("CommandBars"));


	// Save the current state for docking panes.
	//CXTPDockingPaneLayout layoutNormal(&m_paneManager);
	//m_paneManager.GetLayout(&layoutNormal);
	//layoutNormal.Save(_T("NormalLayout"));

	CFrameWnd::OnClose();
}


void CParticleDialog::OnCustomize()
{
	// Get a pointer to the command bars object.
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if(pCommandBars != NULL)
	{
		// Instanciate the customize dialog object.
		CXTPCustomizeSheet dlg(pCommandBars);

		// Add the options page to the customize dialog.
		CXTPCustomizeOptionsPage pageOptions(&dlg);
		dlg.AddPage(&pageOptions);

		// Add the commands page to the customize dialog.
		CXTPCustomizeCommandsPage* pCommands = dlg.GetCommandsPage();
		pCommands->AddCategories(IDR_MAINFRAME);

		// Use the command bar manager to initialize the
		// customize dialog.
		pCommands->InsertAllCommandsCategory();
		pCommands->InsertBuiltInMenus(IDR_MAINFRAME);
		pCommands->InsertNewMenuCategory();

		// Dispaly the dialog.
		dlg.DoModal();
	}
}


LRESULT CParticleDialog::OnDockingPaneNotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam == XTP_DPN_SHOWWINDOW)
	{
		CXTPDockingPane* pPane = (CXTPDockingPane*)lParam;

		if (!pPane->IsValid())
		{
			switch (pPane->GetID())
			{
			case IDR_PANE_OPTIONS:
				{
					if (!mpParticleResView)
					{
						mpParticleResView = DYNAMIC_DOWNCAST(CParticleResView,
							pPane->AttachView(this, RUNTIME_CLASS(CParticleResView)));
					}
					else
					{
						pPane->Attach(mpParticleResView->GetParent());
					}
					break;
				}
			case IDR_PANE_PROPERTIES:
				{
					if (!mpParticlePropertyView)
					{
						mpParticlePropertyView = DYNAMIC_DOWNCAST(CParticlePropertyView,
							pPane->AttachView(this, RUNTIME_CLASS(CParticlePropertyView)));
					} 
					else
					{
						pPane->Attach(mpParticlePropertyView->GetParent());
					}
					break;
				}
			case IDR_PANE_PREVIEW:
				{
					if (!mParticlePreviewDialog.GetSafeHwnd())
					{
						mParticlePreviewDialog.Create(CParticlePreviewDialog::IDD, this);
					}
					pPane->Attach(&mParticlePreviewDialog);

					break;
				}
			}
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CParticleDialog::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;


	cs.lpszClass = _T("XTPMainFrame");
	CXTPDrawHelpers::RegisterWndClass(AfxGetInstanceHandle(), cs.lpszClass, 
		CS_DBLCLKS, AfxGetApp()->LoadIcon(IDR_MAINFRAME));

	cs.style |= WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CParticleDialog diagnostics

#ifdef _DEBUG
void CParticleDialog::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CParticleDialog::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CParticleDialog message handlers

// The following code will ensure that messages are correctly routed to
// the docking pane child views.

CXTPDockingPane* CParticleDialog::GetActivePane()
{
	return m_paneManager.GetActivePane();
}

CWnd* CParticleDialog::GetActivePaneView()
{
	CXTPDockingPane* pActivePane = GetActivePane();
	if (pActivePane)
		return pActivePane->GetChild();
	return NULL;
}

BOOL CParticleDialog::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// Use GetActivePane() to search the pane manager's list for the pane
	// with a view that has input focus.  If one is found, give the view an
	// opportunity to handle the OnCmdMsg first before we pass it to the base
	// class.

	CWnd* pView = GetActivePaneView();
	if (pView != NULL && pView->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
	{
		return TRUE; // handled.
	}

	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CParticleDialog::OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState)
{
// Toggle CommandBars
	GetCommandBars()->OnSetPreviewMode(bPreview);

// Toggle Docking Panes.
	m_paneManager.OnSetPreviewMode(bPreview);


	CFrameWnd::OnSetPreviewMode(bPreview, pState);
}
