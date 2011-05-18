

#include "stdafx.h"
#include "../EditTool.h"
#include "../Editor.h"
#include "../Resource.h"
#include "../TerrainModifyDialog.h"
#include "TerrainModifyTool.h"

IMPLEMENT_DYNCREATE(CTerrainModifyTool, CEditTool)

CTerrainModifyTool::CTerrainModifyTool() 
	: mBrushType(E_TERRAIN_BRUSH_NULL_TYPE),
	mbBeginEdit(false),
	mBrushSize(0.02f),
	mHeightUpdateCountDown(0),
	mTimeLastFrame(0),
	mBrushHardness(0.001f)
{
	m_iCurrentPageId = 0;
	m_pTerrainCtrl = GetEditor()->GetRollupCtrl(E_TERRAIN_CTRL);

	mName = "地形修改工具";

}

CTerrainModifyTool::~CTerrainModifyTool()
{

}

void CTerrainModifyTool::BeginTool()
{
	// TODO: 在此添加控件通知处理程序代码

	m_iCurrentPageId = m_pTerrainCtrl->InsertPage("修改地形", IDD_TERRAIN_MODIFY_DIALOG, RUNTIME_CLASS(CTerrainModifyDialog), 2);
	m_pTerrainCtrl->ExpandPage(m_iCurrentPageId);

	mEditDecal.Create( "DecalEdit", "DecalEdit", 16 );
	//mEditDecal.SetScale( mBrushSize*TERRAIN_WORLD_SIZE * 0.5f );

	// Update terrain at max 20fps
	mHeightUpdateRate = 50;
	mTimeLastFrame = Ogre::Root::getSingletonPtr()->getTimer()->getMilliseconds();
}

void CTerrainModifyTool::EndTool()
{
	// TODO: 在此添加控件通知处理程序代码
	if ( m_iCurrentPageId )
	{
		m_pTerrainCtrl->RemovePage(m_iCurrentPageId);
	}
	mEditDecal.Destroy();
}

void CTerrainModifyTool::Update()
{
	unsigned long newTime = Ogre::Root::getSingletonPtr()->getTimer()->getMilliseconds();

	unsigned long timeSinceLastFrame = newTime - mTimeLastFrame;
	if (mHeightUpdateCountDown > 0)
	{
		mHeightUpdateCountDown -= timeSinceLastFrame;
		if (mHeightUpdateCountDown <= 0)
		{
			GetEditor()->GetTerrainGroup()->update();
			mHeightUpdateCountDown = 0;
		}
	}
	mTimeLastFrame = newTime;
}

void CTerrainModifyTool::OnLButtonDown(UINT nFlags, CPoint point)
{
	mbBeginEdit = true;
}

void CTerrainModifyTool::OnLButtonUp(UINT nFlags, CPoint point)
{
	mbBeginEdit = false;
}

void CTerrainModifyTool::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//处理键盘响应
	switch(nChar)
	{
		//esc键
	case VK_ESCAPE:
		{

		}
		break;
	case VK_LEFT:
		break;
	case VK_RETURN:
		break;
	}
}

void CTerrainModifyTool::OnMouseMove(UINT nFlags, CPoint point)
{
	Ogre::TerrainGroup::RayResult rayResult = GetEditor()->TerrainHitTest( point );
	if (rayResult.hit)
	{
		mEditDecal.setPosition(rayResult.position.x, rayResult.position.z, 2.0f);

		if ( mBrushType == E_TERRAIN_BRUSH_NULL_TYPE )
			return;

		if ( mbBeginEdit )
		{
			// figure out which terrains this affects
			TerrainGroup::TerrainList terrainList;
			Real brushSizeWorldSpace = TERRAIN_WORLD_SIZE * mBrushSize;
			Sphere sphere(rayResult.position, brushSizeWorldSpace);
			GetEditor()->GetTerrainGroup()->sphereIntersects(sphere, &terrainList);

			for (TerrainGroup::TerrainList::iterator it = terrainList.begin();
				it != terrainList.end(); ++it)
			{
				Terrain* terrain = *it;
				Vector3 tsPos;
				terrain->getTerrainPosition(rayResult.position, &tsPos);

				// we need point coords
				Real terrainSize = (terrain->getSize() - 1);
				long startx = (tsPos.x - mBrushSize) * terrainSize;
				long starty = (tsPos.y - mBrushSize) * terrainSize;
				long endx = (tsPos.x + mBrushSize) * terrainSize;
				long endy= (tsPos.y + mBrushSize) * terrainSize;
				startx = std::max(startx, 0L);
				starty = std::max(starty, 0L);
				endx = std::min(endx, (long)terrainSize);
				endy = std::min(endy, (long)terrainSize);
				for (long y = starty; y <= endy; ++y)
				{
					for (long x = startx; x <= endx; ++x)
					{
						Real tsXdist = (x / terrainSize) - tsPos.x;
						Real tsYdist = (y / terrainSize)  - tsPos.y;

						Real weight = std::min((Real)1.0, 
							Math::Sqrt(tsYdist * tsYdist + tsXdist * tsXdist) / Real(0.5 * mBrushSize));
						weight = 1.0 - (weight * weight);

						float addedHeight = weight * 250.0 * mBrushHardness;
						float newheight;

						switch( mBrushType )
						{
						case E_TERRAIN_RISE:
							{
								newheight = terrain->getHeightAtPoint(x, y) + addedHeight;
							}
							break;
						case E_TERRAIN_LOWER:
							{
								newheight = terrain->getHeightAtPoint(x, y) - addedHeight;
								if( newheight < 0.0f )
									newheight = 0.0f;
							}
							break;
						}

						terrain->setHeightAtPoint(x, y, newheight);
					}
				}
			}
			if (mHeightUpdateCountDown == 0)
				mHeightUpdateCountDown = mHeightUpdateRate;
		}
	}
}

void CTerrainModifyTool::SetBrushHardness( Real hardness )
{
	mBrushHardness = hardness;
}

void CTerrainModifyTool::SetBrushSize( Real brushSize )
{
	mBrushSize = brushSize;
	mEditDecal.SetScale( mBrushSize*TERRAIN_WORLD_SIZE * 0.5f );
}

void CTerrainModifyTool::SetBrushType( E_TERRAIN_BRUSH_TYPE type )
{
	mBrushType = type;
}
