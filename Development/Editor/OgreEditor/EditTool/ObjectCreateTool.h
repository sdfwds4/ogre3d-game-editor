

#pragma once

#include "../controls/RollupCtrl.h"
#include "../BrushEntity.h"

enum E_OBJECT_TYPE
{
	E_NULL_TYPE,
	E_NPC,
	E_MONSTER,
	E_BRUSH,
	E_BRUSH_TYPE_COUNT
};

class CObjectCreateTool : public CEditTool
{
public:
	DECLARE_DYNCREATE(CObjectCreateTool)

	CObjectCreateTool();
	~CObjectCreateTool();

	void BeginTool();
	void EndTool();
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void OnMouseMove(UINT nFlags, CPoint point);

	void SetBrush( CBrushLine* pBrush );
	void SetObjectType( E_OBJECT_TYPE type );
protected:

	CRollupCtrl* m_pObjectCtrl;
	int m_iCurrentPageId;
	E_OBJECT_TYPE mObjectType;
	CBrushLine* mpBrush;
};