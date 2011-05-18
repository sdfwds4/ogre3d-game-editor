// OgreEditor.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "OgreEditor.h"
#include "MainFrm.h"

#include "OgreEditorDoc.h"
#include "BladeView.h"

#include "Editor.h"
#include "LogoDialog.h"

#include "ParticleViewDoc.h"
#include "ParticleRenderView.h"
#include <Common/Base/Ext/hkBaseExt.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Gdiplus;

// COgreEditorApp

BEGIN_MESSAGE_MAP(COgreEditorApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &COgreEditorApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	ON_COMMAND(ID_VIEW_DATABASE, &COgreEditorApp::OnViewDatabase)
	ON_COMMAND(ID_VIEW_PARTICLE, &COgreEditorApp::OnViewParticle)

END_MESSAGE_MAP()


// COgreEditorApp construction

COgreEditorApp::COgreEditorApp() : mBlackBox(NULL)
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

COgreEditorApp::~COgreEditorApp()
{
	hkBaseSystem::quit();
	hkMemoryInitUtil::quit();
	extAllocator::quit();
}


// The one and only COgreEditorApp object

COgreEditorApp theApp;


// COgreEditorApp initialization

BOOL COgreEditorApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

#ifndef _DEBUG
	mBlackBox = LoadLibrary( "BlackBox.dll" );
#endif

	GdiplusStartupInput gdiplusStartupInput;
	// Initialize GDI+.
	GdiplusStartup(&mGdiplusToken, &gdiplusStartupInput, NULL);


	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(COgreEditorDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CBladeView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	pDocTemplate = new CSingleDocTemplate(
		IDR_PARTICLE_EDIT,
		RUNTIME_CLASS(CParticleViewDoc),
		RUNTIME_CLASS(CParticleDialog),       // main SDI frame window
		RUNTIME_CLASS(CParticleRenderView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	//CCommandLineInfo cmdInfo;
	//ParseCommandLine(cmdInfo);

	CLogoDialog logo;
	logo.Create( CLogoDialog::IDD );
	logo.ShowWindow(SW_SHOW);
	logo.UpdateWindow();

	GetEditor()->PreLoadResources();

	// TODO: Add your specialized code here and/or call the base class
	Ogre::ResourceGroupManager::getSingleton().addResourceGroupListener(&logo);

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	//if (!ProcessShellCommand(cmdInfo))
	//	return FALSE;

	OnFileNew();

	//CMainFrame* pMainFrame = new CMainFrame;
	//if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
	//	return FALSE;
	//m_pMainWnd = pMainFrame;

	//// The one and only window has been initialized, so show and update it
	//m_pMainWnd->ShowWindow(SW_SHOW);
	//m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	return TRUE;
}


BOOL COgreEditorApp::ExitInstance()
{
	CWinApp::ExitInstance();
	GdiplusShutdown(mGdiplusToken);

#ifndef _DEBUG
	if ( NULL != mBlackBox ) 
	{
		FreeLibrary( mBlackBox );
	}
#endif

	return TRUE;
}

BOOL COgreEditorApp::OnIdle(LONG lCount)
{
	CWinApp::OnIdle(lCount);

	GetEditor()->UpDate();
	return TRUE;
}




// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void COgreEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void COgreEditorApp::OnViewDatabase()
{
	GetEditor()->ShowDatabaseDialog(true);
}

void COgreEditorApp::OnViewParticle()
{
	//GetEditor()->ShowParticleDialog(true);

	POSITION curTemplatePos = GetFirstDocTemplatePosition();

	while(curTemplatePos != NULL)
	{
		CDocTemplate* curTemplate =
			GetNextDocTemplate(curTemplatePos);
		CString str;
		curTemplate->GetDocString(str, CDocTemplate::docName);
		if(str == _T("ParticleEditor"))
		{
			curTemplate->OpenDocumentFile(NULL);
			return;
		}
	}
}

void COgreEditorApp::OnViewActor()
{
}

void COgreEditorApp::OnFileNew()
{
	POSITION curTemplatePos = GetFirstDocTemplatePosition();

	while(curTemplatePos != NULL)
	{
		CDocTemplate* curTemplate =
			GetNextDocTemplate(curTemplatePos);
		CString str;
		curTemplate->GetDocString(str, CDocTemplate::docName);
		if(str == _T("OgreEditor"))
		{
			curTemplate->OpenDocumentFile(NULL);
			return;
		}
	}
}


// COgreEditorApp message handlers

