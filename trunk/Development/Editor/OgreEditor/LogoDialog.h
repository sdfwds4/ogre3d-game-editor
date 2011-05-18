#pragma once
#include "afxwin.h"


// CLogoDialog 对话框

class CLogoDialog : public CDialog, public Ogre::ResourceGroupListener
{
	DECLARE_DYNAMIC(CLogoDialog)
	friend class Ogre::Root;
public:
	CLogoDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLogoDialog();

// 对话框数据
	enum { IDD = IDD_LOGO_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	BOOL OnInitDialog();

	//----------------------------------------------------------
	//资源加载
	void resourceGroupScriptingStarted(const Ogre::String& groupName, size_t scriptCount)
	{
		m_LogText.SetWindowText( "解析资源..." );
		m_LogText.Invalidate();
		m_LogText.UpdateWindow();
	}

	void scriptParseStarted(const Ogre::String& scriptName, bool& skipThisScript)
	{
		CString logText;
		logText = "解析资源:";
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
		m_LogText.SetWindowText( "加载资源..." );
		m_LogText.Invalidate();
		m_LogText.UpdateWindow();
	}

	void resourceLoadStarted(const Ogre::ResourcePtr& resource)
	{
		CString logText;
		logText = "加载资源:";
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
