

#pragma once

class CMaterialTool : public CEditTool
{
public:
	DECLARE_DYNCREATE(CMaterialTool)

	CMaterialTool();
	~CMaterialTool();

	void BeginTool();
	void EndTool();

	void SetParent( CWnd* pWnd )
	{
		mpParent = pWnd;
	}
	CWnd* mpParent;
};