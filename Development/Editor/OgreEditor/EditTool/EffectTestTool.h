

#pragma once

#include "../controls/RollupCtrl.h"
#include "../AxisGizmo.h"

class CEffectTestTool : public CEditTool
{
public:
	DECLARE_DYNCREATE(CEffectTestTool)

	CEffectTestTool();
	~CEffectTestTool();

	void BeginTool();
	void EndTool();
	void Update();
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void OnMouseMove(UINT nFlags, CPoint point);

	void SetCurEffectName( const CString& name )
	{
		mCurEffectName = name;
	}
protected:
	CRollupCtrl* m_pObjectCtrl;
	int m_iCurrentPageId;

	CString mCurEffectName;
	ParticleUniverse::ParticleSystem* m_pSystem;
	Ogre::SceneNode* mpEffectTestNode;

	AxisGizmo mAxisGizmo;
};