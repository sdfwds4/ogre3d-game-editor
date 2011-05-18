// TerrainTextureCombineDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OgreEditor.h"
#include "TerrainTextureCombineDialog.h"


// CTerrainTextureCombineDialog �Ի���

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


// CTerrainTextureCombineDialog ��Ϣ�������

void CTerrainTextureCombineDialog::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	if ( mDiffuse.IsEmpty() )
	{
		MessageBox( "��������ͼ��Ϣ��������" );
		return;
	}

	if ( mSpecular.IsEmpty() )
	{
		MessageBox( "������ͼ��Ϣ��������" );
		return;
	}

	if ( mNormal.IsEmpty() )
	{
		MessageBox( "������ͼ��Ϣ��������" );
		return;
	}

	if ( mDisp.IsEmpty() )
	{
		MessageBox( "λ����ͼ��Ϣ��������" );
		return;
	}

	if ( mDiffuseSpecular.IsEmpty() )
	{
		MessageBox( "�����侵��ϳ���ͼ��Ϣ��������" );
		return;
	}

	if ( mNormalDisp.IsEmpty() )
	{
		MessageBox( "����λ�ƺϳ���ͼ��Ϣ��������" );
		return;
	}

	Ogre::Image combined;

	combined.loadTwoImagesAsRGBA( mDiffuse.GetBuffer(), mSpecular.GetBuffer(), 
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::PF_BYTE_RGBA);
	combined.save(mDiffuseSpecular.GetBuffer());

	combined.loadTwoImagesAsRGBA( mNormal.GetBuffer(), mDisp.GetBuffer(),
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::PF_BYTE_RGBA);
	combined.save(mNormalDisp.GetBuffer());

	MessageBox( "�ر���ͼ�ϲ���ɣ�" );

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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
