// LayerDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OgreEditor.h"
#include "LayerDialog.h"


// CLayerDialog �Ի���

IMPLEMENT_DYNCREATE(CLayerDialog, CDialog)

CLayerDialog::CLayerDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CLayerDialog::IDD, pParent)
{

}

CLayerDialog::~CLayerDialog()
{
}

void CLayerDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLayerDialog, CDialog)
END_MESSAGE_MAP()


// CLayerDialog ��Ϣ�������
