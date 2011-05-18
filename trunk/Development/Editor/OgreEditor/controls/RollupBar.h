

#pragma once


class CRollupBar : public CWnd
{
public:
	void SetRollupCtrl( int i, CRollupCtrl* pCtrl, const char* sToolTip );


	CTabCtrl m_tabCtrl;
	std::vector<CRollupCtrl*> m_Controls;
	CImageList m_tabImageList;
};