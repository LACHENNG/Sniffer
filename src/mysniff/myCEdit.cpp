#include "stdafx.h"
#include "myCEdit.h"

IMPLEMENT_DYNAMIC(CMyEdit, CEdit)

CMyEdit::CMyEdit():m_strTip("������˹���(Ĭ�ϲ�����)"){
//	SetWindowText(_T(" "));
	
}

CMyEdit::~CMyEdit() {

}

void CMyEdit::ShowPrompt() {
	SetWindowText(m_strTip);
}

void CMyEdit::setPrompt()
{
	this->OnEnKillfocus();
}

void CMyEdit::OnEnKillfocus()
{
	CString str;
	GetWindowText(str);
	if (str == _T("")) {
		SetWindowText(m_strTip);
	}
	else {
		printf("��ȡ���û����룺%s\n", CT2A(str));
	}
}

void CMyEdit::OnEnSetfocus()
{
	CString str;
	GetWindowText(str);
	if (str == m_strTip) {
		SetWindowText(_T(""));
	}
}

HBRUSH CMyEdit::CtlColor(CDC * pDC, UINT nCtlColor)
{
	return HBRUSH();
}

BEGIN_MESSAGE_MAP(CMyEdit, CEdit)
		ON_CONTROL_REFLECT(EN_KILLFOCUS, &CMyEdit::OnEnKillfocus)
		ON_CONTROL_REFLECT(EN_SETFOCUS, &CMyEdit::OnEnSetfocus)
		ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()


// CMyEdit ��Ϣ����
