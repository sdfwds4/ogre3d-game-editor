// TerrainLightDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Editor.h"
#include "OgreEditor.h"
#include "TerrainLightDialog.h"

// CTerrainLightDialog �Ի���

IMPLEMENT_DYNCREATE(CTerrainLightDialog, CDialog)

CTerrainLightDialog::CTerrainLightDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTerrainLightDialog::IDD, pParent), mTerrainLight(NULL)
{

}

CTerrainLightDialog::~CTerrainLightDialog()
{
}

void CTerrainLightDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_X_EDIT, mXEdit);
	DDX_Control(pDX, IDC_Y_EDIT, mYEdit);
	DDX_Control(pDX, IDC_Z_EDIT, mZEdit);

	DDX_Text(pDX, IDC_X_EDIT, mDir.x);
	DDX_Text(pDX, IDC_Y_EDIT, mDir.y);
	DDX_Text(pDX, IDC_Z_EDIT, mDir.z);

}

BOOL CTerrainLightDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	mTerrainLight = GetEditor()->GetSceneManager()->getLight( "terrainLight" );
	mDir = mTerrainLight->getDerivedDirection();
	UpdateData(FALSE);
	return TRUE;
}



BEGIN_MESSAGE_MAP(CTerrainLightDialog, CDialog)
	ON_BN_CLICKED(IDC_APPLY_BTN, &CTerrainLightDialog::OnBnClickedApplyBtn)
END_MESSAGE_MAP()


// CTerrainLightDialog ��Ϣ�������

void CTerrainLightDialog::OnBnClickedApplyBtn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	UpdateData(TRUE);
	mTerrainLight->setDirection( mDir );
	GetEditor()->GetTerrainGlobalOptions()->setLightMapDirection(mDir);
	//GetEditor()->GetTerrainGroup()->update();
	//GetEditor()->GetTerrainGroup()->getTerrain(0,0)->dirtyLightmap();
	//GetEditor()->GetTerrainGroup()->updateDerivedData(true);
}
