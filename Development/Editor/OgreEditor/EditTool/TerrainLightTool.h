

#pragma once

#include "../controls/RollupCtrl.h"

class CTerrainLightTool : public CEditTool
{
public:
	DECLARE_DYNCREATE(CTerrainLightTool)

	CTerrainLightTool();
	~CTerrainLightTool();

	void BeginTool();
	void EndTool();

	void OnMouseMove(UINT nFlags, CPoint point);
protected:
	CRollupCtrl* m_pTerrainCtrl;
	int m_iCurrentPageId;

	SceneManager* mpSceneMgr;
	SceneNode* mpSceneNode;
	ManualObject* mpManualObject;
};