

#pragma once

#include "../controls/RollupCtrl.h"
#include "../DecalEntity.h"

enum E_TERRAIN_PAINT_TYPE
{
	E_TERRAIN_PAINT_NULL_TYPE,
	E_TERRAIN_ADD,
	E_TERRAIN_MINUS,
	E_TERRAIN_PAINT_TYPE_COUNT
};

class CTerrainPaintTool : public CEditTool
{
public:
	DECLARE_DYNCREATE(CTerrainPaintTool)

	CTerrainPaintTool();
	~CTerrainPaintTool();

	void BeginTool();
	void EndTool();
	void Update();
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void OnMouseMove(UINT nFlags, CPoint point);

	void SetBrushSize( Real brushSize );
	void SetBrushHardness( Real hardness );
	void SetBrushType( E_TERRAIN_PAINT_TYPE type );
	void SetLayerEdit( uint8 layerId );
protected:

	CRollupCtrl* m_pTerrainCtrl;
	int m_iCurrentPageId;
	E_TERRAIN_PAINT_TYPE mBrushType;
	CDecalEntity mEditDecal;
	bool mbBeginEdit;
	Real mBrushSize;
	Real mBrushHardness;
	uint8 mLayerEdit;
	int mHeightUpdateCountDown;
	unsigned long mHeightUpdateRate;
	unsigned long mTimeLastFrame;
};