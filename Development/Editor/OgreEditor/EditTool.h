

#pragma once

#include "smartptr.h"


class CEditTool : public _reference_target_t , public CObject
{
public:
	CEditTool()
	{
		mName = "BaseTool";
	}

	virtual ~CEditTool()
	{

	}

	virtual void BeginTool()
	{

	}

	virtual void EndTool()
	{

	}

	virtual void Update()
	{

	}

	virtual void OnLButtonDown(UINT nFlags, CPoint point)
	{

	}

	virtual void OnLButtonUp(UINT nFlags, CPoint point)
	{

	}

	virtual void OnMouseMove(UINT nFlags, CPoint point)
	{

	}

	virtual void OnRButtonDown(UINT nFlags, CPoint point)
	{

	}

	virtual void OnRButtonUp(UINT nFlags, CPoint point)
	{

	}

	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{

	}

	virtual void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{

	}

	virtual BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
	{
		return TRUE;
	}

	virtual void OnPaint()
	{

	}

	virtual void OnTimer(UINT_PTR nIDEvent) 
	{

	}

	virtual void FromName( const CString& fileName )
	{

	}

	const CString& GetName() const
	{
		return mName;
	}
protected:
	CString mName;
};