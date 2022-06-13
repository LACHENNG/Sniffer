
// mysniffDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "snifferCore.h"
#include "functional"
#include "string"
#include "myCEdit.h"

using namespace std;

// ǰ�����������snifferCore.h �� mysniffDlg.h ���ֵ��໥������ѭ����������
class CmysniffDlg;

// CmysniffDlg �Ի���
class CmysniffDlg : public CDialogEx
{


public:
	CmysniffDlg(CWnd* pParent = NULL);	

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYSNIFF_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	
	DECLARE_MESSAGE_MAP();

// �ؼ��¼�������
public:
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();

	// �ؼ��������
	CEdit m_editTCP;
	CEdit m_editUDP;
	CEdit m_editICMP;
	CEdit m_editHTTP;
	CEdit m_editARP;
	CEdit m_editICMPv6;
	CEdit m_editIPv4;
	CEdit m_editIPv6;
	CEdit m_editOther;
	CEdit m_editSum;

	CListCtrl m_listCtrl;  // �м�Ŀ�
	CTreeCtrl m_treeCtrl;  // ���Ľϴ�Ŀ�
	CEdit m_edit;          // �Ҳ�ϴ�Ŀ�

	// ��ť 
	CButton m_buttonStart;
	CButton m_buttonStop;
	CButton m_buttonSave;
	CButton m_buttonRead;
	
	// �豸�б������� �� ���˹�����������
	CComboBox m_comboBox;
//	CComboBox m_comboBoxRule;

// ץ�������ࣨCore)
	SnifferCore m_snifferCore;


// �������ݸ������
public:
	void UpdateGui(const pktCount* npkt, const datapkt* hdrspack);

private:
	void update_listCtrl(const pktCount* npkt, const datapkt* hdrsPack);
	void updateNPacket(const pktCount* npkt);
	void updateTree(int index, const pktCount* npkt, const datapkt* hdrsPack);
	void updateEdit(datapkt* hdrsPack);

public:
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton5();
	afx_msg void OnNMCustomdrawList1(NMHDR *pNMHDR, LRESULT *pResult);
public:
	string getFilterRule();
// 	CMyEdit m_cmyEdit;
	CMyEdit m_CMyEdit;
	afx_msg void OnEnChangeEdit12();
	afx_msg void OnClickedButtonHelp();
};
