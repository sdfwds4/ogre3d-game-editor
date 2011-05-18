

#pragma once

class CBitmapTooltip : public CWnd
{
public:
	DECLARE_DYNAMIC(CBitmapTooltip)

	CBitmapTooltip( CRect& rect );
	~CBitmapTooltip();

	bool LoadImage( const CString& imageFileName );

	CStatic m_Image;
	CStatic m_Text;
	CBitmap m_Bitmap;
	CString m_FileName;

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual void OnPaint(); 

	DECLARE_MESSAGE_MAP()

};