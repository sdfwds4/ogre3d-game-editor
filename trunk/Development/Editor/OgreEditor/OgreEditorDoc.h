// OgreEditorDoc.h : interface of the CTerrainEditorDoc class
//


#pragma once


class COgreEditorDoc : public CDocument
{
protected: // create from serialization only
	COgreEditorDoc();
	DECLARE_DYNCREATE(COgreEditorDoc)

// Attributes
public:

// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();

	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~COgreEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	afx_msg void OnSetSelectMode();
	afx_msg void OnSetMoveMode();
	afx_msg void OnSetRotateMode();
	afx_msg void OnSetScaleMode();
	afx_msg void OnUpdateToolModeButton(CCmdUI* pCmdUI);

	UINT mToolModeId;
// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};
