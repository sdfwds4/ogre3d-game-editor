


#include "stdafx.h"
#include "resource.h"
#include "Editor.h"
#include "EditTool/TextureTool.h"
#include "EditTool/MaterialTool.h"
#include "EditTool/ModelTool.h"
#include "ItemTreeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef enum
{
	IDX_MATERIAL,
	IDX_TEXTURE,
	IDX_MODEL,
	IDX_SCENENODE,
	IDX_COUNT
} TREE_INDEX;

typedef struct
{
	UINT    m_nIndex;
	UINT    m_nTextID;
	UINT    m_nIconID;
}
TREE_ENTRY;

static TREE_ENTRY TreeEntries[] =
{
	{ IDX_MATERIAL,        IDS_MATERIAL,      IDI_MATERIAL       },
	{ IDX_TEXTURE,         IDS_TEXTURE,       IDI_TEXTURE        },
	{ IDX_MODEL,		   IDS_MODEL,		  IDI_MODEL		     },
	{ IDX_SCENENODE,       IDS_SCENENODE,     IDI_SCENENODE      }
};

/////////////////////////////////////////////////////////////////////////////
// CItemTreeView

IMPLEMENT_DYNCREATE(CItemTreeView, CView)

CItemTreeView::CItemTreeView()
{

}

CItemTreeView::~CItemTreeView()
{
	m_ilTreeIcons.DeleteImageList();
}


BEGIN_MESSAGE_MAP(CItemTreeView, CView)
	//{{AFX_MSG_MAP(CItemTreeView)
	ON_WM_CREATE()
	ON_WM_WINDOWPOSCHANGED()
	//ON_BN_CLICKED(IDC_CAPT_BUTTON, OnCaptButton)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_CTRL, OnSelchanged)
	ON_WM_SYSCOLORCHANGE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CItemTreeView drawing

void CItemTreeView::OnDraw(CDC* /*pDC*/)
{
}

/////////////////////////////////////////////////////////////////////////////
// CItemTreeView diagnostics

#ifdef _DEBUG
void CItemTreeView::AssertValid() const
{
	CView::AssertValid();
}

void CItemTreeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

void CItemTreeView::DoDataExchange(CDataExchange* pDX)
{
	CView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_CTRL, m_wndTreeCtrl);

}

/////////////////////////////////////////////////////////////////////////////
// CItemTreeView message handlers

int CItemTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Create the caption.
	if (!m_wndCaption.Create( this, _T("资源列表"), CPWS_EX_RAISED_EDGE | CPWS_EX_CLOSEBUTTON,
		WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, CRect(0,0,0,0), IDC_CAPT_BUTTON ))
	{
		TRACE0( "Unable to create caption.\n" );
		return -1;
	}

	// create the tree control.
	if (!m_wndTreeCtrl.Create (WS_VISIBLE|TVS_HASLINES|TVS_LINESATROOT|TVS_HASBUTTONS,
		CRect(0,0,0,0), this, IDC_TREE_CTRL))
	{
		TRACE0( "Unable to create tree control.\n" );
		return -1;
	}

	// create the image lists used by the caption and tree controls.
	if (!InitializeImageLists())
		return -1;

	// fill the tree control.
	if (!InitilalizeTreeControl())
		return -1;

	return 0;
}

void CItemTreeView::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos)
{
	CView::OnWindowPosChanged(lpwndpos);

	// Move the caption and tree control to their correct locations.
	if( ::IsWindow( m_wndCaption.m_hWnd ) )
	{
		::SetWindowPos( m_wndCaption.m_hWnd, NULL, 0, 0, lpwndpos->cx - 2, 19,
			SWP_NOZORDER | SWP_NOACTIVATE );
	}
	if( ::IsWindow( m_wndTreeCtrl.m_hWnd ) )
	{
		::SetWindowPos( m_wndTreeCtrl.m_hWnd, NULL, 0, 19, lpwndpos->cx - 2, lpwndpos->cy-21,
			SWP_NOZORDER | SWP_NOACTIVATE );
	}
}

void CItemTreeView::OnCaptButton()
{
	//CMainFrame* pFrameWnd = (CMainFrame*)GetParentFrame();
	//ASSERT_KINDOF (CMainFrame, pFrameWnd);
	//pFrameWnd->CloseTreeViewPane();
}

void CItemTreeView::OnSelchanged(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	// Get the selected tree item and its icon.
	HTREEITEM htItem = m_wndTreeCtrl.GetSelectedItem();
	if (htItem != NULL)
	{
		CString name = m_wndTreeCtrl.GetItemText(htItem);

		mpGdiView->GetEditTool()->FromName(name);
		//mpGdiView->LoadImage(name);
	}

	*pResult = 0;
}

BOOL CItemTreeView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CView::PreCreateWindow(cs))
		return FALSE;

	// TODO: Add your specialized code here and/or call the base class
	cs.style |= WS_CLIPCHILDREN|WS_CLIPSIBLINGS;

	return TRUE;
}

BOOL CItemTreeView::InitilalizeTreeControl()
{
	// Set the tree controls image list.
	m_wndTreeCtrl.SetImageList (&m_ilTreeIcons, TVSIL_NORMAL);

	//HTREEITEM hItem = NULL;
	//CString   str;

	//int nEntry;
	//for (nEntry = 0; nEntry < _countof(TreeEntries); nEntry++)
	//{
	//	TREE_ENTRY* pEntry = &TreeEntries[nEntry];

	//	str.LoadString (pEntry->m_nTextID);

	//	if (nEntry == 0)
	//		hItem = m_wndTreeCtrl.InsertItem (str, pEntry->m_nIndex, pEntry->m_nIndex);
	//	else
	//		m_wndTreeCtrl.InsertItem (str, pEntry->m_nIndex, pEntry->m_nIndex, hItem);
	//}

	//m_wndTreeCtrl.SetItemBold (hItem, TRUE);
	//m_wndTreeCtrl.Expand (hItem, TVE_EXPAND);
	//m_wndTreeCtrl.SetItemState(hItem, TVIS_SELECTED, TVIS_SELECTED);
	//m_wndTreeCtrl.EnableMultiSelect(FALSE);
	return TRUE;
}

BOOL CItemTreeView::InitializeImageLists()
{
	// Create the image list used by the tree control.
	if (!m_ilTreeIcons.Create(16, 16, ILC_MASK|ILC_COLOR32, 2, 1))
		return FALSE;

	 //Load icons displayed in the tree control and add them to the image list.
	int nEntry;
	for (nEntry = 0; nEntry < _countof(TreeEntries); nEntry++)
	{
		TREE_ENTRY* pEntry = &TreeEntries[nEntry];

		HICON hIcon = AfxGetApp()->LoadIcon(pEntry->m_nIconID);
		ASSERT(hIcon);

		m_ilTreeIcons.Add (hIcon);
	}

	return TRUE;
}

void CItemTreeView::OnSysColorChange()
{
	CView::OnSysColorChange();

	m_wndCaption.SetCaptionColors(
		::GetSysColor(COLOR_3DFACE),
		::GetSysColor(COLOR_3DFACE),
		::GetSysColor(COLOR_BTNTEXT) );
}

void CItemTreeView::UpdateTree( LPCTSTR lpszText )
{
	// TODO: Add items to the tree control.
	m_wndTreeCtrl.DeleteAllItems();

	HTREEITEM hItem = NULL;
	CString resName = "材质";
	if ( resName == lpszText )
	{
		CMaterialTool* pTool = new CMaterialTool;
		pTool->SetParent( mpGdiView );
		mpGdiView->SetEditTool( pTool );

		Ogre::MaterialManager* mtlMgr = Ogre::MaterialManager::getSingletonPtr();
		Ogre::MaterialManager::ResourceMapIterator it = mtlMgr->getResourceIterator();

		hItem = m_wndTreeCtrl.InsertItem (lpszText, IDX_MATERIAL, IDX_MATERIAL);
		int index = 0;
		while ( it.hasMoreElements() )
		{
			Ogre::MaterialPtr mtlPtr = it.getNext();

			m_wndTreeCtrl.InsertItem (mtlPtr->getName().c_str(), IDX_MATERIAL, IDX_MATERIAL, hItem);
			index++;
		}
	}

	resName = "贴图";
	if ( resName == lpszText )
	{
		CTextureTool* pTool = new CTextureTool;
		pTool->SetParent( mpGdiView );
		mpGdiView->SetEditTool( pTool );
		Ogre::TextureManager* texMgr = Ogre::TextureManager::getSingletonPtr();
		Ogre::TextureManager::ResourceMapIterator it = texMgr->getResourceIterator();

		hItem = m_wndTreeCtrl.InsertItem (lpszText, IDX_TEXTURE, IDX_TEXTURE);
		int index = 0;
		while ( it.hasMoreElements() )
		{
			Ogre::TexturePtr texPtr = it.getNext();

			m_wndTreeCtrl.InsertItem (texPtr->getName().c_str(), IDX_TEXTURE, IDX_TEXTURE, hItem);
			index++;
		}
	}

	resName = "模型";
	if ( resName == lpszText )
	{
		CModelTool* pTool = new CModelTool;
		pTool->SetParent( mpGdiView );
		mpGdiView->SetEditTool( pTool );

		Ogre::MeshManager* meshMgr = Ogre::MeshManager::getSingletonPtr();
		Ogre::MeshManager::ResourceMapIterator it = meshMgr->getResourceIterator();

		hItem = m_wndTreeCtrl.InsertItem (lpszText, IDX_MODEL, IDX_MODEL);
		int index = 0;
		while ( it.hasMoreElements() )
		{
			Ogre::MeshPtr meshPtr = it.getNext();

			m_wndTreeCtrl.InsertItem (meshPtr->getName().c_str(), IDX_MODEL, IDX_MODEL, hItem);
			index++;
		}
	}

	resName = "场景节点";
	if ( resName == lpszText )
	{
		hItem = CreateSceneTree();
	}

	m_wndTreeCtrl.SetItemBold (hItem, TRUE);
	m_wndTreeCtrl.Expand (hItem, TVE_EXPAND);
	m_wndTreeCtrl.SetItemState(hItem, TVIS_SELECTED, TVIS_SELECTED);
	m_wndTreeCtrl.EnableMultiSelect(FALSE);
}

HTREEITEM CItemTreeView::CreateSceneTree()
{
	HTREEITEM hItem = NULL;

	Ogre::SceneNode* root = GetEditor()->GetSceneManager()->getRootSceneNode();
	hItem = m_wndTreeCtrl.InsertItem (root->getName().c_str(), IDX_SCENENODE, IDX_SCENENODE);
	int index = 0;

	Ogre::Node::ChildNodeIterator it = root->getChildIterator();

	while ( it.hasMoreElements() )
	{
		Ogre::Node* pNode = it.getNext();

		HTREEITEM hChildItem = m_wndTreeCtrl.InsertItem (pNode->getName().c_str(), IDX_SCENENODE, IDX_SCENENODE, hItem);
		UpdateSceneNodeTree( pNode, hChildItem );
		index++;
	}

	return hItem;
}

void CItemTreeView::UpdateSceneNodeTree( Ogre::Node* node, HTREEITEM treeitem )
{
	int index = 0;

	Ogre::Node::ChildNodeIterator it = node->getChildIterator();

	while ( it.hasMoreElements() )
	{
		Ogre::Node* pNode = it.getNext();

		HTREEITEM hChildItem = m_wndTreeCtrl.InsertItem (pNode->getName().c_str(), 3, index, treeitem);
		UpdateSceneNodeTree( pNode, hChildItem );
		index++;
	}
}
