

#include "stdafx.h"
#include "../EditTool.h"
#include "../Editor.h"
#include "../Resource.h"
#include "../TerrainLightDialog.h"
#include "TerrainLightTool.h"

IMPLEMENT_DYNCREATE(CTerrainLightTool, CEditTool)

CTerrainLightTool::CTerrainLightTool() 
{
	m_iCurrentPageId = 0;
	m_pTerrainCtrl = GetEditor()->GetRollupCtrl(E_TERRAIN_CTRL);

	mpSceneMgr = GetEditor()->GetSceneManager();
	mpManualObject = NULL;
	mpSceneNode = NULL;

	mName = "地形光影工具";

}

CTerrainLightTool::~CTerrainLightTool()
{

}

void CTerrainLightTool::BeginTool()
{
	// TODO: 在此添加控件通知处理程序代码
	m_iCurrentPageId = m_pTerrainCtrl->InsertPage("地形光影设置", IDD_TERRAIN_LIGHT_DIALOG, RUNTIME_CLASS(CTerrainLightDialog), 2);
	m_pTerrainCtrl->ExpandPage(m_iCurrentPageId);

	mpManualObject = new Ogre::ManualObject("testManual");
	mpSceneNode = mpSceneMgr->getRootSceneNode()->createChildSceneNode();
	mpSceneNode->attachObject(mpManualObject);

	mpManualObject->begin("Examples/OgreLogo2", Ogre::RenderOperation::OT_TRIANGLE_LIST);
	for (int i=0; i<=8; i++)
	{
		for (int j=0; j<=8; j++)
		{
			mpManualObject->position(Ogre::Vector3(i * 50.0f, 6, j * 50.0f));
			if ( i == 0 || j == 0 || i == 8 || j == 8 )
			{
				mpManualObject->colour( 1.0f, 1.0f, 1.0f, 0.0f );
			}
			else
			{
				mpManualObject->colour( 1.0f, 1.0f, 1.0f );
			}

			mpManualObject->textureCoord((float)i / (float)8, (float)j / (float)8);
			mpManualObject->setVisible(true);
		}
	}

	// redefine quads
	for (int i=0; i<8; i++)
	{
		for (int j=0; j<8; j++)
		{
			mpManualObject->quad( i * (8+1) + j,
				i * (8+1) + j + 1,
				(i + 1) * (8+1) + j + 1,
				(i + 1) * (8+1) + j);
		}
	}

	mpManualObject->end();
}

void CTerrainLightTool::EndTool()
{
	// TODO: 在此添加控件通知处理程序代码
	if ( m_iCurrentPageId )
	{
		m_pTerrainCtrl->RemovePage(m_iCurrentPageId);
	}

	if ( mpManualObject )
	{
		mpSceneNode->detachObject(mpManualObject);
		mpSceneMgr->destroySceneNode(mpSceneNode);
		mpSceneNode = NULL;
		delete mpManualObject;
		mpManualObject = NULL;
	}
}


void CTerrainLightTool::OnMouseMove(UINT nFlags, CPoint point)
{
	Ogre::TerrainGroup::RayResult rayResult = GetEditor()->TerrainHitTest( point );
	if (rayResult.hit)
	{
		if ( !mpSceneNode )
		{
			return;
		}

		mpSceneNode->setPosition(rayResult.position);

		//if ( mBrushType == E_TERRAIN_BRUSH_NULL_TYPE )
		//	return;

		//if ( mbBeginEdit )
		//{
		//	// figure out which terrains this affects
		//	TerrainGroup::TerrainList terrainList;
		//	Real brushSizeWorldSpace = TERRAIN_WORLD_SIZE * mBrushSize;
		//	Sphere sphere(rayResult.position, brushSizeWorldSpace);
		//	GetEditor()->GetTerrainGroup()->sphereIntersects(sphere, &terrainList);

		//	for (TerrainGroup::TerrainList::iterator it = terrainList.begin();
		//		it != terrainList.end(); ++it)
		//	{
		//		Terrain* terrain = *it;
		//		Vector3 tsPos;
		//		terrain->getTerrainPosition(rayResult.position, &tsPos);

		//		// we need point coords
		//		Real terrainSize = (terrain->getSize() - 1);
		//		long startx = (tsPos.x - mBrushSize) * terrainSize;
		//		long starty = (tsPos.y - mBrushSize) * terrainSize;
		//		long endx = (tsPos.x + mBrushSize) * terrainSize;
		//		long endy= (tsPos.y + mBrushSize) * terrainSize;
		//		startx = std::max(startx, 0L);
		//		starty = std::max(starty, 0L);
		//		endx = std::min(endx, (long)terrainSize);
		//		endy = std::min(endy, (long)terrainSize);
		//		for (long y = starty; y <= endy; ++y)
		//		{
		//			for (long x = startx; x <= endx; ++x)
		//			{
		//				Real tsXdist = (x / terrainSize) - tsPos.x;
		//				Real tsYdist = (y / terrainSize)  - tsPos.y;

		//				Real weight = std::min((Real)1.0, 
		//					Math::Sqrt(tsYdist * tsYdist + tsXdist * tsXdist) / Real(0.5 * mBrushSize));
		//				weight = 1.0 - (weight * weight);

		//				float addedHeight = weight * 250.0 * mBrushHardness;
		//				float newheight;

		//				switch( mBrushType )
		//				{
		//				case E_TERRAIN_RISE:
		//					{
		//						newheight = terrain->getHeightAtPoint(x, y) + addedHeight;
		//					}
		//					break;
		//				case E_TERRAIN_LOWER:
		//					{
		//						newheight = terrain->getHeightAtPoint(x, y) - addedHeight;
		//					}
		//					break;
		//				}

		//				terrain->setHeightAtPoint(x, y, newheight);
		//			}
		//		}
		//	}
		//	if (mHeightUpdateCountDown == 0)
		//		mHeightUpdateCountDown = mHeightUpdateRate;
		//}
	}
}
