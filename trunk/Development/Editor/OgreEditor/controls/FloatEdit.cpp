

#include "stdafx.h"
#include "FloatEdit.h"

BEGIN_MESSAGE_MAP(CFloatEdit, CEdit)
	ON_WM_CHAR()
END_MESSAGE_MAP()

CFloatEdit::CFloatEdit()
{
}

CFloatEdit::~CFloatEdit()
{
}

void CFloatEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nChar == VK_RETURN)
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
	if ((nChar <= '9' && nChar >= '0') || nChar == '-' || nChar == '.' || nChar == 8)
	{
		CString str;
		GetWindowText(str);
		if (Check(str,nChar))
		{
			CEdit::OnChar(nChar, nRepCnt, nFlags);
		}
	}
}

BOOL CFloatEdit::Check( CString str,TCHAR c )
{

	if (c == '.')
	{
		if (-1 == str.Find(c))
		{
			int n1, n2;
			GetSel(n1,n2);
			if (n1 == 0)
			{
				return FALSE;
			}
			TCHAR d = str.GetAt(n1-1);
			if (!(d <= '9' && d >= '0'))
			{
				return FALSE;
			}
		}
		else
			return FALSE;
	}
	else if (c == '-')
	{
		if (-1 !=str.Find(c))
			return FALSE;
		else
		{
			int n1, n2;
			GetSel(n1,n2);
			if (n1 != 0)
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}
