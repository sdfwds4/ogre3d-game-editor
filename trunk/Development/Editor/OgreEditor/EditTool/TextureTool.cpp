

#include "stdafx.h"
#include "../EditTool.h"
#include "../Editor.h"
#include "TextureTool.h"
using namespace Gdiplus;

#define MAX_MOVE_SPEED 18

IMPLEMENT_DYNCREATE(CTextureTool, CEditTool)

CTextureTool::CTextureTool() : mpParent(NULL), mpBitmap(NULL)
{

}

CTextureTool::~CTextureTool()
{

}

void CTextureTool::BeginTool()
{
	// TODO: 在此添加控件通知处理程序代码
	mLbuttonDownPos.SetPoint( 0, 0 );
	mLbuttonMovePos.SetPoint( 0, 0 );
	mRbuttonDownPos.SetPoint( 0, 0 );
	mRbuttonMovePos.SetPoint( 0, 0 );
	mScrollOffset.SetPoint( 0, 0 );

	mbDirty = false;

	mpParent->SetTimer(m_nIDEvent, 20, NULL);
}

void CTextureTool::EndTool()
{
	mpParent->KillTimer(m_nIDEvent);

	if ( mpBitmap )
	{
		::delete mpBitmap;
	}
}

void CTextureTool::OnLButtonDown(UINT nFlags, CPoint point)
{
	mLbuttonDownPos = point;

}

void CTextureTool::OnLButtonUp(UINT nFlags, CPoint point)
{
	mLbuttonDownPos.SetPoint(0,0);
	mLbuttonMovePos.SetPoint(0,0);
}


void CTextureTool::OnMouseMove(UINT nFlags, CPoint point)
{
	if ( nFlags & MK_RBUTTON )
	{
		if ( point.x - mRbuttonMovePos.x > MAX_MOVE_SPEED )
			mScrollOffset.x += MAX_MOVE_SPEED;
		else if ( point.x - mRbuttonMovePos.x < -MAX_MOVE_SPEED )
			mScrollOffset.x -= MAX_MOVE_SPEED;
		else
			mScrollOffset.x += point.x - mRbuttonMovePos.x;

		if ( point.y - mRbuttonMovePos.y > MAX_MOVE_SPEED )
			mScrollOffset.y += MAX_MOVE_SPEED;
		else if ( point.y - mRbuttonMovePos.y < -MAX_MOVE_SPEED )
			mScrollOffset.y -= MAX_MOVE_SPEED;
		else
			mScrollOffset.y += point.y - mRbuttonMovePos.y;
		mRbuttonMovePos = point;
		mbDirty = true;
	}
}

void CTextureTool::OnRButtonDown(UINT nFlags, CPoint point)
{
	mRbuttonDownPos = point;
	mRbuttonMovePos = point;
}

void CTextureTool::OnRButtonUp(UINT nFlags, CPoint point)
{
	mRbuttonDownPos.SetPoint(0,0);
	mRbuttonMovePos.SetPoint(0,0);
}

void CTextureTool::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{

}

void CTextureTool::OnPaint()
{
	CPaintDC paintDC(mpParent);
	Graphics graphics(paintDC.GetSafeHdc());
	CRect rc = paintDC.m_ps.rcPaint;
	graphics.Clear(Color::Gray);
	graphics.TranslateTransform(mScrollOffset.x - rc.left, mScrollOffset.y - rc.top);

	if ( mpBitmap )
	{
		int posX = (rc.right - mpBitmap->GetWidth())*0.5;
		int posY = (rc.bottom - mpBitmap->GetHeight())* 0.5;
		graphics.DrawImage( mpBitmap, posX, posY);
	}

}

void CTextureTool::OnTimer(UINT_PTR nIDEvent) 
{
	if (m_nIDEvent == nIDEvent)
	{
		if ( mbDirty )
		{
			mpParent->Invalidate();
			mbDirty = false;
		}
	}
}

bool CTextureTool::LoadImage( const CString& imageFileName )
{
	if ( !mpParent )
		return false;

	int pos = imageFileName.FindOneOf( "." );
	if ( pos == -1 )
	{
		return false;
	}

	if ( mImageName == imageFileName )
		return true;

	mImageName = imageFileName;
	Ogre::Image image;
	image.load(mImageName.GetBuffer(), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	int w = image.getWidth();
	int h = image.getHeight();

	Ogre::PixelBox* pixel_box = &image.getPixelBox();

	int rowSpan = image.getBPP() / 8;
	const int nPixelCount = w * h;
	const unsigned char *pPBData = (unsigned char *)pixel_box->data;

	Ogre::uint8 r, g, b, a;
	if ( mpBitmap )
	{
		::delete mpBitmap;
	}
	mpBitmap = ::new Bitmap( w, h );

	for ( int i = 0; i < w; i++ )
	{
		for ( int j = 0; j < h; j++ )
		{
			int pos = j * w + i;
			Ogre::PixelUtil::unpackColour(&r, &g, &b, &a, pixel_box->format, pPBData + pos*rowSpan);

			mpBitmap->SetPixel( i, j, Color(a, r, g, b) );
		}
	}
	mbDirty = true;
	return true;
}