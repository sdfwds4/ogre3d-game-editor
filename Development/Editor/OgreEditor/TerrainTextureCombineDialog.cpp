// TerrainTextureCombineDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "OgreEditor.h"
#include "TerrainTextureCombineDialog.h"


// CTerrainTextureCombineDialog 对话框

IMPLEMENT_DYNAMIC(CTerrainTextureCombineDialog, CDialog)

CTerrainTextureCombineDialog::CTerrainTextureCombineDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTerrainTextureCombineDialog::IDD, pParent)
{
	mDiffuse = "";
	mSpecular = "";
	mNormal = "";
	mDisp = "";
	mDiffuseSpecular = "test_diffusespecular.png";
	mNormalDisp = "test_normalheight.png";
}

CTerrainTextureCombineDialog::~CTerrainTextureCombineDialog()
{
}

void CTerrainTextureCombineDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_DIFFUSE_EDIT, mDiffuse);
	DDX_Text(pDX, IDC_SPECULAR_EDIT, mSpecular);
	DDX_Text(pDX, IDC_NORMAL_EDIT, mNormal);
	DDX_Text(pDX, IDC_DISP_EDIT, mDisp);
	DDX_Text(pDX, IDC_DIFFUSE_SPECULAR_EDIT, mDiffuseSpecular);
	DDX_Text(pDX, IDC_NORMAL_DISP_EDIT, mNormalDisp);
}

BEGIN_MESSAGE_MAP(CTerrainTextureCombineDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CTerrainTextureCombineDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTerrainTextureCombineDialog::OnBnClickedCancel)
END_MESSAGE_MAP()


// CTerrainTextureCombineDialog 消息处理程序

void CTerrainTextureCombineDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if ( mDiffuse.IsEmpty() )
	{
		MessageBox( "漫反射贴图信息不完整！" );
		return;
	}

	if ( mSpecular.IsEmpty() )
	{
		MessageBox( "镜面贴图信息不完整！" );
		return;
	}

	if ( mNormal.IsEmpty() )
	{
		MessageBox( "法线贴图信息不完整！" );
		return;
	}

	if ( mDisp.IsEmpty() )
	{
		MessageBox( "位移贴图信息不完整！" );
		return;
	}

	if ( mDiffuseSpecular.IsEmpty() )
	{
		MessageBox( "漫反射镜面合成贴图信息不完整！" );
		return;
	}

	if ( mNormalDisp.IsEmpty() )
	{
		MessageBox( "法线位移合成贴图信息不完整！" );
		return;
	}

	Ogre::Image combined;

	combined.loadTwoImagesAsRGBA( mDiffuse.GetBuffer(), mSpecular.GetBuffer(), 
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::PF_BYTE_RGBA);
	combined.save(mDiffuseSpecular.GetBuffer());

	combined.loadTwoImagesAsRGBA( mNormal.GetBuffer(), mDisp.GetBuffer(),
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::PF_BYTE_RGBA);
	combined.save(mNormalDisp.GetBuffer());

	MessageBox( "地表贴图合并完成！" );

	mDiffuse = "";
	mSpecular = "";
	mNormal = "";
	mDisp = "";
	mDiffuseSpecular = "test_diffusespecular.png";
	mNormalDisp = "test_normalheight.png";
	UpdateData(FALSE);

	ShowWindow(SW_HIDE);
	//OnOK();
}

void CTerrainTextureCombineDialog::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	//OnCancel();
	mDiffuse = "";
	mSpecular = "";
	mNormal = "";
	mDisp = "";
	mDiffuseSpecular = "test_diffusespecular.png";
	mNormalDisp = "test_normalheight.png";
	UpdateData(FALSE);

	ShowWindow(SW_HIDE);
}
