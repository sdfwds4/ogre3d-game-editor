// TerrainPaintDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "Editor.h"
#include "OgreEditor.h"
#include "TerrainPaintDialog.h"
#include "EditTool/TerrainPaintTool.h"
#include "TerrainAddLayerDialog.h"

// CTerrainPaintDialog 对话框

IMPLEMENT_DYNCREATE(CTerrainPaintDialog, CDialog)

CTerrainPaintDialog::CTerrainPaintDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTerrainPaintDialog::IDD, pParent),mMaxLayers(0),mLayers(0),
	miScale(10), miHardness(1), mpTerrain(NULL), mLayerInfos(NULL), mhRootItem(NULL)
{
	mBrushType = "";
}

CTerrainPaintDialog::~CTerrainPaintDialog()
{
}

void CTerrainPaintDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BRUSH_SCALE_SLIDER, mScaleSlider);
	DDX_Control(pDX, IDC_BRUSH_HARDNESS_SLIDER, mHardnessSlider);
	DDX_Control(pDX, IDC_SCALE_EDIT, mScaleEdit);
	DDX_Control(pDX, IDC_HARDNESS_EDIT, mHardnessEdit);
	DDX_Control(pDX, IDC_BRUSH_TYPE_COMBO, mBrushTypeList);
	DDX_Control(pDX, IDC_LAYER_TREE, mLayerTree);

	DDX_Text(pDX, IDC_BRUSH_TYPE_COMBO, mBrushType);
	DDX_Text(pDX, IDC_SCALE_EDIT, miScale);
	DDV_MinMaxInt(pDX,miScale, 1, 100);
	DDX_Text(pDX, IDC_HARDNESS_EDIT, miHardness);
	DDV_MinMaxInt(pDX,miHardness, 1, 100);
	DDX_Control(pDX, IDC_LAYER_INFO, mLayerInfoText);
}

BOOL CTerrainPaintDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	mBrushTypeList.AddString( "加深工具" );
	mBrushTypeList.AddString( "减淡工具" );
	mBrushTypeList.SetCurSel(0);

	mScaleSlider.SetRange( 1, 100, TRUE);
	mScaleSlider.SetPos(miScale);

	mHardnessSlider.SetRange( 1, 100, TRUE);
	mHardnessSlider.SetPos(miHardness);

	mpTerrain = GetEditor()->GetTerrainGroup()->getTerrain(0,0);

	mMaxLayers = mpTerrain->getMaxLayers();
	mLayerInfos = new LayerInfo[mMaxLayers];
	CreateLayerTree();

	UpdateData(FALSE);

	CTerrainPaintTool* pTool = (CTerrainPaintTool*)GetEditor()->GetCurrentEditTool();

	pTool->SetBrushSize( 0.001f*miScale );
	pTool->SetBrushHardness( 0.001f*miHardness );
	pTool->SetBrushType(E_TERRAIN_ADD);

	return TRUE;
}

BEGIN_MESSAGE_MAP(CTerrainPaintDialog, CDialog)
	ON_WM_DESTROY()
	ON_WM_HSCROLL()
	ON_NOTIFY(TVN_SELCHANGED, IDC_LAYER_TREE, OnSelchanged)
	ON_CBN_SELCHANGE(IDC_BRUSH_TYPE_COMBO, OnCbnSelchangeBrushTypeCombo)
	ON_BN_CLICKED(IDC_ADD_LAYER_BTN, &CTerrainPaintDialog::OnBnClickedAddLayerBtn)
	ON_BN_CLICKED(IDC_DEL_LAYER_BTN, &CTerrainPaintDialog::OnBnClickedDelLayerBtn)
END_MESSAGE_MAP()

void CTerrainPaintDialog::OnDestroy()
{
	CDialog::OnDestroy();

	if ( mLayerInfos )
	{
		delete[] mLayerInfos;
		mLayerInfos = NULL;
	}
}

void CTerrainPaintDialog::CreateLayerTree()
{
	mLayers = mpTerrain->getLayerCount();

	mLayerTree.DeleteAllItems();
	mhRootItem = mLayerTree.InsertItem ("图层列表");
	for ( int i = 0; i < mLayers; i++ )
	{
		CString layer;
		if( i == 0 )
			layer.Format( "默认图层" );
		else
			layer.Format( "layer%d", i );
		HTREEITEM hItem = mLayerTree.InsertItem ( layer, mhRootItem);

		mLayerInfos[i].mItem = hItem;
		mLayerInfos[i].mId = i;

		for ( int j = 0; j < 4; j++ )
		{
			String name = mpTerrain->getLayerTextureName(i, j);

			if ( StringUtil::BLANK == name )
				break;
			mLayerTree.InsertItem ( name.c_str(), hItem);
		}
		mLayerTree.Expand (hItem, TVE_EXPAND);
	}
	mLayerTree.Expand (mhRootItem, TVE_EXPAND);
	mLayerTree.SetItemState(mhRootItem, TVIS_SELECTED, TVIS_SELECTED);

	CTerrainPaintTool* pTool = (CTerrainPaintTool*)GetEditor()->GetCurrentEditTool();
	pTool->SetLayerEdit( 0 );

	CString info;
	info = "当前选择的图层:无";
	mLayerInfoText.SetWindowText( info );
}


// CTerrainPaintDialog 消息处理程序
void CTerrainPaintDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CTerrainPaintTool* pTool = (CTerrainPaintTool*)GetEditor()->GetCurrentEditTool();

	miScale = mScaleSlider.GetPos();
	pTool->SetBrushSize( 0.001f*miScale );
	miHardness = mHardnessSlider.GetPos();
	pTool->SetBrushHardness( 0.001f*miHardness );
	UpdateData(FALSE);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CTerrainPaintDialog::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	HTREEITEM htItem = mLayerTree.GetSelectedItem();
	if (htItem != NULL)
	{
		for ( int i = 0; i < mLayers; i++ )
		{
			if ( mLayerInfos[i].mItem == htItem )
			{
				if ( mLayerInfos[i].mId == 0 )
					break;

				CTerrainPaintTool* pTool = (CTerrainPaintTool*)GetEditor()->GetCurrentEditTool();
				pTool->SetLayerEdit( mLayerInfos[i].mId );

				CString info;
				info.Format( "当前选择的图层:%d", mLayerInfos[i].mId );
				mLayerInfoText.SetWindowText( info );
				break;
			}
		}
	}

	*pResult = 0;
}

void CTerrainPaintDialog::OnCbnSelchangeBrushTypeCombo()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();

	if ( mBrushType.IsEmpty() )
		return;

	CTerrainPaintTool* pTool = (CTerrainPaintTool*)GetEditor()->GetCurrentEditTool();

	if ( mBrushType == "加深工具" )
	{
		pTool->SetBrushType(E_TERRAIN_ADD);
	}
	else if ( mBrushType == "减淡工具" )
	{
		pTool->SetBrushType(E_TERRAIN_MINUS);
	}
}
void CTerrainPaintDialog::OnBnClickedAddLayerBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	if ( mMaxLayers == mLayers )
	{
		MessageBox( "地形层已达到最大数，不能创建新的层！" );
		return;
	}

	CTerrainAddLayerDialog dlg;
	if ( IDOK == dlg.DoModal() )
	{
		StringVector sv;
		if ( !dlg.mTexture1.IsEmpty() )
		{
			String texture = dlg.mTexture1;
			sv.push_back( texture );
		}
		if ( !dlg.mTexture2.IsEmpty() )
		{
			String texture = dlg.mTexture2;
			sv.push_back( texture );
		}

		Real worldSize = dlg.mWorldSize;

		mpTerrain->addLayer(worldSize, &sv);

		CreateLayerTree();
	}
	
}

void CTerrainPaintDialog::OnBnClickedDelLayerBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM htItem = mLayerTree.GetSelectedItem();
	if (htItem != NULL)
	{
		for ( int i = 0; i < mLayers; i++ )
		{
			if ( mLayerInfos[i].mItem == htItem )
			{
				if ( mLayerInfos[i].mId == 0 )
					break;

				mpTerrain->removeLayer( mLayerInfos[i].mId );

				CreateLayerTree();
			}
		}
	}
}
