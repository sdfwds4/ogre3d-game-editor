

#include "stdafx.h"
#include "../EditTool.h"
#include "../Editor.h"
#include "../Resource.h"
#include "../EffectDialog.h"
#include "EffectTestTool.h"

IMPLEMENT_DYNCREATE(CEffectTestTool, CEditTool)

CEffectTestTool::CEffectTestTool():m_pObjectCtrl(NULL), m_pSystem(NULL), mpEffectTestNode(NULL)
{
	m_iCurrentPageId = 0;
	m_pObjectCtrl = GetEditor()->GetRollupCtrl(E_OBJECT_CTRL);

	mName = "��Ч���Թ���";
}

CEffectTestTool::~CEffectTestTool()
{

}

void CEffectTestTool::BeginTool()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_iCurrentPageId = m_pObjectCtrl->InsertPage("��Ч�б�", IDD_EFFECT_DIALOG, RUNTIME_CLASS(CEffectDialog), 2);
	m_pObjectCtrl->ExpandPage(m_iCurrentPageId);

	mAxisGizmo.CreateGizmo();
	mAxisGizmo.SetGizmoMode(TOOL_MOVE);
	mAxisGizmo.setVisible(false);
}

void CEffectTestTool::EndTool()
{
	if ( m_iCurrentPageId )
	{
		m_pObjectCtrl->RemovePage(m_iCurrentPageId);
	}

	mAxisGizmo.DestroyGizmo();

	if ( mpEffectTestNode )
	{
		if ( m_pSystem )
		{
			m_pSystem->stop();
			mpEffectTestNode->detachObject(m_pSystem);
			ParticleUniverse::ParticleSystemManager::getSingletonPtr()->destroyParticleSystem( m_pSystem, GetEditor()->GetSceneManager() );
		}
		GetEditor()->GetSceneManager()->destroySceneNode( mpEffectTestNode );
	}
}

void CEffectTestTool::Update()
{
	mAxisGizmo.SetGizmoScale(1.0f);
}

void CEffectTestTool::OnLButtonDown(UINT nFlags, CPoint point)
{
	if ( !mpEffectTestNode )
	{
		mpEffectTestNode = GetEditor()->GetSceneManager()->getRootSceneNode()->createChildSceneNode();
	}

	if ( !mCurEffectName.IsEmpty() )
	{
		Ogre::TerrainGroup::RayResult rayResult = GetEditor()->TerrainHitTest( point );
		if (rayResult.hit)
		{
			if ( m_pSystem )
			{
				m_pSystem->stop();
				mpEffectTestNode->detachObject(m_pSystem);
				ParticleUniverse::ParticleSystemManager::getSingletonPtr()->destroyParticleSystem( m_pSystem, GetEditor()->GetSceneManager() );
			}

			m_pSystem = ParticleUniverse::ParticleSystemManager::getSingletonPtr()->createParticleSystem(
				"sys", mCurEffectName.GetBuffer(), GetEditor()->GetSceneManager() );
			if ( m_pSystem )
			{
				mpEffectTestNode->setVisible(true);
				mpEffectTestNode->setPosition(rayResult.position);

				mpEffectTestNode->attachObject(m_pSystem);
				m_pSystem->start();
			}

			mAxisGizmo.setPosition( rayResult.position );
			mAxisGizmo.setVisible( true );
		}
	}
}

void CEffectTestTool::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//���������Ӧ
	switch(nChar)
	{
	case VK_LEFT:
		break;
	case VK_RETURN:
		break;
	}
}

void CEffectTestTool::OnMouseMove(UINT nFlags, CPoint point)
{
	if ( m_pSystem )
	{
		Ogre::Ray ray = GetEditor()->getCameraToViewportRay(point);
		int editorAxis;
		if ( mAxisGizmo.PickGizmos(ray, editorAxis) )
		{
			mAxisGizmo.HighlightGizmo(editorAxis);
		}
	}
}