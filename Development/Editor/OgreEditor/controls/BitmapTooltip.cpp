

#include "stdafx.h"
#include "BitmapTooltip.h"

IMPLEMENT_DYNAMIC(CBitmapTooltip, CWnd)

BEGIN_MESSAGE_MAP(CBitmapTooltip, CWnd)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

CBitmapTooltip::CBitmapTooltip( CRect& rect )
{
	// Register your unique class name that you wish to use
	CString strMyClass;

	// load stock cursor, brush, and icon for
	// my own window class

	try
	{
		strMyClass = AfxRegisterWndClass(
			CS_VREDRAW | CS_HREDRAW,
			::LoadCursor(NULL, IDC_ARROW),
			(HBRUSH) ::GetStockObject(NULL_BRUSH),
			NULL);
	}
	catch (CResourceException* pEx)
	{
		AfxMessageBox(
			_T("Couldn't register class! (Already registered?)"));
		pEx->Delete();
	}

	BOOL hRes = CreateEx( WS_EX_TOOLWINDOW  | WS_EX_TOPMOST, strMyClass, "", WS_VISIBLE | WS_POPUP | WS_BORDER , rect, GetDesktopWindow(), 0 );

	if ( hRes )
	{
		ModifyStyle(WS_CAPTION,0);
		SetMenu(NULL);
		CRect rectCL;
		GetClientRect(rectCL);
		m_Image.Create(_T(""),  WS_CHILD|WS_VISIBLE|SS_BITMAP, rectCL,this, 0); 
		m_Text.Create( "", WS_CHILD | WS_VISIBLE | SS_LEFT, CRect( 0, 0, 0, 0 ), this, 1 );

		m_Image.ModifyStyleEx( 0, WS_EX_STATICEDGE );
		m_Text.ModifyStyleEx( 0, WS_EX_STATICEDGE );

	}

	LoadImage("nskingr.jpg");
}

CBitmapTooltip::~CBitmapTooltip()
{
	if ( m_Bitmap.m_hObject )
	{
		m_Bitmap.DeleteObject();
	}
}

bool CBitmapTooltip::LoadImage( const CString& imageFileName )
{
	if ( !m_hWnd )
		return false;

	if ( m_FileName == imageFileName )
		return true;

	m_FileName = imageFileName;
	Ogre::Image image;
	image.load(m_FileName.GetBuffer(), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	CString imgInfo;
	imgInfo.Format( "%dX%d (%d) %s", image.getWidth(), image.getHeight(), image.getBPP(), 
		Ogre::PixelUtil::getFormatName( image.getPixelBox().format ).c_str() );
	m_Text.SetWindowText( imgInfo );

	int w = image.getWidth();
	int h = image.getHeight();
	if ( w > 256 || w == 0 )
	{
		w = 256;
	}

	if ( h > 256 || h == 0 )
	{
		h = 256;
	}
	w = 256;
	h = 256;

	CRect rc;
	GetWindowRect(rc);

	if ( rc.Width() != w || rc.Height() != h )
	{
		SetWindowPos( NULL, 0, 0, w + 4, h + 4 + 20, SWP_NOMOVE );
	}
	ShowWindow(SW_SHOW);

	CRect bmpRc;
	m_Image.GetClientRect( bmpRc );
	w = bmpRc.Width();
	h = bmpRc.Height();

	//image.resize( w, h );
	w = image.getWidth();
	h = image.getHeight();

	Ogre::PixelBox* pixel_box = &image.getPixelBox();

	int rowSpan = image.getBPP() / 8;
	const int nPixelCount = w * h;
	const unsigned char *pPBData = (unsigned char *)pixel_box->data;

	Ogre::uint8 *pImage = new Ogre::uint8[w * h*4];
	Ogre::uint8 r, g, b, a;

	for (int i=0; i<nPixelCount; i++)
	{
		Ogre::PixelUtil::unpackColour(&r, &g, &b, &a, pixel_box->format, pPBData + i*rowSpan);
		if ( pixel_box->format == Ogre::PF_A8R8G8B8 )
		{
			pImage[i*4] = b;
			pImage[i*4+1] = g;
			pImage[i*4+2] = r;
			pImage[i*4+3] = a;
		}
		else
		{
			pImage[i*4] = b;
			pImage[i*4+1] = g;
			pImage[i*4+2] = r;
			pImage[i*4+3] = a;
		}

	}

	BOOL bOk = m_Bitmap.CreateBitmap( w, h, 1, 32, pImage );

	delete[] pImage;
	//if ( m_Bitmap.m_hObject )
	//{
	//	m_Image.SetBitmap( m_Bitmap );
	//}
	//else
	//{
	//	m_Image.SetBitmap( NULL );
	//}
	m_Image.SetBitmap( NULL );
	RedrawWindow();
	Invalidate();
	return true;
}

void CBitmapTooltip::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType,cx,cy);

	if ( m_Image.m_hWnd )
	{
		CRect rcClient;
		GetClientRect(rcClient);
		CRect rc(rcClient);
		rc.bottom -= 20;
		m_Image.MoveWindow(rc,FALSE);
		rc = rcClient;
		rc.top = rc.bottom - 20;
		m_Text.MoveWindow(rc, FALSE);
	}
}

BOOL CBitmapTooltip::OnEraseBkgnd(CDC* pDC) 
{
	// "Tile" bitmap (see demo for actual code)
	return TRUE; // tell Windows we handled it
}

void CBitmapTooltip::OnPaint()
{
	CPaintDC dc(this);

	HBITMAP hBitmap = m_Bitmap;
	if( hBitmap )
	{
		CDC bitmapDC;
		CRect rect;
		GetClientRect(rect);
		dc.FillSolidRect(&rect,RGB(125, 125, 125));

		if (bitmapDC.CreateCompatibleDC(&dc))
		{
			BITMAP bm;
			m_Bitmap.GetBitmap(&bm);
			CBitmap* pOldBmp = bitmapDC.SelectObject(&m_Bitmap);
			dc.StretchBlt(1,1,257,258,&bitmapDC, 0,0,bm.bmWidth,bm.bmHeight, SRCCOPY);
			bitmapDC.SelectObject(pOldBmp);
		}
	}
}
