

#include "stdafx.h"
#include "RollupCtrl.h"
#include "Rollupbar.h"

void CRollupBar::SetRollupCtrl( int i, CRollupCtrl* pCtrl, const char* sToolTip )
{
	pCtrl->ShowWindow( SW_HIDE );
	if ( i >= m_Controls.size() )
	{
		m_Controls.resize(i + 1);
		pCtrl->SetParent( &m_tabCtrl );
		m_Controls[i] = pCtrl;
		m_tabCtrl.InsertItem( i, "",1 );
		m_tabCtrl.SetCurSel(0);
	}
}


