

#include "stdafx.h"
#include "../EditTool.h"
#include "../Editor.h"
#include "NpcTestTool.h"

IMPLEMENT_DYNCREATE(CNpcTestTool, CEditTool)

CNpcTestTool::CNpcTestTool() : mpNpc(NULL)
{
	mName = "NPC���Թ���";

}

CNpcTestTool::~CNpcTestTool()
{

}

void CNpcTestTool::BeginTool()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	mState = E_NPC_BORN;
	mpNpc = new CNpc;
	mpNpc->Create();

	CPhysicsManager::getSingletonPtr()->SetHeightfieldData();
}

void CNpcTestTool::EndTool()
{
	mpNpc->Destroy();
	delete mpNpc;
}

void CNpcTestTool::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//���������Ӧ
	switch(nChar)
	{
		//esc��
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

void CNpcTestTool::OnLButtonDown(UINT nFlags, CPoint point)
{
	switch (mState)
	{
	case E_NPC_BORN:
		{
			Ogre::TerrainGroup::RayResult rayResult = GetEditor()->TerrainHitTest( point );
			if (rayResult.hit)
			{
				mpNpc->setPosition(rayResult.position, true);
				mState = E_NPC_IDLE;
			}
		}
		break;
	case E_NPC_IDLE:
		{
			Ogre::TerrainGroup::RayResult rayResult = GetEditor()->TerrainHitTest( point );
			if (rayResult.hit)
			{
				mpNpc->setGoalDirection(rayResult.position);
				mState = E_NPC_IDLE;
			}
		}
		break;
	}
}

void CNpcTestTool::OnMouseMove(UINT nFlags, CPoint point)
{
	switch (mState)
	{
	case E_NPC_BORN:
		{
			Ogre::TerrainGroup::RayResult rayResult = GetEditor()->TerrainHitTest( point );
			if (rayResult.hit)
			{
				mpNpc->setPosition(rayResult.position);
			}
		}
		break;
	case E_NPC_IDLE:
		{
			//Ogre::TerrainGroup::RayResult rayResult = GetEditor()->TerrainHitTest( point );
			//if (rayResult.hit)
			//{
			//	mpNpc->setGoalDirection(rayResult.position);
			//}
		}
		break;
	}
}
