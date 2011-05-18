// TerrainAddLayerDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OgreEditor.h"
#include "TerrainAddLayerDialog.h"


// CTerrainAddLayerDialog �Ի���

IMPLEMENT_DYNAMIC(CTerrainAddLayerDialog, CDialog)

CTerrainAddLayerDialog::CTerrainAddLayerDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTerrainAddLayerDialog::IDD, pParent), mWorldSize(100)
{
	mTexture1 = "";
	mTexture2 = "";
}

CTerrainAddLayerDialog::~CTerrainAddLayerDialog()
{
}

void CTerrainAddLayerDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SIZE_EDIT, mWorldSize);
	DDV_MinMaxInt(pDX,mWorldSize, 10, 1000);
	DDX_Text(pDX, IDC_TEXTURE1_EDIT, mTexture1);
	DDX_Text(pDX, IDC_TEXTURE2_EDIT, mTexture2);

}


BEGIN_MESSAGE_MAP(CTerrainAddLayerDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CTerrainAddLayerDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CTerrainAddLayerDialog ��Ϣ�������

void CTerrainAddLayerDialog::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();

	if ( mTexture1.IsEmpty() )
	{
		MessageBox( "��ͼ��Ϣ��������" );
	}

	OnOK();
}
