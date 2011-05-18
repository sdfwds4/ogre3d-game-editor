

#pragma once

#include "../SelectionBuffer.h"
#include "../AxisGizmo.h"


class CObjectSelectTool : public CEditTool , public IEditorEventListener
{
public:
	DECLARE_DYNCREATE(CObjectSelectTool)

	CObjectSelectTool();
	~CObjectSelectTool();

	void BeginTool();
	void EndTool();
	void Update();

	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void OnMouseMove(UINT nFlags, CPoint point);

	void Notify( E_NOTIFY_EVENTS event, UINT nflag = 0 );
	Vector3 GetGizmoIntersect( Ray &pickRay, Plane &planetouse );

protected:
	CRollupCtrl* m_pObjectCtrl;
	int m_iCurrentPageId;

	SelectionBuffer *mSelectionBuffer;
	Entity *mLastSelectedEntity;
	AxisGizmo mAxisGizmo;
	int mLockedAxis;
	bool mbLButtonDown;
	Vector3 mLButtonDownPos;
	Vector3 mSelcetEntityPos;
	Plane mAxisPlane;
	CPoint mLastMouse;
	Quaternion mLastQuat;
	Vector3 mLastScale;

	Plane GetGizmoTranslationPlane(Ray &pickRay);

	/** Last object translation change (used with gizmos) */
	Vector3       mLastTranslationDelta;

};