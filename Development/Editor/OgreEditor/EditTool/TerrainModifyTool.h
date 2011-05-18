

#pragma once

#include "../controls/RollupCtrl.h"
#include "../DecalEntity.h"

enum E_TERRAIN_BRUSH_TYPE
{
	E_TERRAIN_BRUSH_NULL_TYPE,
	E_TERRAIN_RISE,
	E_TERRAIN_LOWER,
	E_TERRAIN_BRUSH_TYPE_COUNT
};

class CTerrainModifyTool : public CEditTool
{
public:
	DECLARE_DYNCREATE(CTerrainModifyTool)

	CTerrainModifyTool();
	~CTerrainModifyTool();

	void BeginTool();
	void EndTool();
	void Update();
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void OnMouseMove(UINT nFlags, CPoint point);

	void SetBrushSize( Real brushSize );
	void SetBrushHardness( Real hardness );
	void SetBrushType( E_TERRAIN_BRUSH_TYPE type );
protected:

	CRollupCtrl* m_pTerrainCtrl;
	int m_iCurrentPageId;
	E_TERRAIN_BRUSH_TYPE mBrushType;
	CDecalEntity mEditDecal;
	bool mbBeginEdit;
	Real mBrushSize;
	Real mBrushHardness;
	int mHeightUpdateCountDown;
	unsigned long mHeightUpdateRate;
	unsigned long mTimeLastFrame;
};