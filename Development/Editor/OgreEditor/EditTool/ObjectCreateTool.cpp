

#include "stdafx.h"
#include "../EditTool.h"
#include "../Editor.h"
#include "../Resource.h"
#include "../BrushDialog.h"
#include "../NpcDialog.h"
#include "../MonsterDialog.h"
#include "../EntityManager.h"
#include "ObjectCreateTool.h"

IMPLEMENT_DYNCREATE(CObjectCreateTool, CEditTool)

CObjectCreateTool::CObjectCreateTool() : mObjectType(E_NULL_TYPE), mpBrush(NULL)
{
	m_iCurrentPageId = 0;
	m_pObjectCtrl = GetEditor()->GetRollupCtrl(E_OBJECT_CTRL);

	mName = "对象创建工具";

}

CObjectCreateTool::~CObjectCreateTool()
{

}

void CObjectCreateTool::BeginTool()
{
	// TODO: 在此添加控件通知处理程序代码
	switch( mObjectType )
	{
	case E_NPC:
		{
			m_iCurrentPageId = m_pObjectCtrl->InsertPage("NPC列表", IDD_NPC_DIALOG, RUNTIME_CLASS(CNpcDialog), 2);
			m_pObjectCtrl->ExpandPage(m_iCurrentPageId);
		}
		break;
	case E_MONSTER:
		{
			m_iCurrentPageId = m_pObjectCtrl->InsertPage("怪物列表", IDD_MONSTER_DIALOG, RUNTIME_CLASS(CMonsterDialog), 2);
			m_pObjectCtrl->ExpandPage(m_iCurrentPageId);
		}
		break;
	case E_BRUSH:
		{
			m_iCurrentPageId = m_pObjectCtrl->InsertPage("画刷列表", IDD_BRUSH_DIALOG, RUNTIME_CLASS(CBrushDialog), 2);
			m_pObjectCtrl->ExpandPage(m_iCurrentPageId);
		}
		break;
	}

}

void CObjectCreateTool::EndTool()
{
	// TODO: 在此添加控件通知处理程序代码
	if ( m_iCurrentPageId )
	{
		m_pObjectCtrl->RemovePage(m_iCurrentPageId);
	}
}


void CObjectCreateTool::OnLButtonDown(UINT nFlags, CPoint point)
{
	if ( mpBrush )
	{
		CBrushEntity* pEntity = CEntityManager::getSingletonPtr()->CreateBrush( mpBrush->mstrName.GetBuffer(), mpBrush->mstrMesh.GetBuffer(), mpBrush->mstrMtl.GetBuffer() );
		pEntity->setPosition( mpBrush->mpBrushEntity->GetSceneNode()->getPosition() );
		pEntity->SetVisible(true);
		mpBrush->mpBrushEntity->SetVisible(false);

		GetEditor()->SetEditTool(NULL);
	}
}

void CObjectCreateTool::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//处理键盘响应
	switch(nChar)
	{
		//esc键
	case VK_ESCAPE:
		{
			if ( mpBrush )
			{
				mpBrush->mpBrushEntity->SetVisible(false);

				//CEntityManager::getSingletonPtr()->DestroyBrush( mpBrush->GetName() );
				//mpBrush = NULL;
			}
		}
		break;
	case VK_LEFT:
		break;
	case VK_RETURN:
		break;
	}
}

void CObjectCreateTool::OnMouseMove(UINT nFlags, CPoint point)
{
	if ( mpBrush )
	{
		Ogre::TerrainGroup::RayResult rayResult = GetEditor()->TerrainHitTest( point );
		if (rayResult.hit)
		{
			mpBrush->mpBrushEntity->setPosition(rayResult.position);
		}
	}
}

void CObjectCreateTool::SetBrush( CBrushLine* pBrush )
{
	if ( mpBrush )
	{
		mpBrush->mpBrushEntity->SetVisible(false);
	}
	mpBrush = pBrush;
	mpBrush->mpBrushEntity->SetVisible(true);
}

void CObjectCreateTool::SetObjectType( E_OBJECT_TYPE type )
{
	mObjectType = type;
}
