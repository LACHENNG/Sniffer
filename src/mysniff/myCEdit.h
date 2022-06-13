#pragma once
/*
* Copyright (c) Lang. NWPU
* Description: 自定义的编辑框CEdit, 得到焦点提示文本消失 失去焦点，提示文本显示 
*/
#include "stdafx.h"
#include "windows.h"
#include "Winuser.h"

class CMyEdit : public CEdit {
	DECLARE_DYNAMIC(CMyEdit)
public:
	CMyEdit();
	virtual ~CMyEdit();

public:
	void ShowPrompt();
	const CString GetPrompt();
	void setPrompt();
private:
	const CString m_strTip;

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnEnKillfocus();
	afx_msg void OnEnSetfocus();
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);

};
