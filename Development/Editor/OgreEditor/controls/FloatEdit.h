

#pragma once

class CFloatEdit : public CEdit
{
public:
	CFloatEdit();
	virtual ~CFloatEdit();
	BOOL Check(CString str,TCHAR c);

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};
