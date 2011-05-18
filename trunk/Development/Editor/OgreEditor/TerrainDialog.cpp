// TerrainDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "Editor.h"
#include "OgreEditor.h"
#include "TerrainDialog.h"
#include "EditTool/TerrainModifyTool.h"
#include "EditTool/TerrainPaintTool.h"
#include "EditTool/TerrainLightTool.h"


// CTerrainDialog 对话框
IMPLEMENT_DYNCREATE(CTerrainDialog, CDialog)

CTerrainDialog::CTerrainDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTerrainDialog::IDD, pParent)
{

}

CTerrainDialog::~CTerrainDialog()
{
}

void CTerrainDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CTerrainDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	mTextureCombinDialog.Create(CTerrainTextureCombineDialog::IDD);
	mTextureCombinDialog.ShowWindow(SW_HIDE);
	return TRUE;
}


BEGIN_MESSAGE_MAP(CTerrainDialog, CDialog)
	ON_BN_CLICKED(ID_MODIFY, &CTerrainDialog::OnBnClickedModify)
	ON_BN_CLICKED(ID_PAINT, &CTerrainDialog::OnBnClickedPaint)
	ON_BN_CLICKED(IDC_LAYER_TEXTURE_BTN, &CTerrainDialog::OnBnClickedLayerTextureBtn)
	ON_BN_CLICKED(ID_LIGHT, &CTerrainDialog::OnBnClickedLight)
END_MESSAGE_MAP()


// CTerrainDialog 消息处理程序

void CTerrainDialog::OnBnClickedModify()
{
	// TODO: 在此添加控件通知处理程序代码
	if ( !GetEditor()->IsLevelLoaded() )
		return;

	GetEditor()->SetEditTool(new CTerrainModifyTool);	
}

void CTerrainDialog::OnBnClickedPaint()
{
	// TODO: 在此添加控件通知处理程序代码
	if ( !GetEditor()->IsLevelLoaded() )
		return;

	GetEditor()->SetEditTool(new CTerrainPaintTool);
}

void CTerrainDialog::OnBnClickedLayerTextureBtn()
{
	// TODO: 在此添加控件通知处理程序代码

	mTextureCombinDialog.ShowWindow(SW_SHOW);
	mTextureCombinDialog.SetFocus();
}

void CTerrainDialog::OnBnClickedLight()
{
	// TODO: 在此添加控件通知处理程序代码
	if ( !GetEditor()->IsLevelLoaded() )
		return;

	GetEditor()->SetEditTool(new CTerrainLightTool);
}
