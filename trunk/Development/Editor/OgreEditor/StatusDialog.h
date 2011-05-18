#pragma once

#include "OgreEditor.h"
//#include "f:\unrealengine\development\sdks\xt\source\controls\xtpspinbuttonctrl.h"
//#include "afxwin.h"

// CStatusDialog 对话框
using namespace Ogre;
class CStatusDialog : public CDialog
{
	DECLARE_DYNAMIC(CStatusDialog)

public:
	CStatusDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStatusDialog();

// 对话框数据
	enum { IDD = IDD_STATUS_DIALOG };

	void SetToolName( const CString& name );
	void SetEditPos( Vector3& pos );
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	BOOL OnInitDialog();
	void OnOK();
	void OnCancel();

	void SetEditorTopSpeed( Ogre::Real speed );

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	CString mToolName;
	Ogre::Real mX;
	Ogre::Real mY;
	Ogre::Real mZ;

	Ogre::Real mfTopSpeed;;
public:
	CXTPSpinButtonCtrl mSpeedSpinButton;
	CEdit mSpeedEdit;
	afx_msg void OnDeltaposSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditSpin();
};
