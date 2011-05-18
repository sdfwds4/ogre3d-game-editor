// StatusDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OgreEditor.h"
#include "Editor.h"
#include "StatusDialog.h"


// CStatusDialog �Ի���

IMPLEMENT_DYNAMIC(CStatusDialog, CDialog)

CStatusDialog::CStatusDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CStatusDialog::IDD, pParent), mX(0.0f), mY(0.0f), mZ(0.0f), mfTopSpeed(180.0f)
{
	mToolName = "";
}

CStatusDialog::~CStatusDialog()
{
}

void CStatusDialog::SetToolName( const CString& name )
{
	mToolName = name;
	UpdateData(FALSE);
}

void CStatusDialog::SetEditPos( Vector3& pos )
{
	mX = pos.x;
	mY = pos.y;
	mZ = pos.z;
	UpdateData(FALSE);
}


void CStatusDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITTOOL_NAME, mToolName);
	DDX_Text(pDX, IDC_X_EDIT, mX);
	DDX_Text(pDX, IDC_Y_EDIT, mY);
	DDX_Text(pDX, IDC_Z_EDIT, mZ);
	DDX_Control(pDX, IDC_SPIN, mSpeedSpinButton);
	DDX_Control(pDX, IDC_EDIT_SPIN, mSpeedEdit);

	DDX_Text(pDX, IDC_EDIT_SPIN, mfTopSpeed);
}

BOOL CStatusDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//mSpeedSpinButton.SetBuddy( &mSpeedEdit );
	return TRUE;
}

void CStatusDialog::OnOK()
{
	UpdateData();
	mfTopSpeed = Math::Clamp(mfTopSpeed, 1.0f, 500.0f);
	UpdateData(FALSE);

	SetEditorTopSpeed(mfTopSpeed);
}

void CStatusDialog::OnCancel()
{

}

BEGIN_MESSAGE_MAP(CStatusDialog, CDialog)
	ON_WM_CREATE()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN, &CStatusDialog::OnDeltaposSpin)
	ON_EN_CHANGE(IDC_EDIT_SPIN, &CStatusDialog::OnEnChangeEditSpin)
END_MESSAGE_MAP()


// CStatusDialog ��Ϣ�������

int CStatusDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// TODO: Remove this lihe if you don't want intelligent menus.

	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	GetEditor()->SetStatusDialog( this );

	return 0;
}
void CStatusDialog::OnDeltaposSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(pNMUpDown->iDelta > 0)
	{
		mfTopSpeed -= pNMUpDown->iDelta * 1.0f;
	}
	else if(pNMUpDown->iDelta < 0)
	{
		mfTopSpeed -= pNMUpDown->iDelta * 1.0f;
	}

	mfTopSpeed = Math::Clamp(mfTopSpeed, 1.0f, 500.0f);
	CString log;
	log.Format( "Delta = %d", pNMUpDown->iDelta );
	GetEditor()->Log(log);
	UpdateData(FALSE);

	*pResult = 0;
}

void CStatusDialog::OnEnChangeEditSpin()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	mfTopSpeed = Math::Clamp(mfTopSpeed, 1.0f, 500.0f);

	SetEditorTopSpeed(mfTopSpeed);
}

void CStatusDialog::SetEditorTopSpeed( Ogre::Real speed )
{
	GetEditor()->SetTopSpeed(speed);
}