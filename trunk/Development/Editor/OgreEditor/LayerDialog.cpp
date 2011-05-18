// LayerDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "OgreEditor.h"
#include "LayerDialog.h"


// CLayerDialog 对话框

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


// CLayerDialog 消息处理程序
