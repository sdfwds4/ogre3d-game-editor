

#pragma once

#include "../GdiView.h"

class CTextureTool : public CEditTool
{
public:
	DECLARE_DYNCREATE(CTextureTool)

	CTextureTool();
	~CTextureTool();

	void BeginTool();
	void EndTool();
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);
	void OnMouseMove(UINT nFlags, CPoint point);
	void OnRButtonDown(UINT nFlags, CPoint point);
	void OnRButtonUp(UINT nFlags, CPoint point);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void OnPaint();
	void OnTimer(UINT_PTR nIDEvent);

	void SetParent( CWnd* pWnd )
	{
		mpParent = pWnd;
	}

	void FromName( const CString& fileName )
	{
		LoadImage(fileName);
	}


	bool LoadImage( const CString& imageFileName );

protected:

	CWnd* mpParent;

	CPoint mLbuttonDownPos;
	CPoint mLbuttonMovePos;
	CPoint mRbuttonDownPos;
	CPoint mRbuttonMovePos;
	CPoint mScrollOffset;

	UINT m_nIDEvent;
	bool mbDirty;

	CString mImageName;
	Gdiplus::Bitmap* mpBitmap;

};