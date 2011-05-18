

#include "stdafx.h"
#include "../EditTool.h"
#include "../Editor.h"
#include "../Resource.h"
#include "../TerrainPaintDialog.h"
#include "TerrainPaintTool.h"

IMPLEMENT_DYNCREATE(CTerrainPaintTool, CEditTool)

CTerrainPaintTool::CTerrainPaintTool() 
	: mBrushType(E_TERRAIN_PAINT_NULL_TYPE),
	mbBeginEdit(false),
	mBrushSize(0.02f),
	mHeightUpdateCountDown(0),
	mTimeLastFrame(0),
	mBrushHardness(0.001f),
	mLayerEdit(0)
{
	m_iCurrentPageId = 0;
	m_pTerrainCtrl = GetEditor()->GetRollupCtrl(E_TERRAIN_CTRL);

	mName = "地表绘制工具";
}

CTerrainPaintTool::~CTerrainPaintTool()
{

}

void CTerrainPaintTool::BeginTool()
{
	// TODO: 在此添加控件通知处理程序代码
	m_iCurrentPageId = m_pTerrainCtrl->InsertPage("绘制地形表面", IDD_TERRAIN_PAINT_DIALOG, RUNTIME_CLASS(CTerrainPaintDialog), 2);
	m_pTerrainCtrl->ExpandPage(m_iCurrentPageId);

	mEditDecal.Create( "DecalEdit", "DecalEdit", 16 );
	//mEditDecal.SetScale( mBrushSize*TERRAIN_WORLD_SIZE * 0.5f );

	// Update terrain at max 20fps
	mHeightUpdateRate = 50;

	mTimeLastFrame = Ogre::Root::getSingletonPtr()->getTimer()->getMilliseconds();
}

void CTerrainPaintTool::EndTool()
{
	// TODO: 在此添加控件通知处理程序代码
	if ( m_iCurrentPageId )
	{
		m_pTerrainCtrl->RemovePage(m_iCurrentPageId);
	}
	mEditDecal.Destroy();
}

void CTerrainPaintTool::Update()
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

void CTerrainPaintTool::OnLButtonDown(UINT nFlags, CPoint point)
{
	mbBeginEdit = true;
}

void CTerrainPaintTool::OnLButtonUp(UINT nFlags, CPoint point)
{
	mbBeginEdit = false;
}

void CTerrainPaintTool::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
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

void CTerrainPaintTool::OnMouseMove(UINT nFlags, CPoint point)
{
	Ogre::TerrainGroup::RayResult rayResult = GetEditor()->TerrainHitTest( point );
	if (rayResult.hit)
	{
		mEditDecal.setPosition(rayResult.position.x, rayResult.position.z, 2.0f);

		if ( mBrushType == E_TERRAIN_PAINT_NULL_TYPE )
			return;

		if ( mLayerEdit == 0 )
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

				TerrainLayerBlendMap* layer = terrain->getLayerBlendMap(mLayerEdit);
				// we need image coords
				Real imgSize = terrain->getLayerBlendMapSize();
				long startx = (tsPos.x - mBrushSize) * imgSize;
				long starty = (tsPos.y - mBrushSize) * imgSize;
				long endx = (tsPos.x + mBrushSize) * imgSize;
				long endy= (tsPos.y + mBrushSize) * imgSize;
				startx = std::max(startx, 0L);
				starty = std::max(starty, 0L);
				endx = std::min(endx, (long)imgSize);
				endy = std::min(endy, (long)imgSize);
				for (long y = starty; y <= endy; ++y)
				{
					for (long x = startx; x <= endx; ++x)
					{
						Real tsXdist = (x / imgSize) - tsPos.x;
						Real tsYdist = (y / imgSize)  - tsPos.y;

						Real weight = std::min((Real)1.0, 
							Math::Sqrt(tsYdist * tsYdist + tsXdist * tsXdist) / Real(0.5 * mBrushSize));
						weight = 1.0 - (weight * weight);

						float paint = weight * mBrushHardness;
						size_t imgY = imgSize - y;
						float val;

						switch( mBrushType )
						{
						case E_TERRAIN_ADD:
							{
								val = layer->getBlendValue(x, imgY) + paint;
							}
							break;
						case E_TERRAIN_MINUS:
							{
								val = layer->getBlendValue(x, imgY) - paint;
							}
							break;
						}

						val = Math::Clamp(val, 0.0f, 1.0f);
						layer->setBlendValue(x, imgY, val);

					}
				}

				layer->update();
			}
			if (mHeightUpdateCountDown == 0)
				mHeightUpdateCountDown = mHeightUpdateRate;
		}
	}
}

void CTerrainPaintTool::SetBrushHardness( Real hardness )
{
	mBrushHardness = hardness;
}

void CTerrainPaintTool::SetBrushSize( Real brushSize )
{
	mBrushSize = brushSize;
	mEditDecal.SetScale( mBrushSize*TERRAIN_WORLD_SIZE * 0.5f );
}

void CTerrainPaintTool::SetBrushType( E_TERRAIN_PAINT_TYPE type )
{
	mBrushType = type;
}

void CTerrainPaintTool::SetLayerEdit( uint8 layerId )
{
	mLayerEdit = layerId;
}