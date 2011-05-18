// TerrainDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Editor.h"
#include "OgreEditor.h"
#include "TerrainDialog.h"
#include "EditTool/TerrainModifyTool.h"
#include "EditTool/TerrainPaintTool.h"
#include "EditTool/TerrainLightTool.h"


// CTerrainDialog �Ի���
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


// CTerrainDialog ��Ϣ�������

void CTerrainDialog::OnBnClickedModify()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if ( !GetEditor()->IsLevelLoaded() )
		return;

	GetEditor()->SetEditTool(new CTerrainModifyTool);	
}

void CTerrainDialog::OnBnClickedPaint()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if ( !GetEditor()->IsLevelLoaded() )
		return;

	GetEditor()->SetEditTool(new CTerrainPaintTool);
}

void CTerrainDialog::OnBnClickedLayerTextureBtn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	mTextureCombinDialog.ShowWindow(SW_SHOW);
	mTextureCombinDialog.SetFocus();
}

void CTerrainDialog::OnBnClickedLight()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if ( !GetEditor()->IsLevelLoaded() )
		return;

	GetEditor()->SetEditTool(new CTerrainLightTool);
}
