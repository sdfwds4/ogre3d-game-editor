// OgreEditorDoc.cpp : implementation of the CTerrainEditorDoc class
//

#include "stdafx.h"
#include "OgreEditor.h"

#include "OgreEditorDoc.h"
#include "Editor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COgreEditorDoc

IMPLEMENT_DYNCREATE(COgreEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(COgreEditorDoc, CDocument)
	ON_COMMAND( ID_SELECT_MODE, OnSetSelectMode )
	ON_COMMAND( ID_MOVE_MODE, OnSetMoveMode )
	ON_COMMAND( ID_ROTATE_MODE, OnSetRotateMode )
	ON_COMMAND( ID_SCALE_MODE, OnSetScaleMode )
	ON_UPDATE_COMMAND_UI_RANGE(ID_SELECT_MODE, ID_SCALE_MODE, OnUpdateToolModeButton)
END_MESSAGE_MAP()


// COgreEditorDoc construction/destruction

COgreEditorDoc::COgreEditorDoc() : mToolModeId(ID_SELECT_MODE)
{
	// TODO: add one-time construction code here

}

COgreEditorDoc::~COgreEditorDoc()
{
}

BOOL COgreEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	GetEditor()->CreateLevel();
	return TRUE;
}

BOOL COgreEditorDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	GetEditor()->Load( lpszPathName );
	return TRUE;
}

BOOL COgreEditorDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnSaveDocument(lpszPathName))
		return FALSE;

	GetEditor()->SaveLevel();

	return TRUE;
}

void COgreEditorDoc::OnCloseDocument()
{
	CDocument::OnCloseDocument();

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

}

void COgreEditorDoc::OnSetSelectMode()
{
	GetEditor()->SetToolMode( TOOL_SELECT );
	mToolModeId = ID_SELECT_MODE;
}

void COgreEditorDoc::OnSetMoveMode()
{
	GetEditor()->SetToolMode( TOOL_MOVE );
	mToolModeId = ID_MOVE_MODE;
}

void COgreEditorDoc::OnSetRotateMode()
{
	GetEditor()->SetToolMode( TOOL_ROTATE );
	mToolModeId = ID_ROTATE_MODE;
}

void COgreEditorDoc::OnSetScaleMode()
{
	GetEditor()->SetToolMode( TOOL_SCALE );
	mToolModeId = ID_SCALE_MODE;
}

void COgreEditorDoc::OnUpdateToolModeButton(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(pCmdUI->m_nID == mToolModeId ? TRUE : FALSE);
}

// COgreEditorDoc serialization

void COgreEditorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// COgreEditorDoc diagnostics

#ifdef _DEBUG
void COgreEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void COgreEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// COgreEditorDoc commands
