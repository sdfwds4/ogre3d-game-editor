// TerrainModifyDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Editor.h"
#include "OgreEditor.h"
#include "TerrainModifyDialog.h"
#include "EditTool/TerrainModifyTool.h"

// CTerrainModifyDialog �Ի���

IMPLEMENT_DYNCREATE(CTerrainModifyDialog, CDialog)

CTerrainModifyDialog::CTerrainModifyDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTerrainModifyDialog::IDD, pParent), miScale(10), miHardness(1)
{
	mBrushType = "";
}

CTerrainModifyDialog::~CTerrainModifyDialog()
{
}

void CTerrainModifyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BRUSH_TYPE_COMBO, mBrushTypeList);
	DDX_Control(pDX, IDC_BRUSH_SCALE_SLIDER, mBrushScaleSlider);
	DDX_Control(pDX, IDC_HARDNESS_SLIDER, mHardnessSlider);

	DDX_Text(pDX, IDC_BRUSH_TYPE_COMBO, mBrushType);
	DDX_Text(pDX, IDC_SCALE_EDIT, miScale);
	DDV_MinMaxInt(pDX,miScale, 1, 100);
	DDX_Text(pDX, IDC_HARDNESS_EDIT, miHardness);
	DDV_MinMaxInt(pDX,miHardness, 1, 100);

}


BOOL CTerrainModifyDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	mBrushTypeList.AddString( "���ߵ���" );
	mBrushTypeList.AddString( "���͵���" );
	mBrushTypeList.SetCurSel(0);

	mBrushScaleSlider.SetRange( 1, 100, TRUE);
	mBrushScaleSlider.SetPos(miScale);

	mHardnessSlider.SetRange( 1, 100, TRUE);
	mHardnessSlider.SetPos(miHardness);

	UpdateData(FALSE);

	CTerrainModifyTool* pTool = (CTerrainModifyTool*)GetEditor()->GetCurrentEditTool();

	pTool->SetBrushSize( 0.001f*miScale );
	pTool->SetBrushHardness( 0.001f*miHardness );
	pTool->SetBrushType(E_TERRAIN_RISE);

	return TRUE;
}

BEGIN_MESSAGE_MAP(CTerrainModifyDialog, CDialog)
	ON_CBN_SELCHANGE(IDC_BRUSH_TYPE_COMBO, &CTerrainModifyDialog::OnCbnSelchangeBrushTypeCombo)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

// CTerrainModifyDialog ��Ϣ�������

void CTerrainModifyDialog::OnCbnSelchangeBrushTypeCombo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();

	if ( mBrushType.IsEmpty() )
		return;

	CTerrainModifyTool* pTool = (CTerrainModifyTool*)GetEditor()->GetCurrentEditTool();

	if ( mBrushType == "���ߵ���" )
	{
		pTool->SetBrushType(E_TERRAIN_RISE);
	}
	else if ( mBrushType == "���͵���" )
	{
		pTool->SetBrushType(E_TERRAIN_LOWER);
	}
}

void CTerrainModifyDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CTerrainModifyTool* pTool = (CTerrainModifyTool*)GetEditor()->GetCurrentEditTool();

	miScale = mBrushScaleSlider.GetPos();
	pTool->SetBrushSize( 0.001f*miScale );
	miHardness = mHardnessSlider.GetPos();
	pTool->SetBrushHardness( 0.001f*miHardness );
	UpdateData(FALSE);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
