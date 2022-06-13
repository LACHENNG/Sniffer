#pragma once
/*
* Copyright (c) Lang. NWPU
* Description: �Զ���ı༭��CEdit, �õ�������ʾ�ı���ʧ ʧȥ���㣬��ʾ�ı���ʾ 
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
