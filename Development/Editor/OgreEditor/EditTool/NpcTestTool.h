

#pragma once
#include "../NpcEntity.h"
#include "../AxisGizmo.h"

class CNpcTestTool : public CEditTool
{
public:
	DECLARE_DYNCREATE(CNpcTestTool)

	enum E_NPC_STATE
	{
		E_NPC_BORN,
		E_NPC_IDLE,
		E_NPC_FIGHT
	};

	CNpcTestTool();
	~CNpcTestTool();

	void BeginTool();
	void EndTool();
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnMouseMove(UINT nFlags, CPoint point);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

protected:
	CNpc* mpNpc;
	E_NPC_STATE mState;
	AxisGizmo mAxisGizmo;
};