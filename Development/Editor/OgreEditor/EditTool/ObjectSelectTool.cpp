

#include "stdafx.h"
#include "../Editor.h"
#include "../EntityDialog.h"
#include "ObjectSelectTool.h"

IMPLEMENT_DYNCREATE(CObjectSelectTool, CEditTool)

CObjectSelectTool::CObjectSelectTool() 
	: mSelectionBuffer(NULL), mLastSelectedEntity(NULL), 
	mLockedAxis(0), mbLButtonDown(false), mLastTranslationDelta(Vector3::ZERO), 
	mLButtonDownPos(Vector3::ZERO), mSelcetEntityPos(Vector3::ZERO), mLastScale(1.0f,1.0f,1.0f)
{
	m_iCurrentPageId = 0;
	m_pObjectCtrl = GetEditor()->GetRollupCtrl(E_OBJECT_CTRL);

	mName = "对象选择工具";

}

CObjectSelectTool::~CObjectSelectTool()
{

}

void CObjectSelectTool::BeginTool()
{
	// TODO: 在此添加控件通知处理程序代码
	mSelectionBuffer = new SelectionBuffer;

	mAxisGizmo.CreateGizmo();
	mAxisGizmo.SetGizmoMode(TOOL_MOVE);
	mAxisGizmo.setVisible(false);
	mAxisGizmo.SetGizmoMode( GetEditor()->GetToolMode() );

	GetEditor()->RegisterEditorListener( this );
}

void CObjectSelectTool::EndTool()
{
	// TODO: 在此添加控件通知处理程序代码
	if ( mSelectionBuffer )
	{
		delete mSelectionBuffer;
		mSelectionBuffer = NULL;
	}

	GetEditor()->unRegisterEditorListener( this );

	if(mLastSelectedEntity)
		mLastSelectedEntity->getParentSceneNode()->showBoundingBox(false);

	if ( m_iCurrentPageId )
	{
		m_pObjectCtrl->RemovePage(m_iCurrentPageId);
	}

	mAxisGizmo.DestroyGizmo();
}

void CObjectSelectTool::Update()
{
	mAxisGizmo.SetGizmoScale(1.0f);
}

void CObjectSelectTool::OnLButtonDown(UINT nFlags, CPoint point)
{
	mbLButtonDown = true;
	Entity *selectedEntity;
	mLastMouse = point;
	Ogre::Ray ray = GetEditor()->getCameraToViewportRay(point);

	if ( mLastSelectedEntity )
	{
		mAxisGizmo.PickGizmos(ray, mLockedAxis);
		mAxisPlane = GetGizmoTranslationPlane(ray);

		if ( mLockedAxis )
		{
			mLButtonDownPos = GetGizmoIntersect(ray, mAxisPlane);

			return;
		}
	}

	if(mSelectionBuffer)
	{
		// mouseAbsX and mouseAbsY are the absolute coordinates of the click from, for instance, an OIS::MouseState
		selectedEntity = mSelectionBuffer->OnSelectionClick(point.x, point.y);
		if ( mLastSelectedEntity == selectedEntity )
		{
			if ( mLastSelectedEntity )
			{
				switch( GetEditor()->GetToolMode() )
				{
				case TOOL_MOVE:
					{
						Ogre::TerrainGroup::RayResult rayResult = GetEditor()->TerrainHitTest( point );
						if (rayResult.hit)
						{
							mLButtonDownPos = rayResult.position;
						}
					}
					break;
				case TOOL_SCALE:
					{
						mAxisPlane = GetGizmoTranslationPlane(ray);
						mLButtonDownPos = GetGizmoIntersect(ray, mAxisPlane);
					}
					break;
				}
			}

			return;
		}

		if ( mLastSelectedEntity )
			mLastSelectedEntity->getParentSceneNode()->showBoundingBox(false);

		if( selectedEntity )
		{
			selectedEntity->getParentSceneNode()->showBoundingBox(true);
			mSelcetEntityPos = selectedEntity->getParentSceneNode()->getPosition();
			mLastQuat = selectedEntity->getParentSceneNode()->getOrientation();
			mLastScale = selectedEntity->getParentSceneNode()->getScale();

			mAxisGizmo.setPosition( mSelcetEntityPos );
			mAxisGizmo.setVisible( true );

			if ( m_iCurrentPageId )
			{
				m_pObjectCtrl->RemovePage(m_iCurrentPageId);
			}

			m_iCurrentPageId = m_pObjectCtrl->InsertPage("Entity信息", IDD_ENTITY_DIALOG, RUNTIME_CLASS(CEntityDialog), 2);
			m_pObjectCtrl->ExpandPage(m_iCurrentPageId);
		}
		else
		{
			mAxisGizmo.setVisible( false );
			mSelcetEntityPos = Vector3::ZERO;

			if ( m_iCurrentPageId )
			{
				m_pObjectCtrl->RemovePage(m_iCurrentPageId);
			}
		}

		mLastSelectedEntity = selectedEntity;

		if ( mLastSelectedEntity )
		{
			switch( GetEditor()->GetToolMode() )
			{
			case TOOL_MOVE:
				{
					Ogre::TerrainGroup::RayResult rayResult = GetEditor()->TerrainHitTest( point );
					if (rayResult.hit)
					{
						mLButtonDownPos = rayResult.position;
					}
				}
				break;
			case TOOL_SCALE:
				{
					mAxisPlane = GetGizmoTranslationPlane(ray);
					mLButtonDownPos = GetGizmoIntersect(ray, mAxisPlane);
				}
				break;
			}
		}
	}
}

void CObjectSelectTool::OnLButtonUp(UINT nFlags, CPoint point)
{
	mbLButtonDown = false;
	mLButtonDownPos = Vector3::ZERO;
	if(mLastSelectedEntity)
	{
		mSelcetEntityPos = mLastSelectedEntity->getParentSceneNode()->getPosition();
		mLastQuat = mLastSelectedEntity->getParentSceneNode()->getOrientation();
		mLastScale = mLastSelectedEntity->getParentSceneNode()->getScale();
	}
}

void CObjectSelectTool::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//处理键盘响应
	switch(nChar)
	{
		//esc键
	case VK_ESCAPE:
		{
			if ( mLastSelectedEntity )
			{
				mLastSelectedEntity->getParentSceneNode()->showBoundingBox(false);
				mLastSelectedEntity = NULL;
				mAxisGizmo.setVisible( false );

				if ( m_iCurrentPageId )
				{
					m_pObjectCtrl->RemovePage(m_iCurrentPageId);
				}
			}
		}
		break;
	case VK_LEFT:
		break;
	case VK_RETURN:
		break;
	}
}

void CObjectSelectTool::OnMouseMove(UINT nFlags, CPoint point)
{
	Ogre::Ray ray = GetEditor()->getCameraToViewportRay(point);
	if ( mLastSelectedEntity )
	{
		int editorAxis;
		if ( mAxisGizmo.PickGizmos(ray, editorAxis) )
		{
			mAxisGizmo.HighlightGizmo(editorAxis);
		}
		else
		{
			mAxisGizmo.HighlightGizmo(mLockedAxis);
		}
	}

	if ( mLastSelectedEntity && mbLButtonDown  )
	{
		switch( GetEditor()->GetToolMode() )
		{
		case TOOL_MOVE:
			{
				if ( !mLockedAxis )
				{
					Ogre::TerrainGroup::RayResult rayResult = GetEditor()->TerrainHitTest( point );
					if (rayResult.hit)
					{
						Vector3 pos = Vector3::ZERO;
						pos.x = mSelcetEntityPos.x + rayResult.position.x - mLButtonDownPos.x;
						pos.y = mSelcetEntityPos.y + rayResult.position.y - mLButtonDownPos.y;
						pos.z = mSelcetEntityPos.z + rayResult.position.z - mLButtonDownPos.z;
						mLastSelectedEntity->getParentSceneNode()->setPosition(pos);
						mAxisGizmo.setPosition( pos );
						return;
					}
				}
				else
				{
					Vector3 pos = GetGizmoIntersect(ray, mAxisPlane);
					Vector3 vNewPos = mLastSelectedEntity->getParentSceneNode()->getPosition();

					if( mLockedAxis & AXIS_X ) 
						vNewPos.x = mSelcetEntityPos.x + pos.x - mLButtonDownPos.x;
					if( mLockedAxis & AXIS_Y ) 
						vNewPos.y = mSelcetEntityPos.y + pos.y - mLButtonDownPos.y;
					if( mLockedAxis & AXIS_Z ) 
						vNewPos.z = mSelcetEntityPos.z + pos.z - mLButtonDownPos.z;

					mLastSelectedEntity->getParentSceneNode()->setPosition(vNewPos);
					mAxisGizmo.setPosition( vNewPos );
				}
			}
			break;
		case TOOL_ROTATE:
			{
				float DeltaX = (point.x - mLastMouse.x) * 0.5f;
				float DeltaY = (point.y - mLastMouse.y) * 0.5f;

				if(mLockedAxis == 0)
					mLockedAxis = AXIS_Y;
				mAxisGizmo.HighlightGizmo(mLockedAxis);

				Ogre::Quaternion quat = mLastQuat;
				switch(mLockedAxis)
				{
				case AXIS_X:
					quat = Ogre::Quaternion(Ogre::Degree(-DeltaY),Ogre::Vector3(1,0,0)) * quat;
					break;
				case AXIS_Y:
					quat = Ogre::Quaternion(Ogre::Degree(-DeltaY),Ogre::Vector3(0,1,0)) * quat;
					break;
				case AXIS_Z:
					quat = Ogre::Quaternion(Ogre::Degree(-DeltaY),Ogre::Vector3(0,0,1)) * quat;
					break;
				}

				mLastSelectedEntity->getParentSceneNode()->setOrientation(quat);
			}
			break;
		case TOOL_SCALE:
			{
				Vector3 pos = GetGizmoIntersect(ray, mAxisPlane);
				Ogre::Vector3 vScale = mLastScale;
				float fNewDist = pos.length();
				float fLength = mLButtonDownPos.length();

				if ( !mLockedAxis )
				{
					vScale.x *= (fNewDist / fLength);
					vScale.y *= (fNewDist / fLength);
					vScale.z *= (fNewDist / fLength);
				}
				else
				{
					if( mLockedAxis & AXIS_X ) 
						vScale.x *= (fNewDist / fLength);
					if( mLockedAxis & AXIS_Y ) 
						vScale.y *= (fNewDist / fLength);
					if( mLockedAxis & AXIS_Z ) 
						vScale.z *= (fNewDist / fLength);
				}

				mLastSelectedEntity->getParentSceneNode()->setScale(vScale);
				Vector3 entityPos = mLastSelectedEntity->getParentSceneNode()->getPosition();
				mAxisGizmo.setPosition( entityPos );
			}
			break;
		}
	}
	else
	{
		Ogre::TerrainGroup::RayResult rayResult = GetEditor()->TerrainHitTest( point );
		if (rayResult.hit)
		{
			GetEditor()->SetEditPos( rayResult.position );
		}
		else
		{
			Vector3 pos = Vector3::ZERO;
			GetEditor()->SetEditPos( pos );
		}
	}
}

void CObjectSelectTool::Notify( E_NOTIFY_EVENTS event, UINT nflag )
{
	if ( event == NE_TOOLMODE_CHANGED )
	{
		mAxisGizmo.SetGizmoMode( GetEditor()->GetToolMode() );
	}
}

//-----------------------------------------------------------------------------------------
Plane CObjectSelectTool::GetGizmoTranslationPlane(Ray &pickRay)
{
	Quaternion qOrient = GetEditor()->GetSceneManager()->getRootSceneNode()->getOrientation();

	Vector3 vPos = mLastSelectedEntity->getParentSceneNode()->getPosition();

	Vector3 vCamBack = GetEditor()->GetCamera()->getDerivedDirection();
	vCamBack = -vCamBack;
	if(!mLockedAxis)
		return Plane(vCamBack,vPos);

	Plane planeX(qOrient.xAxis(), vPos);
	Plane planeY(qOrient.yAxis(), vPos);
	Plane planeZ(qOrient.zAxis(), vPos);

	float vX = planeX.projectVector(pickRay.getDirection()).length();
	float vY = planeY.projectVector(pickRay.getDirection()).length();
	float vZ = planeZ.projectVector(pickRay.getDirection()).length();

	if(mLockedAxis & AXIS_X) 
		vX = 10000.0f;
	if(mLockedAxis & AXIS_Y) 
		vY = 10000.0f;
	if(mLockedAxis & AXIS_Z) 
		vZ = 10000.0f;

	if (vX < vY && vX < vZ)
		return planeX;
	else
	{
		if (vY < vX && vY < vZ )
			return planeY;
		else
			return planeZ;
	}
}

//--------------------------------------------------------------------------------
Vector3 CObjectSelectTool::GetGizmoIntersect( Ray &pickRay, Plane &planetouse )
{
	std::pair<bool,Real> result;

	result = pickRay.intersects(planetouse);

	if (result.first)
	{
		Vector3 AxisX = Vector3::ZERO;
		Vector3 AxisY = Vector3::ZERO;
		Vector3 AxisZ = Vector3::ZERO;

		Quaternion qOrient = GetEditor()->GetSceneManager()->getRootSceneNode()->getOrientation();

		if(mLockedAxis)
		{
			if(mLockedAxis & AXIS_X) 
				AxisX = qOrient.xAxis();
			if(mLockedAxis & AXIS_Y) 
				AxisY = qOrient.yAxis();
			if(mLockedAxis & AXIS_Z) 
				AxisZ = qOrient.zAxis();
		}
		else
		{
			AxisX = qOrient.xAxis();
			AxisY = qOrient.yAxis();
			AxisZ = qOrient.zAxis();
		}

		Vector3 Proj = pickRay.getPoint(result.second) - mSelcetEntityPos;
		Vector3 vPos1 = (AxisX.dotProduct(Proj) * AxisX);
		Vector3 vPos2 = (AxisY.dotProduct(Proj) * AxisY);
		Vector3 vPos3 = (AxisZ.dotProduct(Proj) * AxisZ);
		Vector3 vPos = vPos1 + vPos2 + vPos3;

		mLastTranslationDelta = vPos;
		return vPos;
	}
	return mLastTranslationDelta;
}