// TerrainEditor.h : main header file for the TerrainEditor application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

// COgreEditorApp:
// See TerrainEditor.cpp for the implementation of this class
//

class COgreEditorApp : public CWinApp
{
public:
	COgreEditorApp();
	~COgreEditorApp();

	ULONG_PTR  mGdiplusToken;
	HINSTANCE mBlackBox;
// Overrides
public:
	virtual BOOL InitInstance();
	virtual BOOL ExitInstance();
	BOOL OnIdle(LONG lCount);

	// Implementation
	afx_msg void OnAppAbout();
	afx_msg void OnViewDatabase();
	afx_msg void OnViewParticle();
	afx_msg void OnViewActor();

	afx_msg void OnFileNew();
	DECLARE_MESSAGE_MAP()
};

extern COgreEditorApp theApp;