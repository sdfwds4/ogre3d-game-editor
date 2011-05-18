#pragma once
#include "afxwin.h"


// CLogoDialog �Ի���

class CLogoDialog : public CDialog, public Ogre::ResourceGroupListener
{
	DECLARE_DYNAMIC(CLogoDialog)
	friend class Ogre::Root;
public:
	CLogoDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLogoDialog();

// �Ի�������
	enum { IDD = IDD_LOGO_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	BOOL OnInitDialog();

	//----------------------------------------------------------
	//��Դ����
	void resourceGroupScriptingStarted(const Ogre::String& groupName, size_t scriptCount)
	{
		m_LogText.SetWindowText( "������Դ..." );
		m_LogText.Invalidate();
		m_LogText.UpdateWindow();
	}

	void scriptParseStarted(const Ogre::String& scriptName, bool& skipThisScript)
	{
		CString logText;
		logText = "������Դ:";
		logText += scriptName.c_str();
		m_LogText.SetWindowText( logText );
		m_LogText.Invalidate();
		m_LogText.UpdateWindow();
	}

	void scriptParseEnded(const Ogre::String& scriptName, bool skipped)
	{
	}

	void resourceGroupScriptingEnded(const Ogre::String& groupName) {}

	void resourceGroupLoadStarted(const Ogre::String& groupName, size_t resourceCount)
	{
		m_LogText.SetWindowText( "������Դ..." );
		m_LogText.Invalidate();
		m_LogText.UpdateWindow();
	}

	void resourceLoadStarted(const Ogre::ResourcePtr& resource)
	{
		CString logText;
		logText = "������Դ:";
		logText += resource->getName().c_str();
		m_LogText.SetWindowText( logText );
		m_LogText.Invalidate();
		m_LogText.UpdateWindow();
	}

	void resourceLoadEnded()
	{
	}

	void worldGeometryStageStarted(const Ogre::String& description)
	{
	}

	void worldGeometryStageEnded()
	{
	}

	void resourceGroupLoadEnded(const Ogre::String& groupName) {}

	afx_msg void OnSetFocus(CWnd* pOldWnd);
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_LogoPic;
	CStatic m_LogText;
};
