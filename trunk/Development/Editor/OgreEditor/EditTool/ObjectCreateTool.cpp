

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

	mName = "���󴴽�����";

}

CObjectCreateTool::~CObjectCreateTool()
{

}

void CObjectCreateTool::BeginTool()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	switch( mObjectType )
	{
	case E_NPC:
		{
			m_iCurrentPageId = m_pObjectCtrl->InsertPage("NPC�б�", IDD_NPC_DIALOG, RUNTIME_CLASS(CNpcDialog), 2);
			m_pObjectCtrl->ExpandPage(m_iCurrentPageId);
		}
		break;
	case E_MONSTER:
		{
			m_iCurrentPageId = m_pObjectCtrl->InsertPage("�����б�", IDD_MONSTER_DIALOG, RUNTIME_CLASS(CMonsterDialog), 2);
			m_pObjectCtrl->ExpandPage(m_iCurrentPageId);
		}
		break;
	case E_BRUSH:
		{
			m_iCurrentPageId = m_pObjectCtrl->InsertPage("��ˢ�б�", IDD_BRUSH_DIALOG, RUNTIME_CLASS(CBrushDialog), 2);
			m_pObjectCtrl->ExpandPage(m_iCurrentPageId);
		}
		break;
	}

}

void CObjectCreateTool::EndTool()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	//���������Ӧ
	switch(nChar)
	{
		//esc��
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
